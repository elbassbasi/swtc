/*
 * TTrackers.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTrackers.h"

void TTrackers::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Tracker", new TTrackers());
}

bool TTrackers::Expand(WTreeItem &item) {
	return true;
}
