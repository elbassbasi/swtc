/*
 * TGLCanvass.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TGLCanvass.h"

void TGLCanvass::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "GLCanvas", new TGLCanvass());
}

bool TGLCanvass::Expand(WTreeItem &item) {
	return true;
}
