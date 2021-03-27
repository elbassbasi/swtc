/*
 * textedit.c
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */
#include "textedit.h"
#include "../widgets/toolkit.h"
wresult _w_textedit_append(w_textedit *text, const char *string, size_t length,
		int enc) {
	return W_FALSE;
}
wuint64 _w_textedit_check_style(w_widget *control, wuint64 style) {
	return style;
}
wresult _w_textedit_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_textedit_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_textedit_create_handle(w_widget *widget, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_textedit_clear_selection(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_copy(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_cut(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_get_caret_line_number(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_get_caret_location(w_textedit *text, w_point *location) {
	return W_FALSE;
}
wresult _w_textedit_get_caret_position(w_textedit *text, int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_char_count(w_textedit *text, int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_doubleclick_enabled(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_get_echo_char(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_get_editable(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_get_line_count(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_get_line_delimiter(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_get_line_height(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_get_message(w_textedit *text, w_alloc alloc,
		void *user_data, int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_position(w_textedit *text, w_point *point, int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_selection(w_textedit *text, w_range *range, int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_selection_count(w_textedit *text, int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_selection_text(w_textedit *text, w_alloc alloc,
		void *user_data, int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_tabs(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_get_text(w_textedit *text, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_text_range(w_textedit *text, w_range *range,
		w_alloc alloc, void *user_data, int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_text_limit(w_textedit *text, int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_top_index(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_get_top_pixel(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_insert(w_textedit *text, const char *string, size_t length,
		int enc) {
	return W_FALSE;
}
wresult _w_textedit_paste(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_select_all(w_textedit *text) {
	return W_FALSE;
}
wresult _w_textedit_set_doubleclick_enabled(w_textedit *text, int doubleClick) {
	return W_FALSE;
}
wresult _w_textedit_set_echochar(w_textedit *text, int echo) {
	return W_FALSE;
}
wresult _w_textedit_set_editable(w_textedit *text, int editable) {
	return W_FALSE;
}
wresult _w_textedit_set_message(w_textedit *text, const char *message,
		size_t length, int enc) {
	return W_FALSE;
}
wresult _w_textedit_set_selection(w_textedit *text, int start, int enc) {
	return W_FALSE;
}
wresult _w_textedit_set_selection_range(w_textedit *text, w_range *selection,
		int enc) {
	return W_FALSE;
}
wresult _w_textedit_set_tabs(w_textedit *text, int tabs) {
	return W_FALSE;
}
wresult _w_textedit_set_text(w_textedit *text, const char *string,
		size_t length, int enc) {
	return W_FALSE;
}
wresult _w_textedit_set_text_limit(w_textedit *text, int limit) {
	return W_FALSE;
}
wresult _w_textedit_set_top_index(w_textedit *text, int index) {
	return W_FALSE;
}
wresult _w_textedit_show_selection(w_textedit *text) {
	return W_FALSE;
}
void _w_textedit_class_init(struct _w_textedit_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TEXTEDIT;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_textedit_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_textedit);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_textedit);
	/*
	 * public function
	 */
	clazz->append = _w_textedit_append;
	clazz->clear_selection = _w_textedit_clear_selection;
	clazz->copy = _w_textedit_copy;
	clazz->cut = _w_textedit_cut;
	clazz->get_caret_line_number = _w_textedit_get_caret_line_number;
	clazz->get_caret_location = _w_textedit_get_caret_location;
	clazz->get_caret_position = _w_textedit_get_caret_position;
	clazz->get_char_count = _w_textedit_get_char_count;
	clazz->get_doubleclick_enabled = _w_textedit_get_doubleclick_enabled;
	clazz->get_echo_char = _w_textedit_get_echo_char;
	clazz->get_editable = _w_textedit_get_editable;
	clazz->get_line_count = _w_textedit_get_line_count;
	clazz->get_line_delimiter = _w_textedit_get_line_delimiter;
	clazz->get_line_height = _w_textedit_get_line_height;
	clazz->get_message = _w_textedit_get_message;
	clazz->get_position = _w_textedit_get_position;
	clazz->get_selection = _w_textedit_get_selection;
	clazz->get_selection_count = _w_textedit_get_selection_count;
	clazz->get_selection_text = _w_textedit_get_selection_text;
	clazz->get_tabs = _w_textedit_get_tabs;
	clazz->get_text = _w_textedit_get_text;
	clazz->get_text_range = _w_textedit_get_text_range;
	clazz->get_text_limit = _w_textedit_get_text_limit;
	clazz->get_top_index = _w_textedit_get_top_index;
	clazz->get_top_pixel = _w_textedit_get_top_pixel;
	clazz->insert = _w_textedit_insert;
	clazz->paste = _w_textedit_paste;
	clazz->select_all = _w_textedit_select_all;
	clazz->set_doubleclick_enabled = _w_textedit_set_doubleclick_enabled;
	clazz->set_echochar = _w_textedit_set_echochar;
	clazz->set_editable = _w_textedit_set_editable;
	clazz->set_message = _w_textedit_set_message;
	clazz->set_selection = _w_textedit_set_selection;
	clazz->set_selection_range = _w_textedit_set_selection_range;
	clazz->set_tabs = _w_textedit_set_tabs;
	clazz->set_text = _w_textedit_set_text;
	clazz->set_text_limit = _w_textedit_set_text_limit;
	clazz->set_top_index = _w_textedit_set_top_index;
	clazz->show_selection = _w_textedit_show_selection;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.create_handle = _w_textedit_create_handle;
	priv->widget.check_style = _w_textedit_check_style;
	priv->widget.compute_size = _w_textedit_compute_size;
	priv->widget.compute_trim = _w_textedit_compute_trim;
}

