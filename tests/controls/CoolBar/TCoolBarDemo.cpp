/*
 * TCoolBarDemo.cpp
 *
 *  Created on: 12 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TCoolBarDemo.h"

void TCoolBarDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TCoolBarDemo());
}
const char *asCoolItemSection[] = { "File", "Formatting", "Search" };
TToolBarDemoItems0 TCoolBarDemo::items[] = { //
		{ W_PUSH, 0, "Push" }, //
				{ W_CHECK, 1, "Check" }, //
				{ W_RADIO, 2, "Radio" }, //
				{ W_SEPARATOR, 3, 0 }, //
				{ W_DROP_DOWN, 4, "Dropdown" }, //
				{ 0, 0, 0 }, //

		};
void TCoolBarDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	layout.numColumns = 3;
	this->SetLayout(layout);
	layout.marginHeight = layout.marginWidth = 0;
	CreateCoolBar(0, W_HORIZONTAL);
	CreateCoolBar(1, W_VERTICAL);
}
void TCoolBarDemo::CreateToolBar(WToolBar &toolbar, int style, bool set_text,
		bool set_image) {
	TToolBarDemoItems0 *item = TCoolBarDemo::items;
	WToolItem toolitem;
	while (item->style != 0) {
		toolbar.Append(toolitem, item->style);
		if (set_text)
			toolitem.SetText(item->text);
		if (set_image)
			toolitem.SetImage(item->image);
		item++;
	}
}
void TCoolBarDemo::CreateCoolBar(int index, int style) {
	WCoolItem item;
	WToolItem ti;
	WSize sz;
	WCoolBar *coolbar = &this->coolbar[index];
	coolbar->Create(this, style);
	if (style & W_HORIZONTAL) {
		coolbar->SetLayoutData(
				WGridData(W_GRID_FILL_HORIZONTAL, layout.numColumns, 1));
	}
	for (int i = 0; i < num_toolbar; ++i) {
		WToolBar *toolbar = &this->toolbars[index][i];
		toolbar->Create(coolbar, style);
		CreateToolBar(*toolbar, style, false, true);
		toolbar->SetImageList(MShell::GetImageList16_(this));
		/** resize to toolbar */
		toolbar->ComputeSize(sz, W_DEFAULT, W_DEFAULT);
		toolbar->SetSize(sz);
		coolbar->AppendItem(item);
		item.ComputeSize(sz, sz.width, sz.height);
		item.SetControl(toolbar);
		item.SetSize(sz);
	}
}
