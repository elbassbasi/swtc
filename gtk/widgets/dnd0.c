/*
 * Name:        dnd.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
#include "dnd.h"
void w_clipboard_init(w_clipboard *clipboard) {
	_W_CLIPBOARD(clipboard)->clipboards = 0;
}
wresult _w_toolkit_open_clipboard(w_toolkit *toolkit, w_clipboard *clipboard,
		int clipboards) {
	if (clipboards == W_CLIPBOARD) {
		if (_W_TOOLKIT(toolkit)->GTKCLIPBOARD == 0) {
			_W_TOOLKIT(toolkit)->GTKCLIPBOARD = gtk_clipboard_get(GDK_NONE);
			//_W_TOOLKIT(toolkit)->TARGET = gdk_atom_intern("TARGETS", FALSE);
			if (_W_TOOLKIT(toolkit)->GTKCLIPBOARD == 0) {
				return W_FALSE;
			}
		}
		_W_CLIPBOARD(clipboard)->clipboards = clipboards;
		_W_CLIPBOARD(clipboard)->GTKCLIPBOARD =
		_W_TOOLKIT(toolkit)->GTKCLIPBOARD;
		return W_TRUE;
	}
	if (clipboards == W_SELECTION_CLIPBOARD) {
		if (((_w_toolkit*) toolkit)->GTKPRIMARYCLIPBOARD == 0) {
			GdkAtom primary = gdk_atom_intern("PRIMARY", FALSE);
			_W_TOOLKIT(toolkit)->GTKPRIMARYCLIPBOARD = gtk_clipboard_get(
					primary);
			if (_W_TOOLKIT(toolkit)->GTKPRIMARYCLIPBOARD == 0)
				return W_FALSE;
		}
		_W_CLIPBOARD(clipboard)->clipboards = clipboards;
		_W_CLIPBOARD(clipboard)->GTKCLIPBOARD =
				((_w_toolkit*) toolkit)->GTKPRIMARYCLIPBOARD;
		return W_TRUE;
	}
	return W_TRUE;
}
wresult w_clipboard_clear_contents(w_clipboard *clipboard) {
	if (clipboard == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_CLIPBOARD(clipboard)->clipboards == 0)
		return W_ERROR_NO_HANDLES;
	GtkClipboard *_clipboard = 0;
	if ((_W_CLIPBOARD(clipboard)->clipboards & W_CLIPBOARD) != 0) {
		_clipboard = gtk_toolkit->GTKCLIPBOARD;
	}
	if ((_W_CLIPBOARD(clipboard)->clipboards & W_SELECTION_CLIPBOARD) != 0) {
		_clipboard = gtk_toolkit->GTKPRIMARYCLIPBOARD;
	}
	if (_clipboard == 0)
		return W_ERROR_NO_HANDLES;
#if GTK4
		gdk_clipboard_set_content(_clipboard, 0);
#endif
#if GTK3
	gtk_clipboard_clear(_clipboard);
#endif
	return W_TRUE;
}
wresult w_clipboard_close(w_clipboard *clipboard) {
	_W_CLIPBOARD(clipboard)->clipboards = 0;
	return W_TRUE;
}
wresult w_clipboard_is_ok(w_clipboard *clipboard) {
	if (clipboard == 0)
		return W_FALSE;
	if (_W_CLIPBOARD(clipboard)->clipboards == 0)
		return W_FALSE;
	return W_TRUE;
}
wresult w_clipboard_get_available_types(w_clipboard *clipboard,w_iterator* types){
	w_iterator_close(types);
	if (clipboard == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_CLIPBOARD(clipboard)->clipboards == 0)
		return W_ERROR_NO_HANDLES;
	GtkClipboard *_clipboard = 0;
	if ((_W_CLIPBOARD(clipboard)->clipboards & W_CLIPBOARD) != 0) {
		_clipboard = gtk_toolkit->GTKCLIPBOARD;
	}
	if ((_W_CLIPBOARD(clipboard)->clipboards & W_SELECTION_CLIPBOARD) != 0) {
		_clipboard = gtk_toolkit->GTKPRIMARYCLIPBOARD;
	}
	if (_clipboard == 0)
		return W_ERROR_NO_HANDLES;
	return W_FALSE;
}
wresult w_clipboard_get_contents(w_clipboard *clipboard, w_transfer *transfer,
		w_alloc alloc,void* user_data) {
	if (clipboard == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_CLIPBOARD(clipboard)->clipboards == 0)
		return W_ERROR_NO_HANDLES;
	if (transfer == 0)
		return W_ERROR_NULL_ARGUMENT;
	GtkClipboard *_clipboard = 0;
	if ((_W_CLIPBOARD(clipboard)->clipboards & W_CLIPBOARD) != 0) {
		_clipboard = gtk_toolkit->GTKCLIPBOARD;
	}
	if ((_W_CLIPBOARD(clipboard)->clipboards & W_SELECTION_CLIPBOARD) != 0) {
		_clipboard = gtk_toolkit->GTKPRIMARYCLIPBOARD;
	}
	if (_clipboard == 0)
		return W_ERROR_NO_HANDLES;

	GtkSelectionData *selection_data = 0;
	size_t types_length = 0;
	w_transfer_type *types = transfer->get_types(transfer, &types_length);
	for (size_t i = 0; i < types_length; i++) {
		selection_data = gtk_clipboard_wait_for_contents(_clipboard,
				_GDK_MAKE_ATOM(types[i].id));
		/*
		 * This call to gdk_threads_leave() is a temporary work around
		 * to avoid deadlocks when gdk_threads_init() is called by native
		 * code outside of SWT (i.e AWT, etc). It ensures that the current
		 * thread leaves the GTK lock acquired by the function above.
		 */
		gdk_threads_leave();
		if (selection_data != 0)
			break;
	}
	if (selection_data == 0)
		return W_FALSE;
	w_transfer_data tdata;
	tdata.type = gtk_selection_data_get_data_type(selection_data);
	tdata.pValue = (guchar*) gtk_selection_data_get_data(selection_data);
	tdata.length = gtk_selection_data_get_length(selection_data);
	tdata.format = gtk_selection_data_get_format(selection_data);
	transfer->get_data(transfer, &tdata, alloc,user_data);
	gtk_selection_data_free(selection_data);
	return W_TRUE;
}
struct w_clipboard_transfers {
	size_t count;
	GtkTargetEntry *entry;
	w_transfer_data tdata[0];
};
gboolean w_clipboard_GtkClipboardClearFunc(GtkClipboard *clipboard,
		gpointer user_data_or_owner) {
	struct w_clipboard_transfers *data =
			(struct w_clipboard_transfers*) user_data_or_owner;
	int length = data->count;
	for (int i = 0; i < length; i++) {
		w_transfer_data *tdata = &data->tdata[i];
		if (tdata->pValue != 0) {
			if (tdata->format >= 8 && tdata->format % 8 == 0) {
				g_free(tdata->pValue);
			}
		}
	}
	free(user_data_or_owner);
	return TRUE;
}
gboolean w_clipboard_GtkClipboardGetFunc(GtkClipboard *clipboard,
		GtkSelectionData *selection_data, guint info,
		gpointer user_data_or_owner) {
	if (selection_data == 0)
		return FALSE;
	struct w_clipboard_transfers *data =
			(struct w_clipboard_transfers*) user_data_or_owner;
	GdkAtom target = gtk_selection_data_get_target(selection_data);
	w_transfer_data *tdata;
	int index = -1;
	int length = data->count;
	for (int i = 0; i < length; i++) {
		if (data->tdata[i].type == target) {
			tdata = &data->tdata[i];
			break;
		}
	}
	if (tdata == 0)
		return FALSE;
	if (tdata->format < 8 || tdata->format % 8 != 0) {
		return FALSE;
	}
	gtk_selection_data_set(selection_data, tdata->type, tdata->format,
			tdata->pValue, tdata->length);
	return 1;
}
wresult w_clipboard_set_contents(w_clipboard *clipboard, w_transfers *transfers,
		size_t length) {
	GtkClipboard *_clipboard = 0;
	if ((_W_CLIPBOARD(clipboard)->clipboards & W_CLIPBOARD) != 0) {
		_clipboard = gtk_toolkit->GTKCLIPBOARD;
	}
	if ((_W_CLIPBOARD(clipboard)->clipboards & W_SELECTION_CLIPBOARD) != 0) {
		_clipboard = gtk_toolkit->GTKPRIMARYCLIPBOARD;
	}
	if (_clipboard == 0)
		return W_ERROR_NO_HANDLES;
	GtkTargetEntry *entries = 0, *entry;
	size_t count = 0, types_length;
	w_transfer_type *types;
	for (int i = 0; i < length; i++) {
		w_transfer *transfer = transfers[i].transfer;
		if (transfer != 0 && transfers[i].data != 0) {
			types_length = 0;
			types = transfer->get_types(transfer, &types_length);
			count += types_length;
		}
	}
	struct w_clipboard_transfers *data = malloc(
			sizeof(struct w_clipboard_transfers)
					+ count * sizeof(GtkTargetEntry)
					+ count * sizeof(w_transfer_data));
	if (data == 0)
		return W_ERROR_NO_MEMORY;
	data->count = count;
	data->entry = (void*) data + sizeof(struct w_clipboard_transfers)
			+ count * sizeof(w_transfer_data);
	entries = data->entry;
	entry = entries;
	w_transfer_data *tdata = data->tdata;
	for (size_t i = 0; i < length; i++) {
		w_transfer *transfer = transfers[i].transfer;
		if (transfer != 0 && transfers[i].data != 0) {
			types_length = 0;
			types = transfer->get_types(transfer, &types_length);
			for (size_t i = 0; i < types_length; i++) {
				entry->info = types[i].id;
				entry->target = (gchar*) types->name;
				entry->flags = 0;
				tdata->type = (GdkAtom) types[i].id;
				transfer->set_data(transfer, tdata, transfers[i].data,
						transfers[i].size);
				entry++;
				tdata++;
			}
		}
	}
	if (!gtk_clipboard_set_with_data(_clipboard, entries, count,
			(GtkClipboardGetFunc) w_clipboard_GtkClipboardGetFunc,
			(GtkClipboardClearFunc) w_clipboard_GtkClipboardClearFunc, data)) {
		free(data);
		return W_FALSE;
	}
	gtk_clipboard_set_can_store(_clipboard, 0, 0);
	return W_TRUE;
}
