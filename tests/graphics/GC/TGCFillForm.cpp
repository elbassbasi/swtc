/*
 * Name:        TGCForm.cpp
 * Author:      Azeddine EL Bassbasi
 * Created:     2021-01-20
 * Copyright:   (c) 2021 Azeddine EL Bassbasi
 * Licence:
 */
#include "TGCFillForm.h"
void TGCFillForm::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "FillForm", new TGCFillForm(false));
	ITreeItem::Regitre(parent, "FillFormAdvanced", new TGCFillForm(true));
}

void TGCFillForm::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
}

TGCFillForm::TGCFillForm(bool advanced) {
	this->advanced = advanced;
}

bool TGCFillForm::OnPaint(WPaintEvent &e) {
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
	e->SetForeground(W_COLOR_CYAN);
	e->SetBackground(W_COLOR_GRAY);
	e->FillRectangle(r);
	e->DrawText("Rectangle", -1, r, W_DRAW_CENTRE | W_DRAW_TRANSPARENT);
	r.x += x_inc;
	e->SetBackground(W_COLOR_BLUE);
	e->SetForeground(W_COLOR_RED);
	e->FillGradientRectangle(r, true);
	e->SetForeground(W_COLOR_CYAN);
	e->DrawText("GradientRectangle", -1, r, W_DRAW_CENTRE | W_DRAW_TRANSPARENT);
	r.x += x_inc;
	e->SetBackground(W_COLOR_GREEN);
	e->FillOval(r);
	e->DrawText("Oval", -1, r, W_DRAW_CENTRE | W_DRAW_TRANSPARENT);
	r.x = clientArea.x + space;
	r.y += y_inc;
	e->SetBackground(W_COLOR_YELLOW);
	e->FillArc(r, 0, 90);
	e->DrawText("Arc", -1, r, W_DRAW_CENTRE | W_DRAW_TRANSPARENT);
	r.x += x_inc;
	e->SetBackground(W_COLOR_MAGENTA);
	e->FillRoundRectangle(r, 30, 30);
	e->DrawText("RoundRectangle", -1, r, W_DRAW_CENTRE | W_DRAW_TRANSPARENT);
	r.x += x_inc;
	e->SetBackground(W_COLOR_RED);
	e->FillPolygon(3, r.x, r.y, r.x + r.width, r.y, r.x, r.y + r.height);
	e->DrawText("Polygon", -1, r, W_DRAW_CENTRE | W_DRAW_TRANSPARENT);
	return true;
}
