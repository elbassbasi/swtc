/*
 * Name:        expandbar.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_EXPANDBAR_H_
#define SWTC_CONTROLS_EXPANDBAR_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_expandbar {
	w_composite composite;
	void *handle[0x10];
} w_expandbar;

#define W_EXPANDBAR(x) ((w_expandbar*)x)
typedef struct w_expanditem {
	w_item item;
	void *handle[6];
} w_expanditem;
#define W_EXPANDITEM(x) ((w_expanditem*)x)
struct _w_expanditem_class {
	struct _w_item_class item;
	w_control* (*get_control)(w_expanditem *expanditem);
	wresult (*get_expanded)(w_expanditem *expanditem);
	int (*get_header_height)(w_expanditem *expanditem);
	int (*get_height)(w_expanditem *expanditem);
	wresult (*set_control)(w_expanditem *expanditem, w_control *control);
	wresult (*set_expanded)(w_expanditem *expanditem, int expanded);
	wresult (*set_height)(w_expanditem *expanditem, int height);
	wresult (*set_image)(w_expanditem *expanditem, int image);
};
struct _w_expandbar_class {
	struct _w_composite_class composite;
	struct _w_expanditem_class *class_expanditem;
	wresult (*get_item)(w_expandbar *expandbar, int index, w_expanditem *item);
	int (*get_item_count)(w_expandbar *expandbar);
	void (*get_items)(w_expandbar *expandbar, w_iterator *items);
	int (*get_spacing)(w_expandbar *expandbar);
	wresult (*insert_item)(w_expandbar *expandbar,w_expanditem* item,const char* text,int index);
	wresult (*set_spacing)(w_expandbar *expandbar, int spacing);
};

SWT_PUBLIC w_control* w_expanditem_get_control(w_expanditem *expanditem);
SWT_PUBLIC wresult w_expanditem_get_expanded(w_expanditem *expanditem);
SWT_PUBLIC int w_expanditem_get_header_height(w_expanditem *expanditem);
SWT_PUBLIC int w_expanditem_get_height(w_expanditem *expanditem);
SWT_PUBLIC wresult w_expanditem_set_control(w_expanditem *expanditem, w_control *control);
SWT_PUBLIC wresult w_expanditem_set_expanded(w_expanditem *expanditem, int expanded);
SWT_PUBLIC wresult w_expanditem_set_height(w_expanditem *expanditem, int height);
SWT_PUBLIC wresult w_expanditem_set_image(w_expanditem *expanditem, int image);
/*
 *
 */
SWT_PUBLIC wresult w_expandbar_create(w_expandbar *expandbar, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_expandbar* w_expandbar_new(struct w_toolkit *toolkit, w_composite *parent,
                                        wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_expandbar_get_item(w_expandbar *expandbar, int index,
		w_expanditem *item);
SWT_PUBLIC int w_expandbar_get_item_count(w_expandbar *expandbar);
SWT_PUBLIC void w_expandbar_get_items(w_expandbar *expandbar, w_iterator *items);
SWT_PUBLIC int w_expandbar_get_spacing(w_expandbar *expandbar);
SWT_PUBLIC wresult w_expandbar_insert_item(w_expandbar *expandbar,w_expanditem* item,const char* text,int index);
SWT_PUBLIC wresult w_expandbar_set_spacing(w_expandbar *expandbar, int spacing);

#define W_EXPANDBAR_CLASS(x) ((struct _w_expandbar_class*)x)
#define W_EXPANDBAR_GET_CLASS(x) ((struct _w_expandbar_class*)W_WIDGET_GET_CLASS(x))
#define W_EXPANDITEM_CLASS(x) ((struct _w_expanditem_class*)x)
#define W_EXPANDITEM_GET_CLASS(x) ((struct _w_expanditem_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_EXPANDBAR_H_ */
