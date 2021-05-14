/*
 * TLogin.cpp
 *
 *  Created on: 1 oct. 2020
 *      Author: azeddine
 */

#include "TLogin.h"
void TLogin::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Login", new TLogin());
}
TLogin::TLogin() {
}

TLogin::~TLogin() {
}

void TLogin::CreateControl(WComposite *parent) {
	this->Create(parent, W_DELETE_ON_DISPOSE);
	this->layout.numColumns = 2;
	this->SetLayout(&this->layout);
	/*
	 *
	 */
	label[0].Create(this, W_CENTER);
	label[0].SetText("User name:");
	text[0].Create(this, W_LEFT | W_BORDER | W_SINGLE);

	label[1].Create(this, W_CENTER);
	label[1].SetText("Password:");
	text[1].Create(this, W_LEFT | W_BORDER | W_SINGLE | W_PASSWORD);

	button.Create(this, W_PUSH);
	button.SetText("Login");

	text[0].SetLayoutData(WGridData(WGridData::FILL_HORIZONTAL));
	text[1].SetLayoutData(WGridData(WGridData::FILL_HORIZONTAL));
	button.SetLayoutData(WGridData(WGridData::HORIZONTAL_ALIGN_CENTER, 2, 1));
	button.SetId(1);
}
bool TLogin::OnNotify(WEvent &e) {
	char txt[0x100];
	if (e.widget->GetId() == 1) {
		snprintf(txt, sizeof(txt), "User name:%s\nPassword:%s",
				text[0].GetTextStd().c_str(), text[1].GetTextStd().c_str());
		WMessageBox(GetFrame(), W_OK, "Login", txt);
	}
	return true;
}
