/*
 * TDnd.cpp
 *
 *  Created on: 5 oct. 2020
 *      Author: azeddine
 */

#include "TDnd.h"
#include "TClipboard.h"
#include "TSimpleDNDExample.h"
#include "TWordJumbles.h"
void TDnd::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Dnd", new TDnd);
}

bool TDnd::Expand(WTreeItem &item) {
	TClipboard::Registre(item);
	TSimpleDNDExample::Registre(item);
	TWordJumbles::Registre(item);
	return true;
}
