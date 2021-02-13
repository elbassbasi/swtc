/*
 * MMenu.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "MMenu.h"
#include "MApp.h"
WMenuItems MMenu::menuitems[] = { //
				{ W_CASCADE, 0, "File", 0 },	//
				{ W_PUSH, 0, "NewPlatform", W_ACTION(MMenu::NewPlatformShell) },//
				{ W_PUSH, 0, "NewCustom", W_ACTION(MMenu::NewCustomShell) },//
				{ W_SEPARATOR, 0, 0, 0 },	//
				{ W_PUSH, W_ALT | 'Q', "Exit", W_ACTION(MMenu::Exit) },	//
				{ W_CASCADE_END, 0, 0, 0 },	//
				{ W_CASCADE, 0, "Dialogs", 0 },	//
				{ W_PUSH, 0, "MessageBox", W_ACTION(MMenu::MessageBox) },	//
				{ W_PUSH, 0, "FileDialog", W_ACTION(MMenu::FileDialog) },	//
				{ W_PUSH, 0, "DirectoryDialog", W_ACTION(MMenu::DirectoryDialog) },	//
				{ W_PUSH, 0, "FontDialog", W_ACTION(MMenu::FontDialog) },	//
				{ W_PUSH, 0, "ColorDialog", W_ACTION(MMenu::ColorDialog) },	//
				{ W_CASCADE_END, 0, 0, 0 },	//
				{ W_CASCADE, 0, "Source", 0 },	//
				{ W_CASCADE_END, 0, 0, 0 },	//
				{ 0, 0, 0, 0 },	//
		};
bool MMenu::Create(WShell *parent) {
	WMenu::CreateBar(parent);
	CreateItems(MMenu::menuitems);
	parent->SetMenuBar(this);
	return true;
}

bool MMenu::OnItemSelection(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener)
		return listener->HandleEvent(reinterpret_cast<WEvent*>(&e));
	else {
		if (this->items != 0) {
			wushort id = e.item->GetId();
			WControl::SelectionAction action = this->items[id].action;
			if (action != 0) {
				(((WControl*) this)->*action)(&e);
			}
		}
	}
	return Notify(e);
}

bool MMenu::Exit() {
	this->GetShell()->Close();
	return true;
}
bool MMenu::NewPlatformShell() {
	MApp::Get()->NewPlatformShell();
	return true;
}
bool MMenu::NewCustomShell() {
	MApp::Get()->NewCustomShell();
	return true;
}

bool MMenu::MessageBox() {
	WMessageBox(this->GetShell(), W_YES | W_NO, "Message Box",
			"Test Message Box");
	return true;
}

bool MMenu::DirectoryDialog() {
	return true;
}

bool MMenu::ColorDialog() {
	return true;
}

bool MMenu::FontDialog() {
	return true;
}

bool MMenu::FileDialog() {
	return true;
}
