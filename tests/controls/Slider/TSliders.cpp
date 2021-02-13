/*
 * TSliders.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TSliders.h"
#include "TSliderDemo.h"

void TSliders::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Slider", new TSliders());
}

bool TSliders::Expand(WTreeItem &item) {
	TSliderDemo::Registre(item);
	return true;
}
