/*
 * TTreeCustom.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_TREEVIEW_TTREECUSTOM_H_
#define TESTS_CONTROLS_TREEVIEW_TTREECUSTOM_H_
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
class TTreeCustomEdit: public WTextEdit {
public:
	WTreeItem item;
protected:
	bool OnFocusOut(WEvent &e);
	bool OnTraverse(WKeyEvent &e);
};
class TTreeCustom: public WTreeView, public ITreeItem {
public:
	static void Registre(WTreeItem &parent);
	TTreeCustom();
	~TTreeCustom();
	WControl* GetControl(WComposite *parent);
	void CreateControl(WComposite *parent);
	bool OnItemMeasure(WTreeEvent &e);
	bool OnItemErase(WTreeEvent &e);
	bool OnItemPaint(WTreeEvent &e);
	bool OnItemGetValue(WTreeEvent &e);
	bool OnItemGetAttr(WTreeEvent &e);
	bool OnItemDefaultSelection(WTreeEvent &e);
	bool OnItemDispose(WTreeEvent &e);
	WFont fontBold;
	TTreeCustomEdit text;
};

#endif /* TESTS_CONTROLS_TREEVIEW_TTREECUSTOM_H_ */
