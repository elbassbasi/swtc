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
GType gtk_store_types[COLUMN_COUNT] = {
G_TYPE_INT, //
		G_TYPE_INT, //
		G_TYPE_STRING, //
		G_TYPE_POINTER };
wresult _w_listviewbase_create_handle(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	GtkWidget *scrolledHandle = 0, *handle = 0, *fixed;
	GtkListStore *modelHandle = 0;
	GtkSelectionMode mode;
	GtkTreeSelection *selectionHandle;
	GtkPolicyType hsp;
	GtkPolicyType vsp;
	fixed = (GtkWidget*) _w_fixed_new();
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

	gtk_container_add(GTK_CONTAINER(fixed), scrolledHandle);
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
	_w_listviewbase_insert_column_0(W_LISTVIEWBASE(widget), 0, priv);
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
	_W_WIDGETDATA(&c->column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(data);
	_W_ITEM(&c->column)->parent = W_WIDGET(data);
	_W_ITEM(&c->column)->index = (intptr_t) _w_widget_find_control(tree_column);
	/*
	 * tree item
	 */
	_W_WIDGETDATA(&c->treeitem)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(data);
	_W_ITEM(&c->treeitem)->parent = W_WIDGET(data);
	_W_ITEM(&c->treeitem)->index = -1;
	memcpy(&_W_TREEITEM(&c->treeitem)->iter, iter, sizeof(GtkTreeIter));
}
wresult _w_listviewbase_insert_column_0(w_listviewbase *list, int index,
		_w_control_priv *priv) {
	wuint64 style = _W_WIDGET(list)->style;
	GtkTreeViewColumn *columnHandle = 0;
	GtkCellRenderer *textrenderer = 0, *pixbufrenderer = 0, *togglerenderer = 0;
	GtkWidget *boxHandle = 0, *labelHandle = 0, *imageHandle = 0;
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
	imageHandle = gtk_image_new();
	if (imageHandle == 0) {
		goto _err;
	}
	gtk_container_add(GTK_CONTAINER(boxHandle), imageHandle);
	gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
	gtk_widget_show_all(boxHandle);
	//gtk_widget_show (labelHandle);
	gtk_tree_view_column_set_widget(columnHandle, boxHandle);
	//g_object_ref(renderer);

	gtk_tree_view_column_set_resizable(columnHandle, TRUE);
	gtk_tree_view_column_set_clickable(columnHandle, TRUE);
	//gtk_tree_view_column_set_visible(columnHandle, FALSE);
	gtk_tree_view_column_set_sort_column_id(columnHandle, 0);
	if (index == 0) {
		if ((style & W_CHECK) != 0) {
			togglerenderer = _w_toggle_renderer_new(W_WIDGET(list));
			if (togglerenderer == 0)
				goto _err;
			g_object_ref(togglerenderer);
			gtk_tree_view_column_pack_start(columnHandle, togglerenderer,
			FALSE);
			gtk_tree_view_column_set_cell_data_func(columnHandle,
					togglerenderer, _w_tree_cell_data, list, 0);
			_w_widget_set_control(togglerenderer, W_WIDGET(list));
			if (togglerenderer != 0) {
				if (_W_LISTVIEWBASE_PRIV(priv)->signal_toggled_id == 0) {
					_W_LISTVIEWBASE_PRIV(priv)->signal_toggled_id =
							g_signal_lookup("toggled",
									gtk_cell_renderer_toggle_get_type());
				}
				_w_widget_connect((GtkWidget*) togglerenderer, SIGNAL_TOGGLED,
				_W_LISTVIEWBASE_PRIV(priv)->signal_toggled_id,
				FALSE);
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
	GtkWidget *_handle = _W_WIDGET(list)->handle;
	gtk_tree_view_insert_column(GTK_TREE_VIEW(_handle), columnHandle, 0);
	_w_widget_set_control(columnHandle, (w_widget*) ((long) 0));
	return W_TRUE;
	_err: if (columnHandle)
		g_object_unref(columnHandle);
	return W_ERROR_NO_HANDLES;
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
	_W_WIDGET_PRIV(priv)->create_handle = _w_listviewbase_create_handle;
	_W_WIDGET_PRIV(priv)->check_style = _w_listviewbase_check_style;
	_W_WIDGET_PRIV(priv)->handle_top = _w_widget_hpp;
	priv->handle_fixed = _w_widget_hpp;
	_W_SCROLLABLE_PRIV(priv)->handle_scrolled = _w_widget_hp;
	_W_LISTVIEWBASE_PRIV(priv)->renderer_render =
			_w_listviewbase_renderer_render;
#if GTK3
	_W_LISTVIEWBASE_PRIV(priv)->renderer_get_preferred_width =
			_w_listviewbase_renderer_get_preferred_width;
#else
	_W_LISTVIEWBASE_PRIV(priv)->renderer_get_preferred_width =_w_listviewbase_rendererGetSizeProc;
#endif
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
#else
void _w_cell_renderer_GetSize(GtkCellRenderer *cell, GtkWidget *widget,
		const GdkRectangle *cell_area, gint *x_offset, gint *y_offset,
		gint *width, gint *height) {
	w_widget *table = ((_w_cell_renderer*) cell)->widget;
	if (tree != 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(table);
		_W_LISTVIEWBASE_PRIV(priv)->rendererGetSizeProc(table, priv, cell, widget, cell_area, x_offset,
				y_offset, width, height);
	}
}
#endif
void _w_cell_renderer_ClassInit(struct _w_cell_renderer_class *klass) {
	klass->default_render = klass->cellClass.render;
	klass->cellClass.render = _w_cell_renderer_render;
#if GTK3
	klass->default_get_preferred_width = klass->cellClass.get_preferred_width;
	klass->cellClass.get_preferred_width = _w_cell_renderer_get_preferred_width;
#else
	klass->default_get_size = klass->cellClass.get_size;
	klass->cellClass.get_size =  _w_cell_renderer_GetSize;
#endif
}
void _w_cell_renderer_init(_w_cell_renderer *cell) {
	_W_WIDGETDATA(&cell->column)->clazz = 0;
	_W_WIDGETDATA(&cell->treeitem)->clazz = 0;
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
	_w_cell_renderer *cell = (_w_cell_renderer*) _cell;
	_w_cell_renderer_class *clazz =
			(_w_cell_renderer_class*) GTK_CELL_RENDERER_GET_CLASS(_cell);
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(widget)->handle));
	if (GTK_IS_CELL_RENDERER_TEXT(_cell)) {
		const char *text = 0;
		gtk_tree_model_get(modelHandle, &_W_TREEITEM(&cell->treeitem)->iter,
				COLUMN_TEXT, &text, -1);
		g_object_set(cell, "text", text, NULL);
	}
	if (GTK_IS_CELL_RENDERER_PIXBUF(_cell)) {
		int index;
		GdkPixbuf *pixbuf = 0;
		gtk_tree_model_get(modelHandle, &_W_TREEITEM(&cell->treeitem)->iter,
				COLUMN_IMAGE, &index, -1);
		if (index >= 0) {
			w_imagelist *imagelist = _W_LISTVIEWBASE(widget)->imagelist;
			if (imagelist != 0 && _W_IMAGELIST(imagelist)->images != 0
					&& _W_IMAGELIST(imagelist)->images->count > index) {
				pixbuf = _W_IMAGELIST(imagelist)->images->images[index];
			}
		}
		g_object_set(cell, "pixbuf", pixbuf, NULL);
	}
	clazz->default_render(_cell, cr, gtkwidget, background_area, cell_area,
			flags);
}
#if GTK3
void _w_listviewbase_renderer_get_preferred_width(w_widget *widget,
		_w_control_priv *priv, GtkCellRenderer *_cell, GtkWidget *gtkwidget,
		gint *minimum_size, gint *natural_size) {
	_w_cell_renderer *cell = (_w_cell_renderer*) _cell;
	_w_cell_renderer_class *clazz =
			(_w_cell_renderer_class*) GTK_CELL_RENDERER_GET_CLASS(_cell);
	clazz->default_get_preferred_width(_cell, gtkwidget, minimum_size,
			natural_size);
}
#else
void _w_listviewbase_rendererGetSizeProc(w_widget* widget,_w_control_priv *priv, GtkCellRenderer *cell,
		GtkWidget *widget, const GdkRectangle *cell_area, gint *x_offset,
		gint *y_offset, gint *width, gint *height);
#endif
