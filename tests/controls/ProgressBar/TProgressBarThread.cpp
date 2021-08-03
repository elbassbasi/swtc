/*
 * TProgressBarThread.cpp
 *
 *  Created on: 19 juin 2021
 *      Author: azeddine
 */

#include "TProgressBarThread.h"
void TProgressBarThread::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Thread", new TProgressBarThread());
}
TProgressBarThread::TProgressBarThread() {
	this->is_started = 0;
	this->selection = 0;
	this->maximum = 100;
}
TProgressBarThread::~TProgressBarThread() {
}
void TProgressBarThread::CreateControl(WComposite *parent) {
	WGridData data;
	this->Create(parent, 0);
	SetLayout(&layout);
	progress.Create(this, 0);
	progress.SetMaximum(100);
	progress.SetLayoutData(WGridData(WGridData::FILL_HORIZONTAL));
	progress.SetMaximum(maximum);
	progress.SetSelection(selection);
	label.CreateLabel(this, "click Start");
	label.SetLayoutData(WGridData(WGridData::FILL_HORIZONTAL));
	start.CreatePush(this, "Start");
	start.SetLayoutData(WGridData(WGridData::FILL_HORIZONTAL));
	start.SetAction(this, W_ACTION(TProgressBarThread::OnStart));
}
void TProgressBarThread::OnNotifyExec(void* args) {
	char txt[30];
	if (this->selection <= this->maximum) {
		progress.SetSelection(selection);
		snprintf(txt, sizeof(txt), "%d%%", this->selection);
		label.SetText(txt);
	}
}

void TProgressBarThread::Run(void *args) {
	while (this->selection <= this->maximum) {
		this->selection++;
		this->AsyncExec(0);
		WThread::Sleep(100);
	}
}

bool TProgressBarThread::OnStart(WEvent &e) {
	if (!this->is_started) {
		for (int i = 0; i < thread_length; i++) {
			threads[i].Create(this,0);
		}
		start.SetEnabled(false);
	}
	return true;
}
