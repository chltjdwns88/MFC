#pragma once

#include "pch.h"
class Figure {
public:
	Figure() : p1(0, 0), p2(0, 0) {};
	Figure(CPoint& _p1, CPoint& _p2) : p1(_p1), p2(_p2) {}
	virtual void draw(CPaintDC* dc);
	CPoint p1, p2; /* khlee: 좌상단 및 우하단 꼭지점 */
};

class FigureRectangle : public Figure
{
public:
	FigureRectangle() : Figure() {}
	FigureRectangle(CPoint& _p1, CPoint& _p2) : Figure(_p1, _p2) {}
public:
	virtual void draw(CPaintDC* dc);
};

class FigureCircle : public Figure
{
public:
	FigureCircle() : Figure() {}
	FigureCircle(CPoint& _p1, CPoint& _p2) : Figure(_p1, _p2) {}
public:
	virtual void draw(CPaintDC* dc);
};

class FigureLine : public Figure
{
public:
	FigureLine() : Figure() {}
	FigureLine(CPoint& _p1, CPoint& _p2) : Figure(_p1, _p2) {}
public:
	virtual void draw(CPaintDC* dc);
};