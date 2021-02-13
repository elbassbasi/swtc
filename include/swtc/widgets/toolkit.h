/*
 * Name:        toolkit.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_TOOLKIT_H_
#define SWT_WIDGETS_TOOLKIT_H_
#include "../runtime/runtime.h"
#include "theme.h"
#ifdef __cplusplus
extern "C" {
#endif
/**
 * w_disposable
 */
typedef struct w_disposable {
	void *ptr; //used in c++
	struct w_disposable_class *clazz;
	struct w_disposable *next;
} w_disposable;
struct w_disposable_class {
	void (*dispose)(w_disposable *disposable);
};
/**
 * w_toolkit
 */
struct w_toolkit {
	w_disposable disposable;
};
#define W_TOOLKIT(x) ((w_toolkit*)x)
struct _w_toolkit_class {
	struct w_disposable_class disposable;
	wresult (*check_widget)(w_toolkit *toolkit, w_widget *widget);
	wresult (*check_widgetdata)(w_toolkit *toolkit, w_widgetdata *widgetdata);
	struct _w_widget_class* (*get_class)(w_toolkit *toolkit, wushort clazz_id);
	w_theme* (*get_theme)(w_toolkit *toolkit);
	wresult (*async_exec)(w_toolkit *toolkit, w_thread_start function,
			void *args);
	wresult (*beep)(w_toolkit *toolkit);
	w_shell* (*get_active_shell)(w_toolkit *toolkit);
	w_menu* (*get_menubar)(w_toolkit *toolkit);
	wresult (*get_bounds)(w_toolkit *toolkit, w_rect *bounds);
	wresult (*get_client_area)(w_toolkit *toolkit, w_rect *clientArea);
	w_control* (*get_cursor_control)(w_toolkit *toolkit);
	wresult (*get_cursor_location)(w_toolkit *toolkit, w_point *location);
	size_t (*get_cursor_sizes)(w_toolkit *toolkit, w_size *sizes,
			size_t length);
	int (*get_dismissal_alignment)(w_toolkit *toolkit);
	int (*get_double_click_time)(w_toolkit *toolkit);
	w_control* (*get_focus_control)(w_toolkit *toolkit);
	wresult (*get_high_contrast)(w_toolkit *toolkit);
	int (*get_icon_depth)(w_toolkit *toolkit);
	size_t (*get_icon_sizes)(w_toolkit *toolkit, w_size *sizes, size_t length);
	void (*get_shells)(w_toolkit *toolkit, w_iterator *shells);
	w_color (*get_system_color)(w_toolkit *toolkit, wuint id);
	w_cursor* (*get_system_cursor)(w_toolkit *toolkit, wuint id);
	w_font* (*get_system_font)(w_toolkit *toolkit);
	wresult (*get_system_image)(w_toolkit *toolkit, wuint id, w_image **image);
	w_menu* (*get_system_menu)(w_toolkit *toolkit);
	struct w_taskbar* (*get_system_taskbar)(w_toolkit *toolkit);
	struct w_tray* (*get_system_tray)(w_toolkit *toolkit);
	w_thread* (*get_thread)(w_toolkit *toolkit);
	wuint64 (*get_thread_id)(w_toolkit *toolkit);
	wresult (*get_touch_enabled)(w_toolkit *toolkit);
	wresult (*map_0)(w_toolkit *toolkit, w_control *from, w_control *to,
			w_point *result, w_point *point);
	wresult (*map_1)(w_toolkit *toolkit, w_control *from, w_control *to,
			w_rect *result, w_rect *rectangle);
	wresult (*post)(w_toolkit *toolkit, w_event *event);
	wresult (*post_quit)(w_toolkit *toolkit,int quit);
	wresult (*dispatch)(w_toolkit *toolkit);
	wresult (*set_cursor_location)(w_toolkit *toolkit, w_point *point);
    wresult (*set_theme)(w_toolkit *toolkit,w_theme* theme);
	int (*run)(w_toolkit *toolkit);
	wresult (*read)(w_toolkit *toolkit);
	wresult (*sync_exec)(w_toolkit *toolkit, w_thread_start function,
			void *args);
	wresult (*timer_exec)(w_toolkit *toolkit, wuint milliseconds,
			w_thread_start function, void *args);
	wresult (*update)(w_toolkit *toolkit);
	wresult (*wake)(w_toolkit *toolkit);
	/**
	 *
	 */
	int (*register_type)(w_toolkit *toolkit, const char *formatName);
	wresult (*open_clipboard)(w_toolkit *toolkit, w_clipboard *clipboard,int clipboards);
	/*
	 * dialogs
	 */
	wresult (*messagebox_open)(w_toolkit *toolkit, w_messagebox *messagebox);
	wresult (*dialog_color_open)(w_toolkit *toolkit,
			w_dialog_color *dialog_color);
	wresult (*dialog_font_open)(w_toolkit *toolkit, w_dialog_font *dialog_font);
	wresult (*dialog_directory_open)(w_toolkit *toolkit,
			w_dialog_directory *dialog_directory);
	wresult (*dialog_file_open)(w_toolkit *toolkit, w_dialog_file *dialog_file);
};
#define W_TOOLKIT_CLASS(x) ((struct _w_toolkit_class*)x)
#define W_TOOLKIT_GET_CLASS(x) W_TOOLKIT_CLASS(((w_disposable*)x)->clazz)

SWT_PUBLIC struct _w_widget_class* w_toolkit_get_class(w_toolkit *toolkit,
		wushort clazz_id);
SWT_PUBLIC wresult w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
		void *args);
SWT_PUBLIC wresult w_toolkit_beep(w_toolkit *toolkit);
SWT_PUBLIC w_shell* w_toolkit_get_active_shell(w_toolkit *toolkit);
SWT_PUBLIC w_menu* w_toolkit_get_menubar(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *bounds);
SWT_PUBLIC wresult w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea);
SWT_PUBLIC w_control* w_toolkit_get_cursor_control(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location);
SWT_PUBLIC size_t w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length);
SWT_PUBLIC int w_toolkit_get_dismissal_alignment(w_toolkit *toolkit);
SWT_PUBLIC int w_toolkit_get_double_click_time(w_toolkit *toolkit);
SWT_PUBLIC w_control* w_toolkit_get_focus_control(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_get_high_contrast(w_toolkit *toolkit);
SWT_PUBLIC int w_toolkit_get_icon_depth(w_toolkit *toolkit);
SWT_PUBLIC size_t w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length);
SWT_PUBLIC void w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells);
SWT_PUBLIC w_color w_toolkit_get_system_color(w_toolkit *toolkit, wuint id);
SWT_PUBLIC w_cursor* w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint id);
SWT_PUBLIC w_font* w_toolkit_get_system_font(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
		w_image **image);
SWT_PUBLIC w_menu* w_toolkit_get_system_menu(w_toolkit *toolkit);
SWT_PUBLIC struct w_taskbar* w_toolkit_get_system_taskbar(w_toolkit *toolkit);
SWT_PUBLIC struct w_tray* w_toolkit_get_system_tray(w_toolkit *toolkit);
SWT_PUBLIC w_thread* w_toolkit_get_thread(w_toolkit *toolkit);
SWT_PUBLIC wuint64 w_toolkit_get_thread_id(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_get_touch_enabled(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
		w_point *result, w_point *point);
SWT_PUBLIC wresult w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
		w_rect *result, w_rect *rectangle);
SWT_PUBLIC wresult w_toolkit_post(w_toolkit *toolkit, w_event *event);
SWT_PUBLIC wresult w_toolkit_post_quit(w_toolkit *toolkit,int quit);
SWT_PUBLIC wresult w_toolkit_read_and_dispatch(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *point);
SWT_PUBLIC int w_toolkit_run(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_sleep(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args);
SWT_PUBLIC wresult w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args);
SWT_PUBLIC wresult w_toolkit_update(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_wake(w_toolkit *toolkit);
SWT_PUBLIC w_theme* w_toolkit_get_theme(w_toolkit *toolkit);
SWT_PUBLIC wresult w_toolkit_set_theme(w_toolkit *toolkit,w_theme* theme);
SWT_PUBLIC int w_toolkit_register_type(w_toolkit *toolkit,
		const char *formatName);
SWT_PUBLIC wresult w_toolkit_open_clipboard(w_toolkit *toolkit,
		w_clipboard *clipboard,int clipboards);

#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_TOOLKIT_H_ */
