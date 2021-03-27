/*
 * progressbar.c
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */
#include "progressbar.h"
#include "../widgets/toolkit.h"
int _w_progressbar_get_maximum(w_progressbar *progressbar) {
	return W_FALSE;
}
int _w_progressbar_get_selection(w_progressbar *progressbar) {
	return W_FALSE;
}
int _w_progressbar_get_state(w_progressbar *progressbar) {
	return W_NORMAL;
}
wresult _w_progressbar_set_maximum(w_progressbar *progressbar, int value) {
	return W_FALSE;
}
wresult _w_progressbar_set_selection(w_progressbar *progressbar, int value) {
	return W_FALSE;
}
wresult _w_progressbar_set_state(w_progressbar *progressbar, int state) {
	return W_FALSE;
}
wresult _w_progressbar_create_handle(w_widget *widget, _w_control_priv *priv) {
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_progressbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	return W_FALSE;
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
