/*
 * TTrays.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTrays.h"
#include "TTrayDemo.h"

void TTrays::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Tray", new TTrays());
}

bool TTrays::Expand(WTreeItem &item) {
	TTrayDemo::Registre(item);
	return true;
}
