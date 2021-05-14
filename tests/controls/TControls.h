/*
 * TControls.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_TCONTROLS_H_
#define TESTS_CONTROLS_TCONTROLS_H_
#include "../main/MFrame.h"
class TControls: public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_CONTROLS_TCONTROLS_H_ */
