/*
 * tabview.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "tabview.h"
#include "../widgets/toolkit.h"
wresult _w_tabitem_find_handles(w_tabitem *item, _w_widget_handles *handles) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
	GtkWidget *pageHandle = gtk_notebook_get_nth_page(GTK_NOTEBOOK(handle),
	_W_ITEM(item)->index);
	if (pageHandle != 0) {
		GtkWidget *tab = gtk_notebook_get_tab_label(GTK_NOTEBOOK(handle),
				pageHandle);
		if (tab != 0) {
			_w_widget_get_handles(tab, handles);
		}
	}
	return W_FALSE;
}
wresult _w_tabitem_get_bounds(w_tabitem *item, w_rect *rect) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
	GtkAllocation allocation;
	gtk_widget_get_allocation(handle, &allocation);
	rect->x = allocation.x;
	rect->y = allocation.y;
	rect->width = allocation.width;
	rect->height = allocation.height;
	if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(parent);
		int client_width = priv->get_client_width(W_CONTROL(parent), priv);
		rect->x = client_width - rect->width - rect->x;
	}
	return W_TRUE;
}
int _w_tabitem_get_image(w_tabitem *item) {
	_w_widget_handles handles;
	wresult result = _w_tabitem_find_handles(item, &handles);
	if (handles.image != 0) {
		_w_image_widget *_image = (_w_image_widget*) handles.image;
		return _image->index;
	}
	return -1;
}
wresult _w_tabitem_remove(w_tabitem *item, int destroy_Control) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
	gtk_notebook_remove_page(GTK_NOTEBOOK(handle),
	_W_ITEM(item)->index);
	return W_TRUE;
}
wresult _w_tabitem_set_image(w_tabitem *item, int index) {
	_w_widget_handles handles;
	wresult result = _w_tabitem_find_handles(item, &handles);
	if (handles.image != 0) {
		_w_image_widget *_image = (_w_image_widget*) handles.image;
		_image->index = index;
		return W_TRUE;
	}
	return result;
}
wresult _w_tabitem_pack(w_tabitem *item, w_control *control) {
	if (w_widget_is_ok(W_WIDGET(control))) {
		w_widget *parent = _W_ITEM(item)->parent;
		GtkWidget *handle = _W_WIDGET(parent)->handle;
		GtkWidget *pageHandle = gtk_notebook_get_nth_page(GTK_NOTEBOOK(handle),
		_W_ITEM(item)->index);
		if (pageHandle != 0) {
			_w_control_priv *priv = _W_CONTROL_GET_PRIV(W_WIDGET(control));
			GtkWidget *topHandle = priv->widget.handle_top(W_WIDGET(control),
					priv);
			if (gtk_widget_get_parent(topHandle) != pageHandle) {
				g_object_ref(topHandle);
				gtk_widget_unparent(topHandle);
				gtk_widget_set_parent(topHandle, pageHandle);
				g_object_unref(topHandle);
			}
		}
		int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(handle));
		if (index == _W_ITEM(item)->index) {
			w_rect r;
			w_scrollable_get_client_area(W_SCROLLABLE(parent), &r);
			w_control_set_visible(control, W_TRUE);
			w_control_set_bounds(control, &r.pt, &r.sz);
		} else {
			w_control_set_visible(control, W_FALSE);
		}
	}
	return W_TRUE;
}
wresult _w_tabitem_get_data(w_item *item, void **data) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
	GtkWidget *pageHandle = gtk_notebook_get_nth_page(GTK_NOTEBOOK(handle),
	_W_ITEM(item)->index);
	if (pageHandle != 0) {
		*data = g_object_get_qdata(G_OBJECT(pageHandle), gtk_toolkit->quark[1]);
		return W_TRUE;
	} else {
		*data = 0;
		return W_FALSE;
	}
}
wresult _w_tabitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	_w_widget_handles handles;
	wresult result = _w_tabitem_find_handles(W_TABITEM(item), &handles);
	if (handles.label != 0) {
		const gchar *label = gtk_label_get_label(GTK_LABEL(handles.label));
		return _gtk_alloc_set_text(alloc, user_data, label, -1, enc);
	}
	return result;
}
wresult _w_tabitem_set_data(w_item *item, void *data) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
	GtkWidget *pageHandle = gtk_notebook_get_nth_page(GTK_NOTEBOOK(handle),
	_W_ITEM(item)->index);
	if (pageHandle != 0) {
		g_object_set_qdata(G_OBJECT(pageHandle), gtk_toolkit->quark[1], data);
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_tabitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	_w_widget_handles handles;
	wresult result = _w_tabitem_find_handles(W_TABITEM(item), &handles);
	if (handles.label != 0) {
		int newlength, mnemonic;
		char *str = _gtk_text_fix(text, length, enc, &newlength, &mnemonic);
		gtk_label_set_label(GTK_LABEL(handles.label), str);
		_gtk_text_free(text, str, newlength);
		return W_TRUE;
	}
	return result;
}
/*
 *	tabview
 */
GtkWidget* _w_tabview_handle_parenting(w_widget *widget,
		_w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	return gtk_widget_get_parent(handle);
}
GtkWidget* _w_tabview_handle_client(w_widget *widget, _w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(handle));
	if (index != -1) {
		return gtk_notebook_get_nth_page(GTK_NOTEBOOK(handle), index);
	}
	return handle;
}
wuint64 _w_tabview_check_style(w_widget *widget, wuint64 style) {
	style = _w_widget_check_bits(style, W_TOP, W_BOTTOM, 0, 0, 0, 0);
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_tabview_create_handle(w_widget *widget, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	GtkWidget *fixedHandle, *handle = 0;
	GtkPositionType pos;
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	handle = gtk_notebook_new();
	if (handle == 0)
		goto _err;
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	gtk_notebook_set_show_tabs(GTK_NOTEBOOK(handle), TRUE);
	gtk_notebook_set_scrollable(GTK_NOTEBOOK(handle), TRUE);
	pos = GTK_POS_TOP;
	if (style & W_BOTTOM) {
		pos = GTK_POS_BOTTOM;
	}
	if (style & W_LEFT) {
		pos = GTK_POS_LEFT;
	}
	if (style & W_RIGHT) {
		pos = GTK_POS_RIGHT;
	}
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(handle), pos);
	_w_widget_set_control(fixedHandle, widget);
	_w_widget_set_control(handle, widget);
	_W_WIDGET(widget)->handle = handle;
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: if (fixedHandle)
		gtk_widget_destroy(fixedHandle);
	if (handle)
		gtk_widget_destroy(handle);
	return W_ERROR_NO_HANDLES;
}
void _w_tabview_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_composite_hook_events(widget, priv);
	_w_tabview_priv *tpriv = (_w_tabview_priv*) priv;
	if (tpriv->signal_switch_page_id == 0) {
		tpriv->signal_switch_page_id = g_signal_lookup("switch-page",
				gtk_notebook_get_type());
	}
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	_w_widget_connect(handle, SIGNAL_SWITCH_PAGE, tpriv->signal_switch_page_id,
	FALSE);
}
wresult _w_tabview_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	wresult result = _w_composite_compute_size(widget, e, priv);
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	gboolean scrollable = gtk_notebook_get_scrollable(GTK_NOTEBOOK(handle));
	w_size notebookSize, *size;
	size = e->size;
	e->size = &notebookSize;
	gtk_notebook_set_scrollable(GTK_NOTEBOOK(handle), FALSE);
	_w_control_compute_native_size(widget, handle, e, priv);
	e->size = size;
	gtk_notebook_set_scrollable(GTK_NOTEBOOK(handle), scrollable);
	if (GTK_VERSION >= VERSION(3, 2, 0)) {
		int initialGap = 0;
		gtk_widget_style_get(handle, "initial-gap", &initialGap, NULL);
		notebookSize.width += initialGap * 2;
	}
	size->width = WMAX(notebookSize.width, size->width);
	size->height = WMAX(notebookSize.height, size->height);
	return result;
}
wresult _w_tabview_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	priv->force_resize(W_CONTROL(widget), priv);
	GtkWidget *clientHandle = priv->handle_client(widget, priv);
	GtkAllocation allocation;
	gtk_widget_get_allocation(clientHandle, &allocation);
	int clientX = allocation.x;
	int clientY = allocation.y;
	e->rect->x -= clientX;
	e->rect->y -= clientY;
	e->rect->width += clientX + clientX;
	if ((_W_WIDGET(widget)->style & W_BOTTOM) != 0) {
		GtkWidget *handle = _W_WIDGET(widget)->handle;
		int clientHeight = allocation.height;
		gtk_widget_get_allocation(handle, &allocation);
		int parentHeight = allocation.height;
		e->rect->height += parentHeight - clientHeight;
	} else {
		e->rect->height += clientX + clientY;
	}
	return W_TRUE;
}
wresult _w_tabview_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	wresult result = _w_composite_get_client_area(widget, e, priv);
	e->rect->x = 0;
	e->rect->y = 0;
	return result;
}
wresult _w_tabview_get_imagelist(w_tabview *tabview, w_imagelist **imagelist) {
	*imagelist = _W_TABVIEW(tabview)->imagelist;
	return W_TRUE;
}
wresult _w_tabview_get_item(w_tabview *tabview, int index, w_tabitem *item) {
	GtkWidget *handle = _W_WIDGET(tabview)->handle;
	int count = gtk_notebook_get_n_pages(GTK_NOTEBOOK(handle));
	if (index < count && index >= 0) {
		if (item != 0) {
			_W_WIDGETDATA(item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
			_W_ITEM(item)->parent = W_WIDGET(tabview);
			_W_ITEM(item)->index = index;
		}
		return W_TRUE;
	} else {
		_W_WIDGETDATA(item)->clazz = 0;
		return W_FALSE;
	}
}
wresult _w_tabview_get_item_p(w_tabview *tabview, w_point *point,
		w_tabitem *item) {
	return W_FALSE;
}
wresult _w_tabview_get_item_count(w_tabview *tabview) {
	GtkWidget *handle = _W_WIDGET(tabview)->handle;
	return gtk_notebook_get_n_pages(GTK_NOTEBOOK(handle));
}
wresult _w_tabview_get_items(w_tabview *tabview, w_iterator *items) {
	return W_FALSE;
}
wresult _w_tabview_get_selection(w_tabview *tabview, w_tabitem *item) {
	GtkWidget *handle = _W_WIDGET(tabview)->handle;
	int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(handle));
	return _w_tabview_get_item(tabview, index, item);
}
w_imagelist* _w_tabview_get_image_list(w_widget *widget) {
	return _W_TABVIEW(widget)->imagelist;
}
wresult _w_tabview_insert_item(w_tabview *tabview, w_tabitem *item, int index) {
	GtkWidget *handle = _W_WIDGET(tabview)->handle;
	_w_tabview_priv *priv = (_w_tabview_priv*) _w_widget_get_priv(
			W_WIDGET(tabview));
	GtkWidget *boxHandle, *labelHandle = 0, *pageHandle = 0, *closeHandle;
	_w_image_widget *imageHandle = 0;
	int i;
	boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	if (boxHandle == 0)
		goto _err;
	labelHandle = gtk_label_new_with_mnemonic("");
	if (labelHandle == 0)
		goto _err;
	imageHandle = _w_image_widget_new();
	if (imageHandle == 0)
		goto _err;
	imageHandle->get_image_list = _w_tabview_get_image_list;
	imageHandle->parent = W_WIDGET(tabview);
	imageHandle->index = -1;

	gtk_container_add(GTK_CONTAINER(boxHandle), (GtkWidget*) imageHandle);
	gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
	gtk_box_set_child_packing(GTK_BOX(boxHandle), labelHandle, TRUE, TRUE, 0,
			GTK_PACK_START);
	gtk_box_set_child_packing(GTK_BOX(boxHandle), (GtkWidget*) imageHandle,
	TRUE, TRUE, 0, GTK_PACK_START);
	pageHandle = _w_fixed_new();
	if (pageHandle == 0)
		goto _err;
	g_object_set_qdata(G_OBJECT(pageHandle), gtk_toolkit->quark[0], tabview);
	if (_W_WIDGET(tabview)->style & W_CLOSE) {
		closeHandle = gtk_button_new_from_icon_name("gtk-close",
				GTK_ICON_SIZE_MENU);
		if (closeHandle == 0)
			goto _err;
		if (priv->signal_selection_id == 0) {
			priv->signal_selection_id = g_signal_lookup("clicked",
					gtk_button_get_type());
		}
		g_object_set_qdata(G_OBJECT(closeHandle), gtk_toolkit->quark[0],
				tabview);
		g_object_set_qdata(G_OBJECT(closeHandle), gtk_toolkit->quark[1],
				pageHandle);
		_w_widget_connect(closeHandle, SIGNAL_CLICKED,
				priv->signal_selection_id,
				FALSE);
		gtk_button_set_relief(GTK_BUTTON(closeHandle), GTK_RELIEF_NONE);
		gtk_container_add(GTK_CONTAINER(boxHandle), closeHandle);
		gtk_box_set_child_packing(GTK_BOX(boxHandle), (GtkWidget*) imageHandle,
		TRUE, TRUE, 0, GTK_PACK_START);
	}
	g_signal_handlers_block_matched(GTK_NOTEBOOK(handle), G_SIGNAL_MATCH_DATA,
			priv->signal_switch_page_id, 0, 0, 0, 0);
	i = gtk_notebook_insert_page(GTK_NOTEBOOK(handle), pageHandle, boxHandle,
			index);
	g_signal_handlers_unblock_matched(GTK_NOTEBOOK(handle), G_SIGNAL_MATCH_DATA,
			priv->signal_switch_page_id, 0, 0, 0, 0);
	if (i < 0)
		goto _err;

	gtk_widget_show_all(boxHandle);
	gtk_widget_show_all(pageHandle);
	gtk_widget_hide(GTK_WIDGET(imageHandle));
	if (item != 0) {
		_W_WIDGETDATA(item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
		_W_ITEM(item)->parent = W_WIDGET(tabview);
		_W_ITEM(item)->index = i;
	}
	return W_TRUE;
	_err: if (labelHandle)
		gtk_widget_destroy(labelHandle);
	if (pageHandle)
		gtk_widget_destroy(pageHandle);
	if (item != 0) {
		_W_WIDGETDATA(item)->clazz = 0;
	}
	return W_ERROR_NO_HANDLES;
}
wresult _w_tabview_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_priv *priv) {
	wresult result = _w_control_set_bounds_0(control, location, size, priv);
	if (result >= 2) {
		w_tabitem item;
		_w_tabview_get_selection(W_TABVIEW(control), &item);
		w_control *c = 0;
		if (W_WIDGETDATA(&item)->clazz != 0) {
			c = w_tabitem_get_control(W_TABITEM(&item));
		}
		if (c != 0) {
			w_event e;
			e.type = W_EVENT_RESIZE;
			e.platform_event = 0;
			e.data = 0;
			e.widget = W_WIDGET(c);
			w_rect r;
			w_scrollable_get_client_area(W_SCROLLABLE(control), &r);
			w_control_set_bounds(c, &r.pt, &r.sz);
		}
	}
	return result;
}
wresult _w_tabview_set_imagelist(w_tabview *tabview, w_imagelist *imagelist) {
	wresult ret = W_TRUE;
	if (imagelist == 0) {
		_W_TABVIEW(tabview)->imagelist = 0;
	} else {
		if (_W_IMAGELIST(imagelist)->images == 0) {
			_W_TABVIEW(tabview)->imagelist = 0;
			ret = W_ERROR_INVALID_ARGUMENT;
		} else {
			_W_TABVIEW(tabview)->imagelist = imagelist;
		}
	}
	_w_control_redraw(W_CONTROL(tabview), 0, 1);
	return ret;
}
wresult _w_tabview_set_selection(w_tabview *tabview, int index) {
	gtk_notebook_set_current_page(GTK_NOTEBOOK(_W_WIDGET(tabview)->handle),
			index);
	return W_TRUE;
}
/*
 * signals
 */
gboolean _gtk_tabview_focus(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_tabview_switch_page(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_tabitem _e;
	_w_tabitem item;
	_w_tabview_get_selection(W_TABVIEW(widget), W_TABITEM(&item));
	w_control *c = 0;
	if (W_WIDGETDATA(&item)->clazz != 0) {
		c = w_tabitem_get_control(W_TABITEM(&item));
	}
	if (c != 0 && w_widget_is_ok(W_WIDGET(c)) > 0) {
		w_control_set_visible(c, W_FALSE);
		GtkWidget *pageHandle = gtk_notebook_get_nth_page(
				GTK_NOTEBOOK(_W_WIDGET(item.item.parent)->handle),
				_W_ITEM(&item)->index);
		if (pageHandle != 0) {
			_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(c);
			GtkWidget *topHandle = cpriv->widget.handle_top(W_WIDGET(c), cpriv);
			if (gtk_widget_get_parent(topHandle) != pageHandle) {
				gtk_widget_unparent(pageHandle);
				gtk_widget_set_parent(topHandle, pageHandle);
			}
		}
	}
	_w_tabview_get_item(W_TABVIEW(widget), (intptr_t) e->args[1],
			W_TABITEM(&item));
	c = w_tabitem_get_control(W_TABITEM(&item));
	if (c != 0) {
		if (w_widget_is_ok(W_WIDGET(c)) > 0) {
			w_rect r;
			w_scrollable_get_client_area(W_SCROLLABLE(widget), &r);
			w_control_set_visible(c, W_TRUE);
			w_control_set_bounds(c, &r.pt, &r.sz);
		} else
			c = 0;
	}
	_e.event.type = W_EVENT_ITEM_SELECTION;
	_e.event.widget = widget;
	_e.event.platform_event = (struct w_event_platform*) e;
	_e.event.data = 0;
	_e.item = W_TABITEM(&item);
	_e.control = c;
	_w_widget_send_event(widget, (w_event*) &_e);
	return FALSE;
}
gboolean _gtk_tabview_clicked(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_tabitem _e;
	_w_tabitem item;
	GtkWidget *pageHandle = (GtkWidget*) g_object_get_qdata(G_OBJECT(e->widget),
			gtk_toolkit->quark[1]);
	GtkNotebook *notebook = GTK_NOTEBOOK(_W_WIDGET(widget)->handle);
	wuint l = gtk_notebook_get_n_pages(notebook);
	int index = -1;
	for (wuint i = 0; i < l; i++) {
		if (gtk_notebook_get_nth_page(notebook, i) == pageHandle) {
			index = i;
			break;
		}
	}
	if (index == -1)
		return FALSE;
	_w_tabview_get_item(W_TABVIEW(widget), index, W_TABITEM(&item));
	_e.event.type = W_EVENT_ITEM_CLOSE;
	_e.event.widget = widget;
	_e.event.platform_event = _EVENT_PLATFORM(e);
	_e.event.data = 0;
	_e.item = W_TABITEM(&item);
	_e.control = 0;
	_w_widget_send_event(widget, (w_event*) &_e);
	return FALSE;
}
void _w_tabview_class_init(struct _w_tabview_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TABVIEW;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tabview_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tabview);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tabview);

	clazz->get_imagelist = _w_tabview_get_imagelist;
	clazz->get_item = _w_tabview_get_item;
	clazz->get_item_p = _w_tabview_get_item_p;
	clazz->get_item_count = _w_tabview_get_item_count;
	clazz->get_items = _w_tabview_get_items;
	clazz->get_selection = _w_tabview_get_selection;
	clazz->insert_item = _w_tabview_insert_item;
	clazz->set_imagelist = _w_tabview_set_imagelist;
	clazz->set_selection = _w_tabview_set_selection;
	/*
	 * class item
	 */
	struct _w_tabitem_class *item = clazz->class_tabitem;
	_w_item_class_init(W_ITEM_CLASS(item));
	item->item.widgetdata.toolkit = (w_toolkit*) gtk_toolkit;
	item->get_bounds = _w_tabitem_get_bounds;
	item->get_image = _w_tabitem_get_image;
	item->set_image = _w_tabitem_set_image;
	item->remove = _w_tabitem_remove;
	item->pack = _w_tabitem_pack;
	item->item.get_data = _w_tabitem_get_data;
	item->item.get_text = _w_tabitem_get_text;
	item->item.set_data = _w_tabitem_set_data;
	item->item.set_text = _w_tabitem_set_text;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.handle_top = _w_widget_hp;
	priv->handle_fixed = _w_widget_hp;
	priv->handle_event = _w_widget_h;
	priv->handle_client = _w_tabview_handle_client;
	priv->widget.check_style = _w_tabview_check_style;
	priv->widget.create_handle = _w_tabview_create_handle;
	priv->widget.hook_events = _w_tabview_hook_events;
	priv->set_bounds_0 = _w_tabview_set_bounds_0;
	priv->widget.get_client_area = _w_tabview_get_client_area;
	priv->widget.compute_size = _w_tabview_compute_size;
	priv->widget.compute_trim = _w_tabview_compute_trim;
	_W_SCROLLABLE_PRIV(priv)->handle_scrolled = _w_widget_h0;
	_W_COMPOSITE_PRIV(priv)->handle_parenting = _w_tabview_handle_parenting;
	/*
	 * signals
	 */
	_gtk_signal *signals = priv->widget.signals;
	signals[SIGNAL_FOCUS] = _gtk_tabview_focus;
	signals[SIGNAL_SWITCH_PAGE] = _gtk_tabview_switch_page;
	signals[SIGNAL_CLICKED] = _gtk_tabview_clicked;
}
