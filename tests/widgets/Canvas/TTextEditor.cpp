/*
 * TTextEditor.cpp
 *
 *  Created on: 6 sept. 2020
 *      Author: azeddine
 */

#include "TTextEditor.h"

TTextEditor::TTextEditor() {
	strcpy(this->text,"hh int hh  int ");
	this->length = strlen(this->text);
}

TTextEditor::~TTextEditor() {
}

void TTextEditor::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent,"TextEditor",new TTextEditor());
}

void TTextEditor::CreateControl(WComposite *parent) {
	this->Create(parent, W_HSCROLL | W_VSCROLL);
	menu.Create(this,W_POP_UP);
	WMenuItem item,root;
	menu.GetRoot(root);
	for(int i=0;i<5;i++){
		root.AppendItem(item,"item1");
	}
	SetMenu(&this->menu);
}
bool TTextEditor::OnPaint(WPaintEvent &e) {
	WRect rect;
	WTextLayout textlayout;
	WTextStyle style;
	GetClientArea(rect);
	e.gc->SetBackground(W_COLOR_WHITE);
	e.gc->FillRectangle(rect);
	textlayout.Create();
	textlayout.SetText(this->text, this->length);
	for (int i = 0; i < this->length - 3; i++) {
		if (!strncmp(&this->text[i], " int ", 5)) {
			style.Init();
			style.SetUnderlineSingle(W_COLOR_BLUE);
			style.SetForeground(W_COLOR_RED);
			style.SetStrikeout(W_COLOR_BLACK);
			textlayout.SetStyle(style, i + 1, i + 4);
			i += 3;
		}
	}
	textlayout.Draw(*e.gc, 0, 0);
	return true;
}

bool TTextEditor::OnKeyDown(WKeyEvent &e) {
	Add(e.character);
	Redraw();
	return false;
}

void TTextEditor::Add(int c) {
	char txt[10];
	int sz = w_utf8_from_unichar(c, txt);
	if ((this->length + sz) >= (int) sizeof(this->text))
		return;
	memcpy(&this->text[this->length], txt, sz);
	this->length += sz;
	this->text[this->length] = 0;
}

bool TTextEditor::OnMouseDown(WMouseEvent &e) {
	ForceFocus();
	return false;
}

bool TTextEditor::OnMouseUp(WMouseEvent &e) {
	ForceFocus();
	return false;
}
