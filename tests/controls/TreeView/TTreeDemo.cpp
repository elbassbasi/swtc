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
	this->Create(parent, W_HSCROLL | W_VSCROLL | W_FULL_SELECTION | W_CHECK);
	CreateColumns();
	CreateItems();
	CreateFonts();
}
WControl* TTreeDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}

void TTreeDemo::CreateColumns() {
	WColumnItem column;
	MFrame *frame = (MFrame*) GetFrame();
	this->SetHeaderImageList(frame->GetImageList32());
	this->GetColumn(0, column).SetText("id");
	column.SetImage(1);
	this->AppendColumn(column, "progress").SetImage(2);
	this->AppendColumn(column, "int").SetImage(3);
	column.SetAlignment(W_CENTER);
	this->SetHeaderVisible(true);
	this->SetLinesVisible(true);
}

void TTreeDemo::CreateItems() {
	WTreeItem item1, root, item2;
	char txt[50];
	MFrame *frame = (MFrame*) GetFrame();
	this->SetImageList(frame->GetImageList32());
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

void TTreeDemo::CreateFonts() {
	WFontData fontdata;
	this->GetFont()->GetFontData(fontdata);
	fontdata.SetStyle(W_NORMAL);
	this->fonts[0].Create(fontdata);
	fontdata.SetStyle(W_BOLD);
	this->fonts[1].Create(fontdata);
	fontdata.SetStyle(W_ITALIC);
	this->fonts[2].Create(fontdata);
}
bool TTreeDemo::OnItemGetValue(WTreeEvent &e) {
	PersonDemo *p = (PersonDemo*) e.item->GetData();
	switch (e.column->GetIndex()) {
	case 0:
		e.value->SetString(p->name);
		break;
	case 2:
		e.value->Sprint("i %s %d", p->name, p->progress);
		break;
	default:
		return WTreeView::OnItemGetValue(e);
		break;
	}
	return true;
}

bool TTreeDemo::OnItemGetAttr(WTreeEvent &e) {
	PersonDemo *p;
	if (e.column->GetIndex() == 2) {
		p = (PersonDemo*) e.item->GetData();
		if (p != 0) {
			if (e.item->GetChecked()) {
				e.attr->foreground = W_COLOR_RED;
				e.attr->background = W_COLOR_DARK_GREEN;
				e.attr->font = &this->fonts[2];
			} else {
				e.attr->foreground = W_COLOR_MAGENTA;
				e.attr->background = W_COLOR_DARK_GREEN;
				e.attr->font = &this->fonts[1];
			}
		}
		return true;
	}
	return false;
}

bool TTreeDemo::OnItemDispose(WTreeEvent &e) {
	PersonDemo *p = (PersonDemo*) e.item->GetData();
	if (p != 0) {
		e.item->SetData(0);
		delete p;
	}
	return false;
}
