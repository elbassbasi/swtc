/*
 * TClipboard.h
 *
 *  Created on: 5 oct. 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_DND_TCLIPBOARD_H_
#define TESTS_WIDGETS_DND_TCLIPBOARD_H_
#include "TDnd.h"
class TClipboard: public CompositeTreeItem {
public:
	TClipboard();
	~TClipboard();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	WGridLayout layout;
	WTextEdit text;
	WButton copy;
	WButton paste;
protected:
	bool OnNotify(WEvent &e);
	bool OnCopy();
	bool OnPaste();
};

#endif /* TESTS_WIDGETS_DND_TCLIPBOARD_H_ */
