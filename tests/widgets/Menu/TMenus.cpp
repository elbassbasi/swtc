/*
 * TMenus.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TMenus.h"

void TMenus::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Menu", new TMenus());
}

bool TMenus::Expand(WTreeItem &item) {
	return true;
}
