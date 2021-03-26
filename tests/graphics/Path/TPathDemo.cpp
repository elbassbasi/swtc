/*
 * TPathDemo.cpp
 *
 *  Created on: 23 oct. 2020
 *      Author: azeddine
 */

#include "TPathDemo.h"
void TPathDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TPathDemo());
}
TPathDemo::TPathDemo() {
	this->rotate = 0;
}

TPathDemo::~TPathDemo() {
}
void TPathDemo::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
	this->font.Create("Arial", W_BOLD, 30);
	this->transform.Create();
	this->SetTimer(500);
}
bool TPathDemo::OnTimer(WTimerEvent &e) {
	rotate++;
	transform.Identity();
	this->transform.Translate(200, 200);
	transform.Rotate(rotate);
	if (rotate >= 360)
		rotate = 0;
	Redraw();
	return true;
}
bool TPathDemo::OnPaint(WPaintEvent &e) {
	WPath path;
	WRectF bounds;
	path.Create();
	path.AddString("Test", 0, 0, &font);
	path.GetBounds(bounds);
	e->SetTransform(&transform);
	e->SetBackground(W_COLOR_BLACK);
	e->FillPath(path);
	e->SetForeground(W_COLOR_RED);
	e->DrawPath(path);
	e->DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height);
	/*path.Create();
	 path.LineTo(50, 50);
	 path.QuadTo(0.1, 0.2, 30, 60);
	 path.Close();
	 e->FillPath(path);*/
	return true;
}
