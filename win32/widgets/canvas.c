/*
 * Name:        canvas.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "canvas.h"
#include "toolkit.h"
const char* _w_canvas_window_class(w_control *control, _w_control_priv *priv) {
	if (win_toolkit->useOwnDC)
		return WindowOwnDCClass;
	return _w_scrollable_window_class(control, priv);
}
/*
 * messages
 */
wresult _CANVAS_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CANVAS_WM_IME_COMPOSITION(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CANVAS_WM_IME_COMPOSITION_START(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CANVAS_WM_IME_ENDCOMPOSITION(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CANVAS_WM_INPUTLANGCHANGE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CANVAS_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CANVAS_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CANVAS_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return _COMPOSITE_WM_LBUTTONDOWN(widget, e, priv);
}
wresult _CANVAS_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CANVAS_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_SIZE(widget, e, priv);
	return result;
}
wresult _CANVAS_WM_WINDOWPOSCHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _CANVAS_WM_WINDOWPOSCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
void _w_canvas_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_canvas_class *clazz) {
	if (classId == _W_CLASS_CANVAS) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_canvas_priv;
	}
	_w_composite_class_init(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_CANVAS;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_canvas_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_canvas);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_canvas);
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_CANVAS) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->window_class = _w_canvas_window_class;
		/*
		 * messages
		 */
		dispatch_message *msg = priv->messages;
		msg[_WM_CHAR] = _CANVAS_WM_CHAR;
		msg[_WM_IME_COMPOSITION] = _CANVAS_WM_IME_COMPOSITION;
		msg[_WM_IME_COMPOSITION_START] = _CANVAS_WM_IME_COMPOSITION_START;
		msg[_WM_IME_ENDCOMPOSITION] = _CANVAS_WM_IME_ENDCOMPOSITION;
		msg[_WM_INPUTLANGCHANGE] = _CANVAS_WM_INPUTLANGCHANGE;
		msg[_WM_KEYDOWN] = _CANVAS_WM_KEYDOWN;
		msg[_WM_KILLFOCUS] = _CANVAS_WM_KILLFOCUS;
		msg[_WM_LBUTTONDOWN] = _CANVAS_WM_LBUTTONDOWN;
		msg[_WM_SETFOCUS] = _CANVAS_WM_SETFOCUS;
		msg[_WM_SIZE] = _CANVAS_WM_SIZE;
		msg[_WM_WINDOWPOSCHANGED] = _CANVAS_WM_WINDOWPOSCHANGED;
		msg[_WM_WINDOWPOSCHANGING] = _CANVAS_WM_WINDOWPOSCHANGING;
	}
}
