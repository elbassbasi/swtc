/*
 * TCoolbars.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TCoolBars.h"
#include "TCoolBarDemo.h"

void TCoolBars::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "CoolBar", new TCoolBars());
}

bool TCoolBars::Expand(WTreeItem &item) {
	TCoolBarDemo::Registre(item);
	return true;
}
