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
wresult w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_bounds(item, bounds);
	} else {
		memset(bounds, 0, sizeof(w_rect));
		return result;
	}
}
wresult w_treeitem_get_checked(w_treeitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_checked(item);
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
	w_widgetdata_close(W_WIDGETDATA(child));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_first_child(item, child);
	} else
		return result;
}
wresult w_treeitem_get_grayed(w_treeitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_grayed(item);
	} else
		return result;
}
wresult w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem) {
	w_widgetdata_close(W_WIDGETDATA(subitem));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_item(item, index, subitem);
	} else
		return result;
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
wresult w_treeitem_get_image(w_treeitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_image(item);
	} else
		return result;
}
wresult w_treeitem_get_last_child(w_treeitem *item, w_treeitem *child) {
	w_widgetdata_close(W_WIDGETDATA(child));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_last_child(item, child);
	} else
		return result;
}
wresult w_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next) {
	w_widgetdata_close(W_WIDGETDATA(next));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_next_sibling(item, next);
	} else
		return result;
}
wresult w_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent) {
	w_widgetdata_close(W_WIDGETDATA(parent));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_parent_item(item, parent);
	} else
		return result;
}
wresult w_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev) {
	w_widgetdata_close(W_WIDGETDATA(prev));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->get_prev_sibling(item, prev);
	} else
		return result;
}
wresult w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		int index) {
	w_widgetdata_close(W_WIDGETDATA(subitem));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->insert_item(item, subitem, index);
	} else
		return result;
}
wresult w_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
		w_treeitem *after) {
	w_widgetdata_close(W_WIDGETDATA(subitem));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->insert_item_after(item, subitem,
				after);
	} else
		return result;
}
wresult w_treeitem_insert_item_before(w_treeitem *item, w_treeitem *subitem,
		w_treeitem *before) {
	w_widgetdata_close(W_WIDGETDATA(subitem));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->insert_item_before(item, subitem,
				before);
	} else
		return result;
}
wresult w_treeitem_remove_all(w_treeitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->remove_all(item);
	} else
		return result;
}
wresult w_treeitem_set_checked(w_treeitem *item, int checked) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->set_checked(item, checked);
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
wresult w_treeitem_set_grayed(w_treeitem *item, int grayed) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->set_grayed(item, grayed);
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
wresult w_treeitem_set_image(w_treeitem *item, int image) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->set_image(item, image);
	} else
		return result;
}
wresult w_treeitem_set_item_count(w_treeitem *item, int count) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TREEITEM_GET_CLASS(item)->set_item_count(item, count);
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
wresult w_treeview_get_parent_item(w_treeview *tree, w_treeitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGET_CHECK0(tree);
	if (result > 0) {
		return W_TREEVIEW_GET_CLASS(tree)->get_parent_item(tree, item);
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
