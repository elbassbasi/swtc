/*
 * TTextLayoutDemo.cpp
 *
 *  Created on: 26 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TTextLayoutDemo.h"

void TTextLayoutDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TTextLayoutDemo());
}

void TTextLayoutDemo::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
	this->font[0].Create("Arial", W_BOLD, 14);
	this->font[1].Create("Arial", W_ITALIC, 20);
	this->font[2].Create("Arial", W_NORMAL, 30);
}

bool TTextLayoutDemo::OnPaint(WPaintEvent &e) {
	WRect r;
	GetClientArea(r);
	e->SetBackground(W_COLOR_WHITE);
	e->SetForeground(W_COLOR_BLUE);
	//e.gc->FillRectangle(r);
	e->DrawTextTransparent("Hello", r.x, r.y);
	WTextLayout layout;
	WTextStyle style;
	layout.Create();
	layout.SetWidth(r.width);
	layout.SetText("English فرض محروس رقم 6 韓國語 \nFrench");
	style.SetBackground(W_COLOR_BLUE);
	style.SetForeground(W_COLOR_YELLOW);
	style.SetStrikeout(W_COLOR_RED);
	style.SetFont(&this->font[0]);
	layout.SetStyle(style, 0, 8);
	style.Init();
	style.SetBackground(W_COLOR_GREEN);
	style.SetForeground(W_COLOR_BLUE);
	style.SetFont(&this->font[1]);
	layout.SetStyle(style, 8, 15);
	style.Init();
	style.SetUnderline(W_UNDERLINE_DOUBLE, W_COLOR_MAGENTA);
	style.SetBackground(W_COLOR_BLUE);
	style.SetForeground(W_COLOR_MAGENTA);
	style.SetFont(&this->font[1]);
	style.SetRise(5);
	layout.SetStyle(style, 15, -1);
	layout.Draw(*e.gc, r.x, r.y);
	return true;
}
