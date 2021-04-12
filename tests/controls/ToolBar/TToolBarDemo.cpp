/*
 * TToolBarDemo.cpp
 *
 *  Created on: 12 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TToolBarDemo.h"

void TToolBarDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TToolBarDemo());
}
TToolBarDemoItems TToolBarDemo::items[] = { //
		{ W_PUSH, 0, "Push" }, //
				{ W_CHECK, 1, "Check" }, //
				{ W_RADIO, 2, "Radio" }, //
				{ W_SEPARATOR, 3, 0 }, //
				{ W_DROP_DOWN, 4, "Dropdown" }, //
				{ 0, 0, 0 }, //

		};
void TToolBarDemo::CreateToolBar(int index, int style, bool set_text,
		bool set_image) {
	bar[index].Create(this, style);
	TToolBarDemoItems *item = TToolBarDemo::items;
	WToolItem toolitem;
	while (item->style != 0) {
		bar[index].Append(toolitem, item->style);
		if (set_text)
			toolitem.SetText(item->text);
		if (set_image)
			toolitem.SetImage(item->image);
		if (item->style & W_SEPARATOR) {
			if (index == 0)
				SetToolCombo(index, toolitem);
		}
		if (item->style & W_DROP_DOWN) {
			SetToolMenu(index, toolitem);
		}
		item++;
	}
}

void TToolBarDemo::SetToolMenu(int index, WToolItem &item) {
	WMenuItem menuitem, root;
	char txt[30];
	menu[index].CreatePopUp(&bar[index]);
	menu[index].GetRoot(root);
	for (int i = 0; i < 5; i++) {
		snprintf(txt, sizeof(txt), "menu item %d", i);
		root.AppendItem(menuitem, txt);
	}
	item.SetMenu(&menu[index]);
}

void TToolBarDemo::SetToolCombo(int index, WToolItem &item) {
	WComboItem comboitem;
	WSize size;
	char txt[30];
	combo.Create(&bar[index], W_READ_ONLY);
	for (int i = 0; i < 4; i++) {
		snprintf(txt, sizeof(txt), "combo Item %d", i);
		combo.Add(comboitem, txt);
	}
	combo.Pack();
	combo.GetSize(size);
	item.SetWidth(size.width);
	item.SetControl(combo);
}
void TToolBarDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	layout.numColumns = 2;
	this->SetLayout(layout);
	/* text */
	CreateToolBar(0, W_HORIZONTAL, true, false);
	bar[0].SetLayoutData(WGridData(0, layout.numColumns, 1));
	CreateToolBar(1, W_HORIZONTAL, true, true);
	bar[1].SetLayoutData(WGridData(0, layout.numColumns, 1));
	/* image */
	CreateToolBar(2, W_VERTICAL, false, true);
	CreateToolBar(3, W_VERTICAL, false, true);
	bar[2].SetImageList(MShell::GetImageList16_(this));
	bar[3].SetImageList(MShell::GetImageList32_(this));
}

