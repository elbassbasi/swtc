/*
 * TTooltips.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTooltips.h"

void TTooltips::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "ToolTip", new TTooltips());
}

bool TTooltips::Expand(WTreeItem &item) {
	return true;
}
