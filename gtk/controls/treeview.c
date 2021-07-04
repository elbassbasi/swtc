/*
 * Name:        treeview.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "treeview.h"
#include "../widgets/toolkit.h"
/*
 * treeitem
 */
wresult _w_treeitem_clear(w_treeitem *item, int index, int all) {
	return W_FALSE;
}
wresult _w_treeitem_clear_all(w_treeitem *item, int all) {
	return W_FALSE;
}
wresult _w_treeitem_for_all_item(w_treeitem *_item, w_widget_callback callback,
		void *user_data) {
	w_widget *tree = _W_ITEM(_item)->parent;
	GtkTreeView *_handle = GTK_TREE_VIEW(_W_WIDGET(tree)->handle);
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(_handle);
	w_treeitem item;
	_W_WIDGETDATA(&item)->clazz = _W_WIDGETDATA(tree)->clazz;
	_W_ITEM(&item)->parent = W_WIDGET(tree);
	_W_ITEM(&item)->index = -1;
	int i = 0;
	gboolean ret;
	GtkTreeIter *iter = &_W_TREEITEM(&item)->iter;
	GtkTreeIter pIter;
	memcpy(&pIter, & _W_TREEITEM(_item)->iter, sizeof(GtkTreeIter));
	while (ret) {
		ret = gtk_tree_model_iter_nth_child(modelHandle, iter, &pIter, 0);
		if (ret) {
			i++;
			memcpy(&pIter, iter, sizeof(GtkTreeIter));
		} else {
			memcpy(iter, &pIter, sizeof(GtkTreeIter));
			callback(tree, &item, user_data);
			while (!ret) {
				gtk_tree_model_iter_parent(modelHandle, iter, iter);
				ret = gtk_tree_model_iter_next(modelHandle, iter);
				if (!ret) {
					callback(tree, &item, user_data);
				}
				i--;
				if (i <= 0)
					break;
			}
		}
	}
	return W_TRUE;
}
wresult _w_treeitem_get_expanded(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem,
		int flags) {
	wresult result = W_FALSE;
	w_widget *tree = _W_ITEM(item)->parent;
	GtkTreeView *_handle = GTK_TREE_VIEW(_W_WIDGET(tree)->handle);
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(_handle);
	GtkTreeIter *itemIter, *subitemIter, tmp;
	if (subitem != 0) {
		subitemIter = &_W_TREEITEM(subitem)->iter;
	} else {
		subitemIter = &tmp;
	}
	itemIter = &_W_TREEITEM(item)->iter;
	gint n = -1;
	switch (flags) {
	case W_TREEITEM_INDEX:
		result = gtk_tree_model_iter_nth_child(modelHandle, subitemIter,
				itemIter, index);
		n = index;
		break;
	case W_TREEITEM_PARENT:
		result = gtk_tree_model_iter_parent(modelHandle, subitemIter, itemIter);
		break;
	case W_TREEITEM_NEXT:
		memcpy(subitemIter, itemIter, sizeof(GtkTreeIter));
		result = gtk_tree_model_iter_next(modelHandle, subitemIter);
		break;
	case W_TREEITEM_PREV:
		memcpy(subitemIter, itemIter, sizeof(GtkTreeIter));
		result = gtk_tree_model_iter_previous(modelHandle, subitemIter);
		break;
	case W_TREEITEM_LAST:
		n = gtk_tree_model_iter_n_children(modelHandle, itemIter);
		if (n > 0) {
			result = gtk_tree_model_iter_nth_child(modelHandle, subitemIter,
					itemIter, n - 1);
		}
		n -= 1;
		break;
	case W_TREEITEM_FIRST:
		result = gtk_tree_model_iter_nth_child(modelHandle, subitemIter,
				itemIter, 0);
		n = 0;
		break;
	}
	if (result > 0) {
		_W_WIDGETDATA(subitem)->clazz = _W_WIDGETDATA(item)->clazz;
		_W_ITEM(subitem)->parent = W_WIDGET(tree);
		_W_ITEM(subitem)->index = n;
		return W_TRUE;
	}
	return result;
}
wresult _w_treeitem_get_item_count(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_items(w_treeitem *item, w_iterator *items) {
	return W_FALSE;
}
wresult _w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		int index, w_treeitem *sibling, int flags) {
	GtkTreeIter *parentIter, *handle, tmp, *_sibling;
	if ((flags & W_TREEITEM_AFTER) || (flags & W_TREEITEM_BEFORE)) {
		if (sibling == 0)
			return W_ERROR_NULL_ARGUMENT;
		if (_W_WIDGETDATA(sibling)->clazz == 0)
			return W_ERROR_INVALID_ARGUMENT;
		_sibling = &_W_TREEITEM(sibling)->iter;
	}
	w_widget *tree = _W_ITEM(item)->parent;
	GtkTreeView *_handle = GTK_TREE_VIEW(_W_WIDGET(tree)->handle);
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(_handle);
	if (subitem != 0) {
		handle = &_W_TREEITEM(subitem)->iter;
	} else {
		handle = &tmp;
	}
	if (_W_TREEITEM(item)->iter.stamp != 0) {
		parentIter = &_W_TREEITEM(item)->iter;
	} else {
		parentIter = 0;
	}
	if (parentIter != 0) {
		_w_treeitem_remove_children_flags(modelHandle, parentIter);
	}
	if (flags & W_TREEITEM_AFTER) {
		if (sibling == 0)
			return W_ERROR_INVALID_ARGUMENT;
		gtk_tree_store_insert_after(GTK_TREE_STORE(modelHandle), handle,
				parentIter, _sibling);
	} else if (flags & W_TREEITEM_BEFORE) {
		gtk_tree_store_insert_before(GTK_TREE_STORE(modelHandle), handle,
				parentIter, _sibling);
	} else {
		/*
		 * Feature in GTK.  It is much faster to append to a tree store
		 * than to insert at the end using gtk_tree_store_insert().
		 */
		if (index < 0) {
			gtk_tree_store_append(GTK_TREE_STORE(modelHandle), handle,
					parentIter);
		} else {
			int count = gtk_tree_model_iter_n_children(modelHandle, parentIter);
			if (index > count) {
				return -W_ERROR_INVALID_RANGE;
			}
			gtk_tree_store_insert(GTK_TREE_STORE(modelHandle), handle,
					parentIter, index);
		}
	}
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), handle, COLUMN_IMAGE, -1,
			-1);
	if (subitem != 0) {
		_W_WIDGETDATA(subitem)->clazz = _W_WIDGETDATA(item)->clazz;
		_W_ITEM(subitem)->parent = W_WIDGET(tree);
		_W_ITEM(subitem)->index = -1;
	}
	return W_TRUE;
}
wresult _w_treeitem_remove_all(w_treeitem *item) {
	return W_FALSE;
}
void _w_treeitem_remove_children_flags(GtkTreeModel *modelHandle,
		GtkTreeIter *parent) {
	int info = 0;
	GtkTreeIter tmp;
	gtk_tree_model_get(modelHandle, parent, COLUMN_INFO, &info, -1);
	if (info & COLUMN_INFO_HAS_CHILDREN) {
		while (gtk_tree_model_iter_children(modelHandle, &tmp, parent)) {
			gtk_tree_store_remove(GTK_TREE_STORE(modelHandle), &tmp);
		}
		info &= ~COLUMN_INFO_HAS_CHILDREN;
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle), parent, COLUMN_INFO,
				info, -1);
	}
}
wresult _w_treeitem_set_expanded(w_treeitem *item, int expanded) {
	return W_FALSE;
}
wresult _w_treeitem_set_has_children(w_treeitem *item) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkTreeIter child;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(tree)->handle));
	if (!gtk_tree_model_iter_has_child(modelHandle, &_W_TREEITEM(item)->iter)) {
		int info = 0;
		gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_INFO,
				&info, -1);
		info |= COLUMN_INFO_HAS_CHILDREN;
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_INFO, info, -1);
		gtk_tree_store_append(GTK_TREE_STORE(modelHandle), &child,
				&_W_TREEITEM(item)->iter);
	}
	return W_TRUE;
}
wresult _w_treeitem_set_item_count(w_treeitem *item, int count) {
	return W_FALSE;
}
/*
 * treeview
 */
wresult _w_treeview_clear(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_deselect(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_for_all_item(w_listviewbase *list,
		w_widget_callback callback, void *user_data) {
	w_treeitem root;
	w_treeview_get_root_item(W_TREEVIEW(list), &root);
	return _w_treeitem_for_all_item(&root, callback, user_data);
}
wresult _w_treeview_get_item_from_point(w_treeview *tree, w_point *point,
		w_treeitem *item) {
	if (point == 0)
		return W_ERROR_NULL_ARGUMENT;
	GtkTreePath *path = 0;
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(tree)->handle);
	gtk_widget_realize((GtkWidget*) handle);
	int x = point->x;
	int y = point->y;
	/*
	 * On GTK4 the header is included in the entire widget's surface, so we must subtract
	 * its size from the y-coordinate. This does not apply on GTK3 as the header and
	 * "main-widget" have separate GdkWindows.
	 */
#if GTK4
#endif
	if ((_W_WIDGET(tree)->style & W_MIRRORED) != 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(tree);
		x = priv->get_client_width(W_CONTROL(tree), priv) - x;
	}
	GtkTreeViewColumn *columnHandle = 0;
	if (!gtk_tree_view_get_path_at_pos(handle, x, y, &path, &columnHandle, NULL,
	NULL))
		return W_FALSE;
	if (path == 0)
		return W_FALSE;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(handle);
	GtkTreeIter *iter = &_W_TREEITEM(item)->iter;
	if (gtk_tree_model_get_iter(modelHandle, iter, path)) {
		gboolean overExpander = FALSE;
		if (gtk_tree_view_get_expander_column(handle) == columnHandle) {
			GdkRectangle rect;
			gtk_tree_view_get_cell_area(handle, path, columnHandle, &rect);
			if ((_W_WIDGET(tree)->style & W_MIRRORED) != 0) {
				overExpander = x > rect.x + rect.width;
			} else {
				overExpander = x < rect.x;
			}
		}
		if (!overExpander) {
			_W_WIDGETDATA(item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(tree);
			_W_ITEM(item)->parent = W_WIDGET(tree);
			_W_ITEM(item)->index = -1;
		}
	}
	gtk_tree_path_free(path);
	return W_TRUE;
}
wresult _w_treeview_get_parent_item(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_get_root_item(w_treeview *tree, w_treeitem *root) {
	_W_WIDGETDATA(root)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(tree);
	_W_ITEM(root)->parent = W_WIDGET(tree);
	_W_ITEM(root)->index = -1;
	memset(&_W_TREEITEM(root)->iter, 0, sizeof(GtkTreeIter));
	return W_TRUE;
}
wresult _w_treeview_get_top_item(w_treeview *tree, w_treeitem *topitem) {
	return W_FALSE;
}
wresult _w_treeview_remove(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_set_insert_mark(w_treeview *tree, w_treeitem *item,
		int before) {
	return W_FALSE;
}
wresult _w_treeview_set_selection(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_set_top_item(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_select(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_show_item(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
/*
 * signals
 */
wresult _gtk_treeview_get_focusitem(w_widget *widget, _w_treeitem *item,
		_w_control_priv *priv) {
	wresult result = W_FALSE;
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(widget)->handle);
	GtkTreePath *path = 0;
	gtk_tree_view_get_cursor(handle, &path, NULL);
	if (path != 0) {
		GtkTreeModel *modelHandle = gtk_tree_view_get_model(handle);
		if (gtk_tree_model_get_iter(modelHandle, &item->iter, path)) {
			_W_WIDGETDATA(item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
			_W_ITEM(item)->parent = widget;
			_W_ITEM(item)->index = -1;
			result = W_TRUE;
		}
		gtk_tree_path_free(path);
	}
	return result;
}
void _gtk_treeview_send_default_selection(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	w_event_list event;
	_w_treeitem item;
	//Note, similar DefaultSelectionHandling in SWT List/Table/Tree
	wresult result = _gtk_treeview_get_focusitem(widget, &item, priv);
	if (result > 0) {
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_DEFAULTSELECTION;
		event.event.widget = widget;
		event.event.platform_event = _EVENT_PLATFORM(e);
		event.item = W_ITEM(&item);
		_w_widget_post_event(widget, (w_event*) &event);
	}
}
gboolean _gtk_treeview_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	GdkEventButton *gdkEvent = e->args[0];
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(widget)->handle);
	if (gdkEvent->window != gtk_tree_view_get_bin_window(handle))
		return FALSE;
	gboolean result = _gtk_composite_button_press_event(widget, e, priv);
	if (result != 0)
		return result;

	/*
	 * Bug 312568: If mouse double-click pressed, manually send a DefaultSelection.
	 * Bug 518414: Added rowActivated guard flag to only send a DefaultSelection when the
	 * double-click triggers a 'row-activated' signal. Note that this relies on the fact
	 * that 'row-activated' signal comes before double-click event. This prevents
	 * opening of the current highlighted item when double clicking on any expander arrow.
	 */
	if (gdkEvent->type == GDK_2BUTTON_PRESS) {
		_gtk_treeview_send_default_selection(widget, e, priv);
		_W_LISTVIEWBASE(widget)->rowActivated = FALSE;
	}

	return result;
}
gboolean _gtk_treeview_row_activated(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_treeview_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_treeview_button_release_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_treeview_changed(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GtkTreePath *path = 0;
	w_treeitem item;
	w_event_list event;
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	gtk_tree_view_get_cursor(GTK_TREE_VIEW(handle), &path, 0);
	if (path != 0) {
		GtkTreeModel *modelHandle = gtk_tree_view_get_model(
				GTK_TREE_VIEW(handle));
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_SELECTION;
		event.event.widget = widget;
		event.item = W_ITEM(&item);
		_W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
		_W_ITEM(&item)->parent = widget;
		_W_ITEM(&item)->index = -1;
		gtk_tree_model_get_iter(modelHandle, &_W_TREEITEM(&item)->iter, path);
		_w_widget_post_event(widget, (w_event*) &event);
		gtk_tree_path_free(path);
	}
	return FALSE;
}
gboolean _gtk_treeview_event_after(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_treeview_expand_collapse_cursor_row(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	return W_FALSE;
}
void _gtk_treeview_draw_inherited_background(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {

}
gboolean _gtk_treeview_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((_W_WIDGET(widget)->state & STATE_OBSCURED) != 0)
		return FALSE;
	_gtk_treeview_draw_inherited_background(widget, e, priv);
	return _gtk_composite_draw(widget, e, priv);
}
gboolean _gtk_treeview_motion_notify_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	GdkEventMotion *gdkEvent = (GdkEventMotion*) e->args[0];
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	if (gdkEvent->window != gtk_tree_view_get_bin_window(GTK_TREE_VIEW(handle)))
		return FALSE;
	return _gtk_control_motion_notify_event(widget, e, priv);
}
gboolean _gtk_treeview_row_deleted(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_treeview_row_has_child_toggled(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_treeview_start_interactive_search(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_treeview_test_collapse_row(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_treeview_test_expand_row(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gint index;
	w_treeitem item;
	w_event_list event;
	GtkTreeIter *iter = (GtkTreeIter*) e->args[0];
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	_w_treeitem_remove_children_flags(modelHandle, iter);
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_ITEM_EXPAND;
	event.event.widget = widget;
	event.item = W_ITEM(&item);
	_W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
	_W_ITEM(&item)->parent = widget;
	_W_ITEM(&item)->index = -1;
	memcpy(&_W_TREEITEM(&item)->iter, iter, sizeof(GtkTreeIter));
	_w_widget_post_event(widget, (w_event*) &event);
	return FALSE;
}
gboolean _gtk_treeview_toggled(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	_w_treeitem item;
	w_event_list event;
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	char *path = (char*) e->args[0];
	if (gtk_tree_model_get_iter_from_string(modelHandle, &item.iter, path)) {
		_W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
		_W_ITEM(&item)->parent = widget;
		_W_ITEM(&item)->index = -1;
		wresult checked = w_listitem_get_checked(W_LISTITEM(&item));
		w_listitem_set_checked(W_LISTITEM(&item), !checked);
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_SELECTION;
		event.event.widget = widget;
		event.item = W_ITEM(&item);
		_w_widget_post_event(widget, (w_event*) &event);
	}
	return FALSE;
}
void _w_treeview_class_init(struct _w_treeview_class *clazz) {
	_w_listviewbase_class_init(W_LISTVIEWBASE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TREEVIEW;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_treeview_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_treeview);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_treeview);
	/*
	 * functions
	 */
	W_LISTVIEWBASE_CLASS(clazz)->for_all_item = _w_treeview_for_all_item;
	clazz->clear = _w_treeview_clear;
	clazz->deselect = _w_treeview_deselect;
	clazz->get_item_from_point = _w_treeview_get_item_from_point;
	clazz->get_parent_item = _w_treeview_get_parent_item;
	clazz->get_root_item = _w_treeview_get_root_item;
	clazz->get_top_item = _w_treeview_get_top_item;
	clazz->remove = _w_treeview_remove;
	clazz->set_insert_mark = _w_treeview_set_insert_mark;
	clazz->set_selection = _w_treeview_set_selection;
	clazz->set_top_item = _w_treeview_set_top_item;
	clazz->select = _w_treeview_select;
	clazz->show_item = _w_treeview_show_item;
	/*
	 * tree item
	 */
	struct _w_treeitem_class *treeitem = W_TREEITEM_CLASS(
			clazz->base.class_item);
	_w_listitem_class_init(W_LISTITEM_CLASS(treeitem));
	treeitem->clear = _w_treeitem_clear;
	treeitem->clear_all = _w_treeitem_clear_all;
	treeitem->get_expanded = _w_treeitem_get_expanded;
	treeitem->get_item = _w_treeitem_get_item;
	treeitem->get_item_count = _w_treeitem_get_item_count;
	treeitem->get_items = _w_treeitem_get_items;
	treeitem->insert_item = _w_treeitem_insert_item;
	treeitem->remove_all = _w_treeitem_remove_all;
	treeitem->set_expanded = _w_treeitem_set_expanded;
	treeitem->set_has_children = _w_treeitem_set_has_children;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	/*
	 * signals
	 */
	_gtk_signal_fn *signals = _W_WIDGET_PRIV(priv)->signals;
	signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_treeview_button_press_event;
	signals[SIGNAL_ROW_ACTIVATED] = _gtk_treeview_row_activated;
	signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_treeview_key_press_event;
	signals[SIGNAL_BUTTON_RELEASE_EVENT] = _gtk_treeview_button_release_event;
	signals[SIGNAL_CHANGED] = _gtk_treeview_changed;
	signals[SIGNAL_EVENT_AFTER] = _gtk_treeview_event_after;
	signals[SIGNAL_EXPAND_COLLAPSE_CURSOR_ROW] =
			_gtk_treeview_expand_collapse_cursor_row;
	signals[SIGNAL_DRAW] = _gtk_treeview_draw;
	signals[SIGNAL_MOTION_NOTIFY_EVENT] = _gtk_treeview_motion_notify_event;
	signals[SIGNAL_ROW_DELETED] = _gtk_treeview_row_deleted;
	signals[SIGNAL_ROW_HAS_CHILD_TOGGLED] = _gtk_treeview_row_has_child_toggled;
	//signals[SIGNAL_ROW_INSERTED] = _gtk_treeview_row_inserted;
	signals[SIGNAL_START_INTERACTIVE_SEARCH] =
			_gtk_treeview_start_interactive_search;
	signals[SIGNAL_TEST_COLLAPSE_ROW] = _gtk_treeview_test_collapse_row;
	signals[SIGNAL_TEST_EXPAND_ROW] = _gtk_treeview_test_expand_row;
	signals[SIGNAL_TOGGLED] = _gtk_treeview_toggled;
}
