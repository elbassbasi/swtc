/*
 * TComboDemo.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TComboDemo.h"

void TComboDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TComboDemo());
}
const char *TComboDemo::ITEMS[] = { "Alpha", "Bravo", "Charlie", "Delta",
		"Echo", "Foxtrot", "Golf", "Hotel", "India", "Juliet", "Kilo", "Lima",
		"Mike", "November", "Oscar", "Papa", "Quebec", "Romeo", "Sierra",
		"Tango", "Uniform", "Victor", "Whiskey", "X-Ray", "Yankee", "Zulu" };

void TComboDemo::SetItems(WComboBox &combo) {
	WComboItem item;
	for (int i = 0; i < sizeof(ITEMS) / sizeof(ITEMS[0]); i++) {
		combo.Add(item, ITEMS[i]);
	}
}

void TComboDemo::CreateControls(int index, const char *text, wuint64 style) {
	label[index].Create(this, W_NONE);
	label[index].SetText(text);
	combo[index].Create(this, style);
	SetItems(combo[index]);
	btn[index].Create(this, W_PUSH);
	btn[index].SetId(index + 1);
	btn[index].SetText("show selected");
}
void TComboDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	layout.numColumns = 3;
	this->SetLayout(layout);
	// Create a drop-down combo
	CreateControls(0, "DropDown", W_DROP_DOWN);
	// Create a read only combo
	CreateControls(1, "ReadOnly", W_DROP_DOWN | W_READ_ONLY);
	// Create a "simple" combo
	CreateControls(2, "Simple", W_SIMPLE);
}

bool TComboDemo::OnNotify(WEvent &e) {
	wuint id = e.widget->GetId();
	if (id != 0) {
		WString text = combo[id - 1].GetText();
		WMessageBox(GetShell(), W_OK, "Select Button", text.GetChars());
	}
	return true;
}
