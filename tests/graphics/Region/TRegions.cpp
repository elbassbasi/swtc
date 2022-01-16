/*
 * TRegions.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TRegions.h"
#include "TRegionDemo.h"
#include "TRegionPolygon.h"

void TRegions::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Region", new TRegions());
}

bool TRegions::Expand(WTreeItem &item) {
	TRegionDemo::Registre(item);
	TRegionPolygon::Registre(item);
	return true;
}
