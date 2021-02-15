/*
 * TRegionDemo.h
 *
 *  Created on: 15 févr. 2021
 *      Author: azeddine
 */

#ifndef TESTS_GRAPHICS_REGION_TREGIONDEMO_H_
#define TESTS_GRAPHICS_REGION_TREGIONDEMO_H_
#include "TRegions.h"
class TRegionDemo: public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TRegionDemo();
	~TRegionDemo();
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
};

#endif /* TESTS_GRAPHICS_REGION_TREGIONDEMO_H_ */
