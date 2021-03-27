/*
 * groupbox.c
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */
#include "groupbox.h"
#include "../widgets/toolkit.h"
wuint64 _w_groupbox_check_style(w_widget *widget, wuint64 style) {
	style |= W_NO_FOCUS;
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_groupbox_create_handle(w_widget *widget, _w_control_priv *priv) {
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_groupbox_get_text(w_groupbox *group, w_alloc alloc, void *user_data,
		int enc) {
	return W_TRUE;
}
wresult _w_groupbox_set_text(w_groupbox *group, const char *string, int length,
		int enc) {
	return W_TRUE;
}
wresult _w_groupbox_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	return W_TRUE;
}
wresult _w_groupbox_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_groupbox_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	return W_FALSE;
}

void _w_groupbox_class_init(struct _w_groupbox_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_GROUPBOX;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_groupbox_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_groupbox);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_groupbox);
	/*
	 * public function
	 */
	clazz->get_text = _w_groupbox_get_text;
	clazz->set_text = _w_groupbox_set_text;
	/*
	 * priv
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->check_style = _w_groupbox_check_style;
	_W_WIDGET_PRIV(priv)->create_handle = _w_groupbox_create_handle;
	_W_WIDGET_PRIV(priv)->get_client_area = _w_groupbox_get_client_area;
	_W_WIDGET_PRIV(priv)->compute_trim = _w_groupbox_compute_trim;
	_W_WIDGET_PRIV(priv)->compute_size = _w_groupbox_compute_size;
}
