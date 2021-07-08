/*
 * Name:        control1.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "control.h"
#include "toolkit.h"
/*
 * messages
 */
wresult _w_control_post_event_platform(w_widget *widget, _w_event_platform *ee,
		_w_control_priv *priv) {
	wresult ret = W_FALSE;
	wuchar msgid;
	if (ee->msg
			< (sizeof(win_toolkit->wm_msg) / sizeof(win_toolkit->wm_msg[0]))) {
		msgid = win_toolkit->wm_msg[ee->msg];
		if (msgid != 0) {
			ret = priv->messages[msgid](widget, ee, priv);
		}
	}
	if (ret == W_FALSE) {
		_W_WIDGET_PRIV(priv)->call_window_proc(widget, ee, priv);
	}
	return ret;
}
wresult _w_control_post_event(w_widget *widget, w_event *e,int flags) {
	if (widget->post_event != 0) {
		widget->post_event(widget, e);
	}
	switch (e->type) {
	case W_EVENT_PLATFORM:
		return _w_control_post_event_platform(widget, _W_EVENT_PLATFORM(e),
				_W_CONTROL_GET_PRIV(widget));
		break;
	case W_EVENT_COMPUTE_SIZE: {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(widget);
		return priv->compute_size(widget, (w_event_compute_size*) e, priv);
	}
		break;
	case W_EVENT_CLIENT_AREA: {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(widget);
		return priv->get_client_area(widget, (w_event_client_area*) e, priv);
	}
		break;
	case W_EVENT_COMPUTE_TRIM: {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(widget);
		return priv->compute_trim(widget, (w_event_compute_trim*) e, priv);
	}
		break;
	case W_EVENT_LAYOUTDETECT: {
		w_layout *layout;
		w_composite_get_layout(W_COMPOSITE(widget), &layout);
		w_layout_do_layout(layout, W_COMPOSITE(widget), W_TRUE);
		return W_TRUE;
	}
		break;
	}
	return W_FALSE;
}
LRESULT CALLBACK _w_control_window_proc(HWND hWnd, UINT message, WPARAM wParam,
		LPARAM lParam) {
	w_widget *widget = (w_widget*) GetWindowLongPtrW(hWnd,
	GWLP_USERDATA);
	_w_event_platform e;
	if (widget != 0) {
		const int wm_msg_size = sizeof(win_toolkit->wm_msg)
				/ sizeof(win_toolkit->wm_msg[0]);
		if (message < wm_msg_size) {
			e.msg_id = win_toolkit->wm_msg[message];
		} else {
			e.msg_id = 0;
		}
		e.event.type = W_EVENT_PLATFORM;
		e.event.widget = widget;
		e.event.platform_event = 0;
		e.event.data = 0;
		e.hwnd = hWnd;
		e.msg = message;
		e.wparam = wParam;
		e.lparam = lParam;
		e.result = 0;
		_w_widget_send_event(widget, (w_event*) &e, W_EVENT_SEND);
		return e.result;
	} else {
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
}
wresult _CONTROL_WM_NULL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_DESTROY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_CREATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_CTLCOLOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_CHANGEUISTATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_COMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (e->lparam != 0) {
		w_widget *control = _w_widget_find_control((HWND) e->lparam);
		if (control != 0) {
			_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(control);
			return cpriv->messages[_WM_COMMANDCHILD](control, e, cpriv);
		}
	} else {
		return _MENU_WM_COMMAND(widget, e, priv);
	}
	return W_FALSE;
}
wresult _CONTROL_WM_DRAWITEM(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	DRAWITEMSTRUCT *st = (DRAWITEMSTRUCT*) e->lparam;
	if (st->CtlType == ODT_MENU) {
		return _MENU_WM_DRAWITEM(widget, e, priv);
	} else {
		w_widget *control = _w_widget_find_control(st->hwndItem);
		if (control != 0) {
			_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(control);
			return cpriv->messages[_WM_DRAWCHILD](control, e, cpriv);
		}
	}
	return W_FALSE;
}
wresult _CONTROL_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_GETOBJECT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_GETDLGCODE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_HELP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_HSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_widget *control = 0;
	if (e->lparam != 0)
		control = _w_widget_find_control((HWND) e->lparam);
	if (control != 0) {
		_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(control);
		return cpriv->messages[_WM_SCROLLCHILD](control, e, cpriv);
	} else
		return W_FALSE;
}
wresult _CONTROL_WM_INPUTLANGCHANGE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_MEASUREITEM(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	MEASUREITEMSTRUCT *st = (MEASUREITEMSTRUCT*) e->lparam;
	if (st->CtlType == ODT_MENU) {
		return _MENU_WM_MEASUREITEM(widget, e, priv);
	} else {
		HWND hwnd = GetDlgItem(_W_WIDGET(widget)->handle, st->CtlID);
		w_widget *control = _w_widget_find_control(hwnd);
		if (control != 0) {
			_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(control);
			return cpriv->messages[_WM_MEASURECHILD](control, e, cpriv);
		}
	}
	return W_FALSE;
}
wresult _CONTROL_WM_MOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_NCHITTEST(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_NOTIFY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMHDR *nmhdr = (NMHDR*) e->lparam;
	w_widget *control = _w_widget_find_control(nmhdr->hwndFrom);
	if (control != 0) {
		_w_control_priv *r = _W_CONTROL_GET_PRIV(control);
		return r->messages[_WM_NOTIFYCHILD](control, e, r);
	} else
		return W_FALSE;
}
wresult _CONTROL_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((_W_WIDGET(widget)->state & STATE_DISPOSE_SENT) != 0) {
		e->result = 0;
		return W_TRUE;
	}
	return _WIDGET_WM_PAINT(widget, e, priv);
}
wresult _CONTROL_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int hitTest = LOWORD(e->lparam);
	if (hitTest == HTCLIENT) {
		w_control *control = (w_control*) _w_widget_find_control(
				(HWND) e->wparam);
		if (control == 0)
			return W_FALSE;
		_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(control);
		w_cursor *cursor = cpriv->find_cursor(control, cpriv);
		if (cursor != 0) {
			SetCursor((HICON) _W_CURSOR(cursor)->handle);
			e->result = 1;
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _CONTROL_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_RESIZE_OCCURRED;
	//if (_W_WIDGET(widget)->state & STATE_RESIZE_DEFERRED) {
	w_event event;
	memset(&event, 0, sizeof(event));
	event.platform_event = (w_event_platform*) e;
	event.type = W_EVENT_RESIZE;
	event.widget = widget;
	_w_widget_send_event(widget, &event, W_EVENT_SEND);
	//}
	return W_FALSE;
}
wresult _CONTROL_WM_SYSCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_TABLET_FLICK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_TOUCH(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_TIMER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (e->wparam >= 0x200) {
		w_event_time event;
		memset(&event, 0, sizeof(event));
		event.event.platform_event = (w_event_platform*) e;
		event.event.type = W_EVENT_TIMER;
		event.event.widget = widget;
		event.id = e->wparam - 0x200;
		_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	}
	return W_FALSE;
}
wresult _CONTROL_WM_VSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_widget *control = 0;
	if (e->lparam != 0)
		control = _w_widget_find_control((HWND) e->lparam);
	if (control != 0) {
		_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(control);
		return cpriv->messages[_WM_SCROLLCHILD](control, e, cpriv);
	} else
		return W_FALSE;
}
wresult _CONTROL_WM_WINDOWPOSCHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	win_toolkit->resizeCount++;
	wresult result = priv->widget.call_window_proc(widget, e, priv);
	--win_toolkit->resizeCount;
	return result;
}
wresult _CONTROL_WM_WINDOWPOSCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Bug in Windows.  When WM_SETREDRAW is used to turn off drawing
	 * for a control and the control is moved or resized, Windows does
	 * not redraw the area where the control once was in the parent.
	 * The fix is to detect this case and redraw the area.
	 */
	if (!_w_control_get_drawing(W_CONTROL(widget))) {
		w_shell *shell;
		w_widget_get_shell(widget, &shell);
		if (W_WIDGET(shell) != widget) {
			WINDOWPOS *lpwp = (WINDOWPOS*) e->lparam;
			if ((lpwp->flags & SWP_NOMOVE) == 0
					|| (lpwp->flags & SWP_NOSIZE) == 0) {
				HWND topHandle = priv->handle_top(W_CONTROL(widget));
				RECT rect;
				GetWindowRect(topHandle, &rect);
				int width = rect.right - rect.left;
				int height = rect.bottom - rect.top;
				if (width != 0 && height != 0) {
					w_widget *parent = _W_WIDGET(widget)->parent;
					HWND hwndParent =
							parent == 0 ? 0 : _W_WIDGET(parent)->handle;
					MapWindowPoints(0, hwndParent, (POINT*) &rect, 2);
					HRGN rgn1 = CreateRectRgn(rect.left, rect.top, rect.right,
							rect.bottom);
					HRGN rgn2 = CreateRectRgn(lpwp->x, lpwp->y,
							lpwp->x + lpwp->cx, lpwp->y + lpwp->cy);
					CombineRgn(rgn1, rgn1, rgn2, RGN_DIFF);
					int flags = RDW_ERASE | RDW_FRAME | RDW_INVALIDATE
							| RDW_ALLCHILDREN;
					RedrawWindow(hwndParent, NULL, rgn1, flags);
					DeleteObject(rgn1);
					DeleteObject(rgn2);
				}
			}
		}
	}
	return W_FALSE;
}
wresult _CONTROL_WM_CTLCOLORCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CONTROL_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
