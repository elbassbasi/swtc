/*
 * TTexts.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTextEdits.h"

#include "TTextEditDemo.h"

void TTextEdits::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Text", new TTextEdits());
}

bool TTextEdits::Expand(WTreeItem &item) {
	TTextEditDemo::Registre(item);
	return true;
}
