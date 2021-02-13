/*
 * TToolkits.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TToolkits.h"

void TToolkits::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Toolkit", new TToolkits());
}

bool TToolkits::Expand(WTreeItem &item) {
	return true;
}
