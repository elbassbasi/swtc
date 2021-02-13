/*
 * TExpandBars.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_EXPANDBAR_TEXPANDBARS_H_
#define TESTS_CONTROLS_EXPANDBAR_TEXPANDBARS_H_
#include "../TControls.h"
class TExpandBars : public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_CONTROLS_EXPANDBAR_TEXPANDBARS_H_ */
