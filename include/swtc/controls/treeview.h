/*
 * Name:        tree.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_TREE_H_
#define SWTC_CONTROLS_TREE_H_
#include "listview.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * w_treeitem
 */
typedef struct w_treeitem {
	w_listitem item;
} w_treeitem;
#define W_TREEITEM(x) ((w_treeitem*)x)
enum {
	W_TREEITEM_INDEX = 0,
	W_TREEITEM_BEFORE,
	W_TREEITEM_AFTER,
	W_TREEITEM_PARENT,
	W_TREEITEM_NEXT,
	W_TREEITEM_PREV,
	W_TREEITEM_LAST,
	W_TREEITEM_FIRST
};
struct _w_treeitem_class {
	struct _w_listitem_class item;
	wresult (*clear)(w_treeitem *item, int index, int all);
	wresult (*clear_all)(w_treeitem *item, int all);
	wresult (*for_all_children)(w_treeitem *item, w_widget_callback callback,
			void *user_data, int flags);
	wresult (*get_expanded)(w_treeitem *item);
	wresult (*get_item)(w_treeitem *item, int index, w_treeitem *subitem,
			int flags);
	wresult (*get_item_count)(w_treeitem *item);
	wresult (*get_items)(w_treeitem *item, w_iterator *items);
	wresult (*insert_item)(w_treeitem *item, w_treeitem *subitem, int index,
			w_treeitem *after, int flags);
	wresult (*remove_all)(w_treeitem *item);
	wresult (*set_expanded)(w_treeitem *item, int expanded);
	wresult (*set_has_children)(w_treeitem *item);
	wresult (*set_item_count)(w_treeitem *item, int count, void **userdata);
};
#define W_TREEITEM_CLASS(x) ((struct _w_treeitem_class*)x)
#define W_TREEITEM_GET_CLASS(x) ((struct _w_treeitem_class*)W_WIDGETDATA_GET_CLASS(x))
SWT_PUBLIC wresult w_treeitem_clear(w_treeitem *item, int index, int all);
SWT_PUBLIC wresult w_treeitem_clear_all(w_treeitem *item, int all);
SWT_PUBLIC wresult w_treeitem_for_all_children(w_treeitem *item,
		w_widget_callback callback, void *user_data, int flags);
SWT_PUBLIC wresult w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds);
SWT_PUBLIC wresult w_treeitem_get_checked(w_treeitem *item);
SWT_PUBLIC wresult w_treeitem_get_expanded(w_treeitem *item);
SWT_PUBLIC wresult w_treeitem_get_first_child(w_treeitem *item,
		w_treeitem *child);
SWT_PUBLIC wresult w_treeitem_get_item_0(w_treeitem *item, int index,
		w_treeitem *subitem, int flags);
SWT_PUBLIC wresult w_treeitem_get_item(w_treeitem *item, int index,
		w_treeitem *subitem);
SWT_PUBLIC wresult w_treeitem_get_item_count(w_treeitem *item);
SWT_PUBLIC wresult w_treeitem_get_items(w_treeitem *item, w_iterator *items);
SWT_PUBLIC wresult w_treeitem_get_image(w_treeitem *item);
SWT_PUBLIC wresult w_treeitem_get_last_child(w_treeitem *item,
		w_treeitem *child);
SWT_PUBLIC wresult w_treeitem_get_next_sibling(w_treeitem *item,
		w_treeitem *next);
SWT_PUBLIC wresult w_treeitem_get_parent_item(w_treeitem *item,
		w_treeitem *parent);
SWT_PUBLIC wresult w_treeitem_get_prev_sibling(w_treeitem *item,
		w_treeitem *prev);
SWT_PUBLIC wresult w_treeitem_insert_item_0(w_treeitem *item,
		w_treeitem *subitem, int index, w_treeitem *after, int flags);
SWT_PUBLIC wresult w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		int index);
SWT_PUBLIC wresult w_treeitem_insert_item_after(w_treeitem *item,
		w_treeitem *subitem, w_treeitem *after);
SWT_PUBLIC wresult w_treeitem_insert_item_before(w_treeitem *item,
		w_treeitem *subitem, w_treeitem *before);
SWT_PUBLIC wresult w_treeitem_remove_all(w_treeitem *item);
SWT_PUBLIC wresult w_treeitem_set_checked(w_treeitem *item, int checked);
SWT_PUBLIC wresult w_treeitem_set_expanded(w_treeitem *item, int expanded);
SWT_PUBLIC wresult w_treeitem_set_has_children(w_treeitem *item);
SWT_PUBLIC wresult w_treeitem_set_image(w_treeitem *item, int image);
SWT_PUBLIC wresult w_treeitem_set_item_count(w_treeitem *item, int count,
		void **userdata);
/*
 * w_treeview
 */
typedef struct w_treeview {
	w_listviewbase base;
} w_treeview;
#define W_TREEVIEW(x) ((w_treeview*)x)
struct _w_treeview_class {
	struct _w_listviewbase_class base;
	wresult (*clear)(w_treeview *tree, w_treeitem *item);
	wresult (*deselect)(w_treeview *tree, w_treeitem *item);
	wresult (*get_item_from_point)(w_treeview *tree, w_point *point,
			w_treeitem *item);
	wresult (*get_root_item)(w_treeview *tree, w_treeitem *root);
	wresult (*get_top_item)(w_treeview *tree, w_treeitem *topitem);
	wresult (*remove)(w_treeview *tree, w_treeitem *item);
	wresult (*set_insert_mark)(w_treeview *tree, w_treeitem *item, int before);
	wresult (*set_selection)(w_treeview *tree, w_treeitem *item);
	wresult (*set_top_item)(w_treeview *tree, w_treeitem *item);
	wresult (*select)(w_treeview *tree, w_treeitem *item);
	wresult (*show_item)(w_treeview *tree, w_treeitem *item);
};
#define W_TREEVIEW_CLASS(x) ((struct _w_treeview_class*)x)
#define W_TREEVIEW_GET_CLASS(x) ((struct _w_treeview_class*)W_WIDGET_GET_CLASS(x))
SWT_PUBLIC wresult w_treeview_clear(w_treeview *tree, w_treeitem *item);
SWT_PUBLIC wresult w_treeview_deselect(w_treeview *tree, w_treeitem *item);
SWT_PUBLIC wresult w_treeview_get_item_from_point(w_treeview *tree,
		w_point *point, w_treeitem *item);
SWT_PUBLIC wresult w_treeview_get_root_item(w_treeview *tree, w_treeitem *root);
SWT_PUBLIC wresult w_treeview_get_top_item(w_treeview *tree,
		w_treeitem *topitem);
SWT_PUBLIC wresult w_treeview_remove(w_treeview *tree, w_treeitem *item);
SWT_PUBLIC wresult w_treeview_set_insert_mark(w_treeview *tree,
		w_treeitem *item, int before);
SWT_PUBLIC wresult w_treeview_set_selection(w_treeview *tree, w_treeitem *item);
SWT_PUBLIC wresult w_treeview_set_top_item(w_treeview *tree, w_treeitem *item);
SWT_PUBLIC wresult w_treeview_select(w_treeview *tree, w_treeitem *item);
SWT_PUBLIC wresult w_treeview_show_item(w_treeview *tree, w_treeitem *item);
#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_TREE_H_ */
