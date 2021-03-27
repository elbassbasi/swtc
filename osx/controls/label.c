/*
 * label.c
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */
#include "label.h"
#include "../widgets/toolkit.h"

wresult _w_label_get_image(w_label *label, w_image *image) {
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & (W_SEPARATOR | W_HYPERLINK)) != 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_label_get_text(w_label *label, w_alloc string, void *user_data,
		int enc) {
	return W_TRUE;
}
wresult _w_label_set_image(w_label *label, w_image *image) {
	if ((_W_WIDGET(label)->style & (W_SEPARATOR | W_HYPERLINK)) != 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_label_set_text(w_label *label, const char *text, int length,
		int enc) {
	return W_TRUE;
}
wuint64 _w_label_check_style(w_widget *widget, wuint64 style) {
	style |= W_NO_FOCUS;
	if ((style & W_SEPARATOR) != 0) {
		style = _w_widget_check_bits(style, W_VERTICAL, W_HORIZONTAL, 0, 0, 0,
				0);
		return _w_widget_check_bits(style, W_SHADOW_OUT, W_SHADOW_IN,
				W_SHADOW_NONE, 0, 0, 0);
	}
	return _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
}
wresult _w_label_create_handle(w_widget *widget, _w_control_priv *priv) {
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_label_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	return W_TRUE;
}
void _w_label_class_init(struct _w_label_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LABEL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_label_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_label);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_label);
	/*
	 * public function
	 */
	clazz->get_image = _w_label_get_image;
	clazz->get_text = _w_label_get_text;
	clazz->set_image = _w_label_set_image;
	clazz->set_text = _w_label_set_text;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.check_style = _w_label_check_style;
	priv->widget.create_handle = _w_label_create_handle;
	priv->widget.compute_size = _w_label_compute_size;
}
