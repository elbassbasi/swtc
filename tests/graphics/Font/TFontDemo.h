/*
 * TFontDemo.h
 *
 *  Created on: 8 févr. 2021
 *      Author: azeddine
 */

#ifndef TESTS_GRAPHICS_FONT_TFONTDEMO_H_
#define TESTS_GRAPHICS_FONT_TFONTDEMO_H_
#include "TFonts.h"
class TFontDemo: public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TFontDemo();
	virtual ~TFontDemo();
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
public:
	WFont fonts[10];
};
#endif /* TESTS_GRAPHICS_FONT_TFONTDEMO_H_ */
