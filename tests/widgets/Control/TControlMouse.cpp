/*
 * TControlMouse.cpp
 *
 *  Created on: 20 sept. 2020
 *      Author: azeddine
 */

#include "TControlMouse.h"

TControlMouse::TControlMouse() {
}

TControlMouse::~TControlMouse() {
}

void TControlMouse::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Mouse", new TControlMouse());
}

void TControlMouse::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	txt.Create(this, W_MULTI | W_HSCROLL | W_VSCROLL);
}
bool TControlMouse::OnMouse(WMouseEvent &e, const char *text) {
	e.ToChars(this->text, sizeof(this->text), text);
	Redraw();
	return false;
}
bool TControlMouse::OnPaint(WPaintEvent &e) {
	WRect r;
	GetClientArea(r);
	e->SetBackground(W_COLOR_BLACK);
	e->FillRectangle(r);
	e->SetForeground(W_COLOR_WHITE);
	e->DrawTextTransparent(text, 0, 0);
	return true;
}

bool TControlMouse::OnResize(WEvent &e) {
	sprintf(text, "Resize");
	Redraw();
	return false;
}

bool TControlMouse::OnMove(WEvent &e) {
	sprintf(text, "Move");
	Redraw();
	return false;
}

bool TControlMouse::OnFocusIn(WEvent &e) {
	sprintf(text, "FocusIn");
	Redraw();
	return false;
}

bool TControlMouse::OnFocusOut(WEvent &e) {
	sprintf(text, "FocusOut");
	Redraw();
	return false;
}

bool TControlMouse::OnHelp(WEvent &e) {
	sprintf(text, "Help");
	Redraw();
	return false;
}

bool TControlMouse::OnKeyUp(WKeyEvent &e) {
	e.ToChars(this->text, sizeof(this->text), "KeyUp");
	Redraw();
	return false;
}

bool TControlMouse::OnKeyDown(WKeyEvent &e) {
	e.ToChars(this->text, sizeof(this->text), "KeyDown");
	Redraw();
	return false;
}

bool TControlMouse::OnTraverse(WKeyEvent &e) {
	e.ToChars(this->text, sizeof(this->text), "Traverse");
	Redraw();
	return false;
}

bool TControlMouse::OnMenuDetect(WMenuDetectEvent &e) {
	sprintf(text, "MenuDetect");
	Redraw();
	return false;
}

bool TControlMouse::OnMouseDown(WMouseEvent &e) {
	this->ForceFocus();
	return OnMouse(e, "MouseDown");
}

bool TControlMouse::OnMouseUp(WMouseEvent &e) {
	return OnMouse(e, "MouseUp");
}

bool TControlMouse::OnMouseDoubleClick(WMouseEvent &e) {
	return OnMouse(e, "MouseDoubleClick");
}

bool TControlMouse::OnMouseEnter(WMouseEvent &e) {
	//return OnMouse(e, "MouseEnter");
	return false;
}

bool TControlMouse::OnMouseExit(WMouseEvent &e) {
	//return OnMouse(e, "MouseExit");
	return false;
}

bool TControlMouse::OnMouseHover(WMouseEvent &e) {
	return OnMouse(e, "MouseHover");
}
bool TControlMouse::OnDragDetect(WMouseEvent &e) {
	return OnMouse(e, "DragDetect");
}
bool TControlMouse::OnMouseMove(WMouseEvent &e) {
	//return OnMouse(e, "MouseMove");
	return false;
}

bool TControlMouse::OnMouseWheel(WMouseEvent &e) {
	return OnMouse(e, "MouseWheel");
}

bool TControlMouse::OnGesture(WGestureEvent &e) {
	sprintf(text, "Gesture");
	Redraw();
	return false;
}

bool TControlMouse::OnTouch(WTouchEvent &e) {
	sprintf(text, "Touch");
	Redraw();
	return false;
}

bool TControlMouse::OnLayout(WEvent &e) {
	WRect bounds;
	GetClientArea(bounds);
	txt.SetBounds(0, bounds.height / 2, bounds.width, bounds.height / 2);
	return true;
}
