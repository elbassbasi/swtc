/*
 * TTabFolders.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTabViews.h"

#include "TTabViewClose.h"
#include "TTabViewDemo.h"

void TTabViews::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "TabView", new TTabViews());
}

bool TTabViews::Expand(WTreeItem &item) {
	TTabViewDemo::Registre(item);
	TTabViewClose::Registre(item);
	return true;
}
