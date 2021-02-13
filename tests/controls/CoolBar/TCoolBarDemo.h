/*
 * TCoolBarDemo.h
 *
 *  Created on: 12 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_COOLBAR_TCOOLBARDEMO_H_
#define TESTS_CONTROLS_COOLBAR_TCOOLBARDEMO_H_
#include "TCoolBars.h"
class TCoolBarDemo : public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	void InitCoolBar(WCoolBar* coolbar,WToolBar* toolbars,int style);
public:
	WGridLayout layout;
	WCoolBar coolbar[2];
	WToolBar toolbar[8];
};


#endif /* TESTS_CONTROLS_COOLBAR_TCOOLBARDEMO_H_ */
