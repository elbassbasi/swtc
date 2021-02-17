/*
 * Name:        toolkit1.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
void* _w_toolkit_malloc(size_t size) {
	if ((win_toolkit->tmp_length + size) < win_toolkit->tmp_alloc) {
		int i = win_toolkit->tmp_length;
		win_toolkit->tmp_length += size;
		return &win_toolkit->tmp[i];
	} else {
		return malloc(size);
	}
}
void* _w_toolkit_malloc_all(size_t *size) {
	*size = win_toolkit->tmp_alloc - win_toolkit->tmp_length;
	int i = win_toolkit->tmp_length;
	win_toolkit->tmp_length += *size;
	return &win_toolkit->tmp[i];
}
void _w_toolkit_free(void *ptr, size_t size) {
	wintptr diff = (char*)ptr - (char*) win_toolkit->tmp;
	if (diff >= 0 && diff < win_toolkit->tmp_alloc) {
		win_toolkit->tmp_length -= size;
	} else
		free(ptr);
}
void _w_toolkit_add_shell(_w_shell *shell) {
	shell->next = 0;
	if (win_toolkit->shells == 0) {
		win_toolkit->shells = shell;
		shell->prev = shell; //last
	} else {
		_w_shell *last = win_toolkit->shells->prev;
		last->next = shell;
		shell->prev = last;
		win_toolkit->shells->prev = shell;
	}
	win_toolkit->shells_count++;
}
void _w_toolkit_remove_shell(_w_shell *shell) {
	if (shell == win_toolkit->shells) {
		win_toolkit->shells = shell->next;
		if (win_toolkit->shells != 0)
			win_toolkit->shells->prev = shell->prev; //last
	} else {
		if (shell->next == 0) {
			win_toolkit->shells->prev = shell->prev;
		} else {
			shell->next->prev = shell->prev;
		}
		shell->prev->next = shell->next;
	}
	win_toolkit->shells_count--;
}
typedef struct _DLLVERSIONINFO {
	DWORD cbSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformID;
} DLLVERSIONINFO;
typedef VOID (*WINAPI DllGetVersion)(DLLVERSIONINFO *pcdvi);
HMODULE _w_toolkit_load_library(const char *dll) {
	WCHAR tmp[0x100];
	w_utf8_to_utf16(dll, -1, tmp, sizeof(tmp) / sizeof(tmp[0]));
	HMODULE hModule = LoadLibraryW(tmp);
	return hModule;
}
int _w_toolkit_get_dll_version(HMODULE hModule) {
	DLLVERSIONINFO dvi;
	dvi.cbSize = sizeof(DLLVERSIONINFO);
	dvi.dwMajorVersion = 4;
	dvi.dwMinorVersion = 0;
	if (hModule != 0) {
		DllGetVersion dllGetVersion = (DllGetVersion) GetProcAddress(hModule,
				"DllGetVersion");
		if (dllGetVersion != 0) {
			dllGetVersion(&dvi);
		}
		FreeLibrary(hModule);
	}
	return VERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
}
void _w_toolkit_init_comctl32(_w_toolkit *toolkit) {
	/* enabling visual style */
	toolkit->hactCtx = INVALID_HANDLE_VALUE;
	ACTCTXW actCtx;
	ULONG_PTR cookie;
	actCtx.cbSize = sizeof(actCtx);
	actCtx.dwFlags =
	ACTCTX_FLAG_HMODULE_VALID | ACTCTX_FLAG_RESOURCE_NAME_VALID;
	actCtx.lpSource = NULL;
	actCtx.wProcessorArchitecture = 0;
	actCtx.wLangId = 0;
	actCtx.lpAssemblyDirectory = NULL;
	actCtx.lpResourceName = MAKEINTRESOURCEW(SWT_MANIFEST);
	actCtx.lpApplicationName = NULL;
	actCtx.hModule = hinst;
	toolkit->hactCtx = CreateActCtxW(&actCtx);
	if (toolkit->hactCtx != INVALID_HANDLE_VALUE) {
		ActivateActCtx(toolkit->hactCtx, &cookie);
	}
	HMODULE comctrl32dll = _w_toolkit_load_library("comctl32.dll");
	toolkit->comctrl32_version = _w_toolkit_get_dll_version(comctrl32dll);
	INITCOMMONCONTROLSEX picce;
	picce.dwSize = sizeof(picce);
	picce.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES
			| ICC_NATIVEFNTCTL_CLASS;
	InitCommonControlsEx(&picce);
}
void _w_toolkit_init_dll(_w_toolkit *toolkit) {
	OSVERSIONINFOA info;
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	if (GetVersionExA(&info)) {
		toolkit->win32_version = VERSION(info.dwMajorVersion,
				info.dwMinorVersion);
	}
	_w_toolkit_init_comctl32(toolkit);
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput input;
	GdiplusStartupOutput output;
	memset(&input, 0, sizeof(input));
	memset(&output, 0, sizeof(output));
	input.GdiplusVersion = 1;
	if (GdiplusStartup(&gdiplusToken, &input, &output) == 0) {
	}
	OleInitialize(NULL);
	/* Get the DBCS flag */
	int dbcsEnabled = GetSystemMetrics(SM_DBCSENABLED) != 0;
	int immEnabled = GetSystemMetrics(SM_IMMENABLED) != 0;
	win_toolkit->IsDBLocale = dbcsEnabled || immEnabled;
}
void _w_toolkit_registre_class_0(_w_toolkit *toolkit, const char *clazz,
		int style) {
	WCHAR clazzName[50];
	WNDCLASSEXW wincl;
	ATOM atom;
	wincl.hInstance = hinst;
	wincl.lpfnWndProc = _w_control_window_proc;
	wincl.style = style;
	wincl.cbSize = sizeof(WNDCLASSEXW);
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH) (COLOR_3DFACE + 1);
	w_utf8_to_utf16(clazz, -1, clazzName,
			sizeof(clazzName) / sizeof(clazzName[0]));
	wincl.lpszClassName = clazzName;
	atom = RegisterClassExW(&wincl);
	if (atom == 0) {
		fprintf(stderr, "Error registre Atom %s", clazz);
		exit(0);
	}
}
void _w_toolkit_registre_class(_w_toolkit *toolkit) {
	_w_toolkit_registre_class_0(toolkit, WindowClass, CS_DBLCLKS);
	_w_toolkit_registre_class_0(toolkit, WindowShadowClass, CS_DBLCLKS);
	_w_toolkit_registre_class_0(toolkit, WindowOwnDCClass,
	CS_DBLCLKS | CS_OWNDC);
}
void _w_toolkit_widget_class_init(_w_toolkit *toolkit) {
	/*
	 * shell
	 */
	W_WIDGET_CLASS(&toolkit->class_shell)->init_class =
			(w_widget_init_class) _w_shell_class_init;
	W_WIDGET_CLASS(&toolkit->class_shell)->reserved[0] =
			&toolkit->class_shell_priv;
	toolkit->classes[_W_CLASS_SHELL] = W_WIDGET_CLASS(&toolkit->class_shell);
	/*
	 * canvas
	 */
	W_WIDGET_CLASS(&toolkit->class_canvas)->init_class =
			(w_widget_init_class) _w_canvas_class_init;
	W_WIDGET_CLASS(&toolkit->class_canvas)->reserved[0] =
			&toolkit->class_canvas_priv;
	toolkit->classes[_W_CLASS_CANVAS] = W_WIDGET_CLASS(&toolkit->class_canvas);
	/*
	 * composite
	 */
	W_WIDGET_CLASS(&toolkit->class_composite)->init_class =
			(w_widget_init_class) _w_composite_class_init;
	W_WIDGET_CLASS(&toolkit->class_composite)->reserved[0] =
			&toolkit->class_composite_priv;
	toolkit->classes[_W_CLASS_COMPOSITE] = W_WIDGET_CLASS(
			&toolkit->class_composite);
	/*
	 * menu
	 */
	W_WIDGET_CLASS(&toolkit->class_menu)->init_class =
			(w_widget_init_class) _w_menu_class_init;
	W_WIDGET_CLASS(&toolkit->class_menu)->reserved[0] =
			&toolkit->class_menu_priv;
	W_MENU_CLASS(&toolkit->class_menu)->class_menuitem =
			&toolkit->class_menuitem;
	toolkit->classes[_W_CLASS_MENU] = W_WIDGET_CLASS(&toolkit->class_menu);
	/*
	 * treeview
	 */
	W_WIDGET_CLASS(&toolkit->class_treeview)->init_class =
			(w_widget_init_class) _w_treeview_class_init;
	W_WIDGET_CLASS(&toolkit->class_treeview)->reserved[0] =
			&toolkit->class_treeview_priv;
	W_LISTVIEWBASE_CLASS(&toolkit->class_treeview)->class_item = W_ITEM_CLASS(
			&toolkit->class_treeitem);
	W_LISTVIEWBASE_CLASS(&toolkit->class_treeview)->class_column =
			&toolkit->class_treecolumn;
	toolkit->classes[_W_CLASS_TREEVIEW] = W_WIDGET_CLASS(
			&toolkit->class_treeview);
	/*
	 * listview
	 */
	W_WIDGET_CLASS(&toolkit->class_listview)->init_class =
			(w_widget_init_class) _w_listview_class_init;
	W_WIDGET_CLASS(&toolkit->class_listview)->reserved[0] =
			&toolkit->class_listview_priv;
	W_LISTVIEWBASE_CLASS(&toolkit->class_listview)->class_item = W_ITEM_CLASS(
			&toolkit->class_listitem);
	W_LISTVIEWBASE_CLASS(&toolkit->class_listview)->class_column =
			&toolkit->class_listcolumn;
	toolkit->classes[_W_CLASS_LISTVIEW] = W_WIDGET_CLASS(
			&toolkit->class_listview);
}
wushort _wm_msg[] = { //
		[_WM_ACTIVATE] = WM_ACTIVATE, //
				[_WM_CAPTURECHANGED] = WM_CAPTURECHANGED, //
				[_WM_CHANGEUISTATE] = WM_CHANGEUISTATE, //
				[_WM_CHAR] = WM_CHAR, //
				[_WM_CLEAR] = WM_CLEAR, //
				[_WM_CLOSE] = WM_CLOSE, //
				[_WM_COMMAND] = WM_COMMAND, //
				[_WM_CONTEXTMENU] = WM_CONTEXTMENU, //
				[_WM_CUT] = WM_CUT, //
				[_WM_DESTROY] = WM_DESTROY, //
				[_WM_DRAWITEM] = WM_DRAWITEM, //
				[_WM_ENDSESSION] = WM_ENDSESSION, //
				[_WM_ENTERIDLE] = WM_ENTERIDLE, //
				[_WM_ERASEBKGND] = WM_ERASEBKGND, //
				//[_WM_GESTURE] = WM_GESTURE,//
				[_WM_GETDLGCODE] = WM_GETDLGCODE,			//
				[_WM_GETFONT] = WM_GETFONT,			//
				[_WM_GETOBJECT] = WM_GETOBJECT,			//
				[_WM_GETMINMAXINFO] = WM_GETMINMAXINFO,			//
				[_WM_HELP] = WM_HELP,			//
				[_WM_HSCROLL] = WM_HSCROLL,			//
				[_WM_IME_CHAR] = WM_IME_CHAR,			//
				[_WM_IME_COMPOSITION] = WM_IME_COMPOSITION,			//
				//[_WM_IME_COMPOSITION_START] = WM_IME_COMPOSITION_START,//
				[_WM_IME_ENDCOMPOSITION] = WM_IME_ENDCOMPOSITION,			//
				[_WM_INITMENUPOPUP] = WM_INITMENUPOPUP,			//
				[_WM_INPUTLANGCHANGE] = WM_INPUTLANGCHANGE,			//
				[_WM_HOTKEY] = WM_HOTKEY,			//
				[_WM_KEYDOWN] = WM_KEYDOWN,			//
				[_WM_KEYUP] = WM_KEYUP,			//
				[_WM_KILLFOCUS] = WM_KILLFOCUS,			//
				[_WM_LBUTTONDBLCLK] = WM_LBUTTONDBLCLK,			//
				[_WM_LBUTTONDOWN] = WM_LBUTTONDOWN,			//
				[_WM_LBUTTONUP] = WM_LBUTTONUP,			//
				[_WM_MBUTTONDBLCLK] = WM_MBUTTONDBLCLK,			//
				[_WM_MBUTTONDOWN] = WM_MBUTTONDOWN,			//
				[_WM_MBUTTONUP] = WM_MBUTTONUP,			//
				[_WM_MEASUREITEM] = WM_MEASUREITEM,			//
				[_WM_MENUCHAR] = WM_MENUCHAR,			//
				[_WM_MENUSELECT] = WM_MENUSELECT,			//
				[_WM_MOUSEACTIVATE] = WM_MOUSEACTIVATE,			//
				[_WM_MOUSEHOVER] = WM_MOUSEHOVER,			//
				[_WM_MOUSELEAVE] = WM_MOUSELEAVE,			//
				[_WM_MOUSEMOVE] = WM_MOUSEMOVE,			//
				[_WM_MOUSEWHEEL] = WM_MOUSEWHEEL,			//
				[_WM_MOUSEHWHEEL] = WM_MOUSEHWHEEL,			//
				[_WM_MOVE] = WM_MOVE,			//
				[_WM_NCACTIVATE] = WM_NCACTIVATE,			//
				[_WM_NCCALCSIZE] = WM_NCCALCSIZE,			//
				[_WM_NCHITTEST] = WM_NCHITTEST,			//
				[_WM_NCLBUTTONDOWN] = WM_NCLBUTTONDOWN,			//
				[_WM_NCPAINT] = WM_NCPAINT,			//
				[_WM_NOTIFY] = WM_NOTIFY,			//
				[_WM_PAINT] = WM_PAINT,			//
				[_WM_PALETTECHANGED] = WM_PALETTECHANGED,			//
				[_WM_PARENTNOTIFY] = WM_PARENTNOTIFY,			//
				[_WM_PASTE] = WM_PASTE,			//
				[_WM_PRINT] = WM_PRINT,			//
				[_WM_PRINTCLIENT] = WM_PRINTCLIENT,			//
				[_WM_QUERYENDSESSION] = WM_QUERYENDSESSION,			//
				[_WM_QUERYNEWPALETTE] = WM_QUERYNEWPALETTE,			//
				[_WM_QUERYOPEN] = WM_QUERYOPEN,			//
				[_WM_RBUTTONDBLCLK] = WM_RBUTTONDBLCLK,			//
				[_WM_RBUTTONDOWN] = WM_RBUTTONDOWN,			//
				[_WM_RBUTTONUP] = WM_RBUTTONUP,			//
				[_WM_SETCURSOR] = WM_SETCURSOR,			//
				[_WM_SETFOCUS] = WM_SETFOCUS,			//
				[_WM_SETFONT] = WM_SETFONT,			//
				[_WM_SETTINGCHANGE] = WM_SETTINGCHANGE,			//
				[_WM_SETREDRAW] = WM_SETREDRAW,			//
				[_WM_SHOWWINDOW] = WM_SHOWWINDOW,			//
				[_WM_SIZE] = WM_SIZE,			//
				[_WM_SYSCHAR] = WM_SYSCHAR,			//
				[_WM_SYSCOLORCHANGE] = WM_SYSCOLORCHANGE,			//
				[_WM_SYSCOMMAND] = WM_SYSCOMMAND,			//
				[_WM_SYSKEYDOWN] = WM_SYSKEYDOWN,			//
				[_WM_SYSKEYUP] = WM_SYSKEYUP,			//
				//[_WM_TABLET_FLICK] = WM_TABLET_FLICK,//
				[_WM_TIMER] = WM_TIMER,			//
				[_WM_TOUCH] = WM_TOUCH,			//
				[_WM_UNDO] = WM_UNDO,			//
				[_WM_UNINITMENUPOPUP] = WM_UNINITMENUPOPUP,			//
				[_WM_UPDATEUISTATE] = WM_UPDATEUISTATE,			//
				[_WM_VSCROLL] = WM_VSCROLL,			//
				[_WM_WINDOWPOSCHANGED] = WM_WINDOWPOSCHANGED,			//
				[_WM_WINDOWPOSCHANGING] = WM_WINDOWPOSCHANGING,			//
				[_WM_XBUTTONDBLCLK] = WM_XBUTTONDBLCLK,			//
				[_WM_XBUTTONDOWN] = WM_XBUTTONDOWN,			//
				[_WM_XBUTTONUP] = WM_XBUTTONUP,			//
				//[_WM_DPICHANGED] = WM_DPICHANGED,//
				[_WM_MENUCOMMAND] = WM_MENUCOMMAND,			//
		};
void _w_toolkit_init_messages(_w_toolkit *toolkit) {
	wuchar *wm_msg = toolkit->wm_msg;
	for (int i = 0; i < sizeof(_wm_msg) / sizeof(_wm_msg[0]); i++) {
		wm_msg[_wm_msg[i]] = i;
	}
	wm_msg[0] = 0;
	wm_msg[WM_CTLCOLORMSGBOX] = _WM_CTLCOLOR;
	wm_msg[WM_CTLCOLOREDIT] = _WM_CTLCOLOR;
	wm_msg[WM_CTLCOLORLISTBOX] = _WM_CTLCOLOR;
	wm_msg[WM_CTLCOLORBTN] = _WM_CTLCOLOR;
	wm_msg[WM_CTLCOLORDLG] = _WM_CTLCOLOR;
	wm_msg[WM_CTLCOLORSCROLLBAR] = _WM_CTLCOLOR;
	wm_msg[WM_CTLCOLORSTATIC] = _WM_CTLCOLOR;

}
void _w_toolkit_init(_w_toolkit *toolkit) {
	_w_toolkit_init_dll(toolkit);
	_w_toolkit_class_init(toolkit);
	_w_toolkit_widget_class_init(toolkit);
	_w_toolkit_init_messages(toolkit);
	_w_toolkit_registre_class(toolkit);
	_w_theme_init();
	toolkit->thread.id = GetCurrentThreadId();
}
void _w_toolkit_dispose(w_disposable *disposable) {
	_w_toolkit *toolkit = _W_TOOLKIT(disposable);
	if (toolkit->hactCtx != INVALID_HANDLE_VALUE) {
		ReleaseActCtx(toolkit->hactCtx);
	}

}
