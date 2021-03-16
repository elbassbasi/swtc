/*
 * Name:        toolkit.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_WIDGETS_TOOLKIT_H_
#define WIN32_WIDGETS_TOOLKIT_H_
#include "shell.h"
#include "menu.h"
#include "../controls/controls.h"
typedef struct _win32_theme {
	w_theme theme;

} _win32_theme;
typedef struct _w_toolkit {
	w_toolkit toolkit;
	int win32_version;
	int comctrl32_version;
	w_thread thread;
	int exit_code;
	unsigned exit_loop :1;
	unsigned IsDBLocale :1;
	unsigned captureChanged :1;
	unsigned lastNull :1;
	unsigned lastVirtual :1;
	unsigned xMouse :1;
	unsigned useOwnDC :1;
	unsigned init_startup :1;
	unsigned ENABLE_TVS_EX_FADEINOUTEXPANDOS :1;
	unsigned EXPLORER_THEME :1;
	int lastAscii;
	int lastKey;
	MSG msg;
	w_widget *widget_free;
	w_control *focusControl;
	HANDLE hactCtx;
	void *_BeginBufferedPaint;
	void *_EndBufferedPaint;
	wuchar wm_msg[WM_USER + 0x10];
	_w_cursor cursors[W_CURSOR_HAND + 1];
	_w_image images[5];
	w_taskbar taskbar;
	w_tray tray;
	_w_font systemFont;
	GpFontCollection *fontCollection;
	/*
	 * shell
	 */
	w_shell *activeShell;
	_w_shell *shells;
	int shells_count;
	//theme
	_win32_theme win32theme;
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
	struct _w_treeview_class class_treeview;
	_w_treeview_priv class_treeview_priv;
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
	 * internal memory
	 */
	size_t tmp_alloc;
	size_t tmp_length;
	char tmp[0];
} _w_toolkit;
extern _w_toolkit *win_toolkit;
extern const char *WindowClass;
extern const char *WindowShadowClass;
extern const char *WindowOwnDCClass;
#define _W_TOOLKIT(x) ((_w_toolkit*)x)
#define WIN32_VERSION (win_toolkit->win32_version)
#define _COMCTL32_VERSION (win_toolkit->comctrl32_version)
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
HMODULE _w_toolkit_load_library(const char *dll);
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
wresult _w_toolkit_messagebox_open(w_toolkit *toolkit,
		w_messagebox *messagebox);
#endif /* WIN32_WIDGETS_TOOLKIT_H_ */
