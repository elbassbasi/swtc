/*
 * TPatterns.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_GRAPHICS_PATTERN_TPATTERNS_H_
#define TESTS_GRAPHICS_PATTERN_TPATTERNS_H_
#include "../TGraphics.h"
class TPatterns : public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_GRAPHICS_PATTERN_TPATTERNS_H_ */
