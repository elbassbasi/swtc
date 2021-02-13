/*
 * Name:        canvas.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_WIDGETS_CANVAS_H_
#define WIN32_WIDGETS_CANVAS_H_
#include "composite.h"
/*
 * canvas
 */
typedef struct _w_canvas {
	_w_composite composite;
} _w_canvas;
#define _W_CANVAS(x) ((_w_canvas*)x)
/*
 * private
 */
typedef struct _w_canvas_priv _w_canvas_priv;
struct _w_canvas_priv {
	_w_composite_priv composite;
};
#define _W_CANVAS_PRIV(x) ((_w_canvas_priv*)x)
#define _W_CANVAS_GET_PRIV(x) ((_w_canvas_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
const char* _w_canvas_window_class(w_control *control, _w_control_priv *priv);
void _w_canvas_class_init(struct _w_canvas_class *clazz);
/*
 * messages
 */
wresult _CANVAS_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_IME_COMPOSITION(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_IME_COMPOSITION_START(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_IME_ENDCOMPOSITION(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_INPUTLANGCHANGE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_WINDOWPOSCHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CANVAS_WM_WINDOWPOSCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
#endif /* WIN32_WIDGETS_CANVAS_H_ */
