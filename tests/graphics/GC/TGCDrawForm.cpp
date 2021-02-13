/*
 * Name:        TGCForm.cpp
 * Author:      Azeddine EL Bassbasi
 * Created:     2021-01-09
 * Copyright:   (c) 2021 Azeddine EL Bassbasi
 * Licence:
 */
#include "TGCDrawForm.h"

void TGCDrawForm::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "DrawForm", new TGCDrawForm(false));
	ITreeItem::Regitre(parent, "DrawFormAdvanced", new TGCDrawForm(true));
}

void TGCDrawForm::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
}

TGCDrawForm::TGCDrawForm(bool advanced) {
	this->advanced = advanced;
}

bool TGCDrawForm::OnPaint(WPaintEvent &e) {
	e->SetAdvanced(this->advanced);
	WRect clientArea;
	GetClientArea(clientArea);
	clientArea.y += 50;
	clientArea.height -= 50;
	WRect r(clientArea);
	const int space = 2;
	int x_inc = clientArea.width / 3;
	int y_inc = clientArea.height / 2;
	r.x = clientArea.x + space;
	r.y = clientArea.y + space;
	r.width = x_inc - 6 * space;
	r.height = y_inc - 4 * space;
	e->SetForeground(W_COLOR_GRAY);
	e->DrawText("Rectangle", -1, r, W_DRAW_CENTRE);
	e->DrawRectangle(r);
	r.x += x_inc;
	e->SetForeground(W_COLOR_BLUE);
	e->DrawText("Polyline", -1, r, W_DRAW_CENTRE);
	e->DrawPolyline(3, r.x, r.y, r.x + r.width, r.y, r.x, r.y + r.height);
	r.x += x_inc;
	e->SetForeground(W_COLOR_GREEN);
	e->DrawText("Oval", -1, r, W_DRAW_CENTRE);
	e->DrawOval(r);
	r.x = clientArea.x + space;
	r.y += y_inc;
	e->SetForeground(W_COLOR_YELLOW);
	e->DrawText("Arc", -1, r, W_DRAW_CENTRE);
	e->DrawArc(r, 0, 90);
	r.x += x_inc;
	e->SetForeground(W_COLOR_MAGENTA);
	e->DrawText("RoundRectangle", -1, r, W_DRAW_CENTRE);
	e->DrawRoundRectangle(r, 30, 30);
	r.x += x_inc;
	e->SetForeground(W_COLOR_RED);
	e->DrawText("Polygon", -1, r, W_DRAW_CENTRE);
	e->DrawPolygon(3, r.x, r.y, r.x + r.width, r.y, r.x, r.y + r.height);
	return true;
}
