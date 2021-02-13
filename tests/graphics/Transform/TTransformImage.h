/*
 * TTransformImage.h
 *
 *  Created on: 8 févr. 2021
 *      Author: azeddine
 */

#ifndef TESTS_GRAPHICS_TRANSFORM_TTRANSFORMIMAGE_H_
#define TESTS_GRAPHICS_TRANSFORM_TTRANSFORMIMAGE_H_
#include "TTransforms.h"
class TTransformImage: public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TTransformImage();
	~TTransformImage();
	void DrawText(WGraphics &gc, int x, int y);
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
	WImage image;
	WFont font;
};

#endif /* TESTS_GRAPHICS_TRANSFORM_TTRANSFORMIMAGE_H_ */
