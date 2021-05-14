/*
 * TGraphics.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_GRAPHICS_TGRAPHICS_H_
#define TESTS_GRAPHICS_TGRAPHICS_H_
#include "../main/MFrame.h"
class TGraphics: public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_GRAPHICS_TGRAPHICS_H_ */
