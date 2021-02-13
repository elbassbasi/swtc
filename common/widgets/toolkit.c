/*
 * toolkit.c
 *
 *  Created on: 29 avr. 2020
 *      Author: Azeddine El Bassbasi
 */
#include <swtc.h>
w_toolkit *w_toolkit_default = 0;
w_toolkit* w_toolkit_get_default() {
	return w_app_get_default_toolkit(w_app_get());
}
wresult w_toolkit_set_default(w_toolkit *toolkit) {
	return w_app_set_default_toolkit(w_app_get(),toolkit);
}
struct _w_widget_class* w_toolkit_get_class(w_toolkit *toolkit,
		wushort clazz_id) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_class(toolkit, clazz_id);
}
wresult w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->async_exec(toolkit, function, args);
}
wresult w_toolkit_beep(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->beep(toolkit);
}
w_shell* w_toolkit_get_active_shell(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_active_shell(toolkit);
}
w_menu* w_toolkit_get_menubar(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_menubar(toolkit);
}
wresult w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *bounds) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_bounds(toolkit, bounds);
}
wresult w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_client_area(toolkit, clientArea);
}
w_control* w_toolkit_get_cursor_control(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_cursor_control(toolkit);
}
wresult w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_cursor_location(toolkit, location);
}
size_t w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_cursor_sizes(toolkit, sizes,
			length);
}
int w_toolkit_get_dismissal_alignment(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_dismissal_alignment(toolkit);
}
int w_toolkit_get_double_click_time(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_double_click_time(toolkit);
}
w_control* w_toolkit_get_focus_control(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_focus_control(toolkit);
}
wresult w_toolkit_get_high_contrast(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_high_contrast(toolkit);
}
int w_toolkit_get_icon_depth(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_icon_depth(toolkit);
}
size_t w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_icon_sizes(toolkit, sizes, length);
}
void w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	w_iterator_close(shells);
	return W_TOOLKIT_GET_CLASS(toolkit)->get_shells(toolkit, shells);
}
w_color w_toolkit_get_system_color(w_toolkit *toolkit, wuint id) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_color(toolkit, id);
}
w_cursor* w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint id) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_cursor(toolkit, id);
}
w_font* w_toolkit_get_system_font(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_font(toolkit);
}
wresult w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
		w_image **image) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_image(toolkit, id, image);
}
w_menu* w_toolkit_get_system_menu(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_menu(toolkit);
}
struct w_taskbar* w_toolkit_get_system_taskbar(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_taskbar(toolkit);
}
struct w_tray* w_toolkit_get_system_tray(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_tray(toolkit);
}
w_theme* w_toolkit_get_theme(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_theme(toolkit);
}
w_thread* w_toolkit_get_thread(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_thread(toolkit);
}
wuint64 w_toolkit_get_thread_id(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_thread_id(toolkit);
}
wresult w_toolkit_get_touch_enabled(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->get_touch_enabled(toolkit);
}
wresult w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
		w_point *result, w_point *point) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->map_0(toolkit, from, to, result, point);
}
wresult w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
		w_rect *result, w_rect *rectangle) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->map_1(toolkit, from, to, result,
			rectangle);
}
wresult w_toolkit_post(w_toolkit *toolkit, w_event *event) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->post(toolkit, event);
}
wresult w_toolkit_post_quit(w_toolkit *toolkit, int quit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->post_quit(toolkit, quit);
}
wresult w_toolkit_read_and_dispatch(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->dispatch(toolkit);
}
wresult w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *point) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->set_cursor_location(toolkit, point);
}
wresult w_toolkit_set_theme(w_toolkit *toolkit, w_theme *theme) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->set_theme(toolkit, theme);
}
int w_toolkit_run(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->run(toolkit);
}
wresult w_toolkit_sleep(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->read(toolkit);
}
wresult w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->sync_exec(toolkit, function, args);
}
wresult w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->timer_exec(toolkit, milliseconds,
			function, args);
}
wresult w_toolkit_update(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->update(toolkit);
}
wresult w_toolkit_wake(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->wake(toolkit);
}
/*
 *
 */
int w_toolkit_register_type(w_toolkit *toolkit, const char *formatName) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->register_type(toolkit, formatName);
}
wresult w_toolkit_open_clipboard(w_toolkit *toolkit, w_clipboard *clipboard,
		int clipboards) {
	if (toolkit == 0)
		toolkit = w_toolkit_get_default();
	return W_TOOLKIT_GET_CLASS(toolkit)->open_clipboard(toolkit, clipboard,
			clipboards);
}
