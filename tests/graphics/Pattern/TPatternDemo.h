/*
 * TPatternDemo.h
 *
 *  Created on: 8 févr. 2021
 *      Author: azeddine
 */

#ifndef TESTS_GRAPHICS_PATTERN_TPATTERNDEMO_H_
#define TESTS_GRAPHICS_PATTERN_TPATTERNDEMO_H_
#include "TPatterns.h"
class TPatternDemo: public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TPatternDemo();
	~TPatternDemo();
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
	WPattern pattern;
};
#endif /* TESTS_GRAPHICS_PATTERN_TPATTERNDEMO_H_ */
