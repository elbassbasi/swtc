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

void TToolBarDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->SetLayout(layout);
	/* text */
	bar[0].Create(this, W_HORIZONTAL);
	WToolItem item;
	bar[0].Append(item, "Push1", W_PUSH);
	bar[0].Append(item, "Push2", W_PUSH);
	bar[0].AppendSeparator(item);
	bar[0].Append(item, "Check1", W_CHECK);
	bar[0].Append(item, "Check2", W_CHECK);
	bar[0].AppendSeparator(item);
	bar[0].Append(item, "Radio1", W_RADIO);
	bar[0].Append(item, "Radio2", W_RADIO);
	bar[0].AppendSeparator(item);
	bar[0].Append(item, "Dropdown1", W_DROP_DOWN);
	bar[0].Append(item, "Dropdown2", W_DROP_DOWN);
	/* image */
	bar[1].Create(this, W_VERTICAL);
	bar[1].SetImageList(MShell::GetImageList16_(this));
	bar[1].Append(item, 0, W_PUSH).SetImage(0);
	bar[1].Append(item, 0, W_PUSH).SetImage(1);
	bar[1].AppendSeparator(item);
	bar[1].Append(item, 0, W_CHECK).SetImage(2);
	bar[1].Append(item, 0, W_CHECK).SetImage(3);
	bar[1].AppendSeparator(item);
	bar[1].Append(item, 0, W_RADIO).SetImage(0);
	bar[1].Append(item, 0, W_RADIO).SetImage(1);
	bar[1].AppendSeparator(item);
	bar[1].Append(item, 0, W_DROP_DOWN).SetImage(2);
	bar[1].Append(item, 0, W_DROP_DOWN).SetImage(3);
}
