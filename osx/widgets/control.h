/*
 * Name:        control.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef OSX_WIDGETS_CONTROL_H_
#define OSX_WIDGETS_CONTROL_H_
#include "widget.h"
/*
 * states
 */
#define STATE_MENU (1 << (STATE_WIDGET_END + 2))
#define STATE_DRAG_DETECT (1 << (STATE_WIDGET_END + 3))
#define STATE_TIMER_START (1 << (STATE_WIDGET_END + 4))
#define STATE_THEME_BACKGROUND (1 << (STATE_WIDGET_END + 5))
#define STATE_CHECK_SUBWINDOW (1 << (STATE_WIDGET_END + 6))
#define STATE_LAYOUT_DATA_LOCALE (1 << (STATE_WIDGET_END + 7))
#define STATE_DRAW_REGION (1 << (STATE_WIDGET_END + 8))
#define STATE_ZERO_WIDTH  (1 << (STATE_WIDGET_END + 9))
#define STATE_ZERO_HEIGHT (1 << (STATE_WIDGET_END + 10))
#define STATE_HIDDEN (1 << (STATE_WIDGET_END + 11))
#define STATE_LAYOUT_NEEDED (1 << (STATE_WIDGET_END + 12))
#define STATE_LAYOUT_CHANGED (1 << (STATE_WIDGET_END + 13))
#define STATE_LAYOUT_CHILD (1 << (STATE_WIDGET_END + 14))
#define STATE_TAB_LIST (1 << (STATE_WIDGET_END + 15))
#define STATE_PARENT_BACKGROUND (1 << (STATE_WIDGET_END + 16))
#define STATE_CONTROL_END (STATE_WIDGET_END + 17)
/*
 * control
 */
typedef struct _w_control {
	_w_widget widget;
	w_composite *parent;
	w_menu *menu;
	w_cursor *cursor;
	union {
		w_font *font;
		_w_font _font;
	};
} _w_control;
#define _W_CONTROL(x) ((_w_control*)x)
/*
 * private
 */
struct _w_control_priv {
	_w_widget_priv widget;
	NSView* (*get_view)(w_widget *widget);
	NSView* (*top_view)(w_widget *widget);
	NSView* (*content_view)(w_widget *widget, _w_control_priv *priv);
	wresult (*check_orientation)(w_widget *widget, w_composite *parent,
			_w_control_priv *priv);
	wresult (*check_background)(w_widget *widget, _w_control_priv *priv);
	wresult (*check_buffered)(w_widget *widget, _w_control_priv *priv);
	wresult (*draw_widget)(w_widget *widget, NSView *view,
			NSGraphicsContext *context, NSRect *rect, _w_control_priv *priv);
	wresult (*set_default_font)(w_widget *widget, _w_control_priv *priv);
	wresult (*set_zorder)(w_widget *widget, _w_control_priv *priv);
	wresult (*set_relations)(w_widget *widget, _w_control_priv *priv);
	wresult (*set_background_0)(w_widget *widget, _w_control_priv *priv);
	wresult (*has_border)(w_widget *widget, _w_control_priv *priv);
	wresult (*mark_layout)(w_control *control, int flags,
			_w_control_priv *priv);
	wresult (*update_layout)(w_control *control, int flags,
			_w_control_priv *priv);
};
#define _W_CONTROL_PRIV(x) ((_w_control_priv*)x)
#define _W_CONTROL_GET_PRIV(x) ((_w_control_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
wresult _w_control_check_background(w_widget *widget, _w_control_priv *priv);
wresult _w_control_check_buffered(w_widget *widget, _w_control_priv *priv);
wresult _w_control_check_orientation(w_widget *widget, w_composite *parent,
		_w_control_priv *priv);
NSView* _w_control_content_view(w_widget *widget, _w_control_priv *priv);
wresult _w_control_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event);
wresult _w_control_create_dragsource(w_control *control,
		w_dragsource *dragsource, wuint64 style,
		w_widget_post_event_proc post_event);
wresult _w_control_create_droptarget(w_control *control,
		w_droptarget *droptarget, wuint64 style,
		w_widget_post_event_proc post_event);
wresult _w_control_create_widget(w_widget *widget, _w_control_priv *priv);
wresult _w_control_drag_detect(w_control *control, w_event_mouse *event);
wresult _w_control_draw_widget(w_widget *widget, NSView *view,
NSGraphicsContext *context, NSRect *rect, _w_control_priv *priv);
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
wresult _w_control_get_shell(w_control *control, w_shell **shell);
wresult _w_control_get_tab(w_control *control);
wresult _w_control_get_text_direction(w_control *control);
wresult _w_control_get_tooltip_text(w_control *control, w_alloc alloc,
		void *user_data, int enc);
wresult _w_control_get_touch_enabled(w_control *control);
NSView* _w_control_get_view(w_widget *widget);
wresult _w_control_get_visible(w_control *control);
wresult _w_control_has_border(w_widget *widget, _w_control_priv *priv);
wresult _w_control_is_enabled(w_control *control);
wresult _w_control_is_focus_control(w_control *control);
wresult _w_control_is_reparentable(w_control *control);
wresult _w_control_is_visible(w_control *control);
wresult _w_control_kill_timer(w_control *control, wushort id);
wresult _w_control_mark_layout(w_control *control, int flags,
		_w_control_priv *priv);
wresult _w_control_move_above(w_control *control, w_control *_control);
wresult _w_control_move_below(w_control *control, w_control *_control);
wresult _w_control_new_layout_data(w_control *control, void **data,
		size_t size);
wresult _w_control_pack(w_control *control, int flags);
wresult _w_control_print(w_control *control, w_graphics *gc);
wresult _w_control_request_layout(w_control *control);
wresult _w_control_redraw(w_control *control, w_rect *rect, int all);
wresult _w_control_set_background(w_control *control, w_color color);
wresult _w_control_set_background_0(w_widget *widget, _w_control_priv *priv);
wresult _w_control_set_background_image(w_control *control, w_image *image);
wresult _w_control_set_bounds(w_control *control, w_point *location,
		w_size *size);
wresult _w_control_set_capture(w_control *control, int capture);
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor);
wresult _w_control_set_default_font(w_widget *widget, _w_control_priv *priv);
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
wresult _w_control_set_relations(w_widget *widget, _w_control_priv *priv);
wresult _w_control_set_tab(w_control *control, int tab);
wresult _w_control_set_text_direction(w_control *control, int textDirection);
wresult _w_control_set_timer(w_control *control, wint64 ms, wushort id);
wresult _w_control_set_tooltip_text(w_control *control, const char *text,
		int length, int enc);
wresult _w_control_set_touch_enabled(w_control *control, int enabled);
wresult _w_control_set_visible(w_control *control, int visible);
wresult _w_control_set_zorder(w_widget *widget, _w_control_priv *priv);
wresult _w_control_to_control(w_control *control, w_point *result,
		w_point *point);
wresult _w_control_to_display(w_control *control, w_point *result,
		w_point *point);
NSView* _w_control_top_view(w_widget *widget);
wresult _w_control_traverse(w_control *control, int traversal,
		w_event_key *event);
wresult _w_control_update(w_control *control);
wresult _w_control_update_layout(w_control *control, int flags,
		_w_control_priv *priv);
void _w_control_class_init(struct _w_control_class *clazz);
/*
 * messages
 */
#endif /* OSX_WIDGETS_CONTROL_H_ */
