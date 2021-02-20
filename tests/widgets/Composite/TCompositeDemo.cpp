/*
 * TCompositeDemo.cpp
 *
 *  Created on: 3 août 2020
 *      Author: azeddine
 */

#include "TCompositeDemo.h"
WMenuItems TCompositeDemo::menuitems[] = { //
		{ W_CASCADE, 0,-1, "File", 0 },	//
				{ W_PUSH, 0,-1, "NewPlatform", 0 },	//
				{ W_PUSH, 0,-1, "NewCustom", 0 },	//
				{ W_SEPARATOR, 0,-1, 0, 0 },	//
				{ W_PUSH, W_ALT | 'Q',-1, "Exit", 0 },	//
				{ W_CASCADE_END, 0,-1, 0, 0 },	//
				{ W_CASCADE, 0,-1, "Dialogs", 0 },	//
				{ W_PUSH, 0,-1, "MessageBox", 0 },	//
				{ W_PUSH, 0,-1, "FileDialog", 0 },	//
				{ W_PUSH, 0,-1, "DirectoryDialog", 0 },	//
				{ W_PUSH, 0,-1, "FontDialog", 0 },	//
				{ W_PUSH, 0,-1, "ColorDialog", 0 },	//
				{ W_CASCADE_END, 0,-1, 0, 0 },	//
				{ W_CASCADE, 0,-1, "Source", 0 },	//
				{ W_CASCADE_END, 0,-1, 0, 0 },	//
		};
void TCompositeDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TCompositeDemo());
}

void TCompositeDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_HSCROLL | W_VSCROLL);
	button.CreatePush(this, "Test");
	button.SetId(1);
	this->SetLayout(layout);
	menu.CreatePopUp(this);
	menu.CreateItems(menuitems, sizeof(menuitems)/sizeof(menuitems[0]));
	this->SetMenu(&menu);
}

bool TCompositeDemo::OnPaint(WPaintEvent &e) {
	WRect r;
	GetClientArea(r);
	r.x += 20;
	r.width -= 20;
	e->SetBackground(W_COLOR_BLUE);
	e->FillRectangle(r);
	button.Redraw();
	return false;
}

bool TCompositeDemo::OnNotify(WEvent &e) {
	if(e.widget->GetId() == 1){
		menu.SetLocation(300,300);
		menu.SetVisible(true);
	}
	return true;
}
