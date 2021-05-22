/*
 * TClipboard.cpp
 *
 *  Created on: 5 oct. 2020
 *      Author: azeddine
 */

#include "TClipboard.h"

TClipboard::TClipboard() {
}

TClipboard::~TClipboard() {
}

void TClipboard::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Clipboard", new TClipboard());
}

void TClipboard::CreateControl(WComposite *parent) {
	this->Create(parent, W_FREE_MEMORY);
	text.Create(this, W_BORDER | W_MULTI | W_VSCROLL | W_HSCROLL);
	copy.CreatePush(this, "Copy");
	copy.SetId(1);
	paste.CreatePush(this, "Paste");
	paste.SetId(2);

	this->SetLayout(layout);
	layout.numColumns = 2;
	text.SetLayoutData(WGridData(W_GRID_FILL_BOTH, 2, 1));
}

bool TClipboard::OnNotifySelection(WEvent &e) {
	if (e.widget->GetId() == 1)
		return OnCopy();
	if (e.widget->GetId() == 2)
		return OnPaste();
	return false;
}

bool TClipboard::OnCopy() {
	WString txt = this->text.GetText();
	if (!txt.IsEmpty()) {
		WClipboard clipboard;
		if (clipboard.Open()) {
			clipboard.SetContents(WTransfer::GetTextTransfer(), txt);
		}
	}
	return true;
}

bool TClipboard::OnPaste() {
	WClipboard clipboard;
	if (clipboard.Open()) {
		WString text;
		if (clipboard.GetContents(WTransfer::GetTextTransfer(), text)) {
			this->text.SetText(text);
		}
	}
	return true;
}
