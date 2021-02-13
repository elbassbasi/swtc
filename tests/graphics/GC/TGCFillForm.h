/*
 * Name:        TGCForm.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2021-01-20
 * Copyright:   (c) 2021 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef TESTS_GRAPHICS_GC_TGCFILLFORM_H_
#define TESTS_GRAPHICS_GC_TGCFILLFORM_H_
#include "TGCs.h"
class TGCFillForm: public CanvasTreeItem {
public:
	bool advanced;
	static void Registre(WTreeItem &parent);
	TGCFillForm(bool advanced);
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
};
#endif /* TESTS_GRAPHICS_GC_TGCFILLFORM_H_ */
