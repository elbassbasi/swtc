/*
 * listviewbase.c
 *
 *  Created on: 5 févr. 2021
 *      Author: azeddine
 */
#include "treeview.h"
#include "../widgets/toolkit.h"
/*
 *
 */
wresult _w_columnitem_get_data(w_item *item, void **data) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(item)->column;
	*data = g_object_get_qdata(G_OBJECT(columnhandle), gtk_toolkit->quark[1]);
	return W_TRUE;
}
wresult _w_columnitem_get_index(w_item *item) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(item)->column;
	if (_W_ITEM(item)->index == -1) {
		w_widget *widget = _W_ITEM(item)->parent;
		GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(widget)->handle);
		int count = gtk_tree_view_get_n_columns(handle);
		for (int i = 0; i < count; i++) {
			GtkTreeViewColumn *c = gtk_tree_view_get_column(handle, i);
			if (c == columnhandle) {
				_W_ITEM(item)->index = i;
				break;
			}
		}
	}
	return _W_ITEM(item)->index;
}
wresult _w_columnitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(item)->column;
	GtkWidget *box = gtk_tree_view_column_get_widget(columnhandle);
	_w_widget_handles handles;
	_w_widget_get_handles(box, &handles);
	wresult result = W_FALSE;
	if (handles.label != 0) {
		const char *str = gtk_label_get_label(GTK_LABEL(handles.label));
		_gtk_alloc_set_text(alloc, user_data, str, -1, enc);
	}
	return result;
}
wresult _w_columnitem_set_data(w_item *item, void *data) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(item)->column;
	g_object_set_qdata(G_OBJECT(columnhandle), gtk_toolkit->quark[1], data);
	return W_TRUE;
}
wresult _w_columnitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(item)->column;
	GtkWidget *box = gtk_tree_view_column_get_widget(columnhandle);
	_w_widget_handles handles;
	_w_widget_get_handles(box, &handles);
	if (handles.label != 0) {
		int newlength, mnemonic;
		char *str = _gtk_text_fix(text, length, enc, &newlength, &mnemonic);
		gtk_label_set_text_with_mnemonic(GTK_LABEL(handles.label), str);
		_gtk_text_free(text, str, newlength);
	}
	return W_TRUE;
}
wresult _w_columnitem_get_alignment(w_columnitem *column) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	wresult result = W_LEFT;
	if (columnhandle != 0) {
		gfloat al = gtk_tree_view_column_get_alignment(columnhandle);
		if (al >= 1) {
			result = W_RIGHT;
		} else if (al >= 0.5) {
			result = W_CENTER;
		} else {
			result = W_LEFT;
		}
	}
	return result;
}
wresult _w_columnitem_get_image(w_columnitem *column) {
	wresult image = -1;
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	GtkWidget *box = gtk_tree_view_column_get_widget(columnhandle);
	_w_widget_handles handles;
	_w_widget_get_handles(box, &handles);
	if (handles.image != 0) {
		image = _W_IMAGE_WIDGET(handles.image)->index;
	}
	return image;
}
wresult _w_columnitem_get_moveable(w_columnitem *column) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	return gtk_tree_view_column_get_reorderable(columnhandle);
}
wresult _w_columnitem_get_resizable(w_columnitem *column) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	return gtk_tree_view_column_get_resizable(columnhandle);
}
wresult _w_columnitem_get_tooltip_text(w_columnitem *column, w_alloc alloc,
		void *user_data, int enc) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	return W_FALSE;
}
wresult _w_columnitem_get_width(w_columnitem *column) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	return W_FALSE;
}
wresult _w_columnitem_pack(w_columnitem *column) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	return W_FALSE;
}
wresult _w_columnitem_set_alignment(w_columnitem *column, int alignment) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	wresult result = W_FALSE;
	if (columnhandle != 0) {
		GtkCellRenderer *renderer = g_object_get_qdata(G_OBJECT(columnhandle),
				gtk_toolkit->quark[2]);
		float al;
		if (alignment & W_RIGHT) {
			al = 1;
		} else if (alignment & W_CENTER) {
			al = 0.5;
		} else {
			al = 0;
		}
		if (renderer != 0) {
			g_object_set(G_OBJECT(renderer), "xalign", al, NULL);
		}
		gtk_tree_view_column_set_alignment(columnhandle, al);
		result = W_TRUE;
	}
	return result;
}
wresult _w_columnitem_set_image(w_columnitem *column, int image) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	GtkWidget *box = gtk_tree_view_column_get_widget(columnhandle);
	_w_widget_handles handles;
	_w_widget_get_handles(box, &handles);
	if (handles.image != 0) {
		_W_IMAGE_WIDGET(handles.image)->index = image;
	}
	return W_TRUE;
}
wresult _w_columnitem_set_moveable(w_columnitem *column, int moveable) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	gtk_tree_view_column_set_reorderable(columnhandle, moveable);
	return W_TRUE;
}
wresult _w_columnitem_set_resizable(w_columnitem *column, int resizable) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	gtk_tree_view_column_set_resizable(columnhandle, resizable);
	return W_TRUE;
}
wresult _w_columnitem_set_tooltip_text(w_columnitem *column, const char *text,
		int length, int enc) {
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	return W_FALSE;
}
wresult _w_columnitem_set_width(w_columnitem *column, int width) {
	w_widget *tree = _W_ITEM(column)->parent;
	GtkWidget *handle = _W_WIDGET(tree)->handle;
	GtkTreeViewColumn *columnhandle = _W_COLUMNITEM(column)->column;
	if (width > 0) {
		gtk_tree_view_column_set_fixed_width(columnhandle, width);
	}
	/*
	 * Bug in GTK.  For some reason, calling gtk_tree_view_column_set_visible()
	 * when the parent is not realized fails to show the column. The fix is to
	 * ensure that the table has been realized.
	 */
	if (width != 0) {
		gtk_widget_realize(handle);
	}
	gtk_tree_view_column_set_visible(columnhandle, width != 0);
	/*
	 * Bug in GTK. When the column is made visible the event window of column
	 * header is raised above the gripper window of the previous column. In
	 * some cases, this can cause the previous column to be not resizable by
	 * the mouse. The fix is to find the event window and lower it to bottom to
	 * the z-order stack.
	 */
	if (width != 0) {
		GtkWidget *buttonHandle = gtk_tree_view_column_get_button(columnhandle);
		if (buttonHandle != 0) {
			GdkWindow *window = gtk_widget_get_parent_window(buttonHandle);
			if (window != 0) {
				GList *windowList = gdk_window_peek_children(window);
				if (windowList != 0) {
					GList *windows = windowList;
					void *userData = 0;
					while (windows != 0) {
						GdkWindow *child = (GdkWindow*) windows->data;
						gdk_window_get_user_data(child, &userData);
						if (userData == buttonHandle) {
							gdk_window_lower(child);
							break;
						}
						windows = windows->next;
					}
				}
			}
		}
	}
	w_event_list e;
	memset(&e, 0, sizeof(e));
	e.event.type = W_EVENT_ITEM_RESIZE;
	e.event.widget = tree;
	e.column = column;
	_w_widget_send_event(tree, (w_event*) &e, W_EVENT_SEND);
	return W_TRUE;
}
/*
 *
 */
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
GType gtk_store_types[COLUMN_COUNT] = {
G_TYPE_STRING, //
		G_TYPE_POINTER, //
		G_TYPE_INT, //
		G_TYPE_INT //
		};
wresult _w_listviewbase_create_handle(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	GtkWidget *scrolledHandle = 0, *handle = 0, *fixed;
	GtkListStore *modelHandle = 0;
	GtkSelectionMode mode;
	GtkTreeSelection *selectionHandle;
	GtkPolicyType hsp;
	GtkPolicyType vsp;
	fixed = (GtkWidget*) _w_fixed_new(widget);
	if (fixed == 0)
		goto _err;
	gtk_widget_set_has_window(fixed, TRUE);
	scrolledHandle = gtk_scrolled_window_new(0, 0);
	if (scrolledHandle == 0)
		goto _err;
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledHandle),
			GTK_SHADOW_IN);
	w_class_id clazz_id = W_WIDGET_GET_CLASS(widget)->class_id;
	if (clazz_id == _W_CLASS_TREEVIEW) {
		modelHandle = (GtkListStore*) gtk_tree_store_newv(COLUMN_COUNT,
				gtk_store_types);
	} else {
		modelHandle = gtk_list_store_newv(COLUMN_COUNT, gtk_store_types);
	}
	if (modelHandle == 0)
		goto _err;
	handle = gtk_tree_view_new_with_model(GTK_TREE_MODEL(modelHandle));
	if (handle == 0)
		goto _err;

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(handle), FALSE);

	_w_fixed_set_child(fixed, scrolledHandle);
	gtk_container_add(GTK_CONTAINER(scrolledHandle), handle);
	_w_widget_set_control(modelHandle, widget);
	wuint64 style = _W_WIDGET(widget)->style;
	mode = (style & W_MULTI) != 0 ?
			GTK_SELECTION_MULTIPLE : GTK_SELECTION_BROWSE;
	selectionHandle = gtk_tree_view_get_selection(GTK_TREE_VIEW(handle));
	gtk_tree_selection_set_mode(selectionHandle, mode);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(handle), FALSE);
	hsp = (style & W_HSCROLL) != 0 ? GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER;
	vsp = (style & W_VSCROLL) != 0 ? GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER;
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle), hsp,
			vsp);
	if ((style & W_BORDER) != 0)
		gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledHandle),
				GTK_SHADOW_ETCHED_IN);
	_w_widget_set_control(fixed, widget);
	_w_widget_set_control(scrolledHandle, widget);
	_w_widget_set_control(handle, widget);
	_w_widget_set_control(selectionHandle, widget);
	_W_WIDGET(widget)->handle = handle;
	_w_listviewbase_insert_column_0(W_LISTVIEWBASE(widget), 0, 0, priv);
	gtk_widget_show_all(GTK_WIDGET(fixed));
	return W_TRUE;
	_err: if (scrolledHandle)
		gtk_widget_destroy(scrolledHandle);
	if (modelHandle)
		g_object_unref(modelHandle);
	if (handle)
		gtk_widget_destroy(handle);
	return W_ERROR_NO_HANDLES;
}
void _w_tree_cell_data(GtkTreeViewColumn *tree_column, GtkCellRenderer *cell,
		GtkTreeModel *tree_model, GtkTreeIter *iter, gpointer data) {
	_w_cell_renderer *c = (_w_cell_renderer*) cell;
	/*
	 * tree column
	 */
	W_WIDGETDATA(&c->column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(data);
	_W_ITEM(&c->column)->parent = W_WIDGET(data);
	_W_ITEM(&c->column)->index = -1;
	_W_COLUMNITEM(&c->column)->column = tree_column;
	/*
	 * tree item
	 */
	W_WIDGETDATA(&c->treeitem)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(data);
	_W_ITEM(&c->treeitem)->parent = W_WIDGET(data);
	_W_ITEM(&c->treeitem)->index = -1;
	memcpy(&_W_TREEITEM(&c->treeitem)->iter, iter, sizeof(GtkTreeIter));
}
wresult _w_listviewbase_insert_column_0(w_listviewbase *list, int index,
		w_columnitem *column, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(list)->style;
	GtkTreeViewColumn *columnHandle = 0;
	GtkCellRenderer *textrenderer = 0, *pixbufrenderer = 0, *togglerenderer = 0;
	GtkWidget *boxHandle = 0, *labelHandle = 0;
	_w_image_widget *imageHandle = 0;
	columnHandle = gtk_tree_view_column_new();
	if (columnHandle == 0) {
		goto _err;
	}
	boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	if (boxHandle == 0) {
		goto _err;
	}
	labelHandle = gtk_label_new_with_mnemonic("");
	if (labelHandle == 0) {
		goto _err;
	}
	imageHandle = _w_image_widget_new();
	if (imageHandle == 0)
		goto _err;
	imageHandle->get_image_list = (__get_image_list) _w_listviewbase_get_header_imagelist;
	imageHandle->parent = W_WIDGET(list);
	imageHandle->index = -1;
	if (imageHandle == 0) {
		goto _err;
	}
	gtk_container_add(GTK_CONTAINER(boxHandle), (GtkWidget*) imageHandle);
	gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
	gtk_widget_show_all(boxHandle);
	gtk_tree_view_column_set_widget(columnHandle, boxHandle);
	//g_object_ref(renderer);

	gtk_tree_view_column_set_resizable(columnHandle, TRUE);
	gtk_tree_view_column_set_clickable(columnHandle, TRUE);
	gtk_tree_view_column_set_sort_column_id(columnHandle, 0);
	if (index == 0) {
		if ((style & W_CHECK) != 0) {
			togglerenderer = _w_toggle_renderer_new(W_WIDGET(list));
			if (togglerenderer == 0)
				goto _err;
			g_object_ref(togglerenderer);
			_W_LISTVIEWBASE(list)->checkrenderer = togglerenderer;
			gtk_tree_view_column_pack_start(columnHandle, togglerenderer,
			FALSE);
			gtk_tree_view_column_set_cell_data_func(columnHandle,
					togglerenderer, _w_tree_cell_data, list, 0);
			_w_widget_set_control(togglerenderer, W_WIDGET(list));
			if (togglerenderer != 0) {
				_w_widget_connect((GtkWidget*) togglerenderer,
						&_W_LISTVIEWBASE_PRIV(priv)->signals[_W_LISTVIEW_SIGNAL_TOGGLED],
						FALSE); //SIGNAL_TOGGLED
			}
		}
		pixbufrenderer = _w_pixbuf_renderer_new(W_WIDGET(list));
		if (pixbufrenderer == 0)
			goto _err;
		_W_LISTVIEWBASE(list)->pixbufrenderer = pixbufrenderer;
		gtk_tree_view_column_pack_start(columnHandle, pixbufrenderer, FALSE);
		gtk_tree_view_column_set_cell_data_func(columnHandle, pixbufrenderer,
				_w_tree_cell_data, list, 0);
		_w_widget_set_control(pixbufrenderer, W_WIDGET(list));
		gtk_cell_renderer_set_fixed_size(pixbufrenderer, 0, 0);
	}
	textrenderer = _w_text_renderer_new(W_WIDGET(list));
	if (textrenderer == 0) {
		goto _err;
	}
	gtk_tree_view_column_pack_start(columnHandle, textrenderer, FALSE);
	gtk_tree_view_column_set_cell_data_func(columnHandle, textrenderer,
			_w_tree_cell_data, list, 0);
	//gtk_tree_view_column_pack_end(columnHandle, renderer, FALSE);
	_w_widget_set_control(textrenderer, W_WIDGET(list));
	/* Set alignment */
	if (index == 0) {
		if ((style & W_RIGHT) != 0) {
			g_object_set(textrenderer, "xalign", 1.0, NULL);
			gtk_tree_view_column_pack_end(columnHandle, textrenderer, TRUE);
			gtk_tree_view_column_pack_end(columnHandle, pixbufrenderer, FALSE);
			gtk_tree_view_column_set_alignment(columnHandle, 1.0f);
		} else if ((style & W_CENTER) != 0) {
			g_object_set(textrenderer, "xalign", 0.5, NULL);
			gtk_tree_view_column_pack_start(columnHandle, pixbufrenderer,
			FALSE);
			gtk_tree_view_column_pack_end(columnHandle, textrenderer, TRUE);
			gtk_tree_view_column_set_alignment(columnHandle, 0.5);
		} else if ((style & W_LEFT) != 0) {
			gtk_tree_view_column_pack_start(columnHandle, pixbufrenderer,
			FALSE);
			gtk_tree_view_column_pack_start(columnHandle, textrenderer, TRUE);
			gtk_tree_view_column_set_alignment(columnHandle, 0.0);
		}
	}
	g_object_set_qdata(G_OBJECT(columnHandle), gtk_toolkit->quark[1],
			textrenderer);
	GtkWidget *_handle = _W_WIDGET(list)->handle;
	gtk_tree_view_insert_column(GTK_TREE_VIEW(_handle), columnHandle, index);
	_w_widget_set_control(columnHandle, (w_widget*) ((long) 0));
	if (column != 0) {
		W_WIDGETDATA(column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(list);
		_W_ITEM(column)->parent = W_WIDGET(list);
		_W_ITEM(column)->index = index;
		_W_COLUMNITEM(column)->column = columnHandle;
	}
	return W_TRUE;
	_err: if (columnHandle)
		g_object_unref(columnHandle);
	return W_ERROR_NO_HANDLES;
}
wresult _w_listview_clear_all_callback(w_widget *widget, void *item,
		void *user_data) {
	return W_FALSE;
}
wresult _w_treeview_clear_all_callback(w_widget *widget, void *item,
		void *user_data) {
	return W_FALSE;
}
wresult _w_listviewbase_clear_all(w_listviewbase *list) {
	w_class_id clazz_id = W_WIDGET_GET_CLASS(list)->class_id;
	w_widget_callback callback;
	if (clazz_id == _W_CLASS_TREEVIEW) {
		callback = _w_treeview_clear_all_callback;
	} else {
		callback = _w_listview_clear_all_callback;
	}
	return w_listviewbase_for_all_item(list, callback, 0, 0);
}
wresult _w_listviewbase_deselect_all(w_listviewbase *list) {
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(list)->handle);
	GtkTreeSelection *selection = gtk_tree_view_get_selection(handle);
	_w_listviewbase_priv *lpriv = (_w_listviewbase_priv*) _W_CONTROL_GET_PRIV(
			list);
	_gtk_signal *changed = &lpriv->signals[_W_LISTVIEW_SIGNAL_CHANGED];
	g_signal_handlers_block_matched(selection, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			changed);
	gtk_tree_selection_unselect_all(selection);
	g_signal_handlers_unblock_matched(selection, G_SIGNAL_MATCH_DATA, 0, 0, 0,
			0, changed);
	return W_TRUE;
}
wresult _w_listviewbase_for_all_column(w_listviewbase *list,
		w_widget_callback callback, void *user_data) {
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(list)->handle);
	w_columnitem column;
	W_WIDGETDATA(&column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(list);
	_W_ITEM(&column)->parent = W_WIDGET(list);
	int n = gtk_tree_view_get_n_columns(handle);
	for (int i = 0; i < n; i++) {
		GtkTreeViewColumn *columnHandle = gtk_tree_view_get_column(handle, i);
		_W_ITEM(&column)->index = i;
		_W_COLUMNITEM(&column)->column = columnHandle;
		callback(W_WIDGET(list), &column, user_data);
	}
	return W_TRUE;
}
wresult _w_listviewbase_get_column(w_listviewbase *list, int index,
		w_columnitem *column) {
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(list)->handle);
	GtkTreeViewColumn *columnHandle = gtk_tree_view_get_column(handle, index);
	if (columnHandle != 0) {
		W_WIDGETDATA(column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(list);
		_W_ITEM(column)->parent = W_WIDGET(list);
		_W_ITEM(column)->index = index;
		_W_COLUMNITEM(column)->column = columnHandle;
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_listviewbase_get_column_count(w_listviewbase *list) {
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(list)->handle);
	return gtk_tree_view_get_n_columns(handle);
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
wresult _w_listviewbase_get_header_imagelist(w_listviewbase *list,
		w_imagelist **imagelist) {
	*imagelist = _W_LISTVIEWBASE(list)->headerimagelist;
	return W_TRUE;
}
wresult _w_listviewbase_get_header_visible(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_imagelist(w_listviewbase *list,
		w_imagelist **imagelist) {
	*imagelist = _W_LISTVIEWBASE(list)->imagelist;
	return W_TRUE;
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
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(list)->handle);
	GtkTreeSelection *selection = gtk_tree_view_get_selection(handle);
	return gtk_tree_selection_count_selected_rows(selection);
}
wresult _w_listviewbase_get_sort_column(w_listviewbase *list,
		w_columnitem *column) {
	return W_FALSE;
}
wresult _w_listviewbase_get_sort_direction(w_listviewbase *list) {
	return W_FALSE;
}
void _w_listviewbase_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_composite_hook_events(widget, priv);
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	GtkTreeSelection *selection = gtk_tree_view_get_selection(
			GTK_TREE_VIEW(handle));
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	_w_listviewbase_priv *tpriv = (_w_listviewbase_priv*) priv;
	int class_id = w_widget_class_id(widget);
	_w_widget_connect((GtkWidget*) selection,
			&tpriv->signals[_W_LISTVIEW_SIGNAL_CHANGED], FALSE);
	_w_widget_connect((GtkWidget*) modelHandle,
			&tpriv->signals[_W_LISTVIEW_SIGNAL_ROW_HAS_CHILD_TOGGLED], FALSE);
	//if (fixAccessibility()) {
	_w_widget_connect((GtkWidget*) modelHandle,
			&tpriv->signals[_W_LISTVIEW_SIGNAL_ROW_INSERTED], FALSE);
	_w_widget_connect((GtkWidget*) modelHandle,
			&tpriv->signals[_W_LISTVIEW_SIGNAL_ROW_DELETED], FALSE);
	//}
	_w_widget_connect(handle, &tpriv->signals[_W_LISTVIEW_SIGNAL_ROW_ACTIVATED],
	FALSE);
	_w_widget_connect(handle,
			&tpriv->signals[_W_LISTVIEW_SIGNAL_START_INTERACTIVE_SEARCH],
			FALSE);
	if (class_id == _W_CLASS_TREEVIEW) {
		_w_widget_connect(handle,
				&tpriv->signals[_W_LISTVIEW_SIGNAL_TEST_EXPAND_ROW], FALSE);
		_w_widget_connect(handle,
				&tpriv->signals[_W_LISTVIEW_SIGNAL_TEST_COLLAPSE_ROW], FALSE);
		_w_widget_connect(handle,
				&tpriv->signals[_W_LISTVIEW_SIGNAL_EXPAND_COLLAPSE_CURSOR_ROW],
				FALSE);
	}
}
wresult _w_listviewbase_insert_column(w_listviewbase *list,
		w_columnitem *column, int index) {
	if (index == 0)
		return W_FALSE;
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(list);
	return _w_listviewbase_insert_column_0(list, index, column, priv);
}
wresult _w_listviewbase_remove_all(w_listviewbase *list) {
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(list)->handle);
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(handle);
	int length = gtk_tree_model_iter_n_children(modelHandle, 0);
	if (length == 0)
		return W_TRUE;
	GtkTreeIter iter, parentIter, tmp;
	gboolean valid = gtk_tree_model_iter_children(modelHandle, &iter, 0);
	if (w_widget_class_id(W_WIDGET(list)) == _W_CLASS_TREEVIEW) {
		GtkTreeStore *store = GTK_TREE_STORE(modelHandle);
		while (valid) {
			memcpy(&tmp, &iter, sizeof(GtkTreeIter));
			valid = gtk_tree_model_iter_next(modelHandle, &iter);
			gtk_tree_store_remove(store, &tmp);
		}
	} else {
		GtkListStore *store = GTK_LIST_STORE(modelHandle);
		while (valid) {
			memcpy(&tmp, &iter, sizeof(GtkTreeIter));
			valid = gtk_tree_model_iter_next(modelHandle, &iter);
			gtk_list_store_remove(store, &tmp);
		}
	}
	return W_TRUE;
}
wresult _w_listviewbase_select_all(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_priv *priv) {
	wresult result = _w_composite_set_bounds_0(control, location, size, priv);
	/*
	 * Bug on GTK.  The tree view sometimes does not get a paint
	 * event or resizes to a one pixel square when resized in a new
	 * shell that is not visible after any event loop has been run.  The
	 * problem is intermittent. It doesn't seem to happen the first time
	 * a new shell is created. The fix is to ensure the tree view is realized
	 * after it has been resized.
	 */
	GtkWidget *handle = _W_WIDGET(control)->handle;
	gtk_widget_realize(handle);
	return result;
}
wresult _w_listviewbase_set_header_imagelist(w_listviewbase *list,
		w_imagelist *imagelist) {
	if (imagelist == 0) {
		_W_LISTVIEWBASE(list)->headerimagelist = 0;
	} else {
		if (_W_IMAGELIST(imagelist)->images == 0) {
			_W_LISTVIEWBASE(list)->headerimagelist = 0;
			return W_ERROR_INVALID_ARGUMENT;
		} else {
			_W_LISTVIEWBASE(list)->headerimagelist = imagelist;
		}
	}
	W_CONTROL_GET_CLASS(list)->update(W_CONTROL(list));
	return W_TRUE;
}
wresult _w_listviewbase_set_header_visible(w_listviewbase *list, int show) {
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(list)->handle);
	gtk_tree_view_set_headers_visible(handle, show);
	return W_TRUE;
}
wresult _w_listviewbase_set_imagelist(w_listviewbase *list,
		w_imagelist *imagelist) {
	if (imagelist == 0) {
		_W_LISTVIEWBASE(list)->imagelist = 0;
		gtk_cell_renderer_set_fixed_size(_W_LISTVIEWBASE(list)->pixbufrenderer,
				0, 0);
	} else {
		if (_W_IMAGELIST(imagelist)->images == 0) {
			_W_LISTVIEWBASE(list)->imagelist = 0;
			gtk_cell_renderer_set_fixed_size(
			_W_LISTVIEWBASE(list)->pixbufrenderer, 0, 0);
			return W_ERROR_INVALID_ARGUMENT;
		} else {
			_W_LISTVIEWBASE(list)->imagelist = imagelist;
			gtk_cell_renderer_set_fixed_size(
			_W_LISTVIEWBASE(list)->pixbufrenderer,
			_W_IMAGELIST(imagelist)->images->width,
			_W_IMAGELIST(imagelist)->images->height);
		}
	}
	W_CONTROL_GET_CLASS(list)->update(W_CONTROL(list));
	return W_TRUE;
}
wresult _w_listviewbase_set_item_height(w_listviewbase *list, int itemHeight) {
	return W_FALSE;
}
wresult _w_listviewbase_set_lines_visible(w_listviewbase *list, int show) {
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(list)->handle);
	gtk_tree_view_set_grid_lines(handle,
			show ? GTK_TREE_VIEW_GRID_LINES_BOTH : GTK_TREE_VIEW_GRID_LINES_NONE);
	return W_TRUE;
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
_gtk_signal_info _gtk_listview_signal_lookup[_W_LISTVIEW_LAST] = { //
				{ SIGNAL_TOGGLED, 3, "toggled" }, //
				{ SIGNAL_CHANGED, 2, "changed" }, //
				{ SIGNAL_ROW_HAS_CHILD_TOGGLED, 4, "row-has-child-toggled" }, //
				{ SIGNAL_ROW_INSERTED, 4, "row-inserted" }, //
				{ SIGNAL_ROW_DELETED, 3, "row-deleted" }, //
				{ SIGNAL_ROW_ACTIVATED, 4, "row-activated" }, //
				{ SIGNAL_START_INTERACTIVE_SEARCH, 2, "start-interactive-search" }, //
				{ SIGNAL_TEST_EXPAND_ROW, 4, "test-expand-row" }, //
				{ SIGNAL_TEST_COLLAPSE_ROW, 4, "test-collapse-row" }, //
				{ SIGNAL_EXPAND_COLLAPSE_CURSOR_ROW, 5,
						"expand-collapse-cursor-row" }, //
		};
void _w_listviewbase_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_listviewbase_class *clazz) {
	_w_composite_class_init(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	clazz->clear_all = _w_listviewbase_clear_all;
	clazz->deselect_all = _w_listviewbase_deselect_all;
	clazz->for_all_column = _w_listviewbase_for_all_column;
	clazz->get_column = _w_listviewbase_get_column;
	clazz->get_column_count = _w_listviewbase_get_column_count;
	clazz->get_columns = _w_listviewbase_get_columns;
	clazz->get_gridline_width = _w_listviewbase_get_gridline_width;
	clazz->get_header_height = _w_listviewbase_get_header_height;
	clazz->get_header_imagelist = _w_listviewbase_get_header_imagelist;
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
	clazz->set_header_imagelist = _w_listviewbase_set_header_imagelist;
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
	 * tree item
	 */
	struct _w_columnitem_class *columnitem = W_COLUMNITEM_CLASS(
			clazz->class_column);
	_w_item_class_init(W_ITEM_CLASS(columnitem));
	W_WIDGETDATA_CLASS(columnitem)->toolkit = toolkit;
	W_ITEM_CLASS(columnitem)->get_data = _w_columnitem_get_data;
	W_ITEM_CLASS(columnitem)->get_text = _w_columnitem_get_text;
	W_ITEM_CLASS(columnitem)->set_data = _w_columnitem_set_data;
	W_ITEM_CLASS(columnitem)->set_text = _w_columnitem_set_text;
	W_ITEM_CLASS(columnitem)->get_index = _w_columnitem_get_index;
	columnitem->get_alignment = _w_columnitem_get_alignment;
	columnitem->get_image = _w_columnitem_get_image;
	columnitem->get_moveable = _w_columnitem_get_moveable;
	columnitem->get_resizable = _w_columnitem_get_resizable;
	columnitem->get_tooltip_text = _w_columnitem_get_tooltip_text;
	columnitem->get_width = _w_columnitem_get_width;
	columnitem->pack = _w_columnitem_pack;
	columnitem->set_alignment = _w_columnitem_set_alignment;
	columnitem->set_image = _w_columnitem_set_image;
	columnitem->set_moveable = _w_columnitem_set_moveable;
	columnitem->set_resizable = _w_columnitem_set_resizable;
	columnitem->set_tooltip_text = _w_columnitem_set_tooltip_text;
	columnitem->set_width = _w_columnitem_set_width;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		_W_WIDGET_PRIV(priv)->create_handle = _w_listviewbase_create_handle;
		_W_WIDGET_PRIV(priv)->check_style = _w_listviewbase_check_style;
		_W_WIDGET_PRIV(priv)->handle_top = _w_widget_hpp;
		_W_WIDGET_PRIV(priv)->hook_events = _w_listviewbase_hook_events;
		priv->handle_fixed = _w_widget_hpp;
		priv->set_bounds_0 = _w_listviewbase_set_bounds_0;
		_W_SCROLLABLE_PRIV(priv)->handle_scrolled = _w_widget_hp;
		_W_LISTVIEWBASE_PRIV(priv)->renderer_render =
				_w_listviewbase_renderer_render;
#if GTK3
		_W_LISTVIEWBASE_PRIV(priv)->renderer_get_preferred_width =
				_w_listviewbase_renderer_get_preferred_width;
#endif
		_w_widget_init_signal(_W_LISTVIEWBASE_PRIV(priv)->signals,
				_gtk_listview_signal_lookup, _W_LISTVIEW_LAST);
	}
}
/*
 * cell renderer
 */
void _w_cell_renderer_render(GtkCellRenderer *cell, cairo_t *cr,
		GtkWidget *widget, const GdkRectangle *background_area,
		const GdkRectangle *cell_area, GtkCellRendererState flags) {
	w_widget *table = ((_w_cell_renderer*) cell)->widget;
	if (table != 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(table);
		_W_LISTVIEWBASE_PRIV(priv)->renderer_render(table, priv, cell, cr,
				widget, background_area, cell_area, flags);
	}
}
#if GTK3
void _w_cell_renderer_get_preferred_width(GtkCellRenderer *cell,
		GtkWidget *widget, gint *minimum_size, gint *natural_size) {
	w_widget *table = ((_w_cell_renderer*) cell)->widget;
	if (table != 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(table);
		_W_LISTVIEWBASE_PRIV(priv)->renderer_get_preferred_width(table, priv,
				cell, widget, minimum_size, natural_size);
	}
}
#endif
void _w_cell_renderer_ClassInit(struct _w_cell_renderer_class *klass) {
	klass->default_render = klass->cellClass.render;
	klass->cellClass.render = _w_cell_renderer_render;
#if GTK3
	klass->default_get_preferred_width = klass->cellClass.get_preferred_width;
	klass->cellClass.get_preferred_width = _w_cell_renderer_get_preferred_width;
#endif
}
void _w_cell_renderer_init(_w_cell_renderer *cell) {
	W_WIDGETDATA(&cell->column)->clazz = 0;
	W_WIDGETDATA(&cell->treeitem)->clazz = 0;
}
GType _w_registre_renderer_class(const char *name, GType parent) {
	GTypeInfo renderer_info = { sizeof(_w_cell_renderer_class),
	NULL, /* base_init */
	NULL, /* base_finalize */
	(GClassInitFunc) _w_cell_renderer_ClassInit,
	NULL, /* class_finalize */
	NULL, /* class_data */
	sizeof(_w_cell_renderer), 0, /* n_preallocs */
	(GInstanceInitFunc) _w_cell_renderer_init, NULL };
	return g_type_register_static(parent, name, &renderer_info, (GTypeFlags) 0);
}
GType _w_text_renderer_type = 0;
GtkCellRenderer* _w_text_renderer_new(w_widget *widget) {
	if (_w_text_renderer_type == 0) {
		_w_text_renderer_type = _w_registre_renderer_class("swt_text_renderer",
		GTK_TYPE_CELL_RENDERER_TEXT);
	}
	struct _w_cell_renderer *renderer = (_w_cell_renderer*) g_object_new(
			_w_text_renderer_type, NULL);
	if (renderer != 0) {
		renderer->widget = widget;
	}
	return (GtkCellRenderer*) renderer;
}
GType _w_pixbuf_renderer_type = 0;
GtkCellRenderer* _w_pixbuf_renderer_new(w_widget *widget) {
	if (_w_text_renderer_type == 0) {
		_w_pixbuf_renderer_type = _w_registre_renderer_class(
				"swt_pixbuf_renderer",
				GTK_TYPE_CELL_RENDERER_PIXBUF);
	}
	_w_cell_renderer *renderer = (_w_cell_renderer*) g_object_new(
			_w_pixbuf_renderer_type, NULL);
	if (renderer != 0) {
		renderer->widget = widget;
	}
	return (GtkCellRenderer*) renderer;
}
GType _w_toggle_renderer_type = 0;
GtkCellRenderer* _w_toggle_renderer_new(w_widget *widget) {
	if (_w_toggle_renderer_type == 0) {
		_w_toggle_renderer_type = _w_registre_renderer_class(
				"swt_toggle_renderer",
				GTK_TYPE_CELL_RENDERER_TOGGLE);
	}
	_w_cell_renderer *renderer = (_w_cell_renderer*) g_object_new(
			_w_toggle_renderer_type, NULL);
	if (renderer != 0) {
		renderer->widget = widget;
	}
	return (GtkCellRenderer*) renderer;
}
void _w_listviewbase_renderer_render(w_widget *widget, _w_control_priv *priv,
		GtkCellRenderer *_cell, cairo_t *cr, GtkWidget *gtkwidget,
		const GdkRectangle *background_area, const GdkRectangle *cell_area,
		GtkCellRendererState flags) {
	_w_listviewbase *_list = _W_LISTVIEWBASE(widget);
	_w_cell_renderer *cell = (_w_cell_renderer*) _cell;
	_w_cell_renderer_class *clazz =
			(_w_cell_renderer_class*) GTK_CELL_RENDERER_GET_CLASS(_cell);
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(widget)->handle);
	_w_columnitem *column = _W_COLUMNITEM(&cell->column);
	_w_treeitem *item = _W_TREEITEM(&cell->listitem);
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(handle);
	GtkTreeViewColumn *columnHandle = column->column;
	w_event_list event;
	w_rect rect;
	_w_graphics gc;
	w_item_attr attr;
	char *text;
	int wasSelected = FALSE;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_ITEM_GET_TEXT;
	event.event.widget = widget;
	event.column = W_COLUMNITEM(column);
	event.item = W_ITEM(item);
	event.rect = 0;
	event.gc = 0;
	event.textattr = &attr;
	memset(&attr, 0, sizeof(attr));
	int ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		memset(&attr, 0, sizeof(attr));
	}
	if (W_WIDGETDATA(item)->clazz != 0) {
		if (GTK_IS_CELL_RENDERER_TOGGLE(cell)
				|| ((GTK_IS_CELL_RENDERER_PIXBUF(cell)
						|| GTK_VERSION > VERSION(3, 13, 0))
						&& (columnHandle != 0
								|| (_W_WIDGET(widget)->style & W_CHECK) == 0))) {
			_list->drawFlags = (int) flags;
			_list->drawState = W_FOREGROUND;
			if (attr.background != 0) {
				_list->drawState |= W_BACKGROUND;
			}
			if ((flags & GTK_CELL_RENDERER_SELECTED) != 0)
				_list->drawState |= W_SELECTED;
#if GTK3
			if ((flags & GTK_CELL_RENDERER_SELECTED) == 0) {
				if ((flags & GTK_CELL_RENDERER_FOCUSED) != 0)
					_list->drawState |= W_FOCUSED;
			}
#endif
			rect.x = cell_area->x;
			rect.y = cell_area->y;
			rect.width = cell_area->width;
			rect.height = cell_area->height;
			GtkTreePath *path = gtk_tree_model_get_path(modelHandle,
					&item->iter);
			gtk_tree_view_get_background_area(handle, path, columnHandle,
					(GdkRectangle*) &rect);
			gtk_tree_path_free(path);
			// Use the x and width information from the Cairo context. See bug 535124.
#if GTK3
			if (cr
					!= 0&& GTK_VERSION > VERSION(3, 9, 0) && GTK_VERSION <= VERSION(3, 14, 8)) {
				GdkRectangle r2;
				gdk_cairo_get_clip_rectangle(cr, &r2);
				rect.x = r2.x;
				rect.width = r2.width;
			}
#endif
			if ((_list->drawState & W_SELECTED) == 0) {
				if ((_W_WIDGET(widget)->state & STATE_PARENT_BACKGROUND) != 0
						|| _W_CONTROL(widget)->backgroundImage.pixbuf != 0) {
					w_control *control = priv->find_background_control(
							W_CONTROL(widget), priv);
					if (control != 0) {
						if (cr != 0) {
							cairo_save(cr);
#if GTK2
								cairo_reset_clip(cr);
#endif
						}
						priv->draw_background(W_CONTROL(widget), control, cr, 0,
								&rect, priv);
						if (cr != 0) {
							cairo_restore(cr);
						}
					}
				}
			}

			//if (hooks (SWT.EraseItem)) {
			/*
			 * Cache the selection state so that it is not lost if a
			 * PaintListener wants to draw custom selection foregrounds.
			 * See bug 528155.
			 */
			wasSelected = (_list->drawState & W_SELECTED) != 0;
			if (wasSelected) {
			}
			_w_graphics_init(W_GRAPHICS(&gc), cr);
			if ((_list->drawState & W_SELECTED) != 0) {
				w_toolkit *toolkit = w_widget_get_toolkit(widget);
				w_color color = w_toolkit_get_system_color(toolkit,
						W_COLOR_LIST_SELECTION);
				w_graphics_set_background(W_GRAPHICS(&gc), color);
				color = w_toolkit_get_system_color(toolkit,
						W_COLOR_LIST_SELECTION_TEXT);
				w_graphics_set_foreground(W_GRAPHICS(&gc), color);
			} else {
				if (attr.background != 0)
					w_graphics_set_background(W_GRAPHICS(&gc), attr.background);
				if (attr.foreground != 0)
					w_graphics_set_foreground(W_GRAPHICS(&gc), attr.foreground);
			}
			if (attr.font != 0) {
				w_graphics_set_font(W_GRAPHICS(&gc), attr.font);
			} else {
				w_font *font;
				w_control_get_font(W_CONTROL(widget), &font);
				w_graphics_set_font(W_GRAPHICS(&gc), font);
			}
			if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
				int clientWidth = priv->get_client_width(W_CONTROL(widget),
						priv);
				rect.x = clientWidth - rect.width - rect.x;
			}
#if GTK3
			//w_graphics_set_clipping_rect(W_GRAPHICS(&gc), &rect);
#endif
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_ERASE;
			event.event.widget = widget;
			event.column = W_COLUMNITEM(column);
			event.item = W_ITEM(item);
			event.rect = &rect;
			event.gc = W_GRAPHICS(&gc);
			event.detail = _list->drawState;
			int ret = w_widget_post_event(widget, (w_event*) &event,
					W_EVENT_SEND);
			if (ret) {
#if GTK3
				//drawForegroundRGBA = null;
#endif
				_list->drawState = event.doit ? event.detail : 0;
				_list->drawFlags &= ~(GTK_CELL_RENDERER_FOCUSED
						| GTK_CELL_RENDERER_SELECTED);
				if ((_list->drawState & W_SELECTED) != 0)
					_list->drawFlags |= GTK_CELL_RENDERER_SELECTED;
				if ((_list->drawState & W_FOCUSED) != 0)
					_list->drawFlags |= GTK_CELL_RENDERER_FOCUSED;
				if ((_list->drawState & W_SELECTED) != 0) {
				} else {
					if (wasSelected) {
#if GTK3
						//drawForegroundRGBA = gc.getForeground().handleRGBA;
#endif
					}
				}
			}
			w_graphics_dispose(W_GRAPHICS(&gc));
			//}//if (hooks (SWT.EraseItem))
		}
	}
	if ((_list->drawState & W_BACKGROUND) != 0
			&& (_list->drawState & W_SELECTED) == 0) {
		_w_graphics_init(W_GRAPHICS(&gc), cr);
		w_graphics_set_background(W_GRAPHICS(&gc), attr.background);
		w_graphics_fill_rectangle(W_GRAPHICS(&gc), (w_rect*) background_area);
		w_graphics_dispose(W_GRAPHICS(&gc));
	}
	if ((_list->drawState & W_FOREGROUND)
			!= 0|| GTK_IS_CELL_RENDERER_TOGGLE (cell)) {
#if GTK3
		if (/*drawForegroundRGBA != 0 &&*/GTK_IS_CELL_RENDERER_TEXT(cell)) {
			//g_object_set (cell, "foreground-rgba", drawForegroundRGBA, 0);
		}
#endif
		if (GTK_IS_CELL_RENDERER_TEXT(cell)) {
			text = 0;
			GdkRGBA rgba;
			if (attr.background != 0) {
				g_object_set(cell, "background-set", TRUE, NULL);
				rgba.alpha = 1;
				rgba.red = W_RED(attr.background);
				rgba.green = W_GREEN(attr.background);
				rgba.blue = W_BLUE(attr.background);
				g_object_set(cell, "background-rgba", &rgba, NULL);
			} else {
				g_object_set(cell, "background-set", FALSE, NULL);
			}
			if (attr.foreground != 0) {
				g_object_set(cell, "foreground-set", TRUE, NULL);
				rgba.alpha = 1;
				rgba.red = W_RED(attr.foreground);
				rgba.green = W_GREEN(attr.foreground);
				rgba.blue = W_BLUE(attr.foreground);
				g_object_set(cell, "foreground-rgba", &rgba, NULL);
			} else {
				g_object_set(cell, "foreground-set", FALSE, NULL);
			}
			const PangoFontDescription *pangoHandle = 0;
			if (attr.font != 0) {
				pangoHandle = (PangoFontDescription*) attr.font;
			}
			g_object_set(cell, "font-desc", pangoHandle, NULL);
			GtkTreeViewColumn *firstColumn = gtk_tree_view_get_column(handle,
					0);
			if (columnHandle == firstColumn) {
				gtk_tree_model_get(modelHandle, &item->iter, COLUMN_TEXT, &text,
						-1);
			}
			if (text == 0) {
				memset(&event, 0, sizeof(event));
				event.event.type = W_EVENT_ITEM_GET_TEXT;
				event.event.widget = widget;
				event.column = W_COLUMNITEM(column);
				event.item = W_ITEM(item);
				event.rect = 0;
				event.gc = 0;
				event.textattr = &attr;
				int ret = _w_widget_send_event(widget, (w_event*) &event,
						W_EVENT_SEND);
				if (text == 0)
					text = "";
				g_object_set(cell, "text", text, NULL);
			} else {
				g_object_set(cell, "text", text, NULL);
			}
			clazz->default_render(_cell, cr, gtkwidget, background_area,
					cell_area, flags);
		}
		if (GTK_IS_CELL_RENDERER_TOGGLE(cell)) {
			int info = 0, b;
			gtk_tree_model_get(modelHandle, &item->iter, COLUMN_INFO, &info,
					-1);
			if (info & COLUMN_INFO_CHECK) {
				b = TRUE;
			} else {
				b = FALSE;
			}
			g_object_set(cell, "active", b, NULL);
			clazz->default_render(_cell, cr, gtkwidget, background_area,
					cell_area, flags);
			return;
		}
		if (GTK_IS_CELL_RENDERER_PIXBUF(cell)) {
			int index;
			GdkPixbuf *pixbuf = 0;
			gtk_tree_model_get(modelHandle, &item->iter, COLUMN_IMAGE, &index,
					-1);
			if (index >= 0) {
				w_imagelist *imagelist = _list->imagelist;
				if (imagelist != 0 && _W_IMAGELIST(imagelist)->images != 0
						&& _W_IMAGELIST(imagelist)->images->count > index) {
					pixbuf = _W_IMAGELIST(imagelist)->images->images[index];
				}
			}
			g_object_set(cell, "pixbuf", pixbuf, NULL);
			clazz->default_render(_cell, cr, gtkwidget, background_area,
					cell_area, flags);
			return;
		}
#if GTK3
		clazz->default_render(_cell, cr, gtkwidget, background_area, cell_area,
				flags);
#endif
	}
	if (W_WIDGETDATA(item)->clazz) {
		if (GTK_IS_CELL_RENDERER_TEXT(cell)) {
			//if (hooks (SWT.PaintItem)) {
			if (wasSelected)
				_list->drawState |= W_SELECTED;
			GdkRectangle r2; //cell_area
			rect.x = cell_area->x;
			rect.y = cell_area->y;
			rect.width = cell_area->width;
			rect.height = cell_area->height;
			GtkTreePath *path = gtk_tree_model_get_path(modelHandle,
					&item->iter);
			gtk_tree_view_get_background_area(handle, path, columnHandle,
					(GdkRectangle*) &rect);
			gtk_tree_path_free(path);
			// Use the x and width information from the Cairo context. See bug 535124 and 465309.
			if (GTK_VERSION > VERSION(3, 9, 0)
					&& GTK_VERSION <= VERSION(3, 14, 8)) {
				gdk_cairo_get_clip_rectangle(cr, &r2);
				rect.x = r2.x;
				rect.width = r2.width;
			}
			_list->ignoreSize = TRUE;
			int contentX, contentWidth;
			GtkRequisition req;
			gtk_cell_renderer_get_preferred_size(GTK_CELL_RENDERER(cell),
					(GtkWidget*) handle, &req, NULL);
			contentWidth = req.width;
			gtk_tree_view_column_cell_get_position(columnHandle,
					GTK_CELL_RENDERER(cell), &contentX, NULL);
			_list->ignoreSize = FALSE;
			// Account for the expander arrow offset in x position
			if (gtk_tree_view_get_expander_column(
					GTK_TREE_VIEW(_W_WIDGET(widget)->handle)) == columnHandle) {
				/* indent */
				GdkRectangle rect3;
				gtk_widget_realize(GTK_WIDGET(handle));
				path = gtk_tree_model_get_path(modelHandle, &item->iter);
				gtk_tree_view_get_cell_area(handle, path, columnHandle, &rect3);
				contentX += rect3.x;
			}
			_w_graphics_init(W_GRAPHICS(&gc), cr);
			if ((_list->drawState & W_SELECTED) != 0) {
				w_toolkit *toolkit = w_widget_get_toolkit(widget);
				w_color color = w_toolkit_get_system_color(toolkit,
						W_COLOR_LIST_SELECTION);
				w_graphics_set_background(W_GRAPHICS(&gc), color);
				color = w_toolkit_get_system_color(toolkit,
						W_COLOR_LIST_SELECTION_TEXT);
				w_graphics_set_foreground(W_GRAPHICS(&gc), color);
			} else {
				if (attr.background != 0)
					w_graphics_set_background(W_GRAPHICS(&gc), attr.background);
				if (attr.foreground != 0)
					w_graphics_set_foreground(W_GRAPHICS(&gc), attr.foreground);
			}
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_PAINT;
			event.event.widget = widget;
			event.column = W_COLUMNITEM(column);
			event.item = W_ITEM(item);
			event.gc = W_GRAPHICS(&gc);
			event.rect = &rect;
			if (attr.font != 0) {
				w_graphics_set_font(W_GRAPHICS(&gc), attr.font);
			} else {
				w_font *font;
				w_control_get_font(W_CONTROL(widget), &font);
				w_graphics_set_font(W_GRAPHICS(&gc), font);
			}
			if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
				int clientWidth = priv->get_client_width(W_CONTROL(widget),
						priv);
				rect.x = clientWidth - rect.width - rect.x;
			}
			// Caveat: rect2 is necessary because GC#setClipping(Rectangle) got broken by bug 446075
			//w_graphics_set_clipping_rect(W_GRAPHICS(&gc), &rect);
			rect.x += contentX;
			rect.width = cell_area->width;			//contentWidth;
			event.detail = _list->drawState;		//drawState;
			int ret = w_widget_post_event(widget, (w_event*) &event,
					W_EVENT_SEND);
			w_graphics_dispose(W_GRAPHICS(&gc));
			//}//if (hooks (PaintItem))
		}
	}
}
#if GTK3
void _w_listviewbase_renderer_get_preferred_width(w_widget *widget,
		_w_control_priv *priv, GtkCellRenderer *_cell, GtkWidget *gtkwidget,
		gint *minimum_size, gint *natural_size) {
	_w_listviewbase *_list = _W_LISTVIEWBASE(widget);
	struct _w_cell_renderer *cell = (struct _w_cell_renderer*) _cell;
	struct _w_cell_renderer_class *clazz =
			(struct _w_cell_renderer_class*) GTK_CELL_RENDERER_GET_CLASS(_cell);
	clazz->default_get_preferred_width(_cell, gtkwidget, minimum_size,
			natural_size);
	if (GTK_IS_CELL_RENDERER_TEXT(cell)) {
		gint contentWidth = *minimum_size, contentHeight = 0;
#if GTK3
		gtk_cell_renderer_get_preferred_height_for_width(_cell, gtkwidget,
				contentWidth, &contentHeight, NULL);
#endif
		int imageWidth = 0;
		if (_list->imagelist != 0
				&& _W_IMAGELIST(_list->imagelist)->images != 0) {
			imageWidth =
			_W_IMAGELIST(_list->imagelist)->images->width;
		}
		contentWidth += imageWidth;
		w_event_list event;
		w_rect rect;
		w_graphics gc;
		memset(&event, 0, sizeof(event));
		_w_graphics_init(&gc, 0);
		rect.x = 0;
		rect.y = 0;
		rect.width = contentWidth;
		rect.height = contentHeight;
		event.event.type = W_EVENT_ITEM_MEASURE;
		event.event.widget = widget;
		event.column = W_COLUMNITEM(&cell->column);
		event.item = W_ITEM(&cell->listitem);
		event.rect = &rect;
		event.gc = &gc;
		int ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
		if (contentHeight < rect.height)
			contentHeight = rect.height;
#if GTK3
		*minimum_size = contentWidth;
		gtk_cell_renderer_set_fixed_size(_cell, contentWidth, contentHeight);
#endif
		return;
	}
}
#endif
