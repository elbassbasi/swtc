/*
 * tree.c
 *
 *  Created on: 17 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
/*
 * tree item
 */
wresult w_treeitem_clear(w_treeitem *item, int index, int all) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->clear(item, index, all);
	} else
		return result;
}
wresult w_treeitem_clear_all(w_treeitem *item, int all) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->clear_all(item, all);
	} else
		return result;
}
wresult w_treeitem_for_all_children(w_treeitem *item,
		w_widget_callback callback, void *user_data, int flags) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->for_all_children(item, callback,
				user_data, flags);
	} else
		return result;
}
wresult w_treeitem_get_expanded(w_treeitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_expanded(item);
	} else
		return result;
}
wresult w_treeitem_get_first_child(w_treeitem *item, w_treeitem *child) {
	return w_treeitem_get_item_0(item, -1, child, W_TREEITEM_FIRST);
}
wresult w_treeitem_get_item_0(w_treeitem *item, int index, w_treeitem *subitem,
		int flags) {
	w_widgetdata_close(W_WIDGETDATA(subitem));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_item(item, index, subitem, flags);
	} else
		return result;
}
wresult w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem) {
	return w_treeitem_get_item_0(item, index, subitem, W_TREEITEM_INDEX);
}
wresult w_treeitem_get_item_count(w_treeitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_item_count(item);
	} else
		return result;
}
wresult w_treeitem_get_items(w_treeitem *item, w_iterator *items) {
	w_iterator_close(items);
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_items(item, items);
	} else
		return result;
}
wresult w_treeitem_get_last_child(w_treeitem *item, w_treeitem *child) {
	return w_treeitem_get_item_0(item, -1, child, W_TREEITEM_LAST);
}
wresult w_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next) {
	return w_treeitem_get_item_0(item, -1, next, W_TREEITEM_NEXT);
}
wresult w_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent) {
	return w_treeitem_get_item_0(item, -1, parent, W_TREEITEM_PARENT);
}
wresult w_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev) {
	return w_treeitem_get_item_0(item, -1, prev, W_TREEITEM_PREV);
}
wresult w_treeitem_insert_item_0(w_treeitem *item, w_treeitem *subitem,
		int index, w_treeitem *after, int flags) {
	w_widgetdata_close(W_WIDGETDATA(subitem));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->insert_item(item, subitem, index,
				after, flags);
	} else
		return result;
}
wresult w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		int index) {
	return w_treeitem_insert_item_0(item, subitem, index, 0, W_TREEITEM_INDEX);
}
wresult w_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
		w_treeitem *after) {
	return w_treeitem_insert_item_0(item, subitem, -1, after, W_TREEITEM_AFTER);
}
wresult w_treeitem_insert_item_before(w_treeitem *item, w_treeitem *subitem,
		w_treeitem *before) {
	return w_treeitem_insert_item_0(item, subitem, -1, before, W_TREEITEM_AFTER);
}
wresult w_treeitem_remove_all(w_treeitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->remove_all(item);
	} else
		return result;
}
wresult w_treeitem_set_expanded(w_treeitem *item, int expanded) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->set_expanded(item, expanded);
	} else
		return result;
}
wresult w_treeitem_set_has_children(w_treeitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->set_has_children(item);
	} else
		return result;
}
wresult w_treeitem_set_item_count(w_treeitem *item, int count,
		void **userdata) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->set_item_count(item, count, userdata);
	} else
		return result;
}
/*
 * w_treeview
 */
wresult w_treeview_create(w_treeview *tree, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(tree), toolkit, W_WIDGET(parent), style,
			_W_CLASS_TREEVIEW, post_event);
}
w_treeview* w_treeview_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_TREEVIEW(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TREEVIEW,post_event));
}
wresult w_treeview_clear(w_treeview *tree, w_treeitem *item) {
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->clear(tree, item);
	} else
		return result;
}
wresult w_treeview_deselect(w_treeview *tree, w_treeitem *item) {
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->deselect(tree, item);
	} else
		return result;
}
wresult w_treeview_get_item_from_point(w_treeview *tree, w_point *point,
		w_treeitem *item) {
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->get_item_from_point(tree, point,
				item);
	} else
		return result;
}
wresult w_treeview_get_root_item(w_treeview *tree, w_treeitem *root) {
	w_widgetdata_close(W_WIDGETDATA(root));
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->get_root_item(tree, root);
	} else
		return result;
}
wresult w_treeview_get_top_item(w_treeview *tree, w_treeitem *topitem) {
	w_widgetdata_close(W_WIDGETDATA(topitem));
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->get_top_item(tree, topitem);
	} else
		return result;
}
wresult w_treeview_remove(w_treeview *tree, w_treeitem *item) {
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->remove(tree, item);
	} else
		return result;
}
wresult w_treeview_set_insert_mark(w_treeview *tree, w_treeitem *item,
		int before) {
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->set_insert_mark(tree, item, before);
	} else
		return result;
}
wresult w_treeview_select(w_treeview *tree, w_treeitem *item) {
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->select(tree, item);
	} else
		return result;
}
wresult w_treeview_set_selection(w_treeview *tree, w_treeitem *item) {
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->set_selection(tree, item);
	} else
		return result;
}
wresult w_treeview_set_top_item(w_treeview *tree, w_treeitem *item) {
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->set_top_item(tree, item);
	} else
		return result;
}
wresult w_treeview_show_item(w_treeview *tree, w_treeitem *item) {
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->show_item(tree, item);
	} else
		return result;
}
