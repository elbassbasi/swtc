/*
 * TrayDemo.cpp
 *
 *  Created on: 1 juin 2021
 *      Author: Azeddine
 */

#include "TTrayDemo.h"
void TTrayDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TTrayDemo());
}
TTrayDemo::TTrayDemo() {
}

TTrayDemo::~TTrayDemo() {
}

void TTrayDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->SetLayout(layout);
	layout.numColumns = 2;
	MFrame *frame = (MFrame*) parent->GetFrame();
	menu.CreatePopUp(frame);
	char txt[30];

	for (int i = 0; i < 8; i++) {
		snprintf(txt, sizeof(txt), "Item %d", i);
		menu.AppendItem(txt).SetId(i);
	}
	trayItem.Create(this);
	tooltipText.CreateNormal(this);
	tooltipText.SetLayoutData(WGridData(W_GRID_FILL_HORIZONTAL));
	showtooltip.CreatePush(this, "show tooltip text");
	showtooltip.SetAction(this, W_ACTION(TTrayDemo::OnShowTooltip));
	show.CreatePush(this, "show");
	show.SetAction(this, W_ACTION(TTrayDemo::OnShowTray));
	show.SetLayoutData(WGridData(W_GRID_FILL_HORIZONTAL, layout.numColumns, 1));
}
bool TTrayDemo::OnShowTray(WEvent &e) {
	if (trayItem.GetVisible()) {
		trayItem.SetVisible(false);
		show.SetText("show");
	} else {
		trayItem.SetVisible(true);
		show.SetText("hide");
	}
	return false;
}
bool TTrayDemo::OnShowTooltip(WEvent &e) {
	trayItem.SetToolTipText(tooltipText.GetText());
	return false;
}
bool TTrayItem::Create(TTrayDemo *parent) {
	this->parent = parent;
	WTray *tray = WApp::Get()->GetDefaultToolkit()->GetSystemTray();
	if (tray != 0) {

		WTrayItem::Create(tray, W_NONE);
		this->SetToolTipText("SWT TrayItem");
		MFrame *frame = (MFrame*) parent->GetFrame();
		this->SetImage(frame->GetImage16(IMAGELIST_FLDR));
	}
	return true;
}
bool TTrayItem::OnSelection(WEvent &e) {
	return false;
}

bool TTrayItem::OnDefaultSelection(WEvent &e) {
	return false;
}

bool TTrayItem::OnMenuDetect(WEvent &e) {
	return false;
}

bool TTrayItem::OnShow(WEvent &e) {
	return false;
}

bool TTrayItem::OnHide(WEvent &e) {
	return false;
}
