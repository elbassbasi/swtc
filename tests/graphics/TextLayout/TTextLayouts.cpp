/*
 * TTextLayouts.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTextLayouts.h"
#include "TTextLayoutDemo.h"

void TTextLayouts::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "TextLayout", new TTextLayouts());
}

bool TTextLayouts::Expand(WTreeItem &item) {
	TTextLayoutDemo::Registre(item);
	return true;
}
