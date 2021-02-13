/*
 * Name:        TGCDrawText.cpp
 * Author:      Azeddine EL Bassbasi
 * Created:     2021-01-23
 * Copyright:   (c) 2021 Azeddine EL Bassbasi
 * Licence:
 */
#include "TGCDrawText.h"
void TGCDrawText::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "DrawText", new TGCDrawText(false));
	ITreeItem::Regitre(parent, "DrawTextAdvanced", new TGCDrawText(true));
}

void TGCDrawText::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
}

TGCDrawText::TGCDrawText(bool advanced) {
	this->advanced = advanced;
}

bool TGCDrawText::OnPaint(WPaintEvent &e) {
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
	e->SetForeground(W_COLOR_BLUE);
	e->SetBackground(W_COLOR_GRAY);
	e->DrawRectangle(r);
	e->DrawText("transparent", -1, r, W_DRAW_TRANSPARENT);
	r.x += x_inc;
	e->SetForeground(W_COLOR_BLUE);
	e->DrawRectangle(r);
	e->DrawText("left", -1, r, W_DRAW_LEFT);
	r.x += x_inc;
	e->SetForeground(W_COLOR_GREEN);
	e->DrawRectangle(r);
	e->DrawText("top", -1, r, W_DRAW_TOP);
	r.x = clientArea.x + space;
	r.y += y_inc;
	e->SetForeground(W_COLOR_YELLOW);
	e->DrawRectangle(r);
	e->DrawText("right", -1, r, W_DRAW_RIGHT);
	r.x += x_inc;
	e->SetForeground(W_COLOR_MAGENTA);
	e->DrawRectangle(r);
	e->DrawText("bottom", -1, r, W_DRAW_BOTTOM);
	r.x += x_inc;
	e->SetForeground(W_COLOR_RED);
	e->DrawRectangle(r);
	e->DrawText("centre", -1, r, W_DRAW_CENTRE);
	return true;
}
