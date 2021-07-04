/*
 * Name:        scrollable.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "scrollable.h"
#include "toolkit.h"
/*
 * scrollbar
 */
HWND _w_scrollbar_hwnd_scrollbar(w_widget *parent) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(parent);
	return _W_SCROLLABLE_PRIV(priv)->handle_scrolled(W_CONTROL(parent));
}
wresult _w_scrollbar_copy(w_widgetdata *from, w_widgetdata *to) {
	W_WIDGETDATA(to)->clazz = W_WIDGETDATA(from)->clazz;
	_W_SCROLLBAR(to)->parent = _W_SCROLLBAR(from)->parent;
	_W_SCROLLBAR(to)->type = _W_SCROLLBAR(from)->type;
	return W_TRUE;
}
wresult _w_scrollbar_get_parent(w_scrollbar *scrollbar, w_scrollable **parent) {
	*parent = (w_scrollable*) _W_SCROLLBAR(scrollbar)->parent;
	return W_FALSE;
}
wresult _w_scrollbar_get_enabled(w_scrollbar *scrollbar) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	int state_mask = (type == SB_HORZ) ?
	STATE_HSCROLLBAR_DISABLED :
											STATE_VSCROLLBAR_DISABLED;
	return (_W_WIDGET(parent)->state & state_mask) == 0;
}
wresult _w_scrollbar_get_increment(w_scrollbar *scrollbar) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	return (type == SB_HORZ) ?
	_W_SCROLLABLE(parent)->hincrement :
								_W_SCROLLABLE(parent)->vincrement;
}
wresult _w_scrollbar_get_maximum(w_scrollbar *scrollbar) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_RANGE;
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	GetScrollInfo(hwnd, type, &info);
	return info.nMax;
}
wresult _w_scrollbar_get_minimum(w_scrollbar *scrollbar) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_RANGE;
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	GetScrollInfo(hwnd, type, &info);
	return info.nMin;
}
wresult _w_scrollbar_get_page_increment(w_scrollbar *scrollbar) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	return (type == SB_HORZ) ?
	_W_SCROLLABLE(parent)->hpageIncrement :
								_W_SCROLLABLE(parent)->vpageIncrement;
}
wresult _w_scrollbar_get_selection(w_scrollbar *scrollbar) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_POS;
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	GetScrollInfo(hwnd, type, &info);
	return info.nPos;
}
wresult _w_scrollbar_get_size(w_scrollbar *scrollbar, w_size *size) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(parent);
	//priv->force_resize(W_CONTROL(parent),priv);
	RECT rect;
	HWND hwnd = _W_SCROLLABLE_PRIV(priv)->handle_scrolled(W_CONTROL(parent));
	GetClientRect(hwnd, &rect);
	if (type == SB_HORZ) {
		size->width = rect.right - rect.left;
		size->height = GetSystemMetrics(SM_CYHSCROLL);
	} else {
		size->width = GetSystemMetrics(SM_CXVSCROLL);
		size->height = rect.bottom - rect.top;
	}
	return W_TRUE;
}
wresult _w_scrollbar_get_thumb(w_scrollbar *scrollbar) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_PAGE;
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	GetScrollInfo(hwnd, type, &info);
	if (info.nPage != 0)
		--info.nPage;
	return info.nPage;
}
wresult _w_scrollbar_get_thumb_bounds(w_scrollbar *scrollbar, w_rect *rect) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(parent);
	//priv->force_resize(W_CONTROL(parent),priv);
	HWND handle = _W_WIDGET(parent)->handle;
	SCROLLBARINFO info;
	info.cbSize = sizeof(info);
	int x, y, width, height;
	if (type == SB_HORZ) {
		GetScrollBarInfo(handle, OBJID_HSCROLL, &info);
		x = info.rcScrollBar.left + info.xyThumbTop;
		y = info.rcScrollBar.top;
		width = info.xyThumbBottom - info.xyThumbTop;
		height = info.rcScrollBar.bottom - info.rcScrollBar.top;
	} else {
		GetScrollBarInfo(handle, OBJID_VSCROLL, &info);
		x = info.rcScrollBar.left;
		y = info.rcScrollBar.top + info.xyThumbTop;
		width = info.rcScrollBar.right - info.rcScrollBar.left;
		height = info.xyThumbBottom - info.xyThumbTop;
	}
	RECT r;
	r.left = x;
	r.top = y;
	r.right = x + width;
	r.bottom = y + height;
	MapWindowPoints(0, handle, (POINT*) &r, 2);
	rect->x = r.left;
	rect->y = r.top;
	rect->width = r.right - r.left;
	rect->height = r.bottom - r.top;
	return W_TRUE;
}
wresult _w_scrollbar_get_thumb_track_bounds(w_scrollbar *scrollbar,
		w_rect *rect) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	HWND handle = _W_WIDGET(parent)->handle;
	SCROLLBARINFO info;
	info.cbSize = sizeof(info);
	int x = 0, y = 0, width, height;
	if (type == SB_HORZ) {
		GetScrollBarInfo(handle, OBJID_HSCROLL, &info);
		int size = GetSystemMetrics(SM_CYHSCROLL);
		y = info.rcScrollBar.top;
		width = info.rcScrollBar.right - info.rcScrollBar.left;
		height = size;
		if (width <= 2 * size) {
			x = info.rcScrollBar.left + width / 2;
			width = 0;
		} else {
			x = info.rcScrollBar.left + size;
			width -= 2 * size;
		}
	} else {
		GetScrollBarInfo(handle, OBJID_VSCROLL, &info);
		int size = GetSystemMetrics(SM_CYVSCROLL);
		x = info.rcScrollBar.left;
		width = size;
		height = info.rcScrollBar.bottom - info.rcScrollBar.top;
		if (height <= 2 * size) {
			y = info.rcScrollBar.top + height / 2;
			height = 0;
		} else {
			y = info.rcScrollBar.top + size;
			height -= 2 * size;
		}
	}
	RECT r;
	r.left = x;
	r.top = y;
	r.right = x + width;
	r.bottom = y + height;
	MapWindowPoints(0, handle, (POINT*) &r, 2);
	rect->x = r.left;
	rect->y = r.top;
	rect->width = r.right - r.left;
	rect->height = r.bottom - r.top;
	return W_TRUE;
}
wresult _w_scrollbar_get_values(w_scrollbar *scrollbar,
		w_scrollbar_value *values) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	if (type == SB_HORZ) {
		values->increment = _W_SCROLLABLE(parent)->hincrement;
		values->pageIncrement = _W_SCROLLABLE(parent)->hpageIncrement;
	} else {
		values->increment = _W_SCROLLABLE(parent)->vincrement;
		values->pageIncrement = _W_SCROLLABLE(parent)->vpageIncrement;
	}
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	GetScrollInfo(hwnd, type, &info);
	values->selection = info.nPos;
	values->minimum = info.nMin;
	values->maximum = info.nMax;
	if (info.nPage != 0)
		--info.nPage;
	values->thumb = info.nPage;
	return W_TRUE;
}
wresult _w_scrollbar_get_visible(w_scrollbar *scrollbar) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	if (WIN32_VERSION >= VERSION(4, 10)) {
		SCROLLBARINFO psbi;
		psbi.cbSize = sizeof(SCROLLBARINFO);
		int idObject = type == SB_VERT ? OBJID_VSCROLL : OBJID_HSCROLL;
		HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
		GetScrollBarInfo(hwnd, idObject, &psbi);
		return (psbi.rgstate[0] & STATE_SYSTEM_INVISIBLE) == 0;
	}
	int state_mask = (type == SB_HORZ) ?
	STATE_HSCROLLBAR_HIDDEN :
											STATE_VSCROLLBAR_HIDDEN;
	return (_W_WIDGET(parent)->state & state_mask) == 0;
}
wresult _w_scrollbar_is_enabled(w_scrollbar *scrollbar) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	return _w_scrollbar_get_enabled(scrollbar)
			&& w_control_is_enabled(W_CONTROL(parent));
}
wresult _w_scrollbar_is_visible(w_scrollbar *scrollbar) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	return _w_scrollbar_get_visible(scrollbar)
			&& w_control_is_visible(W_CONTROL(parent));
}
int _SetScrollInfo(w_scrollbar *scrollbar, HWND hwnd, int flags,
		SCROLLINFO *info, int fRedraw) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	/*
	 * Bug in Windows.  For some reason, when SetScrollInfo()
	 * is used with SIF_POS and the scroll bar is hidden,
	 * the opposite scroll bar is incorrectly made visible
	 * so that the next time the parent is resized (or another
	 * scroll bar operation is performed), the opposite scroll
	 * bar draws.  The fix is to hide both scroll bars.
	 */
	int barVisible = FALSE;
	int visible = _w_scrollbar_get_visible(scrollbar);

	/*
	 * This line is intentionally commented.  Currently
	 * always show scrollbar as being enabled and visible.
	 */
	_w_scrollbar bar;
	w_widgetdata_init(W_WIDGETDATA(&bar));
	switch (flags) {
	case SB_HORZ:
		w_scrollable_get_vertical_bar(W_SCROLLABLE(parent), W_SCROLLBAR(&bar));
		break;
	case SB_VERT:
		w_scrollable_get_horizontal_bar(W_SCROLLABLE(parent),
				W_SCROLLBAR(&bar));
		break;
	}
	barVisible = w_widgetdata_is_ok(W_WIDGETDATA(&bar)) > 0
			&& _w_scrollbar_get_visible(W_SCROLLBAR(&bar));
	int type = _W_SCROLLBAR(scrollbar)->type;
	int state_mask_disabled = (type == SB_HORZ) ?
	STATE_HSCROLLBAR_DISABLED :
													STATE_VSCROLLBAR_DISABLED;
	if (!visible || (_W_WIDGET(parent)->state & state_mask_disabled) != 0)
		fRedraw = FALSE;
	int result = SetScrollInfo(hwnd, flags, info, fRedraw);
	/*
	 * Bug in Windows.  For some reason, when the widget
	 * is a standard scroll bar, and SetScrollInfo() is
	 * called with SIF_RANGE or SIF_PAGE, the widget is
	 * incorrectly made visible so that the next time the
	 * parent is resized (or another scroll bar operation
	 * is performed), the scroll bar draws.  The fix is
	 * to hide the scroll bar (again) when already hidden.
	 */
	if (!visible) {
		/*
		 * This line is intentionally commented.  Currently
		 * always show scrollbar as being enabled and visible.
		 */
		ShowScrollBar(hwnd, !barVisible ? SB_BOTH : flags, FALSE);
	}

	/*
	 * Bug in Windows.  When only one scroll bar is visible,
	 * and the thumb changes using SIF_RANGE or SIF_PAGE
	 * from being visible to hidden, the opposite scroll
	 * bar is incorrectly made visible.  The next time the
	 * parent is resized (or another scroll bar operation
	 * is performed), the opposite scroll bar draws.  The
	 * fix is to hide the opposite scroll bar again.
	 *
	 * NOTE: This problem only happens on Vista
	 */
	if (WIN32_VERSION >= VERSION(6, 0)) {
		if (visible && w_widgetdata_is_ok(W_WIDGETDATA(&bar)) > 0
				&& !barVisible) {
			ShowScrollBar(hwnd, flags == SB_HORZ ? SB_VERT : SB_HORZ, FALSE);
		}
	}
	/*
	 * Feature in Windows.  Using SIF_DISABLENOSCROLL,
	 * SetScrollInfo () can change enabled and disabled
	 * state of the scroll bar causing a scroll bar that
	 * was disabled by the application to become enabled.
	 * The fix is to disable the scroll bar (again) when
	 * the application has disabled the scroll bar.
	 */
	if ((_W_WIDGET(parent)->state & state_mask_disabled) != 0) {
		/*
		 * This line is intentionally commented.  Currently
		 * always show scrollbar as being enabled and visible.
		 */
		EnableScrollBar(hwnd, flags, ESB_DISABLE_BOTH);
	}
	return result;
}
wresult _w_scrollbar_set_enabled(w_scrollbar *scrollbar, int enabled) {
	/*
	 * This line is intentionally commented.  Currently
	 * always show scrollbar as being enabled and visible.
	 */
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	int state_mask = (type == SB_HORZ) ?
	STATE_HSCROLLBAR_DISABLED :
											STATE_VSCROLLBAR_DISABLED;
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	int flags = enabled ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH;
	EnableScrollBar(hwnd, type, flags);
	if (enabled) {
		_W_WIDGET(parent)->state &= ~state_mask;
	} else {
		_W_WIDGET(parent)->state |= state_mask;
	}
	return W_TRUE;
}
wresult _w_scrollbar_set_increment(w_scrollbar *scrollbar, int increment) {
	if (increment < 1)
		return W_FALSE;
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	if (type == SB_HORZ) {
		_W_SCROLLABLE(parent)->hincrement = increment;
	} else {
		_W_SCROLLABLE(parent)->vincrement = increment;
	}
	return W_TRUE;
}
wresult _w_scrollbar_set_maximum(w_scrollbar *scrollbar, int maximum) {
	if (maximum < 0)
		return W_FALSE;
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	info.fMask = SIF_RANGE | SIF_DISABLENOSCROLL;
	GetScrollInfo(hwnd, type, &info);
	if (maximum - info.nMin - info.nPage < 1)
		return W_FALSE;
	info.nMax = maximum;
	_SetScrollInfo(scrollbar, hwnd, type, &info, TRUE);
	return W_TRUE;
}
wresult _w_scrollbar_set_minimum(w_scrollbar *scrollbar, int minimum) {
	if (minimum < 0)
		return W_FALSE;
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	info.fMask = SIF_RANGE | SIF_DISABLENOSCROLL;
	GetScrollInfo(hwnd, type, &info);
	if (info.nMax - minimum - info.nPage < 1)
		return W_FALSE;
	info.nMin = minimum;
	_SetScrollInfo(scrollbar, hwnd, type, &info, TRUE);
	return W_TRUE;
}
wresult _w_scrollbar_set_page_increment(w_scrollbar *scrollbar,
		int pageIncrement) {
	if (pageIncrement < 1)
		return W_FALSE;
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	if (type == SB_HORZ) {
		_W_SCROLLABLE(parent)->hpageIncrement = pageIncrement;
	} else {
		_W_SCROLLABLE(parent)->vpageIncrement = pageIncrement;
	}
	return W_TRUE;
}
wresult _w_scrollbar_set_selection(w_scrollbar *scrollbar, int selection) {
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	info.fMask = SIF_POS;
	info.nPos = selection;
	_SetScrollInfo(scrollbar, hwnd, type, &info, TRUE);
	return W_TRUE;
}
wresult _w_scrollbar_set_thumb(w_scrollbar *scrollbar, int thumb) {
	if (thumb < 1)
		return W_FALSE;
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	info.fMask = SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
	GetScrollInfo(hwnd, type, &info);
	info.nPage = thumb;
	if (info.nPage != 0)
		info.nPage++;
	_SetScrollInfo(scrollbar, hwnd, type, &info, TRUE);
	return W_TRUE;
}
wresult _w_scrollbar_set_values(w_scrollbar *scrollbar,
		w_scrollbar_value *values) {
	if (values->minimum < 0)
		return W_FALSE;
	if (values->maximum < 0)
		return W_FALSE;
	if (values->thumb < 1)
		return W_FALSE;
	if (values->increment < 1)
		return W_FALSE;
	if (values->pageIncrement < 1)
		return W_FALSE;
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	if (type == SB_HORZ) {
		_W_SCROLLABLE(parent)->hincrement = values->increment;
		_W_SCROLLABLE(parent)->hpageIncrement = values->pageIncrement;
	} else {
		_W_SCROLLABLE(parent)->vincrement = values->increment;
		_W_SCROLLABLE(parent)->vpageIncrement = values->pageIncrement;
	}
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
	info.nPos = values->selection;
	info.nMin = values->minimum;
	info.nMax = values->maximum;
	info.nPage = values->thumb;
	if (info.nPage != 0)
		info.nPage++;
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	_SetScrollInfo(scrollbar, hwnd, type, &info, TRUE);
	return W_TRUE;
}
wresult _w_scrollbar_set_visible(w_scrollbar *scrollbar, int visible) {
	if (visible == _w_scrollbar_get_visible(scrollbar))
		return W_TRUE;
	w_widget *parent = _W_SCROLLBAR(scrollbar)->parent;
	int type = _W_SCROLLBAR(scrollbar)->type;
	int state_hidden_mask = (type == SB_HORZ) ?
	STATE_HSCROLLBAR_HIDDEN :
												STATE_VSCROLLBAR_HIDDEN;
	/*
	 * Set the state bits before calling ShowScrollBar ()
	 * because hiding and showing the scroll bar can cause
	 * WM_SIZE messages when the client area is resized.
	 * Setting the state before the call means that code
	 * that runs during WM_SIZE that queries the visibility
	 * of the scroll bar will get the correct value.
	 */
	_W_WIDGET(parent)->state =
			visible ?
			_W_WIDGET(parent)->state & ~state_hidden_mask :
						_W_WIDGET(parent)->state | state_hidden_mask;
	HWND hwnd = _w_scrollbar_hwnd_scrollbar(parent);
	/*
	 * Bug in Windows 7. Windows will cause pixel corruption
	 * when there is only one scroll bar visible and it is
	 * hidden.  The fix is to temporarily show the other scroll
	 * bar and hide both.
	 */
	if (!visible) {
		if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
			SCROLLBARINFO psbi;
			psbi.cbSize = sizeof(psbi);
			int idObject = type == SB_HORZ ? OBJID_HSCROLL : OBJID_VSCROLL;
			GetScrollBarInfo(hwnd, idObject, &psbi);
			if ((psbi.rgstate[0] & STATE_SYSTEM_INVISIBLE) != 0) {
				ShowScrollBar(hwnd, type == SB_VERT ? SB_HORZ : SB_VERT, TRUE);
				type = SB_BOTH;
			}
		}
	}
	if (ShowScrollBar(hwnd, type, visible)) {
		/*
		 * Bug in Windows.  For some reason, when the widget
		 * is a standard scroll bar, and SetScrollInfo () is
		 * called with SIF_RANGE or SIF_PAGE while the widget
		 * is not visible, the widget is incorrectly disabled
		 * even though the values for SIF_RANGE and SIF_PAGE,
		 * when set for a visible scroll bar would not disable
		 * the scroll bar.  The fix is to enable the scroll bar
		 * when not disabled by the application and the current
		 * scroll bar ranges would cause the scroll bar to be
		 * enabled had they been set when the scroll bar was
		 * visible.
		 */
		if ((_W_WIDGET(parent)->state & state_hidden_mask) == 0) {
			SCROLLINFO info;
			info.cbSize = sizeof(info);
			info.fMask = SIF_RANGE | SIF_PAGE;
			GetScrollInfo(hwnd, type, &info);
			if (info.nMax - info.nMin - info.nPage >= 0) {
				EnableScrollBar(hwnd, type, ESB_ENABLE_BOTH);
			}
		}
//sendEvent(visible ? SWT.Show : SWT.Hide);
// widget could be disposed at this point
	}
	return W_TRUE;
}
/*
 * scrollable
 */
wresult _w_scrollable_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result = _w_control_create_handle(control, priv);
	if (result > 0) {
		_W_SCROLLABLE(control)->hincrement = 1;
		_W_SCROLLABLE(control)->vincrement = 1;
		_W_SCROLLABLE(control)->hpageIncrement = 10;
		_W_SCROLLABLE(control)->vpageIncrement = 10;
		/*
		 * Do not set the initial values of the maximum
		 * or the thumb.  These values normally default
		 * to 100 and 10 but may have been set already
		 * by the widget that owns the scroll bar.  For
		 * example, a scroll bar that is created for a
		 * list widget, setting these defaults would
		 * override the initial values provided by the
		 * list widget.
		 */
	}
	return result;
}
DWORD _w_scrollable_widget_extstyle(w_control *control, _w_control_priv *priv) {
	return _w_control_widget_extstyle(control, priv);
}
WCHAR* _w_scrollable_window_class(w_control *control,
		_w_control_priv *priv) {
	return WindowClass;
}
DWORD _w_scrollable_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_control_widget_style(control, priv) | WS_TABSTOP;
	if ((_W_WIDGET(control)->style & W_HSCROLL) != 0)
		bits |= WS_HSCROLL;
	if ((_W_WIDGET(control)->style & W_VSCROLL) != 0)
		bits |= WS_VSCROLL;
	return bits;
}
wresult _w_scrollable_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	RECT rect;
	HWND scrolledHandle = _W_SCROLLABLE_PRIV(priv)->handle_scrolled(
			W_CONTROL(widget));
	GetClientRect(scrolledHandle, &rect);
	e->rect->x = rect.left;
	e->rect->y = rect.top;
	e->rect->width = rect.right - rect.left;
	e->rect->height = rect.bottom - rect.top;
	if (scrolledHandle != _W_WIDGET(widget)->handle) {
		GetClientRect(_W_WIDGET(widget)->handle, &rect);
		MapWindowPoints(_W_WIDGET(widget)->handle, scrolledHandle,
				(POINT*) &rect, 2);
		e->rect->x = -rect.left;
		e->rect->y = -rect.top;
	}
	return W_TRUE;
}
wresult _w_scrollable_get_horizontal_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
	W_WIDGETDATA(scrollbar)->clazz = W_WIDGETDATA_CLASS(
			W_SCROLLABLE_GET_CLASS(scrollable)->class_scrollbar);
	_W_SCROLLBAR(scrollbar)->type = SB_HORZ;
	_W_SCROLLBAR(scrollbar)->parent = W_WIDGET(scrollable);
	return W_TRUE;
}
wresult _w_scrollable_get_scrollbars_mode(w_scrollable *scrollable) {
	return W_NONE;
}
wresult _w_scrollable_get_vertical_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
	W_WIDGETDATA(scrollbar)->clazz = W_WIDGETDATA_CLASS(
			W_SCROLLABLE_GET_CLASS(scrollable)->class_scrollbar);
	_W_SCROLLBAR(scrollbar)->type = SB_VERT;
	_W_SCROLLBAR(scrollbar)->parent = W_WIDGET(scrollable);
	return W_TRUE;
}
wresult _w_scrollable_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	RECT rect;
	HWND scrolledHandle = _W_SCROLLABLE_PRIV(priv)->handle_scrolled(
			W_CONTROL(widget));
	SetRect(&rect, e->rect->x, e->rect->y, e->rect->x + e->rect->width,
			e->rect->y + e->rect->height);
	int bits1 = GetWindowLong(scrolledHandle, GWL_STYLE);
	int bits2 = GetWindowLong(scrolledHandle, GWL_EXSTYLE);
	AdjustWindowRectEx(&rect, bits1, FALSE, bits2);
	if (_W_WIDGET(widget)->style & W_HSCROLL)
		rect.bottom += GetSystemMetrics(SM_CYHSCROLL);
	if (_W_WIDGET(widget)->style & W_VSCROLL)
		rect.right += GetSystemMetrics(SM_CXVSCROLL);
	e->result->width = rect.right - rect.left;
	e->result->height = rect.bottom - rect.top;
	e->result->x = rect.left;
	e->result->y = rect.top;
	return W_TRUE;
}
/*
 * messages
 */
wresult _SCROLLABLE_WM_MOUSEWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SCROLLABLE_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return _CONTROL_WM_SIZE(widget, e, priv);
}
wresult _SCROLLABLE_WM_SCROLL(w_widget *widget, _w_event_platform *e,
		wresult update, _w_control_priv *priv) {
	int type = e->msg == WM_HSCROLL ? SB_HORZ : SB_VERT;
	int increment;
	if (update) {
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_TRACKPOS | SIF_POS | SIF_RANGE;
		GetScrollInfo(e->hwnd, type, &info);
		info.fMask = SIF_POS;
		int code = LOWORD(e->wparam);
		switch (code) {
		case SB_ENDSCROLL:
			return W_FALSE;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			/*
			 * Note: On WinCE, the value in SB_THUMBPOSITION is relative to nMin.
			 * Same for SB_THUMBPOSITION 'except' for the very first thumb track
			 * message which has the actual value of nMin. This is a problem when
			 * nMin is not zero.
			 */
			info.nPos = info.nTrackPos;
			break;
		case SB_TOP:
			info.nPos = info.nMin;
			break;
		case SB_BOTTOM:
			info.nPos = info.nMax;
			break;
		case SB_LINEDOWN:
			increment = type == SB_HORZ ?
			_W_SCROLLABLE(widget)->hincrement :
											_W_SCROLLABLE(widget)->vincrement;
			info.nPos += increment;
			break;
		case SB_LINEUP:
			increment = type == SB_HORZ ?
			_W_SCROLLABLE(widget)->hincrement :
											_W_SCROLLABLE(widget)->vincrement;
			info.nPos = WMAX(info.nMin, info.nPos - increment);
			break;
		case SB_PAGEDOWN:
			increment =
					type == SB_HORZ ?
					_W_SCROLLABLE(widget)->hpageIncrement :
										_W_SCROLLABLE(widget)->vpageIncrement;
			info.nPos += increment;
			break;
		case SB_PAGEUP:
			increment =
					type == SB_HORZ ?
					_W_SCROLLABLE(widget)->hpageIncrement :
										_W_SCROLLABLE(widget)->vpageIncrement;
			info.nPos = WMAX(info.nMin, info.nPos - increment);
			break;
		}
		SetScrollInfo(e->hwnd, type, &info, TRUE);
	} else {
		priv->widget.call_window_proc(widget, e, priv);
	}

	/* Do nothing when scrolling is ending */
	int code = LOWORD(e->wparam);
	if (code == SB_ENDSCROLL)
		return W_FALSE;

	/*
	 * Send the event because WM_HSCROLL and
	 * WM_VSCROLL are sent from a modal message
	 * loop in Windows that is active when the
	 * user is scrolling.
	 */
	w_event_scrollbar event;
	w_scrollbar scrollbar;
	memset(&event, 0, sizeof(event));
	if (type == SB_HORZ) {
		event.event.type = W_EVENT_HSCROLL;
		_w_scrollable_get_horizontal_bar(W_SCROLLABLE(widget), &scrollbar);
	} else {
		event.event.type = W_EVENT_VSCROLL;
		_w_scrollable_get_vertical_bar(W_SCROLLABLE(widget), &scrollbar);
	}
	event.event.widget = widget;
	event.scrollbar = &scrollbar;
	switch (code) {
	case SB_THUMBPOSITION:
		event.detail = W_NONE;
		break;
	case SB_THUMBTRACK:
		event.detail = W_DRAG;
		break;
	case SB_TOP:
		event.detail = W_HOME;
		break;
	case SB_BOTTOM:
		event.detail = W_END;
		break;
	case SB_LINEDOWN:
		event.detail = W_ARROW_DOWN;
		break;
	case SB_LINEUP:
		event.detail = W_ARROW_UP;
		break;
	case SB_PAGEDOWN:
		event.detail = W_PAGE_DOWN;
		break;
	case SB_PAGEUP:
		event.detail = W_PAGE_UP;
		break;
	}
	_w_widget_post_event(widget, (w_event*) &event, W_EVENT_SEND);
	return W_TRUE;
}

wresult _SCROLLABLE_WM_VSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int result = _CONTROL_WM_VSCROLL(widget, e, priv);
	if (result)
		return result;
	if ((_W_WIDGET(widget)->style & W_VSCROLL) && e->lparam == 0) {
		return _W_SCROLLABLE_PRIV(priv)->WM_SCROLL(widget, e,
				(_W_WIDGET(widget)->state & STATE_CANVAS) != 0, priv);
	}
	return result;
}
wresult _SCROLLABLE_WM_HSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int result = _CONTROL_WM_HSCROLL(widget, e, priv);
	if (result)
		return result;
	if ((_W_WIDGET(widget)->style & W_HSCROLL) && e->lparam == 0) {
		return _W_SCROLLABLE_PRIV(priv)->WM_SCROLL(widget, e,
				(_W_WIDGET(widget)->state & STATE_CANVAS) != 0, priv);
	}
	return result;
}
void _w_scrollable_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_scrollable_class *clazz) {
	_w_control_class_init(toolkit, classId, W_CONTROL_CLASS(clazz));
	/*
	 * scrollable class
	 */
	clazz->get_horizontal_bar = _w_scrollable_get_horizontal_bar;
	clazz->get_scrollbars_mode = _w_scrollable_get_scrollbars_mode;
	clazz->get_vertical_bar = _w_scrollable_get_vertical_bar;
	/*
	 * scrollbar class
	 */
	struct _w_scrollbar_class *scrollbar = clazz->class_scrollbar;
	if (scrollbar->get_values == 0) {
		scrollbar->widgetdata.toolkit = W_WIDGET_CLASS(clazz)->toolkit;
		scrollbar->widgetdata.is_ok = _w_widgetdata_is_ok;
		scrollbar->widgetdata.close = _w_widgetdata_close;
		scrollbar->widgetdata.copy = _w_scrollbar_copy;
		scrollbar->get_parent = _w_scrollbar_get_parent;
		scrollbar->get_enabled = _w_scrollbar_get_enabled;
		scrollbar->is_enabled = _w_scrollbar_is_enabled;
		scrollbar->set_enabled = _w_scrollbar_set_enabled;
		scrollbar->get_visible = _w_scrollbar_get_visible;
		scrollbar->is_visible = _w_scrollbar_is_visible;
		scrollbar->set_visible = _w_scrollbar_set_visible;
		scrollbar->get_values = _w_scrollbar_get_values;
		scrollbar->get_increment = _w_scrollbar_get_increment;
		scrollbar->get_maximum = _w_scrollbar_get_maximum;
		scrollbar->get_minimum = _w_scrollbar_get_minimum;
		scrollbar->get_page_increment = _w_scrollbar_get_page_increment;
		scrollbar->get_selection = _w_scrollbar_get_selection;
		scrollbar->get_thumb = _w_scrollbar_get_thumb;
		scrollbar->set_values = _w_scrollbar_set_values;
		scrollbar->set_increment = _w_scrollbar_set_increment;
		scrollbar->set_maximum = _w_scrollbar_set_maximum;
		scrollbar->set_minimum = _w_scrollbar_set_minimum;
		scrollbar->set_page_increment = _w_scrollbar_set_page_increment;
		scrollbar->set_selection = _w_scrollbar_set_selection;
		scrollbar->set_thumb = _w_scrollbar_set_thumb;
		scrollbar->get_size = _w_scrollbar_get_size;
		scrollbar->get_thumb_bounds = _w_scrollbar_get_thumb_bounds;
		scrollbar->get_thumb_track_bounds = _w_scrollbar_get_thumb_track_bounds;
	}
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		priv->get_client_area = _w_scrollable_get_client_area;
		priv->compute_trim = _w_scrollable_compute_trim;
		priv->window_class = _w_scrollable_window_class;
		priv->create_handle = _w_scrollable_create_handle;
		_W_SCROLLABLE_PRIV(priv)->handle_scrolled = _w_control_handle;
		_W_SCROLLABLE_PRIV(priv)->WM_SCROLL = _SCROLLABLE_WM_SCROLL;
		/*
		 * messages
		 */
		dispatch_message *msg = priv->messages;
		msg[_WM_HSCROLL] = _SCROLLABLE_WM_HSCROLL;
		msg[_WM_MOUSEWHEEL] = _SCROLLABLE_WM_MOUSEWHEEL;
		msg[_WM_SIZE] = _SCROLLABLE_WM_SIZE;
		msg[_WM_VSCROLL] = _SCROLLABLE_WM_VSCROLL;
	}
}
