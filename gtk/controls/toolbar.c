/*
 * toolbar.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "toolbar.h"
#include "../widgets/toolkit.h"
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
/*
 * toolitem
 *
 * GQuark : 1 : user_data
 * 			2 : id
 */
#define GQUARK_DATA 0
#define GQUARK_ID 1
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
	g_object_set_qdata(G_OBJECT(toolitem), gtk_toolkit->quark[GQUARK_DATA],
			data);
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
void _w_toolitem_arrow_handle_callback(GtkWidget *widget, gpointer data) {
	if (GTK_IS_ARROW(widget)) {
		*((GtkWidget**) data) = widget;
	}
}
void _w_toolitem_hook_events(w_widget *toolbar, w_toolitem *item, int style,
		_w_toolbar_priv *priv) {
	if ((style & W_SEPARATOR) != 0)
		return;
	_gtk_signal *signals = priv->signals;
	GtkToolItem *handle = _W_TOOLITEM(item)->toolItem;
	_w_widget_set_control(handle, toolbar);
	_w_widget_connect(handle, &priv->signals[_W_TOOLBAR_SIGNAL_CLICKED], FALSE);
	/*
	 * Feature in GTK. GtkToolItem does not respond to basic listeners
	 * such as button-press, enter-notify to it. The fix is to assign
	 * the listener to child (GtkButton) of the tool-item.
	 */
	GtkWidget *eventHandle = gtk_bin_get_child(GTK_BIN(handle));
	if ((style & W_DROP_DOWN) != 0) {
		GtkWidget *arrowHandle = 0;
		gtk_container_forall(GTK_CONTAINER(eventHandle),
				_w_toolitem_arrow_handle_callback, &arrowHandle);
		if (arrowHandle != 0) {
			_w_widget_set_control(arrowHandle, toolbar);
			_w_widget_connect(arrowHandle,
					&priv->signals[_W_TOOLBAR_SIGNAL_CLICKED_ARROW], FALSE);
		}
	}
	_w_widget_connect(handle, &priv->signals[_W_TOOLBAR_SIGNAL_MENU_PROXY],
	FALSE);
	_w_widget_set_control(eventHandle, toolbar);
	_w_widget_connect(eventHandle, &signals[SIGNAL_ENTER_NOTIFY_EVENT], FALSE);
	_w_widget_connect(eventHandle, &signals[SIGNAL_LEAVE_NOTIFY_EVENT], FALSE);
	_w_widget_connect(eventHandle, &signals[SIGNAL_FOCUS_IN_EVENT], FALSE);
	_w_widget_connect(eventHandle, &signals[SIGNAL_FOCUS_OUT_EVENT], FALSE);
	/*
	 * Feature in GTK.  Usually, GTK widgets propagate all events to their
	 * parent when they are done their own processing.  However, in contrast
	 * to other widgets, the buttons that make up the tool items, do not propagate
	 * the mouse up/down events. It is interesting to note that they DO propagate
	 * mouse motion events.  The fix is to explicitly forward mouse up/down events
	 * to the parent.
	 */
	int mask = GDK_EXPOSURE_MASK | GDK_POINTER_MOTION_MASK
			| GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK
			| GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_KEY_PRESS_MASK
			| GDK_KEY_RELEASE_MASK | GDK_FOCUS_CHANGE_MASK;
	gtk_widget_add_events(eventHandle, mask);
	_w_widget_connect(eventHandle, &signals[SIGNAL_BUTTON_PRESS_EVENT], FALSE);
	_w_widget_connect(eventHandle, &signals[SIGNAL_BUTTON_RELEASE_EVENT],
	FALSE);
	_w_widget_connect(eventHandle, &signals[SIGNAL_EVENT_AFTER], FALSE);

	_w_widget_connect(handle, &signals[SIGNAL_MAP], TRUE);
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
		w_control *control = 0;
		_w_toolitem_get_control(item, &control);
		if (control != 0) {
			rect->height = WMAX(rect->height, 23);
		}
	}
	return W_TRUE;
}
wresult _w_toolitem_get_control(w_toolitem *item, w_control **control) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_SEPARATOR_TOOL_ITEM(toolitem)) {
		w_widget *parent = _W_ITEM(item)->parent;
		w_event_toolbar event;
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_GET_CONTROL;
		event.event.widget = parent;
		event.item = item;
		_w_widget_send_event(parent, W_EVENT(&event), W_EVENT_SEND);
		*control = event.control;
		return W_TRUE;
	} else {
		*control = 0;
		return W_FALSE;
	}
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
wresult _w_toolitem_get_image(w_toolitem *item) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_TOOL_BUTTON(toolitem)) {
		_w_image_widget *im =
				(_w_image_widget*) gtk_tool_button_get_icon_widget(
						GTK_TOOL_BUTTON(toolitem));
		if (im != 0) {
			return im->index;
		}
	}
	return -1;
}
wresult _w_toolitem_get_menu(w_toolitem *item, w_menu **menu) {
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_MENU_TOOL_BUTTON(toolitem)) {
		w_widget *parent = _W_ITEM(item)->parent;
		w_event_toolbar event;
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_GET_CONTROL;
		event.event.widget = parent;
		event.item = item;
		_w_widget_send_event(parent, W_EVENT(&event), W_EVENT_SEND);
		*menu = event.menu;
		return W_TRUE;
	} else {
		*menu = 0;
		return W_FALSE;
	}
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
	GtkToolItem *toolitem = _W_TOOLITEM(item)->toolItem;
	if (GTK_IS_SEPARATOR_TOOL_ITEM(toolitem)) {
		w_widget *parent = _W_ITEM(item)->parent;
		w_event_toolbar event;
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_SET_CONTROL;
		event.event.widget = parent;
		event.item = item;
		event.control = control;
		_w_widget_send_event(parent, W_EVENT(&event), W_EVENT_SEND);
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
		w_widget *parent = _W_ITEM(item)->parent;
		w_event_toolbar event;
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_SET_CONTROL;
		event.event.widget = parent;
		event.item = item;
		event.menu = menu;
		_w_widget_send_event(parent, W_EVENT(&event), W_EVENT_SEND);
		GtkWidget *hmenu = _W_WIDGET(menu)->handle;
		gtk_menu_tool_button_set_menu(GTK_MENU_TOOL_BUTTON(toolitem), hmenu);
		return W_TRUE;
	} else
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
	fixedHandle = _w_fixed_new(widget);
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
	_w_fixed_set_child(fixedHandle, handle);
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
		W_WIDGETDATA(&item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(toolbar);
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
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(toolbar);
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
	case W_CHECK:
		handle = gtk_toggle_tool_button_new();
		if (handle == 0)
			goto _err;
		if (style & W_RADIO) {
			/*
			 * Because GTK enforces radio behavior in a button group
			 * a radio group is not created for each set of contiguous
			 * buttons, each radio button will not draw unpressed.
			 * The fix is to use toggle buttons instead.
			 */
			intptr_t _id = 0x10000;
			g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[GQUARK_ID],
					(void*) _id);
		}
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
		W_WIDGETDATA(item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(toolbar);
		_W_ITEM(item)->parent = W_WIDGET(toolbar);
		_W_ITEM(item)->index = _index;
		_W_TOOLITEM(item)->toolItem = GTK_TOOL_ITEM(handle);
	}
	_w_toolitem_hook_events(W_WIDGET(toolbar), item, style,
			(_w_toolbar_priv*) priv);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
void _w_toolbar_relayout(w_toolbar *toolbar) {
	GtkToolbar *handle = GTK_TOOLBAR(_W_WIDGET(toolbar)->handle);
	int length = gtk_toolbar_get_n_items(handle);
	gboolean hasText = FALSE, hasImage = FALSE;
	w_toolitem item;
	W_WIDGETDATA(&item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(toolbar);
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
		if (_W_IMAGELIST(imagelist)->handles == 0) {
			_W_TOOLBAR(toolbar)->imagelist = 0;
			ret = W_ERROR_INVALID_ARGUMENT;
		} else {
			_W_TOOLBAR(toolbar)->imagelist = imagelist;
		}
	}
	//_w_control_update(W_CONTROL(tree));
	return ret;
}
/*
 * signals
 */
gboolean _gtk_toolbar_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GdkEventButton *gdkEvent = (GdkEventButton*) e->args[0];
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	wresult result;
	if (handle != e->widget) {
		GtkAllocation allocation;
		gtk_widget_get_allocation(handle, &allocation);
		double x = gdkEvent->x + allocation.x;
		double y = gdkEvent->y + allocation.y;
		result = _gtk_composite_button_press_event(widget, e, priv);
		gdkEvent->x = x;
		gdkEvent->y = y;
	} else {
		result = _gtk_composite_button_press_event(widget, e, priv);
	}
	return result;
}

gboolean _gtk_toolbar_button_release_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	GdkEventButton *gdkEvent = (GdkEventButton*) e->args[0];
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	wresult result;
	if (handle != e->widget) {
		GtkAllocation allocation;
		gtk_widget_get_allocation(handle, &allocation);
		double x = gdkEvent->x + allocation.x;
		double y = gdkEvent->y + allocation.y;
		result = _gtk_control_button_release_event(widget, e, priv);
		gdkEvent->x = x;
		gdkEvent->y = y;
	} else {
		result = _gtk_control_button_release_event(widget, e, priv);
	}
	return result;
}
struct _gtk_toolbar_select_radio_struct {
	w_widget *parent;
	GtkWidget *widget;
	GtkWidget *lastSelected;
	_w_event_platform *e;
	_w_toolbar_priv *priv;
	union {
		int flags;
		struct {
			unsigned is_after :1;
			unsigned is_end :1;

		};
	};

};
void _gtk_toolbar_send_selection(GtkWidget *widget,
		struct _gtk_toolbar_select_radio_struct *st, gboolean activate) {
	w_event_toolbar event;
	_w_toolitem item;
	g_signal_handlers_block_matched(st->lastSelected, G_SIGNAL_MATCH_DATA, 0, 0,
			0, 0, &st->priv->signals[_W_TOOLBAR_SIGNAL_CLICKED]);
	gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(st->lastSelected),
			activate);
	g_signal_handlers_unblock_matched(st->lastSelected, G_SIGNAL_MATCH_DATA, 0,
			0, 0, 0, &st->priv->signals[_W_TOOLBAR_SIGNAL_CLICKED]);
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_ITEM_SELECTION;
	event.event.widget = st->parent;
	event.event.platform_event = _EVENT_PLATFORM(st->e);
	event.item = W_TOOLITEM(&item);
	W_WIDGETDATA(&item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(st->parent);
	_W_ITEM(&item)->parent = st->parent;
	_W_ITEM(&item)->index = -1;
	_W_TOOLITEM(&item)->toolItem = GTK_TOOL_ITEM(st->lastSelected);
	_w_widget_send_event(st->parent, W_EVENT(&event), W_EVENT_SEND);
}
void _gtk_toolbar_select_radio_callback(GtkWidget *widget, gpointer data) {
	struct _gtk_toolbar_select_radio_struct *st =
			(struct _gtk_toolbar_select_radio_struct*) data;
	gboolean is_radio;
	gboolean is_selected;
	if (widget == st->widget) {
		st->is_after = 1;
		if (st->lastSelected != 0) {
			_gtk_toolbar_send_selection(widget, st, FALSE);
		}
	} else if (!st->is_end) {
		is_radio = GTK_IS_TOGGLE_TOOL_BUTTON(widget);
		if (is_radio) {
			intptr_t _id = (intptr_t) g_object_get_qdata(G_OBJECT(widget),
					gtk_toolkit->quark[GQUARK_ID]);
			is_radio = (_id & 0x10000) != 0;
		}
		if (is_radio) {
			is_selected = gtk_toggle_tool_button_get_active(
					GTK_TOGGLE_TOOL_BUTTON(widget));
			if (is_selected) {
				st->lastSelected = widget;
				if (st->is_after) {
					_gtk_toolbar_send_selection(widget, st, FALSE);
				}
			}
		} else {
			st->lastSelected = 0;
			if (st->is_after)
				st->is_end = 1;
		}
	}
}
gboolean _gtk_toolbar_clicked(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_toolbar event;
	_w_toolitem item;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_ITEM_SELECTION;
	event.event.widget = widget;
	event.event.platform_event = _EVENT_PLATFORM(e);
	event.item = W_TOOLITEM(&item);
	W_WIDGETDATA(&item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(widget);
	_W_ITEM(&item)->parent = widget;
	_W_ITEM(&item)->index = -1;
	_W_TOOLITEM(&item)->toolItem = GTK_TOOL_ITEM(e->widget);
	if (GTK_IS_MENU_TOOL_BUTTON(e->widget) || GTK_IS_ARROW(e->widget)) {
		GdkEvent *gdkEvent = gtk_get_current_event();
		if (gdkEvent != 0) {
			switch (gdkEvent->type) {
			case GDK_KEY_RELEASE: //Fall Through..
			case GDK_BUTTON_PRESS:
			case GDK_2BUTTON_PRESS:
			case GDK_BUTTON_RELEASE: {
				if (GTK_IS_ARROW(e->widget)) {
					GtkWidget *topHandle = e->widget;
					_w_control_priv *priv = _W_CONTROL_GET_PRIV(widget);
					_w_toolbar_priv *tpriv = _W_TOOLBAR_PRIV(priv);
					/*
					 * Feature in GTK. ArrowButton stays in toggled state if there is no popup menu.
					 * It is required to set back the state of arrow to normal state after it is clicked.
					 */
					g_signal_handlers_block_matched(e->widget,
							G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
							&tpriv->signals[_W_TOOLBAR_SIGNAL_CLICKED_ARROW]);
					gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(e->widget),
					FALSE);
					g_signal_handlers_unblock_matched(e->widget,
							G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
							&tpriv->signals[_W_TOOLBAR_SIGNAL_CLICKED_ARROW]);
					event.detail = W_ARROW;
					GtkAllocation allocation;
					gtk_widget_get_allocation(topHandle, &allocation);
					event.location.x = allocation.x;
					if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
						int clientWidth = priv->get_client_width(
								W_CONTROL(widget), priv);
						event.location.x = clientWidth - allocation.width
								- event.location.x;
					}
					event.location.y = allocation.y + allocation.height;
				}
				break;
			}
			}
			gdk_event_free(gdkEvent);
		}
	}
	if (GTK_IS_TOGGLE_TOOL_BUTTON(e->widget)) {
		if ((_W_WIDGET(widget)->style & W_NO_RADIO_GROUP) == 0) {
			intptr_t _id = (intptr_t) g_object_get_qdata(G_OBJECT(e->widget),
					gtk_toolkit->quark[GQUARK_ID]);
			if ((_id & 0x10000) != 0) {
				GtkWidget *handle = _W_WIDGET(widget)->handle;
				struct _gtk_toolbar_select_radio_struct st;
				st.parent = widget;
				st.widget = e->widget;
				st.lastSelected = 0;
				st.flags = 0;
				st.e = e;
				st.priv = _W_TOOLBAR_PRIV(priv);
				gtk_container_forall(GTK_CONTAINER(handle),
						_gtk_toolbar_select_radio_callback, &st);
			}
		}
	}
	_w_widget_send_event(widget, W_EVENT(&event), W_EVENT_SEND);
	return FALSE;
}

gboolean _gtk_toolbar_create_menu_proxy(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return FALSE;
}
gboolean _gtk_toolbar_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	_gtk_control_enter_notify_event(widget, e, priv);
	return FALSE;
}

gboolean _gtk_toolbar_event_after(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GdkEvent *event = (GdkEvent*) e->args[0];
	switch (event->type) {
	case GDK_BUTTON_PRESS: {
		GdkEventButton *gdkEventButton = (GdkEventButton*) event;
		if (gdkEventButton->button == 3) {
			/*parent.showMenu((int) gdkEventButton.x_root,
			 (int) gdkEventButton.y_root);*/
		}
		break;
	}
	}
	return FALSE;
}

gboolean _gtk_toolbar_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*parent.hasChildFocus = true;
	 parent.currentFocusItem = this;*/
	return 0;
}

gboolean _gtk_toolbar_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	//parent.hasChildFocus = false;
	return 0;
}

gboolean _gtk_toolbar_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	_gtk_control_leave_notify_event(widget, e, priv);
	return FALSE;
}

gboolean _gtk_toolbar_map(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	//parent.fixZOrder();
	return FALSE;
}

gboolean _gtk_toolbar_mnemonic_activate(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return _gtk_control_mnemonic_activate(widget, e, priv);
}
_gtk_signal_info _gtk_toolbar_signal_lookup[_W_TOOLBAR_LAST] = { //
		{ SIGNAL_CLICKED, 2, "clicked" }, //
				{ SIGNAL_CLICKED, 2, "clicked" }, //
				{ SIGNAL_CREATE_MENU_PROXY, 2, "create-menu-proxy" }, //
		};
wresult _w_toolbar_dispose_class(struct _w_widget_class *clazz){
	return W_TRUE;
}
void _w_toolbar_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_toolbar_class *clazz) {
	if (classId == _W_CLASS_TOOLBAR) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&gtk_toolkit->class_toolbar_priv;
	}
	_w_composite_class_init(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TOOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_toolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_toolbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_toolbar);
	/*
	 * public function
	 */
	W_WIDGET_CLASS(clazz)->dispose_class = _w_toolbar_dispose_class;
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
	W_WIDGETDATA_CLASS(item)->toolkit = toolkit;
	_w_item_class_init(W_ITEM_CLASS(item));
	W_ITEM_CLASS(item)->get_data = _w_toolitem_get_data;
	W_ITEM_CLASS(item)->get_text = _w_toolitem_get_text;
	W_ITEM_CLASS(item)->set_data = _w_toolitem_set_data;
	W_ITEM_CLASS(item)->set_text = _w_toolitem_set_text;
	item->get_bounds = _w_toolitem_get_bounds;
	item->get_control = _w_toolitem_get_control;
	item->get_enabled = _w_toolitem_get_enabled;
	item->get_id = _w_toolitem_get_id;
	item->get_image = _w_toolitem_get_image;
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
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_TOOLBAR) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->widget.handle_top = _w_widget_hp;
		priv->handle_fixed = _w_widget_hp;
		priv->handle_event = _w_widget_hp;
		priv->handle_enterexit = _w_widget_h;
		priv->widget.compute_size = _w_toolbar_compute_size;
		priv->widget.check_style = _w_toolbar_check_style;
		priv->widget.create_handle = _w_toolbar_create_handle;
		priv->widget.hook_events = _w_toolbar_hook_events;
		priv->set_bounds_0 = _w_toolbar_set_bounds_0;
		_w_widget_init_signal(_W_TOOLBAR_PRIV(priv)->signals,
				_gtk_toolbar_signal_lookup, _W_TOOLBAR_LAST);
		/*
		 * signals
		 */
		_gtk_signal_fn *signals = priv->widget.signals;
		signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_toolbar_button_press_event;
		signals[SIGNAL_BUTTON_RELEASE_EVENT] =
				_gtk_toolbar_button_release_event;
		signals[SIGNAL_CLICKED] = _gtk_toolbar_clicked;
		signals[SIGNAL_CREATE_MENU_PROXY] = _gtk_toolbar_create_menu_proxy;
		signals[SIGNAL_ENTER_NOTIFY_EVENT] = _gtk_toolbar_enter_notify_event;
		signals[SIGNAL_EVENT_AFTER] = _gtk_toolbar_event_after;
		signals[SIGNAL_FOCUS_IN_EVENT] = _gtk_toolbar_focus_in_event;
		signals[SIGNAL_FOCUS_OUT_EVENT] = _gtk_toolbar_focus_out_event;
		signals[SIGNAL_LEAVE_NOTIFY_EVENT] = _gtk_toolbar_leave_notify_event;
		signals[SIGNAL_MAP] = _gtk_toolbar_map;
		signals[SIGNAL_MNEMONIC_ACTIVATE] = _gtk_toolbar_mnemonic_activate;
	}
}
