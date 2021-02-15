/*
 * TImages.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TImages.h"
#include "TImageDemo.h"

void TImages::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Image", new TImages());
}

bool TImages::Expand(WTreeItem &item) {
	TImageDemo::Registre(item);
	return true;
}
