/*
 * TLabelLink.h
 *
 *  Created on: 16 mars 2021
 *      Author: azeddine
 */

#ifndef TESTS_CONTROLS_LABEL_TLABELLINK_H_
#define TESTS_CONTROLS_LABEL_TLABELLINK_H_
#include "TLabels.h"
class TLabelLink: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TLabelLink();
	~TLabelLink();
	void CreateControl(WComposite *parent);
	WFillLayoutVertical layout;
	WLabel link[2];
};

#endif /* TESTS_CONTROLS_LABEL_TLABELLINK_H_ */
