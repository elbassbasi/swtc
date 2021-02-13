/*
 * TImages.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TImages.h"

void TImages::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Image", new TImages());
}

bool TImages::Expand(WTreeItem &item) {
	return true;
}
