/*
 * TControlMouse.h
 *
 *  Created on: 20 sept. 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_CONTROL_TCONTROLMOUSE_H_
#define TESTS_WIDGETS_CONTROL_TCONTROLMOUSE_H_
#include "TControlss.h"
class TControlMouse: public CanvasTreeItem {
public:
	char text[0x100];
	WTextEdit txt;
	TControlMouse();
	~TControlMouse();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
protected:
	bool OnMouse(WMouseEvent &e,const char* text);
protected:
	bool OnPaint(WPaintEvent &e);
	bool OnResize(WEvent &e);
	bool OnMove(WEvent &e);
	bool OnDragDetect(WMouseEvent &e);
	bool OnFocusIn(WEvent &e);
	bool OnFocusOut(WEvent &e);
	bool OnHelp(WEvent &e);
	bool OnKeyUp(WKeyEvent &e);
	bool OnKeyDown(WKeyEvent &e);
	bool OnTraverse(WKeyEvent &e);
	bool OnMenuDetect(WMenuDetectEvent &e);
	bool OnMouseDown(WMouseEvent &e);
	bool OnMouseUp(WMouseEvent &e);
	bool OnMouseDoubleClick(WMouseEvent &e);
	bool OnMouseEnter(WMouseEvent &e);
	bool OnMouseExit(WMouseEvent &e);
	bool OnMouseHover(WMouseEvent &e);
	bool OnMouseMove(WMouseEvent &e);
	bool OnMouseWheel(WMouseEvent &e);
	bool OnGesture(WGestureEvent &e);
	bool OnTouch(WTouchEvent &e);
public:
	bool OnLayout(WEvent &e);
};

#endif /* TESTS_WIDGETS_CONTROL_TCONTROLMOUSE_H_ */
