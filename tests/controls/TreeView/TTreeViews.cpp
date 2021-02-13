/*
 * TTrees.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "../TreeView/TTreeCustom.h"
#include "TTreeViews.h"
void TTreeViews::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Tree", new TTreeViews());
}

bool TTreeViews::Expand(WTreeItem &item) {
	TTreeCustom::Registre(item);
	return true;
}
