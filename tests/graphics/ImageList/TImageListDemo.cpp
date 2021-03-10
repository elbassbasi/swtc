/*
 * TImageListDemo.cpp
 *
 *  Created on: 15 févr. 2021
 *      Author: azeddine
 */

#include "TImageListDemo.h"
void TImageListDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TImageListDemo());
}
TImageListDemo::TImageListDemo() {
}

TImageListDemo::~TImageListDemo() {
}

void TImageListDemo::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
}

bool TImageListDemo::OnPaint(WPaintEvent &e) {
	MShell *shell = (MShell*) GetShell();
	WRect r;
	WSize sz;
	int x = 0, y = 0;
	GetClientArea(r);
	WImageList *imagelist = shell->GetImageList32();
	int count = imagelist->GetCount();
	imagelist->GetSize(sz);
	count = WMAX(count, 1);
	for (int i = 0; i < count; i++) {
		imagelist->DrawNormal(*e.gc, i, x, y);
		x += sz.width + 1;
		if ((x + sz.width) > r.width) {
			x = 0;
			y += sz.height + 1;
		}
	}
	return true;
}
