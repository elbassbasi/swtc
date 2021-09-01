/*
 * TCaretDemo.h
 *
 *  Created on: 21 sept. 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_CARET_TCARETDEMO_H_
#define TESTS_WIDGETS_CARET_TCARETDEMO_H_
#include "TCarets.h"
class TCaretDemo : public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TCaretDemo();
	~TCaretDemo();
	void CreateControl(WComposite *parent);
protected:
	bool OnMouseUp(WMouseEvent &e);
public:
	WCaret caret;
};

#endif /* TESTS_WIDGETS_CARET_TCARETDEMO_H_ */
