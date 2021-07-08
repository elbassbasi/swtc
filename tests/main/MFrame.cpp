/*
 * MShell.cpp
 *
 *  Created on: 8 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "MFrame.h"
#include "MApp.h"
WMenuItems MFrame::menuitems[] = { //
				{ W_CASCADE, 0, IMAGELIST_FILE, "File", 0 },	//
				{ W_PUSH, 0, IMAGELIST_ADD, "NewPlatform", W_ACTION(
						MFrame::NewPlatformShell) },	//
				{ W_PUSH, 0, -1, "NewCustom", W_ACTION(MFrame::NewCustomShell) },//
				{ W_SEPARATOR, 0, -1, 0, 0 },	//
				{ W_PUSH, W_ALT | 'Q', -1, "Exit", W_ACTION(MFrame::Exit) },//
				{ W_CASCADE_END, 0, -1, 0, 0 },	//
				{ W_CASCADE, 0, -1, "Dialogs", 0 },	//
				{ W_PUSH, 0, -1, "MessageBox", W_ACTION(MFrame::MessageBox) },//
				{ W_PUSH, 0, -1, "FileDialog", W_ACTION(MFrame::FileDialog) },//
				{ W_PUSH, 0, -1, "DirectoryDialog", W_ACTION(
						MFrame::DirectoryDialog) },	//
				{ W_PUSH, 0, -1, "FontDialog", W_ACTION(MFrame::FontDialog) },//
				{ W_PUSH, 0, -1, "ColorDialog", W_ACTION(MFrame::ColorDialog) },//
				{ W_CASCADE_END, 0, -1, 0, 0 },	//
				{ W_CASCADE, 0, -1, "Source", 0 },	//
				{ W_CASCADE_END, 0, -1, 0, 0 },	//
				{ W_CASCADE, 0, -1, "Radio", 0 },	//
				{ W_RADIO, 0, -1, "Radio0", 0 },	//
				{ W_RADIO, 0, -1, "Radio1", 0 },	//
				{ W_RADIO, 0, -1, "Radio2", 0 },	//
				{ W_SEPARATOR, 0, -1, 0, 0 },	//
				{ W_CHECK, 0, -1, "Check0", 0 },	//
				{ W_CHECK, 0, -1, "Check1", 0 },	//
				{ W_CHECK, 0, -1, "Check2", 0 },	//
				{ W_CASCADE_END, 0, -1, 0, 0 },	//
				{ 0, 0, 0, 0 },	//
		};
MFrame::MFrame() {
	this->width = 200;
	this->control = 0;
}
MFrame::~MFrame() {
}
bool MFrame::Create(WToolkit *toolkit) {
	if (WFrame::Create(toolkit,
			W_FRAME_TRIM | W_DELETE_ON_DISPOSE | W_DISPOSE_ON_CLOSE)) {
		panel.Create(this);
		sash.Create(this, W_VERTICAL);
		menu.CreateBar(this);
		menu.CreateItems(this, MApp::Get()->GetImageList16(), menuitems);
		this->SetMenuBar(menu);
		this->SetBounds(100, 100, 600, 500);
		return true;
	} else
		return false;
}

bool MFrame::OnLayout(WEvent &e) {
	UpdateSize();
	return true;
}
void MFrame::UpdateSize() {
	WRect area;
	GetFrame()->GetClientArea(area);
	panel.SetBounds(area.x, area.y, this->width, area.height - 3);
	sash.SetBounds(this->width, area.y, 3, area.height - 3);
	if (this->control != 0) {
		this->control->SetBounds(this->width + 3, area.y,
				area.width - (this->width + 3) - 3, area.height - 3);
	}
}
bool MFrame::OnNotifySelection(WEvent &e) {
	if (e.widget == &sash) {
		WSashEvent &event = reinterpret_cast<WSashEvent&>(e);
		WRect sashBounds;
		sash.GetBounds(sashBounds);
		int shift = event.bounds.x - sashBounds.x;
		this->width += shift;
		UpdateSize();
		return true;
	}
	return false;
}
WImageList* MFrame::GetImageList16() {
	return MApp::Get()->GetImageList16();
}
WImageList* MFrame::GetImageList32() {
	return MApp::Get()->GetImageList32();
}
WImage* MFrame::GetImage16(int index) {
	return MApp::Get()->GetImage16(index);
}
WImage* MFrame::GetImage32(int index) {
	return MApp::Get()->GetImage32(index);
}
const char* MFrame::GetExePath() {
	return MApp::Get()->GetExecutablePath();
}
/*
 * action method
 */
bool MFrame::Exit(WEvent &e) {
	this->Close();
	return true;
}
bool MFrame::NewPlatformShell(WEvent &e) {
	MApp::Get()->NewPlatformShell();
	return true;
}
bool MFrame::NewCustomShell(WEvent &e) {
	MApp::Get()->NewCustomShell();
	return true;
}

bool MFrame::MessageBox(WEvent &e) {
	WMessageBox(this, W_YES | W_NO, "Message Box", "Test Message Box");
	return true;
}

bool MFrame::DirectoryDialog(WEvent &e) {
	return true;
}

bool MFrame::ColorDialog(WEvent &e) {
	WColorDialog dialog;
	dialog.Open(this, "Select Color");
	return true;
}

bool MFrame::FontDialog(WEvent &e) {
	return true;
}

bool MFrame::FileDialog(WEvent &e) {
	return true;
}
