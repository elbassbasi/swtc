/*
 * TDragging.cpp
 *
 *  Created on: 3 oct. 2020
 *      Author: azeddine
 */

#include "TDragging.h"

TDragging::TDragging() {
	this->op = NONE;
}

TDragging::~TDragging() {
}

void TDragging::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Dragging", new TDragging());
}

void TDragging::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->rect.width = 30;
	this->rect.height = 30;
	this->op = NONE;
}

bool TDragging::OnPaint(WPaintEvent &e) {
	e->SetBackground(W_COLOR_GREEN);
	e->FillRectangle(rect);
	e->SetForeground(W_COLOR_RED);
	e->SetLineWidth(3);
	e->DrawRectangle(rect);
	return true;
}

bool TDragging::OnDragDetect(WMouseEvent &e) {
	if (rect.Contains(e.x, e.y)) {
		this->op = MOVE;
		this->pt.x = e.x - rect.x;
		this->pt.y = e.y - rect.y;
	}
	return false;
}

bool TDragging::OnMouseMove(WMouseEvent &e) {
	if(this->op == MOVE){
		//WRect last(this->rect);
		this->rect.x = e.x - this->pt.x;
		this->rect.y = e.y - this->pt.y;
		/*WRect result;
		last.Union(result,rect);
		this->Redraw(result,false);*/
		this->Redraw();
	}
	return false;
}

bool TDragging::OnMouseDown(WMouseEvent &e) {
	return false;
}

bool TDragging::OnMouseUp(WMouseEvent &e) {
	this->op = NONE;
	return false;
}
