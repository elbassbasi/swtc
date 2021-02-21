/*
 * button.c
 *
 *  Created on: 3 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_button_create(w_button *button, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(button), toolkit, W_WIDGET(parent), style,
			_W_CLASS_BUTTON, post_event);
}
w_button* w_button_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_BUTTON(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_BUTTON,post_event));
}
wresult w_button_get_alignment(w_button *button) {
	wresult result = W_WIDGET_CHECK0(button);
	if (result >= 0) {
		return W_BUTTON_GET_CLASS(button)->get_alignment(button);
	} else
		return result;
}
wresult w_button_get_grayed(w_button *button) {
	wresult result = W_WIDGET_CHECK0(button);
	if (result >= 0) {
		return W_BUTTON_GET_CLASS(button)->get_grayed(button);
	} else
		return result;
}
wresult w_button_get_image(w_button *button, w_image *image) {
	wresult result = W_WIDGET_CHECK0(button);
	if (result >= 0) {
		return W_BUTTON_GET_CLASS(button)->get_image(button, image);
	} else
		return result;
}
wresult w_button_get_selection(w_button *button) {
	wresult result = W_WIDGET_CHECK0(button);
	if (result >= 0) {
		return W_BUTTON_GET_CLASS(button)->get_selection(button);
	} else
		return result;
}
wresult w_button_get_text(w_button *button, w_alloc alloc, void *user_data,int enc) {
	wresult result = W_WIDGET_CHECK0(button);
	if (result >= 0) {
		return W_BUTTON_GET_CLASS(button)->get_text(button, alloc, user_data,enc);
	} else
		return result;
}
wresult w_button_set_alignment(w_button *button, int alignment) {
	wresult result = W_WIDGET_CHECK0(button);
	if (result >= 0) {
		return W_BUTTON_GET_CLASS(button)->set_alignment(button, alignment);
	} else
		return result;
}
wresult w_button_set_grayed(w_button *button, int grayed) {
	wresult result = W_WIDGET_CHECK0(button);
	if (result >= 0) {
		return W_BUTTON_GET_CLASS(button)->set_grayed(button, grayed);
	} else
		return result;
}
wresult w_button_set_image(w_button *button, w_image *image) {
	wresult result = W_WIDGET_CHECK0(button);
	if (result >= 0) {
		return W_BUTTON_GET_CLASS(button)->set_image(button, image);
	} else
		return result;
}
wresult w_button_set_selection(w_button *button, int selected) {
	wresult result = W_WIDGET_CHECK0(button);
	if (result >= 0) {
		return W_BUTTON_GET_CLASS(button)->set_selection(button, selected);
	} else
		return result;
}
wresult w_button_set_text(w_button *button, const char *string,size_t length,int enc) {
	wresult result = W_WIDGET_CHECK0(button);
	if (result >= 0) {
		return W_BUTTON_GET_CLASS(button)->set_text(button, string,length,enc);
	} else
		return result;
}
