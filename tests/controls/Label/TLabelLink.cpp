/*
 * TLabelLink.cpp
 *
 *  Created on: 16 mars 2021
 *      Author: azeddine
 */

#include "TLabelLink.h"
void TLabelLink::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Link", new TLabelLink());
}
TLabelLink::TLabelLink() {
}

TLabelLink::~TLabelLink() {
}
void TLabelLink::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->layout.numColumns = 2;
	this->SetLayout(this->layout);
	this->link[0].Create(this, W_HYPERLINK);
	const char *text =
			"The SWT component is designed to provide <a>efficient</a>, <a>portable</a>"
					"<a href=\"native\">access to the user-interface facilities of the operating systems</a>"
					"on which it is implemented.";
	this->link[0].SetText(text);
}
