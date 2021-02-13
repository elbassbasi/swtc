/*
 * TSashes.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TSashes.h"
#include "TSashDemo.h"

void TSashes::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Sash", new TSashes());
}

bool TSashes::Expand(WTreeItem &item) {
	TSashDemo::Registre(item);
	return true;
}
