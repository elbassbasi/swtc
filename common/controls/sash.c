/*
 * sash.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
wresult w_sash_create(w_sash *sash, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(sash), toolkit, W_WIDGET(parent), style,
			_W_CLASS_SASH, post_event);
}
w_sash* w_sash_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_SASH(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_SASH,post_event));
}

