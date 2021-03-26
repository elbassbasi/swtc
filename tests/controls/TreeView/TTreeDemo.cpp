/*
 * TTreeDemo.cpp
 *
 *  Created on: 18 mars 2021
 *      Author: azeddine
 */

#include "TTreeDemo.h"
void TTreeDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TTreeDemo());
}
PersonDemo::PersonDemo(int i, int j) {
	this->i = i;
	this->j = j;
	if (j < 0) {
		sprintf(this->name, "person %d", i);
		this->progress = i % 5;
	} else {
		sprintf(this->name, "_person %d,%d", i, j);
		this->progress = j;
	}
}
TTreeDemo::TTreeDemo() {
}
TTreeDemo::~TTreeDemo() {
}
void TTreeDemo::CreateControl(WComposite *parent) {
	WTreeItem item1, root, item2;
	WColumnItem column;
	char txt[50];
	this->Create(parent, W_HSCROLL | W_VSCROLL | W_FULL_SELECTION | W_CHECK);
	this->SetImageList(MShell::GetImageList32_(this));
	this->GetColumn(0, column).SetText("id");
	this->AppendColumn(column, "progress"); //.SetResizable(false);
	this->AppendColumn(column, "int");
	column.SetAlignment(W_CENTER);
	this->SetHeaderVisible(true);
	this->SetLinesVisible(true);
	int imagelistcount = WMAX(1, this->GetImageList()->GetCount());
	this->GetRootItem(root);
	for (int i = 0; i < 50; i++) {
		sprintf(txt, " test %d", i);
		if (root.AppendItem(item1, txt).IsOk()) {
			item1.SetImage(i % imagelistcount);
			item1.SetData(new PersonDemo(i, -1));
			for (int j = 0; j < 5; j++) {
				sprintf(txt, "test %d,%d", i, j);
				item1.AppendItem(item2);
				if (item2.IsOk()) {
					item2.SetImage(j);
					item2.SetData(new PersonDemo(i, j));
				}
			}
		}
	}
}
WControl* TTreeDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}

