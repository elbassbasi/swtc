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
	this->custom = 0;
	this->w_toolkit_custom_create_theme = 0;
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
			sprintf(path, "%s/images/%s.png", _path.GetCharsNotNull(), images[index]);
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
			sprintf(path, "%s/images/%s@2x.png", _path.GetCharsNotNull(), images[index]);
			this->image32[index].Create(path);
		}
	}
	return &this->image32[index];
}
typedef w_toolkit* (*_w_toolkit_get_custom)();
void MApp::LoadCustomToolkit() {
	char path[0x500], *last, sp = '/';
	WString exe_path = GetExecutablePath();
	sprintf(path, "%s%clibcswt%s", exe_path.GetCharsNotNull(), sp, w_module_default_extension());
	w_module *module = w_module_load(path);
	if (module != 0) {
		_w_toolkit_get_custom __w_toolkit_get_custom =
				(_w_toolkit_get_custom) w_module_get_symbol(module,
						"w_toolkit_get_custom");
		w_toolkit_custom_create_theme =
				(_w_toolkit_custom_create_theme) w_module_get_symbol(module,
						"w_toolkit_custom_create_theme");
		if (__w_toolkit_get_custom != 0) {
			this->custom = (WToolkit*) __w_toolkit_get_custom();
		}
	}
}
WToolkit* MApp::GetCustomToolkit() {
	if (this->custom == 0) {
		LoadCustomToolkit();

	}
	return this->custom;
}
WTheme* MApp::SetCustomTheme(WTheme *theme) {
	WTheme *last = 0;
	if (custom != 0) {
		last = custom->GetTheme();
		custom->SetTheme(theme);
	}
	return last;
}
WTheme* MApp::CreateCustomTheme(const char *name) {
	return (WTheme*) w_toolkit_custom_create_theme(name);
}

WFrame* MApp::NewCustomShell() {
	MFrame *shell = new MFrame();
	WToolkit *custom = MApp::Get()->GetCustomToolkit();
	if (custom != 0) {
		WTheme *theme = MApp::Get()->CreateCustomTheme("Metal");
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
