/*
 * TToolBarDemo.h
 *
 *  Created on: 12 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_TOOLBAR_TTOOLBARDEMO_H_
#define TESTS_CONTROLS_TOOLBAR_TTOOLBARDEMO_H_
#include "TToolBars.h"
struct TToolBarDemoItems {
	int style;
	int image;
	const char *text;
};
class TToolBarDemo: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	static const int num_toolbar = 4;
	static TToolBarDemoItems items[];
	void CreateControl(WComposite *parent);
	void CreateToolBar(int index, int style, bool set_text, bool set_image);
	void SetToolMenu(int index, WToolItem &item);
	void SetToolCombo(int index, WToolItem &item);
public:
	WGridLayout layout;
	WToolBar bar[num_toolbar];
	WMenu menu[num_toolbar];
	WComboBox combo;
};

#endif /* TESTS_CONTROLS_TOOLBAR_TTOOLBARDEMO_H_ */
