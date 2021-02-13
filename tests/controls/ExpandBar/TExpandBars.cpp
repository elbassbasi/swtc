/*
 * TExpandBars.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TExpandBars.h"
#include "TExpandBarDemo.h"

void TExpandBars::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "ExpandBar", new TExpandBars());
}

bool TExpandBars::Expand(WTreeItem &item) {
	TExpandBarDemo::Registre(item);
	return true;
}
