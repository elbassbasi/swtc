/*
 * glcanvas.c
 *
 *  Created on: 1 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_glcanvas_create(w_glcanvas *glcanvas, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(glcanvas), toolkit, W_WIDGET(parent),
			style, _W_CLASS_GLCANVAS, post_event);
}
w_glcanvas* w_glcanvas_new(struct w_toolkit *toolkit, w_composite *parent,
                           wuint64 style, w_widget_post_event_proc post_event) {
	return W_GLCANVAS(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_GLCANVAS,post_event));
}
wresult w_glcanvas_init_gldata(w_glcanvas *glcanvas, w_gldata *data) {
	if (W_WIDGET_CHECK(glcanvas)) {
		return W_GLCANVAS_GET_CLASS(glcanvas)->init_gldata(glcanvas, data);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_glcanvas_get_gldata(w_glcanvas *glcanvas, w_gldata *data) {
	if (W_WIDGET_CHECK(glcanvas)) {
		return W_GLCANVAS_GET_CLASS(glcanvas)->get_gldata(glcanvas, data);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_glcanvas_is_current(w_glcanvas *glcanvas) {
	if (W_WIDGET_CHECK(glcanvas)) {
		return W_GLCANVAS_GET_CLASS(glcanvas)->is_current(glcanvas);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_glcanvas_set_current(w_glcanvas *glcanvas) {
	if (W_WIDGET_CHECK(glcanvas)) {
		return W_GLCANVAS_GET_CLASS(glcanvas)->set_current(glcanvas);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_glcanvas_swap_buffers(w_glcanvas *glcanvas) {
	if (W_WIDGET_CHECK(glcanvas)) {
		return W_GLCANVAS_GET_CLASS(glcanvas)->swap_buffers(glcanvas);
	} else
		return W_ERROR_NO_HANDLES;
}
