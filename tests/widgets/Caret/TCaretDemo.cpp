/*
 * TCaretDemo.cpp
 *
 *  Created on: 21 sept. 2020
 *      Author: azeddine
 */

#include "TCaretDemo.h"
void TCaretDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TCaretDemo());
}
TCaretDemo::TCaretDemo() {
}

TCaretDemo::~TCaretDemo() {
}

void TCaretDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	caret.Create(this);
	WImage image;
	WSurface surface;
	WGraphics gc;
	surface.Create(20, 20);
	surface.GetGraphics(gc);
	gc.SetBackground(W_COLOR_BLACK);
	gc.FillRectangle(0, 0, 20, 20);
	gc.SetForeground(W_COLOR_WHITE);
	gc.DrawLine(0, 0, 19, 19);
	gc.DrawLine(19, 0, 0, 19);
	gc.Dispose();
	surface.GetImage(image);
	surface.Dispose();
	caret.SetLocation(10, 10);
	caret.SetImage(image);
	this->GetGraphics(gc);
	gc.DrawImage(image, 10, 64);
	caret.SetVisible(false);
	gc.DrawString("Test", 12, 12);
	caret.SetVisible(true);
	gc.Dispose();
	this->SetCaret(&caret);
	this->ForceFocus();
}

bool TCaretDemo::OnMouseUp(WMouseEvent &e) {
	ForceFocus();
	return true;
}
