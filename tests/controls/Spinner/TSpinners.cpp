/*
 * TSpinners.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TSpinners.h"
#include "TSpinnerDemo.h"

void TSpinners::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Spinner", new TSpinners());
}

bool TSpinners::Expand(WTreeItem &item) {
	TSpinnerDemo::Registre(item);
	return true;
}
