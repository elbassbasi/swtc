/*
 * TCarets.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_WIDGETS_CARET_TCARETS_H_
#define TESTS_WIDGETS_CARET_TCARETS_H_
#include "../TWidgets.h"
class TCarets : public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_WIDGETS_CARET_TCARETS_H_ */
