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
				{ W_RADIO, 2, "Radio1" }, //
				{ W_RADIO, 3, "Radio2" }, //
				{ W_SEPARATOR, -1, 0 }, //
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
	layout.numColumns = 3;
	this->SetLayout(layout);
	/* text */
	CreateToolBar(0, W_HORIZONTAL, true, false);
	bar[0].SetLayoutData(WGridData(0, layout.numColumns, 1));
	CreateToolBar(1, W_HORIZONTAL, false, true);
	bar[1].SetLayoutData(WGridData(0, layout.numColumns, 1));
	/* image */
	CreateToolBar(2, W_VERTICAL, true, false);
	CreateToolBar(3, W_VERTICAL, false, true);
	MFrame *frame = (MFrame*) GetFrame();
	bar[1].SetImageList(frame->GetImageList16());
	bar[3].SetImageList(frame->GetImageList32());
	label.Create(this, W_WRAP);
	label.SetLayoutData(WGridData(W_GRID_FILL_BOTH));
}

bool TToolBarDemo::OnNotifyItemSelection(WEvent &e) {
	if (WToolBar::IsToolBar(e.widget)) {
		OnToolBarSelection(reinterpret_cast<WToolBarEvent&>(e));
	}
	return WComposite::OnNotifyItemSelection(e);
}

bool TToolBarDemo::OnToolBarSelection(WToolBarEvent &e) {
	if ((e.detail & W_ARROW) == 0) {
		char txt[100];
		WToolItem *item = e.item;
		WString text = item->GetText();
		int image = item->GetImage();
		snprintf(txt, sizeof(txt), "text = %s || image = %d",
				text.GetCharsNotNull(), image);
		label.SetText(txt);
	}
	return true;
}
