/*
 * TProgressBarDemo.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TProgressBarDemo.h"

void TProgressBarDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TProgressBarDemo());
}

void TProgressBarDemo::CreateControl(WComposite *parent) {
	WGridData data;
	this->Create(parent, 0);
	SetLayout(&layout);
	progress.Create(this, 0);
	progress.SetMaximum(100);
	progress.SetLayoutData(WGridData(WGridData::FILL_HORIZONTAL));
	start.Create(this, W_PUSH);
	start.SetText("Start");
	start.SetLayoutData(WGridData(WGridData::FILL_HORIZONTAL));
	stop.Create(this, W_PUSH);
	stop.SetText("Stop");
	stop.SetEnabled(false);
	stop.SetLayoutData(WGridData(WGridData::FILL_HORIZONTAL));
	vertical.Create(this, W_VERTICAL);
	vertical.SetLayoutData(WGridData(WGridData::FILL_VERTICAL));
	vertical.SetMaximum(100);
}

bool TProgressBarDemo::OnTimer(WTimerEvent &e) {
	int selection = progress.GetSelection();
	if (selection >= 100) {
		progress.SetState(W_PAUSED);
		stop.SetEnabled(false);
	} else {
		progress.SetSelection(selection + 1);
		vertical.SetSelection(selection + 1);
	}
	return true;
}

bool TProgressBarDemo::OnNotify(WEvent &e) {
	if (e.widget == &this->start) {
		SetTimer(500);
		start.SetEnabled(false);
		stop.SetEnabled(true);
	}
	if (e.widget == &stop) {
		KillTimer();
		stop.SetEnabled(false);
		start.SetEnabled(true);
	}
	return false;
}
