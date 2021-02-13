/*
 * TGraphics.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TGraphics.h"
#include "Cursor/TCursors.h"
#include "Font/TFonts.h"
#include "GC/TGCs.h"
#include "Image/TImages.h"
#include "ImageList/TImageLists.h"
#include "Path/TPaths.h"
#include "Pattern/TPatterns.h"
#include "Region/TRegions.h"
#include "TextLayout/TTextLayouts.h"
#include "Transform/TTransforms.h"

void TGraphics::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent,"Graphics",new TGraphics());
}

bool TGraphics::Expand(WTreeItem &item) {
	TCursors::Registre(item);
	TFonts::Registre(item);
	TGCs::Registre(item);
	TImages::Registre(item);
	TImageLists::Registre(item);
	TPaths::Registre(item);
	TPatterns::Registre(item);
	TRegions::Registre(item);
	TTextLayouts::Registre(item);
	TTransforms::Registre(item);
	return true;
}
