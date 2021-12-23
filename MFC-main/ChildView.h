
// ChildView.h: CChildView 클래스의 인터페이스
//
#pragma once

#include "Figure.h"
#include <vector>

using namespace std;

/* khlee: 각 메시지 핸들러 내부에서 현재 어떤 메뉴/기능이 선택되어 있는지 확인용 */
enum mouseMode
{
	NORMAL = 1,
	DRAW_RECT,			/* 사각형 그리기 클릭된 상태 */
	DRAW_CIRC,
	DRAW_LINE,
	FIGURE_SELECTED,	/* 도형/그룹이 선택완료된 상태 */
	FIGURE_MOVE,		/* 도형/그룹을 이동중인 상태 */
	DO_FIGURE_SELECT,	/* 도형/그룹을 선택 중 (드래그 중) 인 상태 */
	WAITING,
	SELECTING_GROUP
	/* khlee: fill up here with your own codes */
};

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	enum mouseMode mode;
	CPoint starting_point;
	CPoint current_point;
	vector<Figure*> drawings;
	vector<bool> choiced;
	Figure* selecting_region;
	Figure* selected_region = NULL;
	vector<Figure*> clip_board;
	vector<int> clip_board_separate;
	vector<int> separate;
	vector<int> group;
	int groupnum = 0;
	int total_groupnum = 0;
	int normal_choice;
	bool normal;
	bool resize;
// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawcircle();
	afx_msg void OnUpdateDrawcircle(CCmdUI* pCmdUI);
	afx_msg void OnDrawline();
	afx_msg void OnUpdateDrawline(CCmdUI* pCmdUI);
	afx_msg void OnDrawrect();
	afx_msg void OnUpdateDrawrect(CCmdUI* pCmdUI);
	afx_msg void OnEditcopy();
	afx_msg void OnUpdateEditcopy(CCmdUI* pCmdUI);
	afx_msg void OnEditcut();
	afx_msg void OnUpdateEditcut(CCmdUI* pCmdUI);
	afx_msg void OnEditdel();
	afx_msg void OnUpdateEditdel(CCmdUI* pCmdUI);
	afx_msg void OnEditdelall();
	afx_msg void OnUpdateEditdelall(CCmdUI* pCmdUI);
	afx_msg void OnEditgroup();
	afx_msg void OnUpdateEditgroup(CCmdUI* pCmdUI);
	afx_msg void OnEditpaste();
	afx_msg void OnUpdateEditpaste(CCmdUI* pCmdUI);
	afx_msg void OnEditungroup();
	afx_msg void OnUpdateEditungroup(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEditMove();
	afx_msg void OnUpdatedEditMove(CCmdUI* pCmdUI);
};

