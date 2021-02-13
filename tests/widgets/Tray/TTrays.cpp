/*
 * TTrays.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTrays.h"

void TTrays::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Tray", new TTrays());
}

bool TTrays::Expand(WTreeItem &item) {
	return true;
}
