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
void TCoolBarDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->SetLayout(layout);
	layout.marginHeight= layout.marginWidth=0;
	InitCoolBar(&this->coolbar[0], this->toolbar, W_HORIZONTAL);
	InitCoolBar(&this->coolbar[1], &this->toolbar[3], W_VERTICAL);
	/* image */
	WToolItem item;
	toolbar[7].Create(&this->coolbar[1], W_VERTICAL);
	toolbar[7].SetImageList(MShell::GetImageList16_(this));
	toolbar[7].Append(item, 0, W_PUSH).SetImage(0);
	toolbar[7].Append(item, 0, W_PUSH).SetImage(1);
	toolbar[7].AppendSeparator(item);
	toolbar[7].Append(item, 0, W_CHECK).SetImage(2);
	toolbar[7].Append(item, 0, W_CHECK).SetImage(3);
	toolbar[7].AppendSeparator(item);
	toolbar[7].Append(item, 0, W_RADIO).SetImage(4);
	toolbar[7].Append(item, 0, W_RADIO).SetImage(5);
	toolbar[7].AppendSeparator(item);
	toolbar[7].Append(item, 0, W_DROP_DOWN).SetImage(0);
	toolbar[7].Append(item, 0, W_DROP_DOWN).SetImage(1);
	/** resize to toolbar */
	WCoolItem ti;
	WSize sz;
	toolbar[7].ComputeSize(sz, W_DEFAULT, W_DEFAULT);
	toolbar[7].SetSize(sz);
	coolbar[1].AppendItem(ti);
	ti.ComputeSize(sz, sz.width, sz.height);
	ti.SetControl(&toolbar[7]);
	ti.SetSize(sz);
}
void TCoolBarDemo::InitCoolBar(WCoolBar *coolbar, WToolBar *toolbars,
		int style) {
	WCoolItem item;
	WToolItem ti;
	WSize sz;
	coolbar->Create(this, style);
	for (int idxCoolItem = 0; idxCoolItem < 3; ++idxCoolItem) {
		toolbars[idxCoolItem].Create(coolbar, style);
		toolbars[idxCoolItem].SetImageList(MShell::GetImageList16_(this));
		for (int idxItem = 0; idxItem < 3; ++idxItem) {
			toolbars[idxCoolItem].Append(ti, 0, W_DROP_DOWN);
			ti.SetImage(idxItem + 2);
			//toolbars[idxCoolItem].AppendSeparator(ti);
		}
		/** resize to toolbar */
		toolbars[idxCoolItem].ComputeSize(sz, W_DEFAULT, W_DEFAULT);
		toolbars[idxCoolItem].SetSize(sz);
		coolbar->AppendItem(item);
		item.ComputeSize(sz, sz.width, sz.height);
		item.SetControl(&toolbar[idxCoolItem]);
		item.SetSize(sz);
	}
}
