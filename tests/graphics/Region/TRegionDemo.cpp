/*
 * TRegionDemo.cpp
 *
 *  Created on: 15 févr. 2021
 *      Author: azeddine
 */

#include "TRegionDemo.h"

void TRegionDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TRegionDemo());
}
TRegionDemo::TRegionDemo() {
}

TRegionDemo::~TRegionDemo() {
}

void TRegionDemo::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
}
bool TRegionDemo::OnPaint(WPaintEvent &e) {
	WRect r;
	GetClientArea(r);
	WRegion region;
	region.Create();
	region.Add(r.x, r.y, r.width / 2, r.height / 2);
	e->SetClipping(region);
	e->SetBackground(W_COLOR_BLUE);
	e->FillOval(r);
	return true;
}
