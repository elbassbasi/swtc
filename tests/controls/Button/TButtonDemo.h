/*
 * TButtonStyles.h
 *
 *  Created on: 10 mai 2020
 *      Author: azeddine
 */

#ifndef TESTS_CONTROLS_BUTTON_TBUTTONDEMO_H_
#define TESTS_CONTROLS_BUTTON_TBUTTONDEMO_H_
#include "TButtons.h"
class TButtonDemo: public CompositeTreeItem {
public:
	static const int length = 5;
	static int styles[6];
	static const char *names[6];
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	WGridLayout layout;
	WButton btn[6][length];
protected:
	bool OnNotifySelection(WEvent &e);
};

#endif /* TESTS_CONTROLS_BUTTON_TBUTTONDEMO_H_ */
