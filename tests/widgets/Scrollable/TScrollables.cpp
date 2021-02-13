/*
 * TScrollables.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TScrollables.h"

void TScrollables::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Scrollable", new TScrollables());
}

bool TScrollables::Expand(WTreeItem &item) {
	return true;
}
