/*
 * Name:        composite.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_COMPOSITE_H_
#define SWT_WIDGETS_COMPOSITE_H_
#include "scrollable.h"
#include "layout.h"
#ifdef __cplusplus
extern "C" {
#endif
struct w_composite {
	w_scrollable scrollable;
};

#define W_COMPOSITE(x) ((w_composite*)x)

struct _w_composite_class {
	struct _w_scrollable_class scrollable;
	wresult (*for_all_children)(w_composite *composite,
			w_widget_callback callback, void *user_data, int flags);
	wresult (*get_children)(w_composite *composite, w_iterator *it);
	wresult (*get_layout)(w_composite *composite, w_layout **layout);
	wresult (*get_layout_deferred)(w_composite *composite);
	wresult (*get_tab_list)(w_composite *composite, w_iterator *it);
	wresult (*is_layout_deferred)(w_composite *composite);
	wresult (*do_layout)(w_composite *composite, int changed, int all);
	wresult (*layout_changed)(w_composite *_this, w_control **changed,
			size_t length, int flags);
	wresult (*set_layout)(w_composite *composite, w_layout *do_layout);
	wresult (*set_layout_deferred)(w_composite *composite, int defer);
};
SWT_PUBLIC wresult w_composite_create(w_composite *composite,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_composite* w_composite_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_composite_for_all_children(w_composite *composite,
		w_widget_callback callback, void *user_data, int flags);
SWT_PUBLIC wresult w_composite_get_children(w_composite *composite,
		w_iterator *it);
SWT_PUBLIC wresult w_composite_get_layout(w_composite *composite,
		w_layout **layout);
SWT_PUBLIC wresult w_composite_get_layout_deferred(w_composite *composite);
SWT_PUBLIC wresult w_composite_get_tab_list(w_composite *composite,
		w_iterator *it);
SWT_PUBLIC wresult w_composite_is_layout_deferred(w_composite *composite);
SWT_PUBLIC wresult w_composite_do_layout(w_composite *composite, int changed,
		int all);
SWT_PUBLIC wresult w_composite_layout_changed(w_composite *_this,
		w_control **changed, size_t length, int flags);
SWT_PUBLIC wresult w_composite_set_layout(w_composite *composite,
		w_layout *layout);
SWT_PUBLIC wresult w_composite_set_layout_deferred(w_composite *composite,
		int defer);

#define W_COMPOSITE_CLASS(x) ((struct _w_composite_class*)x)
#define W_COMPOSITE_GET_CLASS(x) ((struct _w_composite_class*)W_WIDGET_GET_CLASS(x))
#ifdef __cplusplus
}
#endif

#endif /* SWT_WIDGETS_COMPOSITE_H_ */
