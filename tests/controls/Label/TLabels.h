/*
 * TLabels.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_LABEL_TLABELS_H_
#define TESTS_CONTROLS_LABEL_TLABELS_H_
#include "../TControls.h"
class TLabels : public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_CONTROLS_LABEL_TLABELS_H_ */
