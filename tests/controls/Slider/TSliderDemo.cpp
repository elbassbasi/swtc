/*
 * TSliderDemo.cpp
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TSliderDemo.h"

void TSliderDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TSliderDemo());
}

void TSliderDemo::CreateControl(WComposite *parent) {
	this->Create(parent,W_NONE);
	this->SetLayout(this->layout);
	slider[0].Create(this,W_NONE);
	slider[0].SetLayoutData(WGridData(W_GRID_FILL_HORIZONTAL));
	slider[1].Create(this,W_VERTICAL);
	slider[1].SetLayoutData(WGridData(W_GRID_FILL_VERTICAL));
}
