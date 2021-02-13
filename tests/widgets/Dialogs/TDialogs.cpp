/*
 * TDialogs.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TDialogs.h"
#include "TLogin.h"

void TDialogs::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Dialogs", new TDialogs());
}

bool TDialogs::Expand(WTreeItem &item) {
	TLogin::Registre(item);
	return true;
}
