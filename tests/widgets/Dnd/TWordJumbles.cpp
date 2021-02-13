/*
 * TWordJumbles.cpp
 *
 *  Created on: 8 oct. 2020
 *      Author: azeddine
 */

#include "TWordJumbles.h"

TWordJumbles::TWordJumbles() {
	this->word = 0;
}

TWordJumbles::~TWordJumbles() {
}

void TWordJumbles::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "WordJumbles", new TWordJumbles());
}
char randomPermutation[] = { 5, 2, 6, 3, 1, 4, 0 };
void TWordJumbles::CreateControl(WComposite *parent) {
	this->Create(parent, W_FREE_MEMORY);
	this->word = "ECLIPSE";
	int width = 40;
	font.Create("Arial", 18, W_BOLD);
	for (size_t i = 0; i < NUM_LABEL; i++) {
		CreateLabel(i, width);
	}
}
void TWordJumbles::CreateLabel(int i, int &width) {
	labelsRowOne[i].Create(this, W_BORDER);
	labelsRowOne[i].SetBounds(10 + width * i, 10, width - 5, width - 5);
	//labelsRowOne[i].SetFont(&font);
	char tmp[2];
	tmp[0] = word[randomPermutation[i]];
	tmp[1] = 0;
	labelsRowOne[i].SetText(tmp);
	//labelsRowOne[i].SetAlignment(W_CENTER);
	if (i == 0) {
		dragSource[i].Create(this, W_DROP_MOVE);
	} else
		dragSource[i].Create(&labelsRowOne[i], W_DROP_MOVE);
	dragSource[i].SetTransfer(WTransfer::GetTextTransfer());

	labelsRowTwo[i].Create(this, W_BORDER);
	labelsRowTwo[i].SetBounds(10 + width * i, 20 + width, width - 5, width - 5);
	labelsRowTwo[i].SetBackground(W_COLOR_WHITE);
	//labelsRowTwo[i].SetFont(&font);
	//labelsRowTwo[i].SetAlignment(W_CENTER);

	dropTarget[i].Create(&labelsRowTwo[i], W_DROP_MOVE);
	dropTarget[i].SetTransfer(WTransfer::GetTextTransfer());
}
void TWordJumbles::check() {
	/*for(int i=0; i<word.length(); i++) {
	 if(! labelsRowTwo[i].getText().equals(word.charAt(i) + ""))
	 return;
	 }
	 MessageBox messageBox = new MessageBox(shell);
	 messageBox.setMessage("Success!");
	 messageBox.open();*/
}
/*
 * TWordJumblesDropTarget
 */
bool TWordJumblesDropTarget::OnDragEnter(WDropTargetEvent &e) {
	WLabel *label = (WLabel*) GetControl();
	// Does not accept any drop if the label has text on it.
	WString str = label->GetText();
	if (!str.IsEmpty())
		e.detail = W_DROP_NONE;
	return true;
}

bool TWordJumblesDropTarget::OnDragOver(WDropTargetEvent &e) {
	return WDropTarget::OnDragOver(e);
}

bool TWordJumblesDropTarget::OnDragLeave(WDropTargetEvent &e) {
	return WDropTarget::OnDragLeave(e);
}

bool TWordJumblesDropTarget::OnDragOperationChanged(WDropTargetEvent &e) {
	return WDropTarget::OnDragOperationChanged(e);
}

bool TWordJumblesDropTarget::OnDropAccept(WDropTargetEvent &e) {
	return WDropTarget::OnDropAccept(e);
}

bool TWordJumblesDropTarget::OnDrop(WDropTargetEvent &e) {
	if (WTransfer::GetTextTransfer()->IsSupportedType(e.currentDataType)) {
		WString text;
		e.GetData(text);
		WLabel *label = (WLabel*) GetControl();
		label->SetText(text);
		// Checks the result.
		TWordJumbles *jumbls = static_cast<TWordJumbles*>(label->GetParent());
		jumbls->check();
	}
	return true;
}
/*
 * TWordJumblesDragSource
 */
bool TWordJumblesDragSource::OnDragStart(WDragSourceEvent &e) {
	// Disallows drags if text is not available.
	WControl *control = GetControl();
	WLabel *label;
	if (WComposite::IsComposite(control)) {
		label = &static_cast<TWordJumbles*>(control)->labelsRowOne[0];
	} else {
		label = static_cast<WLabel*>(control);
	}
	WString str = label->GetText();
	if (str.IsEmpty())
		e.doit = false;
	/*WSurface surface;
	surface.Create(48, 48);
	WGraphics gc;
	surface.GetGraphics(gc);
	gc.SetForeground(W_COLOR_WHITE);
	gc.SetBackground(W_COLOR_BLACK);
	gc.DrawText(str, 0, 0);
	WImage image;
	surface.GetImage(e.image);*/
	return false;
}

bool TWordJumblesDragSource::OnDragSetData(WDragSourceEvent &e) {
	WControl *control = GetControl();
	WLabel *label;
	if (WComposite::IsComposite(control)) {
		label = &static_cast<TWordJumbles*>(control)->labelsRowOne[0];
	} else {
		label = static_cast<WLabel*>(control);
	}
	// Provides the text data.
	if (WTransfer::GetTextTransfer()->IsSupportedType(e.dataType)) {
		WString str = label->GetText();
		e.SetData(str.GetChars(), str.GetLength());
	}
	return false;
}
bool TWordJumblesDragSource::OnDragFinished(WDragSourceEvent &e) {
	WControl *control = GetControl();
	WLabel *label;
	if (WComposite::IsComposite(control)) {
		label = &static_cast<TWordJumbles*>(control)->labelsRowOne[0];
	} else {
		label = static_cast<WLabel*>(control);
	}
	// Removes the text after the move operation.
	if (e.doit == true || e.detail == W_DROP_MOVE) {
		label->SetText("");
	}
	return false;
}
