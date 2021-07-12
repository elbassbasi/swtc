//
// Created by azeddine on 27/08/2020.
//

#include "MApp.h"
int main(int args, char *argv[]) {
	MApp app(args, argv);
	app.NewPlatformShell();
	return app.Run();
}
/*
 * MApp
 */
const char *images[] = { "fldr", "file", "add", "save", "delete", "help" };
MApp::MApp(int args, char *argv[]) :
		WApp(args, argv) {
	this->theme = 0;
}
MApp::~MApp() {

}
WImageList* MApp::GetImageList16() {
	if (!this->imagelist16.IsOk()) {
		this->imagelist16.Create(16, 16, IMAGELIST_LAST);
		for (int i = 0; i < IMAGELIST_LAST; i++) {
			this->imagelist16.Add(*this->GetImage16(i));
		}
	}
	return &this->imagelist16;
}

WImageList* MApp::GetImageList32() {
	if (!this->imagelist32.IsOk()) {
		this->imagelist32.Create(32, 32, IMAGELIST_LAST);
		for (int i = 0; i < IMAGELIST_LAST; i++) {
			this->imagelist32.Add(*this->GetImage32(i));
		}
	}
	return &this->imagelist32;
}

WImage* MApp::GetImage16(int index) {
	if (index > IMAGE_LAST)
		return 0;
	if (!this->image16[index].IsOk()) {
		WString _path = this->GetExecutablePath();
		int l = strlen(_path);
		char *path = (char*) malloc(_path.GetLength() + 30);
		if (path != 0) {
			sprintf(path, "%s/images/%s.png", _path.GetCharsNotNull(),
					images[index]);
			this->image16[index].Create(path);
		}
		free(path);
	}
	return &this->image16[index];
}

WImage* MApp::GetImage32(int index) {
	if (index > IMAGE_LAST)
		return 0;
	if (!this->image32[index].IsOk()) {
		WString _path = this->GetExecutablePath();
		int l = strlen(_path);
		char *path = (char*) malloc(_path.GetLength() + 30);
		if (path != 0) {
			sprintf(path, "%s/images/%s@2x.png", _path.GetCharsNotNull(),
					images[index]);
			this->image32[index].Create(path);
		}
	}
	return &this->image32[index];
}
WFrame* MApp::NewCustomShell() {
	MFrame *shell = new MFrame();
	WToolkit *custom = MApp::Get()->GetCustomToolkit();
	if (custom != 0 && this->theme == 0) {
		this->theme = MApp::Get()->CreateTheme("metal");
		custom->SetTheme(theme);
	}
	shell->Create(custom);
	shell->SetText("Swt Test Custom Theme");
	shell->Open();
	return shell;
}

WFrame* MApp::NewPlatformShell() {
	MFrame *shell = new MFrame();
	shell->Create(WApp::Get()->GetPlatformToolkit());
	shell->SetText("Swt Test Platform Theme");
	shell->Open();
	return shell;
}
