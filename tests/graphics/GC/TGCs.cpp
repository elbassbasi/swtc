/*
 * Name:        TGCs.cpp
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-05-10
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "TGCDrawForm.h"
#include "TGCDemo.h"
#include "TGCLine.h"
#include "TGCFillForm.h"
#include "TGCDrawText.h"

void TGCs::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Graphics", new TGCs());
}
bool TGCs::Expand(WTreeItem &item) {
	TGCDemo::Registre(item);
	TGCLine::Registre(item);
	TGCDrawForm::Registre(item);
	TGCFillForm::Registre(item);
	TGCDrawText::Registre(item);
	return true;
}
