/*
 * TWordJumbles.h
 *
 *  Created on: 8 oct. 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_DND_TWORDJUMBLES_H_
#define TESTS_WIDGETS_DND_TWORDJUMBLES_H_
#include "TDnd.h"
#define NUM_LABEL 5
class TWordJumblesDragSource: public WDragSource {
public:
	bool OnDragStart(WDragSourceEvent &e);
	bool OnDragSetData(WDragSourceEvent &e);
	bool OnDragFinished(WDragSourceEvent &e);
};
class TWordJumblesDropTarget: public WDropTarget {
public:
	bool OnDragEnter(WDropTargetEvent &e);
	bool OnDragOver(WDropTargetEvent &e);
	bool OnDragLeave(WDropTargetEvent &e);
	bool OnDragOperationChanged(WDropTargetEvent &e);
	bool OnDropAccept(WDropTargetEvent &e);
	bool OnDrop(WDropTargetEvent &e);
};
class TWordJumbles: public CompositeTreeItem {
public:
	TWordJumbles();
	~TWordJumbles();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	void CreateLabel(int index,int& width);
	void check();
public:
	const char* word;
	WFont *font;
	WLabel labelsRowOne[NUM_LABEL];
	WLabel labelsRowTwo[NUM_LABEL];
	TWordJumblesDragSource dragSource[NUM_LABEL];
	TWordJumblesDropTarget dropTarget[NUM_LABEL];
};

#endif /* TESTS_WIDGETS_DND_TWORDJUMBLES_H_ */
