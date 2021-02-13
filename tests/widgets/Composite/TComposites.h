/*
 * TComposites.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_WIDGETS_COMPOSITE_TCOMPOSITES_H_
#define TESTS_WIDGETS_COMPOSITE_TCOMPOSITES_H_
#include "../TWidgets.h"
class TComposites : public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_WIDGETS_COMPOSITE_TCOMPOSITES_H_ */
