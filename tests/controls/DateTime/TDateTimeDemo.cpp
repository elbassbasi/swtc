/*
 * TDateTimeDemo.cpp
 *
 *  Created on: 23 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TDateTimeDemo.h"
int TDateTimeDemo::styles[length] = { //
		W_CALENDAR | W_BORDER, //
		W_DATE | W_SHORT, //
		W_TIME | W_SHORT //
		};
const char *TDateTimeDemo::names[length] = { //
		"calendar", //
				"date", //
				"time" //
		};
void TDateTimeDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TDateTimeDemo());
}

void TDateTimeDemo::CreateControl(WComposite *parent) {
	WGridData data;
	char txt[30];
	this->Create(parent, W_NONE);
	this->SetLayout(this->layout);
	layout.numColumns = 2;
	for (int i = 0; i < length; i++) {
		label[i].Create(this, W_NONE);
		snprintf(txt, sizeof(txt), "%s:", names[i]);
		label[i].SetText(txt);
		dates[i].Create(this, styles[i]);
	}
	ok.Create(this, W_PUSH);
	ok.SetText("ok");
	ok.SetLayoutData(WGridData(WGridData::HORIZONTAL_ALIGN_CENTER, 2, 1));
	ok.SetId(1);

}
bool TDateTimeDemo::OnNotify(WEvent &e) {
	char txt[0x100];
	if (e.widget->GetId() == 1) {
		snprintf(txt, sizeof(txt), "Date : ");
		WMessageBox(GetShell(), W_OK, "Login", txt);
	}
	return true;
}
