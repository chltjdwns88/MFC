
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Figure.h"
#include "2020-1_MFCPaint.h"
#include "ChildView.h"
#include <vector>
#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CChildView

/* khlee: window view 의 생성자 */
CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	drawings.clear();
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_DRAWCIRCLE, &CChildView::OnDrawcircle)
	ON_UPDATE_COMMAND_UI(ID_DRAWCIRCLE, &CChildView::OnUpdateDrawcircle)
	ON_COMMAND(ID_DRAWLINE, &CChildView::OnDrawline)
	ON_UPDATE_COMMAND_UI(ID_DRAWLINE, &CChildView::OnUpdateDrawline)
	ON_COMMAND(ID_DRAWRECT, &CChildView::OnDrawrect)
	ON_UPDATE_COMMAND_UI(ID_DRAWRECT, &CChildView::OnUpdateDrawrect)
	ON_COMMAND(ID_EDITCOPY, &CChildView::OnEditcopy)
	ON_UPDATE_COMMAND_UI(ID_EDITCOPY, &CChildView::OnUpdateEditcopy)
	ON_COMMAND(ID_EDITCUT, &CChildView::OnEditcut)
	ON_UPDATE_COMMAND_UI(ID_EDITCUT, &CChildView::OnUpdateEditcut)
	ON_COMMAND(ID_EDITDEL, &CChildView::OnEditdel)
	ON_UPDATE_COMMAND_UI(ID_EDITDEL, &CChildView::OnUpdateEditdel)
	ON_COMMAND(ID_EDITDELALL, &CChildView::OnEditdelall)
	ON_UPDATE_COMMAND_UI(ID_EDITDELALL, &CChildView::OnUpdateEditdelall)
	ON_COMMAND(ID_EDITGROUP, &CChildView::OnEditgroup)
	ON_UPDATE_COMMAND_UI(ID_EDITGROUP, &CChildView::OnUpdateEditgroup)
	ON_COMMAND(ID_EDITPASTE, &CChildView::OnEditpaste)
	ON_UPDATE_COMMAND_UI(ID_EDITPASTE, &CChildView::OnUpdateEditpaste)
	ON_COMMAND(ID_EDITUNGROUP, &CChildView::OnEditungroup)
	ON_UPDATE_COMMAND_UI(ID_EDITUNGROUP, &CChildView::OnUpdateEditungroup)
	ON_COMMAND(ID_EDITCUT, &CChildView::OnEditMove)
	ON_UPDATE_COMMAND_UI(ID_EDITCUT, &CChildView::OnUpdatedEditMove)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	for (int i = 0; i < choiced.size(); i++) {
		if (choiced[i]) {
			for (int j = 0; j < group.size(); j++) {
				if (i == j) continue;
				if (group[i] != -1 && group[i] == group[j]) {
					choiced[j] = true;
				}
			}
		}
	}
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	/* khlee: 참고용 원 그리기 */

	/* khlee: 저장해둔 도형들을 모두 그려줌. */
	CPen temp_pen1(PS_DASHDOT, 1, RGB(0, 0, 200));
	CPen temp_pen2(PS_DASH, 1, RGB(0, 0, 200));
	for (int i = 0; i < (signed)drawings.size(); i++) {
		if (choiced[i]) { dc.SelectObject(&temp_pen1); dc.SelectStockObject(HOLLOW_BRUSH); }
		drawings[i]->draw(&dc);
		if (choiced[i]) { dc.SelectObject(&temp_pen2); dc.SelectStockObject(BLACK_PEN); }
	}
	
	// khlee: 그리기나 영역선택등 임의로 영역을 잡을경우 
	// pen과 brush를 다르게 설정 (점선과 투명 영역)
	CPen temp_pen(PS_DASH, 1, RGB(0,0,200));
	CPen* old_pen = dc.SelectObject(&temp_pen);


	switch(mode)
	{
	case DRAW_RECT:
		dc.SelectStockObject(HOLLOW_BRUSH);
		if (selecting_region)
			dc.Rectangle(selecting_region->p1.x, selecting_region->p1.y, selecting_region->p2.x, selecting_region->p2.y);
		break;
	case DRAW_CIRC:
		dc.SelectStockObject(HOLLOW_BRUSH);
		if (selecting_region)
			dc.Ellipse(selecting_region->p1.x, selecting_region->p1.y, selecting_region->p2.x, selecting_region->p2.y);
		break;
	case DRAW_LINE:
		dc.SelectStockObject(HOLLOW_BRUSH);
		if (selecting_region) {
			dc.MoveTo(selecting_region->p1.x, selecting_region->p1.y);
			dc.LineTo(selecting_region->p2.x, selecting_region->p2.y);
		}
		break;
	case DO_FIGURE_SELECT:
		dc.SelectStockObject(HOLLOW_BRUSH);
		if (selecting_region)
			dc.Rectangle(selecting_region->p1.x, selecting_region->p1.y, selecting_region->p2.x, selecting_region->p2.y);
		break;
	case NORMAL:
		dc.SelectStockObject(HOLLOW_BRUSH);
		if (selecting_region) {
			dc.Rectangle(selecting_region->p1.x, selecting_region->p1.y, selecting_region->p2.x, selecting_region->p2.y);
		}
		break;
	}
	/* khlee: 현재 마우스 포인터의 좌표값 출력 (좌표계에 익숙해지면 삭제하세요) */
	string msg = "Mouse Pose x : " + to_string(current_point.x) + " y: " + to_string(current_point.y);
	CString cmsg(msg.c_str());
	//dc.TextOut(0, 0, cmsg);
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}



void CChildView::OnDrawcircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	mode = DRAW_CIRC;
}


void CChildView::OnUpdateDrawcircle(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}


void CChildView::OnDrawline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	mode = DRAW_LINE;
}


void CChildView::OnUpdateDrawline(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}


void CChildView::OnDrawrect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	/* khlee: 모드를 사각형 그리기 모드로 변경 */
	mode = DRAW_RECT;
}


void CChildView::OnUpdateDrawrect(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}


void CChildView::OnEditcopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//if (mode == NORMAL) {
	//	mode = DO_FIGURE_SELECT;
	//}
	//else {
		if (clip_board.size() > 0) { clip_board.clear(); clip_board_separate.clear(); }
		for (int i = 0; i < (signed)drawings.size(); i++) {
			if (choiced[i]) {
				if (separate[i] == 4) {
					clip_board.push_back(new FigureRectangle(drawings[i]->p1, drawings[i]->p2));
					clip_board_separate.push_back(separate[i]);
				}
				else if (separate[i] == 2) {
					clip_board.push_back(new FigureLine(drawings[i]->p1, drawings[i]->p2));
					clip_board_separate.push_back(separate[i]);
				}
				else {
					clip_board.push_back(new FigureCircle(drawings[i]->p1, drawings[i]->p2));
					clip_board_separate.push_back(separate[i]);
				}
			}
			choiced[i] = false;
		}
		mode = NORMAL;
		Invalidate(true);
	//}
}


void CChildView::OnUpdateEditcopy(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	/* Khlee: 버튼 활성화/비활성화 */
	if (drawings.size() > 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void CChildView::OnEditcut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//if (mode == NORMAL) {
	//	mode = DO_FIGURE_SELECT;
	//}
	//else {
		if (clip_board.size() > 0) { clip_board.clear(); clip_board_separate.clear(); }
		for (int i = 0; i < (signed)drawings.size(); i++) {
			if (choiced[i]) {
				if (separate[i] == 4) {
					clip_board.push_back(new FigureRectangle(drawings[i]->p1, drawings[i]->p2));
					clip_board_separate.push_back(separate[i]);
				}
				else if (separate[i] == 2) {
					clip_board.push_back(new FigureLine(drawings[i]->p1, drawings[i]->p2));
					clip_board_separate.push_back(separate[i]);
				}
				else {
					clip_board.push_back(new FigureCircle(drawings[i]->p1, drawings[i]->p2));
					clip_board_separate.push_back(separate[i]);
				}
				delete drawings[i];
				drawings[i] = NULL;
				drawings.erase(drawings.begin() + i);
				choiced.erase(choiced.begin() + i);
				group.erase(group.begin() + i);
				separate.erase(separate.begin() + i);
				i--;
			}
		}
		mode = NORMAL;
		Invalidate(true);
	//}
}


void CChildView::OnUpdateEditcut(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	/* Khlee: 버튼 활성화/비활성화 */
	if (drawings.size() > 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void CChildView::OnEditdel()
{
	//if (mode == NORMAL) {
		//mode = DO_FIGURE_SELECT;
	//}
	//else {
		for (int i = 0; i < (signed)drawings.size(); i++) {
			if (choiced[i]) {
				delete drawings[i];
				drawings[i] = NULL;
				drawings.erase(drawings.begin() + i);
				choiced.erase(choiced.begin() + i);
				group.erase(group.begin() + i);
				separate.erase(separate.begin() + i);
				i--;
			}
		}
		mode = NORMAL;
		Invalidate(true);
	//}
}

void CChildView::OnUpdateEditdel(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	/* Khlee: 버튼 활성화/비활성화 */
	if (drawings.size() > 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void CChildView::OnEditdelall()
{
	drawings.clear();
	choiced.clear();
	group.clear();
	Invalidate(true);
}


void CChildView::OnUpdateEditdelall(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (drawings.size() > 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);

}


void CChildView::OnEditgroup()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (mode != SELECTING_GROUP)
		mode = SELECTING_GROUP;
	else {

		for (int i = 0; i < choiced.size(); i++) {
			if (choiced[i]) {
				group[i] = groupnum;
				choiced[i] = false;
			}
		}
		groupnum++;
		total_groupnum++;
		mode = NORMAL;
		Invalidate(true);
	}
}


void CChildView::OnUpdateEditgroup(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (drawings.size() > 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void CChildView::OnEditpaste()
{
	if (clip_board.size() == 0) return;
	for (int i = 0; i < clip_board.size(); i++) {
		(clip_board[i]->p1.x) += 10; (clip_board[i]->p1.y) += 10;
		(clip_board[i]->p2.x) += 10; (clip_board[i]->p2.y) += 10;
		drawings.push_back(clip_board[i]);
		choiced.push_back(false);
		group.push_back(-1);
		separate.push_back(clip_board_separate[i]);
	}
	clip_board.clear();
	clip_board_separate.clear();
	Invalidate(true);
}


void CChildView::OnUpdateEditpaste(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	/* Khlee: 버튼 활성화/비활성화 */
	if (clip_board.size() > 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void CChildView::OnEditungroup()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	for (int i = 0; i < choiced.size(); i++) {
		if (choiced[i]) {
			for (int j = 0; j < group.size(); j++) {
				if (i == j) continue;
				if (group[i] != -1 && group[i] == group[j]) {
					group[j] = -1;
					choiced[j] = false;
				}
			}
			choiced[i] = false;
			group[i] = -1;
			total_groupnum--;
		}
	}

	Invalidate(true);
}


void CChildView::OnUpdateEditungroup(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (total_groupnum > 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CChildView::OnEditMove() 
{

}

void CChildView::OnUpdatedEditMove(CCmdUI* pCmdUI) 
{

}

/* khlee: 마우스 왼쪽 버튼 누를 때 */
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/* khlee: 마우스 왼쪽 버튼이 눌렸을 때, cursor mode에 따른 여러가지 처리 */
	/* 두번째 parameter인 point에 클릭 당시의 좌표가 넘어옴 */
	/* CChildView 멤버 변수인 starting_point에 현재 point 좌표를 저장: 
	마우스 드래그를 지원하기 위하여 */
	starting_point = point;
	
	if (mode == DRAW_RECT)
	{
		/* khlee: 선택영역을 표현하기 위한 임시 사각형 모양 생성 */
		selecting_region = new FigureRectangle (starting_point, point);
		/* khlee: 아래 코드를 실행하면 OnPaint()가 바로 실행됨 */
		Invalidate(true);
	}
	else if (mode == DRAW_CIRC)
	{
		/* khlee: 선택영역을 표현하기 위한 임시 사각형 모양 생성 */
		selecting_region = new FigureCircle(starting_point, point);
		/* khlee: 아래 코드를 실행하면 OnPaint()가 바로 실행됨 */
		Invalidate(true);
	}
	else if (mode == DRAW_LINE)
	{
		selecting_region = new FigureLine(starting_point, point);
		Invalidate(true);
	}
	else if (mode == DO_FIGURE_SELECT) 
	{
		selecting_region = new FigureRectangle(starting_point, point);
		Invalidate(true);
	}
	else if (mode == NORMAL) {
		selecting_region = new FigureRectangle(starting_point, point);
		normal_choice = 0;
		normal = true;
		Invalidate(true);
	}
	else if (mode == WAITING && normal_choice != 0) {
		resize = false;
		mode = FIGURE_MOVE;
	}
	else if (mode == WAITING && normal_choice == 0) {
		normal = false;
		for (int i = 0; i < choiced.size(); i++) {
			choiced[i] = false;
		}
		mode = NORMAL;
		return;
	}
	else if (mode == SELECTING_GROUP) {
		selecting_region = new FigureRectangle(starting_point, point);
		Invalidate(true);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

/* khlee: 마우스 왼쪽 버튼 뗄 때 */
void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (mode)
	{
	case DRAW_RECT:
		/* 현재까지 그려진 모양 그대로 도형 (사각형) 을 생성해서, 내부 벡터에 저장해줌*/
		drawings.push_back(new FigureRectangle(selecting_region->p1, selecting_region->p2));
		choiced.push_back(false);
		separate.push_back(4);
		group.push_back(-1);
		/* khlee: 각종 설정을 리셋*/
		delete selecting_region;	/* khlee: 임시 도형 삭제*/
		mode = NORMAL;				/* 마우스 선택 모드를 일반 모드로 변경*/
		Invalidate(true);			/* OnPaint() 호출로 화면 업데이트*/
		break;

	case DRAW_CIRC:
		/* 현재까지 그려진 모양 그대로 도형 (사각형) 을 생성해서, 내부 벡터에 저장해줌*/
		drawings.push_back(new FigureCircle(selecting_region->p1, selecting_region->p2));
		choiced.push_back(false);
		separate.push_back(0);
		group.push_back(-1);
		/* khlee: 각종 설정을 리셋*/
		delete selecting_region;	/* khlee: 임시 도형 삭제*/
		mode = NORMAL;				/* 마우스 선택 모드를 일반 모드로 변경*/
		Invalidate(true);			/* OnPaint() 호출로 화면 업데이트*/
		break;

	case DRAW_LINE:
		/* 현재까지 그려진 모양 그대로 도형 (사각형) 을 생성해서, 내부 벡터에 저장해줌*/
		drawings.push_back(new FigureLine(selecting_region->p1, selecting_region->p2));
		choiced.push_back(false);
		separate.push_back(2);
		group.push_back(-1);
		/* khlee: 각종 설정을 리셋*/
		delete selecting_region;	/* khlee: 임시 도형 삭제*/
		mode = NORMAL;				/* 마우스 선택 모드를 일반 모드로 변경*/
		Invalidate(true);			/* OnPaint() 호출로 화면 업데이트*/
		break;

	case DO_FIGURE_SELECT:
		if (abs(selecting_region->p2.x - selecting_region->p1.x) < 1 && abs(selecting_region->p2.y - selecting_region->p1.y) < 1) {
			for (int i = (signed)drawings.size() - 1 ; i >= 0 ; i--) {
				if (drawings[i]->p1.x <= selecting_region->p1.x && drawings[i]->p2.x >= selecting_region->p2.x) {
					if (drawings[i]->p1.y <= selecting_region->p1.y && drawings[i]->p2.y >= selecting_region->p2.y) {
						choiced[i] = true;
						break;
					}
				}
			}
		}
		else {
			//DRAG
			selected_region = new FigureRectangle(selecting_region->p1, selecting_region->p2);
			for (int i = 0; i < (signed)drawings.size(); i++) {
				if (selected_region->p1.x <= drawings[i]->p1.x && selected_region->p2.x >= drawings[i]->p2.x) {
					if (selected_region->p1.y <= drawings[i]->p1.y && selected_region->p2.y >= drawings[i]->p2.y) {
						choiced[i] = true;
					}
				}
			}
		}
		mode = FIGURE_SELECTED;
		Invalidate(true);
		break;

	case SELECTING_GROUP:
		if (abs(selecting_region->p2.x - selecting_region->p1.x) < 1 && abs(selecting_region->p2.y - selecting_region->p1.y) < 1) {
			for (int i = (signed)drawings.size() - 1; i >= 0; i--) {
				if (drawings[i]->p1.x <= selecting_region->p1.x && drawings[i]->p2.x >= selecting_region->p2.x) {
					if (drawings[i]->p1.y <= selecting_region->p1.y && drawings[i]->p2.y >= selecting_region->p2.y) {
						choiced[i] = true;
						break;
					}
				}
			}
		}
		else {
			//DRAG
			selected_region = new FigureRectangle(selecting_region->p1, selecting_region->p2);
			for (int i = 0; i < (signed)drawings.size(); i++) {
				if (selected_region->p1.x <= drawings[i]->p1.x && selected_region->p2.x >= drawings[i]->p2.x) {
					if (selected_region->p1.y <= drawings[i]->p1.y && selected_region->p2.y >= drawings[i]->p2.y) {
						choiced[i] = true;
					}
				}
			}
		}
		Invalidate(true);
		break;

	case NORMAL:
		if (!normal) {
			Invalidate(true); break;
		}
		if (abs(selecting_region->p2.x - selecting_region->p1.x) < 1 && abs(selecting_region->p2.y - selecting_region->p1.y) < 1) {
			for (int i = (signed)drawings.size() - 1; i >= 0; i--) {
				if (drawings[i]->p1.x <= selecting_region->p1.x && drawings[i]->p2.x >= selecting_region->p2.x) {
					if (drawings[i]->p1.y <= selecting_region->p1.y && drawings[i]->p2.y >= selecting_region->p2.y) {
						choiced[i] = true;
						normal_choice++;
						break;
					}
				}
			}
		}
		else {
			//DRAG
			selected_region = new FigureRectangle(selecting_region->p1, selecting_region->p2);
			for (int i = 0; i < (signed)drawings.size(); i++) {
				if (selected_region->p1.x <= drawings[i]->p1.x && selected_region->p2.x >= drawings[i]->p2.x) {
					if (selected_region->p1.y <= drawings[i]->p1.y && selected_region->p2.y >= drawings[i]->p2.y) {
						choiced[i] = true;
						normal_choice++;
					}
				}
			}
			delete selected_region;
			selected_region = NULL;
		}
		selecting_region->p1 = selecting_region->p2;
		mode = WAITING;
		Invalidate(true);
		break;

	case FIGURE_MOVE:
		normal_choice = 0;
		for (int i = 0; i < choiced.size(); i++) {
			choiced[i] = false;
		}
		Invalidate(true);
		mode = NORMAL;
		break;
	}
	CWnd::OnLButtonUp(nFlags, point);
}

/* khlee: 마우스 이동 중 */
void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDC* dc = GetDC();

	/* Khlee: 마우스 왼쪽 버튼이 눌린 상태로 움직이고 있을 때,
	point는 현재의 마우스 좌표 (이동된) 를 넘겨받음 */
	current_point = point;
	
	if (nFlags & MK_LBUTTON)
	{
		/* khlee: 사각형을 그리기 위한 임시 사각형이 이미 생성되어 있다면,  */
		if (mode == DRAW_RECT && selecting_region)
		{
			selecting_region->p1 = starting_point;
			selecting_region->p2 = point;
			Invalidate(true);
		}
		else if (mode == DRAW_CIRC && selecting_region) {
			selecting_region->p1 = starting_point;
			selecting_region->p2 = point;
			Invalidate(true);
		}
		else if (mode == DRAW_LINE && selecting_region) {
			selecting_region->p1 = starting_point;
			selecting_region->p2 = point;
			Invalidate(true);
		}
		else if (mode == DO_FIGURE_SELECT && selecting_region) {
			selecting_region->p1 = starting_point;
			selecting_region->p2 = point;
			Invalidate(true);
		}
		else if (mode == NORMAL && selecting_region && normal) {
			selecting_region->p1 = starting_point;
			selecting_region->p2 = point;
			Invalidate(true);
		}

		else if (mode == SELECTING_GROUP && selecting_region) {
			selecting_region->p1 = starting_point;
			selecting_region->p2 = point;
			Invalidate(true);
		}
		
		else if (mode == FIGURE_MOVE) {
			for (int i = 0; i < drawings.size(); i++) {
				if (choiced[i] && !resize) {
					if (abs(drawings[i]->p2.x - point.x) < 10 && abs(drawings[i]->p2.y - point.y) < 10) { resize = true; continue; }
					if (starting_point.x < point.x) {
						int move_x = point.x - starting_point.x;
						drawings[i]->p1.x += move_x;
						drawings[i]->p2.x += move_x;
					}
					else if (starting_point.x > point.x) {
						int move_x = starting_point.x - point.x;
						drawings[i]->p1.x -= move_x;
						drawings[i]->p2.x -= move_x;
					}
					if (starting_point.y < point.y) {
						int move_y = point.y - starting_point.y;
						drawings[i]->p1.y += move_y;
						drawings[i]->p2.y += move_y;
					}
					else if (starting_point.y > point.y) {
						int move_y = starting_point.y - point.y;
						drawings[i]->p1.y -= move_y;
						drawings[i]->p2.y -= move_y;
					}
				}

				if (choiced[i] && resize) {
					if (starting_point.x < point.x) {
						int move_x = point.x - starting_point.x;
						drawings[i]->p2.x += move_x;
					}
					else if (starting_point.x > point.x) {
						int move_x = starting_point.x - point.x;
						drawings[i]->p2.x -= move_x;
					}
					if (starting_point.y < point.y) {
						int move_y = point.y - starting_point.y;
						drawings[i]->p2.y += move_y;
					}
					else if (starting_point.y > point.y) {
						int move_y = starting_point.y - point.y;
						drawings[i]->p2.y -= move_y;
					}
				}
			}
			starting_point = point;
			Invalidate(true);
		}
		
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

/* khlee: 커서 모양 설정 */
BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/* khlee: 모드에 따른 커서 모양 변경*/
	/* SetCursor(LoadCursor(0, IDC_CROSS)); 의 마지막 매개변수 조절 (IDC_XXXX)
	 * IDC_HAND 손모양
	 * IDC_SIZENS, IDC_SIZENS, IDC_SIZEWE, IDC_SIZENWSW, IDC_SIZENESW 크기조절화살표
	 * IDC_ARROW 화살표 
	 * IDC_CROSS 십자가 
	 */

	switch (mode)
	{
	case NORMAL:
		SetCursor(LoadCursor(0, IDC_ARROW));
		break;
	case DRAW_RECT:
		SetCursor(LoadCursor(0, IDC_CROSS));
		break;
	case DRAW_CIRC:
		SetCursor(LoadCursor(0, IDC_CROSS));
		break;
	case DRAW_LINE:
		SetCursor(LoadCursor(0, IDC_CROSS));
		break;
	case DO_FIGURE_SELECT:
		SetCursor(LoadCursor(0, IDC_HAND));
		break;
	case FIGURE_SELECTED:
		SetCursor(LoadCursor(0, IDC_ARROW));
		break;
	case FIGURE_MOVE:
		SetCursor(LoadCursor(0, IDC_HAND));
		break;
	case WAITING:
		SetCursor(LoadCursor(0, IDC_HAND));
		break;
	case SELECTING_GROUP:
		SetCursor(LoadCursor(0, IDC_HAND));
		break;
	default:
		SetCursor(LoadCursor(0, IDC_ARROW));
		break;
	}
	return true;

	//return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
