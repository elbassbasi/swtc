/*
 * sash.c
 *
 *  Created on: 7 mars 2021
 *      Author: Azeddine
 */
#include "sash.h"
#include "../widgets/toolkit.h"
wuint64 _w_sash_check_style(w_widget *widget, wuint64 style) {
	style = _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
	return style;
}
wuchar draw_band_bits[] = { -86, 0, 85, 0, -86, 0, 85, 0, -86, 0, 85, 0, -86, 0,
		85, 0 };
void _w_sash_draw_band(w_widget *widget, w_point *pt, w_size *size) {
	if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0)
		return;
	w_widget *parent;
	w_widget_get_parent(widget, &parent);
	HWND hwndTrack = _W_WIDGET(parent)->handle;
	HBITMAP stippleBitmap = CreateBitmap(8, 8, 1, 1, draw_band_bits);
	HBRUSH stippleBrush = CreatePatternBrush(stippleBitmap);
	HDC hDC = GetDCEx(hwndTrack, 0, DCX_CACHE);
	HBRUSH oldBrush = (HBRUSH) SelectObject(hDC, stippleBrush);
	PatBlt(hDC, pt->x, pt->y, size->width, size->height, PATINVERT);
	SelectObject(hDC, oldBrush);
	ReleaseDC(hwndTrack, hDC);
	DeleteObject(stippleBrush);
	DeleteObject(stippleBitmap);
}
WCHAR* _w_sash_window_class(w_control *control, _w_control_priv *priv) {
	return WindowClass;
}
/*
 * messages
 */
wresult _SASH_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult ret = _WIDGET_WM_LBUTTONDOWN(widget, e, priv);
	if (ret != W_FALSE)
		return ret;

	/* Compute the banding rectangle */
	HWND hwndTrack = GetParent(_W_WIDGET(widget)->handle);
	POINT pt;
	POINTSTOPOINT(pt, e->lparam);
	RECT rect;
	GetWindowRect(_W_WIDGET(widget)->handle, &rect);
	MapWindowPoints(_W_WIDGET(widget)->handle, 0, &pt, 1);
	_W_SASH(widget)->start.x = pt.x - rect.left;
	_W_SASH(widget)->start.y = pt.y - rect.top;
	MapWindowPoints(0, hwndTrack, (LPPOINT) &rect, 2);
	_W_SASH(widget)->last.x = rect.left;
	_W_SASH(widget)->last.y = rect.top;
	w_size size;
	size.width = rect.right - rect.left;
	size.height = rect.bottom - rect.top;

	/* The event must be sent because doit flag is used */
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.platform_event = (w_event_platform*) e;
	event.event.data = 0;
	event.event.widget = widget;
	event.event.time = 0;
	if ((_W_WIDGET(widget)->style & W_SMOOTH) == 0) {
		event.detail = W_DRAG;
	} else {
		event.detail = 0;
	}
	event.bounds.x = _W_SASH(widget)->last.x;
	event.bounds.y = _W_SASH(widget)->last.y;
	event.bounds.width = size.width;
	event.bounds.height = size.height;
	int doit = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!w_widget_is_ok(W_WIDGET(widget))) {
		e->result = 0;
		return ret;
	}
	/* Draw the banding rectangle */
	if (doit) {
		_W_WIDGET(widget)->state |= STATE_SASH_DRAGGING;
		_W_SASH(widget)->last.x = event.bounds.x;
		_W_SASH(widget)->last.y = event.bounds.y;
		w_shell *shell;
		W_WIDGET_GET_CLASS(widget)->get_shell(widget, &shell);
		_w_shell_bring_totop(shell);
		if (w_widget_is_ok(widget) <= 0) {
			e->result = 0;
			return ret;
		}
		int flags = RDW_UPDATENOW | RDW_ALLCHILDREN;
		RedrawWindow(hwndTrack, 0, 0, flags);
		_w_sash_draw_band(widget, &event.bounds.pt, &size);
		if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0) {
			W_CONTROL_GET_CLASS(widget)->set_bounds(W_CONTROL(widget),
					&event.bounds.pt, &size);
		}
	}
	return ret;
}
wresult _SASH_WM_LBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult ret = _WIDGET_WM_LBUTTONUP(widget, e, priv);
	if (ret)
		return ret;

	/* Compute the banding rectangle */
	if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING))
		return ret;
	_W_WIDGET(widget)->state &= ~STATE_SASH_DRAGGING;
	RECT rect;
	w_size size;
	GetWindowRect(_W_WIDGET(widget)->handle, &rect);
	size.width = rect.right - rect.left;
	size.height = rect.bottom - rect.top;

	/* The event must be sent because doit flag is used */
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.platform_event = (w_event_platform*) e;
	event.event.data = 0;
	event.event.widget = widget;
	event.event.time = 0;
	event.detail = 0;
	event.bounds.x = _W_SASH(widget)->last.x;
	event.bounds.y = _W_SASH(widget)->last.y;
	event.bounds.width = size.width;
	event.bounds.height = size.height;
	int doit = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!w_widget_is_ok(W_WIDGET(widget))) {
		return ret;
	}
	_w_sash_draw_band(widget, &_W_SASH(widget)->last, &size);
	if (doit) {
		if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0) {
			W_CONTROL_GET_CLASS(widget)->set_bounds(W_CONTROL(widget),
					&event.bounds.pt, &size);
			// widget could be disposed at this point
		}
	}
	return ret;
}
wresult _SASH_WM_MOUSEMOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult ret = _WIDGET_WM_MOUSEMOVE(widget, e, priv);
	if (ret)
		return ret;
	if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING)
			|| (e->wparam & MK_LBUTTON) == 0)
		return ret;

	/* Compute the banding rectangle */
	POINT pt;
	POINTSTOPOINT(pt, e->lparam);
	HWND hwndTrack = GetParent(_W_WIDGET(widget)->handle);
	MapWindowPoints(_W_WIDGET(widget)->handle, hwndTrack, &pt, 1);
	RECT rect, clientRect;
	w_size size;
	GetWindowRect(_W_WIDGET(widget)->handle, &rect);
	size.width = rect.right - rect.left;
	size.height = rect.bottom - rect.top;
	GetClientRect(hwndTrack, &clientRect);
	int newX = _W_SASH(widget)->last.x, newY = _W_SASH(widget)->last.y;
	int _max;
	if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
		int clientWidth = clientRect.right - clientRect.left;
		_max = WMAX(0, pt.x - _W_SASH(widget)->start.x);
		newX = WMIN(_max, clientWidth - size.width);
	} else {
		int clientHeight = clientRect.bottom - clientRect.top;
		_max = WMAX(0, pt.y - _W_SASH(widget)->start.y);
		newY = WMIN(_max, clientHeight - size.height);
	}
	if (newX == _W_SASH(widget)->last.x && newY == _W_SASH(widget)->last.y)
		return ret;
	_w_sash_draw_band(widget, & _W_SASH(widget)->last, &size);

	/* The event must be sent because doit flag is used */
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.platform_event = (w_event_platform*) e;
	event.event.data = 0;
	event.event.widget = widget;
	event.event.time = 0;
	if ((_W_WIDGET(widget)->style & W_SMOOTH) == 0) {
		event.detail = W_DRAG;
	} else
		event.detail = 0;
	event.bounds.x = newX;
	event.bounds.y = newY;
	event.bounds.width = size.width;
	event.bounds.height = size.height;
	int doit = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!w_widget_is_ok(W_WIDGET(widget))) {
		e->result = 0;
		return ret;
	}
	if (doit) {
		_W_SASH(widget)->last.x = event.bounds.x;
		_W_SASH(widget)->last.y = event.bounds.y;
	}
	int flags = RDW_UPDATENOW | RDW_ALLCHILDREN;
	RedrawWindow(hwndTrack, 0, 0, flags);
	_w_sash_draw_band(widget, &_W_SASH(widget)->last, &size);
	if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0) {
		W_CONTROL_GET_CLASS(widget)->set_bounds(W_CONTROL(widget),
				&_W_SASH(widget)->last, &size);
		// widget could be disposed at this point
	}
	return ret;
}
wresult _SASH_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult ret = _CONTROL_WM_SETCURSOR(widget, e, priv);
	if (ret)
		return ret;
	int hitTest = (short) LOWORD(e->lparam);
	if (hitTest == HTCLIENT) {
		HCURSOR hCursor = 0;
		if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
			hCursor = LoadCursor(0, IDC_SIZENS);
		} else {
			hCursor = LoadCursor(0, IDC_SIZEWE);
		}
		SetCursor(hCursor);
	}
	e->result = 1;
	return W_TRUE;
}
void _w_sash_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_sash_class *clazz) {
	if (classId == _W_CLASS_SASH) {
		W_WIDGET_CLASS(clazz)->platformPrivate = &win_toolkit->class_sash_priv;
	}
	_w_control_class_init(toolkit, classId, W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SASH;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_sash_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_sash);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_sash);
	/*
	 * priv
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_SASH) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->check_style = _w_sash_check_style;
		priv->window_class = _w_sash_window_class;
		/*
		 * messages
		 */
		dispatch_message *msg = priv->messages;
		msg[_WM_SETCURSOR] = _SASH_WM_SETCURSOR;
		msg[_WM_MOUSEMOVE] = _SASH_WM_MOUSEMOVE;
		msg[_WM_LBUTTONUP] = _SASH_WM_LBUTTONUP;
		msg[_WM_LBUTTONDOWN] = _SASH_WM_LBUTTONDOWN;
	}
}

