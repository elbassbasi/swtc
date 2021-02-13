/*
 * TButtonStyles.cpp
 *
 *  Created on: 10 mai 2020
 *      Author: azeddine
 */

#include "TButtonDemo.h"

void TButtonDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TButtonDemo());

}
int TButtonDemo::styles[] = { W_PUSH, W_CHECK, W_TOGGLE, W_RADIO, W_FLAT };
const char *TButtonDemo::names[] =
		{ "push", "check", "toggle", "radio", "flat" };
void TButtonDemo::CreateControl(WComposite *parent) {
	char txt[30];
	this->Create(parent, W_FREE_MEMORY);
	this->layout.numColumns = length;
	this->SetLayout(this->layout);
	for (size_t i = 0; i < 5; i++) {
		for (size_t j = 0; j < length; j++) {
			btn[i][j].Create(this, styles[i]);
			sprintf(txt, "%s %d", names[i], (int) j + 1);
			btn[i][j].SetText(txt);
		}
	}
	btn[0][2].SetImage(MShell::GetImage16_(this, 1));
	btn[0][3].SetImage(MShell::GetImage32_(this, 4));

	// Create three arrow buttons
	btn[5][0].Create(this, W_ARROW);
	btn[5][1].Create(this, W_ARROW | W_LEFT);
	btn[5][2].Create(this, W_ARROW | W_DOWN);
	btn[5][3].Create(this, W_ARROW | W_UP);
}

bool TButtonDemo::OnNotify(WEvent &e) {
	if (WButton::IsButton(e.widget)) {
		WButton *b = static_cast<WButton*>(e.widget);
		WString text = b->GetText();
		WMessageBox(GetShell(),W_OK,"button selected",text.GetCharsNotNull());
		return true;
	}
	return false;
}
