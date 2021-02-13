/*
 * TToolbarComplex.h
 *
 *  Created on: 22 sept. 2020
 *      Author: azeddine
 */

#ifndef TESTS_CONTROLS_TOOLBAR_TTOOLBARCOMPLEX_H_
#define TESTS_CONTROLS_TOOLBAR_TTOOLBARCOMPLEX_H_
#include "TToolBars.h"
enum {
	ACTION_BUTTON1=1,
	ACTION_BUTTON2,
	ACTION_CHECK1,
	ACTION_CHECK2,
	ACTION_RADIO1,
	ACTION_RADIO2,

};
class TToolbarComplex: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TToolbarComplex();
	~TToolbarComplex();
	void CreateControl(WComposite *parent);
	WGridLayout layout;
	WToolBar toolBar;
	WMenu menu;
public:
	void CreateToolbar();
	void CreateToolItem(WToolItem &item, int type, const char *text,
			const char *toolTipText);
protected:
	bool OnNotify(WEvent &e);
};

#endif /* TESTS_CONTROLS_TOOLBAR_TTOOLBARCOMPLEX_H_ */
