/*
 * TDateTimeDemo.h
 *
 *  Created on: 23 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_DATETIME_TDATETIMEDEMO_H_
#define TESTS_CONTROLS_DATETIME_TDATETIMEDEMO_H_
#include "TDateTimes.h"
class TDateTimeDemo: public CompositeTreeItem {
public:
	static const int length = 3;
	static int styles[length];
	static const char *names[length];
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
protected:
	bool OnNotifySelection(WEvent &e);
public:
	WGridLayout layout;
	WDateTime dates[length];
	WLabel label[length];
	WButton btn[length];
};

#endif /* TESTS_CONTROLS_DATETIME_TDATETIMEDEMO_H_ */
