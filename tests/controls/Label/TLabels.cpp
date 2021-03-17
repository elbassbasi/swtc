/*
 * TLabels.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TLabels.h"
#include "TLabelDemo.h"
#include "TLabelLink.h"

void TLabels::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Label", new TLabels());
}

bool TLabels::Expand(WTreeItem &item) {
	TLabelDemo::Registre(item);
	TLabelLink::Registre(item);
	return true;
}
