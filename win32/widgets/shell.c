/*
 * Name:        shell.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "shell.h"
#include "toolkit.h"
wuint64 _w_shell_check_style(w_widget *widget, wuint64 style) {
	if ((style & W_NO_TRIM) != 0) {
		style &= ~(W_CLOSE | W_TITLE | W_MIN | W_MAX | W_RESIZE | W_BORDER);
	} else if ((style & W_NO_MOVE) != 0) {
		style |= W_TITLE;
	}
	if ((style & (W_MENU | W_MIN | W_MAX | W_CLOSE)) != 0) {
		style |= W_TITLE;
	}
	style &= ~W_TRANSPARENT;
	if ((style & W_ON_TOP) != 0)
		style &= ~(W_CLOSE | W_TITLE | W_MIN | W_MAX);
	int mask = W_SYSTEM_MODAL | W_APPLICATION_MODAL | W_PRIMARY_MODAL;
	if ((style & W_SHEET) != 0) {
		style &= ~W_SHEET;
		w_composite *parent = _W_WIDGET(widget)->parent;
		style |= parent == 0 ? W_FRAME_TRIM : W_DIALOG_TRIM;
		if ((style & mask) == 0) {
			style |= parent == 0 ? W_APPLICATION_MODAL : W_PRIMARY_MODAL;
		}
	}
	int bits = style & ~mask;
	if ((style & W_SYSTEM_MODAL) != 0)
		return bits | W_SYSTEM_MODAL;
	if ((style & W_APPLICATION_MODAL) != 0)
		return bits | W_APPLICATION_MODAL;
	if ((style & W_PRIMARY_MODAL) != 0)
		return bits | W_PRIMARY_MODAL;
	return bits;
}
wresult _w_shell_create_embedded(w_widget *widget, w_widget *parent,
		wuint64 style, w_widget_post_event_proc post_event, void *handle,
		int embedded) {
	if (parent != 0) {
		if (!w_widget_is_ok(parent))
			return W_ERROR_INVALID_ARGUMENT;
		if (w_widget_class_id(parent) != _W_CLASS_SHELL)
			return W_ERROR_INVALID_ARGUMENT;
	}
	_W_WIDGET(widget)->parent = parent;
	_W_SHELL(widget)->center = parent != 0 && (style & W_SHEET) != 0;
	_W_WIDGET(widget)->post_event = post_event;
	_W_WIDGET(widget)->style = _w_shell_check_style(widget, style);
	if (handle != 0) {
		if (embedded) {
			_W_WIDGET(widget)->handle = handle;
		} else {
			_W_WIDGET(widget)->handle = handle;
			_W_WIDGET(widget)->state |= STATE_FOREIGN_HANDLE;
		}
	}
	_w_control_priv *reserved = _W_CONTROL_GET_PRIV(widget);
	wresult result = reserved->create_widget(W_CONTROL(widget), reserved);
	if (result > 0) {
		_w_toolkit_add_shell(_W_SHELL(widget));
	}
	return result;
}
wresult _w_shell_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_shell_create_embedded(widget, parent, style, post_event, 0,
			W_FALSE);
}
wresult _w_shell_create_tooltip(w_shell *shell, _w_tooltip *tooltip) {
	int id = 110;
	tooltip->next = 0;
	if (_W_SHELL(shell)->tooltips == 0) {
		_W_SHELL(shell)->tooltips = tooltip;
		tooltip->prev = tooltip;
	} else {
		_w_tooltip *last = _W_SHELL(shell)->tooltips->prev;
		last->next = tooltip;
		tooltip->prev = last;
		_W_SHELL(shell)->tooltips->prev = tooltip;
	}
	_W_TOOLTIP(tooltip)->id = id + ID_START;
	TOOLINFOW lpti;
	lpti.cbSize = sizeof(TOOLINFOW);
	lpti.hwnd = _W_WIDGET(shell)->handle;
	lpti.uId = _W_TOOLTIP(tooltip)->id;
	lpti.uFlags = TTF_TRACK;
	lpti.lpszText = LPSTR_TEXTCALLBACKW;
	HWND hwndToolTip;
	if ((_W_WIDGET(tooltip)->style & W_BALLOON) != 0) {
		hwndToolTip = _W_SHELL(shell)->balloonTipHandle;
	} else {
		hwndToolTip = _W_SHELL(shell)->toolTipHandle;
	}
	SendMessageW(hwndToolTip, TTM_ADDTOOLW, 0, (LPARAM) &lpti);
	return W_TRUE;
}
wresult _w_shell_balloontip_handle(w_shell *shell, HWND *handle) {
	if (_W_SHELL(shell)->balloonTipHandle == 0) {
		_W_SHELL(shell)->balloonTipHandle = CreateWindowExW(0, TOOLTIPS_CLASSW,
		NULL, TTS_ALWAYSTIP | TTS_NOPREFIX | TTS_BALLOON, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, _W_WIDGET(shell)->handle, 0, hinst, NULL);
		if (_W_SHELL(shell)->balloonTipHandle == 0)
			return W_ERROR_NO_HANDLES;
		if (_W_SHELL(shell)->ToolTipProc == 0) {
			_W_SHELL(shell)->ToolTipProc = (WNDPROC) GetWindowLongPtrW(
			_W_SHELL(shell)->balloonTipHandle, GWLP_WNDPROC);
		}
		/*
		 * Feature in Windows.  Despite the fact that the
		 * tool tip text contains \r\n, the tooltip will
		 * not honour the new line unless TTM_SETMAXTIPWIDTH
		 * is set.  The fix is to set TTM_SETMAXTIPWIDTH to
		 * a large value.
		 */
		SendMessageW(_W_SHELL(shell)->balloonTipHandle, TTM_SETMAXTIPWIDTH, 0,
				0x7FFF);
		SetWindowLongPtrW(_W_SHELL(shell)->balloonTipHandle, GWLP_USERDATA,
				(LONG_PTR) shell);
		SetWindowLongPtrW(_W_SHELL(shell)->balloonTipHandle, GWLP_WNDPROC,
				(LONG_PTR) _w_control_window_proc);
	}
	*handle = _W_SHELL(shell)->balloonTipHandle;
	return W_TRUE;
}
HWND _w_shell_create_tooltip_handle(w_shell *shell, HWND parent) {
	HWND toolTipHandle = CreateWindowExW(0, TOOLTIPS_CLASSW, NULL,
	TTS_ALWAYSTIP | TTS_NOPREFIX, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, parent, 0,
			hinst, NULL);
	if (toolTipHandle == 0)
		return 0;
	if (_W_SHELL(shell)->ToolTipProc == 0) {
		_W_SHELL(shell)->ToolTipProc = (WNDPROC) GetWindowLongPtrW(
				toolTipHandle, GWLP_WNDPROC);
	}
	/*
	 * Feature in Windows.  Despite the fact that the
	 * tool tip text contains \r\n, the tooltip will
	 * not honour the new line unless TTM_SETMAXTIPWIDTH
	 * is set.  The fix is to set TTM_SETMAXTIPWIDTH to
	 * a large value.
	 */
	SendMessageW(toolTipHandle, TTM_SETMAXTIPWIDTH, 0, 0x7FFF);
	SetWindowLongPtrW(toolTipHandle, GWLP_USERDATA, (LONG_PTR) shell);
	SetWindowLongPtrW(toolTipHandle, GWLP_WNDPROC,
			(LONG_PTR) _w_control_window_proc);
	return toolTipHandle;
}
wresult _w_shell_tooltip_handle(w_shell *shell, HWND *handle) {
	wresult result = W_TRUE;
	if (_W_SHELL(shell)->toolTipHandle == 0) {
		_W_SHELL(shell)->toolTipHandle = _w_shell_create_tooltip_handle(shell,
		_W_WIDGET(shell)->handle);
		if (_W_SHELL(shell)->toolTipHandle == 0)
			result = W_ERROR_NO_HANDLES;
	}
	*handle = _W_SHELL(shell)->toolTipHandle;
	return result;
}
wresult _w_shell_menu_item_tooltip_handle(w_shell *shell, HWND *handle) {
	wresult result = W_TRUE;
	if (_W_SHELL(shell)->toolTipHandle == 0) {
		_W_SHELL(shell)->toolTipHandle = _w_shell_create_tooltip_handle(shell,
		_W_WIDGET(shell)->handle);
		if (_W_SHELL(shell)->toolTipHandle == 0)
			result = W_ERROR_NO_HANDLES;
	}
	*handle = _W_SHELL(shell)->toolTipHandle;
	return result;
}
void _w_shell_set_item_enabled(w_widget *widget, int cmd, boolean enabled) {
	HMENU hMenu = GetSystemMenu(_W_WIDGET(widget)->handle, FALSE);
	if (hMenu == 0)
		return;
	int flags = MF_ENABLED;
	if (!enabled)
		flags = MF_DISABLED | MF_GRAYED;
	EnableMenuItem(hMenu, cmd, MF_BYCOMMAND | flags);
}
wresult _w_shell_call_window_proc(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_WIDGET(widget)->handle == 0)
		return W_FALSE;
	if (e->hwnd == _W_SHELL(widget)->toolTipHandle
			|| e->hwnd == _W_SHELL(widget)->balloonTipHandle
			|| e->hwnd == _W_SHELL(widget)->menuItemToolTipHandle) {
		e->result = CallWindowProcW(_W_SHELL(widget)->ToolTipProc, e->hwnd,
				e->msg, e->wparam, e->lparam);
		return W_TRUE;
	}

	if (_W_SHELL(widget)->windowProc != 0) {
		e->result = CallWindowProcW(_W_SHELL(widget)->windowProc, e->hwnd,
				e->msg, e->wparam, e->lparam);
		return W_TRUE;
	}
	wuint64 style = _W_WIDGET(widget)->style;
	if ((style & W_TOOL) != 0) {
		int trim = W_TITLE | W_CLOSE | W_MIN | W_MAX | W_BORDER | W_RESIZE;
		if ((style & trim) == 0)
			e->result = DefWindowProcW(e->hwnd, e->msg, e->wparam, e->lparam);
		return W_TRUE;
	}
	if ((style & W_NO_MOVE) != 0) {
		_w_shell_set_item_enabled(widget, SC_MOVE, FALSE);
	}
	if (_W_WIDGET(widget)->parent != 0) {
		switch (e->msg) {
		case WM_KILLFOCUS:
		case WM_SETFOCUS:
			e->result = DefWindowProcW(e->hwnd, e->msg, e->wparam, e->lparam);
			return W_TRUE;
		}
		e->result = CallWindowProcW(_W_SHELL(widget)->DialogProc, e->hwnd,
				e->msg, e->wparam, e->lparam);
		return W_TRUE;
	}
	e->result = DefWindowProcW(e->hwnd, e->msg, e->wparam, e->lparam);
	return W_TRUE;
}
void _w_shell_set_system_menu(w_shell *shell) {
	HMENU hMenu = GetSystemMenu(_W_WIDGET(shell)->handle, FALSE);
	if (hMenu == 0)
		return;
	int oldCount = GetMenuItemCount(hMenu);
	wuint64 style = _W_WIDGET(shell)->style;
	if ((style & W_RESIZE) == 0) {
		DeleteMenu(hMenu, SC_SIZE, MF_BYCOMMAND);
	}
	if ((style & W_MIN) == 0) {
		DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
	}
	if ((style & W_MAX) == 0) {
		DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	}
	if ((style & (W_MIN | W_MAX)) == 0) {
		DeleteMenu(hMenu, SC_RESTORE, MF_BYCOMMAND);
	}
	int newCount = GetMenuItemCount(hMenu);
	if ((style & W_CLOSE) == 0 || newCount != oldCount) {
		DeleteMenu(hMenu, SC_TASKLIST, MF_BYCOMMAND);
		MENUITEMINFOW info;
		info.cbSize = sizeof(MENUITEMINFOW);
		info.fMask = MIIM_ID;
		int index = 0;
		while (index < newCount) {
			if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
				if (info.wID == SC_CLOSE)
					break;
			}
			index++;
		}
		if (index != newCount) {
			DeleteMenu(hMenu, index - 1, MF_BYPOSITION);
			if ((style & W_CLOSE) == 0) {
				DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
			}
		}
	}
}
wresult _w_shell_create_handle(w_control *control, _w_control_priv *priv) {
	int embedded = _W_WIDGET(control)->handle != 0
			&& (_W_WIDGET(control)->state & STATE_FOREIGN_HANDLE) == 0;

	/*
	 * On Windows 98 and NT, setting a window to be the
	 * top most window using HWND_TOPMOST can result in a
	 * parent dialog shell being moved behind its parent
	 * if the dialog has a sibling that is currently on top
	 * This only occurs using SetWindowPos (), not when the
	 * handle is created.
	 */
	/*
	 * The following code is intentionally commented.
	 */
//	if ((style & W_ON_TOP) != 0) display.lockActiveWindow = true;
	if (_W_WIDGET(control)->handle == 0 || embedded) {
		wresult result = _w_composite_create_handle(control, priv);
		if (result <= 0)
			return result;
		if (_W_WIDGET(control)->parent != 0
				|| ((_W_WIDGET(control)->style & W_TOOL) != 0)) {
			_w_shell_set_system_menu(W_SHELL(control));
		}
	} else {
		_W_WIDGET(control)->state |= STATE_CANVAS;
		if ((_W_WIDGET(control)->style & (W_HSCROLL | W_VSCROLL)) == 0) {
			_W_WIDGET(control)->state |= STATE_THEME_BACKGROUND;
		}
		_W_SHELL(control)->windowProc = (WNDPROC) GetWindowLongPtrW(
		_W_WIDGET(control)->handle, GWLP_WNDPROC);
	}

	/*
	 * The following code is intentionally commented.
	 */
//	if ((style & W_ON_TOP) != 0)  display.lockActiveWindow = false;
	if (!embedded) {
		int bits = GetWindowLong(_W_WIDGET(control)->handle, GWL_STYLE);
		bits &= ~(WS_OVERLAPPED | WS_CAPTION);
		bits |= WS_POPUP;
		if ((_W_WIDGET(control)->style & W_TITLE) != 0)
			bits |= WS_CAPTION;
		if ((_W_WIDGET(control)->style & W_NO_TRIM) == 0) {
			if ((_W_WIDGET(control)->style & (W_BORDER | W_RESIZE)) == 0)
				bits |= WS_BORDER;
		}
		/*
		 * Bug in Windows.  When the WS_CAPTION bits are cleared using
		 * SetWindowLong(), Windows does not resize the client area of
		 * the window to get rid of the caption until the first resize.
		 * The fix is to use SetWindowPos() with SWP_DRAWFRAME to force
		 * the frame to be redrawn and resized.
		 */
		SetWindowLong(_W_WIDGET(control)->handle, GWL_STYLE, bits);
		int flags = SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER
				| SWP_NOACTIVATE;
		SetWindowPos(_W_WIDGET(control)->handle, 0, 0, 0, 0, 0, flags);
	}
	if (win_toolkit->IsDBLocale) {
		HIMC hIMC = ImmCreateContext();
		if (hIMC != 0)
			ImmAssociateContext(_W_WIDGET(control)->handle, hIMC);
	}
	_W_SHELL(control)->swFlags = SW_SHOWNOACTIVATE;
	return W_TRUE;
}
WCHAR *DialogClass = L"#32770";
WCHAR* _w_shell_window_class(w_control *control, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_TOOL) != 0) {
		int trim = W_TITLE | W_CLOSE | W_MIN | W_MAX | W_BORDER | W_RESIZE;
		if ((style & trim) == 0)
			return WindowShadowClass;
	}
	w_composite *parent = _W_WIDGET(control)->parent;
	return parent != 0 ? DialogClass : _w_canvas_window_class(control, priv);
}
HWND _w_shell_widget_parent(w_control *control, _w_control_priv *priv) {
	if (_W_WIDGET(control)->handle != 0)
		return _W_WIDGET(control)->handle;
	w_composite *parent = _W_WIDGET(control)->parent;
	return parent != 0 ? _W_WIDGET(parent)->handle : 0;
}
DWORD _w_shell_widget_extstyle_0(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_scrollable_widget_extstyle(control, priv) | WS_EX_MDICHILD;
	bits &= ~WS_EX_CLIENTEDGE;
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_NO_TRIM) != 0)
		return bits;
	if ((style & W_RESIZE) != 0)
		return bits;
	if ((style & W_BORDER) != 0)
		bits |= WS_EX_DLGMODALFRAME;
	return bits;
}
DWORD _w_shell_widget_extstyle(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_shell_widget_extstyle_0(control, priv) & ~WS_EX_MDICHILD;
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_TOOL) != 0)
		bits |= WS_EX_TOOLWINDOW;

	/*
	 * Feature in Windows.  When a window that does not have a parent
	 * is created, it is automatically added to the Windows Task Bar,
	 * even when it has no title.  The fix is to use WS_EX_TOOLWINDOW
	 * which does not cause the window to appear in the Task Bar.
	 */
	w_composite *parent = _W_WIDGET(control)->parent;
	if (parent == 0) {
		if ((style & W_ON_TOP) != 0) {
			int trim = W_TITLE | W_CLOSE | W_MIN | W_MAX;
			if ((style & W_NO_TRIM) != 0 || (style & trim) == 0) {
				bits |= WS_EX_TOOLWINDOW;
			}
		}
	}
	/*
	 * Bug in Windows 98 and NT.  Creating a window with the
	 * WS_EX_TOPMOST extended style can result in a dialog shell
	 * being moved behind its parent.  The exact case where this
	 * happens is a shell with two dialog shell children where
	 * each dialog child has another hidden dialog child with
	 * the WS_EX_TOPMOST extended style.  Clicking on either of
	 * the visible dialogs causes them to become active but move
	 * to the back, behind the parent shell.  The fix is to
	 * disallow the WS_EX_TOPMOST extended style on Windows 98
	 * and NT.
	 */
	if (parent != 0) {
		if (WIN32_VERSION < VERSION(4, 10)) {
			return bits;
		}
	}
	if ((style & W_ON_TOP) != 0)
		bits |= WS_EX_TOPMOST;
	return bits;
}
DWORD _w_shell_widget_style_0(w_control *control, _w_control_priv *priv) {
	/*
	 * Clear WS_VISIBLE and WS_TABSTOP.  NOTE: In Windows, WS_TABSTOP
	 * has the same value as WS_MAXIMIZEBOX so these bits cannot be
	 * used to control tabbing.
	 */
	DWORD bits = _w_composite_widget_style(control, priv)
			& ~(WS_TABSTOP | WS_VISIBLE);
	wuint64 style = _W_WIDGET(control)->style;

	/* Set the title bits and no-trim bits */
	bits &= ~WS_BORDER;
	if ((style & W_NO_TRIM) != 0) {
		if (_W_WIDGET(control)->parent == 0) {
			bits |= WS_SYSMENU | WS_MINIMIZEBOX;
		}
		return bits;
	}
	if ((style & W_TITLE) != 0)
		bits |= WS_CAPTION;

	/* Set the min and max button bits */
	if ((style & W_MIN) != 0)
		bits |= WS_MINIMIZEBOX;
	if ((style & W_MAX) != 0)
		bits |= WS_MAXIMIZEBOX;

	/* Set the resize, dialog border or border bits */
	if ((style & W_RESIZE) != 0) {
		bits |= WS_THICKFRAME;
	} else {
		if ((style & W_BORDER) == 0)
			bits |= WS_BORDER;
	}

	/* Set the system menu and close box bits */
	if ((style & W_CLOSE) != 0)
		bits |= WS_SYSMENU;
	return bits;
}
DWORD _w_shell_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_shell_widget_style_0(control, priv);
	if (_W_WIDGET(control)->handle != 0)
		return bits | WS_CHILD;
	bits &= ~WS_CHILD;
	/*
	 * Use WS_OVERLAPPED for all windows, either dialog or top level
	 * so that CreateWindowEx () will respect CW_USEDEFAULT and set
	 * the default window location and size.
	 *
	 * NOTE:  When a WS_OVERLAPPED window is created, Windows gives
	 * the new window WS_CAPTION style bits.  These two constants are
	 * as follows:
	 *
	 * 	WS_OVERLAPPED = 0
	 * 	WS_CAPTION = WS_BORDER | WS_DLGFRAME
	 *
	 */
	return bits | WS_OVERLAPPED | WS_CAPTION;
}
wresult _w_shell_subclass(w_control *control, _w_control_priv *priv) {
	wresult result = _w_control_subclass(control, priv);
	return result;
}
wresult _w_shell_unsubclass(w_control *control, _w_control_priv *priv) {
	wresult result = _w_control_unsubclass(control, priv);
	return result;
}
wresult _w_shell_get_shell(w_widget *control, w_shell **shell) {
	*shell = W_SHELL(control);
	return W_TRUE;
}
void _w_shell_bring_totop(w_shell *shell) {

}
void _w_shell_close_widget(w_shell *shell, _w_event_platform *e) {
	wuint64 style = w_widget_get_style(W_WIDGET(shell));
	w_event event;
	event.type = W_EVENT_CLOSE;
	event.platform_event = (w_event_platform*) e;
	event.widget = W_WIDGET(shell);
	event.data = 0;
	_w_widget_post_event(W_WIDGET(shell), &event, W_EVENT_SEND);
	if (style & W_DISPOSE_ON_CLOSE) {
		w_widget_dispose(W_WIDGET(shell));
	}
}
wresult _w_shell_close(w_shell *shell) {
	_w_shell_close_widget(shell, 0);
	return W_TRUE;
}
w_cursor* _w_shell_find_cursor(w_control *control, _w_control_priv *priv) {
	return _W_CONTROL(control)->cursor;
}
wresult _w_shell_force_active(w_shell *shell) {
	if (_w_shell_is_visible(W_CONTROL(shell)) <= 0)
		return W_FALSE;
	SetForegroundWindow(_W_WIDGET(shell)->handle);
	return W_TRUE;
}
wresult _w_shell_get_bounds(w_control *control, w_point *location,
		w_size *size) {
	if (IsIconic(_W_WIDGET(control)->handle)) {
		WINDOWPLACEMENT lpwndpl;
		lpwndpl.length = sizeof(lpwndpl);
		GetWindowPlacement(_W_WIDGET(control)->handle, &lpwndpl);
		if ((lpwndpl.flags & WPF_RESTORETOMAXIMIZED) != 0) {
			/*int width = maxRect.right - maxRect.left;
			 int height = maxRect.bottom - maxRect.top;
			 return new Rectangle(maxRect.left, maxRect.top, width, height);*/
		}
		if (location != 0) {
			location->x = lpwndpl.rcNormalPosition.left;
			location->y = lpwndpl.rcNormalPosition.top;
		}
		if (size != 0) {
			size->width = lpwndpl.rcNormalPosition.right
					- lpwndpl.rcNormalPosition.left;
			size->height = lpwndpl.rcNormalPosition.bottom
					- lpwndpl.rcNormalPosition.top;
		}
	} else {
		RECT rect;
		GetWindowRect(_W_WIDGET(control)->handle, &rect);
		if (location != 0) {
			location->x = rect.left;
			location->y = rect.top;
		}
		if (size != 0) {
			size->width = rect.right - rect.left;
			size->height = rect.bottom - rect.top;
		}
	}
	return W_TRUE;
}
wresult _w_shell_get_default_button(w_shell *shell, w_button **button) {
	if (_W_SHELL(shell)->defaultButton != 0
			&& w_widget_is_ok(W_WIDGET(_W_SHELL(shell)->defaultButton)) <= 0) {
		*button = 0;
	} else {
		*button = _W_SHELL(shell)->defaultButton;
	}
	return W_TRUE;
}
wresult _w_shell_get_alpha(w_shell *shell) {
	if (WIN32_VERSION >= VERSION(5, 1)) {
		BYTE pbAlpha[2];
		if (GetLayeredWindowAttributes(_W_WIDGET(shell)->handle, 0, pbAlpha,
				0)) {
			return pbAlpha[0] & 0xFF;
		}
	}
	return 0xFF;
}
wresult _w_shell_get_full_screen(w_shell *shell) {
	return _W_SHELL(shell)->fullScreen;
}
wresult _w_shell_get_minimum_size(w_shell *shell, w_size *size) {
	int width = WMAX(0, _W_SHELL(shell)->minWidth);
	wuint64 style = _W_WIDGET(shell)->style;
	int trim = W_TITLE | W_CLOSE | W_MIN | W_MAX;
	if ((style & W_NO_TRIM) == 0 && (style & trim) != 0) {
		width = WMAX(width, GetSystemMetrics(SM_CXMINTRACK));
	}
	int height = WMAX(0, _W_SHELL(shell)->minHeight);
	if ((style & W_NO_TRIM) == 0 && (style & trim) != 0) {
		if ((style & W_RESIZE) != 0) {
			height = WMAX(height, GetSystemMetrics(SM_CYMINTRACK));
		} else {
			RECT rect;
			int bits1 = GetWindowLongW(_W_WIDGET(shell)->handle, GWL_STYLE);
			int bits2 = GetWindowLongW(_W_WIDGET(shell)->handle, GWL_EXSTYLE);
			AdjustWindowRectEx(&rect, bits1, FALSE, bits2);
			height = WMAX(height, rect.bottom - rect.top);
		}
	}
	size->width = width;
	size->height = height;
	return W_TRUE;
}
wresult _w_shell_get_modified(w_shell *shell) {
	return _W_SHELL(shell)->modified;
}
wresult _w_shell_get_images(w_shell *shell, w_image *image, size_t length) {
	return W_FALSE;
}
wresult _w_shell_get_ime_input_mode(w_shell *shell) {
	if (!win_toolkit->IsDBLocale)
		return 0;
	HIMC hIMC = ImmGetContext(_W_WIDGET(shell)->handle);
	DWORD lpfdwConversion, lpfdwSentence;
	WINBOOL open = ImmGetOpenStatus(hIMC);
	if (open)
		open = ImmGetConversionStatus(hIMC, &lpfdwConversion, &lpfdwSentence);
	ImmReleaseContext(_W_WIDGET(shell)->handle, hIMC);
	if (!open)
		return W_NONE;
	wresult result = 0;
	if ((lpfdwConversion & IME_CMODE_ROMAN) != 0)
		result |= W_ROMAN;
	if ((lpfdwConversion & IME_CMODE_FULLSHAPE) != 0)
		result |= W_DBCS;
	if ((lpfdwConversion & IME_CMODE_KATAKANA) != 0)
		return result | W_PHONETIC;
	if ((lpfdwConversion & IME_CMODE_NATIVE) != 0)
		return result | W_NATIVE;
	return result | W_ALPHA;
}
wresult _w_shell_get_maximized(w_shell *shell) {
	if (!_W_SHELL(shell)->fullScreen) {
		//if (IsWinCE) return swFlags == SW_SHOWMAXIMIZED;
		if (IsWindowVisible(_W_WIDGET(shell)->handle))
			return IsZoomed(_W_WIDGET(shell)->handle);
		return _W_SHELL(shell)->swFlags == SW_SHOWMAXIMIZED;
	}
	return W_FALSE;
}
wresult _w_shell_get_menu_bar(w_shell *shell, w_menu **menu) {
	*menu = _W_SHELL(shell)->menubar;
	return W_TRUE;
}
wresult _w_shell_get_minimized(w_shell *shell) {
	if (IsWindowVisible(_W_WIDGET(shell)->handle))
		return IsIconic(_W_WIDGET(shell)->handle);
	return _W_SHELL(shell)->swFlags == SW_SHOWMINNOACTIVE;
}
wresult _w_shell_get_shells(w_shell *shell, w_iterator *iterator) {
	_w_toolkit_get_shells_from_parent(shell, iterator);
	return W_TRUE;
}
wresult _w_shell_get_text(w_shell *shell, w_alloc alloc, void *user_data,
		int enc) {
	wresult result;
	int length = GetWindowTextLengthW(_W_WIDGET(shell)->handle);
	WCHAR *s = _w_toolkit_malloc(length);
	if (s != 0) {
		GetWindowTextW(_W_WIDGET(shell)->handle, s, length);
		result = _win_text_set(s, length, alloc, user_data, enc);
		_w_toolkit_free(s, length);
		return result;
	} else
		return W_ERROR_NO_MEMORY;
}
wresult _w_shell_get_toolbar(w_shell *shell, w_toolbar **toolbar) {
	*toolbar = 0;
	return W_TRUE;
}
wresult _w_shell_is_visible(w_control *control) {
	return _w_control_get_visible(control);
}
wresult _w_shell_open(w_shell *shell) {
	wresult result = W_TRUE;
	if (win_toolkit->init_startup == 0) {
		STARTUPINFOW lpStartupInfo;
		GetStartupInfoW(&lpStartupInfo);
		if ((lpStartupInfo.dwFlags & STARTF_USESHOWWINDOW) == 0) {
			//bringToTop();
			result = w_widget_is_ok(W_WIDGET(shell));
			if (result <= 0)
				return result;
		}
	}
	SendMessageW(_W_WIDGET(shell)->handle, WM_CHANGEUISTATE, UIS_INITIALIZE, 0);
	W_CONTROL_GET_CLASS(shell)->set_visible(W_CONTROL(shell), W_TRUE);
	result = w_widget_is_ok(W_WIDGET(shell));
	if (result <= 0)
		return result;
	/*
	 * Bug in Windows XP.  Despite the fact that an icon has been
	 * set for a window, the task bar displays the wrong icon the
	 * first time the window is made visible with ShowWindow() after
	 * a call to BringToTop(), when a long time elapses between the
	 * ShowWindow() and the time the event queue is read.  The icon
	 * in the window trimming is correct but the one in the task
	 * bar does not get updated.  The fix is to call PeekMessage()
	 * with the flag PM_NOREMOVE and PM_QS_SENDMESSAGE to respond
	 * to a cross thread WM_GETICON.
	 *
	 * NOTE: This allows other cross thread messages to be delivered,
	 * most notably WM_ACTIVATE.
	 */
	MSG msg;
	int flags = PM_NOREMOVE | PM_NOYIELD | PM_QS_SENDMESSAGE;
	PeekMessageW(&msg, 0, 0, 0, flags);
	return result;
}
wresult _w_shell_set_active(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_set_alpha(w_shell *shell, int alpha) {
	if (WIN32_VERSION >= VERSION(5, 1)) {
		alpha &= 0xFF;
		int bits = GetWindowLong(_W_WIDGET(shell)->handle, GWL_EXSTYLE);
		if (alpha == 0xFF) {
			SetWindowLong(_W_WIDGET(shell)->handle, GWL_EXSTYLE,
					bits & ~WS_EX_LAYERED);
			int flags = RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN;
			RedrawWindow(_W_WIDGET(shell)->handle, 0, 0, flags);
		} else {
			SetWindowLong(_W_WIDGET(shell)->handle, GWL_EXSTYLE,
					bits | WS_EX_LAYERED);
			SetLayeredWindowAttributes(_W_WIDGET(shell)->handle, 0,
					(byte) alpha,
					LWA_ALPHA);
		}
	}
	return W_TRUE;
}
wresult _w_shell_set_bounds(w_control *control, w_point *location, w_size *size,
		int flags, int defer) {
//if (fullScreen) setFullScreen (false);
	/*
	 * Bug in Windows.  When a window has alpha and
	 * SetWindowPos() is called with SWP_DRAWFRAME,
	 * the contents of the window are copied rather
	 * than allowing the windows underneath to draw.
	 * This causes pixel corruption.  The fix is to
	 * clear the SWP_DRAWFRAME bits.
	 */
	HWND handle = _W_WIDGET(control)->handle;
	DWORD bits = GetWindowLongW(handle, GWL_EXSTYLE);
	if ((bits & WS_EX_LAYERED) != 0) {
		flags &= ~SWP_DRAWFRAME;
	}
	return W_FALSE;
}
wresult _w_shell_set_default_button_0(w_shell *shell, w_button *button,
		int save) {
	_w_shell *_shell = _W_SHELL(shell);
	if (button == 0) {
		if (_shell->defaultButton == _shell->saveDefault) {
			if (save)
				_shell->saveDefault = 0;
			return W_TRUE;
		}
	} else {
		if ((_W_WIDGET(button)->style & W_PUSH) == 0)
			return W_TRUE;
		if (button == _shell->defaultButton) {
			if (save)
				_shell->saveDefault = _shell->defaultButton;
			return W_TRUE;
		}
	}
	if (_shell->defaultButton != 0) {
		if (w_widget_is_ok(W_WIDGET(_shell->defaultButton)) > 0) {
			//w_button_set_default(_shell->defaultButton,W_FALSE);
		}
	}
	if ((_shell->defaultButton = button) == 0)
		_shell->defaultButton = _shell->saveDefault;
	if (_shell->defaultButton != 0) {
		if (w_widget_is_ok(W_WIDGET(_shell->defaultButton)) > 0) {
			//w_button_set_default(_shell->defaultButton,W_TRUE);
		}
	}
	if (save)
		_shell->saveDefault = _shell->defaultButton;
	if (_shell->saveDefault != 0
			&& w_widget_is_ok(W_WIDGET(_shell->saveDefault)) <= 0)
		_shell->saveDefault = 0;
	return W_TRUE;
}
wresult _w_shell_set_default_button(w_shell *shell, w_button *button) {
	if (button != 0) {
		if (w_widget_is_ok(W_WIDGET(button)))
			return W_ERROR_INVALID_ARGUMENT;
		w_shell *bshell;
		w_widget_get_shell(W_WIDGET(button), &bshell);
		if (bshell != shell)
			return W_ERROR_INVALID_PARENT;
	}
	return _w_shell_set_default_button_0(shell, button, W_TRUE);
}
wresult _w_shell_set_full_screen(w_shell *shell, int fullScreen) {
	if (_W_SHELL(shell)->fullScreen == fullScreen)
		return W_TRUE;
	HWND handle = _W_WIDGET(shell)->handle;
	wuint64 style = _W_WIDGET(shell)->style;
	int stateFlags = fullScreen ? SW_SHOWMAXIMIZED : SW_RESTORE;
	int styleFlags = GetWindowLong(handle, GWL_STYLE);
	int mask = W_TITLE | W_CLOSE | W_MIN | W_MAX;
	if ((style & mask) != 0) {
		if (fullScreen) {
			styleFlags &= ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
		} else {
			styleFlags |= WS_CAPTION;
			if ((style & W_MAX) != 0)
				styleFlags |= WS_MAXIMIZEBOX;
			if ((style & W_MIN) != 0)
				styleFlags |= WS_MINIMIZEBOX;
		}
	}
	if (fullScreen)
		_W_SHELL(shell)->wasMaximized = _w_shell_get_maximized(shell);
	int visible = _w_shell_is_visible(W_CONTROL(shell));
	SetWindowLong(handle, GWL_STYLE, styleFlags);
	if (_W_SHELL(shell)->wasMaximized) {
		ShowWindow(handle, SW_HIDE);
		stateFlags = SW_SHOWMAXIMIZED;
	}
	if (visible)
		ShowWindow(handle, stateFlags);
	UpdateWindow(handle);
	_W_SHELL(shell)->fullScreen = fullScreen;
	return W_TRUE;
}
wresult _w_shell_set_images(w_shell *shell, w_image *image, size_t length) {
	return W_FALSE;
}
wresult _w_shell_set_ime_input_mode(w_shell *shell, int mode) {
	if (!win_toolkit->IsDBLocale)
		return W_TRUE;
	HWND handle = _W_WIDGET(shell)->handle;
	int imeOn = mode != W_NONE;
	HIMC hIMC = ImmGetContext(handle);
	ImmSetOpenStatus(hIMC, imeOn);
	if (imeOn) {
		DWORD lpfdwConversion, lpfdwSentence;
		if (ImmGetConversionStatus(hIMC, &lpfdwConversion, &lpfdwSentence)) {
			int newBits = 0;
			int oldBits = IME_CMODE_NATIVE | IME_CMODE_KATAKANA;
			if ((mode & W_PHONETIC) != 0) {
				newBits = IME_CMODE_KATAKANA | IME_CMODE_NATIVE;
				oldBits = 0;
			} else {
				if ((mode & W_NATIVE) != 0) {
					newBits = IME_CMODE_NATIVE;
					oldBits = IME_CMODE_KATAKANA;
				}
			}
			int fullShape = (mode & W_DBCS) != 0;
			if ((mode & W_NATIVE) != 0) {
				HKL hkl = GetKeyboardLayout(0);
				int langid = PRIMARYLANGID(LOWORD (hkl));
				if (langid == LANG_JAPANESE) {
					fullShape = TRUE;
				}
			}
			if (fullShape) {
				newBits |= IME_CMODE_FULLSHAPE;
			} else {
				oldBits |= IME_CMODE_FULLSHAPE;
			}
			if ((mode & W_ROMAN) != 0) {
				newBits |= IME_CMODE_ROMAN;
			} else {
				oldBits |= IME_CMODE_ROMAN;
			}
			lpfdwConversion |= newBits;
			lpfdwConversion &= ~oldBits;
			ImmSetConversionStatus(hIMC, lpfdwConversion, lpfdwSentence);
		}
	}
	ImmReleaseContext(handle, hIMC);
	return W_TRUE;
}
wresult _w_shell_set_maximized_0(w_shell *shell, int maximized) {
	_W_SHELL(shell)->swFlags = maximized ? SW_SHOWMAXIMIZED : SW_RESTORE;
	wresult result = W_TRUE;
	HWND handle = _W_WIDGET(shell)->handle;
	if (!IsWindowVisible(handle))
		return result;
	if (maximized == IsZoomed(handle))
		return result;
	ShowWindow(handle, _W_SHELL(shell)->swFlags);
	UpdateWindow(handle);
	return result;
}
wresult _w_shell_set_maximized(w_shell *shell, int maximized) {
	win_toolkit->init_startup = 1;
	return _w_shell_set_maximized_0(shell, maximized);
}
wresult _w_shell_set_menu_bar(w_shell *shell, w_menu *menu) {
	if (_W_SHELL(shell)->menubar == menu)
		return W_TRUE;
	if (menu != 0) {
		if (w_widget_is_ok(W_WIDGET(menu)) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		if ((_W_WIDGET(menu)->style & W_BAR) == 0)
			return W_ERROR_MENU_NOT_BAR;
		if (_W_MENU(menu)->parent != W_CONTROL(shell))
			return W_ERROR_INVALID_PARENT;
	}
	_W_SHELL(shell)->menubar = menu;
	HMENU hMenu = menu != 0 ? _W_MENU(menu)->handle : 0;
	wresult result;
	if (SetMenu(_W_WIDGET(shell)->handle, hMenu)) {
		result = W_TRUE;
	} else
		result = W_ERROR_CANNOT_SET_MENU;
	_w_control_destroy_accelerators(W_CONTROL(shell));
	return result;
}
wresult _w_shell_set_minimized_0(w_shell *shell, int minimized) {
	_W_SHELL(shell)->swFlags = minimized ? SW_SHOWMINNOACTIVE : SW_RESTORE;
	wresult result = W_TRUE;
	HWND handle = _W_WIDGET(shell)->handle;
	if (!IsWindowVisible(handle))
		return result;
	if (minimized == IsIconic(handle))
		return result;
	int flags = _W_SHELL(shell)->swFlags;
	if (flags == SW_SHOWMINNOACTIVE && handle == GetActiveWindow()) {
		flags = SW_MINIMIZE;
	}
	ShowWindow(handle, flags);
	UpdateWindow(handle);
	return result;
}
wresult _w_shell_set_minimized(w_shell *shell, int minimized) {
	win_toolkit->init_startup = 1;
	return _w_shell_set_minimized_0(shell, minimized);
}
wresult _w_shell_set_minimum_size(w_shell *shell, w_size *size) {
	int widthLimit = 0, heightLimit = 0;
	HWND handle = _W_WIDGET(shell)->handle;
	wuint64 style = _W_WIDGET(shell)->style;
	int trim = W_TITLE | W_CLOSE | W_MIN | W_MAX;
	if ((style & W_NO_TRIM) == 0 && (style & trim) != 0) {
		widthLimit = GetSystemMetrics(SM_CXMINTRACK);
		if ((style & W_RESIZE) != 0) {
			heightLimit = GetSystemMetrics(SM_CYMINTRACK);
		} else {
			RECT rect;
			int bits1 = GetWindowLong(handle, GWL_STYLE);
			int bits2 = GetWindowLong(handle, GWL_EXSTYLE);
			AdjustWindowRectEx(&rect, bits1, FALSE, bits2);
			heightLimit = rect.bottom - rect.top;
		}
	}
	_W_SHELL(shell)->minWidth = WMAX(widthLimit, size->width);
	_W_SHELL(shell)->minHeight = WMAX(heightLimit, size->height);
	w_size _sz, newsz;
	_w_shell_get_bounds(W_CONTROL(shell), 0, &_sz);
	newsz.width = WMAX(_sz.width, _W_SHELL(shell)->minWidth);
	newsz.height = WMAX(_sz.height, _W_SHELL(shell)->minHeight);
	if (_W_SHELL(shell)->minWidth <= widthLimit)
		_W_SHELL(shell)->minWidth = W_DEFAULT;
	if (_W_SHELL(shell)->minHeight <= heightLimit)
		_W_SHELL(shell)->minHeight = W_DEFAULT;
	if (newsz.width != _sz.width || newsz.height != _sz.height) {
		W_CONTROL_GET_CLASS(shell)->set_bounds(W_CONTROL(shell), 0, &newsz);
	}
	return W_TRUE;
}
wresult _w_shell_set_modified(w_shell *shell, int modified) {
	_W_SHELL(shell)->modified = modified;
	return W_TRUE;
}
wresult _w_shell_set_text(w_shell *shell, const char *text, int length,
		int enc) {
	/* Ensure that the title appears in the task bar.*/
	if (text == 0)
		text = "";
	int newlength;
	wresult result = W_TRUE;
	WCHAR *s;
	_win_text_fix(text, length, enc, &s, &newlength);
	if ((_W_WIDGET(shell)->state & STATE_FOREIGN_HANDLE) != 0) {
		DefWindowProcW(_W_WIDGET(shell)->handle, WM_SETTEXT, 0, (LPARAM) s);
	} else {
		result = SetWindowTextW(_W_WIDGET(shell)->handle, s);
	}
	_win_text_free(text, s, newlength);
	return result;
}
void _w_shell_set_tooltip_title(w_shell *shell, HWND hwndToolTip, char *text,
		int icon) {
	/*
	 * Bug in Windows.  For some reason, when TTM_SETTITLE
	 * is used to set the title of a tool tip, Windows leaks
	 * GDI objects.  This happens even when TTM_SETTITLE is
	 * called with TTI_NONE and NULL.  The documentation
	 * states that Windows copies the icon and that the
	 * programmer must free the copy but does not provide
	 * API to get the icon.  For example, when TTM_SETTITLE
	 * is called with ICON_ERROR, when TTM_GETTITLE is used
	 * to query the title and the icon, the uTitleBitmap
	 * field in the TTGETTITLE struct is zero.  The fix
	 * is to remember these values, only set them when then
	 * change and leak less.
	 *
	 * NOTE:  This only happens on Vista.
	 */
	if (hwndToolTip != _W_SHELL(shell)->toolTipHandle
			&& hwndToolTip != _W_SHELL(shell)->balloonTipHandle
			&& hwndToolTip != _W_SHELL(shell)->menuItemToolTipHandle) {
		return;
	}
	if (text != 0) {
		/*
		 * Feature in Windows. The text point to by pszTitle
		 * must not exceed 100 characters in length, including
		 * the null terminator.
		 */
		WCHAR pszTitle[99];
		w_utf8_to_utf16(text, -1, pszTitle,
				sizeof(pszTitle) / sizeof(pszTitle[0]));
		pszTitle[98] = 0;
		SendMessageW(hwndToolTip, TTM_SETTITLE, icon, (LPARAM) pszTitle);
	} else {
		SendMessageW(hwndToolTip, TTM_SETTITLE, 0, 0);
	}
}

wresult _w_shell_translate_accelerator(w_control *control, MSG *msg,
		_w_control_priv *priv) {
	return _w_control_translate_accelerator(control, msg, priv);
}
/*
 * messages
 */
wresult _SHELL_WM_CLOSE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
//if (w_control_is_enabled(W_CONTROL(widget)) > 0
//		&& _w_shell_is_active(W_SHELL(widget)) > 0) {
	_w_shell_close_widget(W_SHELL(widget), e);
//}
	e->result = 0;
	return W_FALSE;
}
wresult _SHELL_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_NCACTIVATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_QUERYOPEN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _CANVAS_WM_SIZE(widget, e, priv);
	return result;
}
wresult _SHELL_WM_SYSCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_ACTIVATE_0(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_ACTIVATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_DESTROY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _CONTROL_WM_DESTROY(widget, e, priv);
	_w_toolkit_remove_shell(_W_SHELL(widget));
	if (win_toolkit->shells == 0) {
		w_toolkit_post_quit(W_TOOLKIT(win_toolkit), EXIT_SUCCESS);
	}
	return result;
}
wresult _SHELL_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_ENTERIDLE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_GETMINMAXINFO(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_MOUSEACTIVATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_MOVE_0(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_MOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_NCHITTEST(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_NCLBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_PALETTECHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_QUERYNEWPALETTE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_SHOWWINDOW(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_WINDOWPOSCHANGING_0(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SHELL_WM_WINDOWPOSCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
void _w_shell_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_shell_class *clazz) {
	if (classId == _W_CLASS_SHELL) {
		W_WIDGET_CLASS(clazz)->platformPrivate = &win_toolkit->class_shell_priv;
	}
	_w_canvas_class_init(toolkit, classId, W_CANVAS_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SHELL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_shell_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_shell);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_shell);
	/*
	 * functions
	 */
	W_WIDGET_CLASS(clazz)->create = _w_shell_create;
	W_WIDGET_CLASS(clazz)->get_shell = _w_shell_get_shell;
	clazz->close = _w_shell_close;
	clazz->get_toolbar = _w_shell_get_toolbar;
	clazz->get_alpha = _w_shell_get_alpha;
	clazz->get_full_screen = _w_shell_get_full_screen;
	clazz->get_minimum_size = _w_shell_get_minimum_size;
	clazz->get_modified = _w_shell_get_modified;
	clazz->get_ime_input_mode = _w_shell_get_ime_input_mode;
	clazz->get_shells = _w_shell_get_shells;
	clazz->open = _w_shell_open;
	clazz->set_active = _w_shell_set_active;
	clazz->set_alpha = _w_shell_set_alpha;
	clazz->set_full_screen = _w_shell_set_full_screen;
	clazz->set_ime_input_mode = _w_shell_set_ime_input_mode;
	clazz->set_minimum_size = _w_shell_set_minimum_size;
	clazz->set_modified = _w_shell_set_modified;
	clazz->force_active = _w_shell_force_active;
	clazz->get_default_button = _w_shell_get_default_button;
	clazz->get_images = _w_shell_get_images;
	clazz->get_maximized = _w_shell_get_maximized;
	clazz->get_menu_bar = _w_shell_get_menu_bar;
	clazz->get_minimized = _w_shell_get_minimized;
	clazz->get_text = _w_shell_get_text;
	clazz->set_default_button = _w_shell_set_default_button;
	clazz->set_images = _w_shell_set_images;
	clazz->set_maximized = _w_shell_set_maximized;
	clazz->set_menu_bar = _w_shell_set_menu_bar;
	clazz->set_minimized = _w_shell_set_minimized;
	clazz->set_text = _w_shell_set_text;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_SHELL) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->widget.call_window_proc = _w_shell_call_window_proc;
		priv->check_style = _w_shell_check_style;
		priv->create_handle = _w_shell_create_handle;
		priv->window_class = _w_shell_window_class;
		priv->widget_parent = _w_shell_widget_parent;
		priv->widget_extstyle = _w_shell_widget_extstyle;
		priv->widget_style = _w_shell_widget_style;
		priv->subclass = _w_shell_subclass;
		priv->unsubclass = _w_shell_unsubclass;
		priv->find_cursor = _w_shell_find_cursor;
		priv->translate_accelerator = _w_shell_translate_accelerator;
		/*
		 * messages
		 */
		dispatch_message *msg = priv->messages;
		msg[_WM_CLOSE] = _SHELL_WM_CLOSE;
		msg[_WM_KILLFOCUS] = _SHELL_WM_KILLFOCUS;
		msg[_WM_NCACTIVATE] = _SHELL_WM_NCACTIVATE;
		msg[_WM_QUERYOPEN] = _SHELL_WM_QUERYOPEN;
		msg[_WM_SETFOCUS] = _SHELL_WM_SETFOCUS;
		msg[_WM_SIZE] = _SHELL_WM_SIZE;
		msg[_WM_SYSCOMMAND] = _SHELL_WM_SYSCOMMAND;
		msg[_WM_ACTIVATE] = _SHELL_WM_ACTIVATE;
		msg[_WM_DESTROY] = _SHELL_WM_DESTROY;
		msg[_WM_ERASEBKGND] = _SHELL_WM_ERASEBKGND;
		msg[_WM_ENTERIDLE] = _SHELL_WM_ENTERIDLE;
		msg[_WM_GETMINMAXINFO] = _SHELL_WM_GETMINMAXINFO;
		msg[_WM_MOUSEACTIVATE] = _SHELL_WM_MOUSEACTIVATE;
		msg[_WM_MOVE] = _SHELL_WM_MOVE;
		msg[_WM_NCHITTEST] = _SHELL_WM_NCHITTEST;
		msg[_WM_NCLBUTTONDOWN] = _SHELL_WM_NCLBUTTONDOWN;
		msg[_WM_PALETTECHANGED] = _SHELL_WM_PALETTECHANGED;
		msg[_WM_QUERYNEWPALETTE] = _SHELL_WM_QUERYNEWPALETTE;
		msg[_WM_SETCURSOR] = _SHELL_WM_SETCURSOR;
		msg[_WM_SHOWWINDOW] = _SHELL_WM_SHOWWINDOW;
		msg[_WM_WINDOWPOSCHANGING] = _SHELL_WM_WINDOWPOSCHANGING;
	}
}
