/*
 * MTree.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "MPanel.h"
#include "MShell.h"
MPanel::MPanel() {
	this->width = 200;
	this->control = 0;
}

MPanel::~MPanel() {
}

bool MPanel::Create(WComposite *parent) {
	WTreeView::Create(parent, W_HSCROLL | W_VSCROLL);
	sash.Create(parent, W_VERTICAL);
	MShell *shell = (MShell*) this->GetShell();
	this->SetImageList(shell->GetImageList16());
	WTreeItem root;
	GetRootItem(root);
	ITreeItem::RegitreRoot(root);
	return true;
}

bool MPanel::OnItemExpand(WTreeEvent &e) {
	if (e.item != 0) {
		ITreeItem *obj = (ITreeItem*) e.item->GetData();
		if (obj != 0) {
			return obj->OnExpand(*e.item);
		}
	}
	return false;
}

bool MPanel::OnItemCollapse(WTreeEvent &e) {
	return false;
}

void MPanel::UpdateSize() {
	WRect area;
	GetShell()->GetClientArea(area);
	this->SetBounds(area.x, area.y, this->width, area.height - 3);
	sash.SetBounds(this->width, area.y, 3, area.height - 3);
	if (this->control != 0) {
		this->control->SetBounds(this->width + 3, area.y,
				area.width - (this->width + 3) - 3, area.height - 3);
	}
}

bool MPanel::OnSashSelection(WSashEvent &e) {
	WRect sashBounds;
	sash.GetBounds(sashBounds);
	int shift = e.bounds.x - sashBounds.x;
	this->width += shift;
	UpdateSize();
	return true;
}

bool MPanel::OnItemSelection(WTreeEvent &e) {
	if (e.item != 0) {
		ITreeItem *obj = (ITreeItem*) e.item->GetData();
		if (obj != 0) {
			if (this->control != 0) {
				this->control->SetVisible(false);
			}
			WControl *c = obj->GetControl(GetShell());
			if (c != 0) {
				c->SetVisible(true);
			}
			this->control = c;
			this->UpdateSize();
		}
	}
	return false;
}

bool MPanel::OnItemDispose(WTreeEvent &e) {
	if (e.item != 0) {
		ITreeItem *obj = (ITreeItem*) e.item->GetData();
		if (obj != 0)
			delete obj;
	}
	return WTreeView::OnItemDispose(e);
}
