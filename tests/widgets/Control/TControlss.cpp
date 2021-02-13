/*
 * TControlss.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TControlss.h"
#include "TControlMouse.h"
#include "TDragging.h"

void TControlss::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Control", new TControlss());
}

bool TControlss::Expand(WTreeItem &item) {
	TControlMouse::Registre(item);
	TDragging::Registre(item);
	return true;
}
