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
	layout.numColumns = 3;
	for (int i = 0; i < length; i++) {
		label[i].Create(this, W_NONE);
		snprintf(txt, sizeof(txt), "%s:", names[i]);
		label[i].SetText(txt);
		dates[i].Create(this, styles[i]);
		layout.SetData(dates[i], WGridData(W_GRID_FILL_HORIZONTAL));
		btn[i].CreatePush(this, "show");
		btn[i].SetId(i + 1);
	}
}
bool TDateTimeDemo::OnNotifySelection(WEvent &e) {
	char txt[0x100];
	int id = e.widget->GetId();
	if (id > 0) {
		int index = id - 1;
		int year = dates[index].GetYear();
		int month = dates[index].GetMonth();
		int day = dates[index].GetDay();
		int hours = dates[index].GetHours();
		int minutes = dates[index].GetMinutes();
		int seconds = dates[index].GetSeconds();
		snprintf(txt, sizeof(txt),
				"Year = %d\nMonth = %d\nDay = %d\nHours = %d\nMinutes = %d\nSeconds = %d\n",
				year, month, day, hours, minutes, seconds);
		WMessageBox(GetFrame(), W_OK, "Date", txt);
	}
	return true;
}
