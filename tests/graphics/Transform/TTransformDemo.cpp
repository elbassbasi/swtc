/*
 * TTransformDemo.cpp
 *
 *  Created on: 8 févr. 2021
 *      Author: azeddine
 */

#include "TTransformDemo.h"
void TTransformDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TTransformDemo());
}
TTransformDemo::TTransformDemo() {
}

TTransformDemo::~TTransformDemo() {
}

void TTransformDemo::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
}
bool TTransformDemo::OnPaint(WPaintEvent &e) {
	WRect r;
	GetClientArea(r);
	WTransform transform;
	transform.Create();
	transform.Rotate(30);
	e->SetBackground(W_COLOR_BLUE);
	e->SetForeground(W_COLOR_GREEN);
	e->SetTransform(&transform);
	e->FillRectangle(100, 100, 100, 100);
	e->FillGradientRectangle(300, 200, 100, 100, true);
	return true;
}
