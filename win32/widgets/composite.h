/*
 * Name:        composite.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_WIDGETS_COMPOSITE_H_
#define WIN32_WIDGETS_COMPOSITE_H_
#include "scrollable.h"
/*
 * states
 */
#define STATE_FOREIGN_HANDLE (1 << (STATE_SCROLLABLE_END + 0))
#define STATE_COMPOSITE_END (STATE_SCROLLABLE_END + 1)
/*
 * composite
 */
typedef struct _w_composite {
	_w_scrollable scrollable;
	w_layout *layout;
	int children_count;
} _w_composite;
#define _W_COMPOSITE(x) ((_w_composite*)x)
/*
 * private
 */
typedef struct _w_composite_priv _w_composite_priv;
struct _w_composite_priv {
	_w_scrollable_priv scrollable;
};
#define _W_COMPOSITE_PRIV(x) ((_w_composite_priv*)x)
#define _W_COMPOSITE_GET_PRIV(x) ((_w_composite_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
wresult _w_composite_create_handle(w_control *control, _w_control_priv *priv);
wresult _w_composite_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv);
void _w_composite_minimum_size(w_composite *composite, w_size *result,
		int wHint, int hHint, wresult changed);
DWORD _w_composite_widget_style(w_control *control, _w_control_priv *priv);
void _w_composite_class_init(struct _w_composite_class *clazz);
/*
 * messages
 */
wresult _COMPOSITE_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_GETDLGCODE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_GETFONT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_NCHITTEST(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_PARENTNOTIFY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_PRINTCLIENT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_SETFONT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_SYSCOLORCHANGE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_SYSCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_UPDATEUISTATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_NCPAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _COMPOSITE_WM_NOTIFY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
#endif /* WIN32_WIDGETS_COMPOSITE_H_ */
