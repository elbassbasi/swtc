/*
 * TLabelDemo.h
 *
 *  Created on: 10 mai 2020
 *      Author: azeddine
 */

#ifndef TESTS_CONTROLS_LABEL_TLABELDEMO_H_
#define TESTS_CONTROLS_LABEL_TLABELDEMO_H_
#include "TLabels.h"
class TLabelDemo: public CompositeTreeItem {
public:
	WGridLayout layout;
	WLabel label[5];
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
};

#endif /* TESTS_CONTROLS_LABEL_TLABELDEMO_H_ */
