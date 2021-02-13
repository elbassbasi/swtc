/*
 * tracker.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_tracker_create(w_tracker *tracker, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(tracker), toolkit, W_WIDGET(parent), style,
			_W_CLASS_TRACKER, post_event);
}
w_tracker* w_tracker_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_TRACKER(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TRACKER,post_event));
}
wresult w_tracker_close(w_tracker *tracker) {
	wresult result = W_WIDGET_CHECK0(tracker);
	if (result > 0) {
		return W_TRACKER_GET_CLASS(tracker)->close(tracker);
	} else
		return result;
}
wresult w_tracker_get_rectangles(w_tracker *tracker, w_iterator *iterator) {
	wresult result = W_WIDGET_CHECK0(tracker);
	if (result > 0) {
		return W_TRACKER_GET_CLASS(tracker)->get_rectangles(tracker, iterator);
	} else
		return result;
}
wresult w_tracker_get_stippled(w_tracker *tracker) {
	wresult result = W_WIDGET_CHECK0(tracker);
	if (result > 0) {
		return W_TRACKER_GET_CLASS(tracker)->get_stippled(tracker);
	} else
		return result;
}
wresult w_tracker_open(w_tracker *tracker) {
	wresult result = W_WIDGET_CHECK0(tracker);
	if (result > 0) {
		return W_TRACKER_GET_CLASS(tracker)->open(tracker);
	} else
		return result;
}
wresult w_tracker_set_cursor(w_tracker *tracker, w_cursor *cursor) {
	wresult result = W_WIDGET_CHECK0(tracker);
	if (result > 0) {
		return W_TRACKER_GET_CLASS(tracker)->set_cursor(tracker, cursor);
	} else
		return result;
}
wresult w_tracker_set_rectangles(w_tracker *tracker, w_rect *rectangles,
		size_t length) {
	wresult result = W_WIDGET_CHECK0(tracker);
	if (result > 0) {
		return W_TRACKER_GET_CLASS(tracker)->set_rectangles(tracker, rectangles,
				length);
	} else
		return result;
}
wresult w_tracker_set_stippled(w_tracker *tracker, int stippled) {
	wresult result = W_WIDGET_CHECK0(tracker);
	if (result > 0) {
		return W_TRACKER_GET_CLASS(tracker)->set_stippled(tracker, stippled);
	} else
		return result;
}
