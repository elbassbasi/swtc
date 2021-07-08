/*
 * listview.c
 *
 *  Created on: 5 févr. 2021
 *      Author: azeddine
 */
#include "treeview.h"
#include "../widgets/toolkit.h"
/*
 * listitem
 */
wresult _w_listitem_copy(w_widgetdata *from, w_widgetdata *to) {
	memcpy(to, from, sizeof(_w_treeitem));
	return W_TRUE;
}
wresult _w_listitem_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
	return W_FALSE;
}
wresult _w_listitem_get_data(w_item *item, void **data) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_USER_DATA,
			data, -1);
	return W_TRUE;
}
wresult _w_listitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(tree)->handle);
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(handle);
	char *text = 0;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_TEXT,
			&text, -1);
	return _gtk_alloc_set_text(alloc, user_data, text, -1, enc);
}
wresult _w_listitem_set_data(w_item *item, void *data) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	if (tree->clazz->class_id == _W_CLASS_TREEVIEW) {
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_USER_DATA, data, -1);
	} else {
		gtk_list_store_set(GTK_LIST_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_USER_DATA, data, -1);
	}
	return W_TRUE;
}
wresult _w_listitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	int newlength, mnemonic;
	char *s = _gtk_text_fix(text, length, enc, &newlength, &mnemonic);
	if (tree->clazz->class_id == _W_CLASS_TREEVIEW) {
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_TEXT, s, -1);
	} else {
		gtk_list_store_set(GTK_LIST_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_TEXT, s, -1);
	}
	_gtk_text_free(text, s, newlength);
	return W_TRUE;
}
wresult _w_listitem_get_bounds(w_listitem *item, w_rect *bounds) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkTreeView *parentHandle = GTK_TREE_VIEW(_W_WIDGET(tree)->handle);
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(parentHandle);
	GtkTreeViewColumn *column = gtk_tree_view_get_column(parentHandle, 0);
	if (column == 0) {
		memset(bounds, 0, sizeof(w_rect));
		return W_TRUE;
	}
	GtkCellRenderer *textRenderer = g_object_get_qdata(G_OBJECT(column),
			gtk_toolkit->quark[1]);
	GtkCellRenderer *pixbufRenderer = _W_LISTVIEWBASE(tree)->pixbufrenderer;
	if (textRenderer == 0 || pixbufRenderer == 0) {
		memset(bounds, 0, sizeof(w_rect));
		return W_TRUE;
	}

	GtkTreePath *path = gtk_tree_model_get_path(modelHandle,
			&_W_TREEITEM(item)->iter);
	gtk_widget_realize((GtkWidget*) parentHandle);

	gboolean isExpander = gtk_tree_model_iter_n_children(modelHandle,
			&_W_TREEITEM(item)->iter) > 0;
	gboolean isExpanded = gtk_tree_view_row_expanded(parentHandle, path);
	gtk_tree_view_column_cell_set_cell_data(column, modelHandle,
			&_W_TREEITEM(item)->iter, isExpander, isExpanded);

	GdkRectangle rect;
	gtk_tree_view_get_cell_area(parentHandle, path, column, &rect);
	if ((_W_WIDGET(tree)->style & W_MIRRORED) != 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(tree);
		int client_width = priv->get_client_width(W_CONTROL(tree), priv);
		rect.x = client_width - rect.width - rect.x;
	}
	int right = rect.x + rect.width;

	int x = 0, w = 0;
	GtkRequisition size;
	_W_LISTVIEWBASE(tree)->ignoreSize = W_TRUE;
	gtk_cell_renderer_get_preferred_size(textRenderer,
			(GtkWidget*) parentHandle, &size, NULL);
	_W_LISTVIEWBASE(tree)->ignoreSize = W_FALSE;
	w = size.width;
	rect.width = w;
	int buffer = 0;
	gtk_tree_path_free(path);

	gtk_widget_style_get((GtkWidget*) parentHandle, "horizontal-separator",
			&buffer, NULL);
	int horizontalSeparator = buffer;
	rect.x += horizontalSeparator;

	gtk_tree_view_column_cell_get_position(column, textRenderer, &x, NULL);
	rect.x += x;
	guint columnCount = gtk_tree_view_get_n_columns(parentHandle);
	if (columnCount > 0) {
		if (rect.x + rect.width > right) {
			rect.width = WMAX(0, right - rect.x);
		}
	}
	int width = gtk_tree_view_column_get_visible(column) ? rect.width + 1 : 0;
	bounds->x = rect.x;
	bounds->y = rect.y;
	bounds->width = rect.width;
	bounds->height = rect.height + 1;
	if (gtk_tree_view_get_headers_visible(
			parentHandle) && GTK_VERSION > VERSION(3, 9, 0)) {
		bounds->y += _w_listviewbase_get_header_height(W_LISTVIEWBASE(tree));
	}
	return W_TRUE;
}
wresult _w_listitem_get_bounds_index(w_listitem *item, int index,
		w_rect *bounds) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkTreeView *parentHandle = GTK_TREE_VIEW(_W_WIDGET(tree)->handle);
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(parentHandle);
	GtkTreeViewColumn *column = gtk_tree_view_get_column(parentHandle, index);
	if (column == 0) {
		memset(bounds, 0, sizeof(w_rect));
		return W_TRUE;
	}
	GtkTreePath *path = gtk_tree_model_get_path(modelHandle,
			&_W_TREEITEM(item)->iter);
	gtk_widget_realize(GTK_WIDGET(parentHandle));
	GdkRectangle rect;
	gtk_tree_view_get_cell_area(parentHandle, path, column, &rect);
	if ((_W_WIDGET(tree)->style & W_MIRRORED) != 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(tree);
		int client_width = priv->get_client_width(W_CONTROL(tree), priv);
		rect.x = client_width - rect.width - rect.x;
	}
	gtk_tree_path_free(path);

	if (index == 0 && (_W_WIDGET(tree)->style & W_CHECK) != 0) {
		GtkCellRenderer *checkRenderer = _W_LISTVIEWBASE(tree)->checkrenderer;
		gint x = 0, w = 0;
		gtk_tree_view_column_cell_get_position(column, checkRenderer, &x, &w);
		rect.x += x + w;
		rect.width -= x + w;
	}
	int width = gtk_tree_view_column_get_visible(column) ? rect.width + 1 : 0;
	bounds->x = rect.x;
	bounds->y = rect.y;
	bounds->width = rect.width;
	bounds->height = rect.height + 1;
	if (gtk_tree_view_get_headers_visible(
			parentHandle) && GTK_VERSION > VERSION(3, 9, 0)) {
		bounds->y += _w_listviewbase_get_header_height(W_LISTVIEWBASE(tree));
	}
	return W_TRUE;
}
wresult _w_listitem_get_checked(w_listitem *item) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	int info = 0;
	int ret;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_INFO,
			&info, -1);
	if (info & COLUMN_INFO_CHECK) {
		ret = W_TRUE;
	} else {
		ret = W_FALSE;
	}
	return ret;
}
wresult _w_listitem_get_grayed(w_listitem *item) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	int info = 0;
	int ret;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_INFO,
			&info, -1);
	if (info & COLUMN_INFO_GRAYED) {
		ret = W_TRUE;
	} else {
		ret = W_FALSE;
	}
	return ret;
}
wresult _w_listitem_get_image(w_listitem *item) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	int image = -1;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_IMAGE,
			&image, -1);
	return image;
}
wresult _w_listitem_set_checked(w_listitem *item, int checked) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	int info = 0;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_INFO,
			&info, -1);
	if (checked) {
		info |= COLUMN_INFO_CHECK;
	} else {
		info &= ~COLUMN_INFO_CHECK;
	}
	if (tree->clazz->class_id == _W_CLASS_TREEVIEW) {
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_INFO, info, -1);
	} else {
		gtk_list_store_set(GTK_LIST_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_INFO, info, -1);
	}
	return W_TRUE;
}
wresult _w_listitem_set_grayed(w_listitem *item, int grayed) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	int info = 0;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_INFO,
			&info, -1);
	if (grayed) {
		info |= COLUMN_INFO_GRAYED;
	} else {
		info &= ~COLUMN_INFO_GRAYED;
	}
	if (tree->clazz->class_id == _W_CLASS_TREEVIEW) {
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_INFO, info, -1);
	} else {
		gtk_list_store_set(GTK_LIST_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_INFO, info, -1);
	}
	return W_TRUE;
}
wresult _w_listitem_set_image(w_listitem *item, int image) {
	w_widget *tree = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	if (tree->clazz->class_id == _W_CLASS_TREEVIEW) {
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_IMAGE, image, -1);
	} else {
		gtk_list_store_set(GTK_LIST_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_IMAGE, image, -1);
	}
	return W_TRUE;
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
	GtkWidget *_handle = _W_WIDGET(list)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(_handle));
	GtkTreeIter *parentIter = 0, *handle, tmp;
	if (item != 0) {
		handle = &_W_TREEITEM(item)->iter;
	} else {
		handle = &tmp;
	}
	/*
	 * Feature in GTK.  It is much faster to append to a tree store
	 * than to insert at the end using gtk_tree_store_insert().
	 */
	if (index < 0) {
		gtk_list_store_append(GTK_LIST_STORE(modelHandle), handle);
	} else {
		gtk_list_store_insert(GTK_LIST_STORE(modelHandle), handle, index);
	}
	gtk_list_store_set(GTK_LIST_STORE(modelHandle), handle, COLUMN_IMAGE, -1,
			-1);
	if (item != 0) {
		W_WIDGETDATA(item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(list);
		_W_ITEM(item)->parent = W_WIDGET(list);
		_W_ITEM(item)->index = -1;
	}
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
/*
 * signals
 */
void _w_listitem_class_init(struct _w_listitem_class *listitem) {
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
void _w_listview_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_listview_class *clazz) {
	if (classId == _W_CLASS_LISTVIEW) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&gtk_toolkit->class_listview_priv;
	}
	_w_listviewbase_class_init(toolkit, classId,W_LISTVIEWBASE_CLASS(clazz));
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
	W_WIDGETDATA_CLASS(listitem)->toolkit = toolkit;
	_w_listitem_class_init(listitem);
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_LISTVIEW) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		/*
		 * signals
		 */
		_gtk_signal_fn *signals = _W_WIDGET_PRIV(priv)->signals;
		signals[SIGNAL_TOGGLED] = _gtk_treeview_toggled;
	}
}
