/*
 * TTextEditor.h
 *
 *  Created on: 6 sept. 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_CANVAS_TTEXTEDITOR_H_
#define TESTS_WIDGETS_CANVAS_TTEXTEDITOR_H_
#include "TCanvass.h"
class TTextEditor : public CanvasTreeItem {
public:
	WMenu menu;
	char text[0x100];
	int length;
	TTextEditor();
	virtual ~TTextEditor();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
private:
	void Add(int c);
protected:
	bool OnPaint(WPaintEvent &e);
	bool OnKeyDown(WKeyEvent &e);
	bool OnMouseDown(WMouseEvent &e);
	bool OnMouseUp(WMouseEvent &e);
};

#endif /* TESTS_WIDGETS_CANVAS_TTEXTEDITOR_H_ */
