/*
 * TButtonStyles.cpp
 *
 *  Created on: 10 mai 2020
 *      Author: azeddine
 */

#include "TButtonDemo.h"

void TButtonDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TButtonDemo());

}
int TButtonDemo::styles[] = { W_PUSH, W_CHECK, W_TOGGLE, W_RADIO, W_FLAT,
		W_COMMAND };
int TButtonDemo::styles_arrow[] = { W_RIGHT, W_LEFT, W_DOWN, W_UP };
const char *TButtonDemo::names[] = { "push", "check", "toggle", "radio", "flat",
		"command", 0 };
void TButtonDemo::CreateControl(WComposite *parent) {
	char txt[30];
	this->Create(parent, W_FREE_MEMORY);
	this->layout.numColumns = length;
	this->SetLayout(this->layout);
	for (size_t i = 0; i < 6; i++) {
		for (size_t j = 0; j < length; j++) {
			btn[i][j].Create(this, styles[i]);
			sprintf(txt, "%s %d", names[i], (int) j + 1);
			btn[i][j].SetText(txt);
		}
	}
	MFrame *frame = (MFrame*) GetFrame();
	btn[0][2].SetImage(frame->GetImage16(1));
	btn[0][3].SetImage(frame->GetImage32(4));

	// Create three arrow buttons
	for (size_t j = 0; j < length; j++) {
		wuint64 _style = W_ARROW | styles_arrow[j % 4];
		btn[6][j].Create(this, _style);
	}

	label.Create(this, W_NONE);
	label.SetLayoutData(WGridData(W_GRID_FILL_BOTH, length, 1));
	label.SetText("No Selection");
}

bool TButtonDemo::OnNotifySelection(WEvent &e) {
	if (WButton::IsButton(e.widget)) {
		WButton *b = static_cast<WButton*>(e.widget);
		WString text = b->GetText();
		label.SetText("button selected : " + text);
		return true;
	}
	return false;
}
