/*
 * TTreeDemo.h
 *
 *  Created on: 18 mars 2021
 *      Author: azeddine
 */

#ifndef TESTS_CONTROLS_TREEVIEW_TTREEDEMO_H_
#define TESTS_CONTROLS_TREEVIEW_TTREEDEMO_H_
#include "TTreeViews.h"
class PersonDemo {
public:
	PersonDemo(int i, int j);
public:
	char name[30];
	int i;
	int j;
	int progress;
};
class TTreeDemo: public WTreeView, public ITreeItem {
public:
	static void Registre(WTreeItem &parent);
	TTreeDemo();
	~TTreeDemo();
	void CreateControl(WComposite *parent);
	WControl* GetControl(WComposite *parent);
};

#endif /* TESTS_CONTROLS_TREEVIEW_TTREEDEMO_H_ */
