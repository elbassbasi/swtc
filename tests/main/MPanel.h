/*
 * MTree.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_MAIN_MPANEL_H_
#define TESTS_MAIN_MPANEL_H_
#include "TreeItems.h"
class MPanel: public WTreeView {
private:
	friend class MShell;
	WSash sash;
	int width;
	WControl *control;
public:
	MPanel();
	~MPanel();
	bool Create(WComposite *parent);
protected:
	void UpdateSize();
	bool OnSashSelection(WSashEvent &e);
	bool OnItemExpand(WTreeEvent &e);
	bool OnItemCollapse(WTreeEvent &e);
	bool OnItemSelection(WTreeEvent &e);
};

#endif /* TESTS_MAIN_MPANEL_H_ */
