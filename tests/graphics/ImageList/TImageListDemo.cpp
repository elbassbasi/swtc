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
	WImageList *imagelist = shell->GetImageList32();
	imagelist->DrawNormal(*e.gc, IMAGELIST_ADD, 0, 0);
	return true;
}
