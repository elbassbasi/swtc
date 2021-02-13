/*
 * TTextLayouts.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_GRAPHICS_TEXTLAYOUT_TTEXTLAYOUTS_H_
#define TESTS_GRAPHICS_TEXTLAYOUT_TTEXTLAYOUTS_H_
#include "../TGraphics.h"
class TTextLayouts : public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_GRAPHICS_TEXTLAYOUT_TTEXTLAYOUTS_H_ */
