/*
 * listview.c
 *
 *  Created on: 5 févr. 2021
 *      Author: azeddine
 */
#include "treeview.h"
/*
 * listitem
 */
wresult _w_listitem_copy(w_widgetdata *from, w_widgetdata *to) {
	return W_FALSE;
}
wresult _w_listitem_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
	return W_FALSE;
}
wresult _w_listitem_get_data(w_item *item, void **data) {
	return W_FALSE;
}
wresult _w_listitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_listitem_set_data(w_item *item, void *data) {
	return W_FALSE;
}
wresult _w_listitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	return W_TRUE;
}
wresult _w_listitem_get_bounds(w_listitem *item, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_listitem_get_bounds_index(w_listitem *item, int index,
		w_rect *bounds) {
	return W_FALSE;
}
wresult _w_listitem_get_checked(w_listitem *item) {
	return W_FALSE;
}
wresult _w_listitem_get_image(w_listitem *item) {
	return W_FALSE;
}
wresult _w_listitem_set_checked(w_listitem *item, int checked) {
	return W_FALSE;
}
wresult _w_listitem_set_image(w_listitem *item, int image) {
	return W_FALSE;
}
/*
 * listview
 */
wresult _w_listview_clear_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_clear_indices(w_listview *list, int *indices, int length) {
	return W_FALSE;
}
wresult _w_listview_clear_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_clear_range(w_listview *list, int start, int end) {
	return W_FALSE;
}
wresult _w_listview_deselect_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_deselect_indices(w_listview *table, int *indices,
		int length) {
	return W_FALSE;
}
wresult _w_listview_deselect_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_deselect_range(w_listview *list, int start, int end) {
	return W_FALSE;
}
wresult _w_listview_get_item(w_listview *list, int index, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_get_item_count(w_listview *list) {
	return W_FALSE;
}
wresult _w_listview_get_item_from_point(w_listview *list, w_point *point,
		w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_get_items(w_listview *list, w_iterator *items) {
	return W_FALSE;
}
wresult _w_listview_get_selection_index(w_listview *list) {
	return W_FALSE;
}
wresult _w_listview_get_top_index(w_listview *list) {
	return W_FALSE;
}
wresult _w_listview_get_top_item(w_listview *list, w_listitem *topitem) {
	return W_FALSE;
}
wresult _w_listview_is_selected(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_insert_item(w_listview *list, w_listitem *item, int index) {
	return W_TRUE;
}
wresult _w_listview_remove_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_remove_indices(w_listview *list, int *indices, int length) {
	return W_FALSE;
}
wresult _w_listview_remove_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_remove_range(w_listview *list, int start, int end) {
	return W_FALSE;
}
wresult _w_listview_select_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_select_indices(w_listview *list, int *indices, int length) {
	return W_FALSE;
}
wresult _w_listview_select_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_select_range(w_listview *list, int start, int end) {
	return W_FALSE;
}
wresult _w_listview_set_insert_mark(w_listview *list, w_listitem *item,
		int before) {
	return W_FALSE;
}
wresult _w_listview_set_selection_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_set_selection_indices(w_listview *list, int *indices,
		int length) {
	return W_FALSE;
}
wresult _w_listview_set_selection_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_set_selection_range(w_listview *list, int start, int end) {
	return W_FALSE;
}
wresult _w_listview_set_top_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_set_top_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_show_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
void _w_listview_class_init(struct _w_listview_class *clazz) {
	_w_listviewbase_class_init(W_LISTVIEWBASE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LISTVIEW;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_listview_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_listview);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_listview);
	/*
	 * function
	 */
	clazz->clear_index = _w_listview_clear_index;
	clazz->clear_indices = _w_listview_clear_indices;
	clazz->clear_item = _w_listview_clear_item;
	clazz->clear_range = _w_listview_clear_range;
	clazz->deselect_index = _w_listview_deselect_index;
	clazz->deselect_indices = _w_listview_deselect_indices;
	clazz->deselect_item = _w_listview_deselect_item;
	clazz->deselect_range = _w_listview_deselect_range;
	clazz->get_item = _w_listview_get_item;
	clazz->get_item_count = _w_listview_get_item_count;
	clazz->get_item_from_point = _w_listview_get_item_from_point;
	clazz->get_items = _w_listview_get_items;
	clazz->get_selection_index = _w_listview_get_selection_index;
	clazz->get_top_index = _w_listview_get_top_index;
	clazz->get_top_item = _w_listview_get_top_item;
	clazz->is_selected = _w_listview_is_selected;
	clazz->insert_item = _w_listview_insert_item;
	clazz->remove_index = _w_listview_remove_index;
	clazz->remove_indices = _w_listview_remove_indices;
	clazz->remove_item = _w_listview_remove_item;
	clazz->remove_range = _w_listview_remove_range;
	clazz->select_index = _w_listview_select_index;
	clazz->select_indices = _w_listview_select_indices;
	clazz->select_item = _w_listview_select_item;
	clazz->select_range = _w_listview_select_range;
	clazz->set_insert_mark = _w_listview_set_insert_mark;
	clazz->set_selection_index = _w_listview_set_selection_index;
	clazz->set_selection_indices = _w_listview_set_selection_indices;
	clazz->set_selection_item = _w_listview_set_selection_item;
	clazz->set_selection_range = _w_listview_set_selection_range;
	clazz->set_top_index = _w_listview_set_top_index;
	clazz->set_top_item = _w_listview_set_top_item;
	clazz->show_item = _w_listview_show_item;
	/*
	 * tree item
	 */
	struct _w_listitem_class *listitem = W_LISTITEM_CLASS(
			clazz->base.class_item);
	_w_item_class_init(W_ITEM_CLASS(listitem));
	W_ITEM_CLASS(listitem)->get_data = _w_listitem_get_data;
	W_ITEM_CLASS(listitem)->get_text = _w_listitem_get_text;
	W_ITEM_CLASS(listitem)->set_data = _w_listitem_set_data;
	W_ITEM_CLASS(listitem)->set_text = _w_listitem_set_text;
	listitem->get_bounds = _w_listitem_get_bounds;
	listitem->get_bounds_index = _w_listitem_get_bounds_index;
	listitem->get_checked = _w_listitem_get_checked;
	listitem->get_image = _w_listitem_get_image;
	listitem->set_checked = _w_listitem_set_checked;
	listitem->set_image = _w_listitem_set_image;
}
