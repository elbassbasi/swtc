/*
 * TControlss.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_WIDGETS_CONTROL_TCONTROLSS_H_
#define TESTS_WIDGETS_CONTROL_TCONTROLSS_H_
#include "../TWidgets.h"
class TControlss : public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_WIDGETS_CONTROL_TCONTROLSS_H_ */
