/*
 * TTabFolderDemo.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TTabViewDemo.h"

void TTabViewDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TTabViewDemo());
}

WControl* TTabViewDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}

void TTabViewDemo::CreateControl(WComposite *parent) {
	WTabItem item;
	char txt[50];
	this->Create(parent, W_NONE);
	MFrame *frame = (MFrame*) GetFrame();
	this->SetImageList(frame->GetImageList16());
	int ImageListcount = frame->GetImageList16()->GetCount();
	if (ImageListcount <= 0)
		ImageListcount = 1;
	for (int i = 0; i < numofTab; i++) {
		this->text[i].Create(this, W_MULTI | W_WRAP | W_VSCROLL);
		sprintf(txt, "text #%d", i);
		this->text[i].SetText(txt);
		sprintf(txt, "tab %d", i);
		this->AppendItem(item, txt);
		item.SetControl(&this->text[i]);
		item.SetImage(i % ImageListcount);
	}
}
