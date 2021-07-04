/*
 * Name:        widget.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_WIDGET_H_
#define SWT_WIDGETS_WIDGET_H_
#include "theme.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * w_widget
 */
typedef wresult (*w_widget_post_event_proc)(w_widget *widget, w_event *event);
typedef wresult (*w_widget_callback)(w_widget *widget, void *item,
		void *user_data);
struct w_widget {
	void *v_table; //used in c++ as virtual table
	struct _w_widget_class *clazz;
	w_widget_post_event_proc post_event;
	void *handle;
	wuint64 style;
	wuint state0;
	volatile int ref;
	wuint id;
	wushort state;
	wushort children_count;
	w_widget *parent;
	w_widget *first_child;
	w_widget *next_sibling;
	w_widget *prev_sibling;
	w_theme *theme;
	void *data[5];
};
#define W_WIDGET(x) ((w_widget*)x)
#define W_WIDGET_CLASS(x) ((struct _w_widget_class*)x)
#define W_WIDGET_GET_CLASS(x) (((w_widget*)x)->clazz)
#define W_WIDGET_CHECK(x) (w_widget_check((w_widget *)x)>0)
#define W_WIDGET_CHECK0(x) (w_widget_check((w_widget *)x))
#define W_WIDGET_CHECK1(x) (x!=0 && ((w_widget*)x)->clazz !=0)
typedef void (*w_widget_init_class)(w_toolkit *toolkit, wushort classId,
		struct _w_widget_class *clazz);
struct _w_widget_class {
	w_class_id class_id;
	wushort class_size;
	wushort object_used_size;
	wushort object_total_size;
	w_toolkit *toolkit;
	struct _w_widget_class *parentClass;
	void *platformPrivate;
	void *reserved[3];
	wresult (*create)(w_widget *widget, w_widget *parent, wuint64 style,
			w_widget_post_event_proc post_event);
	wresult (*dispose)(w_widget *widget);
	wresult (*is_ok)(w_widget *widget);
	wresult (*init_themedata)(w_widget *widget, w_themedata *data);
	wresult (*get_shell)(w_widget *widget, w_shell **shell);
	wresult (*get_theme)(w_widget *widget, w_theme **theme);
	wresult (*get_parent)(w_widget *widget, w_widget **parent);
	wresult (*get_first_child)(w_widget *widget, w_widget **first_child);
	wresult (*get_next_sibling)(w_widget *widget, w_widget **next_sibling,
			int filter);
	wresult (*get_prev_sibling)(w_widget *widget, w_widget **prev_sibling,
			int filter);
	wresult (*post_event)(w_widget *widget, w_event *e, int flags);
	wresult (*set_theme)(w_widget *widget, w_theme *theme);
};
enum {
	W_EVENT_SEND = 0, //
	W_EVENT_POST = 1, //
};
SWT_PUBLIC void w_widget_init(w_widget *widget);
SWT_PUBLIC int w_widget_class_id(w_widget *widget);
SWT_PUBLIC struct _w_widget_class* w_widget_get_class(w_widget *widget);
SWT_PUBLIC wresult w_widget_is_ok(w_widget *widget);
SWT_PUBLIC wresult w_widget_init_themedata(w_widget *widget, w_themedata *data);
SWT_PUBLIC wresult w_widget_dispose(w_widget *widget);
SWT_PUBLIC void w_widget_free(w_widget *widget);
SWT_PUBLIC void w_widget_ref_create(w_widget *widget);
SWT_PUBLIC void w_widget_ref_inc(w_widget *widget);
SWT_PUBLIC w_widget* w_widget_ref_dec(w_widget *widget);
SWT_PUBLIC wresult w_widget_get_shell(w_widget *widget, w_shell **shell);
SWT_PUBLIC w_toolkit* w_widget_get_toolkit(w_widget *widget);
SWT_PUBLIC wresult w_widget_get_theme(w_widget *widget, w_theme **theme);
SWT_PUBLIC wresult w_widget_get_next_sibling(w_widget *widget,
		w_widget **next_sibling, int filter);
SWT_PUBLIC wresult w_widget_get_parent(w_widget *widget, w_widget **parent);
SWT_PUBLIC wresult w_widget_get_prev_sibling(w_widget *widget,
		w_widget **prev_sibling, int filter);
SWT_PUBLIC w_widget_post_event_proc w_widget_get_post_event(w_widget *widget);
SWT_PUBLIC w_widget_post_event_proc w_widget_set_post_event(w_widget *widget,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_widget_post_event(w_widget *widget, w_event *event,
		int flags);
SWT_PUBLIC wresult w_widget_set_id(w_widget *widget, wuint id);
SWT_PUBLIC wuint w_widget_get_id(w_widget *widget);
SWT_PUBLIC wresult w_widget_set_theme(w_widget *widget, w_theme *theme);
SWT_PUBLIC wuint64 w_widget_get_style(w_widget *widget);
SWT_PUBLIC void* w_widget_get_data(w_widget *widget, wuint index);
SWT_PUBLIC void* w_widget_set_data(w_widget *widget, wuint index, void *data);
wresult _w_widget_create(w_widget *widget, w_toolkit *toolkit, w_widget *parent,
		wuint64 style, wuint class_id, w_widget_post_event_proc post_event);
w_widget* _w_widget_new(w_toolkit *toolkit, w_widget *parent, wuint64 style,
		wuint class_id, w_widget_post_event_proc post_event);
/*
 * w_widgetdata
 */
typedef struct w_widgetdata {
	struct _w_widgetdata_class *clazz;
} w_widgetdata;
#define W_WIDGETDATA(x) ((w_widgetdata*)x)
struct _w_widgetdata_class {
	w_toolkit *toolkit;
	wresult (*is_ok)(w_widgetdata *obj);
	wresult (*close)(w_widgetdata *obj);
	wresult (*copy)(w_widgetdata *from, w_widgetdata *to);
	wresult (*equals)(w_widgetdata *obj1, w_widgetdata *obj2);
};
#define W_WIDGETDATA_GET_CLASS(x) (((w_widgetdata*)x)->clazz)
#define W_WIDGETDATA_CHECK(x) (w_widgetdata_check((w_widgetdata *)x)>0)
#define W_WIDGETDATA_CHECK0(x) (w_widgetdata_check((w_widgetdata *)x))
#define W_WIDGETDATA_CLASS(x) ((struct _w_widgetdata_class*)x)
SWT_PUBLIC wresult w_widget_check(w_widget *widget);
SWT_PUBLIC wresult w_widgetdata_check(w_widgetdata *obj);
SWT_PUBLIC void w_widgetdata_init(w_widgetdata *obj);
SWT_PUBLIC wresult w_widgetdata_is_ok(w_widgetdata *obj);
SWT_PUBLIC wresult w_widgetdata_close(w_widgetdata *obj);
SWT_PUBLIC wresult w_widgetdata_copy(w_widgetdata *from, w_widgetdata *to);
SWT_PUBLIC wresult w_widgetdata_equals(w_widgetdata *obj1, w_widgetdata *obj2);
SWT_PUBLIC wresult w_widgetdata_init_copy(w_widgetdata *from, w_widgetdata *to);
/*
 * private
 */
SWT_PUBLIC wresult _w_widgetdata_is_ok(w_widgetdata *obj);
SWT_PUBLIC wresult _w_widgetdata_close(w_widgetdata *obj);
SWT_PUBLIC wresult _w_widgetdata_copy(w_widgetdata *from, w_widgetdata *to);
SWT_PUBLIC wresult _w_widgetdata_equals(w_widgetdata *obj1, w_widgetdata *obj2);
SWT_PUBLIC void _w_widgetdata_class_init(struct _w_widgetdata_class *clazz);
/*
 * w_item
 */
typedef struct w_item {
	w_widgetdata widgetdata;
} w_item;
#define W_ITEM(x) ((w_item*)x)
struct _w_item_class {
	struct _w_widgetdata_class widgetdata;
	wresult (*get_parent_widget)(w_item *item, w_widget **parent);
	wresult (*get_data)(w_item *item, void **data);
	wresult (*get_index)(w_item *item);
	wresult (*get_text)(w_item *item, w_alloc alloc, void *user_data, int enc);
	wresult (*set_data)(w_item *item, void *data);
	wresult (*set_text)(w_item *item, const char *text, int length, int enc);
};
#define W_ITEM_CLASS(x) ((struct _w_item_class*)x)
#define W_ITEM_GET_CLASS(x) ((struct _w_item_class*)W_WIDGETDATA_GET_CLASS(x))
SWT_PUBLIC wresult w_item_get_parent_widget(w_item *item, w_widget **parent);
SWT_PUBLIC wresult w_item_get_data(w_item *item, void **data);
SWT_PUBLIC wresult w_item_get_index(w_item *item);
SWT_PUBLIC wresult w_item_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc);
SWT_PUBLIC wresult w_item_set_data(w_item *item, void *data);
SWT_PUBLIC wresult w_item_set_text(w_item *item, const char *text, int length,
		int enc);
/*
 * private
 */
typedef struct _w_item {
	w_widgetdata widgetdata;
	w_widget *parent;
	wuint index;
} _w_item;
#define _W_ITEM(x) ((_w_item*)x)
SWT_PUBLIC wresult _w_item_copy(w_widgetdata *from, w_widgetdata *to);
SWT_PUBLIC wresult _w_item_get_parent_widget(w_item *item, w_widget **parent);
SWT_PUBLIC wresult _w_item_get_data(w_item *item, void **data);
SWT_PUBLIC wresult _w_item_get_index(w_item *item);
SWT_PUBLIC wresult _w_item_get_text(w_item *item, w_alloc alloc,
		void *user_data, int enc);
SWT_PUBLIC wresult _w_item_set_data(w_item *item, void *data);
SWT_PUBLIC wresult _w_item_set_text(w_item *item, const char *text, int length,
		int enc);
void _w_item_class_init(struct _w_item_class *clazz);
#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_WIDGET_H_ */
