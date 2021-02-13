/*
 * TWidgets.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_WIDGETS_TWIDGETS_H_
#define TESTS_WIDGETS_TWIDGETS_H_
#include "../main/MShell.h"
class TWidgets: public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_WIDGETS_TWIDGETS_H_ */
