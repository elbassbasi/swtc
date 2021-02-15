/*
 * TCursor.h
 *
 *  Created on: 15 févr. 2021
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_GRAPHICS_CURSOR_TCURSORDEMO_H_
#define TESTS_GRAPHICS_CURSOR_TCURSORDEMO_H_
#include "TCursors.h"
class TCursorDemo: public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TCursorDemo();
	~TCursorDemo();
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
	WCursor cursor;
};

#endif /* TESTS_GRAPHICS_CURSOR_TCURSORDEMO_H_ */
