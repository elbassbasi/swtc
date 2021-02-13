/*
 * TDnd.h
 *
 *  Created on: 5 oct. 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_DND_TDND_H_
#define TESTS_WIDGETS_DND_TDND_H_
#include "../TWidgets.h"
class TDnd : public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_WIDGETS_DND_TDND_H_ */
