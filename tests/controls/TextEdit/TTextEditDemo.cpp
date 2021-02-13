/*
 * TTextDemo.cpp
 *
 *  Created on: 12 mai 2020
 *      Author: azeddine
 */

#include "TTextEditDemo.h"

void TTextEditDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TTextEditDemo());
}

void TTextEditDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->layout.numColumns = 2;
	this->SetLayout(this->layout);

	/* text read only */
	text[0].Create(this, W_READ_ONLY | W_BORDER);
	text[0].SetText("READ ONLY");
	text[0].SetLayoutData(WGridData(WGridData::FILL_BOTH));

	/* text multi */
	text[3].Create(this, W_MULTI | W_BORDER | W_WRAP | W_VSCROLL);
	text[3].SetText("Multi");
	text[3].SetLayoutData(WGridData(WGridData::FILL_BOTH,1,4));

	/* text normal */
	text[1].Create(this, W_BORDER);
	text[1].SetText("Normal");
	text[1].SetLayoutData(WGridData(WGridData::FILL_HORIZONTAL));

	/* text Password */
	text[2].Create(this, W_PASSWORD | W_BORDER);
	text[2].SetText("Password");
	text[2].SetLayoutData(WGridData(WGridData::FILL_HORIZONTAL));
}
