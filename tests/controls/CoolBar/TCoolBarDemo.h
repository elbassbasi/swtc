/*
 * TCoolBarDemo.h
 *
 *  Created on: 12 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_COOLBAR_TCOOLBARDEMO_H_
#define TESTS_CONTROLS_COOLBAR_TCOOLBARDEMO_H_
#include "TCoolBars.h"
struct TToolBarDemoItems0 {
	int style;
	int image;
	const char *text;
};
class TCoolBarDemo: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	static const int num_toolbar = 2;
	static const int num_coolbar = 3;
	static TToolBarDemoItems0 items[];
	void CreateControl(WComposite *parent);
	void CreateToolBar(WToolBar &toolbar, int style, bool set_text,
			bool set_image);
	void CreateCoolBar(int index, int style);
public:
	WGridLayout layout;
	WCoolBar coolbar[num_coolbar];
	WToolBar toolbars[num_coolbar][num_toolbar];
};

#endif /* TESTS_CONTROLS_COOLBAR_TCOOLBARDEMO_H_ */
