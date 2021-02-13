/*
 * TProgressBars.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TProgressBars.h"
#include "TProgressBarDemo.h"

void TProgressBars::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "ProgressBar", new TProgressBars());
}

bool TProgressBars::Expand(WTreeItem &item) {
	TProgressBarDemo::Registre(item);
	return true;
}
