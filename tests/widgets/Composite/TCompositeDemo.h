/*
 * TCompositeDemo.h
 *
 *  Created on: 3 août 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_COMPOSITE_TCOMPOSITEDEMO_H_
#define TESTS_WIDGETS_COMPOSITE_TCOMPOSITEDEMO_H_
#include "TComposites.h"

class TCompositeDemo: public CompositeTreeItem {
public:
	WButton button;
	WGridLayout layout;
	WMenu menu;
	static WMenuItems menuitems[];
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
protected:
	bool OnPaint(WPaintEvent &e);
	bool OnNotifySelection(WEvent& e);
	bool OnMouseUp(WMouseEvent &e);
};

#endif /* TESTS_WIDGETS_COMPOSITE_TCOMPOSITEDEMO_H_ */
