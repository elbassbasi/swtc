/*
 * TGroupDemo.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_GROUPBOX_TGROUPBOXDEMO_H_
#define TESTS_CONTROLS_GROUPBOX_TGROUPBOXDEMO_H_
#include "TGroupBoxs.h"
class TGroupBoxDemo: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	static const char* RADIO_TEXTS[];
	WGridLayout layout;
	WGroupBox group[2];
	WGridLayout grouplayout[2];
	WButton radio[8];
};

#endif /* TESTS_CONTROLS_GROUPBOX_TGROUPBOXDEMO_H_ */
