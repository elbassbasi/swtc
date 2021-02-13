/*
 * TTabFolderDemo.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_TABVIEW_TTABVIEWDEMO_H_
#define TESTS_CONTROLS_TABVIEW_TTABVIEWDEMO_H_
#include "TTabViews.h"
class TTabViewDemo : public WTabView , public ITreeItem {
public:
	static const int numofTab = 20;
	static void Registre(WTreeItem &parent);
	WControl* GetControl(WComposite *parent);
	void CreateControl(WComposite *parent);
	WTextEdit text[numofTab];
};

#endif /* TESTS_CONTROLS_TABVIEW_TTABVIEWDEMO_H_ */
