/*
 * TDateTimes.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TDateTimes.h"
#include "TDateTimeDemo.h"

void TDateTimes::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "DateTime", new TDateTimes());
}

bool TDateTimes::Expand(WTreeItem &item) {
	TDateTimeDemo::Registre(item);
	return true;
}
