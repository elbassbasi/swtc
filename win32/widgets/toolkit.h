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
#include "tooltip.h"
#include "tray.h"
#include "../controls/controls.h"
enum {	 //
	HTHEME_BUTTON = 0, //
	HTHEME_LAST
};
typedef struct _win32_theme {
	w_theme theme;
	void *hthemes[HTHEME_LAST];
} _win32_theme;
#define BRUSHES_SIZE 32
#define ID_START 108
/* Private SWT Window Messages */
enum {	 //
	SWT_GETACCELCOUNT = WM_APP, //
	SWT_GETACCEL,
	SWT_KEYMSG,
	SWT_DESTROY,
	SWT_TRAYICONMSG,
	SWT_NULL,
	SWT_RUNASYNC,
};
typedef struct _win32_toolkit_alloc {
	WCHAR *text;
	size_t size;
} _win32_toolkit_alloc;
size_t _win32_toolkit_alloc_fn(void *user_data, size_t size, void **buf);
LRESULT CALLBACK messageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef struct _w_toolkit {
	w_toolkit toolkit;
	_w_app app;
	int win32_version;
	int comctrl32_version;
	int shell32_version;
	w_thread thread;
	int exit_code;
	unsigned exit_loop :1;
	unsigned IsDBLocale :1;
	unsigned captureChanged :1;
	unsigned lastNull :1;
	unsigned lastVirtual :1;
	unsigned lastDead :1;
	unsigned lastButton :3;
	unsigned xMouse :1;
	unsigned accelKeyHit :1;
	unsigned mnemonicKeyHit :1;
	unsigned useOwnDC :1;
	unsigned init_startup :1;
	unsigned ENABLE_TVS_EX_FADEINOUTEXPANDOS :1;
	unsigned EXPLORER_THEME :1;
	unsigned IsAppThemed :1;
	int lastMouse;
	int lastAscii;
	int lastKey;
	int lastTime;
	int clickCount;
	int scrollHRemainder;
	int scrollRemainder;
	int nextTrayId;
	int nextToolTipId;
	int resizeCount;
	RECT clickRect;
	HWND lastClickHwnd;
	HWND hwndMessage;
	MSG msg;
	w_widget *widget_free;
	w_control *focusControl;
	HANDLE hactCtx;
	void *_BeginBufferedPaint;
	void *_EndBufferedPaint;
	wuchar wm_msg[WM_USER + 0x10];
	_w_cursor cursors[W_CURSOR_HAND + 1];
	_w_image images[5];
	HBRUSH brushes[BRUSHES_SIZE];
	w_taskbar taskbar;
	w_tray tray;
	HFONT systemFont;
	GpFontCollection *fontCollection;
	HPALETTE hPalette;
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
	struct _w_toolkit_class class_toolkit;
	struct _w_toolkit_classes classes;
	_w_shell_priv class_shell_priv;
	_w_canvas_priv class_canvas_priv;
	_w_ccanvas_priv class_ccanvas_priv;
	_w_composite_priv class_composite_priv;
	_w_menu_priv class_menu_priv;
	_w_treeview_priv class_treeview_priv;
	_w_listview_priv class_listview_priv;
	_w_sash_priv class_sash_priv;
	_w_button_priv class_button_priv;
	_w_label_priv class_label_priv;
	_w_textedit_priv class_textedit_priv;
	_w_progressbar_priv class_progressbar_priv;
	_w_groupbox_priv class_groupbox_priv;
	_w_combobox_priv class_combobox_priv;
	_w_coolbar_priv class_coolbar_priv;
	_w_datetime_priv class_datetime_priv;
	_w_slider_priv class_slider_priv;
	_w_spinner_priv class_spinner_priv;
	_w_tabview_priv class_tabview_priv;
	_w_toolbar_priv class_toolbar_priv;
	_w_tray_priv class_tray_priv;
	_w_tooltip_priv class_tooltip_priv;
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
extern w_widget_init_class win_toolkit_classes_init[_W_CLASS_LAST];
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
