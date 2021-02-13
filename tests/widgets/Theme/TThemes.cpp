/*
 * TThemes.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TThemes.h"
#include "TThemeDemo.h"
void TThemes::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Theme", new TThemes());
}

bool TThemes::Expand(WTreeItem &item) {
	TThemeDemo::Registre(item);
	return true;
}
