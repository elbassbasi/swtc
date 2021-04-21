/*
 * expandbar.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "expandbar.h"
#include "../widgets/toolkit.h"
enum {
	GQUARK_ITEM_DATA = 1, GQUARK_ITEM_HEIGTH = 2,
};
GtkWidget* _w_expanditem_handle_control(GtkWidget *handle) {
	GtkWidget *controlHandle = gtk_bin_get_child(GTK_BIN(handle));
	return controlHandle;
}
wresult _w_expanditem_get_data(w_item *item, void **data) {
	GtkWidget *handle = _W_EXPANDITEM(item)->handle;
	*data = g_object_get_qdata(G_OBJECT(handle),
			gtk_toolkit->quark[GQUARK_ITEM_DATA]);
	return W_TRUE;
}
wresult _w_expanditem_get_index(w_item *item) {
	return _W_ITEM(item)->index;
}
wresult _w_expanditem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	wresult result = W_FALSE;
	GtkWidget *handle = _W_EXPANDITEM(item)->handle;
	GtkWidget *box = gtk_expander_get_label_widget(GTK_EXPANDER(handle));
	if (box != 0) {
		_w_widget_handles handles;
		_w_widget_get_handles(box, &handles);
		if (handles.label != 0) {
			const char *str = gtk_label_get_label(GTK_LABEL(handles.label));
			_gtk_alloc_set_text(alloc, user_data, str, -1, enc);
			result = W_TRUE;
		}
	}
	return result;
}
wresult _w_expanditem_set_data(w_item *item, void *data) {
	GtkWidget *handle = _W_EXPANDITEM(item)->handle;
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[GQUARK_ITEM_DATA],
			data);
	return W_TRUE;
}
wresult _w_expanditem_set_text(w_item *item, const char *text, int length,
		int enc) {
	wresult result = W_FALSE;
	GtkWidget *handle = _W_EXPANDITEM(item)->handle;
	GtkWidget *box = gtk_expander_get_label_widget(GTK_EXPANDER(handle));
	if (box != 0) {
		_w_widget_handles handles;
		_w_widget_get_handles(box, &handles);
		if (handles.label != 0) {
			int newlength, mnemonic;
			char *str = _gtk_text_fix(text, length, enc, &newlength, &mnemonic);
			gtk_label_set_label(GTK_LABEL(handles.label), str);
			_gtk_text_free(text, str, newlength);
			result = W_TRUE;
		}
	}
	return result;
}
wresult _w_expanditem_get_control(w_expanditem *expanditem,
		w_control **control) {
	GtkWidget *handle = _W_EXPANDITEM(expanditem)->handle;
	GtkWidget *controlHandle = _w_expanditem_handle_control(handle);
	if (controlHandle == 0)
		return W_FALSE;
	*control = (w_control*) _w_widget_find_control(controlHandle);
	return W_TRUE;
}
wresult _w_expanditem_get_expanded(w_expanditem *expanditem) {
	GtkWidget *handle = _W_EXPANDITEM(expanditem)->handle;
	return gtk_expander_get_expanded(GTK_EXPANDER(handle));
}
wresult _w_expanditem_get_header_height(w_expanditem *expanditem) {
	GtkWidget *handle = _W_EXPANDITEM(expanditem)->handle;
	gboolean expanded = gtk_expander_get_expanded(GTK_EXPANDER(handle));
	GtkAllocation allocation;
	gtk_widget_get_allocation(handle, &allocation);
	int height = 0;
	if (expanded) {
		height = _w_expanditem_get_height(expanditem);
	}
	return allocation.height - height;
}
wresult _w_expanditem_get_height(w_expanditem *expanditem) {
	GtkWidget *handle = _W_EXPANDITEM(expanditem)->handle;
	wintptr heigth = (wintptr) g_object_get_qdata(G_OBJECT(handle),
			gtk_toolkit->quark[GQUARK_ITEM_HEIGTH]);
	return heigth;
}
wresult _w_expanditem_get_image(w_expanditem *expanditem) {
	wresult result = -1;
	GtkWidget *handle = _W_EXPANDITEM(expanditem)->handle;
	GtkWidget *box = gtk_expander_get_label_widget(GTK_EXPANDER(handle));
	if (box != 0) {
		_w_widget_handles handles;
		_w_widget_get_handles(box, &handles);
		_w_image_widget *imageHandle = (_w_image_widget*) handles.image;
		if (imageHandle != 0) {
			result = imageHandle->index;
		}
	}
	return result;
}
void _w_expanditem_resize_control(w_expandbar *expandbar, GtkWidget *handle,
		int yScroll) {
	GtkWidget *controlHandle = _w_expanditem_handle_control(handle);
	if (controlHandle == 0)
		return;
	w_control *control = (w_control*) _w_widget_find_control(controlHandle);
	if (w_widget_is_ok(W_WIDGET(control)) > 0) {
		gboolean visible = gtk_expander_get_expanded(GTK_EXPANDER(handle));
		if (visible) {
			GtkAllocation allocation;
			gtk_widget_get_allocation(controlHandle, &allocation);

			//454940 change in hierarchy
			/* SWT's calls to gtk_widget_size_allocate and gtk_widget_set_allocation
			 * causes GTK+ to move the clientHandle's SwtFixed down by the size of the label.
			 * These calls can come up from 'shell' and ExpandItem has no control over these calls.
			 * This is an undesired side-effect. Client handle's x & y positions should never
			 * be incremented as this is an internal sub-container.
			 * As of GTK3, the hierarchy is changed, this affected child-size allocation and a fix
			 * is now neccessary.
			 * See also other 454940 notes and similar fix in: 453827 */
			w_rect rect;
#if GTK3
			rect.x = 0;
			rect.y = 0;
#endif
#if GTK2
			rect.x = allocation.x;
			rect.y = allocation.y;
#endif

			if (rect.x != -1 && rect.y != -1) {
				rect.width = allocation.width;
				rect.height = allocation.height;
				gint property = 0;
				gtk_widget_style_get(handle, "focus-line-width", &property,
				NULL);
				rect.y += property * 2;
				rect.height -= property * 2;
				/*
				 * Feature in GTK. When the ExpandBar is resize too small the control
				 * shows up on top of the vertical scrollbar. This happen because the
				 * GtkExpander does not set the size of child smaller than the request
				 * size of its parent and because the control is not parented in the
				 * hierarchy of the GtkScrolledWindow.
				 * The fix is calculate the width ourselves when the scrollbar is visible.
				 */
				_w_control_priv *priv = _W_CONTROL_GET_PRIV(expandbar);
				GtkWidget *scrolledHandle = _w_expandbar_handle_scrolled(
						W_WIDGET(expandbar), priv);
				if (scrolledHandle != 0) {
					GtkWidget *handle = gtk_scrolled_window_get_vscrollbar(
							GTK_SCROLLED_WINDOW(scrolledHandle));
					if (handle != 0) {
						if (gtk_widget_get_visible(handle)) {
							gtk_widget_get_allocation(scrolledHandle,
									&allocation);
							int vScrollBarWidth =
									_w_scrollable_vscrollbar_width(
											W_SCROLLABLE(expandbar), priv);
							rect.width = allocation.width - vScrollBarWidth
									- 2 * _W_EXPANDBAR(expandbar)->spacing;
						}
					}
				}
				rect.y -= yScroll;
				rect.height = WMAX(0, rect.height);
				w_control_set_bounds(control, &rect.pt, &rect.sz);
			}
		}
		w_control_set_visible(control, visible);
	}
}

wresult _w_expanditem_set_control(w_expanditem *expanditem,
		w_control *control) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	if (control != 0) {
		if (w_widget_is_ok(W_WIDGET(control)) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (_W_CONTROL(control)->parent != W_COMPOSITE(parent))
			return W_ERROR_INVALID_PARENT;
	}
	if (control != 0) {
		GtkWidget *handle = _W_EXPANDITEM(expanditem)->handle;
		gboolean expanded = gtk_expander_get_expanded(GTK_EXPANDER(handle));
		w_control_set_visible(control, expanded);
#if GTK3
		_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(control);
		GtkWidget *topHandle = cpriv->widget.handle_top(W_WIDGET(control),
				cpriv);
		GtkWidget *controlHandle = _w_expanditem_handle_control(handle);
		if (controlHandle != 0)
			gtk_container_remove(GTK_CONTAINER(handle), controlHandle);
		g_object_ref(topHandle);
		gtk_widget_unparent(topHandle);
		gtk_container_add(GTK_CONTAINER(handle), topHandle);
		g_object_unref(topHandle);
#endif
	}
	_w_expandbar_layout_items(W_EXPANDBAR(parent));
	return W_TRUE;
}
wresult _w_expanditem_set_expanded(w_expanditem *expanditem, int expanded) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	GtkWidget *handle = _W_EXPANDITEM(expanditem)->handle;
	gtk_expander_set_expanded(GTK_EXPANDER(handle), expanded);
	_w_expandbar_layout_items(W_EXPANDBAR(parent));
	return W_TRUE;
}
wresult _w_expanditem_set_height(w_expanditem *expanditem, int height) {
	if (height < 0)
		return FALSE;
	w_widget *parent = _W_ITEM(expanditem)->parent;
	GtkWidget *handle = _W_EXPANDITEM(expanditem)->handle;
	intptr_t _h = height;
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[GQUARK_ITEM_HEIGTH],
			(void*) _h);
	GtkWidget *controlHandle = _w_expanditem_handle_control(handle);
	gtk_widget_set_size_request(controlHandle, -1, height);
	_w_expandbar_layout_items(W_EXPANDBAR(parent));
	return W_TRUE;
}
wresult _w_expanditem_set_image(w_expanditem *expanditem, int image) {
	wresult result = W_FALSE;
	GtkWidget *handle = _W_EXPANDITEM(expanditem)->handle;
	GtkWidget *box = gtk_expander_get_label_widget(GTK_EXPANDER(handle));
	if (box != 0) {
		_w_widget_handles handles;
		_w_widget_get_handles(box, &handles);
		_w_image_widget *imageHandle = (_w_image_widget*) handles.image;
		if (imageHandle != 0) {
			imageHandle->index = image;
			result = W_TRUE;
		}
	}
	return result;
}
/*
 * expandbar
 */
void _w_expandbar_add_child(w_control *composite, w_widget *child,
		_w_control_priv *priv) {
	_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(child);
	GtkWidget *topHandle = _W_WIDGET_PRIV(cpriv)->handle_top(W_WIDGET(child),
			cpriv);
	GtkWidget *parentHandle = _W_COMPOSITE_PRIV(priv)->handle_parenting(
			W_WIDGET(composite), priv);
	gtk_container_add(GTK_CONTAINER(parentHandle), topHandle);
	_W_FIXED(topHandle)->ignore_fixed = 1;
}
wuint64 _w_expandbar_check_style(w_widget *control, wuint64 style) {
	return style;
}
wresult _w_expandbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	wresult result = _w_control_compute_native_size(widget, handle, e, priv);
	w_size *size = e->size;
	if (size->width == 0 && wHint == W_DEFAULT)
		size->width = DEFAULT_WIDTH;
	if (size->height == 0 && hHint == W_DEFAULT)
		size->height = DEFAULT_HEIGHT;
	int border = gtk_container_get_border_width(GTK_CONTAINER(handle));
	size->width += 2 * border;
	size->height += 2 * border;
	return result;
}
wresult _w_expandbar_create_handle(w_widget *widget, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	GtkWidget *fixedHandle, *handle = 0, *scrolledHandle, *viewport;
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
#if GTK3
	handle = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	if (handle == 0)
		goto _err;
	gtk_box_set_homogeneous(GTK_BOX(handle), FALSE);
#endif
	if ((style & W_VSCROLL) != 0) {
		scrolledHandle = gtk_scrolled_window_new(0, 0);
		if (scrolledHandle == 0)
			goto _err;
		_w_widget_set_control(scrolledHandle, widget);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle),
				GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
		gtk_container_add(GTK_CONTAINER(fixedHandle), scrolledHandle);
		if (GTK_VERSION < VERSION(3, 8, 0)) {
			gtk_container_add(GTK_CONTAINER(scrolledHandle), handle);
		} else {
			gtk_container_add(GTK_CONTAINER(scrolledHandle), handle);
		}
		viewport = gtk_bin_get_child(GTK_BIN(scrolledHandle));
		gtk_viewport_set_shadow_type(GTK_VIEWPORT(viewport), GTK_SHADOW_NONE);
	} else {
		gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	}
	gtk_container_set_border_width(GTK_CONTAINER(handle), 0);
	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
#if GTK3
	//setFontDescription(defaultFont().handle);
#endif
	_W_WIDGET(widget)->handle = handle;
	_w_widget_set_control(handle, widget);
	_w_widget_set_control(fixedHandle, widget);
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
GtkWidget* _w_expandbar_handle_scrolled(w_widget *control,
		_w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_VSCROLL) != 0) {
		return _w_widget_hpp(control, priv);
	} else
		return 0;
}
GtkWidget* _w_expandbar_handle_fixed(w_widget *control, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_VSCROLL) != 0) {
		return _w_widget_hppp(control, priv);
	} else
		return _w_widget_hp(control, priv);
}
wresult _w_expandbar_get_imagelist(w_expandbar *expandbar,
		w_imagelist **imagelist) {
	*imagelist = _W_EXPANDBAR(expandbar)->imagelist;
	return W_TRUE;
}
wresult _w_expandbar_get_item(w_expandbar *expandbar, int index,
		w_expanditem *item) {
	GtkWidget *handle = _W_WIDGET(expandbar)->handle;
	return W_FALSE;
}
wresult _w_expandbar_get_item_count(w_expandbar *expandbar) {
	GtkWidget *handle = _W_WIDGET(expandbar)->handle;
	return W_FALSE;
}
wresult _w_expandbar_get_items(w_expandbar *expandbar, w_iterator *items) {
	GtkWidget *handle = _W_WIDGET(expandbar)->handle;
	return W_FALSE;
}
wresult _w_expandbar_get_spacing(w_expandbar *expandbar) {
	GtkWidget *handle = _W_WIDGET(expandbar)->handle;
	return gtk_box_get_spacing(GTK_BOX(handle));
}
void _w_expandbar_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_composite_hook_events(widget, priv);

}
wresult _w_expandbar_insert_item(w_expandbar *expandbar, w_expanditem *item,
		int index) {
	GtkWidget *expanderHandle = 0, *boxHandle = 0, *labelHandle = 0;
	_w_image_widget *imageHandle = 0;
	expanderHandle = gtk_expander_new("");
	if (expanderHandle == 0)
		goto _err;
#if GTK3
	boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
	if (boxHandle == 0)
		goto _err;
	gtk_box_set_homogeneous(GTK_BOX(boxHandle), FALSE);
#endif
	if (boxHandle == 0)
		goto _err;
	labelHandle = gtk_label_new(NULL);
	if (labelHandle == 0)
		goto _err;
	imageHandle = _w_image_widget_new();
	if (imageHandle == 0)
		goto _err;
	imageHandle->get_image_list = (__get_image_list) _w_expandbar_get_imagelist;
	imageHandle->parent = W_WIDGET(expandbar);
	imageHandle->index = -1;

	gtk_container_add(GTK_CONTAINER(boxHandle), (GtkWidget*) imageHandle);
	gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
	gtk_expander_set_label_widget(GTK_EXPANDER(expanderHandle), boxHandle);
	gtk_widget_set_can_focus(expanderHandle, TRUE);
	gtk_widget_show_all(expanderHandle);
	gtk_container_add(GTK_CONTAINER(_W_WIDGET(expandbar)->handle),
			expanderHandle);
	if (item != 0) {
		_W_WIDGETDATA(item)->clazz = _W_EXPANDBAR_GET_ITEM_CLASS(expandbar);
		_W_ITEM(item)->parent = W_WIDGET(expandbar);
		_W_ITEM(item)->index = index;
		_W_EXPANDITEM(item)->handle = expanderHandle;
	}
	return TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
void _w_expandbar_layout_items_callback(GtkWidget *widget, gpointer data) {
	w_expandbar *expandbar = W_EXPANDBAR(data);
	_w_expanditem_resize_control(expandbar, widget, 0);
}
void _w_expandbar_layout_items(w_expandbar *expandbar) {
	GtkWidget *handle = _W_WIDGET(expandbar)->handle;
	gtk_container_forall(GTK_CONTAINER(handle),
			_w_expandbar_layout_items_callback, expandbar);
}
wresult _w_expandbar_set_imagelist(w_expandbar *expandbar,
		w_imagelist *imagelist) {
	if (imagelist == 0) {
		_W_EXPANDBAR(expandbar)->imagelist = 0;
	} else {
		if (_W_IMAGELIST(imagelist)->images == 0) {
			_W_EXPANDBAR(expandbar)->imagelist = 0;
			return W_ERROR_INVALID_ARGUMENT;
		} else {
			_W_EXPANDBAR(expandbar)->imagelist = imagelist;
		}
	}
	return W_TRUE;
}
wresult _w_expandbar_set_spacing(w_expandbar *expandbar, int spacing) {
	if (spacing < 0)
		return W_FALSE;
	GtkWidget *handle = _W_WIDGET(expandbar)->handle;
	gtk_box_set_spacing(GTK_BOX(handle), spacing);
	gtk_container_set_border_width(GTK_CONTAINER(handle), spacing);
	return W_TRUE;
}

void _w_expandbar_class_init(struct _w_expandbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_EXPANDBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_expandbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_expandbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_expandbar);
	/*
	 * public function
	 */
	clazz->get_imagelist = _w_expandbar_get_imagelist;
	clazz->get_item = _w_expandbar_get_item;
	clazz->get_item_count = _w_expandbar_get_item_count;
	clazz->get_items = _w_expandbar_get_items;
	clazz->get_spacing = _w_expandbar_get_spacing;
	clazz->insert_item = _w_expandbar_insert_item;
	clazz->set_imagelist = _w_expandbar_set_imagelist;
	clazz->set_spacing = _w_expandbar_set_spacing;
	/*
	 * expanditem
	 */
	struct _w_expanditem_class *item = clazz->class_expanditem;
	_w_item_class_init(W_ITEM_CLASS(item));
	W_ITEM_CLASS(item)->get_data = _w_expanditem_get_data;
	W_ITEM_CLASS(item)->get_text = _w_expanditem_get_text;
	W_ITEM_CLASS(item)->set_data = _w_expanditem_set_data;
	W_ITEM_CLASS(item)->set_text = _w_expanditem_set_text;
	item->get_control = _w_expanditem_get_control;
	item->get_expanded = _w_expanditem_get_expanded;
	item->get_header_height = _w_expanditem_get_header_height;
	item->get_height = _w_expanditem_get_height;
	item->set_control = _w_expanditem_set_control;
	item->set_expanded = _w_expanditem_set_expanded;
	item->set_height = _w_expanditem_set_height;
	item->set_image = _w_expanditem_set_image;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.handle_top = _w_expandbar_handle_fixed;
	priv->handle_fixed = _w_expandbar_handle_fixed;
	_W_SCROLLABLE_PRIV(priv)->handle_scrolled = _w_expandbar_handle_scrolled;
	_W_COMPOSITE_PRIV(priv)->handle_parenting = _w_expandbar_handle_fixed;
	_W_COMPOSITE_PRIV(priv)->add_child = _w_expandbar_add_child;
	priv->widget.compute_size = _w_expandbar_compute_size;
	priv->widget.check_style = _w_expandbar_check_style;
	priv->widget.create_handle = _w_expandbar_create_handle;
	priv->widget.hook_events = _w_expandbar_hook_events;
	/*
	 * signals
	 */
	_gtk_signal *signals = priv->widget.signals;
}
