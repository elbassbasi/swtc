/*
 * Name:        tooltip.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
wuint64 _w_tooltip_check_style(w_widget *widget, wuint64 style) {
	int mask = W_ICON_ERROR | W_ICON_INFORMATION | W_ICON_WARNING;
	if ((style & mask) == 0)
		return style;
	return _w_widget_check_bits(style, W_ICON_INFORMATION, W_ICON_WARNING,
			W_ICON_ERROR, 0, 0, 0);
}
wresult _w_tooltip_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (w_widget_is_ok(parent) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (w_widget_class_id(parent) < _W_CLASS_SHELL)
		return W_ERROR_INVALID_ARGUMENT;
	_W_TOOLTIP(widget)->parent = W_SHELL(parent);
	_W_WIDGET(widget)->post_event = post_event;
	_W_WIDGET(widget)->style = _w_tooltip_check_style(widget, style);
	return _w_shell_create_tooltip(W_SHELL(parent), _W_TOOLTIP(widget));
}
HWND _w_tooltip_hwnd_tooltip(w_tooltip *tooltip) {
	HWND handle = 0;
	if ((_W_WIDGET(tooltip)->style & W_BALLOON) != 0) {
		_w_shell_balloontip_handle(_W_TOOLTIP(tooltip)->parent, &handle);
	} else {
		_w_shell_tooltip_handle(_W_TOOLTIP(tooltip)->parent, &handle);
	}
	return handle;
}
wresult _w_tooltip_post_event(w_widget *widget, w_event *e,int flags) {
	return _w_control_post_event(widget, e,flags);
}
wresult _w_tooltip_get_auto_hide(w_tooltip *tooltip) {
	return _W_TOOLTIP(tooltip)->autoHide;
}
wresult _w_tooltip_get_message(w_tooltip *tooltip, w_alloc alloc,
		void *user_data, int enc) {
	return _win_text_set_0(_W_TOOLTIP(tooltip)->message, -1, alloc, user_data,
			enc);
}
wresult _w_tooltip_get_parent(w_tooltip *tooltip, w_shell **parent) {
	*parent = _W_TOOLTIP(tooltip)->parent;
	return W_TRUE;
}
wresult _w_tooltip_get_text(w_tooltip *tooltip, w_alloc alloc, void *user_data,
		int enc) {
	return _win_text_set_0(_W_TOOLTIP(tooltip)->text, -1, alloc, user_data, enc);
}
wresult _w_tooltip_get_visible(w_tooltip *tooltip) {
	if (_W_TOOLTIP(tooltip)->item != 0)
		return _W_TOOLTIP(tooltip)->visible;
	HWND hwndToolTip = _w_tooltip_hwnd_tooltip(tooltip);
	if (SendMessageW(hwndToolTip, TTM_GETCURRENTTOOLW, 0, 0) != 0) {
		TOOLINFOW lpti;
		lpti.cbSize = sizeof(lpti);
		if (SendMessageW(hwndToolTip, TTM_GETCURRENTTOOLW, 0, (LPARAM) &lpti)
				!= 0) {
			return (lpti.uFlags & TTF_IDISHWND) == 0
					&& lpti.uId == _W_TOOLTIP(tooltip)->id;
		}
	}
	return W_FALSE;
}

int _w_tooltip_get_width(w_tooltip *tooltip) {
	_w_tooltip *_tooltip = _W_TOOLTIP(tooltip);
	w_shell *parent = _tooltip->parent;
	HWND hwnd = _W_WIDGET(parent)->handle;
	int maxWidth = 0;
	if (WIN32_VERSION < VERSION(4, 10)) {
		RECT rect;
		SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
		maxWidth = rect.right - rect.left;
	} else {
		HMONITOR hmonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO lpmi;
		lpmi.cbSize = sizeof(lpmi);
		GetMonitorInfoW(hmonitor, &lpmi);
		maxWidth = lpmi.rcWork.right - lpmi.rcWork.left;
	}
	return maxWidth /= 4;
}
wresult _w_tooltip_is_visible(w_tooltip *tooltip) {
	if (_W_TOOLTIP(tooltip)->item != 0) {
		return _w_tooltip_get_visible(tooltip)
				&& w_trayitem_get_visible(_W_TOOLTIP(tooltip)->item);
	}
	return _w_tooltip_get_visible(tooltip);
}
wresult _w_tooltip_set_autohide(w_tooltip *tooltip, int autoHide) {
	_W_TOOLTIP(tooltip)->autoHide = autoHide;
	return W_TRUE;
}
wresult _w_tooltip_set_location(w_tooltip *tooltip, w_point *location) {
	_W_TOOLTIP(tooltip)->hasLocation = W_TRUE;
	_W_TOOLTIP(tooltip)->location.x = location->x;
	_W_TOOLTIP(tooltip)->location.y = location->y;
	return W_TRUE;
}
wresult _w_tooltip_set_message(w_tooltip *tooltip, const char *message,
		int length, int enc) {
	wresult result = _win_text_copy(&_W_TOOLTIP(tooltip)->message, message,
			length, enc);
	_w_tooltip_update_message(tooltip);
	return result;
}
wresult _w_tooltip_set_text(w_tooltip *tooltip, const char *text, int length,
		int enc) {
	return _win_text_copy(&_W_TOOLTIP(tooltip)->text, text, length, enc);
}
wresult _w_tooltip_set_visible(w_tooltip *tooltip, int visible) {
	_w_tooltip *_tooltip = _W_TOOLTIP(tooltip);
	w_shell *parent = _tooltip->parent;
	HWND hwnd = _W_WIDGET(parent)->handle;
	void *item = _W_TOOLTIP(tooltip)->item;
	wuint64 style = _W_WIDGET(tooltip)->style;
	if (item == 0) {
		TOOLINFOW lpti;
		lpti.cbSize = sizeof(TOOLINFOW);
		lpti.uId = _tooltip->id;
		lpti.hwnd = hwnd;
		HWND hwndToolTip = _w_tooltip_hwnd_tooltip(tooltip);
		if (_tooltip->text != 0) {
			int icon = TTI_NONE;
			if ((style & W_ICON_INFORMATION) != 0)
				icon = TTI_INFO;
			if ((style & W_ICON_WARNING) != 0)
				icon = TTI_WARNING;
			if ((style & W_ICON_ERROR) != 0)
				icon = TTI_ERROR;
			_w_shell_set_tooltip_title(parent, hwndToolTip,
			_W_TOOLTIP(tooltip)->text, icon);
		} else {
			_w_shell_set_tooltip_title(parent, hwndToolTip, 0, 0);
		}
		SendMessageW(hwndToolTip, TTM_SETMAXTIPWIDTH, 0,
				_w_tooltip_get_width(tooltip));
		if (visible) {
			int nX = _tooltip->location.x;
			int nY = _tooltip->location.y;
			if (!_tooltip->hasLocation) {
				POINT pt;
				if (GetCursorPos(&pt)) {
					nX = pt.x;
					nY = pt.y;
				}
			}
			LPARAM lParam = MAKELPARAM(nX, nY);
			SendMessageW(hwndToolTip, TTM_TRACKPOSITION, 0, lParam);
			/*
			 * Feature in Windows.  Windows will not show a tool tip
			 * if the cursor is outside the parent window (even on XP,
			 * TTM_POPUP will not do this).  The fix is to temporarily
			 * move the cursor into the tool window, show the tool tip,
			 * and then restore the cursor.
			 */
			POINT pt;
			RECT rect;
			GetCursorPos(&pt);
			GetClientRect(hwnd, &rect);
			MapWindowPoints(hwnd, 0, (LPPOINT) &rect, 2);
			if (!PtInRect(&rect, pt)) {
				HCURSOR hCursor = GetCursor();
				SetCursor(0);
				SetCursorPos(rect.left, rect.top);
				SendMessageW(hwndToolTip, TTM_TRACKACTIVATE, 1, (LPARAM) &lpti);
				SetCursorPos(pt.x, pt.y);
				SetCursor(hCursor);
			} else {
				SendMessageW(hwndToolTip, TTM_TRACKACTIVATE, 1, (LPARAM) &lpti);
			}

			int time = SendMessageW(hwndToolTip, TTM_GETDELAYTIME, TTDT_AUTOPOP,
					0);
			SetTimer(hwndToolTip, TIMER_ID, time, 0);
			_w_tooltip_update_message(tooltip);
		} else {
			SendMessageW(hwndToolTip, TTM_TRACKACTIVATE, 0, (LPARAM) &lpti);
			SendMessageW(hwndToolTip, TTM_POP, 0, 0);
			KillTimer(hwndToolTip, TIMER_ID);
		}
		return W_TRUE;
	}
	if (item != 0 && win_toolkit->shell32_version >= VERSION(5, 0)) {
		if (visible) {
			NOTIFYICONDATAW iconData;
			WCHAR *szInfoTitle = iconData.szInfoTitle;
			WCHAR *szInfo = iconData.szInfo;
			w_utf8_to_utf16(_tooltip->text, -1, iconData.szInfoTitle,
					sizeof(iconData.szInfoTitle)
							/ sizeof(iconData.szInfoTitle[0]));
			w_utf8_to_utf16(_tooltip->message, -1, iconData.szInfo,
					sizeof(iconData.szInfo) / sizeof(iconData.szInfo[0]));
			iconData.cbSize = sizeof(iconData);
			iconData.uID = _W_TRAYITEM(item)->id;
			iconData.hWnd = win_toolkit->hwndMessage;
			iconData.uFlags = NIF_INFO;
			if ((style & W_ICON_INFORMATION) != 0)
				iconData.dwInfoFlags = NIIF_INFO;
			if ((style & W_ICON_WARNING) != 0)
				iconData.dwInfoFlags = NIIF_WARNING;
			if ((style & W_ICON_ERROR) != 0)
				iconData.dwInfoFlags = NIIF_ERROR;
			w_event e;
			e.type = W_EVENT_SHOW;
			e.widget = W_WIDGET(tooltip);
			e.time = 0;
			e.data = 0;
			e.platform_event = 0;
			_w_widget_post_event(W_WIDGET(tooltip), &e, W_EVENT_SEND);
			_tooltip->visible = Shell_NotifyIconW(NIM_MODIFY, &iconData);
		} else {
		}
		return W_TRUE;
	}
	return W_FALSE;
}
void _w_tooltip_update_message(w_tooltip *tooltip) {
	HWND hwnd = _w_tooltip_hwnd_tooltip(tooltip);
	if (SendMessageW(hwnd, TTM_GETCURRENTTOOLW, 0, 0) != 0) {
		TOOLINFOW lpti;
		lpti.cbSize = sizeof(lpti);
		if (SendMessageW(hwnd, TTM_GETCURRENTTOOLW, 0, (LPARAM) &lpti) != 0) {
			WCHAR *str;
			int newlength;
			_win_text_fix(_W_TOOLTIP(tooltip)->message, -1, W_ENCODING_UTF8,
					&str, &newlength);
			if (str != 0) {
				lpti.lpszText = str;
				SendMessageW(hwnd, TTM_UPDATETIPTEXTW, 0, (LPARAM) &lpti);
			}
			_win_text_free(_W_TOOLTIP(tooltip)->message, str, newlength);
		}
	}
}
void _w_tooltip_class_init(w_toolkit *toolkit, wushort classId,struct _w_tooltip_class *clazz) {
	if (classId == _W_CLASS_TOOLTIP) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_tooltip_priv;
	}
	_w_widget_class_init(toolkit,classId,W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TOOLTIP;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tooltip_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tooltip);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tooltip);
	/*
	 * public function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_tooltip_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_tooltip_create;
}
