/*
 * Name:        scrollable.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_WIDGETS_SCROLLABLE_H_
#define WIN32_WIDGETS_SCROLLABLE_H_
#include "control.h"
/*
 * states
 */
#define STATE_CANVAS (1 << (STATE_CONTROL_END + 0))
#define STATE_SCROLLABLE_END (STATE_CONTROL_END + 1)
/*
 * scrollable
 */
typedef struct _w_scrollable {
	_w_control control;
} _w_scrollable;
#define _W_SCROLLABLE(x) ((_w_scrollable*)x)
/*
 * private
 */
typedef struct _w_scrollable_priv _w_scrollable_priv;
struct _w_scrollable_priv {
	_w_control_priv control;
	HWND (*handle_scrolled)(w_control *control);
};
#define _W_SCROLLABLE_PRIV(x) ((_w_scrollable_priv*)x)
#define _W_SCROLLABLE_GET_PRIV(x) ((_w_scrollable_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
DWORD _w_scrollable_widget_extstyle(w_control *control, _w_control_priv *priv);
const char* _w_scrollable_window_class(w_control *control,
		_w_control_priv *priv);
DWORD _w_scrollable_widget_style(w_control *control, _w_control_priv *priv);
wresult _w_scrollable_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv);
void _w_scrollable_class_init(struct _w_scrollable_class *clazz);
/*
 * messages
 */
wresult _SCROLLABLE_WM_MOUSEWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SCROLLABLE_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SCROLLABLE_WM_VSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SCROLLABLE_WM_HSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
#endif /* WIN32_WIDGETS_SCROLLABLE_H_ */
