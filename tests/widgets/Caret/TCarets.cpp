/*
 * TCarets.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TCarets.h"
#include "TCaretDemo.h"

void TCarets::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Caret", new TCarets());
}

bool TCarets::Expand(WTreeItem &item) {
	TCaretDemo::Registre(item);
	return true;
}
