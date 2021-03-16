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
wresult _SCROLLABLE_WM_VSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _SCROLLABLE_WM_HSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
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
