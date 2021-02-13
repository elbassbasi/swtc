/*
 * MMenu.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_MAIN_MMENU_H_
#define TESTS_MAIN_MMENU_H_
#include <swtp.h>
class MMenu: public WMenu {
private:
	static WMenuItems menuitems[];
protected:
	bool OnItemSelection(WMenuEvent &e);
public:
	bool Create(WShell *parent);
    bool NewCustomShell();
    bool NewPlatformShell();
	bool Exit();
	bool MessageBox();
	bool DirectoryDialog();
	bool ColorDialog();
	bool FontDialog();
	bool FileDialog();
};

#endif /* TESTS_MAIN_MMENU_H_ */


