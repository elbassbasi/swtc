/*
 * Name:        scrollable.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_SCROLLABLE_H_
#define SWT_WIDGETS_SCROLLABLE_H_
#include "control.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_scrollable {
	w_control control;
	void *handle[0x20];
} w_scrollable;
typedef struct w_scrollbar {
	w_widgetdata widgetdata;
	void *handle[6];
} w_scrollbar;
#define W_SCROLLBAR(x) ((w_scrollbar*)x)
typedef struct w_scrollbar_value {
	int selection;
	int minimum;
	int maximum;
	int thumb;
	int increment;
	int pageIncrement;
} w_scrollbar_value;
#define W_SCROLLABLE(x) ((w_scrollable*)x)
typedef struct w_event_compute_trim {
	w_event event;
	w_rect *rect;
	w_rect *result;
} w_event_compute_trim;
typedef struct w_event_client_area {
	w_event event;
	w_rect *rect;
} w_event_client_area;
typedef struct w_event_scrollbar {
	w_event event;
	int detail;
	w_scrollbar *scrollbar;
} w_event_scrollbar;
struct _w_scrollbar_class {
	struct _w_widgetdata_class widgetdata;
	wresult (*get_parent)(w_scrollbar *scrollbar, w_scrollable **parent);
	wresult (*get_enabled)(w_scrollbar *scrollbar);
	wresult (*get_increment)(w_scrollbar *scrollbar);
	wresult (*get_maximum)(w_scrollbar *scrollbar);
	wresult (*get_minimum)(w_scrollbar *scrollbar);
	wresult (*get_page_increment)(w_scrollbar *scrollbar);
	wresult (*get_selection)(w_scrollbar *scrollbar);
	wresult (*get_size)(w_scrollbar *scrollbar, w_size *size);
	wresult (*get_thumb)(w_scrollbar *scrollbar);
	wresult (*get_thumb_bounds)(w_scrollbar *scrollbar, w_rect *rect);
	wresult (*get_thumb_track_bounds)(w_scrollbar *scrollbar, w_rect *rect);
	wresult (*get_values)(w_scrollbar *scrollbar, w_scrollbar_value *values);
	wresult (*get_visible)(w_scrollbar *scrollbar);
	wresult (*is_enabled)(w_scrollbar *scrollbar);
	wresult (*is_visible)(w_scrollbar *scrollbar);
	wresult (*set_enabled)(w_scrollbar *scrollbar, int enabled);
	wresult (*set_increment)(w_scrollbar *scrollbar, int increment);
	wresult (*set_maximum)(w_scrollbar *scrollbar, int maximum);
	wresult (*set_minimum)(w_scrollbar *scrollbar, int minimum);
	wresult (*set_page_increment)(w_scrollbar *scrollbar, int pageIncrement);
	wresult (*set_selection)(w_scrollbar *scrollbar, int selection);
	wresult (*set_thumb)(w_scrollbar *scrollbar, int thumb);
	wresult (*set_values)(w_scrollbar *scrollbar, w_scrollbar_value *values);
	wresult (*set_visible)(w_scrollbar *scrollbar, int visible);
};
struct _w_scrollable_class {
	struct _w_control_class control;
	struct _w_scrollbar_class *class_scrollbar;
	wresult (*get_horizontal_bar)(w_scrollable *scrollable,
			w_scrollbar *scrollbar);
	wresult (*get_scrollbars_mode)(w_scrollable *scrollable);
	wresult (*get_vertical_bar)(w_scrollable *scrollable,
			w_scrollbar *scrollbar);
};
/*
 * ScrollBar
 */
SWT_PUBLIC wresult w_scrollbar_get_parent(w_scrollbar *scrollbar,
		w_scrollable **parent);
SWT_PUBLIC wresult w_scrollbar_get_enabled(w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollbar_is_enabled(w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollbar_set_enabled(w_scrollbar *scrollbar, int enabled);
SWT_PUBLIC wresult w_scrollbar_get_visible(w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollbar_is_visible(w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollbar_set_visible(w_scrollbar *scrollbar, int visible);
SWT_PUBLIC wresult w_scrollbar_get_values(w_scrollbar *scrollbar,
		w_scrollbar_value *values);
SWT_PUBLIC wresult w_scrollbar_get_increment(w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollbar_get_maximum(w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollbar_get_minimum(w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollbar_get_page_increment(w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollbar_get_selection(w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollbar_get_thumb(w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollbar_set_values(w_scrollbar *scrollbar,
		w_scrollbar_value *values);
SWT_PUBLIC wresult w_scrollbar_set_increment(w_scrollbar *scrollbar,
		int increment);
SWT_PUBLIC wresult w_scrollbar_set_maximum(w_scrollbar *scrollbar, int maximum);
SWT_PUBLIC wresult w_scrollbar_set_minimum(w_scrollbar *scrollbar, int minimum);
SWT_PUBLIC wresult w_scrollbar_set_page_increment(w_scrollbar *scrollbar,
		int pageIncrement);
SWT_PUBLIC wresult w_scrollbar_set_selection(w_scrollbar *scrollbar,
		int selection);
SWT_PUBLIC wresult w_scrollbar_set_thumb(w_scrollbar *scrollbar, int thumb);
SWT_PUBLIC wresult w_scrollbar_get_size(w_scrollbar *scrollbar, w_size *size);
SWT_PUBLIC wresult w_scrollbar_get_thumb_bounds(w_scrollbar *scrollbar,
		w_rect *rect);
SWT_PUBLIC wresult w_scrollbar_get_thumb_track_bounds(w_scrollbar *scrollbar,
		w_rect *rect);
/*
 * Scrollable
 */
SWT_PUBLIC wresult w_scrollable_compute_trim(w_scrollable *scrollable,
		w_rect *result, w_rect *rect);
SWT_PUBLIC wresult w_scrollable_get_client_area(w_scrollable *scrollable,
		w_rect *rect);
SWT_PUBLIC wresult w_scrollable_get_horizontal_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar);
SWT_PUBLIC wresult w_scrollable_get_scrollbars_mode(w_scrollable *scrollable);
SWT_PUBLIC wresult w_scrollable_get_vertical_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar);

#define W_SCROLLABLE_CLASS(x) ((struct _w_scrollable_class*)x)
#define W_SCROLLABLE_GET_CLASS(x) ((struct _w_scrollable_class*)W_WIDGET_GET_CLASS(x))
#define W_SCROLLBAR_CLASS(x) ((struct _w_scrollbar_class*)x)
#define W_SCROLLBAR_GET_CLASS(x) ((struct _w_scrollbar_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif

#endif /* SWT_WIDGETS_SCROLLABLE_H_ */
