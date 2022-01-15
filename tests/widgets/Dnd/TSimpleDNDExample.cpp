/*
 * TSimpleDNDExample.cpp
 *
 *  Created on: 15 oct. 2020
 *      Author: azeddine
 */

#include "TSimpleDNDExample.h"

TSimpleDNDExample::TSimpleDNDExample() {
}

TSimpleDNDExample::~TSimpleDNDExample() {
}

void TSimpleDNDExample::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "SimpleDNDExample", new TSimpleDNDExample());
}

void TSimpleDNDExample::CreateControl(WComposite *parent) {
	this->Create(parent, W_FREE_MEMORY);
	this->SetLayout(layout);
	// Create the tree and some tree items
	tree.Create(this, W_NONE);
	WTreeItem root, item;
	char txt[30];
	tree.GetRootItem(root);
	for (int i = 0; i < 5; i++) {
		sprintf(txt, "Item %d", i);
		root.AppendItem(item, txt);
	}
	// Create the button
	button.Create(this, W_FLAT);
	button.SetText("Button");
	button.SetAlignment(W_CENTER);
	// Create the drag source on the tree
	ds.Create(&tree, W_DROP_MOVE);
	ds.SetTransfer(WTransfer::GetTextTransfer());
	// Create the drop target on the button
	dt.Create(&button, W_DROP_MOVE);
	dt.SetTransfer(WTransfer::GetTextTransfer());
}

bool TSimpleDNDExampleDragSource::OnDragSetData(WDragSourceEvent &e) {
	WTreeView* tree =(WTreeView*) this->GetControl();
	WTreeItem selection;
	tree->GetSelection(selection);
	WString txt = selection.GetText();
	e.SetData(txt, txt.GetLength());
	return true;
}

bool TSimpleDNDExampleDropTarget::OnDrop(WDropTargetEvent &e) {
	WString txt;
	e.GetData(txt);
	WControl* control = GetControl();
	WButton *button;
	if(WComposite::IsComposite(control)){
		button = &((TSimpleDNDExample*)control)->button;
	}else{
		button = (WButton*) control;
	}
	button->SetText(txt);
	return true;
}
