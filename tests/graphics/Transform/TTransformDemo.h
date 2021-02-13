/*
 * TTransformDemo.h
 *
 *  Created on: 8 févr. 2021
 *      Author: azeddine EL Bassbasi
 */

#ifndef TESTS_GRAPHICS_TRANSFORM_TTRANSFORMDEMO_H_
#define TESTS_GRAPHICS_TRANSFORM_TTRANSFORMDEMO_H_
#include "TTransforms.h"
class TTransformDemo: public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TTransformDemo();
	~TTransformDemo();
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
};

#endif /* TESTS_GRAPHICS_TRANSFORM_TTRANSFORMDEMO_H_ */
