/*
 * TGroups.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TGroupBoxs.h"
#include "TGroupBoxDemo.h"

void TGroupBoxs::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "GroupBox", new TGroupBoxs());
}

bool TGroupBoxs::Expand(WTreeItem &item) {
	TGroupBoxDemo::Registre(item);
	return true;
}
