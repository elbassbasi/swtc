/*
 * TFontDemo.cpp
 *
 *  Created on: 8 févr. 2021
 *      Author: azeddine
 */

#include "TFontDemo.h"
void TFontDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TFontDemo());
}
TFontDemo::TFontDemo() {
}
TFontDemo::~TFontDemo() {
}
struct Fonts_Types {
	const char *name;
	int style;
	int size;
};
Fonts_Types types[] = { //
		{ "Tahoma", W_BOLD, 14 }, //
				{ "Arial", W_ITALIC, 30 }, //
				{ "Arial", 0, 7 }, //

		};
void TFontDemo::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
	for (size_t i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
		fonts[i].Create(types[i].name, types[i].style, types[i].size);
	}
}

bool TFontDemo::OnPaint(WPaintEvent &e) {
	WRect bounds;
	GetClientArea(bounds);
	int y = bounds.y;
	int _x = (bounds.width - bounds.x) / 2;
	int x, prev_heigth = 0, heigth = 0;
	char txt[50];
	for (size_t i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
		e->SetFont(&this->fonts[i]);
		snprintf(txt, sizeof(txt), "%s %d", types[i].name, types[i].size);
		if (i % 2 == 0) {
			x = bounds.x;
			y += WMAX(heigth, prev_heigth);
			prev_heigth = e->TextExtent(txt).height;
		} else {
			heigth = e->TextExtent(txt).height;
			x = _x;
		}
		e->DrawTextTransparent(txt, x, y);
	}
	return true;
}
