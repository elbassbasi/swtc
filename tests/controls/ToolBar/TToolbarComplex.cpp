/*
 * TToolbarComplex.cpp
 *
 *  Created on: 22 sept. 2020
 *      Author: azeddine
 */

#include "TToolbarComplex.h"
void TToolbarComplex::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Complex", new TToolbarComplex());
}

TToolbarComplex::TToolbarComplex() {
}

TToolbarComplex::~TToolbarComplex() {
}

void TToolbarComplex::CreateControl(WComposite *parent) {
	this->Create(parent, W_WRAP);
	SetLayout(layout);
	CreateToolbar();
}

void TToolbarComplex::CreateToolbar() {
	WToolItem item;
	WMenuItem menuItem, rootItem;
	WComboItem comboitem;
	WSize size;
	char txt[30];
	toolBar.Create(this, W_HORIZONTAL);
	MFrame *frame = (MFrame*) GetFrame();
	toolBar.SetImageList(frame->GetImageList16());

	// Create push buttons
	CreateToolItem(item, W_PUSH, 0, "Button One", "This is button one");
	item.SetID(ACTION_BUTTON1);

	CreateToolItem(item, W_PUSH, 1, "Button Two", "This is button two");
	item.SetID(ACTION_BUTTON1);

	toolBar.AppendSeparator(item);

	combo.Create(toolBar, W_READ_ONLY);
	for (int i = 0; i < 4; i++) {
		snprintf(txt, sizeof(txt), "Item %d", i);
		combo.Add(comboitem, txt);
	}
	item.SetControlWithPreferredWith(combo);

	// Create "check" buttons
	CreateToolItem(item, W_CHECK, 2, "Check One", "This is check one");
	item.SetID(ACTION_CHECK1);

	CreateToolItem(item, W_CHECK, 3, "Check Two", "This is check two");
	item.SetID(ACTION_CHECK2);

	toolBar.AppendSeparator(item);

	// Create "radio" buttons
	CreateToolItem(item, W_RADIO, 4, "Radio One", "This is radio one");
	item.SetID(ACTION_RADIO1);

	CreateToolItem(item, W_RADIO, 5, "Radio Two", "This is radio two");
	item.SetID(ACTION_RADIO2);

	toolBar.AppendSeparator(item);

	// Create dropdowns
	CreateToolItem(item, W_DROP_DOWN, 1, "Dropdown One",
			"This is dropdown one");
	menu[0].CreatePopUp(&toolBar);
	menu[0].GetRoot(rootItem);
	rootItem.AppendItem(rootItem, "Option One for One");
	rootItem.AppendItem(rootItem, "Option Two for One");
	rootItem.AppendItem(rootItem, "Option Three for One");
	item.SetMenu(&menu[0]);

	CreateToolItem(item, W_DROP_DOWN, 2, "Dropdown Two",
			"This is dropdown two");
	menu[1].CreatePopUp(&toolBar);
	menu[1].GetRoot(rootItem);
	rootItem.AppendItem(rootItem, "Option One for Two");
	rootItem.AppendItem(rootItem, "Option Two for Two");
	rootItem.AppendItem(rootItem, "Option Three for Two");
	item.SetMenu(&menu[1]);
}

void TToolbarComplex::CreateToolItem(WToolItem &item, int type, int image,
		const char *text, const char *toolTipText) {
	toolBar.Append(item, type);
	item.SetImage(image);
	item.SetToolTipText(toolTipText);
}

bool TToolbarComplex::OnNotify(WEvent &e) {
	if (WToolBar::IsToolBar(e.widget)) {
		return OnToolbarSelection(reinterpret_cast<WToolBarEvent&>(e));
	}
	return true;
}

bool TToolbarComplex::OnToolbarSelection(WToolBarEvent &e) {
	WToolItem *toolItem = e.item;
	WToolBar *toolBar = toolItem->GetParent();
	return false;
}
