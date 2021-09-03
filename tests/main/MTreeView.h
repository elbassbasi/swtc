/*
 * MTree.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_MAIN_MTREEVIEW_H_
#define TESTS_MAIN_MTREEVIEW_H_
#include "TreeItems.h"
class MTreeView: public WTreeView {
public:
	WImageList imagelist;
	MTreeView();
	~MTreeView();
	bool Create(WComposite *parent);
protected:
	bool OnItemExpand(WTreeEvent &e);
	bool OnItemCollapse(WTreeEvent &e);
	bool OnItemSelection(WTreeEvent &e);
	bool OnItemDispose(WTreeEvent &e);
};

#endif /* TESTS_MAIN_MTREEVIEW_H_ */
