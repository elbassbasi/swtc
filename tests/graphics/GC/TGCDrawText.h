/*
 * Name:        TGCDrawText.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2021-01-23
 * Copyright:   (c) 2021 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef TESTS_GRAPHICS_GC_TGCDRAWTEXT_H_
#define TESTS_GRAPHICS_GC_TGCDRAWTEXT_H_
#include "TGCs.h"
class TGCDrawText: public CanvasTreeItem {
public:
	bool advanced;
	static void Registre(WTreeItem &parent);
	TGCDrawText(bool advanced);
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
};
#endif /* TESTS_GRAPHICS_GC_TGCDRAWTEXT_H_ */
