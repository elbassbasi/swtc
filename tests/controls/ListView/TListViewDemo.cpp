/*
 * TTableDemo.cpp
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TListViewDemo.h"
Person::Person(int i) {
	this->i = i;
	this->j = 0;
	this->progress = i;
}
void TListViewDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TListViewDemo());
}

void TListViewDemo::CreateControl(WComposite *parent) {
	WListItem item;
	WColumnItem column;
	char txt[50];
	MFrame *frame = (MFrame*) GetFrame();
	this->Create(parent, W_HSCROLL | W_VSCROLL | W_FULL_SELECTION | W_CHECK);
	this->SetImageList(frame->GetImageList32());
	this->GetColumn(0, column).SetText("id");
	this->AppendColumn(column, "progress");
	this->AppendColumn(column, "int");
	column.SetAlignment(W_CENTER);
	this->SetHeaderVisible(true);
	this->SetLinesVisible(true);
	int imagelistcount = WMAX(1, this->GetImageList()->GetCount());
	for (wuint i = 0; i < 100; i++) {
		sprintf(txt, "item %d", i);
		AppendItem(item, txt).SetImage(i % imagelistcount);
		item.SetData(new Person(i));
	}
}

WControl* TListViewDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}
