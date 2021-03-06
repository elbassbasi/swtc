/*
 * Name:        control0.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "control.h"
#include "toolkit.h"
/*
 * function
 */
wresult _w_control_check_background(w_widget *widget, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_check_buffered(w_widget *widget, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_check_orientation(w_widget *widget, w_composite *parent,
		_w_control_priv *priv) {
	return W_FALSE;
}
NSView* _w_control_content_view(w_widget *widget, _w_control_priv *priv) {
	return priv->get_view(widget);
}
wresult _w_control_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (w_widget_is_ok(parent) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (w_widget_class_id(parent) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_ARGUMENT;
	_w_widget_priv *priv = _w_widget_get_priv(widget);
	_W_WIDGET(widget)->style = priv->check_style(parent, style);
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent = W_COMPOSITE(parent);
	return priv->create_widget(widget, _W_CONTROL_PRIV(priv));
}
wresult _w_control_create_dragsource(w_control *control,
		w_dragsource *dragsource, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_FALSE;
}
wresult _w_control_create_droptarget(w_control *control,
		w_droptarget *droptarget, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_FALSE;
}
NSAttributedString* _w_control_create_string(w_control *control,
		NSString *string, w_font *font, w_color foreground, int flags) {
	NSMutableDictionary *dict = NSMutableDictionary_initWithCapacity(5);
	if (font == 0) {
		w_control_get_font(control, &font);
	}
	NSFont *hFont =(NSFont*) font;
	NSMutableDictionary_setObject(dict, NSOBJECT(hFont),
			NSOBJECT(_NSFontAttributeName()));
	_w_font_add_traits_1(font, dict);
	if ((flags & W_DISABLED) == 0) {
		if (foreground != 0) {
			NSColor *color = NSColor_colorWithDeviceRed(
			W_RED(foreground) / 255., W_GREEN(foreground) / 255.,
			W_BLUE(foreground) / 255., W_ALPHA(foreground) / 255.);
			NSMutableDictionary_setObject(dict, NSOBJECT(color),
					NSOBJECT(_NSForegroundColorAttributeName));
		}
	} else {
		NSMutableDictionary_setObject(dict,
				(NSObject*) NSColor_disabledControlTextColor(),
				(NSObject*) _NSForegroundColorAttributeName);
	}
	NSMutableParagraphStyle *paragraphStyle = NSMutableParagraphStyle_init();
	NSMutableParagraphStyle_setLineBreakMode(paragraphStyle,
			(flags & W_WRAP) ?
					NSLineBreakByWordWrapping : NSLineBreakByClipping);
	int align = NSLeftTextAlignment;
	if ((flags & W_CENTER) != 0) {
		align = NSCenterTextAlignment;
	} else if ((flags & W_RIGHT) != 0) {
		align = NSRightTextAlignment;
	}
	NSMutableParagraphStyle_setAlignment(paragraphStyle, align);
	if ((_W_WIDGET(control)->style & W_RIGHT_TO_LEFT) != 0) {
		NSMutableParagraphStyle_setBaseWritingDirection(paragraphStyle,
				NSWritingDirectionRightToLeft);
	} else {
		NSMutableParagraphStyle_setBaseWritingDirection(paragraphStyle,
				NSWritingDirectionLeftToRight);
	}
	NSMutableDictionary_setObject(dict, paragraphStyle,
			_NSParagraphStyleAttributeName());
	NSObject_release(paragraphStyle);
	NSAttributedString *attribStr = NSAttributedString_initWithString_0(string,
			dict);
	NSObject_release(dict);
	return attribStr;
}
wresult _w_control_create_widget(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_DRAG_DETECT;
	priv->check_orientation(W_WIDGET(widget), _W_CONTROL(widget)->parent, priv);
	wresult result = _w_widget_create_widget(W_WIDGET(widget), priv);
	if (result <= 0)
		return result;
	priv->check_background(W_WIDGET(widget), priv);
	priv->check_buffered(W_WIDGET(widget), priv);
	priv->set_default_font(W_WIDGET(widget), priv);
	priv->set_zorder(W_WIDGET(widget), priv);
	priv->set_relations(W_WIDGET(widget), priv);
	if ((_W_WIDGET(widget)->state & STATE_PARENT_BACKGROUND) != 0) {
		priv->set_background_0(W_WIDGET(widget), priv);
	}
	return result;
}
wresult _w_control_drag_detect(w_control *control, w_event_mouse *event) {
	return W_FALSE;
}
wresult _w_control_init_graphics(w_widget *widget, _w_graphics *gc,
		_w_control_priv *priv) {
	NSView *view = gc->view;
	NSGraphicsContext *graphicsContext = 0;
	if (gc->ispaintRect != 0) {
		graphicsContext = NSGraphicsContext_currentContext();
		if (!NSView_isFlipped(view))
			gc->state &= ~GRAPHICS_STATE_VISIBLE_REGION;
	}
	if (graphicsContext == 0) {
		NSWindow *window = NSView_window(view);
		/*
		 * Force the device to be created before attempting
		 * to create a GC on a deferred NSWindow.
		 */
		if (NSWindow_windowNumber(window) <= 0) {
			CGFloat alpha = NSWindow_alphaValue(window);
			NSWindow_setAlphaValue(window, 0);
			NSWindow_orderBack(window, 0);
			NSWindow_orderOut(window, 0);
			NSWindow_setAlphaValue(window, alpha);
		}
		graphicsContext = NSGraphicsContext_graphicsContextWithWindow(window);
		NSGraphicsContext *flippedContext =
				NSGraphicsContext_graphicsContextWithGraphicsPort(
						NSGraphicsContext_graphicsPort(graphicsContext),
						W_TRUE);
		graphicsContext = flippedContext;
		gc->flippedContext = flippedContext;
		gc->state &= ~GRAPHICS_STATE_VISIBLE_REGION;
		//gc->visibleRgn = getVisibleRegion();
		//display.addContext (data);
	}
	int mask = W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT;
	if ((gc->style & mask) == 0) {
		gc->style |= _W_WIDGET(widget)->style & (mask | W_MIRRORED);
	}
	gc->font = W_FONT(mac_toolkit->systemFont);
	return W_TRUE;
}
wresult _w_control_draw_widget(w_widget *widget, NSView *view,
		NSGraphicsContext *context, NSRect *rect, _w_control_priv *priv) {
	/* Send paint event */
	w_event_paint e;
	_w_graphics gc;
	NSRect frame;
	_w_graphics_init(W_GRAPHICS(&gc), context, 0);
	gc.view = view;
	NSView_frame(view, &frame);
	gc.height = frame.height;
	_w_control_init_graphics(widget, &gc, priv);
	e.event.type = W_EVENT_PAINT;
	e.event.platform_event = 0;
	e.event.time = 0;
	e.event.widget = widget;
	e.event.data = 0;
	e.gc = W_GRAPHICS(&gc);
	e.bounds.x = rect->origin.x;
	e.bounds.y = rect->origin.y;
	e.bounds.width = rect->size.width;
	e.bounds.height = rect->size.height;
	wresult result = _w_widget_post_event(widget, W_EVENT(&e));
	w_graphics_dispose(W_GRAPHICS(&gc));
	return result;
}
w_cursor* _w_control_find_cursor(w_control *control, _w_control_priv *priv) {
	if (_W_CONTROL(control)->cursor != 0)
		return _W_CONTROL(control)->cursor;
	w_composite *p = _W_CONTROL(control)->parent;
	_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(p);
	return ppriv->find_cursor(W_CONTROL(p), ppriv);
}
wresult _w_control_force_focus(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_accessible(w_control *control,
		w_accessible **accessible) {
	return W_FALSE;
}
wresult _w_control_get_background(w_control *control, w_color *background) {
	return W_FALSE;
}
wresult _w_control_get_background_image(w_control *control, w_image *image) {
	return W_FALSE;
}
wresult _w_control_get_border_width(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_bounds(w_control *control, w_point *location,
		w_size *size) {
	return W_FALSE;
}
wresult _w_control_get_cursor(w_control *control, w_cursor **cursor) {
	return W_FALSE;
}
wresult _w_control_get_drag_detect(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_enabled(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_font(w_control *control, w_font **font) {
	if (_W_CONTROL(control)->font != 0) {
		*font = _W_CONTROL(control)->font;
	} else {
		*font = w_toolkit_get_system_font(0);
	}
	return W_FALSE;
}
wresult _w_control_get_foreground(w_control *control, w_color *foreground) {
	return W_FALSE;
}
wresult _w_control_get_graphics(w_control *control, w_graphics *gc) {
	return W_FALSE;
}
wresult _w_control_get_layout_data(w_control *control, void **data) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(control);
	if ((_W_WIDGET(control)->state & STATE_LAYOUT_DATA_LOCALE) == 0) {
		*data = *((void**) &((char*) control)[clazz->object_used_size]);
	} else {
		*data = (void*) &((char*) control)[clazz->object_used_size];
	}
	return W_TRUE;
}
wresult _w_control_get_menu(w_control *control, w_menu **menu) {
	return W_FALSE;
}
wresult _w_control_get_orientation(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_parent(w_control *control, w_composite **parent) {
	*parent = _W_CONTROL(control)->parent;
	return W_TRUE;
}
wresult _w_control_get_region(w_control *control, w_region *region) {
	return W_FALSE;
}
wresult _w_control_get_shell(w_control *control, w_shell **shell) {
	w_composite *parent = _W_CONTROL(control)->parent;
	w_widget_get_shell(W_WIDGET(parent), shell);
	return W_TRUE;
}
wresult _w_control_get_tab(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_text_direction(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_tooltip_text(w_control *control, w_alloc alloc,
		void *user_data, int enc) {
	return W_FALSE;
}
wresult _w_control_get_touch_enabled(w_control *control) {
	return W_FALSE;
}
NSView* _w_control_get_view(w_widget *widget) {
	return _W_WIDGET(widget)->handle;
}
wresult _w_control_get_visible(w_control *control) {
	return W_FALSE;
}
wresult _w_control_has_border(w_widget *widget, _w_control_priv *priv) {
	return W_FALSE;
}

wresult _w_control_is_enabled(w_control *control) {
	return W_FALSE;
}
wresult _w_control_is_focus_control(w_control *control) {
	return W_FALSE;
}
wresult _w_control_is_reparentable(w_control *control) {
	return W_FALSE;
}
wresult _w_control_is_visible(w_control *control) {
	return W_FALSE;
}
wresult _w_control_kill_timer(w_control *control, wushort id) {
	return W_FALSE;
}
wresult _w_control_mark_layout(w_control *control, int flags,
		_w_control_priv *priv) {
	return W_TRUE;
}
wresult _w_control_move_above(w_control *control, w_control *_control) {
	return W_FALSE;
}
wresult _w_control_move_below(w_control *control, w_control *_control) {
	return W_FALSE;
}
wresult _w_control_new_layout_data(w_control *control, void **data,
		size_t size) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(control);
	if ((_W_WIDGET(control)->state & STATE_LAYOUT_DATA_LOCALE) == 0) {
		void *layout_data =
				*((void**) &((char*) control)[clazz->object_used_size]);
		if (layout_data != 0) {
			free(layout_data);
		}
	}
	if ((clazz->object_used_size + size) < clazz->object_total_size) {
		_W_WIDGET(control)->state |= STATE_LAYOUT_DATA_LOCALE;
		*data = (void*) &((char*) control)[clazz->object_used_size];
	} else {
		_W_WIDGET(control)->state &= ~STATE_LAYOUT_DATA_LOCALE;
		*data = malloc(size);
		if (*data == 0)
			return W_ERROR_NO_MEMORY;
		*((void**) &((char*) control)[clazz->object_used_size]) = *data;
	}
	return W_TRUE;
}
wresult _w_control_pack(w_control *control, int flags) {
	return W_FALSE;
}
wresult _w_control_print(w_control *control, w_graphics *gc) {
	return W_FALSE;
}
wresult _w_control_request_layout(w_control *control) {
	return W_FALSE;
}
wresult _w_control_redraw(w_control *control, w_rect *rect, int all) {
	return W_FALSE;
}
wresult _w_control_set_background(w_control *control, w_color color) {
	return W_FALSE;
}
wresult _w_control_set_background_0(w_widget *widget, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_set_background_image(w_control *control, w_image *image) {
	return W_FALSE;
}
wresult _w_control_set_bounds(w_control *control, w_point *location,
		w_size *size) {
	/*
	 * Bug in Cocoa. On Mac 10.8, a text control loses and gains focus
	 * when its bounds changes.  The fix is to ignore these events.
	 */
	w_control *oldIgnoreFocusControl = mac_toolkit->ignoreFocusControl;
	mac_toolkit->ignoreFocusControl = control;
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	NSView *topView = priv->top_view(W_WIDGET(control));
	NSRect r;
	CGFloat y = 0;
	if (location != 0) {
		NSView *parent = NSView_superview(topView);
		NSView_frame(parent, &r);
		y = r.height - location->y;
		if (size != 0) {
			y -= size->height;
		} else {
			NSView_frame(topView, &r);
			y -= r.height;
		}
	}
	if (location != 0 && size != 0) {
		r.origin.x = location->x;
		r.origin.y = y;
		r.size.width = size->width;
		r.size.height = size->height;
		NSView_setFrame(topView, &r);
	} else if (location != 0) {
		r.origin.x = location->x;
		r.origin.y = y;
		NSView_setFrameOrigin(topView, &r.origin);
	} else if (size != 0) {
		r.size.width = size->width;
		r.size.height = size->height;
		NSView_setFrameSize(topView, &r.size);
	}
	mac_toolkit->ignoreFocusControl = oldIgnoreFocusControl;
	return W_TRUE;
}
wresult _w_control_set_capture(w_control *control, int capture) {
	return W_FALSE;
}
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor) {
	if (cursor != 0 && _W_CURSOR(cursor)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	_W_CONTROL(control)->cursor = cursor;
	wresult enabled = W_CONTROL_GET_CLASS(control)->is_enabled(control);
	if (enabled <= 0)
		return W_TRUE;
	NSView *view = _W_WIDGET(control)->handle;
	NSWindow *window = NSView_window(view);
	if (!NSWindow_areCursorRectsEnabled(window))
		return W_TRUE;
	_w_toolkit_set_cursor(mac_toolkit->currentControl);
	return W_TRUE;
}
wresult _w_control_set_default_font(w_widget *widget, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_set_drag_detect(w_control *control, int dragDetect) {
	return W_FALSE;
}
wresult _w_control_set_enabled(w_control *control, int enabled) {
	return W_FALSE;
}
wresult _w_control_set_focus(w_control *control) {
	return W_FALSE;
}
wresult _w_control_set_font(w_control *control, w_font *font) {
	return W_FALSE;
}
wresult _w_control_set_foreground(w_control *control, w_color color) {
	return W_FALSE;
}
wresult _w_control_set_menu(w_control *control, w_menu *menu) {
	return W_FALSE;
}
wresult _w_control_set_orientation(w_control *control, int orientation) {
	return W_FALSE;
}
wresult _w_control_set_parent(w_control *control, w_composite *parent) {
	return W_FALSE;
}
wresult _w_control_set_redraw(w_control *control, int redraw) {
	return W_FALSE;
}
wresult _w_control_set_region(w_control *control, w_region *region) {
	return W_FALSE;
}
wresult _w_control_set_relations(w_widget *widget, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_set_tab(w_control *control, int tab) {
	return W_FALSE;
}
wresult _w_control_set_text_direction(w_control *control, int textDirection) {
	return W_FALSE;
}
wresult _w_control_set_timer(w_control *control, wint64 ms, wushort id) {
	return W_FALSE;
}
wresult _w_control_set_tooltip_text(w_control *control, const char *text,
		int length, int enc) {
	return W_FALSE;
}
wresult _w_control_set_touch_enabled(w_control *control, int enabled) {
	return W_FALSE;
}
wresult _w_control_set_visible(w_control *control, int visible) {
	if (visible) {
		if ((_W_WIDGET(control)->state & STATE_HIDDEN) == 0)
			return W_TRUE;
		_W_WIDGET(control)->state &= ~STATE_HIDDEN;
	} else {
		if ((_W_WIDGET(control)->state & STATE_HIDDEN) != 0)
			return W_TRUE;
		_W_WIDGET(control)->state |= STATE_HIDDEN;
	}
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	NSView *topView = priv->top_view(W_WIDGET(control));
	NSView_setHidden(topView, !visible);
	return W_FALSE;
}
wresult _w_control_set_zorder(w_widget *widget, _w_control_priv *priv) {
	NSView *topView = priv->top_view(W_WIDGET(widget));
	w_composite *parent = _W_CONTROL(widget)->parent;
	_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
	NSView *contentView = ppriv->content_view(W_WIDGET(parent), ppriv);
	NSView_addSubview_0(contentView, topView, NSWindowBelow, 0);
	return W_TRUE;
}
wresult _w_control_to_control(w_control *control, w_point *result,
		w_point *point) {
	return W_FALSE;
}
wresult _w_control_to_display(w_control *control, w_point *result,
		w_point *point) {
	return W_FALSE;
}
NSView* _w_control_top_view(w_widget *widget) {
	return _W_WIDGET(widget)->handle;
}
wresult _w_control_traverse(w_control *control, int traversal,
		w_event_key *event) {
	return W_FALSE;
}
wresult _w_control_update(w_control *control) {
	return W_FALSE;
}
wresult _w_control_update_layout(w_control *control, int flags,
		_w_control_priv *priv) {
	return W_TRUE;
}
void _w_control_class_init(struct _w_control_class *clazz) {
	_w_widget_class_init(W_WIDGET_CLASS(clazz));
	/*
	 * functions
	 */
	W_WIDGET_CLASS(clazz)->create = _w_control_create;
	clazz->create_dragsource = _w_control_create_dragsource;
	clazz->create_droptarget = _w_control_create_droptarget;
	clazz->drag_detect = _w_control_drag_detect;
	clazz->force_focus = _w_control_force_focus;
	clazz->get_accessible = _w_control_get_accessible;
	clazz->get_background = _w_control_get_background;
	clazz->get_background_image = _w_control_get_background_image;
	clazz->get_border_width = _w_control_get_border_width;
	clazz->get_bounds = _w_control_get_bounds;
	clazz->get_cursor = _w_control_get_cursor;
	clazz->get_drag_detect = _w_control_get_drag_detect;
	clazz->get_enabled = _w_control_get_enabled;
	clazz->get_font = _w_control_get_font;
	clazz->get_foreground = _w_control_get_foreground;
	clazz->get_graphics = _w_control_get_graphics;
	clazz->get_layout_data = _w_control_get_layout_data;
	clazz->get_menu = _w_control_get_menu;
	clazz->get_orientation = _w_control_get_orientation;
	clazz->get_parent = _w_control_get_parent;
	clazz->get_region = _w_control_get_region;
	clazz->get_shell = _w_control_get_shell;
	clazz->get_tab = _w_control_get_tab;
	clazz->get_text_direction = _w_control_get_text_direction;
	clazz->get_tooltip_text = _w_control_get_tooltip_text;
	clazz->get_touch_enabled = _w_control_get_touch_enabled;
	clazz->get_visible = _w_control_get_visible;
	clazz->is_enabled = _w_control_is_enabled;
	clazz->is_focus_control = _w_control_is_focus_control;
	clazz->is_reparentable = _w_control_is_reparentable;
	clazz->is_visible = _w_control_is_visible;
	clazz->kill_timer = _w_control_kill_timer;
	clazz->move_above = _w_control_move_above;
	clazz->move_below = _w_control_move_below;
	clazz->new_layout_data = _w_control_new_layout_data;
	clazz->pack = _w_control_pack;
	clazz->print = _w_control_print;
	clazz->request_layout = _w_control_request_layout;
	clazz->redraw = _w_control_redraw;
	clazz->set_background = _w_control_set_background;
	clazz->set_background_image = _w_control_set_background_image;
	clazz->set_bounds = _w_control_set_bounds;
	clazz->set_capture = _w_control_set_capture;
	clazz->set_cursor = _w_control_set_cursor;
	clazz->set_drag_detect = _w_control_set_drag_detect;
	clazz->set_enabled = _w_control_set_enabled;
	clazz->set_focus = _w_control_set_focus;
	clazz->set_font = _w_control_set_font;
	clazz->set_foreground = _w_control_set_foreground;
	clazz->set_menu = _w_control_set_menu;
	clazz->set_orientation = _w_control_set_orientation;
	clazz->set_parent = _w_control_set_parent;
	clazz->set_redraw = _w_control_set_redraw;
	clazz->set_region = _w_control_set_region;
	clazz->set_tab = _w_control_set_tab;
	clazz->set_text_direction = _w_control_set_text_direction;
	clazz->set_timer = _w_control_set_timer;
	clazz->set_tooltip_text = _w_control_set_tooltip_text;
	clazz->set_touch_enabled = _w_control_set_touch_enabled;
	clazz->set_visible = _w_control_set_visible;
	clazz->to_control = _w_control_to_control;
	clazz->to_display = _w_control_to_display;
	clazz->traverse = _w_control_traverse;
	clazz->update = _w_control_update;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->create_widget = _w_control_create_widget;
	priv->get_view = _w_control_get_view;
	priv->top_view = _w_control_top_view;
	priv->content_view = _w_control_content_view;
	priv->check_orientation = _w_control_check_orientation;
	priv->check_background = _w_control_check_background;
	priv->check_buffered = _w_control_check_buffered;
	priv->draw_widget = _w_control_draw_widget;
	priv->set_default_font = _w_control_set_default_font;
	priv->set_zorder = _w_control_set_zorder;
	priv->set_relations = _w_control_set_relations;
	priv->set_background_0 = _w_control_set_background_0;
	priv->has_border = _w_control_has_border;
	priv->mark_layout = _w_control_mark_layout;
	priv->update_layout = _w_control_update_layout;
	priv->find_cursor = _w_control_find_cursor;
}
