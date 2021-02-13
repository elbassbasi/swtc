/*
 * TPaths.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TPaths.h"
#include "TPathDemo.h"

void TPaths::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Path", new TPaths());
}

bool TPaths::Expand(WTreeItem &item) {
	TPathDemo::Registre(item);
	return true;
}
