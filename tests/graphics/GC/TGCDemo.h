/*
 * Name:        TGCDemo.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-05-10
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef TESTS_GRAPHICS_GC_TGCDEMO_H_
#define TESTS_GRAPHICS_GC_TGCDEMO_H_
#include "TGCs.h"
class TGCDemo : public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool OnPaint(WPaintEvent &e);
};

#endif /* TESTS_GRAPHICS_GC_TGCDEMO_H_ */
