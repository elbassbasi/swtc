/*
 * sash.c
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */
#include "sash.h"
#include "../widgets/toolkit.h"
wuint64 _w_sash_check_style(w_widget *control, wuint64 style) {
	style |= W_SMOOTH;
	style = _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
	return style;
}
wresult _w_sash_set_cursor(w_control *control, w_cursor *cursor) {
	if (cursor == 0) {
		w_toolkit *toolkit = w_widget_get_toolkit(W_WIDGET(control));
		int cursor_id;
		if (_W_WIDGET(control)->style & W_HORIZONTAL) {
			cursor_id = W_CURSOR_SIZENS;
		} else {
			cursor_id = W_CURSOR_SIZEWE;
		}
		cursor = w_toolkit_get_system_cursor(toolkit, cursor_id);
	}
	return _w_control_set_cursor(control, cursor);
}
wresult _w_sash_create_handle(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
	_W_WIDGET(widget)->handle = (NSView*) SWTView_new(widget);
	return W_TRUE;
}
w_cursor* _w_sash_find_cursor(w_widget *widget, _w_control_priv *priv) {
	w_cursor *cursor = _w_control_find_cursor(widget, priv);
	if (cursor == 0) {
		int cursorType =
				(_W_WIDGET(widget)->style & W_HORIZONTAL) != 0 ?
						W_CURSOR_SIZENS : W_CURSOR_SIZEWE;
		cursor = w_toolkit_get_system_cursor(W_TOOLKIT(mac_toolkit),
				cursorType);
	}
	return cursor;
}
wresult _w_sash_becomeFirstResponder(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result;
	//result = _w_control_becomeFirstResponder(widget, e, priv);
	NSRect r;
	NSView_frame(_W_WIDGET(widget)->handle, &r);
	_W_SASH(widget)->last.x = r.x;
	_W_SASH(widget)->last.y = r.y;
	return result;
}
wresult _w_sash_mouseDown(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	//TODO use sendMouseEvent
	wresult result;
	//result = _w_control_mouseDown(widget, e, priv);
	if (w_widget_is_ok(widget) <= 0)
		return result;
	NSEvent *nsEvent = (NSEvent*) e->args[0]._id;
	if (NSEvent_clickCount(nsEvent) != 1)
		return result;
	NSPoint location, point;
	NSEvent_locationInWindow(nsEvent, &location);
	NSView_convertPoint_fromView_(_W_WIDGET(widget)->handle, &location, 0,
			&point);
	_W_SASH(widget)->start.x = point.x;
	_W_SASH(widget)->start.y = point.y;
	NSRect frame;
	NSView_frame(_W_WIDGET(widget)->handle, &frame);
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.widget = widget;
	event.event.data = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = 0;
	event.bounds.x = frame.x;
	event.bounds.y = frame.y;
	event.bounds.width = frame.width;
	event.bounds.height = frame.height;

	int doit = _w_widget_post_event(widget, (w_event*) &event);
	if (w_widget_is_ok(widget) <= 0)
		return result;
	if (doit) {
		_W_WIDGET(widget)->state |= STATE_SASH_DRAGGING;
		_W_SASH(widget)->last.x = event.bounds.x;
		_W_SASH(widget)->last.y = event.bounds.y;
		_w_control_set_bounds(W_CONTROL(widget), &event.bounds.pt, 0);
	}
	return result;
}
wresult _w_sash_mouseDragged(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	//TODO use sendMouseEvent
	wresult result;
	//result = _w_control_mouseDragged(widget, e, priv);
	if (w_widget_is_ok(widget) <= 0)
		return result;
	if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING))
		return W_FALSE;
	NSEvent *nsEvent = (NSEvent*) e->args[0]._id;
	NSPoint location, point;
	NSEvent_locationInWindow(nsEvent, &location);
	NSView_convertPoint_fromView_(_W_WIDGET(widget)->handle, &location, 0,
			&point);
	NSRect frame;
	NSView_frame(_W_WIDGET(widget)->handle, &frame);

	NSRect parentFrame;
	w_composite *parent = _W_CONTROL(widget)->parent;
	_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
	NSView *topView = ppriv->top_view(W_WIDGET(parent));
	NSView_frame(topView, &parentFrame);
	int newX = _W_SASH(widget)->last.x, newY = _W_SASH(widget)->last.y;
	if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
		int _newX = WMAX(0,
				(int) (point.x + frame.x - _W_SASH(widget)->start.x));
		newX = WMIN(_newX, parentFrame.width - frame.width);
	} else {
		int _newY = WMAX(0,
				(int) (point.y + frame.y - _W_SASH(widget)->start.y));
		newY = WMIN(_newY, parentFrame.height - frame.height);
	}
	if (newX == _W_SASH(widget)->last.x && newY == _W_SASH(widget)->last.y)
		return result;
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.widget = widget;
	event.event.data = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = 0;
	event.bounds.x = newX;
	event.bounds.y = newY;
	event.bounds.width = frame.width;
	event.bounds.height = frame.height;
	int doit = _w_widget_post_event(widget, (w_event*) &event);
	if (w_widget_is_ok(widget) <= 0)
		return result;
	if (doit) {
		_W_WIDGET(widget)->state |= STATE_SASH_DRAGGING;
		_W_SASH(widget)->last.x = event.bounds.x;
		_W_SASH(widget)->last.y = event.bounds.y;
		_w_control_set_bounds(W_CONTROL(widget), &event.bounds.pt,
				&event.bounds.sz);
	}
	return result;
}

wresult _w_sash_mouseUp(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	//TODO use sendMouseEvent
	wresult result;
	//result = _w_control_mouseUp(widget, e, priv);
	if (w_widget_is_ok(widget) <= 0)
		return result;
	if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING))
		return W_FALSE;
	_W_WIDGET(widget)->state &= ~STATE_SASH_DRAGGING;

	NSRect frame;
	NSView_frame(_W_WIDGET(widget)->handle, &frame);
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.widget = widget;
	event.event.data = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = 0;
	event.bounds.x = _W_SASH(widget)->last.x;
	event.bounds.y = _W_SASH(widget)->last.y;
	event.bounds.width = frame.width;
	event.bounds.height = frame.height;

	int doit = _w_widget_post_event(widget, (w_event*) &event);
	if (w_widget_is_ok(widget) <= 0)
		return result;
	if (doit) {
		event.bounds.width = frame.width;
		event.bounds.height = frame.height;
		_w_control_set_bounds(W_CONTROL(widget), &event.bounds.pt,
				&event.bounds.sz);
	}
	return result;
}

wresult _w_sash_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
void _w_sash_class_init(struct _w_sash_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SASH;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_sash_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_sash);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_sash);
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->create_handle = _w_sash_create_handle;
	_W_WIDGET_PRIV(priv)->check_style = _w_sash_check_style;
	_W_WIDGET_PRIV(priv)->compute_size = _w_sash_compute_size;
	/*
	 *  messages
	 */
	dispatch_message *msgs = _W_WIDGET_PRIV(priv)->msgs;
	msgs[_NS_becomeFirstResponder] = _w_sash_becomeFirstResponder;
	msgs[_NS_mouseDown] = _w_sash_mouseDown;
	msgs[_NS_mouseDragged] = _w_sash_mouseDragged;
	msgs[_NS_mouseUp] = _w_sash_mouseUp;
}
