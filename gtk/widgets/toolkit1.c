/*
 * Name:        toolkit1.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
void* _w_toolkit_malloc(size_t size) {
	if ((gtk_toolkit->tmp_length + size) < gtk_toolkit->tmp_alloc) {
		int i = gtk_toolkit->tmp_length;
		gtk_toolkit->tmp_length += size;
		return &gtk_toolkit->tmp[i];
	} else {
		return malloc(size);
	}
}
void* _w_toolkit_malloc_all(size_t *size) {
	*size = gtk_toolkit->tmp_alloc - gtk_toolkit->tmp_length;
	int i = gtk_toolkit->tmp_length;
	gtk_toolkit->tmp_length += *size;
	return &gtk_toolkit->tmp[i];
}
void _w_toolkit_free(void *ptr, size_t size) {
	wintptr diff = ptr - (void*) gtk_toolkit->tmp;
	if (diff >= 0 && diff < gtk_toolkit->tmp_alloc) {
		gtk_toolkit->tmp_length -= size;
	} else
		free(ptr);
}
void _w_toolkit_add_shell(_w_shell *shell) {
	shell->next = 0;
	if (gtk_toolkit->shells == 0) {
		gtk_toolkit->shells = shell;
		shell->prev = shell; //last
	} else {
		_w_shell *last = gtk_toolkit->shells->prev;
		last->next = shell;
		shell->prev = last;
		gtk_toolkit->shells->prev = shell;
	}
	gtk_toolkit->shells_count++;
}
void _w_toolkit_remove_shell(_w_shell *shell) {
	if (shell == gtk_toolkit->shells) {
		gtk_toolkit->shells = shell->next;
		if (gtk_toolkit->shells != 0)
			gtk_toolkit->shells->prev = shell->prev; //last
	} else {
		if (shell->next == 0) {
			gtk_toolkit->shells->prev = shell->prev;
		} else {
			shell->next->prev = shell->prev;
		}
		shell->prev->next = shell->next;
	}
	gtk_toolkit->shells_count--;
}
void _w_toolkit_put_gdk_events(int event, ...) {

}
void _w_toolkit_remove_gdk_events() {

}
const char *_gtk_signal_names[SIGNAL_LAST] = { //
		[SIGNAL_ACTIVATE]="", // //
				[SIGNAL_DESTROY]="destroy", // //widget
				[SIGNAL_CHANGED]="", //
				[SIGNAL_CLICKED]="", //
				[SIGNAL_GRAB_FOCUS]="grab-focus", //
				[SIGNAL_HIDE]="hide", //
				[SIGNAL_MAP]="map", //
				[SIGNAL_OUTPUT]="", //
				[SIGNAL_POPUP_MENU]="popup-menu", //
				[SIGNAL_PREEDIT_CHANGED]="", //
				[SIGNAL_REALIZE]="realize", //
				[SIGNAL_SELECT]="", //
				[SIGNAL_SHOW]="show", //
				[SIGNAL_UNMAP]="unmap", //
				[SIGNAL_UNREALIZE]="unrealize", //
				[SIGNAL_VALUE_CHANGED]="value-changed", //
				[SIGNAL_ACTIVATE_INVERSE]="", //
				[SIGNAL_DAY_SELECTED]="", //
				[SIGNAL_MONTH_CHANGED]="", //
				[SIGNAL_DAY_SELECTED_DOUBLE_CLICK]="", //
				[SIGNAL_SELECTION_DONE]="", //
				[SIGNAL_START_INTERACTIVE_SEARCH]="", //
				[SIGNAL_BACKSPACE]="", //
				[SIGNAL_BACKSPACE_INVERSE]="", //
				[SIGNAL_COPY_CLIPBOARD]="", //
				[SIGNAL_COPY_CLIPBOARD_INVERSE]="", //
				[SIGNAL_CUT_CLIPBOARD]="", //
				[SIGNAL_CUT_CLIPBOARD_INVERSE]="", //
				[SIGNAL_PASTE_CLIPBOARD]="", //
				[SIGNAL_PASTE_CLIPBOARD_INVERSE]="", //
				[SIGNAL_CREATE_MENU_PROXY]="", //
				[SIGNAL_BUTTON_PRESS_EVENT] ="button-press-event", // //widget
				[SIGNAL_BUTTON_PRESS_EVENT_INVERSE]="", // //widget
				[SIGNAL_BUTTON_RELEASE_EVENT]="button-release-event", // //widget
				[SIGNAL_BUTTON_RELEASE_EVENT_INVERSE]="", // //widget
				[SIGNAL_COMMIT]="", //
				[SIGNAL_CONFIGURE_EVENT]="configure-event", //
				[SIGNAL_DELETE_EVENT]="delete-event", //
				[SIGNAL_ENTER_NOTIFY_EVENT]="enter-notify-event", //
				[SIGNAL_EVENT]="", //
				[SIGNAL_EVENT_AFTER]="event-after", //
#ifdef GTK2
				[SIGNAL_EXPOSE_EVENT]="expose-event", //
				[SIGNAL_EXPOSE_EVENT_INVERSE]="expose-event", //
				[SIGNAL_DRAW]="expose-event", //
#endif
#ifdef GTK3
				[SIGNAL_EXPOSE_EVENT]="draw", //
				[SIGNAL_EXPOSE_EVENT_INVERSE]="draw", //
				[SIGNAL_DRAW]="draw", //
#endif
				[SIGNAL_FOCUS]="focus", //
				[SIGNAL_FOCUS_IN_EVENT]="focus-in-event", //
				[SIGNAL_FOCUS_OUT_EVENT]="focus-out-event", //
				[SIGNAL_INPUT]="", //
				[SIGNAL_KEY_PRESS_EVENT]="key-press-event", //
				[SIGNAL_KEY_RELEASE_EVENT]="key-release-event", //
				[SIGNAL_LEAVE_NOTIFY_EVENT]="leave-notify-event", //
				[SIGNAL_MAP_EVENT]="map-event", //
				[SIGNAL_MNEMONIC_ACTIVATE]="mnemonic-activate", //
				[SIGNAL_MOTION_NOTIFY_EVENT]="motion-notify-event", //
				[SIGNAL_MOTION_NOTIFY_EVENT_INVERSE]="", //
				[SIGNAL_MOVE_FOCUS]="", //
				[SIGNAL_POPULATE_POPUP]="", //
				[SIGNAL_SCROLL_EVENT]="scroll-event", //
				[SIGNAL_SHOW_HELP]="show-help", //
				[SIGNAL_SIZE_ALLOCATE]="size-allocate", //
				[SIGNAL_STYLE_SET]="style-set", //
				[SIGNAL_TOGGLED]="", //
				[SIGNAL_UNMAP_EVENT]="unmap-event", //
				[SIGNAL_WINDOW_STATE_EVENT]="window-state-event", //
				[SIGNAL_ROW_DELETED]="", //
				[SIGNAL_DIRECTION_CHANGED]="", //
				[SIGNAL_DELETE_RANGE] ="", //
				[SIGNAL_DELETE_TEXT]="", //
				[SIGNAL_ROW_ACTIVATED]="", //
				[SIGNAL_SCROLL_CHILD]="", //
				[SIGNAL_SWITCH_PAGE]="", //
				[SIGNAL_TEST_COLLAPSE_ROW]="", //
				[SIGNAL_TEST_EXPAND_ROW]="", //
				[SIGNAL_STATUS_ICON_POPUP_MENU]="", //
				[SIGNAL_ROW_INSERTED]="", //
				[SIGNAL_ICON_RELEASE]="", //
				[SIGNAL_DELETE_FROM_CURSOR]="", //
				[SIGNAL_DELETE_FROM_CURSOR_INVERSE]="", //
				[SIGNAL_ROW_HAS_CHILD_TOGGLED]="", //
				[SIGNAL_CHANGE_VALUE] ="change-value", //
				[SIGNAL_EXPAND_COLLAPSE_CURSOR_ROW]="", //
				[SIGNAL_INSERT_TEXT]="", //
				[SIGNAL_TEXT_BUFFER_INSERT_TEXT]="", //
				[SIGNAL_MOVE_CURSOR]="", //
				[SIGNAL_MOVE_CURSOR_INVERSE]="", //
				[SIGNAL_DRAG_DATA_GET]="drag_data_get", //
				[SIGNAL_DRAG_MOTION]="drag_motion", //
				[SIGNAL_DRAG_DROP]="drag_drop", //
				[SIGNAL_DRAG_LEAVE]="drag_leave", //
				[SIGNAL_DRAG_DATA_RECEIVED]="drag_data_received", //
				[SIGNAL_DRAG_END]="drag_end", //
				[SIGNAL_DRAG_DATA_DELETE]="drag_data_delete", //
		};
void _w_toolkit_widget_class_init(_w_toolkit *toolkit) {
	/*
	 * shell
	 */
	W_WIDGET_CLASS(&toolkit->class_shell)->init_class =
			(w_widget_init_class) _w_shell_class_init;
	W_WIDGET_CLASS(&toolkit->class_shell)->reserved[0] =
			&toolkit->class_shell_priv;
	toolkit->classes[_W_CLASS_SHELL] = W_WIDGET_CLASS(&toolkit->class_shell);
	/*
	 * canvas
	 */
	W_WIDGET_CLASS(&toolkit->class_canvas)->init_class =
			(w_widget_init_class) _w_canvas_class_init;
	W_WIDGET_CLASS(&toolkit->class_canvas)->reserved[0] =
			&toolkit->class_canvas_priv;
	toolkit->classes[_W_CLASS_CANVAS] = W_WIDGET_CLASS(&toolkit->class_canvas);
	/*
	 * composite
	 */
	W_WIDGET_CLASS(&toolkit->class_composite)->init_class =
			(w_widget_init_class) _w_composite_class_init;
	W_WIDGET_CLASS(&toolkit->class_composite)->reserved[0] =
			&toolkit->class_composite_priv;
	toolkit->classes[_W_CLASS_COMPOSITE] = W_WIDGET_CLASS(
			&toolkit->class_composite);
	/*
	 * menu
	 */
	W_WIDGET_CLASS(&toolkit->class_menu)->init_class =
			(w_widget_init_class) _w_menu_class_init;
	W_WIDGET_CLASS(&toolkit->class_menu)->reserved[0] =
			&toolkit->class_menu_priv;
	W_MENU_CLASS(&toolkit->class_menu)->class_menuitem =
			&toolkit->class_menuitem;
	toolkit->classes[_W_CLASS_MENU] = W_WIDGET_CLASS(&toolkit->class_menu);
	/*
	 * treeview
	 */
	W_WIDGET_CLASS(&toolkit->class_tree)->init_class =
			(w_widget_init_class) _w_treeview_class_init;
	W_WIDGET_CLASS(&toolkit->class_tree)->reserved[0] =
			&toolkit->class_tree_priv;
	W_LISTVIEWBASE_CLASS(&toolkit->class_tree)->class_item = W_ITEM_CLASS(
			&toolkit->class_treeitem);
	W_LISTVIEWBASE_CLASS(&toolkit->class_tree)->class_column =
			&toolkit->class_treecolumn;
	toolkit->classes[_W_CLASS_TREEVIEW] = W_WIDGET_CLASS(&toolkit->class_tree);
	/*
	 * listview
	 */
	W_WIDGET_CLASS(&toolkit->class_listview)->init_class =
			(w_widget_init_class) _w_listview_class_init;
	W_WIDGET_CLASS(&toolkit->class_listview)->reserved[0] =
			&toolkit->class_listview_priv;
	W_LISTVIEWBASE_CLASS(&toolkit->class_listview)->class_item = W_ITEM_CLASS(
			&toolkit->class_listitem);
	W_LISTVIEWBASE_CLASS(&toolkit->class_listview)->class_column =
			&toolkit->class_listcolumn;
	toolkit->classes[_W_CLASS_LISTVIEW] = W_WIDGET_CLASS(
			&toolkit->class_listview);
	/*
	 * sash
	 */
	W_WIDGET_CLASS(&toolkit->class_sash)->init_class =
			(w_widget_init_class) _w_sash_class_init;
	W_WIDGET_CLASS(&toolkit->class_sash)->reserved[0] =
			&toolkit->class_sash_priv;
	toolkit->classes[_W_CLASS_SASH] = W_WIDGET_CLASS(&toolkit->class_sash);
	/*
	 * button
	 */
	W_WIDGET_CLASS(&toolkit->class_button)->init_class =
			(w_widget_init_class) _w_button_class_init;
	W_WIDGET_CLASS(&toolkit->class_button)->reserved[0] =
			&toolkit->class_button_priv;
	toolkit->classes[_W_CLASS_BUTTON] = W_WIDGET_CLASS(&toolkit->class_button);
	/*
	 * label
	 */
	W_WIDGET_CLASS(&toolkit->class_label)->init_class =
			(w_widget_init_class) _w_label_class_init;
	W_WIDGET_CLASS(&toolkit->class_label)->reserved[0] =
			&toolkit->class_label_priv;
	toolkit->classes[_W_CLASS_LABEL] = W_WIDGET_CLASS(&toolkit->class_label);
	/*
	 * textedit
	 */
	W_WIDGET_CLASS(&toolkit->class_textedit)->init_class =
			(w_widget_init_class) _w_textedit_class_init;
	W_WIDGET_CLASS(&toolkit->class_textedit)->reserved[0] =
			&toolkit->class_textedit_priv;
	toolkit->classes[_W_CLASS_TEXTEDIT] = W_WIDGET_CLASS(
			&toolkit->class_textedit);
	/*
	 * progressbar
	 */
	W_WIDGET_CLASS(&toolkit->class_progressbar)->init_class =
			(w_widget_init_class) _w_progressbar_class_init;
	W_WIDGET_CLASS(&toolkit->class_progressbar)->reserved[0] =
			&toolkit->class_progressbar_priv;
	toolkit->classes[_W_CLASS_PROGRESSBAR] = W_WIDGET_CLASS(
			&toolkit->class_progressbar);
}
void _w_toolkit_init_gtk(_w_toolkit *toolkit) {
	char txt[30];
	gtk_init(0, 0);
	int major = gtk_get_major_version();
	int minor = gtk_get_minor_version();
	int micro = gtk_get_micro_version();
	toolkit->version = VERSION(major, minor, micro);
	for (int i = 0; i < SWT_GQUARK_LAST; i++) {
		sprintf(txt, "swt_quark_%d", i);
		gtk_toolkit->quark[i] = g_quark_from_string(txt);
	}
	toolkit->thread.id = (w_threadid) pthread_self();
	toolkit->empty_tab = pango_tab_array_new(1, FALSE);
	if (toolkit->empty_tab != 0) {
		pango_tab_array_set_tab(toolkit->empty_tab, 0, PANGO_TAB_LEFT, 1);
	}
}
void _w_toolkit_init_display(_w_toolkit *toolkit) {
	GdkDisplay *display = gdk_display_get_default();
	const char *type_name = g_type_name(
			((GTypeInstance*) display)->g_class->g_type);
	if (!strcmp(type_name, "GdkX11Display")) {
		toolkit->ISX11 = 1;
		toolkit->libX11 = dlopen("libX11.so", RTLD_LAZY | RTLD_GLOBAL);
	} else if (!strcmp(type_name, "GdkWaylandDisplay")) {
		toolkit->ISWayland = 1;
	}
}
void _w_toolkit_init(_w_toolkit *toolkit) {
	_w_toolkit_class_init(toolkit);
	_w_toolkit_widget_class_init(toolkit);
	_w_toolkit_init_gtk(toolkit);
	_w_toolkit_init_display(toolkit);
	_w_theme_init();
}
void _w_toolkit_dispose(w_disposable *disposable) {
	_w_toolkit *toolkit = _W_TOOLKIT(disposable);
}
