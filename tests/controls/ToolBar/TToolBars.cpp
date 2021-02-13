/*
 * TToolBars.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TToolBars.h"
#include "TToolBarDemo.h"
#include "TToolbarComplex.h"

void TToolBars::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "ToolBar", new TToolBars());
}

bool TToolBars::Expand(WTreeItem &item) {
	TToolBarDemo::Registre(item);
	TToolbarComplex::Registre(item);
	return true;
}
