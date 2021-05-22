/*
 * Name:        scrollable.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "scrollable.h"
#include "toolkit.h"
DWORD _w_scrollable_widget_extstyle(w_control *control, _w_control_priv *priv) {
	return _w_control_widget_extstyle(control, priv);
}
const char* _w_scrollable_window_class(w_control *control,
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
void _w_scrollable_get_horizontal_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
	_W_WIDGETDATA(scrollbar)->clazz = W_WIDGETDATA_CLASS(
			W_SCROLLABLE_GET_CLASS(scrollable)->class_scrollbar);
	_W_SCROLLBAR(scrollbar)->type = SB_HORZ;
	_W_SCROLLBAR(scrollbar)->parent = scrollable;
}
int _w_scrollable_get_scrollbars_mode(w_scrollable *scrollable) {
	return 0;
}
void _w_scrollable_get_vertical_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
	_W_WIDGETDATA(scrollbar)->clazz = W_WIDGETDATA_CLASS(
			W_SCROLLABLE_GET_CLASS(scrollable)->class_scrollbar);
	_W_SCROLLBAR(scrollbar)->type = SB_VERT;
	_W_SCROLLBAR(scrollbar)->parent = scrollable;
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
	return W_FALSE;
}
int _SCROLLABLE_WM_SCROLL(w_widget *widget, _w_event_platform *e,
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
		priv->widget.window_proc(widget, e, priv);
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
	_w_widget_send_event(widget, (w_event*) &event);
	return W_TRUE;
}

wresult _SCROLLABLE_WM_VSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int result = _CONTROL_WM_VSCROLL(widget, e, priv);
	if (result)
		return result;
	if ((_W_WIDGET(widget)->style & W_VSCROLL) && e->lparam == 0) {
		return _SCROLLABLE_WM_SCROLL(widget, e,
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
		return _SCROLLABLE_WM_SCROLL(widget, e,
				(_W_WIDGET(widget)->state & STATE_CANVAS) != 0, priv);
	}
	return result;
}
void _w_scrollable_class_init(struct _w_scrollable_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->get_client_area = _w_scrollable_get_client_area;
	priv->compute_trim = _w_scrollable_compute_trim;
	priv->window_class = _w_scrollable_window_class;
	_W_SCROLLABLE_PRIV(priv)->handle_scrolled = _w_control_handle;
	/*
	 * messages
	 */
	dispatch_message *msg = priv->messages;
	msg[_WM_HSCROLL] = _SCROLLABLE_WM_HSCROLL;
	msg[_WM_MOUSEWHEEL] = _SCROLLABLE_WM_MOUSEWHEEL;
	msg[_WM_SIZE] = _SCROLLABLE_WM_SIZE;
	msg[_WM_VSCROLL] = _SCROLLABLE_WM_VSCROLL;
}
