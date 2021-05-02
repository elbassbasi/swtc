/*
 * progressbar.c
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */
#include "progressbar.h"
#include "../widgets/toolkit.h"
wresult _w_progressbar_get_maximum(w_progressbar *progressbar) {
	NSProgressIndicator *handle =
			(NSProgressIndicator*) _W_WIDGET(progressbar)->handle;
	NSProgressIndicator_maxValue(handle);
	return W_TRUE;
}
wresult _w_progressbar_get_selection(w_progressbar *progressbar) {
	NSProgressIndicator *handle =
			(NSProgressIndicator*) _W_WIDGET(progressbar)->handle;
	NSProgressIndicator_doubleValue(handle);
	return W_TRUE;
}
wresult _w_progressbar_get_state(w_progressbar *progressbar) {
	return W_NORMAL;
}
wresult _w_progressbar_set_maximum(w_progressbar *progressbar, int value) {
	NSProgressIndicator *handle =
			(NSProgressIndicator*) _W_WIDGET(progressbar)->handle;
	NSProgressIndicator_setMaxValue(handle, value);
	return W_TRUE;
}
wresult _w_progressbar_set_selection(w_progressbar *progressbar, int value) {
	NSProgressIndicator *handle =
			(NSProgressIndicator*) _W_WIDGET(progressbar)->handle;
	NSProgressIndicator_setDoubleValue(handle, value);
	return W_TRUE;
}
wresult _w_progressbar_set_state(w_progressbar *progressbar, int state) {
	return W_FALSE;
}
wresult _w_progressbar_create_handle(w_widget *widget, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	NSProgressIndicator *handle = SWTProgressIndicator_new(widget);
	NSProgressIndicator_setUsesThreadedAnimation(handle, W_FALSE);
	if ((style & W_VERTICAL) != 0) {
		NSView_setBoundsRotation((NSView*) handle, -90);
	}
	NSProgressIndicator_setIndeterminate(handle,
			(style & W_INDETERMINATE) != 0);
	_W_WIDGET(widget)->handle = handle;
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_progressbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	int size = NSProgressIndicatorPreferredThickness;
	int width = 0, height = 0;
	if ((style & W_HORIZONTAL) != 0) {
		height = size;
		width = height * 10;
	} else {
		width = size;
		height = width * 10;
	}
	if (e->wHint != W_DEFAULT)
		width = e->wHint;
	if (e->hHint != W_DEFAULT)
		height = e->hHint;
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
}
void _w_progressbar_class_init(struct _w_progressbar_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
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
	 * priv
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.create_handle = _w_progressbar_create_handle;
	priv->widget.compute_size = _w_progressbar_compute_size;
}
