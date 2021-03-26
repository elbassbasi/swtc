/*
 * TBrowserDemo.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TWebViewDemo.h"

void TWebViewDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent,"Demo",new TWebViewDemo());
}

WControl* TWebViewDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
    
}

void TWebViewDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->SetText("<!DOCTYPE html><html><head><title>Page Title</title></head>"
			"<body><h1>My First Heading</h1><a href=\"hehh\">"
			"Test Browser.</a> HEllO AZEDDINE</body></html>");
}
