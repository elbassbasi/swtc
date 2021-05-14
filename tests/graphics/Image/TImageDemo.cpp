/*
 * TImageDemo.cpp
 *
 *  Created on: 15 févr. 2021
 *      Author: azeddine
 */

#include "TImageDemo.h"
void TImageDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TImageDemo());
}
TImageDemo::TImageDemo() {
}

TImageDemo::~TImageDemo() {
}

void TImageDemo::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
}

bool TImageDemo::OnPaint(WPaintEvent &e) {
	MFrame *shell = (MFrame*) GetFrame();
	WImage *image = shell->GetImage32(IMAGELIST_ADD);
	e->DrawImage(*image, 0, 0);
	return true;
}
