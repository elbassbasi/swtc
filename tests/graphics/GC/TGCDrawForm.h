/*
 * Name:        TGCForm.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2021-01-09
 * Copyright:   (c) 2021 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef TESTS_GRAPHICS_GC_TGCDRAWFORM_H_
#define TESTS_GRAPHICS_GC_TGCDRAWFORM_H_
#include "TGCs.h"
class TGCDrawForm: public CanvasTreeItem {
public:
	bool advanced;
	static void Registre(WTreeItem &parent);
	TGCDrawForm(bool advanced);
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
};
#endif /* TESTS_GRAPHICS_GC_TGCDRAWFORM_H_ */
