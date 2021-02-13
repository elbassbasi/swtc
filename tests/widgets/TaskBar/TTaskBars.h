/*
 * TTaskBars.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_WIDGETS_TASKBAR_TTASKBARS_H_
#define TESTS_WIDGETS_TASKBAR_TTASKBARS_H_
#include "../TWidgets.h"
class TTaskBars: public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_WIDGETS_TASKBAR_TTASKBARS_H_ */
