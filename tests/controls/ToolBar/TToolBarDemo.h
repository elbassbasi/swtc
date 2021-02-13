/*
 * TToolBarDemo.h
 *
 *  Created on: 12 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_TOOLBAR_TTOOLBARDEMO_H_
#define TESTS_CONTROLS_TOOLBAR_TTOOLBARDEMO_H_
#include "TToolBars.h"
class TToolBarDemo: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
public:
	WGridLayout layout;
	WToolBar bar[2];
};


#endif /* TESTS_CONTROLS_TOOLBAR_TTOOLBARDEMO_H_ */
