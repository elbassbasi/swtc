/*
 * TComboDemo.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_COMBOBOX_TCOMBODEMO_H_
#define TESTS_CONTROLS_COMBOBOX_TCOMBODEMO_H_
#include "TComboBoxs.h"
class TComboDemo: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	void SetItems(WComboBox& combo);
	void CreateControls(int index,const char* text,wuint64 style);
	static const char* ITEMS[];
	WGridLayout layout;
	WComboBox combo[4];
	WLabel label[4];
	WButton btn[4];
protected:
	bool OnNotify(WEvent &e);
};

#endif /* TESTS_CONTROLS_COMBOBOX_TCOMBODEMO_H_ */
