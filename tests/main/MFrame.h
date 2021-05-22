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
	bool OnNotifySelection(WEvent &e);
	void UpdateSize();
public:
    bool NewCustomShell();
    bool NewPlatformShell();
	bool Exit();
	bool MessageBox();
	bool DirectoryDialog();
	bool ColorDialog();
	bool FontDialog();
	bool FileDialog();
};
#endif /* TESTS_MAIN_MFRAME_H_ */
