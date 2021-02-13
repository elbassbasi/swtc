/*
 * TScrolledComposite.h
 *
 *  Created on: 6 sept. 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_COMPOSITE_TSCROLLEDCOMPOSITE_H_
#define TESTS_WIDGETS_COMPOSITE_TSCROLLEDCOMPOSITE_H_
#include "TCompositeDemo.h"
#define NUM_OF_BUTTON 10
class TScrolledComposite : public CompositeTreeItem{
private:
	WControl *content;
	int minHeight;
	int minWidth;
	bool expandHorizontal :1;
	bool expandVertical :1;
	bool alwaysShowScroll :1;
	bool showFocusedControl :1;
	bool showNextFocusedControl :1;
public:
	WComposite composite;
	WButton btn[NUM_OF_BUTTON];
	TScrolledComposite();
	virtual ~TScrolledComposite();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	bool OnLayout(WEvent &e);
	bool OnHScroll(WScrollBarEvent &e);
	bool OnVScroll(WScrollBarEvent &e);
	bool NeedHScroll(WScrollBar& hBar,WScrollBar& vBar,WRect& contentRect, bool vVisible);
	bool NeedVScroll(WScrollBar& hBar,WScrollBar& vBar,WRect& contentRect, bool hVisible);
};

#endif /* TESTS_WIDGETS_COMPOSITE_TSCROLLEDCOMPOSITE_H_ */
