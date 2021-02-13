/*
 * TDateTimeDemo.cpp
 *
 *  Created on: 23 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TDateTimeDemo.h"

void TDateTimeDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TDateTimeDemo());
}

void TDateTimeDemo::CreateControl(WComposite *parent) {
	WGridData data;
	this->Create(parent, W_NONE);
	this->SetLayout(this->layout);
	layout.numColumns = 3;
	dates[0].Create(this, W_CALENDAR | W_BORDER);
	dates[1].Create(this, W_DATE | W_SHORT);
	dates[2].Create(this, W_TIME | W_SHORT);
	label[0].Create(this, W_NONE);
	label[1].Create(this, W_NONE);
	ok.Create(this, W_PUSH);
	ok.SetText("ok");

}
