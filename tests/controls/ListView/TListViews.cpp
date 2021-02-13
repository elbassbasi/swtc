/*
 * TListViews.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TListViews.h"

#include "TListViewDemo.h"
void TListViews::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "ListView", new TListViews());
}

bool TListViews::Expand(WTreeItem &item) {
	TListViewDemo::Registre(item);
	return true;
}
