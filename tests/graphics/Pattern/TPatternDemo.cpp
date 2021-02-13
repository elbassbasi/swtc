/*
 * TPatternDemo.cpp
 *
 *  Created on: 8 févr. 2021
 *      Author: azeddine
 */

#include "TPatternDemo.h"
void TPatternDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "demo", new TPatternDemo());
}
TPatternDemo::TPatternDemo() {
}
TPatternDemo::~TPatternDemo() {
}
void TPatternDemo::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
	WSurface surface(1000, 1000);
	WGraphics gc(surface);
	gc.SetBackground(W_COLOR_WHITE);
	gc.SetForeground(W_COLOR_YELLOW);
	gc.FillGradientRectangle(0, 0, 1000, 1000, true);
	for (int i = -500; i < 1000; i += 10) {
		gc.SetForeground(W_COLOR_BLUE);
		gc.DrawLine(i, 0, 500 + i, 1000);
		gc.DrawLine(500 + i, 0, i, 1000);
	}
	gc.Dispose();
	WImage image;
	surface.GetImage(image);
	pattern.Create(image);
}

bool TPatternDemo::OnPaint(WPaintEvent &e) {
	WRect r;
	GetClientArea(r);
	e->SetBackgroundPattern(pattern);
	e->FillOval(5, 5, r.width - 10, r.height - 10);
	return true;
}
