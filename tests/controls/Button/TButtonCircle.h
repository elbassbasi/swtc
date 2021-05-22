/*
 * TButtonCircle.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef TESTS_CONTROLS_BUTTON_TBUTTONCIRCLE_H_
#define TESTS_CONTROLS_BUTTON_TBUTTONCIRCLE_H_
#include "TButtons.h"
class TButtonCircle: public CompositeTreeItem {
public:
	static const int length = 2;
	static void Registre(WTreeItem &parent);
	TButtonCircle();
	~TButtonCircle();
	void CreateCircleRegion(WRegion &region, int r, int offsetX, int offsetY);
	void CreateControl(WComposite *parent);
	WFillLayoutVertical layout;
	WButton btn[length];
protected:
	bool OnNotifySelection(WEvent &e);
};

#endif /* TESTS_CONTROLS_BUTTON_TBUTTONCIRCLE_H_ */
