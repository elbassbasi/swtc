/*
 * combobox.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "combobox.h"
#include "../widgets/toolkit.h"
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#define LIMIT 0xFFFF
/*
 * comboitem
 */
wresult _w_comboitem_get_data(w_item *item, void **data) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
#if GTK3
	GtkTreeModel *modelHandle = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	*data = 0;
	gtk_tree_model_get(modelHandle, &_W_COMBOITEM(item)->iter, COLUMN_USER_DATA,
			data, -1);
#endif
	return W_TRUE;
}
wresult _w_comboitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
#if GTK3
	GtkTreeModel *modelHandle = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	char *s = 0;
	gtk_tree_model_get(modelHandle, &_W_COMBOITEM(item)->iter, COLUMN_TEXT, &s,
			-1);
	_gtk_alloc_set_text(alloc, user_data, s, -1, enc);
	g_free(s);
#endif
	return W_TRUE;
}
wresult _w_comboitem_set_data(w_item *item, void *data) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
#if GTK3
	GtkTreeModel *modelHandle = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	gtk_list_store_set(GTK_LIST_STORE(modelHandle), &_W_COMBOITEM(item)->iter,
			COLUMN_USER_DATA, data, -1);
#endif
	return W_TRUE;
}
wresult _w_comboitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
#if GTK3
	GtkTreeModel *modelHandle = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	int newlength, mnemonic;
	char *s = _gtk_text_fix(text, length, enc, &newlength, &mnemonic);
	gtk_list_store_set(GTK_LIST_STORE(modelHandle), &_W_COMBOITEM(item)->iter,
			COLUMN_TEXT, s, -1);
	_gtk_text_free(text, s, newlength);
#endif
	return W_TRUE;
}
wresult _w_comboitem_get_image(w_comboitem *item) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
	int image = -1;
#if GTK3
	GtkTreeModel *modelHandle = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	gtk_tree_model_get(modelHandle, &_W_COMBOITEM(item)->iter, COLUMN_IMAGE,
			&image, -1);
#endif
	return image;
}
wresult _w_comboitem_set_image(w_comboitem *item, int image) {
	w_widget *parent = _W_ITEM(item)->parent;
	GtkWidget *handle = _W_WIDGET(parent)->handle;
#if GTK3
	GtkTreeModel *modelHandle = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	gtk_list_store_set(GTK_LIST_STORE(modelHandle), &_W_COMBOITEM(item)->iter,
			COLUMN_IMAGE, image, -1);
#endif
	return W_TRUE;
}
/*
 * combobox
 */
wuint64 _w_combobox_check_style(w_widget *control, wuint64 style) {
	/*
	 * Feature in Windows.  It is not possible to create
	 * a combo box that has a border using Windows style
	 * bits.  All combo boxes draw their own border and
	 * do not use the standard Windows border styles.
	 * Therefore, no matter what style bits are specified,
	 * clear the BORDER bits so that the SWT style will
	 * match the Windows widget.
	 *
	 * The Windows behavior is currently implemented on
	 * all platforms.
	 */
	style &= ~W_BORDER;

	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	style &= ~(W_HSCROLL | W_VSCROLL);
	style = _w_widget_check_bits(style, W_DROP_DOWN, W_SIMPLE, 0, 0, 0, 0);
	if ((style & W_SIMPLE) != 0)
		return style & ~W_READ_ONLY;
	return style;
}
wresult _w_combobox_clear_selection(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
	if (entryHandle != 0) {
		int position = gtk_editable_get_position(GTK_EDITABLE(entryHandle));
		gtk_editable_select_region(GTK_EDITABLE(entryHandle), position,
				position);
	}
	return W_TRUE;
}
void _w_combobox_clear_clear_text(w_combobox *combo) {
	GtkComboBox *handle = GTK_COMBO_BOX(_W_WIDGET(combo)->handle);
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_CHANGED);
	if ((_W_WIDGET(combo)->style & W_READ_ONLY) != 0) {
		int index = gtk_combo_box_get_active(handle);
		if (index != -1) {
			GtkTreeModel *modelHandle = gtk_combo_box_get_model(handle);
			GtkTreeIter iter;
			char *ptr = 0;
			gtk_tree_model_iter_nth_child(modelHandle, &iter, 0, index);
			gtk_tree_model_get(modelHandle, &iter, 0, &ptr, -1);
			if (ptr != 0 && strlen(ptr) > 0) {
				w_event e;
				e.type = W_EVENT_MODIFY;
				e.platform_event = 0;
				e.time = 0;
				e.widget = W_WIDGET(combo);
				e.data = 0;
				_w_widget_send_event(W_WIDGET(combo), &e);
			}
			g_free(ptr);
		}
	} else {
		GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
		gtk_entry_set_text(GTK_ENTRY(entryHandle), "");
	}
	gtk_combo_box_set_active(handle, -1);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_CHANGED);
}
wresult _w_combobox_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	wresult result;
#if GTK3
	result = _w_control_compute_native_size(widget, handle, e, priv);
#endif
	return result;
}
wresult _w_combobox_copy(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
	if (entryHandle != 0)
		gtk_editable_copy_clipboard(GTK_EDITABLE(entryHandle));
	return W_TRUE;
}
wresult _w_combobox_create_handle(w_widget *widget, _w_control_priv *priv) {
	GtkWidget *fixedHandle, *cellHandle = 0, *entryHandle = 0, *handle = 0;
	GtkCellRenderer *textRenderer = 0, *pixbufRenderer = 0;
	GtkTreeModel *model = 0;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_MENU;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	model = GTK_TREE_MODEL(gtk_list_store_newv(COLUMN_COUNT, gtk_store_types));
	if (model == 0)
		goto _err;
	if ((_W_WIDGET(widget)->style & W_READ_ONLY) != 0) {
#if GTK3
		handle = gtk_combo_box_new_with_model(model);
#endif
		if (handle == 0)
			goto _err;
		cellHandle = gtk_bin_get_child(GTK_BIN(handle));
		if (cellHandle == 0)
			goto _err;
		// Setting wrap width has the side effect of removing the whitespace on top in popup bug#438992
		gtk_combo_box_set_wrap_width(GTK_COMBO_BOX(handle), 1);
	} else {
#if GTK3
		handle = gtk_combo_box_new_with_model_and_entry(model);
#endif
		if (handle == 0)
			goto _err;
		gtk_combo_box_set_entry_text_column(GTK_COMBO_BOX(handle), COLUMN_TEXT);
		entryHandle = gtk_bin_get_child(GTK_BIN(handle));
		if (entryHandle == 0)
			goto _err;
#if GTK3
		//imContext = imContextLast();
#endif
	}
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	gtk_cell_layout_clear(GTK_CELL_LAYOUT(handle));
	/* icon cell */
	pixbufRenderer = _w_pixbuf_renderer_new(widget);
	if (pixbufRenderer == 0)
		goto _err;
	_w_widget_set_control(pixbufRenderer, widget);
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(handle), pixbufRenderer, FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(handle), pixbufRenderer,
			"pixbuf_index", COLUMN_IMAGE, NULL);
	_W_COMBOBOX(widget)->pixbufrenderer = pixbufRenderer;
	gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(handle), pixbufRenderer,
			_w_combobox_cell_layout_data_func, widget, FALSE);

	textRenderer = _w_text_renderer_new(widget);
	if (textRenderer == 0)
		goto _err;
	_w_widget_set_control(textRenderer, widget);
	/*
	 * Feature in GTK. In order to make a read only combo box the same
	 * height as an editable combo box the ypad must be set to 0. In
	 * versions 2.4.x of GTK, a pad of 0 will clip some letters. The
	 * fix is to set the pad to 1.
	 */
	int pad = 0;
	g_object_set(G_OBJECT(textRenderer), "ypad", pad, NULL);
	/*
	 * Feature in GTK.  In version 2.4.9 of GTK, a warning is issued
	 * when a call to gtk_cell_layout_clear() is made. The fix is to hide
	 * the warning.
	 */
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(handle), textRenderer, FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(handle), textRenderer,
			"text", COLUMN_TEXT, NULL);
	gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(handle), textRenderer,
			_w_combobox_cell_layout_data_func, widget, FALSE);

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
wresult _w_combobox_cut(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
	if (entryHandle != 0)
		gtk_editable_cut_clipboard(GTK_EDITABLE(entryHandle));
	return W_TRUE;
}
wresult _w_combobox_deselect(w_combobox *combo, int index) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	if (gtk_combo_box_get_active(GTK_COMBO_BOX(handle)) == index) {
		_w_combobox_clear_clear_text(combo);
	}
	return W_TRUE;
}
wresult _w_combobox_deselect_all(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	_w_combobox_clear_clear_text(combo);
	return W_TRUE;
}
wresult _w_combobox_get_imagelist(w_combobox *combo, w_imagelist **imagelist) {
	*imagelist = _W_COMBOBOX(combo)->imagelist;
	return W_TRUE;
}
wresult _w_combobox_get_item(w_combobox *combo, int index, w_comboitem *item) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	GtkTreeIter *iter = &_W_COMBOITEM(item)->iter;
	if (gtk_tree_model_iter_nth_child(model, iter, NULL, index)) {
		_W_WIDGETDATA(item)->clazz = _W_COMBOBOX_GET_ITEM_CLASS(combo);
		_W_ITEM(item)->parent = W_WIDGET(combo);
		_W_ITEM(item)->index = index;
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _w_combobox_get_item_count(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	return gtk_tree_model_iter_n_children(model, NULL);
}
wresult _w_combobox_get_item_height(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_get_items(w_combobox *combo, w_iterator *items) {
	return W_FALSE;
}
wresult _w_combobox_get_list_visible(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkWidget *popupHandle = 0;
	return popupHandle != 0 && gtk_widget_get_visible(popupHandle);
}
wresult _w_combobox_get_selection(w_combobox *combo, w_point *pt) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	if ((_W_WIDGET(combo)->style & W_READ_ONLY) != 0) {
		int length = 0;
		int index = gtk_combo_box_get_active(GTK_COMBO_BOX(handle));
		if (index != -1) {
			GtkTreeModel *model = gtk_combo_box_get_model(
					GTK_COMBO_BOX(handle));
			GtkTreeIter iter;
			if (gtk_tree_model_iter_nth_child(model, &iter, NULL, index)) {
				char *s = 0;
				gtk_tree_model_get(model, &iter, COLUMN_TEXT, &s, -1);
				if (s != 0) {
					length = strlen(s);
				}
			}
		}
		pt->x = 0;
		pt->y = length;
	} else {
		GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
		int start = 0;
		int end = 0;
		if (entryHandle != 0) {
			gtk_editable_get_selection_bounds(GTK_EDITABLE(entryHandle), &start,
					&end);
			//const gchar* ptr = gtk_entry_get_text(GTK_ENTRY(entryHandle));
		}
		pt->x = start;
		pt->y = end;
	}
	return W_TRUE;
}
wresult _w_combobox_get_selection_index(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	return gtk_combo_box_get_active(GTK_COMBO_BOX(handle));
}
wresult _w_combobox_get_text(w_combobox *combo, w_alloc alloc, void *user_data,
		int enc) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
	const gchar *str = 0;
	if (GTK_IS_ENTRY(entryHandle)) {
		str = gtk_entry_get_text(GTK_ENTRY(entryHandle));
	} else {
		GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
		GtkTreeIter iter;
		if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(handle), &iter)) {
			gtk_tree_model_get(model, &iter, 0, &str, -1);
		}
	}
	return _gtk_alloc_set_text(alloc, user_data, str, -1, enc);
}
wresult _w_combobox_get_text_height(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkRequisition requisition;
	gtk_widget_size_request(handle, &requisition);
#if GTK3
	return requisition.height;
#endif
}
wresult _w_combobox_get_text_limit(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
	int limit =
			entryHandle != 0 ?
					gtk_entry_get_max_length(GTK_ENTRY(entryHandle)) : 0;
	return limit == 0 ? LIMIT : limit;
}
wresult _w_combobox_get_visible_item_count(w_combobox *combo) {
	return _W_COMBOBOX(combo)->visibleCount;
}
void _w_combobox_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_composite_hook_events(widget, priv);

}
wresult _w_combobox_indexof(w_combobox *combo, const char *string, int length,
		int enc, int start) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	GtkTreeIter iter;
	int newlength, mnemonic, index = -1, i = 0;
	char *str = _gtk_text_fix(string, length, enc, &newlength, &mnemonic);
	if (str != 0) {
		char *s = 0;
		gtk_tree_model_iter_children(model, &iter, NULL);
		while (gtk_tree_model_iter_next(model, &iter)) {
			gtk_tree_model_get(model, &iter, COLUMN_TEXT, &s, -1);
			if (!strcmp(s, str)) {
				index = i;
				break;
			}
			g_free(s);
			i++;
		}
	}
	_gtk_text_free(string, str, newlength);
	return index;
}
wresult _w_combobox_insert_item(w_combobox *combo, w_comboitem *item,
		int index) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkTreeIter *iter, tmp;
	if (item == 0) {
		iter = &tmp;
	} else {
		iter = &_W_COMBOITEM(item)->iter;
	}
#if GTK3
	GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	gtk_list_store_insert(GTK_LIST_STORE(model), iter, index);
	gtk_list_store_set(GTK_LIST_STORE(model), iter, COLUMN_IMAGE, -1, -1);
#endif
	if (item != 0) {
		_W_WIDGETDATA(item)->clazz = _W_COMBOBOX_GET_ITEM_CLASS(combo);
		_W_ITEM(item)->parent = W_WIDGET(combo);
		_W_ITEM(item)->index = index;
	}
	return W_TRUE;
}
wresult _w_combobox_paste(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
	if (entryHandle != 0)
		gtk_editable_paste_clipboard(GTK_EDITABLE(entryHandle));
	return W_TRUE;
}
wresult _w_combobox_remove_index(w_combobox *combo, int index) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	if (gtk_combo_box_get_active(GTK_COMBO_BOX(handle)) == index)
		_w_combobox_clear_clear_text(combo);
#if GTK3
	gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(handle), index);
#endif
	return W_TRUE;
}
wresult _w_combobox_remove_range(w_combobox *combo, int start, int end) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	if (start > end)
		return W_FALSE;
	int index = gtk_combo_box_get_active(GTK_COMBO_BOX(handle));
	if (start <= index && index <= end)
		_w_combobox_clear_clear_text(combo);
	for (int i = end; i >= start; i--) {
#if GTK3
		gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(handle), i);
#endif
	}
	return W_TRUE;
}
wresult _w_combobox_remove_string(w_combobox *combo, const char *string,
		int length, int enc) {
	int index = _w_combobox_indexof(combo, string, length, enc, 0);
	if (index >= 0) {
		_w_combobox_remove_index(combo, index);
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_combobox_remove_all(w_combobox *combo) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	_w_combobox_clear_clear_text(combo);
#if GTK3
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(handle));
#endif
	return W_TRUE;
}
wresult _w_combobox_select(w_combobox *combo, int index) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	int selected = gtk_combo_box_get_active(GTK_COMBO_BOX(handle));
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_CHANGED);
	gtk_combo_box_set_active(GTK_COMBO_BOX(handle), index);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_CHANGED);
	if ((_W_WIDGET(combo)->style & W_READ_ONLY) != 0 && selected != index) {
		/*
		 * Feature in GTK. Read Only combo boxes do not get a chance to send out a
		 * Modify event in the gtk_changed callback. The fix is to send a Modify event
		 * here.
		 */
		w_event e;
		e.type = W_EVENT_MODIFY;
		e.platform_event = 0;
		e.time = 0;
		e.widget = W_WIDGET(combo);
		e.data = 0;
		_w_widget_send_event(W_WIDGET(combo), &e);
	}
	return W_TRUE;
}
wresult _w_combobox_set_imagelist(w_combobox *combo, w_imagelist *imagelist) {
	if (imagelist == 0) {
		_W_COMBOBOX(combo)->imagelist = 0;
		gtk_cell_renderer_set_fixed_size(_W_COMBOBOX(combo)->pixbufrenderer, 0,
				0);
	} else {
		if (_W_IMAGELIST(imagelist)->images == 0) {
			_W_COMBOBOX(combo)->imagelist = 0;
			gtk_cell_renderer_set_fixed_size(
			_W_COMBOBOX(combo)->pixbufrenderer, 0, 0);
			return W_ERROR_INVALID_ARGUMENT;
		} else {
			_W_COMBOBOX(combo)->imagelist = imagelist;
			gtk_cell_renderer_set_fixed_size(
			_W_COMBOBOX(combo)->pixbufrenderer,
			_W_IMAGELIST(imagelist)->images->width,
			_W_IMAGELIST(imagelist)->images->height);
		}
	}
	W_CONTROL_GET_CLASS(combo)->update(W_CONTROL(combo));
	return W_TRUE;
}
wresult _w_combobox_set_list_visible(w_combobox *combo, int visible) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	if (visible) {
		gtk_combo_box_popup(GTK_COMBO_BOX(handle));
	} else {
		gtk_combo_box_popdown(GTK_COMBO_BOX(handle));
	}
	return W_TRUE;
}
wresult _w_combobox_set_selection(w_combobox *combo, w_point *selection) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	if (selection == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(combo)->style & W_READ_ONLY) != 0)
		return W_TRUE;
	GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
	if (entryHandle != 0) {
		//gchar *ptr = gtk_entry_get_text(GTK_ENTRY(entryHandle));
		int start = selection->x;
		int end = selection->y;
		gtk_editable_set_position(GTK_EDITABLE(entryHandle), start);
		gtk_editable_select_region(GTK_EDITABLE(entryHandle), start, end);
	}
	return W_TRUE;
}
wresult _w_combobox_set_text(w_combobox *combo, const char *string, int length,
		int enc) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	if ((_W_WIDGET(combo)->style & W_READ_ONLY) != 0) {
		int index = _w_combobox_indexof(combo, string, length, enc, 0);
		if (index >= 0) {
			_w_combobox_select(combo, index);
		}
		return W_TRUE;
	}
	GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_CHANGED);
	g_signal_handlers_block_matched(entryHandle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
			0, (void*) SIGNAL_CHANGED);
	g_signal_handlers_block_matched(entryHandle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
			0, (void*) SIGNAL_DELETE_TEXT);
	g_signal_handlers_block_matched(entryHandle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
			0, (void*) SIGNAL_INSERT_TEXT);
	int newlength, mnemonic;
	char *s = _gtk_text_fix(string, length, enc, &newlength, &mnemonic);
	gtk_entry_set_text(GTK_ENTRY(entryHandle), s);
	_gtk_text_free(string, s, newlength);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_CHANGED);
	g_signal_handlers_unblock_matched(entryHandle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
			0, (void*) SIGNAL_CHANGED);
	g_signal_handlers_unblock_matched(entryHandle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
			0, (void*) SIGNAL_DELETE_TEXT);
	g_signal_handlers_unblock_matched(entryHandle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
			0, (void*) SIGNAL_INSERT_TEXT);
	w_event e;
	e.type = W_EVENT_MODIFY;
	e.platform_event = 0;
	e.time = 0;
	e.widget = W_WIDGET(combo);
	e.data = 0;
	_w_widget_send_event(W_WIDGET(combo), &e);
	return W_TRUE;
}
wresult _w_combobox_set_text_limit(w_combobox *combo, int limit) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	if (limit == 0)
		return W_ERROR_CANNOT_BE_ZERO;
	GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
	if (entryHandle != 0)
		gtk_entry_set_max_length(GTK_ENTRY(entryHandle), limit);
	return W_TRUE;
}
wresult _w_combobox_set_visible_item_count(w_combobox *combo, int count) {
	GtkWidget *handle = _W_WIDGET(combo)->handle;
	if (count < 0)
		return W_TRUE;
	_W_COMBOBOX(combo)->visibleCount = count;
	return W_TRUE;
}
void _w_combobox_cell_layout_data_func(GtkCellLayout *cell_layout,
		GtkCellRenderer *cell, GtkTreeModel *tree_model, GtkTreeIter *iter,
		gpointer data) {
	_w_cell_renderer *c = (_w_cell_renderer*) cell;
	/*
	 * tree column
	 */
	_W_WIDGETDATA(&c->column)->clazz = 0;
	/*
	 * tree item
	 */
	_W_WIDGETDATA(&c->treeitem)->clazz = _W_COMBOBOX_GET_ITEM_CLASS(data);
	_W_ITEM(&c->treeitem)->parent = W_WIDGET(data);
	_W_ITEM(&c->treeitem)->index = -1;
	memcpy(&_W_COMBOITEM(&c->treeitem)->iter, iter, sizeof(GtkTreeIter));
}
void _w_combobox_renderer_render(w_widget *widget, _w_control_priv *priv,
		GtkCellRenderer *_cell, cairo_t *cr, GtkWidget *gtkwidget,
		const GdkRectangle *background_area, const GdkRectangle *cell_area,
		GtkCellRendererState flags) {
	_w_cell_renderer *cell = (_w_cell_renderer*) _cell;
	_w_cell_renderer_class *clazz =
			(_w_cell_renderer_class*) GTK_CELL_RENDERER_GET_CLASS(_cell);
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	GtkTreeModel *modelHandle = gtk_combo_box_get_model(GTK_COMBO_BOX(handle));
	GtkTreeIter *iter = &_W_COMBOITEM(&cell->treeitem)->iter;
	if (GTK_IS_CELL_RENDERER_TEXT(_cell)) {
		const char *text = 0;
		gtk_tree_model_get(modelHandle, iter, COLUMN_TEXT, &text, -1);
		g_object_set(cell, "text", text, NULL);
	}
	if (GTK_IS_CELL_RENDERER_PIXBUF(_cell)) {
		int index;
		GdkPixbuf *pixbuf = 0;
		gtk_tree_model_get(modelHandle, iter, COLUMN_IMAGE, &index, -1);
		if (index >= 0) {
			w_imagelist *imagelist = _W_COMBOBOX(widget)->imagelist;
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
void _w_combobox_renderer_get_preferred_width(w_widget *widget,
		_w_control_priv *priv, GtkCellRenderer *_cell, GtkWidget *gtkwidget,
		gint *minimum_size, gint *natural_size) {
	_w_cell_renderer *cell = (_w_cell_renderer*) _cell;
	_w_cell_renderer_class *clazz =
			(_w_cell_renderer_class*) GTK_CELL_RENDERER_GET_CLASS(_cell);
	clazz->default_get_preferred_width(_cell, gtkwidget, minimum_size,
			natural_size);
}
#endif
void _w_combobox_class_init(struct _w_combobox_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMBOBOX;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_combobox_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_combobox);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_combobox);
	/*
	 * public
	 */
	clazz->clear_selection = _w_combobox_clear_selection;
	clazz->copy = _w_combobox_copy;
	clazz->cut = _w_combobox_cut;
	clazz->deselect = _w_combobox_deselect;
	clazz->deselect_all = _w_combobox_deselect_all;
	clazz->get_imagelist = _w_combobox_get_imagelist;
	clazz->get_item = _w_combobox_get_item;
	clazz->get_item_count = _w_combobox_get_item_count;
	clazz->get_item_height = _w_combobox_get_item_height;
	clazz->get_items = _w_combobox_get_items;
	clazz->get_list_visible = _w_combobox_get_list_visible;
	clazz->get_selection = _w_combobox_get_selection;
	clazz->get_selection_index = _w_combobox_get_selection_index;
	clazz->get_text = _w_combobox_get_text;
	clazz->get_text_height = _w_combobox_get_text_height;
	clazz->get_text_limit = _w_combobox_get_text_limit;
	clazz->get_visible_item_count = _w_combobox_get_visible_item_count;
	clazz->indexof = _w_combobox_indexof;
	clazz->insert_item = _w_combobox_insert_item;
	clazz->paste = _w_combobox_paste;
	clazz->remove_index = _w_combobox_remove_index;
	clazz->remove_range = _w_combobox_remove_range;
	clazz->remove_string = _w_combobox_remove_string;
	clazz->remove_all = _w_combobox_remove_all;
	clazz->select = _w_combobox_select;
	clazz->set_imagelist = _w_combobox_set_imagelist;
	clazz->set_list_visible = _w_combobox_set_list_visible;
	clazz->set_selection = _w_combobox_set_selection;
	clazz->set_text = _w_combobox_set_text;
	clazz->set_text_limit = _w_combobox_set_text_limit;
	clazz->set_visible_item_count = _w_combobox_set_visible_item_count;
	/*
	 * combo item
	 */
	struct _w_comboitem_class *item = W_COMBOITEM_CLASS(clazz->class_comboitem);
	_w_item_class_init(W_ITEM_CLASS(item));
	W_ITEM_CLASS(item)->get_data = _w_comboitem_get_data;
	W_ITEM_CLASS(item)->get_text = _w_comboitem_get_text;
	W_ITEM_CLASS(item)->set_data = _w_comboitem_set_data;
	W_ITEM_CLASS(item)->set_text = _w_comboitem_set_text;
	item->get_image = _w_comboitem_get_image;
	item->set_image = _w_comboitem_set_image;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.handle_top = _w_widget_hp;
	priv->handle_fixed = _w_widget_hp;
	priv->widget.compute_size = _w_combobox_compute_size;
	priv->widget.check_style = _w_combobox_check_style;
	priv->widget.create_handle = _w_combobox_create_handle;
	priv->widget.hook_events = _w_combobox_hook_events;
	_W_COMBOBOX_PRIV(priv)->renderer_render = _w_combobox_renderer_render;
#if GTK3
	_W_COMBOBOX_PRIV(priv)->renderer_get_preferred_width =
			_w_combobox_renderer_get_preferred_width;
#endif
	/*
	 * signals
	 */
	_gtk_signal_fn *signals = priv->widget.signals;
}
