/*
 * Name:        toolkit.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef OSX_WIDGETS_TOOLKIT_H_
#define OSX_WIDGETS_TOOLKIT_H_
#include "shell.h"
#include "menu.h"
#include "../controls/controls.h"
typedef struct _cocoa_theme {
	w_theme theme;

} _cocoa_theme;
typedef struct _w_toolkit {
	w_toolkit toolkit;
	int version;
	_w_cursor cursors[W_CURSOR_HAND + 1];
	_w_image systemImages[5];
	Class nsclasses[_NS_CLASS_LAST];
	ns_get_widget _ns_get_widget[_NS_CLASS_LAST];
	w_taskbar taskbar;
	w_tray tray;
	w_thread thread;
	w_widget *widget_free;
	w_control *ignoreFocusControl;
	w_control *currentControl;
	NSApplication *application;
	NSAutoreleasePool *pool;
	NSMutableArray *isPainting;
	NSMutableArray *needsDisplay;
	NSNumber *SYNTHETIC_BOLD;
	NSNumber *SYNTHETIC_ITALIC;
	NSString *emptyString;
	w_font* systemFont;
	w_menu appMenuBar;
	w_menu *menuBar;
	NSWindow *screenWindow, *keyWindow;
	int exit_code;
	unsigned exit_loop :1;
	unsigned sheet :1;
	unsigned lockCursor :1;
	int screenID[32];
	char screenCascade_created[32];
	NSPoint screenCascade[32];
	/*
	 * shell
	 */
	w_shell *activeShell;
	_w_shell *shells;
	int shells_count;
	//theme
	_cocoa_theme cocoatheme;
	w_theme *theme;
	/*
	 * classes
	 */
	struct _w_widget_class *classes[_W_CLASS_LAST];
	struct _w_toolkit_class class_toolkit;
	/*
	 * shell
	 */
	struct _w_shell_class class_shell;
	_w_shell_priv class_shell_priv;
	/*
	 * canvas
	 */
	struct _w_canvas_class class_canvas;
	_w_canvas_priv class_canvas_priv;
	struct _w_caret_class class_caret;
	struct _w_ime_class class_ime;
	/*
	 * composite
	 */
	struct _w_composite_class class_composite;
	_w_composite_priv class_composite_priv;
	struct _w_scrollbar_class class_scrollbar;
	/*
	 * menu
	 */
	struct _w_menu_class class_menu;
	_w_menu_priv class_menu_priv;
	struct _w_menuitem_class class_menuitem;
	/*
	 * treeview
	 */
	struct _w_treeview_class class_tree;
	_w_treeview_priv class_tree_priv;
	struct _w_treeitem_class class_treeitem;
	struct _w_columnitem_class class_treecolumn;
	/*
	 * listview
	 */
	struct _w_listview_class class_listview;
	_w_listview_priv class_listview_priv;
	struct _w_listitem_class class_listitem;
	struct _w_columnitem_class class_listcolumn;
	/*
	 * sash
	 */
	struct _w_sash_class class_sash;
	_w_sash_priv class_sash_priv;
	/*
	 * button
	 */
	struct _w_button_class class_button;
	_w_button_priv class_button_priv;
	/*
	 * label
	 */
	struct _w_label_class class_label;
	_w_label_priv class_label_priv;
	/*
	 * textedit
	 */
	struct _w_textedit_class class_textedit;
	_w_textedit_priv class_textedit_priv;
	/*
	 * progressbar
	 */
	struct _w_progressbar_class class_progressbar;
	_w_progressbar_priv class_progressbar_priv;
	/*
	 * groupbox
	 */
	struct _w_groupbox_class class_groupbox;
	_w_groupbox_priv class_groupbox_priv;
	/*
	 * combobox
	 */
	struct _w_combobox_class class_combobox;
	_w_combobox_priv class_combobox_priv;
	struct _w_combobox_class class_comboitem;
	/*
	 * coolbar
	 */
	struct _w_coolbar_class class_coolbar;
	_w_coolbar_priv class_coolbar_priv;
	struct _w_coolitem_class class_coolitem;
	/*
	 * datetime
	 */
	struct _w_datetime_class class_datetime;
	_w_datetime_priv class_datetime_priv;
	/*
	 * expandbar
	 */
	struct _w_expandbar_class class_expandbar;
	_w_expandbar_priv class_expandbar_priv;
	struct _w_expanditem_class class_expanditem;
	/*
	 * slider
	 */
	struct _w_slider_class class_slider;
	_w_slider_priv class_slider_priv;
	/*
	 * spinner
	 */
	struct _w_spinner_class class_spinner;
	_w_spinner_priv class_spinner_priv;
	/*
	 * tabview
	 */
	struct _w_tabview_class class_tabview;
	_w_tabview_priv class_tabview_priv;
	struct _w_tabitem_class class_tabitem;
	/*
	 * toolbar
	 */
	struct _w_toolbar_class class_toolbar;
	_w_toolbar_priv class_toolbar_priv;
	struct _w_toolitem_class class_toolitem;
	/*
	 * internal memory
	 */
	size_t tmp_alloc;
	size_t tmp_length;
	char tmp[0];
} _w_toolkit;
extern _w_toolkit *mac_toolkit;
#define _W_TOOLKIT(x) ((_w_toolkit*)x)
#define VERSION(major,minor,bugfix) ((major << 16) + (minor << 8) + bugfix)
#define MAC_VERSION (mac_toolkit->version)
typedef struct _w_shells_iterator {
	w_basic_iterator base;
	_w_shell *parent;
	_w_shell *current;
	int count;
} _w_shells_iterator;
#define _W_SHELLS_ITERATOR(it) ((_w_shells_iterator*)it)
void* _w_toolkit_malloc(size_t size);
void* _w_toolkit_malloc_all(size_t *size);
void _w_toolkit_free(void *ptr, size_t size);
void _w_toolkit_add_shell(_w_shell *shell);
void _w_toolkit_remove_shell(_w_shell *shell);
void _w_toolkit_init(_w_toolkit *toolkit);
void _w_toolkit_dispose(w_disposable *disposable);
void _w_toolkit_class_init(_w_toolkit *toolkit);
void _w_theme_init();
void _w_toolkit_set_cursor(w_control *control);
void _w_toolkit_set_menubar(w_menu *menu);
void _w_toolkit_cascade_window(NSWindow *window, NSScreen *screen);
wresult _w_toolkit_set_text(NSString *str, w_alloc string, void *user_data,
		int enc);
/**
 * public function
 */
wresult _w_toolkit_check_widget(w_toolkit *toolkit, w_widget *widget);
wresult _w_toolkit_check_widgetdata(w_toolkit *toolkit,
		w_widgetdata *widgetdata);
struct _w_widget_class* _w_toolkit_get_class(w_toolkit *toolkit,
		wushort clazz_id);
struct w_theme* _w_toolkit_get_theme(w_toolkit *toolkit);
wresult _w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
		void *args);
wresult _w_toolkit_beep(w_toolkit *toolkit);
wresult _w_toolkit_close(w_toolkit *toolkit);
w_shell* _w_toolkit_get_active_shell(w_toolkit *toolkit);
w_menu* _w_toolkit_get_menubar(w_toolkit *toolkit);
wresult _w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *bounds);
wresult _w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea);
w_control* _w_toolkit_get_cursor_control(w_toolkit *toolkit);
wresult _w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location);
size_t _w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length);
int _w_toolkit_get_dismissal_alignment(w_toolkit *toolkit);
int _w_toolkit_get_double_click_time(w_toolkit *toolkit);
w_control* _w_toolkit_get_focus_control(w_toolkit *toolkit);
wresult _w_toolkit_get_high_contrast(w_toolkit *toolkit);
int _w_toolkit_get_icon_depth(w_toolkit *toolkit);
size_t _w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length);
void _w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells);
w_color _w_toolkit_get_system_color(w_toolkit *toolkit, wuint id);
w_cursor* _w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint id);
w_font* _w_toolkit_get_system_font(w_toolkit *toolkit);
wresult _w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
		w_image **image);
w_menu* _w_toolkit_get_system_menu(w_toolkit *toolkit);
struct w_taskbar* _w_toolkit_get_system_taskbar(w_toolkit *toolkit);
struct w_tray* _w_toolkit_get_system_tray(w_toolkit *toolkit);
w_thread* _w_toolkit_get_thread(w_toolkit *toolkit);
wuint64 _w_toolkit_get_thread_id(w_toolkit *toolkit);
wresult _w_toolkit_get_touch_enabled(w_toolkit *toolkit);
wresult _w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
		w_point *result, w_point *point);
wresult _w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
		w_rect *result, w_rect *rectangle);
wresult _w_toolkit_post(w_toolkit *toolkit, w_event *event);
wresult _w_toolkit_post_quit(w_toolkit *toolkit, int quit);
wresult _w_toolkit_dispatch(w_toolkit *toolkit);
wresult _w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *point);
int _w_toolkit_run(w_toolkit *toolkit);
wresult _w_toolkit_read(w_toolkit *toolkit);
wresult _w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args);
wresult _w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args);
wresult _w_toolkit_update(w_toolkit *toolkit);
wresult _w_toolkit_wake(w_toolkit *toolkit);
#endif /* OSX_WIDGETS_TOOLKIT_H_ */
