/*
 * TSliderDemo.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_SLIDER_TSLIDERDEMO_H_
#define TESTS_CONTROLS_SLIDER_TSLIDERDEMO_H_
#include "TSliders.h"
class TSliderDemo : public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	WGridLayout layout;
	WSlider slider[2];
};

#endif /* TESTS_CONTROLS_SLIDER_TSLIDERDEMO_H_ */
