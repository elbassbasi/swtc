/*
 * TDragging.h
 *
 *  Created on: 3 oct. 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_CONTROL_TDRAGGING_H_
#define TESTS_WIDGETS_CONTROL_TDRAGGING_H_
#include "TControlss.h"
class TDragging: public CanvasTreeItem {
public:
	TDragging();
	~TDragging();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
protected:
	bool OnPaint(WPaintEvent &e);
	bool OnDragDetect(WMouseEvent &e);
	bool OnMouseMove(WMouseEvent &e);
	bool OnMouseDown(WMouseEvent &e);
	bool OnMouseUp(WMouseEvent &e);
public:
	enum {
		NONE = 0, MOVE, RESIZE_LEFT, RESIZE_RIGHT, RESIZE_TOP, RESIZE_BOTTOM
	} op;
	WRect rect;
	WPoint pt;
};

#endif /* TESTS_WIDGETS_CONTROL_TDRAGGING_H_ */
