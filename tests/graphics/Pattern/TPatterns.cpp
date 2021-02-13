/*
 * TPatterns.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TPatterns.h"
#include "TPatternDemo.h"
void TPatterns::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Pattern", new TPatterns());
}

bool TPatterns::Expand(WTreeItem &item) {
	TPatternDemo::Registre(item);
	return true;
}
