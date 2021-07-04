/*
 * Name:        control.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_WIDGETS_CONTROL_H_
#define WIN32_WIDGETS_CONTROL_H_
#include "widget.h"
/*
 * states
 */
#define STATE_RESIZE_DEFERRED (1 << (STATE_WIDGET_END + 0))
#define STATE_RESIZE_OCCURRED (1 << (STATE_WIDGET_END + 1))
#define STATE_LAYOUT_DATA_LOCALE (1 << (STATE_WIDGET_END + 2))
#define STATE_IGNORE_WM_CHANGEUISTATE (1 << (STATE_WIDGET_END + 3))
#define STATE_THEME_BACKGROUND (1 << (STATE_WIDGET_END + 4))
#define STATE_PARENT_BACKGROUND (1 << (STATE_WIDGET_END + 5))
#define STATE_DISABLED (1 << (STATE_WIDGET_END + 6))
#define STATE_HIDDEN (1 << (STATE_WIDGET_END + 7))
#define STATE_DRAW_BACKGROUND (1 << (STATE_WIDGET_END + 8))
#define STATE_CONTROL_END (STATE_WIDGET_END + 10)
/*
 * control
 */
typedef struct _w_accel_id {
	HMENU hMenu;
	wushort flags;
	wushort sub_id;
	int accelerator;
} _w_accel_id;
typedef struct _w_control {
	_w_widget widget;
	w_menu *menu;
	w_cursor *cursor;
	w_font *font;
	HMENU activeMenu;
	wushort drawCount;
	wuchar backgroundAlpha;
	w_color background;
	w_color foreground;
	_w_image backgroundImage;
	w_array *ids;
	HACCEL hAccel;
} _w_control;
#define _W_CONTROL(x) ((_w_control*)x)
/*
 * private
 */
typedef wresult (*dispatch_message)(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
struct _w_control_priv {
	_w_widget_priv widget;
	dispatch_message messages[_WM_LAST];
	WNDPROC def_window_proc;
	WNDPROC* (*get_def_window_proc)(w_control *control, _w_control_priv *priv);
	HWND (*handle_top)(w_control *control);
	HWND (*handle_border)(w_control *control);
	wresult (*create_widget)(w_control *control, _w_control_priv *priv);
	wuint64 (*check_style)(w_widget *widget, wuint64 style);
	wresult (*check_orientation)(w_control *control, w_widget *parent,
			_w_control_priv *priv);
	wresult (*create_handle)(w_control *control, _w_control_priv *priv);
	wresult (*subclass)(w_control *control, _w_control_priv *priv);
	wresult (*unsubclass)(w_control *control, _w_control_priv *priv);
	wresult (*check_background)(w_control *control, _w_control_priv *priv);
	wresult (*check_buffered)(w_control *control, _w_control_priv *priv);
	wresult (*check_composited)(w_control *control, _w_control_priv *priv);
	wresult (*set_default_font)(w_control *control, _w_control_priv *priv);
	wresult (*check_mirrored)(w_control *control, _w_control_priv *priv);
	wresult (*check_border)(w_control *control, _w_control_priv *priv);
	wresult (*check_gesture)(w_control *control, _w_control_priv *priv);
	void (*draw_background)(w_control *control, HDC hDC, RECT *rect, int pixel,
			int tx, int ty, _w_control_priv *priv);
	void (*fill_background)(w_control *control, HDC hDC, int pixel, RECT *rect);
	int (*get_background_pixel)(w_control *control, _w_control_priv *priv);
	int (*get_foreground_pixel)(w_control *control, _w_control_priv *priv);
	int (*default_background)(w_control *control, _w_control_priv *priv);
	int (*default_foreground)(w_control *control, _w_control_priv *priv);
	HFONT (*default_font)(w_control *control, _w_control_priv *priv);
	w_control* (*find_background_control)(w_control *control,
			_w_control_priv *priv);
	w_control* (*find_image_control)(w_control *control, _w_control_priv *priv);
	wresult (*set_background)(w_control *control, _w_control_priv *priv);
	HWND (*widget_parent)(w_control *control, _w_control_priv *priv);
	DWORD (*widget_style)(w_control *control, _w_control_priv *priv);
	DWORD (*widget_extstyle)(w_control *control, _w_control_priv *priv);
	WCHAR* (*window_class)(w_control *control, _w_control_priv *priv);
	wresult (*compute_size)(w_widget *widget, w_event_compute_size *e,
			_w_control_priv *priv);
	wresult (*get_client_area)(w_widget *widget, w_event_client_area *e,
			_w_control_priv *priv);
	wresult (*compute_trim)(w_widget *widget, w_event_compute_trim *e,
			_w_control_priv *priv);
	wresult (*set_cursor_0)(w_control *control, _w_control_priv *priv);
	w_cursor* (*find_cursor)(w_control *control, _w_control_priv *priv);
	wresult (*translate_accelerator)(w_control *control, MSG *msg,
			_w_control_priv *priv);
	wresult (*translate_mnemonic)(w_control *control, MSG *msg,
			_w_control_priv *priv);
	wresult (*translate_traversal)(w_control *control, MSG *msg,
			_w_control_priv *priv);
	wresult (*traverse)(w_control *control, w_event_key *event,
			_w_control_priv *priv);
	wresult (*traverse_mnemonic)(w_control *control, int key,
			_w_control_priv *priv);
	wresult (*mnemonic_hit)(w_control *control, int key, _w_control_priv *priv);
};
#define _W_CONTROL_PRIV(x) ((_w_control_priv*)x)
#define _W_CONTROL_GET_PRIV(x) ((_w_control_priv*)W_WIDGET_GET_CLASS(x)->platformPrivate)

typedef struct _w_ccanvas_priv {
	_w_control_priv control;
} _w_ccanvas_priv;
/*
 * functions
 */
wresult _w_control_post_event_platform(w_widget *widget, _w_event_platform *ee,
		_w_control_priv *priv);
wresult _w_control_post_event(w_widget *widget, w_event *e,int flags);
LRESULT CALLBACK _w_control_window_proc(HWND hWnd, UINT message, WPARAM wParam,
		LPARAM lParam);
int _w_control_new_id(w_control *control, _w_accel_id **id);
int _w_control_remove_id(w_control *control, int index);
int _w_control_create_accelerators(w_control *control);
void _w_control_destroy_accelerators(w_control *control);
wresult _w_control_call_window_proc(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _w_control_check_background(w_control *control, _w_control_priv *priv);
wresult _w_control_check_border(w_control *control, _w_control_priv *priv);
wresult _w_control_check_buffered(w_control *control, _w_control_priv *priv);
wresult _w_control_check_composited(w_control *control, _w_control_priv *priv);
wresult _w_control_check_gesture(w_control *control, _w_control_priv *priv);
wresult _w_control_check_mirrored(w_control *control, _w_control_priv *priv);
wresult _w_control_check_orientation(w_control *control, w_composite *parent,
		_w_control_priv *priv);
wuint64 _w_control_check_style(w_widget *widget, wuint64 style);
wresult _w_control_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv);
wresult _w_control_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv);
wresult _w_control_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event);
wresult _w_control_create_dragsource(w_control *control,
		w_dragsource *dragsource, wuint64 style,
		w_widget_post_event_proc post_event);
wresult _w_control_create_droptarget(w_control *control,
		w_droptarget *droptarget, wuint64 style,
		w_widget_post_event_proc post_event);
wresult _w_control_create_handle(w_control *control, _w_control_priv *priv);
wresult _w_control_create_widget(w_control *control, _w_control_priv *priv);
wresult _w_control_dispose(w_widget *widget);
wresult _w_control_drag_detect(w_control *control, w_event_mouse *event);
wresult _w_control_force_focus(w_control *control);
HBRUSH _w_control_find_brush(ULONG_PTR value, int lbStyle);
wresult _w_control_get_accessible(w_control *control,
		w_accessible **accessible);
wresult _w_control_get_background(w_control *control, w_color *background);
wresult _w_control_get_background_image(w_control *control, w_image *image);
wresult _w_control_get_border_width(w_control *control);
wresult _w_control_get_bounds(w_control *control, w_point *location,
		w_size *size);
wresult _w_control_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv);
wresult _w_control_get_cursor(w_control *control, w_cursor **cursor);
wresult _w_control_get_drag_detect(w_control *control);
wresult _w_control_get_drawing(w_control *control);
wresult _w_control_get_enabled(w_control *control);
wresult _w_control_get_font(w_control *control, w_font **font);
wresult _w_control_get_foreground(w_control *control, w_color *foreground);
wresult _w_control_get_graphics(w_control *control, w_graphics *gc);
wresult _w_control_get_layout_data(w_control *control, void **data);
wresult _w_control_get_menu(w_control *control, w_menu **menu);
wresult _w_control_get_orientation(w_control *control);
wresult _w_control_get_parent(w_control *control, w_composite **parent);
wresult _w_control_get_region(w_control *control, w_region *region);
wresult _w_control_get_shell(w_widget *control, w_shell **shell);
wresult _w_control_get_tab(w_control *control);
wresult _w_control_get_text_direction(w_control *control);
wresult _w_control_get_tooltip_text(w_control *control, w_alloc alloc,
		void *user_data, int enc);
wresult _w_control_get_touch_enabled(w_control *control);
wresult _w_control_get_visible(w_control *control);
HWND _w_control_handle(w_control *control);
void _w_control_init_graphics(w_control *control, w_graphics *gc, HDC hDC,
		int style);
wresult _w_control_is_enabled(w_control *control);
wresult _w_control_is_focus_control(w_control *control);
wresult _w_control_is_reparentable(w_control *control);
wresult _w_control_is_visible(w_control *control);
wresult _w_control_kill_timer(w_control *control, wushort id);
wresult _w_control_move_above(w_control *control, w_control *_control);
wresult _w_control_move_below(w_control *control, w_control *_control);
wresult _w_control_new_layout_data(w_control *control, void **data,
		size_t size);
wresult _w_control_pack(w_control *control, int flags);
wresult _w_control_print(w_control *control, w_graphics *gc);
wresult _w_control_request_layout(w_control *control);
wresult _w_control_redraw(w_control *control, w_rect *rect, int all);
wresult _w_control_set_background(w_control *control, w_color color);
wresult _w_control_set_background_0(w_control *control, _w_control_priv *priv);
wresult _w_control_set_background_image(w_control *control, w_image *image);
wresult _w_control_set_bounds(w_control *control, w_point *location,
		w_size *size);
wresult _w_control_set_capture(w_control *control, int capture);
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor);
wresult _w_control_set_default_font(w_control *control, _w_control_priv *priv);
wresult _w_control_set_drag_detect(w_control *control, int dragDetect);
wresult _w_control_set_enabled(w_control *control, int enabled);
wresult _w_control_set_focus(w_control *control);
wresult _w_control_set_font(w_control *control, w_font *font);
wresult _w_control_set_foreground(w_control *control, w_color color);
wresult _w_control_set_menu(w_control *control, w_menu *menu);
wresult _w_control_set_orientation(w_control *control, int orientation);
wresult _w_control_set_parent(w_control *control, w_composite *parent);
wresult _w_control_set_redraw(w_control *control, int redraw);
wresult _w_control_set_region(w_control *control, w_region *region);
wresult _w_control_set_tab(w_control *control, int tab);
wresult _w_control_set_text_direction(w_control *control, int textDirection);
wresult _w_control_set_timer(w_control *control, wint64 ms, wushort id);
wresult _w_control_set_tooltip_text(w_control *control, const char *text,
		int length, int enc);
wresult _w_control_set_touch_enabled(w_control *control, int enabled);
wresult _w_control_set_visible(w_control *control, int visible);
wresult _w_control_subclass(w_control *control, _w_control_priv *priv);
wresult _w_control_to_control(w_control *control, w_point *result,
		w_point *point);
wresult _w_control_to_display(w_control *control, w_point *result,
		w_point *point);
wresult _w_control_translate_accelerator(w_control *control, MSG *msg,
		_w_control_priv *priv);
wresult _w_control_traverse(w_control *control, int traversal,
		w_event_key *event);
wresult _w_control_unsubclass(w_control *control, _w_control_priv *priv);
wresult _w_control_update(w_control *control);
DWORD _w_control_widget_extstyle(w_control *control, _w_control_priv *priv);
HWND _w_control_widget_parent(w_control *control, _w_control_priv *priv);
DWORD _w_control_widget_style(w_control *control, _w_control_priv *priv);
WCHAR* _w_control_window_class(w_control *control, _w_control_priv *priv);
void _w_control_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_control_class *clazz);
void _w_ccanvas_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_ccanvas_class *clazz);
/*
 * messages
 */
wresult _CONTROL_WM_NULL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_DESTROY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_CREATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_CTLCOLOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_CHANGEUISTATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_COMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_DRAWITEM(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_GETOBJECT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_GETDLGCODE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_HELP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_HSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_INPUTLANGCHANGE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_MEASUREITEM(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_MOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_NCHITTEST(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_NOTIFY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_SYSCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_TABLET_FLICK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_TOUCH(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_TIMER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_VSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_WINDOWPOSCHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_WINDOWPOSCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_CTLCOLORCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _CONTROL_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
#endif /* WIN32_WIDGETS_CONTROL_H_ */
