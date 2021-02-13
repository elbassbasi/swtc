/*
 * TCanvass.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TCanvass.h"
#include "TTextEditor.h"

void TCanvass::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Canvas", new TCanvass());
}

bool TCanvass::Expand(WTreeItem &item) {
	TTextEditor::Registre(item);
	return true;
}
