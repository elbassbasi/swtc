/*
 * Name:        TLine.cpp
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-05-10
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "TGCLine.h"

void TGCLine::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Lines", new TGCLine());
}
wuchar caps[] = { W_CAP_FLAT, W_CAP_ROUND, W_CAP_SQUARE };
const char *caps_names[] = { "CAP_FLAT", "CAP_ROUND", "CAP_SQUARE" };
bool TGCLine::OnPaint(WPaintEvent &e) {
	int x = 20, y = 20, w = 120, h = 60;
	e->SetForeground(W_COLOR_BLUE);
	e->SetLineWidth(10);
	for (int i = 0; i < sizeof(caps) / sizeof(caps[0]); i++) {
		e->SetLineCap(caps[i]);
		e->DrawLine(x, y, x + w, y);
		e->DrawTextTransparent(caps_names[i], x + w + 20, y - 5);
		y += 20;
	}
	return false;
}
