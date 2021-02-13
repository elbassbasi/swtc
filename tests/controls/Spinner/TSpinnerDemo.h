/*
 * TSpinnerDemo.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_SPINNER_TSPINNERDEMO_H_
#define TESTS_CONTROLS_SPINNER_TSPINNERDEMO_H_
#include "TSpinners.h"
class TSpinnerDemo: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	WGridLayout layout;
	WSpinner spinner;
};

#endif /* TESTS_CONTROLS_SPINNER_TSPINNERDEMO_H_ */
