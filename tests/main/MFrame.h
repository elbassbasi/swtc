/*
 * TShell.h
 *
 *  Created on: 8 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_MAIN_MFRAME_H_
#define TESTS_MAIN_MFRAME_H_
#include "MTreeView.h"
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
class MFrame: public WFrame {
private:
    friend class MApp;
    friend class MTreeView;
    static WMenuItems menuitems[];
	MTreeView panel;
	WMenu menu;
	WSash sash;
	int width;
	WControl *control;
public:
	MFrame();
	~MFrame();
	bool Create(WToolkit* toolkit);
    WImageList* GetImageList16();
    WImageList* GetImageList32();
    WImage* GetImage16(int index);
    WImage* GetImage32(int index);
    const char* GetExePath();
protected:
	bool OnLayout(WEvent &e);
	bool OnSashSelection(WEvent &e);
	void UpdateSize();
public:
    bool NewCustomShell(WEvent &e);
    bool NewPlatformShell(WEvent &e);
	bool Exit(WEvent &e);
	bool MessageBox(WEvent &e);
	bool DirectoryDialog(WEvent &e);
	bool ColorDialog(WEvent &e);
	bool FontDialog(WEvent &e);
	bool FileDialog(WEvent &e);
};
#endif /* TESTS_MAIN_MFRAME_H_ */
