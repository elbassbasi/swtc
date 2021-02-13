/*
 * TSashDemo.cpp
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TSashDemo.h"

void TSashDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TSashDemo());
}

void TSashDemo::CreateControl(WComposite *parent) {
	this->Create(parent, 0);
	this->SetLayout(this->layout);
	char txt[30];
	for (size_t i = 0; i < sizeof(this->btn) / sizeof(this->btn[0]); i++) {
		btn[i].Create(this, W_PUSH | W_CENTER);
		sprintf(txt, "btn %d", (int) i);
		btn[i].SetText(txt);
	}
}
