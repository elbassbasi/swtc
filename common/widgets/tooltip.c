/*
 * tooltip.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_tooltip_create(w_tooltip *tooltip, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(tooltip), toolkit, W_WIDGET(parent), style,
			_W_CLASS_TOOLTIP, post_event);
}
w_tooltip* w_tooltip_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_TOOLTIP(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TOOLTIP,post_event));
}
wresult w_tooltip_get_auto_hide(w_tooltip *tooltip) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->get_auto_hide(tooltip);
	} else
		return result;
}
wresult w_tooltip_get_message(w_tooltip *tooltip, w_alloc alloc,
		void *user_data, int enc) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->get_message(tooltip, alloc,
				user_data, enc);
	} else
		return result;
}
wresult w_tooltip_get_parent(w_tooltip *tooltip, w_shell **parent) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->get_parent(tooltip, parent);
	} else
		return result;
}
wresult w_tooltip_get_text(w_tooltip *tooltip, w_alloc alloc, void *user_data,
		int enc) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->get_text(tooltip, alloc, user_data,
				enc);
	} else
		return result;
}
wresult w_tooltip_get_visible(w_tooltip *tooltip) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->get_visible(tooltip);
	} else
		return result;
}
wresult w_tooltip_is_visible(w_tooltip *tooltip) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->is_visible(tooltip);
	} else
		return result;
}
wresult w_tooltip_set_autohide(w_tooltip *tooltip, int autoHide) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->set_autohide(tooltip, autoHide);
	} else
		return result;
}
wresult w_tooltip_set_location(w_tooltip *tooltip, w_point *location) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->set_location(tooltip, location);
	} else
		return result;
}
wresult w_tooltip_set_message(w_tooltip *tooltip, const char *message,
		int length, int enc) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->set_message(tooltip, message,
				length, enc);
	} else
		return result;
}
wresult w_tooltip_set_text(w_tooltip *tooltip, const char *text, int length,
		int enc) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->set_text(tooltip, text, length,
				enc);
	} else
		return result;
}
wresult w_tooltip_set_visible(w_tooltip *tooltip, int visible) {
	wresult result = W_WIDGET_CHECK0(tooltip);
	if (result > 0) {
		return W_TOOLTIP_GET_CLASS(tooltip)->set_visible(tooltip, visible);
	} else
		return result;
}
