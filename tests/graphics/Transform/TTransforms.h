/*
 * TTransforms.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_GRAPHICS_TRANSFORM_TTRANSFORMS_H_
#define TESTS_GRAPHICS_TRANSFORM_TTRANSFORMS_H_
#include "../TGraphics.h"
class TTransforms : public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_GRAPHICS_TRANSFORM_TTRANSFORMS_H_ */
