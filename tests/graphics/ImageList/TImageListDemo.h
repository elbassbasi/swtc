/*
 * TImageListDemo.h
 *
 *  Created on: 15 févr. 2021
 *      Author: azeddine
 */

#ifndef TESTS_GRAPHICS_IMAGELIST_TIMAGELISTDEMO_H_
#define TESTS_GRAPHICS_IMAGELIST_TIMAGELISTDEMO_H_
#include "TImageLists.h"
class TImageListDemo: public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TImageListDemo();
	~TImageListDemo();
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
};

#endif /* TESTS_GRAPHICS_IMAGELIST_TIMAGELISTDEMO_H_ */
