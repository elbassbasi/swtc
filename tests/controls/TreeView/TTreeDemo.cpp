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
TTreeDemo::TTreeDemo() {
	memset(fonts, 0, sizeof(fonts));
}
TTreeDemo::~TTreeDemo() {
	for (int i = 0; i < font_length; i++) {
		fonts[i]->Dispose();
	}
}
void TTreeDemo::CreateControl(WComposite *parent) {
	this->Create(parent,
			W_DOUBLE_BUFFERED | W_HSCROLL | W_VSCROLL | W_FULL_SELECTION
					| W_CHECK);
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
w_color TTreeDemo::columns_forgrounds[columns_count] = { W_COLOR_CYAN,
		W_COLOR_GREEN, W_COLOR_CYAN };
w_color TTreeDemo::columns_backgrounds[columns_count] = { W_COLOR_RED,
		W_COLOR_DARK_RED, W_COLOR_BLUE };
wuchar TTreeDemo::columns_fonts[columns_count] = { 0, 1, 2, 1 };
void TTreeDemo::CreateColumns() {
	WColumnItem column;
	MFrame *frame = (MFrame*) GetFrame();
	char txt[50];
	this->SetHeaderImageList(frame->GetImageList32());
	for (int i = 0; i < columns_count; i++) {
		if (i == 0)
			this->GetColumn(0, column);
		else
			this->AppendColumn(column);
		sprintf(txt, "column %d", i);
		column.SetText(txt);
		column.SetImage(i % 3);
		//column.SetAlignment(W_CENTER);
	}
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
	int columns = this->GetColumnCount();
	for (int i = 0; i < 50; i++) {
		if (root.AppendItem(item1).IsOk()) {
			item1.SetImage(i % imagelistcount);
			for (int k = 0; k < columns; k++) {
				sprintf(txt, "item (%d) %d", i, k);
				item1.SetText(k, txt);
				item1.SetBackground(k, columns_backgrounds[k]);
				item1.SetForeground(k, columns_forgrounds[k]);
				item1.SetFont(k, this->fonts[columns_fonts[k]]);
			}
			for (int j = 0; j < 5; j++) {
				item1.AppendItem(item2);
				item2.SetImage(j);
				for (int k = 0; k < columns; k++) {
					sprintf(txt, "item (%d,%d) %d", i, j, k);
					item2.SetText(k, txt);
					item2.SetBackground(k, columns_backgrounds[k]);
					item2.SetForeground(k, columns_forgrounds[k]);
					item2.SetFont(k, this->fonts[columns_fonts[k]]);
				}
			}
		}
	}
}

void TTreeDemo::CreateFonts() {
	WFontData fontdata;
	this->GetFont()->GetFontData(fontdata);
	fontdata.SetStyle(W_NORMAL);
	this->fonts[1] = WFont::Create(fontdata);
	fontdata.SetStyle(W_BOLD);
	fontdata.SetHeight(15);
	this->fonts[2] = WFont::Create(fontdata);
	fontdata.SetStyle(W_ITALIC);
	this->fonts[3] = WFont::Create(fontdata);
}
