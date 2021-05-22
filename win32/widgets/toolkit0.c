/*
 * Name:        toolkit0.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
_w_toolkit *win_toolkit = 0;
const char *WindowClass = "SWT_Window";
const char *WindowShadowClass = "SWT_WindowShadow";
const char *WindowOwnDCClass = "SWT_WindowOwnDC";
w_toolkit* w_toolkit_get_platform(w_app *app) {
	if (win_toolkit == 0) {
		const int total_size = 0x10000;
		const int toolkit_size = sizeof(_w_toolkit);
		const int tmp_size = total_size - toolkit_size;
		win_toolkit = calloc(1, total_size);
		if (win_toolkit == 0) {
			fprintf(stderr, "Error : Do not initialize toolkit\n");
			exit(EXIT_FAILURE);
			return 0;
		}
		win_toolkit->tmp_alloc = tmp_size;
		_w_toolkit_init(win_toolkit);
	}
	return (w_toolkit*) win_toolkit;
}
wresult _w_toolkit_check_widget(w_toolkit *toolkit, w_widget *widget) {
	if (GetCurrentThreadId() != _W_TOOLKIT(toolkit)->thread.id) {
		return W_ERROR_THREAD_INVALID_ACCESS;
	} else
		return W_TRUE;
}
wresult _w_toolkit_check_widgetdata(w_toolkit *toolkit,
		w_widgetdata *widgetdata) {
	if (GetCurrentThreadId() != _W_TOOLKIT(toolkit)->thread.id) {
		return W_ERROR_THREAD_INVALID_ACCESS;
	} else
		return W_TRUE;
}
struct _w_widget_class* _w_toolkit_get_class(w_toolkit *toolkit,
		wushort clazz_id) {
	if (clazz_id >= _W_CLASS_LAST)
		return 0;
	else
		return _W_TOOLKIT(toolkit)->classes[clazz_id];
}
w_theme* _w_toolkit_get_theme(w_toolkit *toolkit) {
	return _W_TOOLKIT(toolkit)->theme;
}
wresult _w_toolkit_beep(w_toolkit *toolkit) {
	MessageBeep(MB_OK);
	return W_TRUE;
}
w_shell* _w_toolkit_get_active_shell(w_toolkit *toolkit) {
	HWND hwnd = GetActiveWindow();
	if (hwnd != 0) {
		w_control *control = W_CONTROL(_w_widget_find_control(hwnd));
		if (control != 0) {
			w_shell *shell;
			w_control_get_shell(control, &shell);
			return shell;
		}
	}
	return 0;
}
w_menu* _w_toolkit_get_menubar(w_toolkit *toolkit) {
	return 0;
}
wresult _w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *rect) {
	if (GetSystemMetrics(SM_CMONITORS) < 2) {
		rect->width = GetSystemMetrics(SM_CXSCREEN);
		rect->height = GetSystemMetrics(SM_CYSCREEN);
		rect->x = 0;
		rect->y = 0;
	} else {
		rect->x = GetSystemMetrics(SM_XVIRTUALSCREEN);
		rect->y = GetSystemMetrics(SM_YVIRTUALSCREEN);
		rect->width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		rect->height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}
	return W_TRUE;
}
wresult _w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea) {
	if (GetSystemMetrics(SM_CMONITORS) < 2) {
		RECT rect;
		SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
		clientArea->x = rect.left;
		clientArea->y = rect.top;
		clientArea->width = rect.right - rect.left;
		clientArea->height = rect.bottom - rect.top;
	} else {
		clientArea->x = GetSystemMetrics(SM_XVIRTUALSCREEN);
		clientArea->y = GetSystemMetrics(SM_YVIRTUALSCREEN);
		clientArea->width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		clientArea->height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}
	return W_TRUE;
}
w_control* _w_toolkit_get_cursor_control(w_toolkit *toolkit) {
	POINT pt;
	if (!GetCursorPos(&pt))
		return 0;
	HWND hwnd = WindowFromPoint(pt);
	if (hwnd != 0) {
		return W_CONTROL(_w_widget_find_control(hwnd));
	}
	return 0;
}
wresult _w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location) {
	POINT pt;
	GetCursorPos(&pt);
	location->x = pt.x;
	location->y = pt.y;
	return W_TRUE;
}
size_t _w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	if (sizes != 0 && length > 0) {
		sizes->width = GetSystemMetrics(SM_CXCURSOR);
		sizes->height = GetSystemMetrics(SM_CYCURSOR);
	}
	return 1;
}
int _w_toolkit_get_dismissal_alignment(w_toolkit *toolkit) {
	return W_LEFT;
}
int _w_toolkit_get_double_click_time(w_toolkit *toolkit) {
	return GetDoubleClickTime();
}
w_control* _w_toolkit_get_focus_control(w_toolkit *toolkit) {
	w_control *focusControl = _W_TOOLKIT(toolkit)->focusControl;
	if (w_widget_is_ok(W_WIDGET(focusControl))) {
		return focusControl;
	}
	HWND hwnd = GetFocus();
	if (hwnd != 0) {
		return W_CONTROL(_w_widget_find_control(hwnd));
	}
	return 0;
}
wresult _w_toolkit_get_high_contrast(w_toolkit *toolkit) {
	HIGHCONTRAST pvParam;
	pvParam.cbSize = sizeof(pvParam);
	SystemParametersInfoW(SPI_GETHIGHCONTRAST, 0, &pvParam, 0);
	return (pvParam.dwFlags & HCF_HIGHCONTRASTON) != 0;
}
int _w_toolkit_get_icon_depth(w_toolkit *toolkit) {
	if (WIN32_VERSION >= VERSION(5, 1)) {
		HDC hDC = CreateCompatibleDC(NULL);
		int bits = GetDeviceCaps(hDC, BITSPIXEL);
		int planes = GetDeviceCaps(hDC, PLANES);
		DeleteDC(hDC);
		int depth = bits * planes;
		if (depth >= 24)
			return 32;
	}

	/* Use the character encoding for the default locale */
	const char *s = "Control Panel\\Desktop\\WindowMetrics";
	WCHAR buffer[50];
	w_utf8_to_utf16(s, -1, buffer, sizeof(buffer) / sizeof(buffer[0]));

	HKEY phkResult = 0;
	int result = RegOpenKeyExW(HKEY_CURRENT_USER, buffer, 0, KEY_READ,
			&phkResult);
	if (result != 0)
		return 4;
	int depth = 4;
	DWORD lpcbData = 0;

	/* Use the character encoding for the default locale */
	const char *s1 = "Shell Icon BPP";
	WCHAR buffer1[30];
	w_utf8_to_utf16(s1, -1, buffer1, sizeof(buffer1) / sizeof(buffer1[0]));
	result = RegQueryValueExW(phkResult, buffer1, 0, NULL, NULL, &lpcbData);
	if (result == 0) {
		BYTE *lpData = _w_toolkit_malloc(lpcbData);
		if (lpData != 0) {
			result = RegQueryValueExW(phkResult, buffer1, 0, NULL, lpData,
					&lpcbData);
			if (result == 0) {
				int i = atoi((char*) lpData);
				if (i != 0)
					depth = i;
			}
			_w_toolkit_free(lpData, lpcbData);
		}
	}
	RegCloseKey(phkResult);
	return depth;
}
size_t _w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	if (sizes != 0) {
		if (length >= 1) {
			sizes[0].width = GetSystemMetrics(SM_CXSMICON);
			sizes[0].height = GetSystemMetrics(SM_CYSMICON);
		}
		if (length >= 2) {
			sizes[0].width = GetSystemMetrics(SM_CXICON);
			sizes[1].height = GetSystemMetrics(SM_CYICON);
		}
	}
	return 2;
}
_w_shell* _w_shells_iterator_find_next(_w_shell *parent, _w_shell *current) {
	if (parent == 0)
		return current->next;
	else {
		_w_shell *s = current->next;
		while (s != 0) {
			if (_W_CONTROL(s)->parent == W_COMPOSITE(parent))
				return s;
			s = s->next;
		}
		return 0;
	}
}
_w_shell* _w_shells_iterator_find_first(_w_shell *parent) {
	_w_shell *s = win_toolkit->shells;
	while (s != 0) {
		if (_W_CONTROL(s)->parent == W_COMPOSITE(parent))
			return s;
		s = s->next;
	}
	return 0;
}
wresult _w_shells_iterator_next(w_iterator *it, void *obj) {
	if (_W_SHELLS_ITERATOR(it)->current != 0) {
		*((_w_shell**) obj) = _W_SHELLS_ITERATOR(it)->current;
		_W_SHELLS_ITERATOR(it)->current = _w_shells_iterator_find_next(
		_W_SHELLS_ITERATOR(it)->parent,
		_W_SHELLS_ITERATOR(it)->current);
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_shells_iterator_reset(w_iterator *it) {
	if (_W_SHELLS_ITERATOR(it)->parent == 0)
		_W_SHELLS_ITERATOR(it)->current = win_toolkit->shells;
	else
		_W_SHELLS_ITERATOR(it)->current = _w_shells_iterator_find_first(
		_W_SHELLS_ITERATOR(it)->parent);
	return W_TRUE;
}
wresult _w_shells_iterator_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_shells_iterator_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_shells_iterator_count(w_iterator *it) {
	if (_W_SHELLS_ITERATOR(it)->parent == 0)
		return win_toolkit->shells_count;
	else {
		if (_W_SHELLS_ITERATOR(it)->count == -1) {
			_w_shell *s = win_toolkit->shells;
			size_t count = 0;
			while (s != 0) {
				if (_W_CONTROL(s)->parent
						== (w_composite*) _W_SHELLS_ITERATOR(it)->parent)
					count++;
				s = s->next;
			}
			_W_SHELLS_ITERATOR(it)->count = count;
		}
		return _W_SHELLS_ITERATOR(it)->count;
	}
}
_w_iterator_class _w_shells_iterator_class = { //
		_w_shells_iterator_close, //
				_w_shells_iterator_next, //
				_w_shells_iterator_reset, //
				_w_shells_iterator_remove, //
				_w_shells_iterator_count };
void _w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells) {
	_W_SHELLS_ITERATOR(shells)->base.clazz = &_w_shells_iterator_class;
	_W_SHELLS_ITERATOR(shells)->current = win_toolkit->shells;
	_W_SHELLS_ITERATOR(shells)->parent = 0;
	_W_SHELLS_ITERATOR(shells)->count = -1;
}
void _w_toolkit_get_shells_from_parent(w_shell *shell, w_iterator *iterator) {
	_W_SHELLS_ITERATOR(iterator)->base.clazz = &_w_shells_iterator_class;
	_W_SHELLS_ITERATOR(iterator)->current = _w_shells_iterator_find_first(
	_W_SHELLS_ITERATOR(iterator)->parent);
	_W_SHELLS_ITERATOR(iterator)->parent = _W_SHELL(shell);
	_W_SHELLS_ITERATOR(iterator)->count = -1;
}
w_color _w_toolkit_get_system_color(w_toolkit *toolkit, wuint id) {
	w_theme *theme = _W_TOOLKIT(toolkit)->theme;
	return w_theme_get_color(theme, id);
}
w_cursor* _w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint style) {
	if (style <= W_CURSOR_HAND) {
		return W_CURSOR(&_W_TOOLKIT(toolkit)->cursors[style]);
	} else
		return 0;
}
w_font* _w_toolkit_get_system_font(w_toolkit *toolkit) {
	_w_toolkit *t = (_w_toolkit*) toolkit;
	if (t->systemFont.handle == 0) {
		t->systemFont.handle = GetStockObject(DEFAULT_GUI_FONT);
		if (t->systemFont.handle == 0)
			t->systemFont.handle = GetStockObject(SYSTEM_FONT);
	}
	return (w_font*) &t->systemFont;
}
#ifndef OIC_HAND
#define OIC_HAND 32513
#define OIC_NOTE 32516
#define OIC_INFORMATION OIC_NOTE
#define OIC_QUES 32514
#define OIC_BANG 32515
#endif
wresult _w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
		w_image **image) {
	int index = -1, sys;
	switch (id) {
	case W_ICON_ERROR:
		sys = OIC_HAND;
		index = 0;
		break;
	case W_ICON_WORKING:
	case W_ICON_INFORMATION:
		sys = OIC_INFORMATION;
		index = 1;
		break;
	case W_ICON_QUESTION:
		sys = OIC_QUES;
		index = 2;
		break;
	case W_ICON_WARNING:
		sys = OIC_BANG;
		index = 3;
		break;
	}
	if (index != -1) {
		_w_image *img = &_W_TOOLKIT(toolkit)->images[index];
		if (img->handle == 0) {
			img->handle = LoadImageW(0, MAKEINTRESOURCEW(sys), IMAGE_ICON, 0, 0,
			LR_SHARED);
			if (img->handle != 0) {
				img->type = _IMAGE_ICON;
				*image = (w_image*) img;
				return W_TRUE;
			}
		}
	}
	*image = 0;
	return W_FALSE;
}
w_menu* _w_toolkit_get_system_menu(w_toolkit *toolkit) {
	return 0;
}
w_taskbar* _w_toolkit_get_system_taskbar(w_toolkit *toolkit) {
	w_taskbar *taskbar = &_W_TOOLKIT(toolkit)->taskbar;
	if (w_widget_is_ok(W_WIDGET(taskbar)))
		return taskbar;
	if (WIN32_VERSION >= VERSION(6, 1)) {
		//create taskbar
		return taskbar;
	} else
		return 0;
}
w_tray* _w_toolkit_get_system_tray(w_toolkit *toolkit) {
	w_tray *tray = &_W_TOOLKIT(toolkit)->tray;
	if (w_widget_is_ok(W_WIDGET(tray)))
		return tray;
	//create tray
	return tray;
}
w_thread* _w_toolkit_get_thread(w_toolkit *toolkit) {
	return &_W_TOOLKIT(toolkit)->thread;
}
wuint64 _w_toolkit_get_thread_id(w_toolkit *toolkit) {
	return _W_TOOLKIT(toolkit)->thread.id;
}
#if WINVER < 0x0601
#define NID_INTEGRATED_TOUCH 0x00000001
#define NID_EXTERNAL_TOUCH 0x00000002
#define NID_INTEGRATED_PEN 0x00000004
#define NID_EXTERNAL_PEN 0x00000008
#define NID_MULTI_INPUT 0x00000040
#define NID_READY 0x00000080
#define SM_DIGITIZER 94
#define SM_MAXIMUMTOUCHES 95
#endif
wresult _w_toolkit_get_touch_enabled(w_toolkit *toolkit) {
	int value = GetSystemMetrics(SM_DIGITIZER);
	return (value & (NID_READY | NID_MULTI_INPUT))
			== (NID_READY | NID_MULTI_INPUT);
}
wresult _w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
		w_point *result, w_point *point) {
	if (from != 0 && w_widget_is_ok(W_WIDGET(from)) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (to != 0 && w_widget_is_ok(W_WIDGET(to)) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (from == to) {
		result->x = point->x;
		result->y = point->y;
		return W_TRUE;
	}
	HWND hwndFrom = from != 0 ? _W_WIDGET(from)->handle : 0;
	HWND hwndTo = to != 0 ? _W_WIDGET(to)->handle : 0;
	POINT p;
	p.x = point->x;
	p.y = point->y;
	MapWindowPoints(hwndFrom, hwndTo, &p, 1);
	result->x = p.x;
	result->y = p.y;
	return W_TRUE;
}
wresult _w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
		w_rect *result, w_rect *rectangle) {
	if (from != 0 && w_widget_is_ok(W_WIDGET(from)) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (to != 0 && w_widget_is_ok(W_WIDGET(to)) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (from == to) {
		result->x = rectangle->x;
		result->y = rectangle->y;
		result->width = rectangle->width;
		result->height = rectangle->height;
		return W_TRUE;
	}
	HWND hwndFrom = from != 0 ? _W_WIDGET(from)->handle : 0;
	HWND hwndTo = to != 0 ? _W_WIDGET(to)->handle : 0;
	RECT rect;
	rect.left = rectangle->x;
	rect.top = rectangle->y;
	rect.right = rectangle->x + rectangle->width;
	rect.bottom = rectangle->y + rectangle->height;
	MapWindowPoints(hwndFrom, hwndTo, (LPPOINT) &rect, 2);
	result->x = rect.left;
	result->y = rect.top;
	result->width = rect.right - rect.left;
	result->height = rect.bottom - rect.top;
	return W_TRUE;
}
wresult _w_toolkit_post(w_toolkit *toolkit, w_event *event) {
	return W_TRUE;
}
wresult _w_toolkit_post_quit(w_toolkit *toolkit, int quit) {
	_W_TOOLKIT(toolkit)->exit_code = quit;
	_W_TOOLKIT(toolkit)->exit_loop = 1;
	PostQuitMessage(quit);
	return TRUE;
}
wresult _w_toolkit_filter_message(MSG *msg) {
	int message = msg->message;
	if (WM_KEYFIRST <= message && message <= WM_KEYLAST) {
		w_control *control = _w_widget_find_control(msg->hwnd);
		if (control != 0) {
			_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
			wresult result = W_FALSE;
			win_toolkit->accelKeyHit = TRUE;
			result = priv->translate_accelerator(control, msg, priv);
			win_toolkit->accelKeyHit = FALSE;
			if (!result) {
				//result = priv->translate_mnemonic(control, msg, priv);
			}
			if (!result) {
				//result = priv->translate_traversal(control, msg, priv);
			}
			if (result) {
				win_toolkit->lastAscii = win_toolkit->lastKey = 0;
				win_toolkit->lastVirtual = win_toolkit->lastNull =
						win_toolkit->lastDead = FALSE;
				return W_TRUE;
			}
		}
	}
	return W_FALSE;
}
wresult _w_toolkit_dispatch(w_toolkit *toolkit) {
	w_event _e;
	if (!_w_toolkit_filter_message(&win_toolkit->msg)) {
		TranslateMessage(&win_toolkit->msg);
		DispatchMessageW(&win_toolkit->msg);
	}
	if (win_toolkit->msg.message == WM_ASYNCEXEC) {
		((w_thread_start) win_toolkit->msg.lParam)(
				(void*) win_toolkit->msg.wParam);
	}
	w_widget *widget = ((_w_toolkit*) toolkit)->widget_free, *next;
	while (widget != 0) {
		w_widget_post_event_proc _proc =
				(w_widget_post_event_proc) widget->post_event;
		next = widget->handle; //widget = widget->next
		if (_proc != 0) {
			_e.type = W_EVENT_FREE_MEMORY;
			_e.platform_event = 0;
			_e.data = 0;
			_e.widget = widget;
			_proc(widget, &_e);
		} else {
			free(widget);
		}
		widget = next; //widget = widget->next
	}
	((_w_toolkit*) toolkit)->widget_free = 0;
	return W_TRUE;
}
wresult _w_toolkit_read(w_toolkit *toolkit) {
	GetMessageW(&win_toolkit->msg, NULL, 0, 0);
	return W_TRUE;
}
wresult _w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *point) {
	SetCursorPos(point->x, point->y);
	return W_TRUE;
}
wresult _w_toolkit_set_theme(w_toolkit *toolkit, w_theme *theme) {
	if (theme == 0) {
		_W_TOOLKIT(toolkit)->theme =
				(w_theme*) &_W_TOOLKIT(toolkit)->win32theme;
	} else {
		_W_TOOLKIT(toolkit)->theme = theme;
	}
	return W_TRUE;
}
int _w_toolkit_run(w_toolkit *toolkit) {
	_W_TOOLKIT(toolkit)->exit_loop = 0;
	while (_W_TOOLKIT(toolkit)->exit_loop == 0) {
		_w_toolkit_read(toolkit);
		_w_toolkit_dispatch(toolkit);
	}
	return _W_TOOLKIT(toolkit)->exit_code;
}
wresult _w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	PostThreadMessageW(win_toolkit->thread.id,
	WM_ASYNCEXEC, (WPARAM) args, (LPARAM) function);
	return TRUE;
}
wresult _w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	return W_TRUE;
}
wresult _w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args) {
	return W_TRUE;
}
wresult _w_toolkit_update(w_toolkit *toolkit) {
	return W_TRUE;
}
wresult _w_toolkit_wake(w_toolkit *toolkit) {
	PostThreadMessageW(_W_TOOLKIT(toolkit)->thread.id, WM_NULL, 0, 0);
	return W_TRUE;
}
void _w_toolkit_class_init(_w_toolkit *toolkit) {
	struct _w_toolkit_class *clazz = &toolkit->class_toolkit;
	((w_disposable*) toolkit)->clazz = (struct w_disposable_class*) clazz;
	clazz->disposable.dispose = _w_toolkit_dispose;
	clazz->check_widget = _w_toolkit_check_widget;
	clazz->check_widgetdata = _w_toolkit_check_widgetdata;
	clazz->get_class = _w_toolkit_get_class;
	clazz->get_theme = _w_toolkit_get_theme;
	clazz->async_exec = _w_toolkit_async_exec;
	clazz->beep = _w_toolkit_beep;
	clazz->get_active_shell = _w_toolkit_get_active_shell;
	clazz->get_menubar = _w_toolkit_get_menubar;
	clazz->get_bounds = _w_toolkit_get_bounds;
	clazz->get_client_area = _w_toolkit_get_client_area;
	clazz->get_cursor_control = _w_toolkit_get_cursor_control;
	clazz->get_cursor_location = _w_toolkit_get_cursor_location;
	clazz->get_cursor_sizes = _w_toolkit_get_cursor_sizes;
	clazz->get_dismissal_alignment = _w_toolkit_get_dismissal_alignment;
	clazz->get_double_click_time = _w_toolkit_get_double_click_time;
	clazz->get_focus_control = _w_toolkit_get_focus_control;
	clazz->get_high_contrast = _w_toolkit_get_high_contrast;
	clazz->get_icon_depth = _w_toolkit_get_icon_depth;
	clazz->get_icon_sizes = _w_toolkit_get_icon_sizes;
	clazz->get_shells = _w_toolkit_get_shells;
	clazz->get_system_color = _w_toolkit_get_system_color;
	clazz->get_system_cursor = _w_toolkit_get_system_cursor;
	clazz->get_system_font = _w_toolkit_get_system_font;
	clazz->get_system_image = _w_toolkit_get_system_image;
	clazz->get_system_menu = _w_toolkit_get_system_menu;
	clazz->get_system_taskbar = _w_toolkit_get_system_taskbar;
	clazz->get_system_tray = _w_toolkit_get_system_tray;
	clazz->get_theme = _w_toolkit_get_theme;
	clazz->get_thread = _w_toolkit_get_thread;
	clazz->get_thread_id = _w_toolkit_get_thread_id;
	clazz->get_touch_enabled = _w_toolkit_get_touch_enabled;
	clazz->map_0 = _w_toolkit_map_0;
	clazz->map_1 = _w_toolkit_map_1;
	clazz->post = _w_toolkit_post;
	clazz->post_quit = _w_toolkit_post_quit;
	clazz->dispatch = _w_toolkit_dispatch;
	clazz->set_cursor_location = _w_toolkit_set_cursor_location;
	clazz->set_theme = _w_toolkit_set_theme;
	clazz->run = _w_toolkit_run;
	clazz->read = _w_toolkit_read;
	clazz->sync_exec = _w_toolkit_sync_exec;
	clazz->timer_exec = _w_toolkit_timer_exec;
	clazz->update = _w_toolkit_update;
	clazz->wake = _w_toolkit_wake;
	clazz->messagebox_open = _w_toolkit_messagebox_open;
}
