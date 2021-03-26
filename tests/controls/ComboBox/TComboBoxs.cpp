/*
 * TCombos.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TComboBoxs.h"
#include "TComboDemo.h"

void TComboBoxs::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "ComboBox", new TComboBoxs());
}

bool TComboBoxs::Expand(WTreeItem &item) {
	TComboDemo::Registre(item);
	return true;
}
