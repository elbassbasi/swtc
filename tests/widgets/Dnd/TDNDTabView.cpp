/*
 * TDNDTabView.cpp
 *
 *  Created on: 25 janv. 2022
 *      Author: azeddine
 */

#include "TDNDTabView.h"

TDNDTabView::TDNDTabView() {
	// TODO Auto-generated constructor stub

}

TDNDTabView::~TDNDTabView() {
	// TODO Auto-generated destructor stub
}

void TDNDTabView::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "TDNDTabView", new TDNDTabView());
}

void TDNDTabView::CreateControl(WComposite *parent) {
	WComposite::Create(parent, W_FREE_MEMORY);
	this->SetLayout(layout);
	// Create the tree and some tree items
	tabview.Create(this, W_NONE);
	WTabItem item;
	char txt[30];
	for (int i = 0; i < 5; i++) {
		sprintf(txt, "Item %d", i);
		tabview.AppendItem(item, txt);
	}
	// Create the button
	button.Create(this, W_FLAT);
	button.SetText("Button");
	button.SetAlignment(W_CENTER);
	// Create the drag source on the tree
	WDragSource::Create(&tabview, W_DROP_MOVE);
	WDragSource::SetTransfer(WTransfer::GetTextTransfer());
	// Create the drop target on the button
	WDropTarget::Create(&button, W_DROP_MOVE);
	WDropTarget::SetTransfer(WTransfer::GetTextTransfer());
}

bool TDNDTabView::OnDragStart(WDragSourceEvent &e) {
	/*tabview.GetItem(WPoint(e.x, e.y), dragedItem);
	if (!dragedItem.IsOk()) {
		e.doit = false;
		return false;
	}*/
	return true;
}

bool TDNDTabView::OnDragSetData(WDragSourceEvent &e) {
	WString txt = dragedItem.GetText();
	e.SetData(txt, txt.GetLength());
	return true;
}

bool TDNDTabView::OnDrop(WDropTargetEvent &e) {
	WString txt;
	e.GetData(txt);
	button.SetText(txt);
	return true;
}
