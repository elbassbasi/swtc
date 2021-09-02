/*
 * TTreeDemo.h
 *
 *  Created on: 18 mars 2021
 *      Author: azeddine
 */

#ifndef TESTS_CONTROLS_TREEVIEW_TTREEDEMO_H_
#define TESTS_CONTROLS_TREEVIEW_TTREEDEMO_H_
#include "TTreeViews.h"
class TTreeDemo: public WTreeView, public ITreeItem {
public:
	static const int font_length = 4;
	static const int columns_count = 5;
	static w_color columns_forgrounds[columns_count];
	static w_color columns_backgrounds[columns_count];
	static wuchar columns_fonts[columns_count];
	static WMenuItems menuitems[];
	static void Registre(WTreeItem &parent);
	TTreeDemo();
	~TTreeDemo();
	void CreateControl(WComposite *parent);
	WControl* GetControl(WComposite *parent);
	void CreateColumns();
	void CreateItems();
	void CreateFonts();
protected:
	bool Delete(WEvent& e);
protected:
	WMenu menu;
	WFont *fonts[font_length];
};

#endif /* TESTS_CONTROLS_TREEVIEW_TTREEDEMO_H_ */
