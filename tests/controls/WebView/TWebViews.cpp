/*
 * TBrowsers.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TWebViews.h"

#include "TWebViewDemo.h"

void TWebViews::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Browser", new TWebViews());
}

bool TWebViews::Expand(WTreeItem &item) {
	TWebViewDemo::Registre(item);
	return true;
}
