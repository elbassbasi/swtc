/*
 * label.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc/swtc.h>

wresult w_label_create(w_label *label, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(label), toolkit, W_WIDGET(parent), style,
			_W_CLASS_LABEL, post_event);
}
w_label* w_label_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_LABEL(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_LABEL,post_event));
}

wresult w_label_get_image(w_label *label, w_image *image) {
	wresult result = W_WIDGET_CHECK0(label);
	if (result > 0) {
		return W_LABEL_GET_CLASS(label)->get_image(label, image);
	} else
		return result;
}
wresult w_label_get_text(w_label *label, w_alloc alloc, void *user_data,int enc) {
	wresult result = W_WIDGET_CHECK0(label);
	if (result > 0) {
		return W_LABEL_GET_CLASS(label)->get_text(label,alloc, user_data,enc);
	} else
		return result;
}
wresult w_label_set_image(w_label *label, w_image *image) {
	wresult result = W_WIDGET_CHECK0(label);
	if (result > 0) {
		return W_LABEL_GET_CLASS(label)->set_image(label, image);
	} else
		return result;
}
wresult w_label_set_text(w_label *label, const char *string,int length,int enc) {
	wresult result = W_WIDGET_CHECK0(label);
	if (result > 0) {
		return W_LABEL_GET_CLASS(label)->set_text(label, string,length,enc);
	} else
		return result;
}
