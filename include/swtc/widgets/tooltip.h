/*
 * Name:        tooltip.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_TOOLTIP_H_
#define SWT_WIDGETS_TOOLTIP_H_
#include "widget.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_tooltip {
	w_widget widget;
	void *handle[0x10];
} w_tooltip;

#define W_TOOLTIP(x) ((w_tooltip*)x)

struct _w_tooltip_class {
	struct _w_widget_class widget;
	wresult (*get_auto_hide)(w_tooltip *tooltip);
	wresult (*get_message)(w_tooltip *tooltip, w_alloc alloc, void *user_data,
			int enc);
	wresult (*get_parent)(w_tooltip *tooltip, w_shell **parent);
	wresult (*get_text)(w_tooltip *tooltip, w_alloc alloc, void *user_data,
			int enc);
	wresult (*get_visible)(w_tooltip *tooltip);
	wresult (*is_visible)(w_tooltip *tooltip);
	wresult (*set_autohide)(w_tooltip *tooltip, int autoHide);
	wresult (*set_location)(w_tooltip *tooltip, w_point *location);
	wresult (*set_message)(w_tooltip *tooltip, const char *message, int length,
			int enc);
	wresult (*set_text)(w_tooltip *tooltip, const char *text, int length,
			int enc);
	wresult (*set_visible)(w_tooltip *tooltip, int visible);
};
SWT_PUBLIC wresult w_tooltip_create(w_tooltip *tooltip,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_tooltip* w_tooltip_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_tooltip_get_auto_hide(w_tooltip *tooltip);
SWT_PUBLIC wresult w_tooltip_get_message(w_tooltip *tooltip, w_alloc alloc,
		void *user_data, int enc);
SWT_PUBLIC wresult w_tooltip_get_parent(w_tooltip *tooltip, w_shell **parent);
SWT_PUBLIC wresult w_tooltip_get_text(w_tooltip *tooltip, w_alloc alloc,
		void *user_data, int enc);
SWT_PUBLIC wresult w_tooltip_get_visible(w_tooltip *tooltip);
SWT_PUBLIC wresult w_tooltip_is_visible(w_tooltip *tooltip);
SWT_PUBLIC wresult w_tooltip_set_autohide(w_tooltip *tooltip, int autoHide);
SWT_PUBLIC wresult w_tooltip_set_location(w_tooltip *tooltip,
		w_point *location);
SWT_PUBLIC wresult w_tooltip_set_message(w_tooltip *tooltip,
		const char *message, int length, int enc);
SWT_PUBLIC wresult w_tooltip_set_text(w_tooltip *tooltip, const char *text,
		int length, int enc);
SWT_PUBLIC wresult w_tooltip_set_visible(w_tooltip *tooltip, int visible);

#define W_TOOLTIP_CLASS(x) ((struct _w_tooltip_class*)x)
#define W_TOOLTIP_GET_CLASS(x) ((struct _w_tooltip_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_TOOLTIP_H_ */
