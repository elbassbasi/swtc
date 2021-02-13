/*
 * TDateTimeDemo.h
 *
 *  Created on: 23 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_DATETIME_TDATETIMEDEMO_H_
#define TESTS_CONTROLS_DATETIME_TDATETIMEDEMO_H_
#include "TDateTimes.h"
class TDateTimeDemo : public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
public:
	WGridLayout layout;
	WDateTime dates[3];
	WButton ok;
	WLabel label[2];
};

#endif /* TESTS_CONTROLS_DATETIME_TDATETIMEDEMO_H_ */
