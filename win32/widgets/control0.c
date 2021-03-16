/*
 * Name:        control0.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "control.h"
#include "toolkit.h"
/*
 * function
 */
wresult _w_control_call_window_proc(w_widget *widget, _w_event_platform *e,
		_w_widget_priv *priv) {
	if (_W_WIDGET(widget)->handle == 0) {
		e->result = 0;
		return W_FALSE;
	} else {
		if (_W_CONTROL_PRIV(priv)->def_window_proc != _w_control_window_proc) {
			e->result = CallWindowProcW(_W_CONTROL_PRIV(priv)->def_window_proc,
					e->hwnd, e->msg, e->wparam, e->lparam);
		} else {
			e->result = DefWindowProcW(e->hwnd, e->msg, e->wparam, e->lparam);
		}
		return W_TRUE;
	}
}
wresult _w_control_check_background(w_control *control, _w_control_priv *priv) {
	return W_TRUE;
}
wresult _w_control_check_border(w_control *control, _w_control_priv *priv) {
	return W_TRUE;
}
wresult _w_control_check_buffered(w_control *control, _w_control_priv *priv) {
	return W_TRUE;
}
wresult _w_control_check_composited(w_control *control, _w_control_priv *priv) {
	return W_TRUE;
}
wresult _w_control_check_gesture(w_control *control, _w_control_priv *priv) {
	return W_TRUE;
}
wresult _w_control_check_mirrored(w_control *control, _w_control_priv *priv) {
	return W_TRUE;
}
wresult _w_control_check_orientation(w_control *control, w_composite *parent,
		_w_control_priv *priv) {
	return W_TRUE;
}
wuint64 _w_control_check_style(w_widget *widget, wuint64 style) {
	return style;
}
wresult _w_control_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	e->size->width = e->wHint, e->size->height = e->hHint;
	if (e->wHint == W_DEFAULT)
		e->size->width = 40;
	if (e->hHint == W_DEFAULT)
		e->size->height = 40;
	return W_TRUE;
}
wresult _w_control_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	return FALSE;
}
wresult _w_control_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (w_widget_is_ok(parent) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (w_widget_class_id(parent) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_ARGUMENT;
	_W_CONTROL(widget)->parent = W_COMPOSITE(parent);
	_W_WIDGET(widget)->post_event = post_event;
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(widget);
	_W_WIDGET(widget)->style = priv->check_style(widget, style);
	return priv->create_widget(W_CONTROL(widget), priv);
}
wresult _w_control_create_dragsource(w_control *control,
		w_dragsource *dragsource, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_FALSE;
}
wresult _w_control_create_droptarget(w_control *control,
		w_droptarget *droptarget, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_FALSE;
}
wresult _w_control_create_handle(w_control *control, _w_control_priv *priv) {
	HWND hwndParent = priv->widget_parent(control, priv);
	const char *_clazz = priv->window_class(control, priv);
	DWORD style, extstyle;
	style = priv->widget_style(control, priv);
	extstyle = priv->widget_extstyle(control, priv);
	HWND handle = _w_control_create_window(extstyle, _clazz, style, hwndParent,
			0);
	if (handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_WIDGET(control)->handle = handle;
	w_composite *parent = _W_CONTROL(control)->parent;
	if (parent != 0) {
		_W_COMPOSITE(parent)->children_count++;
	}
	_W_WIDGET(control)->handle = handle;
	SetWindowLongPtrW(_W_WIDGET(control)->handle, GWLP_USERDATA,
			(LONG_PTR) control);
	priv->subclass(control, priv);
	if (win_toolkit->IsDBLocale && hwndParent != 0) {
		HIMC hIMC = ImmGetContext(hwndParent);
		ImmAssociateContext(handle, hIMC);
		ImmReleaseContext(hwndParent, hIMC);
	}
	return W_TRUE;
}
wresult _w_control_create_widget(w_control *control, _w_control_priv *priv) {
	_W_WIDGET(control)->state |= STATE_DRAG_DETECT;
	priv->check_orientation(control, _W_CONTROL(control)->parent, priv);
	wresult result = priv->create_handle(control, priv);
	if (result <= 0)
		return result;
	priv->check_background(control, priv);
	priv->check_buffered(control, priv);
	priv->check_composited(control, priv);
	priv->set_default_font(control, priv);
	priv->check_mirrored(control, priv);
	priv->check_border(control, priv);
	priv->check_gesture(control, priv);
	if ((_W_WIDGET(control)->state & STATE_PARENT_BACKGROUND) != 0) {
		priv->set_background(control, priv);
	}
	return result;
}
HWND _w_control_create_window(DWORD dwExStyle, const char *lpClassName,
		DWORD dwStyle, HWND hWndParent, LPVOID lpParam) {
	int newlength;
	WCHAR *clazz;
	_win_text_fix(lpClassName, -1, W_ENCODING_UTF8, &clazz, &newlength);
	HWND hwnd = CreateWindowExW(dwExStyle, clazz, 0, dwStyle, CW_USEDEFAULT, 0,
	CW_USEDEFAULT, 0, hWndParent, 0, hinst, lpParam);
	_win_text_free(lpClassName, clazz, newlength);
	return hwnd;
}
wresult _w_control_dispose(w_widget *widget) {
	if (widget->clazz != 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(widget);
		HWND tophandle = priv->handle_top(W_CONTROL(widget));
		DestroyWindow(tophandle);
	}
	return W_TRUE;
}
wresult _w_control_drag_detect(w_control *control, w_event_mouse *event) {
	return W_FALSE;
}
w_cursor* _w_control_find_cursor(w_control *control, _w_control_priv *priv) {
	if (_W_CONTROL(control)->cursor != 0)
		return _W_CONTROL(control)->cursor;
	w_composite *parent = _W_CONTROL(control)->parent;
	_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
	return ppriv->find_cursor(W_CONTROL(parent), ppriv);
}
wresult _w_control_force_focus(w_control *control) {
	SetFocus(_W_WIDGET(control)->handle);
	return W_TRUE;
}
wresult _w_control_get_accessible(w_control *control,
		w_accessible **accessible) {
	return W_FALSE;
}
wresult _w_control_get_background(w_control *control, w_color *background) {
	return W_FALSE;
}
wresult _w_control_get_background_image(w_control *control, w_image *image) {
	return W_FALSE;
}
wresult _w_control_get_border_width(w_control *control) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	HWND borderHandle = priv->handle_border(control);
	int bits1 = GetWindowLong(borderHandle, GWL_EXSTYLE);
	if ((bits1 & WS_EX_CLIENTEDGE) != 0)
		return GetSystemMetrics(SM_CXEDGE);
	if ((bits1 & WS_EX_STATICEDGE) != 0)
		return GetSystemMetrics(SM_CXBORDER);
	int bits2 = GetWindowLong(borderHandle, GWL_STYLE);
	if ((bits2 & WS_BORDER) != 0)
		return GetSystemMetrics(SM_CXBORDER);
	return 0;
}
wresult _w_control_get_bounds(w_control *control, w_point *location,
		w_size *size) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	HWND topHandle = priv->handle_top(control);
	//forceResize ();
	RECT r;
	GetWindowRect(topHandle, &r);
	HWND hwndParent = GetParent(topHandle);
	MapWindowPoints(0, hwndParent, (LPPOINT) &r, 2);
	if (location != 0) {
		location->x = r.left;
		location->y = r.top;
	}
	if (size != 0) {
		size->width = r.right - r.left;
		size->height = r.bottom - r.top;
	}
	return W_TRUE;
}
wresult _w_control_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	return FALSE;
}
wresult _w_control_get_cursor(w_control *control, w_cursor **cursor) {
	if (_W_CONTROL(control)->cursor == 0) {
		*cursor = _w_toolkit_get_system_cursor(W_TOOLKIT(win_toolkit),
				W_CURSOR_ARROW);
	} else {
		*cursor = _W_CONTROL(control)->cursor;
	}
	return W_TRUE;
}
wresult _w_control_get_drag_detect(w_control *control) {
	return (_W_WIDGET(control)->state & STATE_DRAG_DETECT) != 0;
}
wresult _w_control_get_drawing(w_control *control) {
	return _W_CONTROL(control)->drawCount <= 0;
}
wresult _w_control_get_enabled(w_control *control) {
	return IsWindowEnabled(_W_WIDGET(control)->handle);
}
wresult _w_control_get_font(w_control *control, w_font **font) {
	if (_W_WIDGET(control)->state & STATE_SET_FONT) {
		*font = _W_CONTROL(control)->font;
	} else {
		HFONT hFont = (HFONT) SendMessageW(_W_WIDGET(control)->handle,
		WM_GETFONT, 0, 0);
		if (hFont == 0) {
			*font = w_toolkit_get_system_font(
					w_widget_get_toolkit(W_WIDGET(control)));
		} else {
			_W_CONTROL(control)->_font.handle = hFont;
			*font = (w_font*) & _W_CONTROL(control)->_font;
		}
	}
	return W_TRUE;
}
wresult _w_control_get_foreground(w_control *control, w_color *foreground) {
	return W_FALSE;
}
wresult _w_control_get_graphics(w_control *control, w_graphics *gc) {
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_graphics_dispose(gc);
	HDC hdc = GetDC(_W_WIDGET(control)->handle);
	if (hdc == 0)
		return W_ERROR_NO_HANDLES;
	_w_graphics_init(gc, hdc);
	_W_GRAPHICS(gc)->state |= GRAPHICS_STATE_RELEASEDC;
	_W_GRAPHICS(gc)->hwnd = _W_WIDGET(control)->handle;
	return W_TRUE;
}
wresult _w_control_get_layout_data(w_control *control, void **data) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(control);
	if ((_W_WIDGET(control)->state & STATE_LAYOUT_DATA_LOCALE) == 0) {
		*data = *((void**) &((char*) control)[clazz->object_used_size]);
	} else {
		*data = (void*) &((char*) control)[clazz->object_used_size];
	}
	return W_TRUE;
}
wresult _w_control_get_menu(w_control *control, w_menu **menu) {
	*menu = _W_CONTROL(control)->menu;
	return W_TRUE;
}
wresult _w_control_get_orientation(w_control *control) {
	return _W_WIDGET(control)->style & (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT);
}
wresult _w_control_get_parent(w_control *control, w_composite **parent) {
	*parent = _W_CONTROL(control)->parent;
	return W_TRUE;
}
wresult _w_control_get_region(w_control *control, w_region *region) {
	return W_FALSE;
}
wresult _w_control_get_shell(w_control *control, w_shell **shell) {
	w_composite *parent = _W_CONTROL(control)->parent;
	return W_CONTROL_GET_CLASS(parent)->get_shell(W_CONTROL(parent), shell);
}
wresult _w_control_get_tab(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_text_direction(w_control *control) {
	int flags = WS_EX_LAYOUTRTL | WS_EX_RTLREADING;
	int bits = GetWindowLongW(_W_WIDGET(control)->handle, GWL_EXSTYLE) & flags;
	return bits == 0 || bits == flags ? W_LEFT_TO_RIGHT : W_RIGHT_TO_LEFT;
}
wresult _w_control_get_tooltip_text(w_control *control, w_alloc alloc,
		void *user_data, int enc) {
	return W_FALSE;
}
wresult _w_control_get_touch_enabled(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_visible(w_control *control) {
	int bits = GetWindowLongW(_W_WIDGET(control)->handle, GWL_STYLE);
	return (bits & WS_VISIBLE) != 0;
}
HWND _w_control_handle(w_control *control) {
	return _W_WIDGET(control)->handle;
}
void _w_control_init_graphics(w_control *control, w_graphics *gc, HDC hDC,
		int style) {
	_w_graphics_init(gc, hDC);
	if (WIN32_VERSION >= VERSION(4, 10)) {
		int mask = W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT;
		if ((style & mask) != 0) {
			//data.layout = (style & W_RIGHT_TO_LEFT) != 0 ? LAYOUT_RTL : 0;
		} else {
			int flags = GetLayout(hDC);
			if ((flags & LAYOUT_RTL) != 0) {
				_W_GRAPHICS(gc)->state |= GRAPHICS_STATE_RIGHT_TO_LEFT
						| GRAPHICS_STATE_MIRRORED;
			} else {
				_W_GRAPHICS(gc)->state |= GRAPHICS_STATE_LEFT_TO_RIGHT;
			}
		}
	} else {
		_W_GRAPHICS(gc)->state |= GRAPHICS_STATE_LEFT_TO_RIGHT;
	}
	/*int foreground = getForegroundPixel ();
	 if (foreground != GetTextColor (hDC)){
	 data.foreground = foreground;
	 }
	 w_control* _control = findBackgroundControl ();
	 if (_control == 0) _control = control;
	 int background = _control.getBackgroundPixel ();
	 if (background != GetBkColor (hDC)){
	 data.background = background;
	 }*/
	w_font *font;
	w_control_get_font(control, &font);
	w_graphics_set_font(gc, font);
	int uiState = (int) SendMessageW(_W_WIDGET(control)->handle,
	WM_QUERYUISTATE, 0, 0);
	if (uiState & UISF_HIDEFOCUS) {
		_W_GRAPHICS(gc)->state |= GRAPHICS_STATE_UISF_HIDEFOCUS;
	}
	if (uiState & UISF_HIDEACCEL) {
		_W_GRAPHICS(gc)->state |= GRAPHICS_STATE_UISF_HIDEACCEL;
	}
}
wresult _w_control_is_enabled(w_control *control) {
	return IsWindowEnabled(_W_WIDGET(control)->handle);
}
wresult _w_control_is_focus_control(w_control *control) {
	return W_FALSE;
}
wresult _w_control_is_reparentable(w_control *control) {
	return W_FALSE;
}
wresult _w_control_is_visible(w_control *control) {
	return IsWindowVisible(_W_WIDGET(control)->handle);
}
wresult _w_control_kill_timer(w_control *control, wushort id) {
	KillTimer(_W_WIDGET(control)->handle, id + 0x200);
	return W_TRUE;
}
wresult _w_control_move_above(w_control *control, w_control *_control) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	HWND topHandle = priv->handle_top(control), hwndAbove = HWND_TOP;
	if (_control != 0) {
		if (!w_widget_is_ok(W_WIDGET(_control)))
			return W_ERROR_INVALID_ARGUMENT;
		if (_W_CONTROL(control)->parent != _W_CONTROL(_control)->parent)
			return W_FALSE;
		_w_control_priv *_priv = _W_CONTROL_GET_PRIV(_control);
		HWND hwnd = _priv->handle_top(_control);
		if (hwnd == 0 || hwnd == topHandle)
			return W_FALSE;
		hwndAbove = GetWindow(hwnd, GW_HWNDPREV);
		/*
		 * Bug in Windows.  For some reason, when GetWindow ()
		 * with GW_HWNDPREV is used to query the previous window
		 * in the z-order with the first child, Windows returns
		 * the first child instead of NULL.  The fix is to detect
		 * this case and move the control to the top.
		 */
		if (hwndAbove == 0 || hwndAbove == hwnd) {
			hwndAbove = HWND_TOP;
		}
	}
	int flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
	SetWindowPos(topHandle, hwndAbove, 0, 0, 0, 0, flags);
	return W_TRUE;
}
wresult _w_control_move_below(w_control *control, w_control *_control) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	HWND topHandle = priv->handle_top(control), hwndAbove = HWND_BOTTOM;
	w_composite *parent = _W_CONTROL(control)->parent;
	if (_control != 0) {
		if (!w_widget_is_ok(W_WIDGET(_control)))
			return W_ERROR_INVALID_ARGUMENT;
		if (parent != _W_CONTROL(_control)->parent)
			return W_FALSE;
		_w_control_priv *_priv = _W_CONTROL_GET_PRIV(_control);
		hwndAbove = _priv->handle_top(_control);
	} else {
		/*
		 * Bug in Windows.  When SetWindowPos() is called
		 * with HWND_BOTTOM on a dialog shell, the dialog
		 * and the parent are moved to the bottom of the
		 * desktop stack.  The fix is to move the dialog
		 * to the bottom of the dialog window stack by
		 * moving behind the first dialog child.
		 */
		w_shell *shell;
		w_control_get_shell(control, &shell);
		if (control == W_CONTROL(shell) && parent != 0) {
			/*
			 * Bug in Windows.  For some reason, when GetWindow ()
			 * with GW_HWNDPREV is used to query the previous window
			 * in the z-order with the first child, Windows returns
			 * the first child instead of NULL.  The fix is to detect
			 * this case and do nothing because the control is already
			 * at the bottom.
			 */
			HWND hwndParent = _W_WIDGET(parent)->handle, hwnd = hwndParent;
			hwndAbove = GetWindow(hwnd, GW_HWNDPREV);
			while (hwndAbove != 0 && hwndAbove != hwnd) {
				if (GetWindow(hwndAbove, GW_OWNER) == hwndParent)
					break;
				hwndAbove = GetWindow(hwnd = hwndAbove, GW_HWNDPREV);
			}
			if (hwndAbove == hwnd)
				return W_TRUE;
		}
	}
	if (hwndAbove == 0 || hwndAbove == topHandle)
		return W_FALSE;
	int flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
	SetWindowPos(topHandle, hwndAbove, 0, 0, 0, 0, flags);
	return W_TRUE;
}
wresult _w_control_new_layout_data(w_control *control, void **data,
		size_t size) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(control);
	if ((_W_WIDGET(control)->state & STATE_LAYOUT_DATA_LOCALE) == 0) {
		void *layout_data =
				*((void**) &((char*) control)[clazz->object_used_size]);
		if (layout_data != 0) {
			free(layout_data);
		}
	}
	if ((clazz->object_used_size + size) < clazz->object_total_size) {
		_W_WIDGET(control)->state |= STATE_LAYOUT_DATA_LOCALE;
		*data = (void*) &((char*) control)[clazz->object_used_size];
	} else {
		_W_WIDGET(control)->state &= ~STATE_LAYOUT_DATA_LOCALE;
		*data = malloc(size);
		if (*data == 0)
			return W_ERROR_NO_MEMORY;
		*((void**) &((char*) control)[clazz->object_used_size]) = *data;
	}
	return W_TRUE;
}
wresult _w_control_pack(w_control *control, int flags) {
	return W_FALSE;
}
wresult _w_control_print(w_control *control, w_graphics *gc) {
	return W_FALSE;
}
wresult _w_control_request_layout(w_control *control) {
	w_shell *shell;
	w_control_get_shell(control, &shell);
	return W_TRUE;
}
wresult _w_control_redraw(w_control *control, w_rect *rect, int all) {
	RECT *r, _r;
	if (rect != 0) {
		if (rect->width <= 0 || rect->height <= 0)
			return W_TRUE;
		r = &_r;
	} else {
		r = 0;
	}
	if (!IsWindowVisible(_W_WIDGET(control)->handle))
		return W_TRUE;
	if (r != 0) {
		SetRect(r, rect->x, rect->y, rect->x + rect->width,
				rect->y + rect->height);
	}
	int flags = RDW_ERASE | RDW_FRAME | RDW_INVALIDATE;
	if (all)
		flags |= RDW_ALLCHILDREN;
	RedrawWindow(_W_WIDGET(control)->handle, r, 0, flags);
	return W_TRUE;
}
wresult _w_control_set_background(w_control *control, w_color color) {
	return W_FALSE;
}
wresult _w_control_set_background_0(w_control *control, _w_control_priv *priv) {
	return W_TRUE;
}
wresult _w_control_set_background_image(w_control *control, w_image *image) {
	return W_FALSE;
}
wresult _w_control_set_bounds(w_control *control, w_point *location,
		w_size *size) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	HWND topHandle = priv->handle_top(control);
	int f = SWP_NOZORDER | SWP_DRAWFRAME | SWP_NOACTIVATE;
	int x, y, w, h;
	if (location != 0) {
		x = location->x;
		y = location->y;
	} else {
		x = 0;
		y = 0;
		f |= SWP_NOMOVE;
	}
	if (size != 0) {
		w = size->width;
		h = size->height;
	} else {
		w = 0;
		h = 0;
		f |= SWP_NOSIZE;
	}
	if (SetWindowPos(topHandle, 0, x, y, w, h, f)) {
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _w_control_set_capture(w_control *control, int capture) {
	if (capture) {
		SetCapture(_W_WIDGET(control)->handle);
	} else {
		if (GetCapture() == _W_WIDGET(control)->handle) {
			ReleaseCapture();
		}
	}
	return W_TRUE;
}
wresult _w_control_set_cursor_0(w_control *control, _w_control_priv *priv) {
	LPARAM lParam = MAKELPARAM(HTCLIENT, WM_MOUSEMOVE);
	SendMessageW(_W_WIDGET(control)->handle, WM_SETCURSOR,
			(WPARAM) _W_WIDGET(control)->handle, lParam);
	return W_TRUE;
}
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor) {
	if (cursor != 0 && _W_CURSOR(cursor)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	_W_CONTROL(control)->cursor = cursor;
	HWND hwndCursor = GetCapture();
	if (hwndCursor == 0) {
		POINT pt;
		if (!GetCursorPos(&pt))
			return W_TRUE;
		HWND hwnd = hwndCursor = WindowFromPoint(pt);
		HWND handle = _W_WIDGET(control)->handle;
		while (hwnd != 0 && hwnd != handle) {
			hwnd = GetParent(hwnd);
		}
		if (hwnd == 0)
			return W_TRUE;
	}
	w_control *c = (w_control*) _w_widget_find_control(hwndCursor);
	if (c == 0)
		c = control;
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(c);
	priv->set_cursor_0(c, priv);
	return W_TRUE;
}
wresult _w_control_set_default_font(w_control *control, _w_control_priv *priv) {
	HFONT hFont = win_toolkit->systemFont.handle;
	SendMessageW(_W_WIDGET(control)->handle, WM_SETFONT, (WPARAM) hFont, 0);
	return W_TRUE;
}
wresult _w_control_set_drag_detect(w_control *control, int dragDetect) {
	return W_FALSE;
}
wresult _w_control_set_enabled(w_control *control, int enabled) {
	if (EnableWindow(_W_WIDGET(control)->handle, enabled))
		return W_TRUE;
	else
		return W_FALSE;
}
wresult _w_control_set_focus(w_control *control) {
	if ((_W_WIDGET(control)->style & W_NO_FOCUS) != 0)
		return W_FALSE;
	return _w_control_force_focus(control);
}
wresult _w_control_set_font(w_control *control, w_font *font) {
	if (font == 0 || _W_FONT(font)->handle == 0) {
		return W_ERROR_INVALID_ARGUMENT;
	} else {
		_W_CONTROL(control)->font = font;
		_W_WIDGET(control)->state |= STATE_SET_FONT;
		SendMessageW(_W_WIDGET(control)->handle, WM_SETFONT, (WPARAM)
		_W_FONT(font)->handle, 0);
		return W_TRUE;
	}
}
wresult _w_control_set_foreground(w_control *control, w_color color) {
	return W_FALSE;
}
wresult _w_control_set_menu(w_control *control, w_menu *menu) {
	if (w_widget_is_ok(W_WIDGET(menu))) {
		_W_CONTROL(control)->menu = menu;
		return W_TRUE;
	} else {
		return W_ERROR_INVALID_ARGUMENT;
	}
}
wresult _w_control_set_orientation(w_control *control, int orientation) {
	return W_FALSE;
}
wresult _w_control_set_parent(w_control *control, w_composite *parent) {
	return W_FALSE;
}
wresult _w_control_set_redraw(w_control *control, int redraw) {
	return W_FALSE;
}
wresult _w_control_set_region(w_control *control, w_region *region) {
	return W_FALSE;
}
wresult _w_control_set_tab(w_control *control, int tab) {
	return W_FALSE;
}
wresult _w_control_set_text_direction(w_control *control, int textDirection) {
	return W_FALSE;
}
wresult _w_control_set_timer(w_control *control, wint64 ms, wushort id) {
	SetTimer(_W_WIDGET(control)->handle, id + 0x200, ms, NULL);
	return W_TRUE;
}
wresult _w_control_set_tooltip_text(w_control *control, const char *text,
		int length, int enc) {
	return W_FALSE;
}
wresult _w_control_set_touch_enabled(w_control *control, int enabled) {
	return W_FALSE;
}
wresult _w_control_set_visible(w_control *control, int visible) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	HWND tophandle = priv->handle_top(control);
	int v;
	if (visible)
		v = SW_SHOWDEFAULT;
	else
		v = SW_HIDE;

	ShowWindow(tophandle, v);
	return W_TRUE;
}
wresult _w_control_subclass(w_control *control, _w_control_priv *priv) {
	WNDPROC oldProc = (WNDPROC) GetWindowLongPtrW(_W_WIDGET(control)->handle,
	GWLP_WNDPROC);
	if (priv->def_window_proc == 0) {
		priv->def_window_proc = oldProc;
	}
	WNDPROC newProc = _w_control_window_proc;
	if (oldProc == newProc)
		return W_TRUE;
	SetWindowLongPtrW(_W_WIDGET(control)->handle, GWLP_WNDPROC,
			(LONG_PTR) newProc);
	return W_TRUE;
}
wresult _w_control_to_control(w_control *control, w_point *result,
		w_point *point) {
	return W_FALSE;
}
wresult _w_control_to_display(w_control *control, w_point *result,
		w_point *point) {
	return W_FALSE;
}
wresult _w_control_traverse(w_control *control, int traversal,
		w_event_key *event) {
	return W_FALSE;
}
wresult _w_control_unsubclass(w_control *control, _w_control_priv *priv) {
	WNDPROC newProc = priv->def_window_proc;
	WNDPROC oldProc = _w_control_window_proc;
	if (oldProc == newProc)
		return W_TRUE;
	SetWindowLongPtrW(_W_WIDGET(control)->handle, GWLP_WNDPROC,
			(LONG_PTR) newProc);
	return W_TRUE;
}
wresult _w_control_update(w_control *control) {
	return W_FALSE;
}
DWORD _w_control_widget_extstyle(w_control *control, _w_control_priv *priv) {
	DWORD bits = 0;
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_BORDER) != 0)
		bits |= WS_EX_CLIENTEDGE;
//	if ((style & W_BORDER) != 0) {
//		if ((style & W_FLAT) == 0) bits |= WS_EX_CLIENTEDGE;
//	}
	/*
	 * Feature in Windows NT.  When CreateWindowEx() is called with
	 * WS_EX_LAYOUTRTL or WS_EX_NOINHERITLAYOUT, CreateWindowEx()
	 * fails to create the HWND. The fix is to not use these bits.
	 */
	if (WIN32_VERSION < VERSION(4, 10)) {
		return bits;
	}
	bits |= WS_EX_NOINHERITLAYOUT;
	if ((style & W_RIGHT_TO_LEFT) != 0)
		bits |= WS_EX_LAYOUTRTL;
	if ((style & W_FLIP_TEXT_DIRECTION) != 0)
		bits |= WS_EX_RTLREADING;
	return bits;
}
HWND _w_control_widget_parent(w_control *control, _w_control_priv *priv) {
	return _W_WIDGET(_W_CONTROL(control)->parent)->handle;
}
DWORD _w_control_widget_style(w_control *control, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	/* Force clipping of siblings by setting WS_CLIPSIBLINGS */
	DWORD bits = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
//	if ((style & W_BORDER) != 0) {
//		if ((style & W_FLAT) != 0) bits |= WS_BORDER;
//	}
	if ((style & W_BORDER) != 0)
		bits |= WS_BORDER;
	return bits;

	/*
	 * This code is intentionally commented.  When clipping
	 * of both siblings and children is not enforced, it is
	 * possible for application code to draw outside of the
	 * control.
	 */
//	int bits = WS_CHILD | WS_VISIBLE;
//	if ((style & W_CLIP_SIBLINGS) != 0) bits |= WS_CLIPSIBLINGS;
//	if ((style & W_CLIP_CHILDREN) != 0) bits |= WS_CLIPCHILDREN;
//	return bits;
}
const char* _w_control_window_class(w_control *control, _w_control_priv *priv) {
	return 0;
}
void _w_control_class_init(struct _w_control_class *clazz) {
	_w_widget_class_init(W_WIDGET_CLASS(clazz));
	/*
	 * functions
	 */
	W_WIDGET_CLASS(clazz)->create = _w_control_create;
	W_WIDGET_CLASS(clazz)->post_event = _w_control_post_event;
	W_WIDGET_CLASS(clazz)->dispose = _w_control_dispose;
	clazz->create_dragsource = _w_control_create_dragsource;
	clazz->create_droptarget = _w_control_create_droptarget;
	clazz->drag_detect = _w_control_drag_detect;
	clazz->force_focus = _w_control_force_focus;
	clazz->get_accessible = _w_control_get_accessible;
	clazz->get_background = _w_control_get_background;
	clazz->get_background_image = _w_control_get_background_image;
	clazz->get_border_width = _w_control_get_border_width;
	clazz->get_bounds = _w_control_get_bounds;
	clazz->get_cursor = _w_control_get_cursor;
	clazz->get_drag_detect = _w_control_get_drag_detect;
	clazz->get_enabled = _w_control_get_enabled;
	clazz->get_font = _w_control_get_font;
	clazz->get_foreground = _w_control_get_foreground;
	clazz->get_graphics = _w_control_get_graphics;
	clazz->get_layout_data = _w_control_get_layout_data;
	clazz->get_menu = _w_control_get_menu;
	clazz->get_orientation = _w_control_get_orientation;
	clazz->get_parent = _w_control_get_parent;
	clazz->get_region = _w_control_get_region;
	clazz->get_shell = _w_control_get_shell;
	clazz->get_tab = _w_control_get_tab;
	clazz->get_text_direction = _w_control_get_text_direction;
	clazz->get_tooltip_text = _w_control_get_tooltip_text;
	clazz->get_touch_enabled = _w_control_get_touch_enabled;
	clazz->get_visible = _w_control_get_visible;
	clazz->is_enabled = _w_control_is_enabled;
	clazz->is_focus_control = _w_control_is_focus_control;
	clazz->is_reparentable = _w_control_is_reparentable;
	clazz->is_visible = _w_control_is_visible;
	clazz->kill_timer = _w_control_kill_timer;
	clazz->move_above = _w_control_move_above;
	clazz->move_below = _w_control_move_below;
	clazz->new_layout_data = _w_control_new_layout_data;
	clazz->pack = _w_control_pack;
	clazz->print = _w_control_print;
	clazz->request_layout = _w_control_request_layout;
	clazz->redraw = _w_control_redraw;
	clazz->set_background = _w_control_set_background;
	clazz->set_background_image = _w_control_set_background_image;
	clazz->set_bounds = _w_control_set_bounds;
	clazz->set_capture = _w_control_set_capture;
	clazz->set_cursor = _w_control_set_cursor;
	clazz->set_drag_detect = _w_control_set_drag_detect;
	clazz->set_enabled = _w_control_set_enabled;
	clazz->set_focus = _w_control_set_focus;
	clazz->set_font = _w_control_set_font;
	clazz->set_foreground = _w_control_set_foreground;
	clazz->set_menu = _w_control_set_menu;
	clazz->set_orientation = _w_control_set_orientation;
	clazz->set_parent = _w_control_set_parent;
	clazz->set_redraw = _w_control_set_redraw;
	clazz->set_region = _w_control_set_region;
	clazz->set_tab = _w_control_set_tab;
	clazz->set_text_direction = _w_control_set_text_direction;
	clazz->set_timer = _w_control_set_timer;
	clazz->set_tooltip_text = _w_control_set_tooltip_text;
	clazz->set_touch_enabled = _w_control_set_touch_enabled;
	clazz->set_visible = _w_control_set_visible;
	clazz->to_control = _w_control_to_control;
	clazz->to_display = _w_control_to_display;
	clazz->traverse = _w_control_traverse;
	clazz->update = _w_control_update;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->call_window_proc = _w_control_call_window_proc;
	priv->check_style = _w_control_check_style;
	priv->compute_size = _w_control_compute_size;
	priv->get_client_area = _w_control_get_client_area;
	priv->compute_trim = _w_control_compute_trim;
	priv->handle_top = _w_control_handle;
	priv->handle_border = _w_control_handle;
	priv->create_widget = _w_control_create_widget;
	priv->check_orientation = _w_control_check_orientation;
	priv->create_handle = _w_control_create_handle;
	priv->check_background = _w_control_check_background;
	priv->check_buffered = _w_control_check_buffered;
	priv->check_composited = _w_control_check_composited;
	priv->set_default_font = _w_control_set_default_font;
	priv->check_mirrored = _w_control_check_mirrored;
	priv->check_border = _w_control_check_border;
	priv->check_gesture = _w_control_check_gesture;
	priv->set_background = _w_control_set_background_0;
	priv->widget_parent = _w_control_widget_parent;
	priv->widget_style = _w_control_widget_style;
	priv->widget_extstyle = _w_control_widget_extstyle;
	priv->window_class = _w_control_window_class;
	priv->subclass = _w_control_subclass;
	priv->unsubclass = _w_control_unsubclass;
	priv->set_cursor_0 = _w_control_set_cursor_0;
	priv->find_cursor = _w_control_find_cursor;
	/*
	 * messages
	 */
	dispatch_message *msg = priv->messages;
	for (size_t i = 0; i < _WM_LAST; i++) {
		priv->messages[i] = _CONTROL_WM_NULL;
	}
	msg[_WM_DESTROY] = _CONTROL_WM_DESTROY;
	msg[_WM_SIZE] = _CONTROL_WM_SIZE;
	msg[_WM_TIMER] = _CONTROL_WM_TIMER;
	msg[_WM_CREATE] = _CONTROL_WM_CREATE;
	msg[_WM_CTLCOLOR] = _CONTROL_WM_CTLCOLOR;
	msg[_WM_CAPTURECHANGED] = _WIDGET_WM_CAPTURECHANGED;
	msg[_WM_CHANGEUISTATE] = _CONTROL_WM_CHANGEUISTATE;
	msg[_WM_CHAR] = _WIDGET_WM_CHAR;
	msg[_WM_COMMAND] = _CONTROL_WM_COMMAND;
	msg[_WM_CONTEXTMENU] = _WIDGET_WM_CONTEXTMENU;
	msg[_WM_DRAWITEM] = _CONTROL_WM_DRAWITEM;
	msg[_WM_ERASEBKGND] = _CONTROL_WM_ERASEBKGND;
	msg[_WM_GETOBJECT] = _CONTROL_WM_GETOBJECT;
	msg[_WM_HELP] = _CONTROL_WM_HELP;
	msg[_WM_HSCROLL] = _CONTROL_WM_HSCROLL;
	msg[_WM_IME_CHAR] = _WIDGET_WM_IME_CHAR;
	msg[_WM_INPUTLANGCHANGE] = _CONTROL_WM_INPUTLANGCHANGE;
	msg[_WM_KEYDOWN] = _WIDGET_WM_KEYDOWN;
	msg[_WM_KEYUP] = _WIDGET_WM_KEYUP;
	msg[_WM_KILLFOCUS] = _WIDGET_WM_KILLFOCUS;
	msg[_WM_MEASUREITEM] = _CONTROL_WM_MEASUREITEM;
	msg[_WM_MENUCHAR] = _CONTROL_WM_MENUCHAR;
	msg[_WM_MENUSELECT] = _CONTROL_WM_MENUSELECT;
	msg[_WM_MOVE] = _CONTROL_WM_MOVE;
	msg[_WM_NCHITTEST] = _CONTROL_WM_NCHITTEST;
	msg[_WM_NOTIFY] = _CONTROL_WM_NOTIFY;
	msg[_WM_PAINT] = _CONTROL_WM_PAINT;
	msg[_WM_SETCURSOR] = _CONTROL_WM_SETCURSOR;
	msg[_WM_SETFOCUS] = _WIDGET_WM_SETFOCUS;
	msg[_WM_SYSCHAR] = _WIDGET_WM_SYSCHAR;
	msg[_WM_SYSCOMMAND] = _CONTROL_WM_SYSCOMMAND;
	msg[_WM_SYSKEYDOWN] = _WIDGET_WM_SYSKEYDOWN;
	msg[_WM_SYSKEYUP] = _WIDGET_WM_SYSKEYUP;
	msg[_WM_TABLET_FLICK] = _CONTROL_WM_TABLET_FLICK;
	msg[_WM_TOUCH] = _CONTROL_WM_TOUCH;
	msg[_WM_VSCROLL] = _CONTROL_WM_VSCROLL;
	msg[_WM_WINDOWPOSCHANGED] = _CONTROL_WM_WINDOWPOSCHANGED;
	msg[_WM_WINDOWPOSCHANGING] = _CONTROL_WM_WINDOWPOSCHANGING;
	msg[_WM_CTLCOLORCHILD] = _CONTROL_WM_CTLCOLORCHILD;
	msg[_WM_GETDLGCODE] = _CONTROL_WM_GETDLGCODE;

	//mouse message
	msg[_WM_LBUTTONDBLCLK] = _WIDGET_WM_LBUTTONDBLCLK;
	msg[_WM_LBUTTONDOWN] = _WIDGET_WM_LBUTTONDOWN;
	msg[_WM_LBUTTONUP] = _WIDGET_WM_LBUTTONUP;
	msg[_WM_MBUTTONDBLCLK] = _WIDGET_WM_MBUTTONDBLCLK;
	msg[_WM_MBUTTONDOWN] = _WIDGET_WM_MBUTTONDOWN;
	msg[_WM_MBUTTONUP] = _WIDGET_WM_MBUTTONUP;
	msg[_WM_MOUSEHOVER] = _WIDGET_WM_MOUSEHOVER;
	msg[_WM_MOUSELEAVE] = _WIDGET_WM_MOUSELEAVE;
	msg[_WM_MOUSEMOVE] = _WIDGET_WM_MOUSEMOVE;
	msg[_WM_MOUSEWHEEL] = _WIDGET_WM_MOUSEWHEEL;
	msg[_WM_RBUTTONDBLCLK] = _WIDGET_WM_RBUTTONDBLCLK;
	msg[_WM_RBUTTONDOWN] = _WIDGET_WM_RBUTTONDOWN;
	msg[_WM_RBUTTONUP] = _WIDGET_WM_RBUTTONUP;
	msg[_WM_XBUTTONDBLCLK] = _WIDGET_WM_XBUTTONDBLCLK;
	msg[_WM_XBUTTONDOWN] = _WIDGET_WM_XBUTTONDOWN;
	msg[_WM_XBUTTONUP] = _WIDGET_WM_XBUTTONUP;
	msg[_WM_MENUCOMMAND] = _MENU_WM_MENUCOMMAND;
	msg[_WM_INITMENUPOPUP] = _MENU_WM_INITMENUPOPUP;
	msg[_WM_UNINITMENUPOPUP] = _MENU_WM_UNINITMENUPOPUP;
}
