/*
 * TTaskBars.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TTaskBars.h"

void TTaskBars::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "TaskBar", new TTaskBars());
}

bool TTaskBars::Expand(WTreeItem &item) {
	return true;
}
