/*
 * TGroupDemo.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TGroupBoxDemo.h"

void TGroupBoxDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TGroupBoxDemo());
}
const char *TGroupBoxDemo::RADIO_TEXTS[] = { "John", "Paul", "George", "Ringo",
		"Barry", "Robin", "Maurice" };
void TGroupBoxDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->SetLayout(this->layout);
	// Create the first group
	group[0].Create(this, W_SHADOW_IN);
	group[0].SetText("Who's your favorite?");
	group[0].SetLayout(this->grouplayout[0]);
	for (int i = 0; i < 4; i++) {
		radio[i].Create(group[0], W_RADIO);
		radio[i].SetText(RADIO_TEXTS[i]);
	}

	// Create the second group
	group[1].Create(this, W_NO_RADIO_GROUP);
	group[1].SetText("Who's your favorite?");
	group[1].SetLayout(this->grouplayout[1]);
	for (int i = 4; i < 7; i++) {
		radio[i].Create(group[1], W_RADIO);
		radio[i].SetText(RADIO_TEXTS[i]);
	}
}
