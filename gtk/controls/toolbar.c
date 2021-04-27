/*
 * toolbar.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "toolbar.h"
#include "../widgets/toolkit.h"
/*
 * toolitem
 *
 * GQuark : 0 : parent
 * 			1 : user_data
 * 			2 : id
 * 			3 : control
 */
#define GQUARK_PARENT 0
#define GQUARK_DATA 1
#define GQUARK_ID 2
#define GQUARK_CONTROL 3
wresult _w_toolitem_get_data(w_item *item, void **data) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	*data = g_object_get_qdata(G_OBJECT(toolitem),
			gtk_toolkit->quark[GQUARK_DATA]);
	return W_TRUE;
}
wresult _w_toolitem_get_index(w_item *item) {
	return _W_ITEM(item)->index;
}
wresult _w_toolitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_TOOL_BUTTON(toolitem)) {
		GtkWidget *label = gtk_tool_button_get_label_widget(
				GTK_TOOL_BUTTON(toolitem));
		if (label != 0) {
			const char *str = gtk_label_get_label(GTK_LABEL(label));
			_gtk_alloc_set_text(alloc, user_data, str, -1, enc);
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_toolitem_set_data(w_item *item, void *data) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	g_object_set_qdata(G_OBJECT(item), gtk_toolkit->quark[GQUARK_DATA], data);
	return W_TRUE;
}
wresult _w_toolitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_TOOL_BUTTON(toolitem)) {
		GtkWidget *label = gtk_tool_button_get_label_widget(
				GTK_TOOL_BUTTON(toolitem));
		if (label != 0) {
			int newlength, mnemonic;
			char *str = _gtk_text_fix(text, length, enc, &newlength, &mnemonic);
			if (str != 0) {
				gtk_label_set_text_with_mnemonic(GTK_LABEL(label), str);
			}
			_gtk_text_free(text, str, newlength);
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_toolitem_get_bounds(w_toolitem *item, w_rect *rect) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(parent);
	priv->force_resize(W_CONTROL(parent), priv);
	GtkWidget *topHandle = (GtkWidget*) toolitem;
	GtkAllocation allocation;
	gtk_widget_get_allocation(topHandle, &allocation);
	rect->x = allocation.x;
	rect->y = allocation.y;
	rect->width = allocation.width;
	rect->height = allocation.height;
	if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
		int clientWidth = priv->get_client_width(W_CONTROL(parent), priv);
		rect->x = clientWidth - rect->width - rect->x;
	}
	if (GTK_IS_SEPARATOR_TOOL_ITEM(toolitem)) {
		void *control = g_object_get_qdata(G_OBJECT(toolitem),
				gtk_toolkit->quark[GQUARK_CONTROL]);
		if (control != 0) {
			rect->height = WMAX(rect->height, 23);
		}

	}
	return W_TRUE;
}
wresult _w_toolitem_get_control(w_toolitem *item, w_control **control) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_SEPARATOR_TOOL_ITEM(toolitem)) {
		*control = g_object_get_qdata(G_OBJECT(toolitem),
				gtk_toolkit->quark[GQUARK_CONTROL]);
		return W_TRUE;
	}
	*control = 0;
	return W_FALSE;
}
wresult _w_toolitem_get_enabled(w_toolitem *item) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	GtkWidget *topHandle = (GtkWidget*) toolitem;
	return gtk_widget_get_sensitive(topHandle);
}
wresult _w_toolitem_get_id(w_toolitem *item) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	intptr_t id = (intptr_t) g_object_get_qdata(G_OBJECT(item),
			gtk_toolkit->quark[GQUARK_ID]);
	return (id & 0xFFFF);
}
wresult _w_toolitem_get_menu(w_toolitem *item, w_menu **menu) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_MENU_TOOL_BUTTON(toolitem)) {
		GtkWidget *hmenu = gtk_menu_tool_button_get_menu(
				GTK_MENU_TOOL_BUTTON(toolitem));
		*menu = (w_menu*) _w_widget_find_control(hmenu);
		return W_TRUE;
	}
	*menu = 0;
	return W_FALSE;
}
wresult _w_toolitem_get_selection(w_toolitem *item) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_TOGGLE_TOOL_BUTTON(toolitem)) {
		return gtk_toggle_tool_button_get_active(
				GTK_TOGGLE_TOOL_BUTTON(toolitem));
	}
	return W_FALSE;
}
wresult _w_toolitem_get_style(w_toolitem *item) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_TOGGLE_TOOL_BUTTON(toolitem)) {
		return W_CHECK;
	}
	if (GTK_IS_MENU_TOOL_BUTTON(toolitem)) {
		return W_DROP_DOWN;
	}
	if (GTK_IS_TOOL_BUTTON(toolitem)) {
		return W_PUSH;
	}
	if (GTK_IS_SEPARATOR_TOOL_ITEM(toolitem)) {
		return W_SEPARATOR;
	}
	return 0;
}
wresult _w_toolitem_get_tooltip_text(w_toolitem *item, w_alloc alloc,
		void *user_data, int enc) {
	return W_FALSE;
}
wresult _w_toolitem_get_width(w_toolitem *item) {
	return W_FALSE;
}
wresult _w_toolitem_is_enabled(w_toolitem *item) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	GtkWidget *topHandle = (GtkWidget*) toolitem;
	if (gtk_widget_get_sensitive(topHandle)) {
		w_widget *parent = _W_ITEM(item)->parent;
		return w_control_is_enabled(W_CONTROL(parent));

	}
	return W_FALSE;
}
void _w_toolitem_resize_control(w_toolitem *item) {
	w_control *control;
	_w_toolitem_get_control(item, &control);
	if (w_widget_is_ok(W_WIDGET(control)) > 0) {
		/*
		 * Set the size and location of the control
		 * separately to minimize flashing in the
		 * case where the control does not resize
		 * to the size that was requested.  This
		 * case can occur when the control is a
		 * combo box.
		 */
		w_widget *parent = _W_ITEM(item)->parent;
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(parent);
		priv->force_resize(W_CONTROL(parent), priv);
		w_rect rect, itemRect;
		_w_toolitem_get_bounds(item, &itemRect);
		w_control_set_bounds(control, 0, &itemRect.sz);
		_w_toolitem_resize_handle(item, itemRect.width, itemRect.height);
		w_control_get_bounds(control, &rect.pt, &rect.sz);
		rect.x = itemRect.x + (itemRect.width - rect.width) / 2, 0;
		rect.y = itemRect.y + (itemRect.height - rect.height) / 2, 0;
		w_control_set_bounds(control, &rect.pt, 0);
	}
}
void _w_toolitem_resize_handle(w_toolitem *item, int width, int height) {
	GtkWidget *handle = (GtkWidget*) _W_TOOLITEM(item)->toolItem;
	gtk_widget_set_size_request(handle, width, height);
	/*
	 * Cause a size allocation this widget's topHandle.  Note that
	 * all calls to gtk_widget_size_allocate() must be preceded by
	 * a call to gtk_widget_size_request().
	 */
	GtkRequisition requisition;
	gtk_widget_size_request(handle, &requisition);
	GtkAllocation allocation;
	gtk_widget_get_allocation(handle, &allocation);
	allocation.width = width;
	allocation.height = height;
	gtk_widget_size_allocate(handle, &allocation);
}
wresult _w_toolitem_set_control(w_toolitem *item, w_control *control) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_SEPARATOR_TOOL_ITEM(toolitem)) {
		g_object_set_qdata(G_OBJECT(toolitem),
				gtk_toolkit->quark[GQUARK_CONTROL], control);
		_w_toolbar_relayout(W_TOOLBAR(parent));
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_toolitem_set_enabled(w_toolitem *item, int enabled) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	GtkWidget *topHandle = (GtkWidget*) toolitem;
	if (gtk_widget_get_sensitive(topHandle) == enabled)
		return W_TRUE;
	gtk_widget_set_sensitive(topHandle, enabled);
	return W_TRUE;
}
wresult _w_toolitem_set_id(w_toolitem *item, wushort id) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	intptr_t _id = (intptr_t) g_object_get_qdata(G_OBJECT(item),
			gtk_toolkit->quark[GQUARK_ID]);
	_id = (_id & ~0xFFFF) | id;
	g_object_set_qdata(G_OBJECT(item), gtk_toolkit->quark[GQUARK_ID],
			(void*) _id);
	return W_TRUE;
}
wresult _w_toolitem_set_image(w_toolitem *item, int image) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_TOOL_BUTTON(toolitem)) {
		_w_image_widget *im =
				(_w_image_widget*) gtk_tool_button_get_icon_widget(
						GTK_TOOL_BUTTON(toolitem));
		if (im != 0) {
			im->index = image;
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_toolitem_set_menu(w_toolitem *item, w_menu *menu) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_MENU_TOOL_BUTTON(toolitem)) {
		GtkWidget *hmenu = _W_WIDGET(menu)->handle;
		gtk_menu_tool_button_set_menu(GTK_MENU_TOOL_BUTTON(toolitem), hmenu);
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_toolitem_set_selection(w_toolitem *item, int selected) {
	GtkWidget *toolitem = (GtkWidget*) _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_TOGGLE_TOOL_BUTTON(toolitem)) {
		g_signal_handlers_block_matched(toolitem, G_SIGNAL_MATCH_DATA, 0, 0, 0,
				0, (void*) SIGNAL_CLICKED);
		gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(toolitem),
				selected);
		g_signal_handlers_unblock_matched(toolitem, G_SIGNAL_MATCH_DATA, 0, 0,
				0, 0, (void*) SIGNAL_CLICKED);
	}
	return W_FALSE;
}
wresult _w_toolitem_set_tooltip_text(w_toolitem *item, const char *string,
		int length, int enc) {
	return W_FALSE;
}
wresult _w_toolitem_set_width(w_toolitem *item, int width) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_SEPARATOR_TOOL_ITEM(toolitem)) {
		_w_toolitem_resize_handle(item, width,
				(_W_WIDGET(parent)->style & W_VERTICAL) != 0 ? 6 : 15);
		_w_toolbar_relayout(W_TOOLBAR(parent));
		return W_TRUE;
	}
	return W_FALSE;
}
/*
 * toolbar
 */
wuint64 _w_toolbar_check_style(w_widget *control, wuint64 style) {
	style |= W_NO_FOCUS;
	return (style | W_NO_REDRAW_RESIZE) & ~(W_VSCROLL | W_HSCROLL);
}
wresult _w_toolbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	/*
	 * Feature in GTK. Size of toolbar is calculated incorrectly
	 * and appears as just the overflow arrow, if the arrow is enabled
	 * to display. The fix is to disable it before the computation of
	 * size and enable it if WRAP style is set.
	 */
	GtkToolbar *handle = GTK_TOOLBAR(_W_WIDGET(widget)->handle);
	gtk_toolbar_set_show_arrow(handle, FALSE);
	wresult result = _w_control_compute_native_size(widget, (GtkWidget*) handle,
			e, priv);
	if ((_W_WIDGET(widget)->style & W_WRAP) != 0)
		gtk_toolbar_set_show_arrow(handle, TRUE);
	return result;
}
wresult _w_toolbar_create_handle(w_widget *widget, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	GtkWidget *fixedHandle, *handle = 0;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_THEME_BACKGROUND;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	handle = gtk_toolbar_new();
	if (handle == 0)
		goto _err;
	GtkOrientation orientation =
			(style & W_VERTICAL) != 0 ?
					GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL;
	gtk_orientable_set_orientation(GTK_ORIENTABLE(handle), orientation);
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	GtkIconSize iconesize;
#if GTK3
	iconesize = GTK_ICON_SIZE_SMALL_TOOLBAR;
#endif

	/*
	 * Bug in GTK.  GTK will segment fault if gtk_widget_reparent() is called
	 * on a tool bar or on a widget hierarchy containing a tool bar when the icon
	 * size is not GTK_ICON_SIZE_LARGE_TOOLBAR.  The fix is to set the icon
	 * size to GTK_ICON_SIZE_LARGE_TOOLBAR.
	 *
	 * Note that the segmentation fault does not happen on GTK 3, but the
	 * tool bar preferred size is too big with GTK_ICON_SIZE_LARGE_TOOLBAR
	 * when the tool bar item has no image or text.
	 */
	gtk_toolbar_set_icon_size(GTK_TOOLBAR(handle), iconesize);

	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
#if GTK3
	//setFontDescription(defaultFont().handle);
#endif
	gtk_toolbar_set_style(GTK_TOOLBAR(handle), GTK_TOOLBAR_TEXT);
	_w_widget_set_control(handle, widget);
	_w_widget_set_control(fixedHandle, widget);
	_W_WIDGET(widget)->handle = handle;
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: if (fixedHandle)
		gtk_widget_destroy(fixedHandle);
	if (handle)
		gtk_widget_destroy(handle);
	return W_ERROR_NO_HANDLES;
}
void _w_toolbar_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_composite_hook_events(widget, priv);
}
wresult _w_toolbar_get_imagelist(w_toolbar *toolbar, w_imagelist **imagelist) {
	*imagelist = _W_TOOLBAR(toolbar)->imagelist;
	return W_TRUE;
}
wresult _w_toolbar_get_item(w_toolbar *toolbar, int index, w_toolitem *item) {
	GtkToolbar *handle = GTK_TOOLBAR(_W_WIDGET(toolbar)->handle);
	GtkToolItem *toolitem = gtk_toolbar_get_nth_item(handle, index);
	if (toolitem != 0) {
		_W_WIDGETDATA(&item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(toolbar);
		_W_ITEM(&item)->parent = W_WIDGET(toolbar);
		_W_ITEM(&item)->index = index;
		_W_TOOLITEM(&item)->toolItem = toolitem;
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_toolbar_get_item_from_point(w_toolbar *toolbar, w_point *point,
		w_toolitem *item) {
	return W_FALSE;
}
wresult _w_toolbar_get_item_count(w_toolbar *toolbar) {
	GtkWidget *handle = _W_WIDGET(toolbar)->handle;
	_w_widget_find_child data;
	data.count = 0;
	gtk_container_forall(GTK_CONTAINER(handle), _w_widget_children_count,
			&data);
	return data.count;
}
wresult _w_toolbar_get_items(w_toolbar *toolbar, w_iterator *items) {
	return W_FALSE;
}
wresult _w_toolbar_get_row_count(w_toolbar *toolbar) {
	/* On GTK, toolbars cannot wrap */
	return 1;
}
void _w_widget_children_sensitive(GtkWidget *widget, gpointer data) {
	gtk_widget_set_sensitive(widget, TRUE);
}
wresult _w_toolbar_insert_item(w_toolbar *toolbar, w_toolitem *item, int style,
		int index) {
	GtkWidget *labelHandle = 0;
	GtkToolItem *handle = 0;
	GtkWidget *arrowHandle = 0;
	_w_image_widget *imageHandle = 0;
	_w_control_priv *priv;
	int bits = W_SEPARATOR | W_RADIO | W_CHECK | W_PUSH | W_DROP_DOWN;
	if ((style & W_SEPARATOR) == 0) {
		labelHandle = gtk_label_new_with_mnemonic("");
		if (labelHandle == 0)
			goto _err;
		imageHandle = _w_image_widget_new();
		if (imageHandle == 0)
			goto _err;
		imageHandle->get_image_list = (__get_image_list) _w_toolbar_get_imagelist;
		imageHandle->parent = W_WIDGET(toolbar);
		imageHandle->index = -1;
	}
	switch (style & bits) {
	case W_SEPARATOR:
		handle = gtk_separator_tool_item_new();
		if (handle == 0)
			goto _err;
		gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(handle), TRUE);
		break;
	case W_DROP_DOWN: {
		handle = gtk_menu_tool_button_new(0, 0);
		if (handle == 0)
			goto _err;
		/*
		 * Feature in GTK. The arrow button of DropDown tool-item is
		 * disabled when it does not contain menu. The fix is to
		 * find the arrow button handle and enable it.
		 */
		GtkWidget *child = gtk_bin_get_child(GTK_BIN(handle));
		gtk_container_forall(GTK_CONTAINER(child), _w_widget_children_sensitive,
				0);
	}
		break;
	case W_RADIO:
		/*
		 * Because GTK enforces radio behavior in a button group
		 * a radio group is not created for each set of contiguous
		 * buttons, each radio button will not draw unpressed.
		 * The fix is to use toggle buttons instead.
		 */
	case W_CHECK:
		handle = gtk_toggle_tool_button_new();
		if (handle == 0)
			goto _err;
		break;
	case W_PUSH:
	default:
		handle = gtk_tool_button_new(0, 0);
		if (handle == 0)
			goto _err;
		break;
	}
	if (labelHandle != 0) {
		gtk_tool_button_set_label_widget(GTK_TOOL_BUTTON(handle), labelHandle);
		gtk_widget_show(labelHandle);
	}
	if (imageHandle != 0) {
		gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(handle),
				(GtkWidget*) imageHandle);
	}
	/*if ((_W_WIDGET(toolbar)->state & STATE_FOREGROUND) != 0) {
	 setForegroundColor(parent.getForegroundGdkColor());
	 }
	 if ((_W_WIDGET(toolbar)->state & STATE_FONT) != 0) {
	 setFontDescription(parent.getFontDescription());
	 }*/
	/*
	 * Feature in GTK. GtkToolButton class uses this property to
	 * determine whether to show or hide its label when the toolbar
	 * style is GTK_TOOLBAR_BOTH_HORIZ (or W_RIGHT).
	 */
	if ((_W_WIDGET(toolbar)->style & W_RIGHT) != 0)
		gtk_tool_item_set_is_important(GTK_TOOL_ITEM(handle), TRUE);
	if ((style & W_SEPARATOR) == 0)
		gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(handle), TRUE);
	gtk_widget_show_all(GTK_WIDGET(handle));
	int _index;
	if (index < 0) {
		_index = gtk_toolbar_get_n_items(
				GTK_TOOLBAR(_W_WIDGET(toolbar)->handle));
	} else
		_index = index;
	gtk_toolbar_insert(GTK_TOOLBAR(_W_WIDGET(toolbar)->handle),
			GTK_TOOL_ITEM(handle), index);
	_w_toolbar_relayout(toolbar);
	if (item != 0) {
		_W_WIDGETDATA(item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(toolbar);
		_W_ITEM(item)->parent = W_WIDGET(toolbar);
		_W_ITEM(item)->index = _index;
		_W_TOOLITEM(item)->toolItem = GTK_TOOL_ITEM(handle);
	}
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
void _w_toolbar_relayout(w_toolbar *toolbar) {
	GtkToolbar *handle = GTK_TOOLBAR(_W_WIDGET(toolbar)->handle);
	int length = gtk_toolbar_get_n_items(handle);
	gboolean hasText = FALSE, hasImage = FALSE;
	w_toolitem item;
	_W_WIDGETDATA(&item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(toolbar);
	_W_ITEM(&item)->parent = W_WIDGET(toolbar);
	for (int i = 0; i < length; i++) {
		GtkToolItem *toolitem = gtk_toolbar_get_nth_item(handle, i);
		if (toolitem != 0) {
			_W_ITEM(&item)->index = i;
			_W_TOOLITEM(&item)->toolItem = toolitem;
			_w_toolitem_resize_control(&item);
			if (GTK_IS_TOOL_BUTTON(toolitem)) {
				GtkWidget *label = gtk_tool_button_get_label_widget(
						GTK_TOOL_BUTTON(toolitem));
				const char *text = gtk_label_get_text(GTK_LABEL(label));
				if (text != 0 && text[0] != 0) {
					hasText = TRUE;
				}
				_w_image_widget *imageHandle =
						(_w_image_widget*) gtk_tool_button_get_icon_widget(
								GTK_TOOL_BUTTON(toolitem));
				if (imageHandle->index >= 0)
					hasImage = TRUE;
			}
		}
	}
	GtkToolbarStyle type = GTK_TOOLBAR_ICONS;
	if (hasText && hasImage) {
		if ((_W_WIDGET(toolbar)->style & W_RIGHT) != 0) {
			type = GTK_TOOLBAR_BOTH_HORIZ;
		} else {
			type = GTK_TOOLBAR_BOTH;
		}
	} else if (hasText) {
		type = GTK_TOOLBAR_TEXT;
	} else if (hasImage) {
		type = GTK_TOOLBAR_ICONS;
	}
	gtk_toolbar_set_style(GTK_TOOLBAR(_W_WIDGET(toolbar)->handle), type);
}
wresult _w_toolbar_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_priv *priv) {
	GtkToolbar *handle = GTK_TOOLBAR(_W_WIDGET(control)->handle);
	gtk_toolbar_set_show_arrow(handle, FALSE);
	wresult result = _w_composite_set_bounds_0(control, location, size, priv);
	if ((result & 2) != 0)
		_w_toolbar_relayout(W_TOOLBAR(control));
	if ((_W_WIDGET(control)->style & W_WRAP) != 0)
		gtk_toolbar_set_show_arrow(handle, TRUE);
	return result;
}
wresult _w_toolbar_set_imagelist(w_toolbar *toolbar, w_imagelist *imagelist) {
	wresult ret = W_TRUE;
	if (imagelist == 0) {
		_W_TOOLBAR(toolbar)->imagelist = 0;
	} else {
		if (_W_IMAGELIST(imagelist)->images == 0) {
			_W_TOOLBAR(toolbar)->imagelist = 0;
			ret = W_ERROR_INVALID_ARGUMENT;
		} else {
			_W_TOOLBAR(toolbar)->imagelist = imagelist;
		}
	}
	//_w_control_update(W_CONTROL(tree));
	return ret;
}
void _w_toolbar_class_init(struct _w_toolbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TOOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_toolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_toolbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_toolbar);
	/*
	 * public function
	 */
	clazz->get_imagelist = _w_toolbar_get_imagelist;
	clazz->get_item = _w_toolbar_get_item;
	clazz->get_item_from_point = _w_toolbar_get_item_from_point;
	clazz->get_item_count = _w_toolbar_get_item_count;
	clazz->get_items = _w_toolbar_get_items;
	clazz->get_row_count = _w_toolbar_get_row_count;
	clazz->insert_item = _w_toolbar_insert_item;
	clazz->set_imagelist = _w_toolbar_set_imagelist;
	/*
	 *
	 */
	struct _w_toolitem_class *item = clazz->class_toolitem;
	_w_item_class_init(W_ITEM_CLASS(item));
	W_ITEM_CLASS(item)->get_data = _w_toolitem_get_data;
	W_ITEM_CLASS(item)->get_text = _w_toolitem_get_text;
	W_ITEM_CLASS(item)->set_data = _w_toolitem_set_data;
	W_ITEM_CLASS(item)->set_text = _w_toolitem_set_text;
	item->get_bounds = _w_toolitem_get_bounds;
	item->get_control = _w_toolitem_get_control;
	item->get_enabled = _w_toolitem_get_enabled;
	item->get_id = _w_toolitem_get_id;
	item->get_menu = _w_toolitem_get_menu;
	item->get_selection = _w_toolitem_get_selection;
	item->get_style = _w_toolitem_get_style;
	item->get_tooltip_text = _w_toolitem_get_tooltip_text;
	item->get_width = _w_toolitem_get_width;
	item->is_enabled = _w_toolitem_is_enabled;
	item->set_control = _w_toolitem_set_control;
	item->set_enabled = _w_toolitem_set_enabled;
	item->set_id = _w_toolitem_set_id;
	item->set_image = _w_toolitem_set_image;
	item->set_menu = _w_toolitem_set_menu;
	item->set_selection = _w_toolitem_set_selection;
	item->set_tooltip_text = _w_toolitem_set_tooltip_text;
	item->set_width = _w_toolitem_set_width;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.handle_top = _w_widget_hp;
	priv->handle_fixed = _w_widget_hp;
	priv->handle_event = _w_widget_hp;
	priv->handle_enterexit = _w_widget_h;
	priv->widget.compute_size = _w_toolbar_compute_size;
	priv->widget.check_style = _w_toolbar_check_style;
	priv->widget.create_handle = _w_toolbar_create_handle;
	priv->widget.hook_events = _w_toolbar_hook_events;
	priv->set_bounds_0 = _w_toolbar_set_bounds_0;

}
