/*
 * TShells.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TShells.h"

void TShells::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Shell", new TShells());
}

bool TShells::Expand(WTreeItem &item) {
	return true;
}
