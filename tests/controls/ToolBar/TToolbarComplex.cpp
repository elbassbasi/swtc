/*
 * TToolbarComplex.cpp
 *
 *  Created on: 22 sept. 2020
 *      Author: azeddine
 */

#include "TToolbarComplex.h"
void TToolbarComplex::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Complexe", new TToolbarComplex());
}

TToolbarComplex::TToolbarComplex() {
}

TToolbarComplex::~TToolbarComplex() {
}

void TToolbarComplex::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	CreateToolbar();
}

void TToolbarComplex::CreateToolbar() {
	WToolItem item;
	toolBar.Create(this, W_HORIZONTAL);

	// Create push buttons
	CreateToolItem(item, W_PUSH, "Button One", "This is button one");
	item.SetDataAsID(ACTION_BUTTON1);

	CreateToolItem(item, W_PUSH, "Button Two", "This is button two");
	item.SetDataAsID(ACTION_BUTTON1);

	toolBar.AppendSeparator(item);

	// Create "check" buttons
	CreateToolItem(item, W_CHECK, "Check One", "This is check one");
	item.SetDataAsID(ACTION_CHECK1);

	CreateToolItem(item, W_CHECK, "Check Two", "This is check two");
	item.SetDataAsID(ACTION_CHECK2);

	toolBar.AppendSeparator(item);

	// Create "radio" buttons
	CreateToolItem(item, W_RADIO, "Radio One", "This is radio one");
	item.SetDataAsID(ACTION_RADIO1);

	CreateToolItem(item, W_RADIO, "Radio Two", "This is radio two");
	item.SetDataAsID(ACTION_RADIO2);

	toolBar.AppendSeparator(item);

	// Create dropdowns
	/*CreateToolItem(item, W_DROP_DOWN, "Dropdown One", "This is dropdown one");
	DropdownSelectionListener listenerOne = new DropdownSelectionListener(item);
	listenerOne.add("Option One for One");
	listenerOne.add("Option Two for One");
	listenerOne.add("Option Three for One");
	item.addSelectionListener(listenerOne);

	CreateToolItem(item, W_DROP_DOWN, "Dropdown Two", "This is dropdown two");
	DropdownSelectionListener listenerTwo = new DropdownSelectionListener(item);
	listenerTwo.add("Option One for Two");
	listenerTwo.add("Option Two for Two");
	listenerTwo.add("Option Three for Two");
	item.addSelectionListener(listenerTwo);*/
}

void TToolbarComplex::CreateToolItem(WToolItem &item, int type,
		const char *text, const char *toolTipText) {
	toolBar.Append(item,text ,type);
	//item.SetImage(image);
	//item.SetHotImage(hotImage);
	//item.SetToolTipText(toolTipText);
}

bool TToolbarComplex::OnNotify(WEvent &e) {
	return true;
}
