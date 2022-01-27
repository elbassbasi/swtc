/*
 * Name:        toolkit1.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
#include "../../custom/controls/expandbar.h"
#include <sys/inotify.h>
#include "dnd.h"
#define INNER_BORDER 2
void* _w_toolkit_malloc(size_t size) {
	void *ptr;
	if ((gtk_toolkit->tmp_alloc + size) < gtk_toolkit->tmp_reserved) {
		int i = gtk_toolkit->tmp_alloc;
		gtk_toolkit->tmp_alloc += size;
		ptr = &gtk_toolkit->_tmp[i + gtk_toolkit->tmpaddr_sz * sizeof(wuint64)];
	} else {
		w_futex_lock(&gtk_toolkit->tmp_lock);
		if ((gtk_toolkit->tmp_alloc + size)
				< (gtk_toolkit->tmp_total - gtk_toolkit->tmpaddr_alloc)) {
			int i = gtk_toolkit->tmp_alloc;
			gtk_toolkit->tmp_alloc += size;
			ptr = &gtk_toolkit->_tmp[i
					+ gtk_toolkit->tmpaddr_sz * sizeof(wuint64)];
		} else {
			ptr = malloc(size);
		}
		w_futex_unlock(&gtk_toolkit->tmp_lock);
	}
	return ptr;
}
void* _w_toolkit_malloc_all(size_t *size) {
	*size = gtk_toolkit->tmp_total - gtk_toolkit->tmp_alloc;
	int i = gtk_toolkit->tmp_alloc;
	gtk_toolkit->tmp_alloc += *size;
	return &gtk_toolkit->_tmp[i + gtk_toolkit->tmpaddr_sz * sizeof(wuint64)];
}
void _w_toolkit_free(void *ptr, size_t size) {
	wintptr diff = ptr - (void*) gtk_toolkit;
	if (diff >= 0 && diff < gtk_toolkit->total_size) {
		gtk_toolkit->tmp_alloc -= size;
	} else
		free(ptr);
}
void* _w_toolkit_new_pages(size_t size) {
	const int sz = gtk_toolkit->tmpaddr_sz;
	int pages_count = size / _PAGES_SIZE;
	if ((size % _PAGES_SIZE) != 0)
		pages_count++;
	wuint k = gtk_toolkit->tmpaddr_sz_bits;
	wuint64 *tmpaddr = (wuint64*) gtk_toolkit->_tmp;
	if (pages_count == 1) {
		int j = -1;
		int i;
		w_futex_lock(&gtk_toolkit->tmp_lock);
		for (i = 0; i < sz; i++) {
			if (tmpaddr[i] != -1) {
				j = __builtin_ctz(~tmpaddr[i]);
				tmpaddr[i] |= 1 << j;
				k = i * 64 + j;
				break;
			}
		}
		w_futex_unlock(&gtk_toolkit->tmp_lock);
	} else {
		int i, j, c = 0, s = 0;
		wuint64 t;
		w_futex_lock(&gtk_toolkit->tmp_lock);
		for (i = 0; i < sz; i++) {
			t = tmpaddr[i];
			if (t != -1) {
				for (j = 0; j < 64; j++) {
					if (t & (1 << j)) {
						c++;
						if (c == pages_count) {
							k = s;
							break;
						}
					} else {
						s = i * 64 + j;
						c = 0;
					}
				}
				if (c == pages_count) {
					break;
				}
			} else {
				s = (i + 1) * 64 + j;
				c = 0;
			}
		}
		w_futex_unlock(&gtk_toolkit->tmp_lock);
	}
	void *addr;
	if (k < gtk_toolkit->tmpaddr_sz_bits) {
		addr = &((char*) gtk_toolkit)[gtk_toolkit->total_size
				- (k + pages_count) * _PAGES_SIZE];
	} else {
		addr = malloc(size);
	}
	return addr;
}
void _w_toolkit_delete_pages(void *ptr, size_t size) {
	wintptr diff = ptr - (void*) gtk_toolkit;
	if (diff >= 0 && diff < gtk_toolkit->total_size) {
		wuint64 *tmpaddr = (wuint64*) gtk_toolkit->_tmp;
		int pages_count = size / _PAGES_SIZE;
		if ((size % _PAGES_SIZE) != 0)
			pages_count++;
		int k = gtk_toolkit->total_size - diff / (4 * sizeof(void*));
		for (; k < pages_count; k++) {
			int i = k / 64;
			int j = k % 64;
			tmpaddr[i] &= ~(1 << j);
		}
	} else
		free(ptr);
}
size_t _gtk_toolkit_alloc_fn(void *user_data, size_t size, void **buf) {
	_gtk_toolkit_alloc *alloc = (_gtk_toolkit_alloc*) user_data;
	alloc->text = (char*) _w_toolkit_malloc(size);
	alloc->size = size;
	*buf = alloc->text;
	return size;
}
void _w_toolkit_add_shell(_w_shell *shell) {
	w_link_linklast_0(&shell->shells_link, shell,
			(void**) &gtk_toolkit->shells);
	gtk_toolkit->shells_count++;
}
void _w_toolkit_remove_shell(_w_shell *shell) {
	w_link_unlink_0(&shell->shells_link, shell, (void**) &gtk_toolkit->shells);
	gtk_toolkit->shells_count--;
}
void _w_toolkit_registre_free(w_widget *widget) {
	widget->handle = gtk_toolkit->widget_free; //widget->next =
	gtk_toolkit->widget_free = widget;
}
void _w_toolkit_put_gdk_events(int event, ...) {

}
void _w_toolkit_remove_gdk_events() {

}
void _w_toolkit_get_entry_inner_border(GtkWidget *handle, GtkBorder *border) {
	GtkBorder *gtkborder = (GtkBorder*) gtk_entry_get_inner_border(
			GTK_ENTRY(handle));
	if (gtkborder != 0) {
		memcpy(border, gtkborder, sizeof(GtkBorder));
		return;
	}
	gtkborder = 0;
	gtk_widget_style_get(handle, "inner-border", &gtkborder, NULL);
	if (gtkborder != 0) {
		memcpy(border, gtkborder, sizeof(GtkBorder));
		gtk_border_free(gtkborder);
		return;
	}
	border->left = INNER_BORDER;
	border->top = INNER_BORDER;
	border->right = INNER_BORDER;
	border->bottom = INNER_BORDER;
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
				[SIGNAL_MOVE_FOCUS]="move-focus", //
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
void cw_coolbar_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_coolbar_class *clazz);
w_widget_init_class gtk_toolkit_classes_init[_W_CLASS_LAST] = {			//
		[_W_CLASS_SHELL] =(w_widget_init_class) _w_shell_class_init,		//
				[_W_CLASS_CANVAS] =(w_widget_init_class) _w_canvas_class_init,//
				[_W_CLASS_CCANVAS] =(w_widget_init_class) _w_ccanvas_class_init,//
				[_W_CLASS_COMPOSITE
						] =(w_widget_init_class) _w_composite_class_init,	//
				[_W_CLASS_MENU] =(w_widget_init_class) _w_menu_class_init,	//
				[_W_CLASS_TREEVIEW
						] =(w_widget_init_class) _w_treeview_class_init,	//
				[_W_CLASS_LISTVIEW
						] =(w_widget_init_class) _w_listview_class_init,	//
				[_W_CLASS_SASH] =(w_widget_init_class) _w_sash_class_init,	//
				[_W_CLASS_BUTTON] =(w_widget_init_class) _w_button_class_init,//
				[_W_CLASS_LABEL] =(w_widget_init_class) _w_label_class_init,//
				[_W_CLASS_TEXTEDIT
						] =(w_widget_init_class) _w_textedit_class_init,	//
				[_W_CLASS_PROGRESSBAR
						] =(w_widget_init_class) _w_progressbar_class_init,	//
				[_W_CLASS_GROUPBOX
						] =(w_widget_init_class) _w_groupbox_class_init,	//
				[_W_CLASS_COMBOBOX
						] =(w_widget_init_class) _w_combobox_class_init,	//
				[_W_CLASS_COOLBAR] =(w_widget_init_class) cw_coolbar_class_init,//
				[_W_CLASS_DATETIME
						] =(w_widget_init_class) _w_datetime_class_init,	//
				[_W_CLASS_SLIDER] =(w_widget_init_class) _w_slider_class_init,//
				[_W_CLASS_SPINNER] =(w_widget_init_class) _w_spinner_class_init,//
				[_W_CLASS_TABVIEW] =(w_widget_init_class) _w_tabview_class_init,//
				[_W_CLASS_TOOLBAR] =(w_widget_init_class) _w_toolbar_class_init,//
				/*[_W_CLASS_TRAY] =(w_widget_init_class) _w_tray_class_init,	//
				 [_W_CLASS_TOOLTIP] =(w_widget_init_class) _w_tooltip_class_init,//*/
				[_W_CLASS_EXPANDBAR
						] =(w_widget_init_class) _w_expandbar_class_init,	//
				[_W_CLASS_DRAGSOURCE
						] =(w_widget_init_class) _w_dragsource_class_init,	//
				[_W_CLASS_DROPTARGET
						] =(w_widget_init_class) _w_droptarget_class_init,	//
		};
void _w_toolkit_init_gtk(_w_toolkit *toolkit, int argc, char **argv) {
	char txt[30];
	//toolkit->gtkApp = gtk_application_new("", G_APPLICATION_FLAGS_NONE);
	gtk_init(&argc, &argv);
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
	pthread_mutex_init(&toolkit->condition_mutex, NULL);
	pthread_cond_init(&toolkit->condition, NULL);
}
void _w_toolkit_init_trims(_w_toolkit *toolkit) {
	toolkit->trimWidths[TRIM_NONE] = 0;
	toolkit->trimHeights[TRIM_NONE] = 0;
	toolkit->trimWidths[TRIM_BORDER] = 4;
	toolkit->trimHeights[TRIM_BORDER] = 4;
	toolkit->trimWidths[TRIM_RESIZE] = 6;
	toolkit->trimHeights[TRIM_RESIZE] = 6;
	toolkit->trimWidths[TRIM_TITLE_BORDER] = 5;
	toolkit->trimHeights[TRIM_TITLE_BORDER] = 28;
	toolkit->trimWidths[TRIM_TITLE_RESIZE] = 6;
	toolkit->trimHeights[TRIM_TITLE_RESIZE] = 29;
	toolkit->trimWidths[TRIM_TITLE] = 0;
	toolkit->trimHeights[TRIM_TITLE] = 23;
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
void _w_toolkit_init(_w_toolkit *toolkit, int argc, char **argv) {
	_w_toolkit_class_init(toolkit);
	_w_toolkit_classes_init(&toolkit->classes);
	_w_toolkit_init_gtk(toolkit,argc,argv);
	_w_toolkit_init_display(toolkit);
	_w_toolkit_init_trims(toolkit);
	_w_widget_init_signal_0();
	_w_theme_init();
}
void _w_toolkit_dispose(w_disposable *disposable) {
	_w_toolkit *toolkit = _W_TOOLKIT(disposable);
	_gtk_signal *signals = toolkit->signals;
	for (int i = 0; i < SIGNAL_LAST; i++) {
		if (signals[i].closure != 0) {
			g_closure_unref(signals[i].closure);
		}
	}
	w_theme_dispose((w_theme*) &toolkit->gtktheme);
	for (int i = 0; i <= W_CURSOR_HAND; i++) {
		w_cursor_dispose((w_cursor*) &toolkit->cursors[i]);
	}
	if (toolkit->empty_tab != 0) {
		pango_tab_array_free(toolkit->empty_tab);
	}
	w_font_dispose((w_font*) toolkit->system_font);
	//g_object_unref(toolkit->gtkApp);
	struct _w_widget_class **classes = toolkit->classes.classes;
	for (int i = 0; i < _W_CLASS_LAST; i++) {
		if (classes[i] != 0 && classes[i]->class_id != 0) {
			classes[i]->dispose_class(classes[i]);
		}
	}
	//gdk_destroy();
}
