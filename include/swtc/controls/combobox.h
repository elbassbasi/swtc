/*
 * Name:        combo.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_COMBOBOX_H_
#define SWTC_CONTROLS_COMBOBOX_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_combobox {
	w_composite composite;
	void *handle[0x10];
} w_combobox;
#define W_COMBOBOX(x) ((w_combobox*)x)
typedef struct w_comboitem {
	w_item item;
	void *handle[6];
} w_comboitem;
#define W_COMBOITEM(x) ((w_comboitem*)x)
struct _w_comboitem_class {
	struct _w_item_class item;
};
struct _w_combobox_class {
	struct _w_composite_class composite;
	struct _w_comboitem_class *class_comboitem;
	wresult (*clear_selection)(w_combobox *combo);
	wresult (*copy)(w_combobox *combo);
	wresult (*cut)(w_combobox *combo);
	wresult (*deselect)(w_combobox *combo, int index);
	wresult (*deselect_all)(w_combobox *combo);
	wresult (*get_item)(w_combobox *combo, int index, w_comboitem *item);
	wresult (*get_item_count)(w_combobox *combo);
	wresult (*get_item_height)(w_combobox *combo);
	wresult (*get_items)(w_combobox *combo, w_iterator *items);
	wresult (*get_list_visible)(w_combobox *combo);
	wresult (*get_selection)(w_combobox *combo, w_point *pt);
	wresult (*get_selection_index)(w_combobox *combo);
	wresult (*get_text)(w_combobox *combo, w_alloc alloc, void *user_data,
			int enc);
	wresult (*get_text_height)(w_combobox *combo);
	wresult (*get_text_limit)(w_combobox *combo);
	wresult (*get_visible_item_count)(w_combobox *combo);
	wresult (*indexof)(w_combobox *combo, const char *string, int length,
			int enc, int start);
	wresult (*insert_item)(w_combobox *combo, w_comboitem *item, int index);
	wresult (*paste)(w_combobox *combo);
	wresult (*remove_index)(w_combobox *combo, int index);
	wresult (*remove_range)(w_combobox *combo, int start, int end);
	wresult (*remove_string)(w_combobox *combo, const char *string,int length, int enc);
	wresult (*remove_all)(w_combobox *combo);
	wresult (*select)(w_combobox *combo, int index);
	wresult (*set_list_visible)(w_combobox *combo, int visible);
	wresult (*set_selection)(w_combobox *combo, w_point *selection);
	wresult (*set_text)(w_combobox *combo, const char *string, int length,
			int enc);
	wresult (*set_text_limit)(w_combobox *combo, int limit);
	wresult (*set_visible_item_count)(w_combobox *combo, int count);
};
SWT_PUBLIC wresult w_combobox_create(w_combobox *combo,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_combobox* w_combobox_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_combobox_insert_item(w_combobox *combo, w_comboitem *item,
		int index);
SWT_PUBLIC wresult w_combobox_clear_selection(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_copy(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_cut(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_deselect(w_combobox *combo, int index);
SWT_PUBLIC wresult w_combobox_deselect_all(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_get_item(w_combobox *combo, int index,
		w_comboitem *item);
SWT_PUBLIC wresult w_combobox_get_item_count(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_get_item_height(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_get_items(w_combobox *combo, w_iterator *items);
SWT_PUBLIC wresult w_combobox_get_list_visible(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_set_list_visible(w_combobox *combo, int visible);
SWT_PUBLIC wresult w_combobox_get_selection(w_combobox *combo, w_point *pt);
SWT_PUBLIC wresult w_combobox_get_selection_index(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_get_text(w_combobox *combo, w_alloc alloc,
		void *user_data, int enc);
SWT_PUBLIC wresult w_combobox_get_text_height(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_get_text_limit(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_get_visible_item_count(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_indexof(w_combobox *combo, const char *text,
		int length, int enc, int start);
SWT_PUBLIC wresult w_combobox_paste(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_remove_index(w_combobox *combo, int index);
SWT_PUBLIC wresult w_combobox_remove_range(w_combobox *combo, int start,
		int end);
SWT_PUBLIC wresult w_combobox_remove_string(w_combobox *combo,
		const char *string,int length, int enc);
SWT_PUBLIC wresult w_combobox_remove_all(w_combobox *combo);
SWT_PUBLIC wresult w_combobox_select(w_combobox *combo, int index);
SWT_PUBLIC wresult w_combobox_set_selection(w_combobox *combo,
		w_point *selection);
SWT_PUBLIC wresult w_combobox_set_text(w_combobox *combo, const char *text,
		int length, int enc);
SWT_PUBLIC wresult w_combobox_set_text_limit(w_combobox *combo, int limit);
SWT_PUBLIC wresult w_combobox_set_visible_item_count(w_combobox *combo,
		int count);

#define W_COMBOBOX_CLASS(x) ((struct _w_combobox_class*)x)
#define W_COMBOBOX_GET_CLASS(x) ((struct _w_combobox_class*)W_WIDGET_GET_CLASS(x))
#define W_COMBOITEM_CLASS(x) ((struct _w_comboitem_class*)x)
#define W_COMBOITEM_GET_CLASS(x) ((struct _w_comboitem_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_COMBOBOX_H_ */
