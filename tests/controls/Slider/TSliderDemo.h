/*
 * TSliderDemo.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_SLIDER_TSLIDERDEMO_H_
#define TESTS_CONTROLS_SLIDER_TSLIDERDEMO_H_
#include "TSliders.h"
class TSliderDemo: public CompositeTreeItem {
public:
	static const int length = 4;
	static int styles[length];
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	WGridLayout layout;
	WLabel label[length];
	WSlider slider[length];
protected:
	void SetLabelText(wuint index);
	bool OnNotifySelection(WEvent &e);
};

#endif /* TESTS_CONTROLS_SLIDER_TSLIDERDEMO_H_ */
