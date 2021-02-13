/*
 * TComposites.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TComposites.h"
#include "TCompositeDemo.h"
#include "TScrolledComposite.h"

void TComposites::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Composite", new TComposites());
}

bool TComposites::Expand(WTreeItem &item) {
	TCompositeDemo::Registre(item);
	TScrolledComposite::Registre(item);
	return true;
}
