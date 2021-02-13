/*
 * TTextDemo.h
 *
 *  Created on: 12 mai 2020
 *      Author: azeddine
 */

#ifndef TESTS_CONTROLS_TEXTEDIT_TTEXTEDITDEMO_H_
#define TESTS_CONTROLS_TEXTEDIT_TTEXTEDITDEMO_H_
#include "TTextEdits.h"
class TTextEditDemo : public CompositeTreeItem {
public:
	WGridLayout layout;
	WTextEdit text[4];
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
};

#endif /* TESTS_CONTROLS_TEXTEDIT_TTEXTEDITDEMO_H_ */
