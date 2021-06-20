/*
 * TTreeCustom.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_TREEVIEW_TTREEVIRTUAL_H_
#define TESTS_CONTROLS_TREEVIEW_TTREEVIRTUAL_H_
#include "TTreeViews.h"
class Person {
public:
	Person(int i, int j);
public:
	char name[30];
	int i;
	int j;
	int progress;
};
class TTreeVirtualEdit: public WTextEdit {
public:
	WTreeItem item;
protected:
	bool OnFocusOut(WEvent &e);
	bool OnTraverse(WKeyEvent &e);
	bool OnDefaultSelection(WTextEditEvent &e);
	void SetTextAndHide();
};
class TTreeVirtual: public WTreeView, public ITreeItem {
public:
	static void Registre(WTreeItem &parent);
	TTreeVirtual();
	~TTreeVirtual();
	WControl* GetControl(WComposite *parent);
	void CreateControl(WComposite *parent);
	bool OnItemMeasure(WTreeEvent &e);
	bool OnItemErase(WTreeEvent &e);
	bool OnItemPaint(WTreeEvent &e);
	bool OnItemGetText(WTreeEvent &e);
	bool OnItemDefaultSelection(WTreeEvent &e);
	bool OnItemDispose(WTreeEvent &e);
	WFont* fontBold;
	TTreeVirtualEdit text;
};

#endif /* TESTS_CONTROLS_TREEVIEW_TTREEVIRTUAL_H_ */
