/*
 * TRegions.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TRegions.h"

void TRegions::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Region", new TRegions());
}

bool TRegions::Expand(WTreeItem &item) {
	return true;
}
