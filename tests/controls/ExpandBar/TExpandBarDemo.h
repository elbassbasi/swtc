/*
 * ExpandBarDemo.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_EXPANDBAR_TEXPANDBARDEMO_H_
#define TESTS_CONTROLS_EXPANDBAR_TEXPANDBARDEMO_H_
#include "TExpandBars.h"
class TExpandBarDemo : public WExpandBar , public ITreeItem {
public:
	static void Registre(WTreeItem &parent);
	WControl* GetControl(WComposite *parent);
	void CreateControl(WComposite *parent);
	WComposite composite[3];
	WGridLayout layout[3];
	WButton button[4];
	WLabel label[12];
	WSlider scale;
	WSpinner spinner;
	WSlider slider;
};

#endif /* TESTS_CONTROLS_EXPANDBAR_TEXPANDBARDEMO_H_ */
