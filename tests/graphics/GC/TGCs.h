/*
 * Name:        TGCs.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-05-10
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef TESTS_GRAPHICS_GC_TGCS_H_
#define TESTS_GRAPHICS_GC_TGCS_H_
#include "../TGraphics.h"
class TGCs : public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_GRAPHICS_GC_TGCS_H_ */
