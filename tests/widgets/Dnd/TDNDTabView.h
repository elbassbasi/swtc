/*
 * TDNDTabView.h
 *
 *  Created on: 25 janv. 2022
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_DND_TDNDTABVIEW_H_
#define TESTS_WIDGETS_DND_TDNDTABVIEW_H_
#include "TDnd.h"
class TDNDTabView: public CompositeTreeItem,
		public WDragSource,
		public WDropTarget {
public:
	TDNDTabView();
	~TDNDTabView();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
protected:
	bool OnDragStart(WDragSourceEvent &e);
	bool OnDragSetData(WDragSourceEvent &e);
	bool OnDrop(WDropTargetEvent &e);
public:
	WFillLayout layout;
	WTabView tabview;
	WButton button;
	WTabItem dragedItem;
};

#endif /* TESTS_WIDGETS_DND_TDNDTABVIEW_H_ */
