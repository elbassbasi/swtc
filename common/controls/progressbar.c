/*
 * progressbar.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_progressbar_create(w_progressbar *progressbar,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(progressbar), toolkit, W_WIDGET(parent),
			style, _W_CLASS_PROGRESSBAR, post_event);
}
w_progressbar* w_progressbar_new(struct w_toolkit *toolkit, w_composite *parent,
                                 wuint64 style, w_widget_post_event_proc post_event) {
	return W_PROGRESSBAR(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_PROGRESSBAR,post_event));
}
wresult w_progressbar_get_maximum(w_progressbar *progressbar) {
	if (W_WIDGET_CHECK(progressbar)) {
		return W_PROGRESSBAR_GET_CLASS(progressbar)->get_maximum(progressbar);
	} else
		return 0;
}
wresult w_progressbar_get_selection(w_progressbar *progressbar) {
	if (W_WIDGET_CHECK(progressbar)) {
		return W_PROGRESSBAR_GET_CLASS(progressbar)->get_selection(progressbar);
	} else
		return 0;
}
wresult w_progressbar_get_state(w_progressbar *progressbar) {
	if (W_WIDGET_CHECK(progressbar)) {
		return W_PROGRESSBAR_GET_CLASS(progressbar)->get_state(progressbar);
	} else
		return 0;
}
wresult w_progressbar_set_maximum(w_progressbar *progressbar, int value) {
	if (W_WIDGET_CHECK(progressbar)) {
		return W_PROGRESSBAR_GET_CLASS(progressbar)->set_maximum(progressbar,value);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_progressbar_set_selection(w_progressbar *progressbar, int value) {
	if (W_WIDGET_CHECK(progressbar)) {
		return W_PROGRESSBAR_GET_CLASS(progressbar)->set_selection(progressbar,value);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_progressbar_set_state(w_progressbar *progressbar, int state) {
	if (W_WIDGET_CHECK(progressbar)) {
		return W_PROGRESSBAR_GET_CLASS(progressbar)->set_state(progressbar,state);
	} else
		return W_ERROR_NO_HANDLES;
}

