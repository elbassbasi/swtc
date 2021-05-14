/*
 * MTree.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "MTreeView.h"

#include "MFrame.h"
MTreeView::MTreeView() {
}

MTreeView::~MTreeView() {
}

bool MTreeView::Create(WComposite *parent) {
	WTreeView::Create(parent, W_HSCROLL | W_VSCROLL);
	MFrame *shell = (MFrame*) this->GetFrame();
	this->SetImageList(shell->GetImageList16());
	WTreeItem root;
	GetRootItem(root);
	ITreeItem::RegitreRoot(root);
	return true;
}

bool MTreeView::OnItemExpand(WTreeEvent &e) {
	if (e.item != 0) {
		ITreeItem *obj = (ITreeItem*) e.item->GetData();
		if (obj != 0) {
			return obj->OnExpand(*e.GetTreeItem());
		}
	}
	return false;
}

bool MTreeView::OnItemCollapse(WTreeEvent &e) {
	return false;
}
bool MTreeView::OnItemSelection(WTreeEvent &e) {
	if (e.item != 0) {
		ITreeItem *obj = (ITreeItem*) e.item->GetData();
		if (obj != 0) {
			MFrame *shell = (MFrame*) this->GetFrame();
			if (shell->control != 0) {
				shell->control->SetVisible(false);
			}
			WControl *c = obj->GetControl(GetFrame());
			if (c != 0) {
				c->SetVisible(true);
			}
			shell->control = c;
			shell->UpdateSize();
		}
	}
	return false;
}

bool MTreeView::OnItemDispose(WTreeEvent &e) {
	if (e.item != 0) {
		ITreeItem *obj = (ITreeItem*) e.item->GetData();
		if (obj != 0)
			delete obj;
	}
	return WTreeView::OnItemDispose(e);
}
