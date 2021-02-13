/*
 * TSimpleDNDExample.h
 *
 *  Created on: 15 oct. 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_DND_TSIMPLEDNDEXAMPLE_H_
#define TESTS_WIDGETS_DND_TSIMPLEDNDEXAMPLE_H_
#include "TDnd.h"
class TSimpleDNDExampleDragSource: public WDragSource {
public:
	bool OnDragSetData(WDragSourceEvent &e);
};
class TSimpleDNDExampleDropTarget: public WDropTarget {
public:
	bool OnDrop(WDropTargetEvent &e);
};
class TSimpleDNDExample: public CompositeTreeItem {
public:
	TSimpleDNDExample();
	~TSimpleDNDExample();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	WFillLayout layout;
	WTreeView tree;
	WButton button;
	TSimpleDNDExampleDragSource ds;
	TSimpleDNDExampleDropTarget dt;
};

#endif /* TESTS_WIDGETS_DND_TSIMPLEDNDEXAMPLE_H_ */
