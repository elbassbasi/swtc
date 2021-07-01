/*
 * progressbar.c
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */
#include "progressbar.h"
#include "../widgets/toolkit.h"
#if NTDDI_VERSION < 0x06000000
#define PBM_GETSTEP (WM_USER+13)
#define PBM_GETBKCOLOR (WM_USER+14)
#define PBM_GETBARCOLOR (WM_USER+15)
#define PBM_SETSTATE (WM_USER+16)
#define PBM_GETSTATE (WM_USER+17)

#define PBS_SMOOTHREVERSE 0x10

#define PBST_NORMAL 1
#define PBST_ERROR 2
#define PBST_PAUSED 3
#endif /* (_WIN32_WINNT >= 0x0600) */
wuint64 _w_progressbar_check_style(w_widget *widget, wuint64 style) {
	style |= W_NO_FOCUS;
	return _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
}
wresult _w_progressbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int border = W_CONTROL_GET_CLASS(widget)->get_border_width(
			W_CONTROL(widget));
	int width = border * 2, height = border * 2;
	if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
		width += GetSystemMetrics(SM_CXHSCROLL) * 10;
		height += GetSystemMetrics(SM_CYHSCROLL);
	} else {
		width += GetSystemMetrics(SM_CXVSCROLL);
		height += GetSystemMetrics(SM_CYVSCROLL) * 10;
	}
	if (e->wHint != W_DEFAULT)
		width = e->wHint + (border * 2);
	if (e->hHint != W_DEFAULT)
		height = e->hHint + (border * 2);
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
}
wresult _w_progressbar_get_maximum(w_progressbar *progressbar) {
	return (int) SendMessageW(_W_WIDGET(progressbar)->handle, PBM_GETRANGE, 0,
			0);
}
wresult _w_progressbar_get_selection(w_progressbar *progressbar) {
	return (int) SendMessageW(_W_WIDGET(progressbar)->handle, PBM_GETPOS, 0, 0);
}
wresult _w_progressbar_get_state(w_progressbar *progressbar) {
	int state = (int) SendMessageW(_W_WIDGET(progressbar)->handle,
	PBM_GETSTATE, 0, 0);
	switch (state) {
	case PBST_NORMAL:
		return W_NORMAL;
	case PBST_ERROR:
		return W_ERROR;
	case PBST_PAUSED:
		return W_PAUSED;
	}
	return W_NORMAL;
}
wresult _w_progressbar_set_maximum(w_progressbar *progressbar, int value) {
	SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETRANGE32, 0, value);
	return W_TRUE;
}
wresult _w_progressbar_set_selection(w_progressbar *progressbar, int value) {
	SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETPOS, value, 0);
	/*
	 * Bug in Vista.  For some reason, when the progress bar is not in
	 * a normal state, it shows the selection of previous call to
	 * PBM_SETPOS. This is undocumented. The fix is to call PBM_SETPOS
	 * a second time.
	 */
	int state = SendMessageW(_W_WIDGET(progressbar)->handle, PBM_GETSTATE, 0,
			0);
	if (state != PBST_NORMAL) {
		SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETPOS, value, 0);
	}
	return W_TRUE;
}
wresult _w_progressbar_set_state(w_progressbar *progressbar, int state) {
	switch (state) {
	case W_NORMAL:
		SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETSTATE, PBST_NORMAL,
				0);
		break;
	case W_ERROR:
		SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETSTATE, PBST_ERROR,
				0);
		break;
	case W_PAUSED:
		SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETSTATE, PBST_PAUSED,
				0);
		break;
	}
	return W_TRUE;
}
DWORD _w_progressbar_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_control_widget_style(control, priv);
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_SMOOTH) != 0)
		bits |= PBS_SMOOTH;
	if ((style & W_VERTICAL) != 0)
		bits |= PBS_VERTICAL;
	if ((style & W_INDETERMINATE) != 0)
		bits |= PBS_MARQUEE;
	return bits;
}
const char* _w_progressbar_window_class(w_control *control,
		_w_control_priv *priv) {
	return PROGRESS_CLASSA;
}
void _w_progressbar_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_progressbar_class *clazz) {
	if (classId == _W_CLASS_PROGRESSBAR) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_progressbar_priv;
	}
	_w_control_class_init(toolkit, classId, W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_PROGRESSBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_progressbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_progressbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_progressbar);
	/*
	 * public function
	 */
	clazz->get_maximum = _w_progressbar_get_maximum;
	clazz->get_selection = _w_progressbar_get_selection;
	clazz->get_state = _w_progressbar_get_state;
	clazz->set_maximum = _w_progressbar_set_maximum;
	clazz->set_selection = _w_progressbar_set_selection;
	clazz->set_state = _w_progressbar_set_state;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_PROGRESSBAR) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->check_style = _w_progressbar_check_style;
		priv->compute_size = _w_progressbar_compute_size;
		priv->widget_style = _w_progressbar_widget_style;
		priv->window_class = _w_progressbar_window_class;
	}
}
