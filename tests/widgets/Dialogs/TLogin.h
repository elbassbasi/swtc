/*
 * TLogin.h
 *
 *  Created on: 1 oct. 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_DIALOGS_TLOGIN_H_
#define TESTS_WIDGETS_DIALOGS_TLOGIN_H_
#include "TDialogs.h"
class TLogin : public CompositeTreeItem {
public:
	TLogin();
	virtual ~TLogin();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
protected:
	bool OnNotifySelection(WEvent &e);
public:
	WGridLayout layout;
	WLabel label[3];
	WTextEdit text[2];
	WButton button;
};

#endif /* TESTS_WIDGETS_DIALOGS_TLOGIN_H_ */
