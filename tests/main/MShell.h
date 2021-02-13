/*
 * TShell.h
 *
 *  Created on: 8 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_MAIN_MSHELL_H_
#define TESTS_MAIN_MSHELL_H_
#include "MPanel.h"
#include "MMenu.h"
enum {
	IMAGELIST_FLDR,
	IMAGELIST_FILE,
	IMAGELIST_ADD,
	IMAGELIST_SAVE,
	IMAGELIST_DELETE,
	IMAGELIST_HELP,
	IMAGELIST_LAST,
	IMAGE_LAST,
};
class MShell: public WShell {
private:
    friend class MApp;
	MPanel panel;
	MMenu menu;
public:
	static WImageList* GetImageList16_(WControl *control);
	static WImageList* GetImageList32_(WControl *control);
	static WImage* GetImage16_(WControl *control, int index);
	static WImage* GetImage32_(WControl *control, int index);
	MShell();
	~MShell();
	bool Create(WToolkit* toolkit);
    WImageList* GetImageList16();
    WImageList* GetImageList32();
    WImage* GetImage16(int index);
    WImage* GetImage32(int index);
    const char* GetExePath();
protected:
	bool OnLayout(WEvent &e);
	bool OnNotify(WEvent &e);
};
#endif /* TESTS_MAIN_MSHELL_H_ */
