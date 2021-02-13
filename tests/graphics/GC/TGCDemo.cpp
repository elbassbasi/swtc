/*
 * Name:        TGCDemo.cpp
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-05-10
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "TGCDemo.h"

void TGCDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TGCDemo());
}

bool TGCDemo::OnPaint(WPaintEvent &e) {
	WRect clientArea;
	GetClientArea(clientArea);
	clientArea.x = clientArea.width / 2 - 2;
	clientArea.y = clientArea.height / 2 - 2;
	clientArea.width /= 2;
	clientArea.height /= 2;
	e->SetForeground(W_COLOR_RED);
	e->DrawRectangle(5, 5, 200, 10);
	e->DrawTextTransparent("You can draw text directly on a canvas", 60, 60);
	int flags = W_DRAW_DELIMITER | W_DRAW_TAB | W_DRAW_TRANSPARENT;
	e->DrawRectangle(clientArea);
	e->DrawText("centre", -1, clientArea, W_DRAW_CENTRE | flags);
	e->DrawText("vcentre", -1, clientArea, W_DRAW_VCENTER | flags);
	e->DrawText("hcentre", -1, clientArea, W_DRAW_HCENTER | flags);
	e->DrawText("right", -1, clientArea, W_DRAW_RIGHT | flags);
	return true;
}
