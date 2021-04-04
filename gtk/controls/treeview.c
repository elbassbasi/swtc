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
wresult _w_treeitem_get_data(w_item *item, void **data) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(tree)->handle));
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_USER_DATA,
			data, -1);
	return W_TRUE;
}
wresult _w_treeitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_treeitem_set_data(w_item *item, void *data) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), &_W_TREEITEM(item)->iter,
			COLUMN_USER_DATA, data, -1);
	return W_TRUE;
}
wresult _w_treeitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	int newlength, mnemonic;
	char *s = _gtk_text_fix(text, length, enc, &newlength, &mnemonic);
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), &_W_TREEITEM(item)->iter,
			COLUMN_TEXT, s, -1);
	_gtk_text_free(text, s, newlength);
	return W_TRUE;
}
wresult _w_treeitem_clear(w_treeitem *item, int index, int all) {
	return W_FALSE;
}
wresult _w_treeitem_clear_all(w_treeitem *item, int all) {
	return W_FALSE;
}
wresult _w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_treeitem_get_checked(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_expanded(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_first_child(w_treeitem *item, w_treeitem *child) {
	return W_FALSE;
}
wresult _w_treeitem_get_grayed(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem) {
	return W_FALSE;
}
wresult _w_treeitem_get_item_count(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_items(w_treeitem *item, w_iterator *items) {
	return W_FALSE;
}
wresult _w_treeitem_get_image(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_last_child(w_treeitem *item, w_treeitem *child) {
	return W_FALSE;
}
wresult _w_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next) {
	return W_FALSE;
}
wresult _w_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent) {
	return W_FALSE;
}
wresult _w_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev) {
	return W_FALSE;
}
wresult _w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		int index) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *_handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(_handle));
	GtkTreeIter *parentIter = 0, *handle, tmp;
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
	/*
	 * Feature in GTK.  It is much faster to append to a tree store
	 * than to insert at the end using gtk_tree_store_insert().
	 */
	if (index < 0) {
		gtk_tree_store_append(GTK_TREE_STORE(modelHandle), handle, parentIter);
	} else {
		int count = gtk_tree_model_iter_n_children(modelHandle, parentIter);
		if (index > count) {
			return -W_ERROR_INVALID_RANGE;
		}
		gtk_tree_store_insert(GTK_TREE_STORE(modelHandle), handle, parentIter,
				index);
	}
	/*int id = getId(item.handle, false);
	 items[id] = item;
	 modelChanged = true;
	 */
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), handle, COLUMN_IMAGE, -1,
			-1);
	if (subitem != 0) {
		_W_WIDGETDATA(subitem)->clazz = _W_WIDGETDATA(item)->clazz;
		_W_ITEM(subitem)->parent = W_WIDGET(tree);
		_W_ITEM(subitem)->index = -1;
	}
	return W_TRUE;
}
wresult _w_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
		w_treeitem *after) {
	return W_FALSE;
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
wresult _w_treeitem_set_checked(w_treeitem *item, int checked) {
	return W_FALSE;
}
wresult _w_treeitem_set_expanded(w_treeitem *item, int expanded) {
	return W_FALSE;
}
wresult _w_treeitem_set_grayed(w_treeitem *item, int grayed) {
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
wresult _w_treeitem_set_image(w_treeitem *item, int image) {
	GtkWidget *handle = _W_WIDGET(_W_ITEM(item)->parent)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), &_W_TREEITEM(item)->iter,
			COLUMN_IMAGE, image, -1);
	return W_TRUE;
}
wresult _w_treeitem_set_item_count(w_treeitem *item, int count) {
	return W_FALSE;
}
/*
 * treeview
 */
void _w_treeview_registre_signal(_w_treeview_priv *priv) {
	if (priv->signal_changed_id == 0) {
		//signal
		priv->signal_changed_id = g_signal_lookup("changed",
				gtk_tree_selection_get_type());
		priv->signal_row_activated_id = g_signal_lookup("row-activated",
				gtk_tree_view_get_type());
		priv->signal_test_expand_row_id = g_signal_lookup("test-expand-row",
				gtk_tree_view_get_type());
		priv->signal_test_collapse_row_id = g_signal_lookup("test-collapse-row",
				gtk_tree_view_get_type());
		priv->signal_expand_collapse_cursor_row_id = g_signal_lookup(
				"expand-collapse-cursor-row", gtk_tree_view_get_type());
		priv->signal_row_has_child_toggled_id = g_signal_lookup(
				"row-has-child-toggled", gtk_tree_model_get_type());
		priv->signal_start_interactive_search_id = g_signal_lookup(
				"start-interactive-search", gtk_tree_view_get_type());
		priv->signal_row_inserted_id = g_signal_lookup("row-inserted",
				gtk_tree_model_get_type());
		priv->signal_row_deleted_id = g_signal_lookup("row-deleted",
				gtk_tree_model_get_type());
	}
}
void _w_treeview_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_composite_hook_events(widget, priv);
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	GtkTreeSelection *selection = gtk_tree_view_get_selection(
			GTK_TREE_VIEW(handle));
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	_w_treeview_priv *tpriv = (_w_treeview_priv*) priv;
	_w_treeview_registre_signal(tpriv);
	_w_widget_connect((GtkWidget*) selection, SIGNAL_CHANGED,
			tpriv->signal_changed_id, FALSE);
	_w_widget_connect(handle, SIGNAL_ROW_ACTIVATED,
			tpriv->signal_row_activated_id, FALSE);
	_w_widget_connect(handle, SIGNAL_TEST_EXPAND_ROW,
			tpriv->signal_test_expand_row_id, FALSE);
	_w_widget_connect(handle, SIGNAL_TEST_COLLAPSE_ROW,
			tpriv->signal_test_collapse_row_id, FALSE);
	_w_widget_connect(handle, SIGNAL_EXPAND_COLLAPSE_CURSOR_ROW,
			tpriv->signal_expand_collapse_cursor_row_id,
			FALSE);
	_w_widget_connect((GtkWidget*) modelHandle, SIGNAL_ROW_HAS_CHILD_TOGGLED,
			tpriv->signal_row_has_child_toggled_id, FALSE);
	_w_widget_connect(handle, SIGNAL_START_INTERACTIVE_SEARCH,
			tpriv->signal_start_interactive_search_id, FALSE);
	//if (fixAccessibility()) {
	_w_widget_connect((GtkWidget*) modelHandle, SIGNAL_ROW_INSERTED,
			tpriv->signal_row_inserted_id, TRUE);
	_w_widget_connect((GtkWidget*) modelHandle, SIGNAL_ROW_DELETED,
			tpriv->signal_row_deleted_id, TRUE);
	//}
}
wresult _w_treeview_clear(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_deselect(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_get_item_from_point(w_treeview *tree, w_point *point,
		w_treeitem *item) {
	return W_FALSE;
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
gboolean _gtk_treeview_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	return W_FALSE;
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
		_w_widget_send_event(widget, (w_event*) &event);
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
gboolean _gtk_treeview_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_treeview_motion_notify_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	return W_FALSE;
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
	_w_widget_send_event(widget, (w_event*) &event);
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
		GtkTreeModel *modelHandle = gtk_tree_view_get_model(
				GTK_TREE_VIEW(handle));
		int info = 0;
		gtk_tree_model_get(modelHandle, &item.iter, COLUMN_INFO, &info, -1);
		if (info & COLUMN_INFO_CHECK) {
			info &= ~COLUMN_INFO_CHECK;
		} else {
			info |= COLUMN_INFO_CHECK;
		}
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle), &item.iter, COLUMN_INFO,
				info, -1);
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_SELECTION;
		event.event.widget = widget;
		event.item = W_ITEM(&item);
		_w_widget_send_event(widget, (w_event*) &event);
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
	_w_item_class_init(W_ITEM_CLASS(treeitem));
	W_ITEM_CLASS(treeitem)->get_data = _w_treeitem_get_data;
	W_ITEM_CLASS(treeitem)->get_text = _w_treeitem_get_text;
	W_ITEM_CLASS(treeitem)->set_data = _w_treeitem_set_data;
	W_ITEM_CLASS(treeitem)->set_text = _w_treeitem_set_text;
	treeitem->clear = _w_treeitem_clear;
	treeitem->clear_all = _w_treeitem_clear_all;
	treeitem->get_bounds = _w_treeitem_get_bounds;
	treeitem->get_checked = _w_treeitem_get_checked;
	treeitem->get_expanded = _w_treeitem_get_expanded;
	treeitem->get_first_child = _w_treeitem_get_first_child;
	treeitem->get_grayed = _w_treeitem_get_grayed;
	treeitem->get_item = _w_treeitem_get_item;
	treeitem->get_item_count = _w_treeitem_get_item_count;
	treeitem->get_items = _w_treeitem_get_items;
	treeitem->get_image = _w_treeitem_get_image;
	treeitem->get_last_child = _w_treeitem_get_last_child;
	treeitem->get_next_sibling = _w_treeitem_get_next_sibling;
	treeitem->get_parent_item = _w_treeitem_get_parent_item;
	treeitem->get_prev_sibling = _w_treeitem_get_prev_sibling;
	treeitem->insert_item = _w_treeitem_insert_item;
	treeitem->insert_item_after = _w_treeitem_insert_item_after;
	treeitem->remove_all = _w_treeitem_remove_all;
	treeitem->set_checked = _w_treeitem_set_checked;
	treeitem->set_expanded = _w_treeitem_set_expanded;
	treeitem->set_grayed = _w_treeitem_set_grayed;
	treeitem->set_image = _w_treeitem_set_image;
	treeitem->set_has_children = _w_treeitem_set_has_children;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->hook_events = _w_treeview_hook_events;
	/*
	 * signals
	 */
	_gtk_signal *signals = _W_WIDGET_PRIV(priv)->signals;
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
