/*
 * TThemeDemo.h
 *
 *  Created on: 11 mai 2020
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_THEME_TTHEMEDEMO_H_
#define TESTS_WIDGETS_THEME_TTHEMEDEMO_H_
#include "TThemes.h"
class TThemeDemo: public CanvasTreeItem {
public:
	static int states[];
	static const char* texts[];
	int clazz;
	int style;
	TThemeDemo(int clazz, int style);
	static void Registre(WTreeItem &parent);
	bool OnPaint(WPaintEvent &e);
};

#endif /* TESTS_WIDGETS_THEME_TTHEMEDEMO_H_ */
