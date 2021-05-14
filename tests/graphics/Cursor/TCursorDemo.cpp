/*
 * TCursor.cpp
 *
 *  Created on: 15 févr. 2021
 *      Author: azeddine El Bassbasi
 */

#include "TCursorDemo.h"
void TCursorDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TCursorDemo());
}
TCursorDemo::TCursorDemo() {
}
TCursorDemo::~TCursorDemo() {
}
void TCursorDemo::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
	WImage *image = ((MFrame*) GetFrame())->GetImage32(IMAGELIST_DELETE);
	cursor.Create(*image, 0, 0);
	SetCursor(&cursor);
}
bool TCursorDemo::OnPaint(WPaintEvent &e) {
	return true;
}
