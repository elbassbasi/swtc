/*
 * TCursors.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TCursors.h"
#include "TCursorDemo.h"

void TCursors::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Cursor", new TCursors());
}

bool TCursors::Expand(WTreeItem &item) {
	TCursorDemo::Registre(item);
	return true;
}
