/*
 * Name:        menu.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "shell.h"
#include "menu.h"
#include "toolkit.h"
/*
 * menuitem
 */
GtkWidget* _w_menuitem_get_submenu(GtkWidget *widget) {
	if (GTK_IS_MENU_SHELL(widget)) {
		return widget;
	} else {
		return gtk_menu_item_get_submenu(GTK_MENU_ITEM(widget));
	}
}
wresult _w_menuitem_copy(w_widgetdata *from, w_widgetdata *to) {
	_w_item_copy(from, to);
	_W_MENUITEM(from)->widget = _W_MENUITEM(to)->widget;
	return W_TRUE;
}
wresult _w_menuitem_get_data(w_item *item, void **data) {
	*data = g_object_get_qdata(G_OBJECT(_W_MENUITEM(item)->widget),
			gtk_toolkit->quark[1]);
	return W_TRUE;
}
wresult _w_menuitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	GtkLabel *label;
#if GTK3
	_w_widget_handles handles;
	_w_widget_get_handles(_W_MENUITEM(item)->widget, &handles);
	label = (GtkLabel*) handles.label;
#endif
#if GTK2
	label =  (GtkLabel*)gtk_bin_get_child(GTK_BIN(_W_MENUITEM(item)->widget));
#endif
	if (label != 0) {
		return _gtk_alloc_set_text(alloc, user_data, gtk_label_get_label(label),
				-1, enc);
	}
	return W_FALSE;
}
wresult _w_menuitem_set_data(w_item *item, void *data) {
	g_object_set_qdata(G_OBJECT(_W_MENUITEM(item)->widget),
			gtk_toolkit->quark[1], data);
	return W_TRUE;
}
wresult _w_menuitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	if (text == 0)
		text = "";
	wresult result = W_FALSE;
	GtkLabel *label;
#if GTK3
	_w_widget_handles handles;
	_w_widget_get_handles(_W_MENUITEM(item)->widget, &handles);
	label = (GtkLabel*) handles.label;
#endif
#if GTK2
	label =  (GtkLabel*)gtk_bin_get_child(GTK_BIN(_W_MENUITEM(item)->widget));
#endif
	if (label != 0) {
		int _l = -1;
		char *s = strchr(text, '\t');
		if (s != 0) {
			_l = (s - text);
		}
		int newlength, mnemonic;
		char *str = _gtk_text_fix(text, _l, enc, &newlength, &mnemonic);
		if (str != 0) {
			gtk_label_set_text_with_mnemonic(label, str);
			result = W_TRUE;
		}
		_gtk_text_free(text, str, newlength);
	}
	return result;
}
wresult _w_menuitem_insert(w_menuitem *parent, w_menuitem *item, int style,
		int index) {
	GtkWidget *menuItem, *sub_menu = 0;
#if GTK3
	GtkWidget *labelHandle = 0, *boxHandle = 0;
#endif
	GtkWidget *_p = _w_menuitem_get_submenu(_W_MENUITEM(parent)->widget);
	if (_p == 0)
		return W_ERROR_NO_HANDLES;
	w_menu *menu = (w_menu*) _W_ITEM(parent)->parent;
	int bits = W_CHECK | W_RADIO | W_PUSH | W_SEPARATOR | W_CASCADE;
	switch (style & bits) {
	case W_SEPARATOR:
		menuItem = gtk_separator_menu_item_new();
		break;
	case W_CHECK:
#if GTK3
		menuItem = gtk_check_menu_item_new();
		if (menuItem != 0) {
			labelHandle = gtk_accel_label_new("");
			boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
		}
#endif
#if GTK2
		menuItem = gtk_check_menu_item_new_with_mnemonic("");
#endif
		break;
	case W_RADIO: {
		struct _w_widget_find_child ii;
		memset(&ii, 0, sizeof(ii));
		ii.index = index;
		gtk_container_forall(GTK_CONTAINER(_p),
				(GtkCallback) _w_widget_find_children, &ii);
		GSList *group = NULL;
		if (ii.widget != 0 && GTK_IS_RADIO_MENU_ITEM(ii.widget)) {
			group = gtk_radio_menu_item_get_group(
					GTK_RADIO_MENU_ITEM(ii.widget));
		}
		if (group == NULL
				&& index
						>= 0&& ii.prev != 0 && GTK_IS_RADIO_MENU_ITEM(ii.prev)) {
			group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(ii.prev));
		}
#if GTK3
		menuItem = gtk_radio_menu_item_new(group);
		if (menuItem != 0) {
			labelHandle = gtk_accel_label_new("");
			boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
		}
#endif
#if GTK2
		menuItem = gtk_radio_menu_item_new_with_mnemonic(group, "");
#endif
	}
		break;
	default:
#if GTK3
		menuItem = gtk_menu_item_new();
		if (menuItem != 0) {
			labelHandle = gtk_accel_label_new("");
			boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
			if (style & W_CASCADE) {
				sub_menu = gtk_menu_new();
				if (sub_menu == 0) {
					gtk_widget_destroy(menuItem);
					menuItem = 0;
				} else {
					g_object_set_qdata(G_OBJECT(sub_menu),
							gtk_toolkit->quark[0], menu);
					gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem),
							sub_menu);
				}
			}
		}
#endif
#if GTK2
		menuItem = gtk_image_menu_item_new_with_mnemonic("");
#endif
		break;
	}
	if (menuItem == 0) {
		return W_ERROR_NO_HANDLES;
	}
#if GTK3
	if (labelHandle != 0) {
		if (GTK_VERSION >= VERSION(3, 16, 0)) {
			gtk_label_set_xalign(GTK_LABEL(labelHandle), 0);
			gtk_widget_set_halign(labelHandle, GTK_ALIGN_FILL);
		} else {
			gtk_misc_set_alignment(GTK_MISC(labelHandle), 0, 0);
		}
		gtk_box_pack_end(GTK_BOX(boxHandle), labelHandle, TRUE, TRUE, 0);
		gtk_widget_show(labelHandle);
	}
	if (boxHandle != 0) {
		gtk_container_add(GTK_CONTAINER(menuItem), boxHandle);
		gtk_widget_show_all(boxHandle);
	}
#endif
	g_object_set_qdata(G_OBJECT(menuItem), gtk_toolkit->quark[0], menu);
	if (GTK_IS_MENU_SHELL(_p)) {
		if (index < 0)
			gtk_menu_shell_append(GTK_MENU_SHELL(_p), menuItem);
		else
			gtk_menu_shell_insert(GTK_MENU_SHELL(_p), menuItem, index);
	}
	if (item != 0) {
		W_WIDGETDATA(item)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
		_W_ITEM(item)->parent = W_WIDGET(menu);
		_W_ITEM(item)->index = -1;
		_W_MENUITEM(item)->widget = menuItem;
	}
	gtk_widget_show(menuItem);
	_w_menu_priv *priv = _W_MENU_GET_PRIV(menu);

	_w_widget_connect(menuItem, &priv->signal_activate, FALSE);
	_w_widget_connect(menuItem, &priv->signal_select, FALSE);
	_w_widget_connect(menuItem, &gtk_toolkit->signals[SIGNAL_SHOW_HELP], FALSE);
	return W_OK;
}
GtkAccelGroup* _w_menuitem_get_accel_group(w_menuitem *item) {
	w_widget *menu = _W_ITEM(item)->parent;
	w_shell *shell;
	w_widget_get_shell(menu, &shell);
	return _w_shell_create_accel_group(shell);
}
wresult _w_menuitem_get_accelerator(w_menuitem *item) {
	int accelerator = 0;
	GtkWidget *widget = _W_MENUITEM(item)->widget;
	if (GTK_IS_MENU_SHELL(widget))
		return accelerator;
	GtkAccelLabel *label = GTK_ACCEL_LABEL(gtk_bin_get_child(GTK_BIN(widget)));
	if (label != 0) {
		guint accelerator_key;
		GdkModifierType t;
		gtk_accel_label_get_accel(label, &accelerator_key, &t);
		if (t & GDK_MOD1_MASK) {
			accelerator |= W_ALT;
		}
		if (t & GDK_CONTROL_MASK) {
			accelerator |= W_CTRL;
		}
		if (t & GDK_SHIFT_MASK) {
			accelerator |= W_SHIFT;
		}
		accelerator |= (accelerator_key & 0xFFFF);
	}
	return accelerator;
}
wresult _w_menuitem_get_enabled(w_menuitem *item) {
	return gtk_widget_get_sensitive(_W_MENUITEM(item)->widget);
}
wresult _w_menuitem_get_item(w_menuitem *item, wuint index,
		w_menuitem *sub_item) {
	GtkWidget *_p, *c = 0;
	_p = _w_menuitem_get_submenu(_W_MENUITEM(item)->widget);
	if (_p == 0)
		return W_FALSE;
	if (GTK_IS_CONTAINER(_p)) {
		struct _w_widget_find_child find;
		memset(&find, 0, sizeof(find));
		find.index = index;
		gtk_container_forall(GTK_CONTAINER(_p), _w_widget_find_children, &find);
		c = find.widget;
	}
	if (c == 0) {
		return W_ERROR_CANNOT_GET_ITEM;
	} else {
		W_WIDGETDATA(sub_item)->clazz = W_WIDGETDATA(item)->clazz;
		_W_ITEM(sub_item)->parent = _W_ITEM(item)->parent;
		_W_ITEM(sub_item)->index = -1;
		_W_MENUITEM(sub_item)->widget = c;
		return W_TRUE;
	}
}
wresult _w_menuitem_get_item_count_0(GtkWidget *widget) {
	GtkWidget *_p = _w_menuitem_get_submenu(widget);
	if (_p != 0 && GTK_IS_CONTAINER(_p)) {
		struct _w_widget_find_child find;
		memset(&find, 0, sizeof(find));
		gtk_container_forall(GTK_CONTAINER(_p), _w_widget_children_count,
				&find);
		return find.count;
	}
	return 0;
}
wresult _w_menuitem_get_item_count(w_menuitem *item) {
	return _w_menuitem_get_item_count_0(_W_MENUITEM(item)->widget);
}
/*
 * iterator
 */
typedef struct _w_menuitem_iterator {
	w_basic_iterator base;
	w_widget *menu;
	GList *original;
	GList *list;
	int count;
} _w_menuitem_iterator;
#define _W_MENUITEM_ITERATOR(x) ((_w_menuitem_iterator*)x)
wresult _w_menuitem_iterator_close(w_iterator *_it) {
	_w_menuitem_iterator *it = _W_MENUITEM_ITERATOR(_it);
	if (it->original != 0) {
		g_list_free(it->original);
	}
	return W_TRUE;
}
wresult _w_menuitem_iterator_reset(w_iterator *_it) {
	_w_menuitem_iterator *it = _W_MENUITEM_ITERATOR(_it);
	it->list = it->original;
	return W_TRUE;
}
wresult _w_menuitem_iterator_next(w_iterator *_it, void *obj) {
	_w_menuitem_iterator *it = _W_MENUITEM_ITERATOR(_it);
	if (it->list != 0) {
		GtkWidget *_widget = (GtkWidget*) it->list->data;
		W_WIDGETDATA(obj)->clazz = _W_MENU_GET_ITEM_CLASS(it->menu);
		_W_ITEM( obj)->parent = it->menu;
		_W_ITEM(obj)->index = -1;
		_W_MENUITEM( obj)->widget = _widget;
		it->list = it->list->next;
		return W_TRUE;
	} else {
		W_WIDGETDATA(obj)->clazz = 0;
		return W_FALSE;
	}
}
size_t _w_menuitem_iterator_get_count(w_iterator *_it) {
	_w_menuitem_iterator *it = _W_MENUITEM_ITERATOR(_it);
	if (it->count < 0) {
		int count = 0;
		GList *list = it->original;
		while (list != 0) {
			count++;
			list = list->next;
		}
		it->count = count;
	}
	return it->count;
}
wresult _w_menuitem_iterator_remove(w_iterator *it) {
	return W_FALSE;
}
_w_iterator_class _w_menuitem_iterator_class = { //
		_w_menuitem_iterator_close, //
				_w_menuitem_iterator_next, //
				_w_menuitem_iterator_reset, //
				_w_menuitem_iterator_remove, //
				_w_menuitem_iterator_get_count //
		};
wresult _w_menuitem_get_items(w_menuitem *item, w_iterator *items) {
	GtkWidget *_p = _w_menuitem_get_submenu(_W_MENUITEM(item)->widget);
	if (_p != 0 && GTK_IS_CONTAINER(_p)) {
		_w_menuitem_iterator *it = _W_MENUITEM_ITERATOR(items);
		it->base.clazz = &_w_menuitem_iterator_class;
		it->original = gtk_container_get_children(GTK_CONTAINER(_p));
		it->list = it->original;
		it->count = -1;
		it->menu = _W_ITEM(item)->parent;
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_menuitem_get_id(w_menuitem *item, int mask) {
	void *id = g_object_get_qdata(G_OBJECT(_W_MENUITEM(item)->widget),
			gtk_toolkit->quark[2]);
	if (mask == 0) {
		w_widget *menu = _W_ITEM(item)->parent;
		mask = _W_MENU(menu)->id_mask & 0xFFFF;
	}
	return ((intptr_t) id) & mask;
}
wresult _w_menuitem_get_image(w_menuitem *item, w_image *image) {
	_w_widget_handles handles;
	GtkWidget *widget = _W_MENUITEM(item)->widget;
	_w_widget_get_handles(widget, &handles);
	if (handles.image != 0) {
		GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(handles.image));
		_W_IMAGE(image)->pixbuf = gdk_pixbuf_copy(pixbuf);
	}
	return W_TRUE;
}
wresult _w_menuitem_get_parent_item(w_menuitem *item, w_menuitem *parent) {
	GtkWidget *widget = _W_MENUITEM(item)->widget;
	if (GTK_IS_MENU_SHELL(widget)) {
		return W_FALSE;
	} else {
		GtkWidget *_p = gtk_widget_get_parent(gtk_widget_get_parent(widget));
		if (_p) {
			W_WIDGETDATA(parent)->clazz = W_WIDGETDATA(item)->clazz;
			_W_ITEM(parent)->parent = _W_ITEM(item)->parent;
			_W_ITEM(parent)->index = -1;
			_W_MENUITEM(parent)->widget = _p;
			return W_TRUE;
		} else {
			W_WIDGETDATA(parent)->clazz = 0;
			return W_FALSE;
		}
	}
}
wresult _w_menuitem_get_selection(w_menuitem *item) {
	GtkWidget *widget = _W_MENUITEM(item)->widget;
	if (GTK_IS_CHECK_MENU_ITEM(widget) || GTK_IS_RADIO_MENU_ITEM(widget)) {
		return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
	} else {
		return W_FALSE;
	}
}
wresult _w_menuitem_get_style_0(GtkWidget *widget) {
	if (GTK_IS_SEPARATOR_MENU_ITEM(widget)) {
		return W_SEPARATOR;
	}
	if (GTK_IS_RADIO_MENU_ITEM(widget)) {
		return W_RADIO;
	}
	if (GTK_IS_CHECK_MENU_ITEM(widget)) {
		return W_CHECK;
	}
	if (GTK_IS_MENU_SHELL(widget)) {
		return W_CASCADE;
	}
	GtkWidget *_widget = gtk_menu_item_get_submenu(GTK_MENU_ITEM(widget));
	if (_widget == 0)
		return W_PUSH;
	else
		return W_CASCADE;
}
wresult _w_menuitem_get_style(w_menuitem *item) {
	return _w_menuitem_get_style_0(_W_MENUITEM(item)->widget);
}
wresult _w_menuitem_is_enabled(w_menuitem *item) {
	return gtk_widget_is_sensitive(_W_MENUITEM(item)->widget);
}
wresult _w_menuitem_remove(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_remove_item(w_menuitem *item, wuint index) {
	return W_FALSE;
}
void _w_menuitem_select_radio(w_menuitem *item) {
}
wresult _w_menuitem_set_accelerator_0(w_menuitem *item, wuint accelerator,
		int add) {
	if (GTK_IS_MENU_SHELL(_W_MENUITEM(item)->widget))
		return W_TRUE;
#if GTK3
	_w_widget_handles handles;
	GtkWidget *widget = _W_MENUITEM(item)->widget;
	_w_widget_get_handles(widget, &handles);
	GtkAccelLabel *label = (GtkAccelLabel*) handles.label;
#endif
#if GTK2
	GtkAccelLabel *label = GTK_ACCEL_LABEL(
			gtk_bin_get_child(GTK_BIN(_W_MENUITEM(item)->widget)));
#endif
	if (label != 0) {
		GdkModifierType t = 0;
		if (accelerator & W_ALT)
			t |= GDK_MOD1_MASK;
		if (accelerator & W_CTRL)
			t |= GDK_CONTROL_MASK;
		if (accelerator & W_SHIFT)
			t |= GDK_SHIFT_MASK;
		gtk_accel_label_set_accel(label, accelerator & 0xFFFF, t);

		GtkAccelGroup *accelgroup = _w_menuitem_get_accel_group(item);
		if (accelgroup == 0)
			return W_ERROR_NO_HANDLES;
		if ((accelerator & W_COMMAND) != 0)
			return W_FALSE;
		GdkModifierType mask = 0;
		if ((accelerator & W_ALT) != 0)
			mask |= GDK_MOD1_MASK;
		if ((accelerator & W_SHIFT) != 0)
			mask |= GDK_SHIFT_MASK;
		if ((accelerator & W_CTRL) != 0)
			mask |= GDK_CONTROL_MASK;
		int keysym = accelerator & W_KEY_MASK;
		int newKey = _w_untranslate_key(keysym);
		if (newKey != 0) {
			keysym = newKey;
		} else {
			switch (keysym) {
			case '\r':
				keysym = GDK_KEY_Return;
				break;
			}
		}
		/* When accel_key is zero, it causes GTK warnings */
		if (keysym != 0) {
			if (add) {
				gtk_widget_add_accelerator(_W_MENUITEM(item)->widget,
						"activate", accelgroup, keysym, mask,
						GTK_ACCEL_VISIBLE);
			} else {
				gtk_widget_remove_accelerator(_W_MENUITEM(item)->widget,
						accelgroup, keysym, mask);
			}
		}
	}
	return W_TRUE;
}
wresult _w_menuitem_set_accelerator(w_menuitem *item, wuint accelerator) {
	return _w_menuitem_set_accelerator_0(item, accelerator, W_TRUE);
}
wresult _w_menuitem_set_enabled(w_menuitem *item, int enabled) {
	gtk_widget_set_sensitive(_W_MENUITEM(item)->widget, enabled);
	return W_TRUE;
}
wresult _w_menuitem_set_id(w_menuitem *item, int mask, wushort id) {
	GtkWidget *widget = _W_MENUITEM(item)->widget;
	if (mask == 0) {
		w_widget *menu = _W_ITEM(item)->parent;
		mask = _W_MENU(menu)->id_mask & 0xFFFF;
	}
	intptr_t _id = (intptr_t) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[2]);
	_id |= (id & mask);
	g_object_set_qdata(G_OBJECT(widget), gtk_toolkit->quark[2], (void*) _id);
	return W_TRUE;
}
wresult _w_menuitem_set_image_0(w_menuitem *item, GdkPixbuf *pixbuf) {
	_w_widget_handles handles;
	GtkWidget *widget = _W_MENUITEM(item)->widget;
	if (GTK_IS_SEPARATOR_MENU_ITEM(widget)) {
		return W_FALSE;
	}
	if (pixbuf != 0) {
#if GTK3
		if (!GTK_IS_MENU_ITEM(widget))
			return W_FALSE;
		_w_widget_get_handles(widget, &handles);
		if (handles.image == 0 && handles.box != 0) {
			handles.image = gtk_image_new_from_pixbuf(pixbuf);
			gtk_container_add(GTK_CONTAINER(handles.box), handles.image);
			gtk_box_reorder_child(GTK_BOX(handles.box), handles.image, 0);
		} else {
			gtk_image_set_from_pixbuf(GTK_IMAGE(handles.image), pixbuf);
		}
		if (handles.box == 0)
			return W_ERROR_NO_HANDLES;
#endif
		if (handles.image == 0)
			return W_ERROR_NO_HANDLES;
		gtk_widget_show(handles.image);
	} else {
#if GTK3
		_w_widget_get_handles(widget, &handles);
		if (handles.image != 0 && handles.box != 0) {
			gtk_container_remove(GTK_CONTAINER(handles.box), handles.image);
		}
#endif
	}
	return W_TRUE;
}
wresult _w_menuitem_set_image(w_menuitem *item, w_image *image) {
	GdkPixbuf *pixbuf;
	if (image != 0) {
		if (_W_IMAGE(image)->pixbuf == 0)
			return W_ERROR_INVALID_ARGUMENT;
		pixbuf = _W_IMAGE(image)->pixbuf;
	} else {
		pixbuf = 0;
	}
	return _w_menuitem_set_image_0(item, pixbuf);
}
wresult _w_menuitem_set_image_index(w_menuitem *item, w_imagelist *imagelist,
		int index) {
	w_widget *menu = _W_ITEM(item)->parent;
	GdkPixbuf *pixbuf = w_imagelist_get_pixbuf(imagelist, index);
	return _w_menuitem_set_image_0(item, pixbuf);
}
wresult _w_menuitem_set_selection(w_menuitem *item, int selected) {
	GtkWidget *widget = _W_MENUITEM(item)->widget;
	if (GTK_IS_CHECK_MENU_ITEM(widget)) {
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), selected);
		return W_TRUE;
	}
	return W_FALSE;
}
/*
 * menu
 */
wresult _w_menu_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_widget_is_ok(parent))
		return W_ERROR_INVALID_ARGUMENT;
	if (w_widget_class_id(parent) < _W_CLASS_CONTROL)
		return W_ERROR_INVALID_ARGUMENT;
	_w_widget_priv *ppriv = _W_WIDGET_GET_PRIV(parent);
	ppriv->check_open(parent, _W_CONTROL_PRIV(ppriv));
	_W_WIDGET(widget)->parent = parent;
	_W_WIDGET(widget)->post_event = post_event;
	_w_widget_priv *priv = _W_WIDGET_GET_PRIV(widget);
	_W_WIDGET(widget)->style = priv->check_style(widget, style);
	_W_WIDGET(widget)->state |= STATE_TAB_LIST;
	return priv->create_widget(widget, _W_CONTROL_PRIV(priv));
}
GtkWidget* _w_menu_handle_top(w_widget *control, _w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(control)->handle;
	if ((_W_WIDGET(control)->style & W_BAR) != 0) {
		return gtk_widget_get_parent(handle);
	} else {
		return handle;
	}
}
wresult _w_menu_create_handle(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	GtkWidget *vboxHandle = 0;
	w_shell *_p;

	w_widget_get_shell(widget, &_p);
	if (_p == 0)
		return W_ERROR_INVALID_ARGUMENT;
	w_widget *parent = _W_WIDGET(widget)->parent;
	GtkWidget *handle = 0;
	if ((_W_WIDGET(widget)->style & W_BAR) != 0) {
		if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_SHELL)
			return W_ERROR_INVALID_ARGUMENT;
		handle = gtk_menu_bar_new();
		if (handle == 0)
			goto _err;
		vboxHandle = _W_SHELL_VBOX(_p);
		gtk_container_add(GTK_CONTAINER(vboxHandle), handle);
		gtk_box_set_child_packing(GTK_BOX(vboxHandle), handle, FALSE,
		TRUE, 0, GTK_PACK_START);
	} else {
		if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_CONTROL)
			return W_ERROR_INVALID_ARGUMENT;
		handle = gtk_menu_new();
		if (handle == 0)
			goto _err;
	}
	_W_MENU(widget)->x = -1;
	_W_MENU(widget)->id_mask = 0xFFFF;
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = handle;
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_menu_get_bounds(w_menu *menu, w_rect *bounds) {
	GtkWidget *handle = _W_WIDGET(menu)->handle;
	if (!gtk_widget_get_mapped(handle)) {
		memset(bounds, 0, sizeof(w_rect));
	} else {
		GdkWindow *window = gtk_widget_get_window(handle);
		int origin_x = 0, origin_y = 0;
		gdk_window_get_origin(window, &origin_x, &origin_y);
		GtkAllocation allocation;
		gtk_widget_get_allocation(handle, &allocation);
		bounds->x = origin_x + allocation.x;
		bounds->y = origin_y + allocation.y;
		bounds->width = allocation.width;
		bounds->height = allocation.height;
	}
	return W_TRUE;
}
wresult _w_menu_get_root(w_menu *menu, w_menuitem *rootitem) {
	W_WIDGETDATA(rootitem)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
	_W_ITEM(rootitem)->parent = W_WIDGET(menu);
	_W_ITEM(rootitem)->index = -1;
	_W_MENUITEM(rootitem)->widget = _W_WIDGET(menu)->handle;
	return W_TRUE;
}
wresult _w_menu_get_shell(w_widget *widget, w_shell **shell) {
	w_control *parent = _W_WIDGET(widget)->parent;
	return w_widget_get_shell(W_WIDGET(parent), shell);
}
wresult _w_menu_get_orientation(w_menu *menu) {
	return W_FALSE;
}
wresult _w_menu_get_parent(w_menu *menu, w_control **parent) {
	*parent = _W_WIDGET(menu)->parent;
	return W_TRUE;
}
wresult _w_menu_get_visible(w_menu *menu) {
	GtkWidget *handle = _W_WIDGET(menu)->handle;
	return gtk_widget_get_mapped(handle);
}
void _w_menu_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_widget_hook_events(widget, priv);
	_gtk_signal *signals = gtk_toolkit->signals;
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	_w_widget_connect(handle, &signals[SIGNAL_SHOW], FALSE);
	_w_widget_connect(handle, &signals[SIGNAL_HIDE], FALSE);
	_w_widget_connect(handle, &signals[SIGNAL_SHOW_HELP], FALSE);
	GtkWidget *topHandle = _W_WIDGET_PRIV(priv)->handle_top(widget, priv);
	_w_widget_connect(topHandle, &signals[SIGNAL_DESTROY], TRUE);
}
wresult _w_menu_is_visible(w_menu *menu) {
	return _w_menu_get_visible(menu);
}
wresult _w_menu_set_location(w_menu *menu, w_point *location) {
	if ((_W_WIDGET(menu)->style & (W_BAR | W_DROP_DOWN)) != 0)
		return W_FALSE;
	_W_MENU(menu)->x = location->x;
	_W_MENU(menu)->y = location->y;
	_W_WIDGET(menu)->state |= STATE_MENU_HAS_LOCATION;
	return W_TRUE;
}
wresult _w_menu_set_orientation(w_menu *menu, int orientation) {
	return W_FALSE;
}
void _w_menu_position_func(GtkMenu *menu, gint *x, gint *y, gboolean *push_in,
		gpointer user_data) {
	*push_in = TRUE;
	*x = _W_MENU(user_data)->x;
	*y = _W_MENU(user_data)->y;
}
wresult _w_menu_set_visible_0(w_menu *menu, int visible) {
	w_event_menu ei;
	if ((_W_WIDGET(menu)->style & (W_BAR | W_DROP_DOWN)) != 0)
		return W_FALSE;
	GtkWidget *handle = _W_WIDGET(menu)->handle;
	if (visible) {
		ei.event.type = W_EVENT_SHOW;
		ei.event.platform_event = 0;
		ei.event.widget = W_WIDGET(menu);
		ei.event.time = 0;
		ei.event.data = 0;
		ei.item = 0;
		_w_widget_send_event(W_WIDGET(menu), (w_event*) &ei, W_EVENT_SEND);
		int itemcount = _w_menuitem_get_item_count_0(handle);
		if (itemcount != 0) {
			/*
			 * Feature in GTK. ON_TOP shells will send out
			 * SWT.Deactivate whenever a context menu is shown.
			 * The fix is to prevent the menu from taking focus
			 * when it is being shown in an ON_TOP shell.
			 */
			w_shell *shell;
			w_widget_get_shell(W_WIDGET(menu), &shell);
			if ((_W_WIDGET(shell)->style & W_ON_TOP) != 0) {
				gtk_menu_shell_set_take_focus(GTK_MENU_SHELL(handle), FALSE);
			}
			GtkMenuPositionFunc func =
					(_W_WIDGET(menu)->state & STATE_MENU_HAS_LOCATION != 0) ?
							_w_menu_position_func : 0;
			_W_WIDGET(menu)->state &= ~STATE_MENU_HAS_LOCATION;
			/*
			 * Bug in GTK.  The timestamp passed into gtk_menu_popup is used
			 * to perform an X pointer grab.  It cannot be zero, else the grab
			 * will fail.  The fix is to ensure that the timestamp of the last
			 * event processed is used.
			 */
			gtk_menu_popup(GTK_MENU(handle), 0, 0, func, menu, 0,
					gdk_event_get_time(gdk_event_peek()));
		} else {
			ei.event.type = W_EVENT_HIDE;
			ei.event.platform_event = 0;
			ei.event.widget = W_WIDGET(menu);
			ei.event.time = 0;
			ei.event.data = 0;
			ei.item = 0;
			_w_widget_send_event(W_WIDGET(menu), (w_event*) &ei, W_EVENT_SEND);
		}
	} else {
		gtk_menu_popdown(GTK_MENU(handle));
	}
	return W_TRUE;
}
wresult _w_menu_set_visible(w_menu *menu, int visible) {
	return _w_menu_set_visible_0(menu, visible);
}
/*
 * signals
 */
gboolean _gtk_menu_destroy(w_widget *widget, _w_event_platform *ee,
		_w_control_priv *priv) {
	w_event e;
	if (!w_widget_is_ok(widget))
		return TRUE;
	w_widget *p = _W_WIDGET(widget)->parent;
	if (p != 0) {
		w_link_unlink_0(&_W_WIDGET(widget)->sibling, widget,
				(void**) &_W_WIDGET(p)->first_child);
		_W_WIDGET(p)->children_count--;
	}
	wuint64 style = w_widget_get_style(widget);
	e.type = W_EVENT_DISPOSE;
	e.widget = widget;
	e.data = 0;
	e.time = 0;
	e.platform_event = (w_event_platform*) ee;
	_w_widget_send_event(widget, &e, W_EVENT_SEND);
	widget->clazz = 0;
	if (style & W_FREE_MEMORY) {
		_w_toolkit_registre_free(widget);
	}
	return FALSE;
}
gboolean _gtk_menu_activate(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_menuitem item;
	w_event_menu ei;
	wresult style = _w_menuitem_get_style_0(e->widget);
	if ((style & W_CASCADE) != 0)
		return FALSE;
	if (gdk_pointer_is_grabbed()) {
		gdk_pointer_ungrab(0);
	}
	W_WIDGETDATA(&item)->clazz = _W_MENU_GET_ITEM_CLASS(widget);
	_W_ITEM(&item)->parent = widget;
	_W_ITEM(&item)->index = -1;
	_W_MENUITEM(&item)->widget = e->widget;
	/*
	 * Bug in GTK.  When an ancestor menu is disabled and
	 * the user types an accelerator key, GTK delivers the
	 * the activate signal even though the menu item cannot
	 * be invoked using the mouse.  The fix is to ignore
	 * activate signals when an ancestor menu is disabled.
	 */
	if (!_w_menuitem_is_enabled(&item))
		return 0;
	if ((style & W_RADIO) != 0) {
		if ((_W_WIDGET(widget)->style & W_NO_RADIO_GROUP) == 0) {
			_w_menuitem_select_radio(&item);
		}
	}
	ei.event.type = W_EVENT_ITEM_SELECTION;
	ei.event.platform_event = _EVENT_PLATFORM(e);
	ei.event.widget = widget;
	ei.event.time = 0;
	ei.event.data = 0;
	ei.item = (w_menuitem*) &item;
	_w_widget_send_event(widget, (w_event*) &ei, W_EVENT_SEND);
	return FALSE;
}
gboolean _gtk_menu_hide(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_menu ei;
	ei.event.type = W_EVENT_HIDE;
	ei.event.platform_event = _EVENT_PLATFORM(e);
	ei.event.widget = widget;
	ei.event.time = 0;
	ei.event.data = 0;
	ei.item = 0;
	_w_widget_send_event(widget, (w_event*) &ei, W_EVENT_SEND);
	return FALSE;
}
gboolean _gtk_menu_select(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_menuitem item;
	w_event_menu ei;
	W_WIDGETDATA(&item)->clazz = _W_MENU_GET_ITEM_CLASS(widget);
	_W_ITEM(&item)->parent = widget;
	_W_ITEM(&item)->index = -1;
	_W_MENUITEM(&item)->widget = e->widget;
	ei.event.type = W_EVENT_ARM;
	ei.event.platform_event = _EVENT_PLATFORM(e);
	ei.event.widget = widget;
	ei.event.time = 0;
	ei.event.data = 0;
	ei.item = (w_menuitem*) &item;
	_w_widget_send_event(widget, (w_event*) &ei, W_EVENT_SEND);
	return FALSE;
}
gboolean _gtk_menu_show(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_menu ei;
	ei.event.type = W_EVENT_SHOW;
	ei.event.platform_event = _EVENT_PLATFORM(e);
	ei.event.widget = widget;
	ei.event.time = 0;
	ei.event.data = 0;
	ei.item = 0;
	_w_widget_send_event(widget, (w_event*) &ei, W_EVENT_SEND);
	return FALSE;
}
gboolean _gtk_menu_show_help(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_menuitem item;
	w_event_menu ei;
	W_WIDGETDATA(&item)->clazz = _W_MENU_GET_ITEM_CLASS(widget);
	_W_ITEM(&item)->parent = widget;
	_W_ITEM(&item)->index = -1;
	_W_MENUITEM(&item)->widget = e->widget;
	ei.event.type = W_EVENT_HELP;
	ei.event.platform_event = _EVENT_PLATFORM(e);
	ei.event.widget = widget;
	ei.event.time = 0;
	ei.event.data = 0;
	ei.item = (w_menuitem*) &item;
	_w_widget_send_event(widget, (w_event*) &ei, W_EVENT_SEND);
	return FALSE;
}

void _w_menu_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_menu_class *clazz) {
	if (classId == _W_CLASS_MENU) {
		W_WIDGET_CLASS(clazz)->platformPrivate = &gtk_toolkit->class_menu_priv;
	}
	_w_widget_class_init(toolkit, classId, W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_MENU;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_menu_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_menu);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_menu);
	/*
	 * functions of menu
	 */
	W_WIDGET_CLASS(clazz)->create = _w_menu_create;
	W_WIDGET_CLASS(clazz)->get_shell = _w_menu_get_shell;
	clazz->get_bounds = _w_menu_get_bounds;
	clazz->get_root = _w_menu_get_root;
	clazz->get_orientation = _w_menu_get_orientation;
	clazz->get_parent = _w_menu_get_parent;
	clazz->get_visible = _w_menu_get_visible;
	clazz->is_visible = _w_menu_is_visible;
	clazz->set_location = _w_menu_set_location;
	clazz->set_orientation = _w_menu_set_orientation;
	clazz->set_visible = _w_menu_set_visible;
	/*
	 * function of menu item
	 */
	struct _w_menuitem_class *item = clazz->class_menuitem;
	_w_item_class_init(W_ITEM_CLASS(item));
	W_WIDGETDATA_CLASS(item)->toolkit = toolkit;
	W_WIDGETDATA_CLASS(item)->copy = _w_menuitem_copy;
	W_ITEM_CLASS(item)->get_data = _w_menuitem_get_data;
	W_ITEM_CLASS(item)->get_text = _w_menuitem_get_text;
	W_ITEM_CLASS(item)->set_data = _w_menuitem_set_data;
	W_ITEM_CLASS(item)->set_text = _w_menuitem_set_text;
	item->insert = _w_menuitem_insert;
	item->get_accelerator = _w_menuitem_get_accelerator;
	item->get_enabled = _w_menuitem_get_enabled;
	item->get_item = _w_menuitem_get_item;
	item->get_item_count = _w_menuitem_get_item_count;
	item->get_items = _w_menuitem_get_items;
	item->get_id = _w_menuitem_get_id;
	item->get_image = _w_menuitem_get_image;
	item->get_parent_item = _w_menuitem_get_parent_item;
	item->get_selection = _w_menuitem_get_selection;
	item->get_style = _w_menuitem_get_style;
	item->is_enabled = _w_menuitem_is_enabled;
	item->remove = _w_menuitem_remove;
	item->remove_item = _w_menuitem_remove_item;
	item->set_accelerator = _w_menuitem_set_accelerator;
	item->set_enabled = _w_menuitem_set_enabled;
	item->set_id = _w_menuitem_set_id;
	item->set_image = _w_menuitem_set_image;
	item->set_image_index = _w_menuitem_set_image_index;
	item->set_selection = _w_menuitem_set_selection;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_MENU) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		_W_WIDGET_PRIV(priv)->create_handle = _w_menu_create_handle;
		_W_WIDGET_PRIV(priv)->hook_events = _w_menu_hook_events;
		_W_WIDGET_PRIV(priv)->handle_top = _w_menu_handle_top;
		_gtk_signal *signal = &_W_MENU_PRIV(priv)->signal_activate;
		signal->msg = SIGNAL_ACTIVATE;
		signal->name = "activate";
		signal->number_of_args = 2;
		signal = &_W_MENU_PRIV(priv)->signal_select;
		signal->msg = SIGNAL_SELECT;
		signal->name = "select";
		signal->number_of_args = 2;
		/*
		 * signals
		 */
		_gtk_signal_fn *signals = _W_WIDGET_PRIV(priv)->signals;
		signals[SIGNAL_DESTROY] = _gtk_menu_destroy;
		signals[SIGNAL_ACTIVATE] = _gtk_menu_activate;
		signals[SIGNAL_HIDE] = _gtk_menu_hide;
		signals[SIGNAL_SELECT] = _gtk_menu_select;
		signals[SIGNAL_SHOW] = _gtk_menu_show;
		signals[SIGNAL_SHOW_HELP] = _gtk_menu_show_help;
	}
}
