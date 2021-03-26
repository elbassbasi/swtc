/*
 * TSpinnerDemo.cpp
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TSpinnerDemo.h"

void TSpinnerDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TSpinnerDemo());
}

void TSpinnerDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->SetLayout(this->layout);
	spinner.Create(this, W_NONE);
	/*spinner.SetMinimum(0);
	spinner.SetMaximum(1000);
	spinner.SetSelection(500);
	spinner.SetIncrement(1);
	spinner.SetPageIncrement(100);*/
}
