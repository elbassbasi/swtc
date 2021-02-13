/*
 * Name:        tracker.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_TRACKER_H_
#define SWT_WIDGETS_TRACKER_H_
#include "widget.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_tracker {
	w_widget widget;
	void *handle[0x10];
} w_tracker;

#define W_TRACKER(x) ((w_tracker*)x)

struct _w_tracker_class {
	struct _w_widget_class widget;
	wresult (*close)(w_tracker *tracker);
	wresult (*get_rectangles)(w_tracker *tracker, w_iterator* iterator);
	wresult (*get_stippled)(w_tracker *tracker);
	wresult (*open)(w_tracker *tracker);
	wresult (*set_cursor)(w_tracker *tracker, w_cursor *cursor);
	wresult (*set_rectangles)(w_tracker *tracker, w_rect *rectangles,
			size_t length);
	wresult (*set_stippled)(w_tracker *tracker, int stippled);
};
SWT_PUBLIC wresult w_tracker_create(w_tracker *tracker, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_tracker* w_tracker_new(struct w_toolkit *toolkit, w_composite *parent,
                                    wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_tracker_close(w_tracker *tracker);
SWT_PUBLIC wresult w_tracker_get_rectangles(w_tracker *tracker,w_iterator* iterator);
SWT_PUBLIC wresult w_tracker_get_stippled(w_tracker *tracker);
SWT_PUBLIC wresult w_tracker_open(w_tracker *tracker);
SWT_PUBLIC wresult w_tracker_set_cursor(w_tracker *tracker, w_cursor *cursor);
SWT_PUBLIC wresult w_tracker_set_rectangles(w_tracker *tracker, w_rect *rectangles,
		size_t length);
SWT_PUBLIC wresult w_tracker_set_stippled(w_tracker *tracker, int stippled);

#define W_TRACKER_CLASS(x) ((struct _w_tracker_class*)x)
#define W_TRACKER_GET_CLASS(x) ((struct _w_tracker_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_TRACKER_H_ */
