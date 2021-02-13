/*
 * TPathDemo.h
 *
 *  Created on: 23 oct. 2020
 *      Author: azeddine
 */

#ifndef SWTTEST_GRAPHICS_PATH_TPATHDEMO_H_
#define SWTTEST_GRAPHICS_PATH_TPATHDEMO_H_
#include "TPaths.h"
class TPathDemo : public CanvasTreeItem {
public:
	TPathDemo();
	virtual ~TPathDemo();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
	bool OnTimer(WTimerEvent &e);
	WFont font;
	WTransform transform;
	int rotate;
};

#endif /* SWTTEST_GRAPHICS_PATH_TPATHDEMO_H_ */
