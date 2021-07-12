/*
 * Name:        control.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_WIDGETS_CONTROL_H_
#define GTK_WIDGETS_CONTROL_H_
#include "widget.h"
/*
 * states
 */
#define STATE_CANVAS (1 << (STATE_WIDGET_END + 1))
#define STATE_MENU (1 << (STATE_WIDGET_END + 2))
#define STATE_DRAG_DETECT (1 << (STATE_WIDGET_END + 3))
#define STATE_TIMER_START (1 << (STATE_WIDGET_END + 4))
#define STATE_THEME_BACKGROUND (1 << (STATE_WIDGET_END + 5))
#define STATE_CHECK_SUBWINDOW (1 << (STATE_WIDGET_END + 6))
#define STATE_LAYOUT_DATA_LOCALE (1 << (STATE_WIDGET_END + 7))
#define STATE_DRAW_REGION (1 << (STATE_WIDGET_END + 8))
#define STATE_ZERO_WIDTH  (1 << (STATE_WIDGET_END + 9))
#define STATE_ZERO_HEIGHT (1 << (STATE_WIDGET_END + 10))
#define STATE_BACKGROUND (1 << (STATE_WIDGET_END + 11))
#define STATE_PARENT_BACKGROUND (1 << (STATE_WIDGET_END + 12))
#define STATE_DISABLED (1 << (STATE_WIDGET_END + 13))
#define STATE_ENABLE_WINDOW (1 << (STATE_WIDGET_END + 14))
#define STATE_REDRAW_WINDOW (1 << (STATE_WIDGET_END + 15))
#define STATE_TAB_LIST (1 << (STATE_WIDGET_END + 16))
#define STATE_LAYOUT_NEEDED (1 << (STATE_WIDGET_END + 17))
#define STATE_LAYOUT_CHANGED (1 << (STATE_WIDGET_END + 18))
#define STATE_LAYOUT_CHILD (1 << (STATE_WIDGET_END + 19))
#define STATE_HIDDEN (1 << (STATE_WIDGET_END + 20))
#define STATE_FOREGROUND (1 << (STATE_WIDGET_END + 21))
#define STATE_DROPTARGET_IS_ACTIVE (1 << (STATE_WIDGET_END + 22))
#define STATE_CONTROL_END (STATE_WIDGET_END + 23)
/*
 * zorder
 */
#define ZORDER_ABOVE (1 << 0)
#define ZORDER_FIX_RELATION (1 << 1)
#define ZORDER_FIX_CHILDREN (1 << 2)
/*
 * redraw
 */
#define REDRAW_REDRAWALL (1 << 0)
#define REDRAW_ALL (1 << 1)
#define REDRAW_TRIM (1 << 2)
/*
 * update
 */
#define UPDATE_ALL (1 << 0)
#define UPDATE_FLUSH (1 << 1)
/*
 * control
 */
typedef struct _w_control_timer {
	w_control *control;
	struct _w_control_timer *next;
	wushort id;
} _w_control_timer;
typedef struct _w_control {
	_w_widget widget;
	w_menu *menu;
	w_cursor *cursor;
	PangoFontDescription *font;
	_w_control_timer timer;
	_w_image backgroundImage;
	w_dragsource *dragsource;
	w_droptarget *droptarget;
	char *tooltiptext;
	w_point lastInput;
	int backgroundAlpha;
	w_color foreground;
	w_color background;
} _w_control;
#define _W_CONTROL(x) ((_w_control*)x)
/*
 * private
 */
#define _W_CONTROL_PRIV(x) ((_w_control_priv*)x)
#define _W_CONTROL_GET_PRIV(x) ((_w_control_priv*)W_WIDGET_GET_CLASS(x)->platformPrivate)
struct _w_control_priv {
	_w_widget_priv widget;
	/*
	 * handles
	 */
	GtkWidget* (*handle_fixed)(w_widget *control, _w_control_priv *priv);
	GtkWidget* (*handle_client)(w_widget *control, _w_control_priv *priv);
	GtkWidget* (*handle_focus)(w_widget *control, _w_control_priv *priv);
	GtkWidget* (*handle_event)(w_widget *control, _w_control_priv *priv);
	GtkWidget* (*handle_enterexit)(w_widget *control, _w_control_priv *priv);
	GtkWidget* (*handle_paint)(w_widget *control, _w_control_priv *priv);
	GtkWidget* (*handle_font)(w_widget *control, _w_control_priv *priv);
	GtkIMContext* (*handle_im)(w_widget *control, _w_control_priv *priv);
	/*
	 * window
	 */
	GdkWindow* (*window_paint)(w_widget *control, _w_control_priv *priv);
	GdkWindow* (*window_event)(w_widget *control, _w_control_priv *priv);
	GdkWindow* (*window_enable)(w_widget *control, _w_control_priv *priv);
	GdkWindow* (*window_redraw)(w_widget *control, _w_control_priv *priv);
	/*
	 * check
	 */
	void (*check_border)(w_control *control, _w_control_priv *priv);
	void (*check_buffered)(w_control *control, _w_control_priv *priv);
	void (*check_background)(w_control *control, _w_control_priv *priv);
	void (*check_foreground)(w_control *control, _w_control_priv *priv);
	void (*check_mirrored)(w_control *control, _w_control_priv *priv);
	wresult (*check_subwindow)(w_control *control, _w_control_priv *priv);
	/*
	 * background
	 */
	void (*set_parent_background)(w_control *control, _w_control_priv *priv);
	void (*show_widget)(w_control *control, _w_control_priv *priv);
	void (*set_initial_bounds)(w_control *control, _w_control_priv *priv);
	void (*set_relations)(w_control *control, _w_control_priv *priv);
	wresult (*set_zorder)(w_control *control, w_control *sibling, int flags,
			_w_control_priv *priv);
	wresult (*set_bounds_0)(w_control *control, w_point *location, w_size *size,
			_w_control_priv *priv);
	wresult (*set_cursor_0)(w_control *control, GdkCursor *cursor,
			_w_control_priv *priv);
	void (*set_font_description)(w_control *control, PangoFontDescription *font,
			_w_control_priv *priv);
	void (*mark_layout)(w_control *control, int flags, _w_control_priv *priv);
	void (*update_layout)(w_control *control, int flags, _w_control_priv *priv);
	void (*move_handle)(w_control *control, w_point *position,
			_w_control_priv *priv);
	void (*move_children)(w_control *control, int oldWidth,
			_w_control_priv *priv);
	void (*resize_handle)(w_control *control, w_size *size,
			_w_control_priv *priv);
	int (*get_client_width)(w_control *control, _w_control_priv *priv);
	w_control* (*find_background_control)(w_control *control,
			_w_control_priv *priv);
	wresult (*draw_background)(w_control *_this, w_control *control,
			cairo_t *cr, cairo_region_t *region, w_rect *rect,
			_w_control_priv *priv);
	void (*draw_widget)(w_control *control, w_graphics *gc,
			_w_control_priv *priv);
	void (*redraw_widget)(w_control *control, w_rect *rect, int flags,
			_w_control_priv *priv);
	wresult (*update_0)(w_control *control, int flags, _w_control_priv *priv);
	void (*force_resize)(w_control *control, _w_control_priv *priv);
	wresult (*has_focus)(w_control *control, _w_control_priv *priv);
	wresult (*get_imcaret_pos)(w_control *control, w_point *pos,
			_w_control_priv *priv);
	wresult (*translate_traversal)(w_control *control, GdkEventKey *keyEvent,
			_w_control_priv *priv);
	wresult (*send_leave_notify)(w_control *control, _w_control_priv *priv);
	gboolean (*is_focus_handle)(w_control *control, GtkWidget *widget,
			_w_control_priv *priv);
};
typedef struct _w_ccanvas_priv {
	_w_control_priv control;
} _w_ccanvas_priv;
/*
 * functions
 */
void gtk_widget_set_align(GtkWidget *widget, GtkAlign hAlign, GtkAlign vAlign);
void _w_control_check_background(w_control *control, _w_control_priv *priv);
void _w_control_check_border(w_control *control, _w_control_priv *priv);
void _w_control_check_buffered(w_control *control, _w_control_priv *priv);
void _w_control_check_foreground(w_control *control, _w_control_priv *priv);
void _w_control_check_mirrored(w_control *control, _w_control_priv *priv);
wresult _w_control_check_subwindow(w_control *control, _w_control_priv *priv);
wresult _w_control_compute_native_size(w_widget *widget, GtkWidget *h,
		w_event_compute_size *e, _w_control_priv *priv);
wresult _w_control_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event);
wresult _w_control_create_dragsource(w_control *control,
		w_dragsource *dragsource, wuint64 style,
		w_widget_post_event_proc post_event);
wresult _w_control_create_droptarget(w_control *control,
		w_droptarget *droptarget, wuint64 style,
		w_widget_post_event_proc post_event);
wresult _w_control_create_widget(w_widget *widget, _w_control_priv *priv);
wresult _w_control_dispose(w_widget *widget);
gboolean _w_control_drag_detect_2(w_control *control, int x, int y,
		gboolean filter, gboolean dragOnTimeout, gboolean *consume);
wresult _w_control_drag_detect(w_control *control, w_event_mouse *event);
wresult _w_control_draw_draw_gripper(w_control *control, w_graphics *gc,
		w_rect *rect, int vertical, _w_control_priv *priv);
wresult _w_control_force_focus(w_control *control);
wresult _w_control_get_accessible(w_control *control,
		w_accessible **accessible);
wresult _w_control_get_background(w_control *control, w_color *background);
wresult _w_control_get_background_image(w_control *control, w_image *image);
wresult _w_control_get_border_width(w_control *control);
wresult _w_control_get_bounds(w_control *control, w_point *location,
		w_size *size);
wresult _w_control_get_cursor(w_control *control, w_cursor **cursor);
wresult _w_control_get_drag_detect(w_control *control);
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
void _w_control_get_thickness(GtkWidget *widget, w_point *thickness);
wresult _w_control_get_tooltip_text(w_control *control, w_alloc alloc,
		void *user_data, int enc);
wresult _w_control_get_touch_enabled(w_control *control);
wresult _w_control_get_visible(w_control *control);
GtkWidget* _w_control_handle_enterexit(w_widget *control,
		_w_control_priv *priv);
GtkWidget* _w_control_handle_top(w_widget *control, _w_control_priv *priv);
GtkWidget* _w_control_handle_paint(w_widget *control, _w_control_priv *priv);
GtkIMContext* _w_control_handle_im(w_widget *control, _w_control_priv *priv);
wresult _w_control_has_focus(w_control *control, _w_control_priv *priv);
void _w_control_hook_events(w_widget *widget, _w_control_priv *priv);
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
wresult _w_control_set_background_pixmap(w_control *control, GdkPixbuf *image,
		_w_control_priv *priv);
wresult _w_control_set_background_image(w_control *control, w_image *image);
wresult _w_control_set_bounds(w_control *control, w_point *location,
		w_size *size);
wresult _w_control_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_priv *priv);
wresult _w_control_set_capture(w_control *control, int capture);
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor);
wresult _w_control_set_drag_detect(w_control *control, int dragDetect);
wresult _w_control_set_enabled(w_control *control, int enabled);
wresult _w_control_set_focus(w_control *control);
wresult _w_control_set_font(w_control *control, w_font *font);
wresult _w_control_set_foreground(w_control *control, w_color color);
void _w_control_set_initial_bounds(w_control *control, _w_control_priv *priv);
wresult _w_control_set_menu(w_control *control, w_menu *menu);
wresult _w_control_set_orientation(w_control *control, int orientation);
wresult _w_control_set_parent(w_control *control, w_composite *parent);
void _w_control_set_parent_background(w_control *control,
		_w_control_priv *priv);
wresult _w_control_set_redraw(w_control *control, int redraw);
wresult _w_control_set_region(w_control *control, w_region *region);
void _w_control_set_relations(w_control *control, _w_control_priv *priv);
wresult _w_control_set_tab(w_control *control, int tab);
wresult _w_control_set_text_direction(w_control *control, int textDirection);
wresult _w_control_set_timer(w_control *control, wint64 ms, wushort id);
wresult _w_control_set_tooltip_text(w_control *control, const char *text,
		int length, int enc);
wresult _w_control_set_touch_enabled(w_control *control, int enabled);
wresult _w_control_set_visible(w_control *control, int visible);
wresult _w_control_set_zorder(w_control *control, w_control *sibling, int flags,
		_w_control_priv *priv);
gboolean _w_control_show_menu(w_control *control, int x, int y, int detail);
void _w_control_show_widget(w_control *control, _w_control_priv *priv);
wresult _w_control_to_control(w_control *control, w_point *result,
		w_point *point);
wresult _w_control_to_display(w_control *control, w_point *result,
		w_point *point);
gboolean _w_control_translate_mnemonic_0(w_control *_this, int keyval,
		GdkEventKey *gdkEvent, _w_control_priv *priv);
wresult _w_control_traverse(w_control *control, int traversal,
		w_event_key *event);
wresult _w_control_update(w_control *control);
GdkWindow* _w_control_window_enable(w_widget *control, _w_control_priv *priv);
GdkWindow* _w_control_window_event(w_widget *control, _w_control_priv *priv);
GdkWindow* _w_control_window_paint(w_widget *control, _w_control_priv *priv);
GdkWindow* _w_control_window_redraw(w_widget *control, _w_control_priv *priv);
void _w_control_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_control_class *clazz);
void _w_ccanvas_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_ccanvas_class *clazz);
/*
 * signals
 */
gboolean _w_control_contained_in_region(w_widget *widget, w_point *input,
		_w_control_priv *priv);
gboolean _gtk_control_destroy(w_widget *widget, _w_event_platform *ee,
		_w_control_priv *priv);
gboolean _gtk_control_button_press_event_0(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv, int sendMouseDown);
gboolean _gtk_control_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_button_release_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_control_commit(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_event_after(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_expose_inverse(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_focus(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_key_release_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_mnemonic_activate(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_motion_notify_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_control_popup_menu(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_preedit_changed(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_realize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_scroll_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_show_help(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_style_set(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_control_unrealize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
#endif /* GTK_WIDGETS_CONTROL_H_ */
