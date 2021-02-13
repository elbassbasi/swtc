/*
 * TButtons.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TButtons.h"

#include "TButtonDemo.h"
void TButtons::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Button", new TButtons());
}

bool TButtons::Expand(WTreeItem &item) {
	TButtonDemo::Registre(item);
	return true;
}
