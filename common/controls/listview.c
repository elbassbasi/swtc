/*
 * listview.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
/*
 * w_columnitem
 */
wresult w_columnitem_get_alignment(w_columnitem *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->get_alignment(column);
	} else
		return result;
}
wresult w_columnitem_get_image(w_columnitem *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->get_image(column);
	} else
		return result;
}
wresult w_columnitem_get_moveable(w_columnitem *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->get_moveable(column);
	} else
		return result;
}
wresult w_columnitem_get_resizable(w_columnitem *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->get_resizable(column);
	} else
		return result;
}
wresult w_columnitem_get_tooltip_text(w_columnitem *column, w_alloc alloc,
		void *user_data, int enc) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->get_tooltip_text(column, alloc,
				user_data, enc);
	} else
		return result;
}
wresult w_columnitem_get_width(w_columnitem *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->get_width(column);
	} else
		return result;
}
wresult w_columnitem_pack(w_columnitem *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->pack(column);
	} else
		return result;
}
wresult w_columnitem_set_alignment(w_columnitem *column, int alignment) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->set_alignment(column, alignment);
	} else
		return result;
}
wresult w_columnitem_set_image(w_columnitem *column, int image) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->set_image(column, image);
	} else
		return result;
}
wresult w_columnitem_set_moveable(w_columnitem *column, int moveable) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->set_moveable(column, moveable);
	} else
		return result;
}
wresult w_columnitem_set_resizable(w_columnitem *column, int resizable) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->set_resizable(column, resizable);
	} else
		return result;
}
wresult w_columnitem_set_tooltip_text(w_columnitem *column, const char *text,
		int length, int enc) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->set_tooltip_text(column, text,
				length, enc);
	} else
		return result;
}
wresult w_columnitem_set_width(w_columnitem *column, int width) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_COLUMNITEM_GET_CLASS(column)->set_width(column, width);
	} else
		return result;
}
/*
 * w_listitem
 */
wresult w_listitem_get_attr(w_listitem *item, int index, int mask,
		w_list_textattr *attr) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->get_attr(item, index, mask, attr);
	} else
		return result;
}
wresult w_listitem_get_bounds(w_listitem *item, w_rect *bounds) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->get_bounds(item, bounds);
	} else
		return result;
}
wresult w_listitem_get_bounds_index(w_listitem *item, int index,
		w_rect *bounds) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->get_bounds(item, bounds);
	} else
		return result;
}
wresult w_listitem_get_checked(w_listitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->get_checked(item);
	} else
		return result;
}
wresult w_listitem_get_grayed(w_listitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->get_grayed(item);
	} else
		return result;
}
wresult w_listitem_get_image(w_listitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->get_image(item);
	} else
		return result;
}
wresult w_listitem_get_text(w_listitem *item, int index, w_alloc alloc,
		void *user_data, int enc) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->get_text(item, index, alloc,
				user_data, enc);
	} else
		return result;
}
wresult w_listitem_set_attr(w_listitem *item, int index, int mask,
		w_list_textattr *attr) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->set_attr(item, index, mask, attr);
	} else
		return result;
}
wresult w_listitem_set_checked(w_listitem *item, int checked) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->set_checked(item, checked);
	} else
		return result;
}
wresult w_listitem_set_grayed(w_listitem *item, int grayed) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->set_grayed(item, grayed);
	} else
		return result;
}
wresult w_listitem_set_image(w_listitem *item, int image) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->set_image(item, image);
	} else
		return result;
}
wresult w_listitem_set_text(w_listitem *item, int index, const char *text,
		int length, int enc) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_LISTITEM_GET_CLASS(item)->set_text(item, index, text, length,
				enc);
	} else
		return result;
}
/*
 * w_listviewbase
 */
wresult w_listviewbase_clear_all(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->clear_all(list);
	} else
		return result;
}
wresult w_listviewbase_deselect_all(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->deselect_all(list);
	} else
		return result;
}
wresult w_listviewbase_for_all_column(w_listviewbase *list,
		w_widget_callback callback, void *user_data) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->for_all_column(list, callback,
				user_data);
	} else
		return result;
}
wresult w_listviewbase_for_all_item(w_listviewbase *list,
		w_widget_callback callback, void *user_data) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->for_all_item(list, callback,
				user_data);
	} else
		return result;
}
wresult w_listviewbase_get_column(w_listviewbase *list, int index,
		w_columnitem *column) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_column(list, index, column);
	} else
		return result;
}
wresult w_listviewbase_get_column_count(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_column_count(list);
	} else
		return result;
}
wresult w_listviewbase_get_columns(w_listviewbase *list, w_iterator *columns) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_columns(list, columns);
	} else
		return result;
}
wresult w_listviewbase_get_gridline_width(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_gridline_width(list);
	} else
		return result;
}
wresult w_listviewbase_get_header_height(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_header_height(list);
	} else
		return result;
}
wresult w_listviewbase_get_header_imagelist(w_listviewbase *list,
		w_imagelist **imagelist) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_header_imagelist(list,
				imagelist);
	} else
		return result;
}
wresult w_listviewbase_get_header_visible(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_header_visible(list);
	} else
		return result;
}
wresult w_listviewbase_get_imagelist(w_listviewbase *list,
		w_imagelist **imagelist) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_imagelist(list, imagelist);
	} else
		return result;
}
wresult w_listviewbase_get_item_height(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_item_height(list);
	} else
		return result;
}
wresult w_listviewbase_get_lines_visible(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_lines_visible(list);
	} else
		return result;
}
wresult w_listviewbase_get_selection(w_listviewbase *list,
		w_iterator *selection) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_selection(list, selection);
	} else
		return result;
}
wresult w_listviewbase_get_selection_count(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_selection_count(list);
	} else
		return result;
}
wresult w_listviewbase_get_sort_column(w_listviewbase *list,
		w_columnitem *column) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_sort_column(list, column);
	} else
		return result;
}
wresult w_listviewbase_get_sort_direction(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->get_sort_direction(list);
	} else
		return result;
}
wresult w_listviewbase_insert_column(w_listviewbase *list, w_columnitem *column,
		int index) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->insert_column(list, column,
				index);
	} else
		return result;
}
wresult w_listviewbase_remove_all(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->remove_all(list);
	} else
		return result;
}
wresult w_listviewbase_select_all(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->select_all(list);
	} else
		return result;
}
wresult w_listviewbase_set_header_imagelist(w_listviewbase *list,
		w_imagelist *imagelist) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->set_header_imagelist(list,
				imagelist);
	} else
		return result;
}
wresult w_listviewbase_set_header_visible(w_listviewbase *list, int show) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->set_header_visible(list, show);
	} else
		return result;
}
wresult w_listviewbase_set_imagelist(w_listviewbase *list,
		w_imagelist *imagelist) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->set_imagelist(list, imagelist);
	} else
		return result;
}
wresult w_listviewbase_set_item_height(w_listviewbase *list, int itemHeight) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->set_item_height(list, itemHeight);
	} else
		return result;
}
wresult w_listviewbase_set_lines_visible(w_listviewbase *list, int show) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->set_lines_visible(list, show);
	} else
		return result;
}
wresult w_listviewbase_set_sort_column(w_listviewbase *list,
		w_columnitem *column) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->set_sort_column(list, column);
	} else
		return result;
}
wresult w_listviewbase_set_sort_direction(w_listviewbase *list, int direction) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->set_sort_direction(list,
				direction);
	} else
		return result;
}
wresult w_listviewbase_show_column(w_listviewbase *list, w_columnitem *column) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->show_column(list, column);
	} else
		return result;
}
wresult w_listviewbase_show_selection(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->show_selection(list);
	} else
		return result;
}
wresult w_listviewbase_sort(w_listviewbase *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEWBASE_GET_CLASS(list)->sort(list);
	} else
		return result;
}
/*
 * w_listview
 */
wresult w_listview_clear_index(w_listview *list, int index) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->clear_index(list, index);
	} else
		return result;
}
wresult w_listview_clear_indices(w_listview *list, int *indices, int length) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->clear_indices(list, indices, length);
	} else
		return result;
}
wresult w_listview_clear_item(w_listview *list, w_listitem *item) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->clear_item(list, item);
	} else
		return result;
}
wresult w_listview_clear_range(w_listview *list, int start, int end) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->clear_range(list, start, end);
	} else
		return result;
}
wresult w_listview_deselect_index(w_listview *list, int index) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->deselect_index(list, index);
	} else
		return result;
}
wresult w_listview_deselect_indices(w_listview *list, int *indices,
		int length) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->deselect_indices(list, indices,
				length);
	} else
		return result;
}
wresult w_listview_deselect_item(w_listview *list, w_listitem *item) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->deselect_item(list, item);
	} else
		return result;
}
wresult w_listview_deselect_range(w_listview *list, int start, int end) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->deselect_range(list, start, end);
	} else
		return result;
}
wresult w_listview_get_item(w_listview *list, int index, w_listitem *item) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->get_item(list, index, item);
	} else
		return result;
}
wresult w_listview_get_item_count(w_listview *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->get_item_count(list);
	} else
		return result;
}
wresult w_listview_get_item_from_point(w_listview *list, w_point *point,
		w_listitem *item) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->get_item_from_point(list, point,
				item);
	} else
		return result;
}
wresult w_listview_get_items(w_listview *list, w_iterator *items) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->get_items(list, items);
	} else
		return result;
}
wresult w_listview_get_selection_index(w_listview *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->get_selection_index(list);
	} else
		return result;
}
wresult w_listview_get_top_index(w_listview *list) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->get_top_index(list);
	} else
		return result;
}
wresult w_listview_get_top_item(w_listview *list, w_listitem *topitem) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->get_top_item(list, topitem);
	} else
		return result;
}
wresult w_listview_is_selected(w_listview *list, int index) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->is_selected(list, index);
	} else
		return result;
}
wresult w_listview_insert_item(w_listview *list, w_listitem *item, int index) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->insert_item(list, item, index);
	} else
		return result;
}
wresult w_listview_remove_index(w_listview *list, int index) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->remove_index(list, index);
	} else
		return result;
}
wresult w_listview_remove_indices(w_listview *list, int *indices, int length) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->remove_indices(list, indices, length);
	} else
		return result;
}
wresult w_listview_remove_item(w_listview *list, w_listitem *item) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->remove_item(list, item);
	} else
		return result;
}
wresult w_listview_remove_range(w_listview *list, int start, int end) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->remove_range(list, start, end);
	} else
		return result;
}
wresult w_listview_select_index(w_listview *list, int index) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->select_index(list, index);
	} else
		return result;
}
wresult w_listview_select_indices(w_listview *list, int *indices, int length) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->select_indices(list, indices, length);
	} else
		return result;
}
wresult w_listview_select_item(w_listview *list, w_listitem *item) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->select_item(list, item);
	} else
		return result;
}
wresult w_listview_select_range(w_listview *list, int start, int end) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->select_range(list, start, end);
	} else
		return result;
}
wresult w_listview_set_insert_mark(w_listview *list, w_listitem *item,
		int before) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->set_insert_mark(list, item, before);
	} else
		return result;
}
wresult w_listview_set_selection_index(w_listview *list, int index) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->set_selection_index(list, index);
	} else
		return result;
}
wresult w_listview_set_selection_indices(w_listview *list, int *indices,
		int length) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->set_selection_indices(list, indices,
				length);
	} else
		return result;
}
wresult w_listview_set_selection_item(w_listview *list, w_listitem *item) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->set_selection_item(list, item);
	} else
		return result;
}
wresult w_listview_set_selection_range(w_listview *list, int start, int end) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->set_selection_range(list, start, end);
	} else
		return result;
}
wresult w_listview_set_top_index(w_listview *list, int index) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->set_top_index(list, index);
	} else
		return result;
}
wresult w_listview_set_top_item(w_listview *list, w_listitem *item) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->set_top_item(list, item);
	} else
		return result;
}
wresult w_listview_show_item(w_listview *list, w_listitem *item) {
	wresult result = W_WIDGET_CHECK0(list);
	if (result > 0) {
		return W_LISTVIEW_GET_CLASS(list)->show_item(list, item);
	} else
		return result;
}
