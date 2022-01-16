/*
 * TButtonCircle.cpp
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#include "TButtonCircle.h"
void TButtonCircle::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Circle", new TButtonCircle());
}
TButtonCircle::TButtonCircle() {
}
TButtonCircle::~TButtonCircle() {
}
void TButtonCircle::CreateControl(WComposite *parent) {
	char txt[30];
	this->Create(parent, W_FREE_MEMORY);
	//this->layout.numColumns = 2;
	this->SetLayout(this->layout);
	WRegion r;
	r.CreateElliptic(87 -67 , 77 - 67, 67, 67);
	for (size_t i = 0; i < length; i++) {
		btn[i].Create(this, W_PUSH);
		sprintf(txt, "button %d", (int) i + 1);
		btn[i].SetText(txt);
		btn[i].SetRegion(r);
	}
}

bool TButtonCircle::OnNotifySelection(WEvent &e) {
	if (WButton::IsButton(e.widget)) {
		WButton *b = static_cast<WButton*>(e.widget);
		WString text = b->GetText();
		WMessageBox(GetFrame(), W_OK, "button selected",
				text.GetCharsNotNull());
		return true;
	}
	return false;
}
