/*
 * Name:        toolbar.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_TOOLBAR_H_
#define SWTC_CONTROLS_TOOLBAR_H_
#include "../widgets/shell.h"
#ifdef __cplusplus
extern "C" {
#endif
struct w_toolbar {
	w_composite composite;
	void *handle[0x10];
};

#define W_TOOLBAR(x) ((w_toolbar*)x)
typedef struct w_toolitem {
	w_item item;
	void *handle[6];
} w_toolitem;
#define W_TOOLITEM(x) ((w_toolitem*)x)
struct _w_toolitem_class {
	struct _w_item_class item;
	wresult (*get_bounds)(w_toolitem *item, w_rect *rect);
	w_control* (*get_control)(w_toolitem *item);
	wresult (*get_enabled)(w_toolitem *item);
	wresult (*get_selection)(w_toolitem *item);
	wresult (*get_tooltip_text)(w_toolitem *item,w_alloc alloc,void* user_data);
	int (*get_width)(w_toolitem *item);
	wresult (*is_enabled)(w_toolitem *item);
	wresult (*set_control)(w_toolitem *item, w_control *control);
	wresult (*set_enabled)(w_toolitem *item, int enabled);
	wresult (*set_image)(w_toolitem *item, int image);
	wresult (*set_selection)(w_toolitem *item, int selected);
	wresult (*set_tooltip_text)(w_toolitem *item, const char *string);
	wresult (*set_width)(w_toolitem *item, int width);

};
struct _w_toolbar_class {
	struct _w_composite_class composite;
	struct _w_toolitem_class *class_toolitem;
	w_imagelist* (*get_imagelist)(w_toolbar *toolbar);
	wresult (*get_item)(w_toolbar *toolbar, int index, w_toolitem *item);
	wresult (*get_item_from_point)(w_toolbar *toolbar, w_point *point,
			w_toolitem *item);
	int (*get_item_count)(w_toolbar *toolbar);
	void (*get_items)(w_toolbar *toolbar, w_iterator *items);
	int (*get_row_count)(w_toolbar *toolbar);
	wresult (*insert)(w_toolbar *toolbar, w_toolitem *item, const char *text,
			int style, int index);
	wresult (*set_imagelist)(w_toolbar *toolbar,w_imagelist* imagelist);
};
SWT_PUBLIC wresult w_toolitem_get_bounds(w_toolitem *item, w_rect *rect);
SWT_PUBLIC w_control* w_toolitem_get_control(w_toolitem *item);
SWT_PUBLIC wresult w_toolitem_get_enabled(w_toolitem *item);
SWT_PUBLIC wresult w_toolitem_get_selection(w_toolitem *item);
SWT_PUBLIC wresult w_toolitem_get_tooltip_text(w_toolitem *item,
		w_alloc alloc,void* user_data);
SWT_PUBLIC int w_toolitem_get_width(w_toolitem *item);
SWT_PUBLIC wresult w_toolitem_is_enabled(w_toolitem *item);
SWT_PUBLIC wresult w_toolitem_set_control(w_toolitem *item,
		w_control *control);
SWT_PUBLIC wresult w_toolitem_set_enabled(w_toolitem *item, int enabled);
SWT_PUBLIC wresult w_toolitem_set_image(w_toolitem *item, int image);
SWT_PUBLIC wresult w_toolitem_set_selection(w_toolitem *item, int selected);
SWT_PUBLIC wresult w_toolitem_set_tooltip_text(w_toolitem *item,
		const char *string);
SWT_PUBLIC wresult w_toolitem_set_width(w_toolitem *item, int width);
/*
 *
 */
SWT_PUBLIC wresult w_toolbar_create(w_toolbar *toolbar,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_toolbar* w_toolbar_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_imagelist* w_toolbar_get_imagelist(w_toolbar *toolbar);
SWT_PUBLIC wresult w_toolbar_get_item(w_toolbar *toolbar, int index,
		w_toolitem *item);
SWT_PUBLIC wresult w_toolbar_get_item_from_point(w_toolbar *toolbar,
		w_point *point, w_toolitem *item);
SWT_PUBLIC int w_toolbar_get_item_count(w_toolbar *toolbar);
SWT_PUBLIC void w_toolbar_get_items(w_toolbar *toolbar, w_iterator *items);
SWT_PUBLIC int w_toolbar_get_row_count(w_toolbar *toolbar);
SWT_PUBLIC wresult w_toolbar_insert(w_toolbar *toolbar, w_toolitem *item, const char *text,
		int style, int index);
SWT_PUBLIC wresult w_toolbar_set_imagelist(w_toolbar *toolbar,w_imagelist* imagelist);

#define W_TOOLBAR_CLASS(x) ((struct _w_toolbar_class*)x)
#define W_TOOLBAR_GET_CLASS(x) ((struct _w_toolbar_class*)W_WIDGET_GET_CLASS(x))
#define W_TOOLITEM_CLASS(x) ((struct _w_toolitem_class*)x)
#define W_TOOLITEM_GET_CLASS(x) ((struct _w_toolitem_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_TOOLBAR_H_ */
