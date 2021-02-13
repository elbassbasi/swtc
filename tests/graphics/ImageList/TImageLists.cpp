/*
 * TImageLists.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TImageLists.h"

void TImageLists::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "ImageList", new TImageLists());
}

bool TImageLists::Expand(WTreeItem &item) {
	return true;
}
