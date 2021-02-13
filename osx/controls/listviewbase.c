/*
 * listviewbase.c
 *
 *  Created on: 5 févr. 2021
 *      Author: azeddine
 */
#include "treeview.h"
wuint64 _w_listviewbase_check_style(w_widget *widget, wuint64 style) {
	/*
	 * Feature in Windows.  Even when WS_HSCROLL or
	 * WS_VSCROLL is not specified, Windows creates
	 * trees and tables with scroll bars.  The fix
	 * is to set H_SCROLL and V_SCROLL.
	 *
	 * NOTE: This code appears on all platforms so that
	 * applications have consistent scroll bar behavior.
	 */
	if ((style & W_NO_SCROLL) == 0) {
		style |= W_HSCROLL | W_VSCROLL;
	}
	/* GTK is always FULL_SELECTION */
	style |= W_FULL_SELECTION;
	return _w_widget_check_bits(style, W_SINGLE, W_MULTI, 0, 0, 0, 0);
}
wresult _w_listviewbase_clear_all(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_deselect_all(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_column(w_listviewbase *list, int index,
		w_columnitem *column) {
	return W_FALSE;
}
wresult _w_listviewbase_get_column_count(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_columns(w_listviewbase *list, w_iterator *columns) {
	return W_FALSE;
}
wresult _w_listviewbase_get_gridline_width(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_header_height(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_header_visible(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_imagelist(w_listviewbase *list,
		w_imagelist **imagelist) {
	return W_FALSE;
}
wresult _w_listviewbase_get_item_height(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_lines_visible(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_selection(w_listviewbase *list,
		w_iterator *selection) {
	return W_FALSE;
}
wresult _w_listviewbase_get_selection_count(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_sort_column(w_listviewbase *list,
		w_columnitem *column) {
	return W_FALSE;
}
wresult _w_listviewbase_get_sort_direction(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_insert_column(w_listviewbase *list,
		w_columnitem *column, int index) {
	return W_FALSE;
}
wresult _w_listviewbase_remove_all(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_select_all(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_set_header_visible(w_listviewbase *list, int show) {
	return W_FALSE;
}
wresult _w_listviewbase_set_imagelist(w_listviewbase *list,
		w_imagelist *imagelist) {
	return W_FALSE;
}
wresult _w_listviewbase_set_item_height(w_listviewbase *list, int itemHeight) {
	return W_FALSE;
}
wresult _w_listviewbase_set_lines_visible(w_listviewbase *list, int show) {
	return W_FALSE;
}
wresult _w_listviewbase_set_sort_column(w_listviewbase *list,
		w_columnitem *column) {
	return W_FALSE;
}
wresult _w_listviewbase_set_sort_direction(w_listviewbase *list,
		int direction) {
	return W_FALSE;
}
wresult _w_listviewbase_show_column(w_listviewbase *list,
		w_columnitem *column) {
	return W_FALSE;
}
wresult _w_listviewbase_show_selection(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_sort(w_listviewbase *list) {
	return W_FALSE;
}
void _w_listviewbase_class_init(struct _w_listviewbase_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	clazz->clear_all = _w_listviewbase_clear_all;
	clazz->deselect_all = _w_listviewbase_deselect_all;
	clazz->get_column = _w_listviewbase_get_column;
	clazz->get_column_count = _w_listviewbase_get_column_count;
	clazz->get_columns = _w_listviewbase_get_columns;
	clazz->get_gridline_width = _w_listviewbase_get_gridline_width;
	clazz->get_header_height = _w_listviewbase_get_header_height;
	clazz->get_header_visible = _w_listviewbase_get_header_visible;
	clazz->get_imagelist = _w_listviewbase_get_imagelist;
	clazz->get_item_height = _w_listviewbase_get_item_height;
	clazz->get_lines_visible = _w_listviewbase_get_lines_visible;
	clazz->get_selection = _w_listviewbase_get_selection;
	clazz->get_selection_count = _w_listviewbase_get_selection_count;
	clazz->get_sort_column = _w_listviewbase_get_sort_column;
	clazz->get_sort_direction = _w_listviewbase_get_sort_direction;
	clazz->insert_column = _w_listviewbase_insert_column;
	clazz->remove_all = _w_listviewbase_remove_all;
	clazz->select_all = _w_listviewbase_select_all;
	clazz->set_header_visible = _w_listviewbase_set_header_visible;
	clazz->set_imagelist = _w_listviewbase_set_imagelist;
	clazz->set_item_height = _w_listviewbase_set_item_height;
	clazz->set_lines_visible = _w_listviewbase_set_lines_visible;
	clazz->set_sort_column = _w_listviewbase_set_sort_column;
	clazz->set_sort_direction = _w_listviewbase_set_sort_direction;
	clazz->show_column = _w_listviewbase_show_column;
	clazz->show_selection = _w_listviewbase_show_selection;
	clazz->sort = _w_listviewbase_sort;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->check_style = _w_listviewbase_check_style;
}
