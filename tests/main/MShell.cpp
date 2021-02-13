/*
 * MShell.cpp
 *
 *  Created on: 8 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "MShell.h"
#include "MApp.h"
MShell::MShell() {
}
MShell::~MShell() {
}
bool MShell::Create(WToolkit *toolkit) {
	if (WShell::Create(toolkit,
			W_SHELL_TRIM | W_DELETE_ON_DISPOSE | W_DISPOSE_ON_CLOSE)) {
		panel.Create(this);
		menu.Create(this);
		this->SetBounds(100, 100, 600,500);
		return true;
	} else
		return false;
}

bool MShell::OnLayout(WEvent &e) {
	panel.UpdateSize();
	return true;
}

bool MShell::OnNotify(WEvent &e) {
	if (e.widget == &panel.sash)
		return panel.OnSashSelection(static_cast<WSashEvent&>(e));
	return false;
}

WImageList* MShell::GetImageList16_(WControl *control) {
	return MApp::Get()->GetImageList16();
}

WImageList* MShell::GetImageList32_(WControl *control) {
	return MApp::Get()->GetImageList32();
}

WImage* MShell::GetImage16_(WControl *control, int index) {
	return MApp::Get()->GetImage16(index);
}

WImage* MShell::GetImage32_(WControl *control, int index) {
	return MApp::Get()->GetImage32(index);
}
WImageList* MShell::GetImageList16() {
	return MApp::Get()->GetImageList16();
}
WImageList* MShell::GetImageList32() {
	return MApp::Get()->GetImageList32();
}
WImage* MShell::GetImage16(int index) {
	return MApp::Get()->GetImage16(index);
}
WImage* MShell::GetImage32(int index) {
	return MApp::Get()->GetImage32(index);
}
const char* MShell::GetExePath() {
	return MApp::Get()->GetExecutablePath();
}
