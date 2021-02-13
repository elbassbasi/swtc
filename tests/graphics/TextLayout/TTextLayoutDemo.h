/*
 * TTextLayoutDemo.h
 *
 *  Created on: 26 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_GRAPHICS_TEXTLAYOUT_TTEXTLAYOUTDEMO_H_
#define TESTS_GRAPHICS_TEXTLAYOUT_TTEXTLAYOUTDEMO_H_
#include "TTextLayouts.h"
class TTextLayoutDemo : public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
	WFont font[3];
};

#endif /* TESTS_GRAPHICS_TEXTLAYOUT_TTEXTLAYOUTDEMO_H_ */
