/*
 * groupbox.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_groupbox_create(w_groupbox *group, w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(group), toolkit, W_WIDGET(parent), style,
			_W_CLASS_GROUPBOX, post_event);
}
w_groupbox* w_groupbox_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_GROUPBOX(
			_w_widget_new(toolkit, W_WIDGET(parent), style, _W_CLASS_GROUPBOX, post_event));
}
wresult w_groupbox_get_text(w_groupbox *group, w_alloc alloc, void *user_data,
		int enc) {
	wresult result = W_WIDGET_CHECK0(group);
	if (result > 0) {
		return W_GROUPBOX_GET_CLASS(group)->get_text(group, alloc, user_data,
				enc);
	} else
		return result;
}
wresult w_groupbox_set_text(w_groupbox *group, const char *text, int length,
		int enc) {
	wresult result = W_WIDGET_CHECK0(group);
	if (result > 0) {
		return W_GROUPBOX_GET_CLASS(group)->set_text(group, text, length, enc);
	} else
		return result;
}
