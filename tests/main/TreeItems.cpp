/*
 * TreeItems.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TreeItems.h"
#include "../controls/TControls.h"
#include "../graphics/TGraphics.h"
#include "../widgets/TWidgets.h"
void ITreeItem::RegitreRoot(WTreeItem &root) {
	TControls::Registre(root);
	TGraphics::Registre(root);
	TWidgets::Registre(root);
}
void ITreeItem::Regitre(WTreeItem &parent, const char *text, ITreeItem *obj) {
	WTreeItem item;
	parent.AppendItem(item, text);
	item.SetData(obj);
	if (obj->HasChildren()) {
		item.SetHasChildren();
		item.SetImage(IMAGELIST_FLDR);
	} else {
		item.SetImage(IMAGELIST_FILE);
	}
}

ITreeItem::~ITreeItem() {
}

bool ITreeItem::OnExpand(WTreeItem &item) {
	return false;
}

bool ITreeItem::HasChildren() {
	return false;
}

FolderTreeItem::FolderTreeItem() {
	this->expand = false;
}

bool FolderTreeItem::Expand(WTreeItem &item) {
	return true;
}

bool FolderTreeItem::OnExpand(WTreeItem &item) {
	if (this->expand == false) {
		this->Expand(item);
		this->expand = true;
	}
	return true;
}

bool FolderTreeItem::HasChildren() {
	return true;
}

WControl* FolderTreeItem::GetControl(WComposite *parent) {
	return 0;
}

bool CompositeTreeItem::OnExpand(WTreeItem &item) {
	return false;
}

bool CompositeTreeItem::HasChildren() {
	return false;
}

WControl* CompositeTreeItem::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}

void CompositeTreeItem::CreateControl(WComposite *parent) {
	this->Create(parent,W_NONE);
}
bool CanvasTreeItem::OnExpand(WTreeItem &item) {
	return false;
}

bool CanvasTreeItem::HasChildren() {
	return false;
}

WControl* CanvasTreeItem::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}


void CanvasTreeItem::CreateControl(WComposite *parent) {
	this->Create(parent,W_NONE);
}
