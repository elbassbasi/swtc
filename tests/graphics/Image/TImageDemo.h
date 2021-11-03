/*
 * TImageDemo.h
 *
 *  Created on: 15 févr. 2021
 *      Author: azeddine
 */

#ifndef TESTS_GRAPHICS_IMAGE_TIMAGEDEMO_H_
#define TESTS_GRAPHICS_IMAGE_TIMAGEDEMO_H_
#include "TImages.h"
class TImageDemo: public CanvasTreeItem {
public:
	WImage streamImage;
	static void Registre(WTreeItem &parent);
	TImageDemo();
	virtual ~TImageDemo();
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
};
#endif /* TESTS_GRAPHICS_IMAGE_TIMAGEDEMO_H_ */
