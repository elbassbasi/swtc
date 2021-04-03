/*
 * Name:        tabfolder.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_TABVIEW_H_
#define SWTC_CONTROLS_TABVIEW_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_tabitem {
	w_item item;
	void *handle[6];
} w_tabitem;
#define W_TABITEM(x) ((w_tabitem*)x)
typedef struct w_tabview {
	w_composite composite;
	void *handle[10];
} w_tabview;

typedef struct w_event_tabitem {
	w_event event;
	w_tabitem *item;
	w_control *control;
} w_event_tabitem;
#define W_TABVIEW(x) ((w_tabview*)x)

struct _w_tabitem_class {
	struct _w_item_class item;
	wresult (*get_bounds)(w_tabitem *item, w_rect *rect);
	int (*get_image)(w_tabitem *item);
	wresult (*remove)(w_tabitem *item, int destroy_Control);
	wresult (*set_image)(w_tabitem *item, int image);
	wresult (*pack)(w_tabitem *item, w_control *control);
};
struct _w_tabview_class {
	struct _w_composite_class composite;
	struct _w_tabitem_class *class_tabitem;
	wresult (*get_imagelist)(w_tabview *tabview, w_imagelist **imagelist);
	wresult (*get_item)(w_tabview *tabview, int index, w_tabitem *item);
	wresult (*get_item_p)(w_tabview *tabview, w_point *point, w_tabitem *item);
	wresult (*get_item_count)(w_tabview *tabview);
	wresult (*get_items)(w_tabview *tabview, w_iterator *items);
	wresult (*get_selection)(w_tabview *tabview, w_tabitem *item);
	wresult (*insert_item)(w_tabview *tabview, w_tabitem *item, int index);
	wresult (*set_imagelist)(w_tabview *tabview, w_imagelist *imagelist);
	wresult (*set_selection)(w_tabview *tabview, int index);
};
/*
 * tabitem
 */
SWT_PUBLIC w_control* w_tabitem_get_control(w_tabitem *item);
SWT_PUBLIC wresult w_tabitem_get_bounds(w_tabitem *item, w_rect *rect);
SWT_PUBLIC int w_tabitem_get_image(w_tabitem *item);
SWT_PUBLIC wresult w_tabitem_remove(w_tabitem *item, int destroy_Control);
SWT_PUBLIC wresult w_tabitem_set_image(w_tabitem *item, int image);
SWT_PUBLIC wresult w_tabitem_set_control(w_tabitem *item, w_control *control);
SWT_PUBLIC wresult w_tabitem_pack(w_tabitem *item, w_control *control);
/*
 * tabview
 */
SWT_PUBLIC wresult w_tabview_create(w_tabview *tabview,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_tabview* w_tabview_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_tabview_get_imagelist(w_tabview *tabview, w_imagelist **imagelist);
SWT_PUBLIC wresult w_tabview_get_item(w_tabview *tabview, int index,
		w_tabitem *item);
SWT_PUBLIC wresult w_tabview_get_item_p(w_tabview *tabview, w_point *point,
		w_tabitem *item);
SWT_PUBLIC wresult w_tabview_get_item_count(w_tabview *tabview);
SWT_PUBLIC wresult w_tabview_get_items(w_tabview *tabview, w_iterator *items);
SWT_PUBLIC wresult w_tabview_get_selection(w_tabview *tabview,
		w_tabitem *item);
SWT_PUBLIC wresult w_tabview_insert_item(w_tabview *tabview, w_tabitem *item, int index);
SWT_PUBLIC wresult w_tabview_set_imagelist(w_tabview *tabview,
		w_imagelist *imagelist);
SWT_PUBLIC wresult w_tabview_set_selection(w_tabview *tabview, int index);
#define W_TABVIEW_CLASS(x) ((struct _w_tabview_class*)x)
#define W_TABVIEW_GET_CLASS(x) ((struct _w_tabview_class*)W_WIDGET_GET_CLASS(x))
#define W_TABITEM_CLASS(x) ((struct _w_tabitem_class*)x)
#define W_TABITEM_GET_CLASS(x) ((struct _w_tabitem_class*)W_WIDGETDATA_GET_CLASS(x))
#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_TABVIEW_H_ */
