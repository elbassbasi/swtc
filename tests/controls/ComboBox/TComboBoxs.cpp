/*
 * TCombos.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TComboBoxs.h"

#include "../ComboBox/TComboDemo.h"

void TComboBoxs::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Combo", new TComboBoxs());
}

bool TComboBoxs::Expand(WTreeItem &item) {
	TComboDemo::Registre(item);
	return true;
}
