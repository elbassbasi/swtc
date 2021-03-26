/*
 * TTrees.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTreeViews.h"
#include "TTreeDemo.h"
#include "TTreeCustom.h"
void TTreeViews::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "TreeView", new TTreeViews());
}

bool TTreeViews::Expand(WTreeItem &item) {
	TTreeDemo::Registre(item);
	TTreeCustom::Registre(item);
	return true;
}
