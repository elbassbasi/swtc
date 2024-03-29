/*
 * Name:        sash.c
 * Author:      Azeddine EL Bassbasi
 * Created:     19 févr. 2021
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "sash.h"
#include "../widgets/toolkit.h"
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#define INCREMENT 1
#define  PAGE_INCREMENT 9
wuint64 _w_sash_check_style(w_widget *control, wuint64 style) {
	style = _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
	return style;
}
wuchar draw_band_bits[] = { -86, 85, -86, 85, -86, 85, -86, 85 };
void _w_sash_draw_band(w_widget *widget, w_point *pt, w_size *size) {
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
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	GtkWidget *handle = _w_fixed_new(0);
	if (handle == 0) {
		return W_ERROR_NO_HANDLES;
	}
	gtk_widget_set_has_window(handle, TRUE);
	gtk_widget_set_can_focus(handle, TRUE);
	_w_widget_set_control(handle, widget);
	_W_WIDGET(widget)->handle = handle;
	//_w_sash_set_cursor(W_CONTROL(widget), 0);
	return W_TRUE;
}
void _w_sash_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_control_hook_events(widget, priv);
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	gtk_widget_add_events(handle, GDK_POINTER_MOTION_HINT_MASK);
}
gboolean _gtk_sash_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_button_press_event(widget, e, priv);
	if (result != 0)
		return result;
	GdkEventButton *gdkEvent = e->args[0];
	int button = gdkEvent->button;
	if (button != 1)
		return 0;
	if (gdkEvent->type == GDK_2BUTTON_PRESS) {
		e->result = 0;
		return W_TRUE;
	}
	if (gdkEvent->type == GDK_3BUTTON_PRESS) {
		e->result = 0;
		return W_TRUE;
	}
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	GdkWindow *window = gtk_widget_get_window(handle);
	gint origin_x, origin_y;
	w_point pt, *_last = &_W_SASH(widget)->last;
	w_point *_start = &_W_SASH(widget)->start;
	w_size size;
	gdk_window_get_origin(window, &origin_x, &origin_y);
	_start->x = gdkEvent->x_root - origin_x;
	_start->y = gdkEvent->y_root - origin_y;
	GtkAllocation allocation;
	gtk_widget_get_allocation(handle, &allocation);
	pt.x = allocation.x;
	pt.y = allocation.y;
	size.width = allocation.width;
	size.height = allocation.height;
	_last->x = pt.x;
	_last->y = pt.y;
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.widget = widget;
	event.event.data = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = gdkEvent->time;
	event.bounds.x = _last->x;
	event.bounds.y = _last->y;
	event.bounds.width = size.width;
	event.bounds.height = size.height;
	if ((_W_WIDGET(widget)->style & W_SMOOTH) == 0) {
		event.detail = W_DRAG;
	}
	w_widget *parent;
	w_widget_get_parent(W_WIDGET(widget), &parent);
	_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
	int ClientWidth = 0;
	if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
		ClientWidth = ppriv->get_client_width(W_CONTROL(parent), ppriv);
		event.bounds.x = ClientWidth - size.width - event.bounds.x;
	}
	int doit = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (w_widget_is_ok(widget) <= 0)
		return FALSE;
	if (doit) {
		_W_WIDGET(widget)->state |= STATE_SASH_DRAGGING;
		_last->x = event.bounds.x;
		_last->y = event.bounds.y;
		if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
			_last->x = ClientWidth - size.width - _last->x;
		}
		int flags = UPDATE_ALL;
		if (_W_WIDGET(widget)->style & W_SMOOTH) {
			flags |= UPDATE_FLUSH;
		}
		ppriv->update_0(W_CONTROL(parent), flags, ppriv);
		pt.x = _last->x;
		pt.y = event.bounds.y;
		_w_sash_draw_band(widget, &pt, &size);
		if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0) {
			w_control_set_bounds(W_CONTROL(widget), &event.bounds.pt, &size);
		}
	}
	return result;
}

gboolean _gtk_sash_button_release_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_button_release_event(widget, e, priv);
	if (result != 0)
		return result;
	GdkEventButton *gdkEvent = e->args[0];
	int button = gdkEvent->button;
	if (button != 1) {
		e->result = 0;
		return W_TRUE;
	}
	if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING)) {
		return 0;
	}
	_W_WIDGET(widget)->state &= ~STATE_SASH_DRAGGING;
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	w_point *_last = &_W_SASH(widget)->last;
	w_point *_start = &_W_SASH(widget)->start;
	GtkAllocation allocation;
	gtk_widget_get_allocation(handle, &allocation);
	w_size size;
	size.width = allocation.width;
	size.height = allocation.height;
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.widget = widget;
	event.event.data = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = gdkEvent->time;
	event.bounds.x = _last->x;
	event.bounds.y = _last->y;
	event.bounds.width = size.width;
	event.bounds.height = size.height;
	_w_sash_draw_band(widget, _last, &size);
	w_widget *parent;
	w_widget_get_parent(W_WIDGET(widget), &parent);
	_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
	int ClientWidth = 0;
	if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
		ClientWidth = ppriv->get_client_width(W_CONTROL(parent), ppriv);
		event.bounds.x = ClientWidth - size.width - event.bounds.x;
	}
	int doit = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (w_widget_is_ok(widget) <= 0)
		return FALSE;
	if (doit) {
		if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0) {
			w_control_set_bounds(W_CONTROL(widget), &event.bounds.pt, &size);
		}
	}
	return W_TRUE;
}

gboolean _gtk_sash_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (GTK_VERSION >= VERSION(3, 16, 0)) {
		GtkStyleContext *context = gtk_widget_get_style_context(e->widget);
		GtkAllocation allocation;
		gtk_widget_get_allocation(e->widget, &allocation);
		int width =
				(_W_WIDGET(widget)->state & STATE_ZERO_WIDTH) != 0 ?
						0 : allocation.width;
		int height =
				(_W_WIDGET(widget)->state & STATE_ZERO_HEIGHT) != 0 ?
						0 : allocation.height;
		// We specify a 0 value for x & y as we want the whole widget to be
		// colored, not some portion of it.
		gtk_render_background(context, (cairo_t*) e->args[0], 0, 0, width,
				height);
	}
	return _gtk_control_draw(widget, e, priv);
}

gboolean _gtk_sash_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_focus_in_event(widget, e, priv);
	if (result != 0)
		return result;
	// widget could be disposed at this point
	if (_W_WIDGET(widget)->handle != 0) {
		GtkWidget *handle = _W_WIDGET(widget)->handle;
		GtkAllocation allocation;
		gtk_widget_get_allocation(handle, &allocation);
		_W_SASH(widget)->last.x = allocation.x;
		_W_SASH(widget)->last.y = allocation.y;
	}
	return FALSE;
}

gboolean _gtk_sash_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int result = _gtk_control_key_press_event(widget, e, priv);
	if (result != 0)
		return result;
	GdkEventKey *gdkEvent = (GdkEventKey*) e->args[1];
	int keyval = gdkEvent->keyval;
	switch (keyval) {
	case GDK_KEY_Left:
	case GDK_KEY_Right:
	case GDK_KEY_Up:
	case GDK_KEY_Down: {
		GtkWidget *handle = _W_WIDGET(widget)->handle;
		int xChange = 0, yChange = 0;
		int stepSize = PAGE_INCREMENT;
		if ((gdkEvent->state & GDK_CONTROL_MASK) != 0)
			stepSize = INCREMENT;
		if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
			if (keyval == GDK_KEY_Up || keyval == GDK_KEY_Down)
				break;
			xChange = keyval == GDK_KEY_Left ? -stepSize : stepSize;
		} else {
			if (keyval == GDK_KEY_Left || keyval == GDK_KEY_Right)
				break;
			yChange = keyval == GDK_KEY_Up ? -stepSize : stepSize;
		}
		w_toolkit *toolkit = w_widget_get_toolkit(widget);
		w_widget *parent;
		w_widget_get_parent(W_WIDGET(widget), &parent);
		_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
		w_point *_last = &_W_SASH(widget)->last;
		w_point *_start = &_W_SASH(widget)->start;
		int parentBorder = 0;
		GtkAllocation allocation;
		gtk_widget_get_allocation(handle, &allocation);
		w_size size;
		size.width = allocation.width;
		size.height = allocation.height;
		gtk_widget_get_allocation(handle, &allocation);
		int parentWidth = allocation.width;
		int parentHeight = allocation.height;
		int newX = _last->y, newY = _last->y;
		if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
			int _newX0 = _last->x + xChange - parentBorder - _start->x;
			int _newX = WMAX(0, _newX0);
			newX = WMIN(_newX, parentWidth - size.width);
		} else {
			int _newY0 = _last->y + yChange - parentBorder - _start->y;
			int _newY = WMAX(0, _newY0);
			newY = WMIN(_newY, parentHeight - size.height);
		}
		if (newX == _last->x && newY == _last->y)
			return result;

		/* Ensure that the pointer image does not change */
		GdkWindow *window = gtk_widget_get_window(handle);
		GdkEventMask grabMask = GDK_POINTER_MOTION_MASK
				| GDK_BUTTON_RELEASE_MASK;
		GdkCursor *gdkCursor;
		w_cursor *_cursor;
		if (_W_CONTROL(widget)->cursor != 0) {
			_cursor = _W_CONTROL(widget)->cursor;
			gdkCursor = _W_CURSOR(_W_CONTROL(widget)->cursor)->handle;
		} else {
			wuint cursor_style;
			if (_W_WIDGET(widget)->style & W_HORIZONTAL) {
				cursor_style = W_CURSOR_SIZENS;
			} else {
				cursor_style = W_CURSOR_SIZEWE;
			}
			_cursor = w_toolkit_get_system_cursor(toolkit, cursor_style);
		}
		gdkCursor = _W_CURSOR(_cursor)->handle;
		GdkGrabStatus ptrGrabResult = gdk_pointer_grab(window, FALSE, grabMask,
				window, gdkCursor, GDK_CURRENT_TIME);

		/* The event must be sent because its doit flag is used. */
		w_event_sash event;
		event.event.type = W_EVENT_SELECTION;
		event.event.platform_event = (w_event_platform*) e;
		event.event.data = 0;
		event.event.widget = 0;
		event.bounds.x = newX;
		event.bounds.y = newY;
		event.bounds.width = size.width;
		event.bounds.height = size.height;
		int ClientWidth = 0;
		if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
			ClientWidth = ppriv->get_client_width(W_CONTROL(parent), ppriv);
			event.bounds.x = ClientWidth - size.width - event.bounds.x;
		}
		int doit = _w_widget_send_event(widget, (w_event*) &event,
				W_EVENT_SEND);
		if (ptrGrabResult == GDK_GRAB_SUCCESS)
			gdk_pointer_ungrab(GDK_CURRENT_TIME);
		if (!w_widget_is_ok(W_WIDGET(widget))) {
			e->result = 0;
			return result;
		}

		if (doit) {
			_last->x = event.bounds.x;
			_last->y = event.bounds.y;
			if (_W_WIDGET(parent)->style & W_MIRRORED) {
				_last->x = ClientWidth - size.width - _last->x;
			}
			if ((_W_WIDGET(parent)->style & W_SMOOTH) != 0) {
				w_control_set_bounds(W_CONTROL(widget), &event.bounds.pt,
						&event.bounds.sz);
				if (!w_widget_is_ok(W_WIDGET(widget))) {
					e->result = 0;
					return result;
				}
			}
			w_point cursor;
			cursor.x = event.bounds.x;
			cursor.y = event.bounds.y;
			if ((_W_WIDGET(parent)->style & W_VERTICAL) != 0) {
				cursor.y += size.height / 2;
			} else {
				cursor.x += size.width / 2;
			}
			w_toolkit_set_cursor_location(toolkit, &cursor);
		}
	}
		break;
	}
	return result;
}

gboolean _gtk_sash_motion_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_motion_notify_event(widget, e, priv);
	if (result != 0)
		return result;
	if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING)) {
		e->result = 0;
		return W_TRUE;
	}
	GdkEventMotion *gdkEvent = e->args[0];
	int eventX, eventY, eventState;
	if (gdkEvent->is_hint != 0) {
		int pointer_x, pointer_y;
		GdkModifierType mask = (GdkModifierType) 0;
		gdk_window_get_device_position(gdkEvent->window,
				gdk_event_get_device((GdkEvent*) gdkEvent), &pointer_x,
				&pointer_y, &mask);
		eventX = pointer_x;
		eventY = pointer_y;
		eventState = mask;
	} else {
		int origin_x, origin_y;
		gdk_window_get_origin(gdkEvent->window, &origin_x, &origin_y);
		eventX = (int) (gdkEvent->x_root - origin_x);
		eventY = (int) (gdkEvent->y_root - origin_y);
		eventState = gdkEvent->state;
	}
	if ((eventState & GDK_BUTTON1_MASK) == 0)
		return 0;
	GtkWidget *handle = _W_WIDGET(widget)->handle, *phandle;
	GtkAllocation allocation;
	w_point pt;
	w_point *_last = &_W_SASH(widget)->last;
	w_point *_start = &_W_SASH(widget)->start;
	w_size size;
	gtk_widget_get_allocation(handle, &allocation);
	pt.x = allocation.x;
	pt.y = allocation.y;
	size.width = allocation.width;
	size.height = allocation.height;
	int parentBorder = 0;
	w_widget *parent;
	w_widget_get_parent(W_WIDGET(widget), &parent);
	phandle = _W_WIDGET(parent)->handle;
	gtk_widget_get_allocation(phandle, &allocation);
	int parentWidth = allocation.width;
	int parentHeight = allocation.height;
	int newX = _last->x, newY = _last->y;
	if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
		int _newX = WMAX(0, eventX + pt.x - _start->x - parentBorder);
		newX = WMIN(_newX, parentWidth - size.width);
	} else {
		int _newY0 = WMAX(0, eventY + pt.y - _start->y - parentBorder);
		newY = WMIN(_newY0, parentHeight - size.height);
	}
	if (newX == _last->x && newY == _last->y)
		return 0;
	_w_sash_draw_band(widget, _last, &size);
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.widget = widget;
	event.event.data = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = gdkEvent->time;
	event.bounds.x = newX;
	event.bounds.y = newY;
	event.bounds.width = size.width;
	event.bounds.height = size.height;
	if ((_W_WIDGET(widget)->style & W_SMOOTH) == 0) {
		event.detail = W_DRAG;
	}
	_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
	int ClientWidth = 0;
	if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
		ClientWidth = ppriv->get_client_width(W_CONTROL(parent), ppriv);
		event.bounds.x = (ClientWidth - size.width) - event.bounds.x;
	}
	wresult doit = _w_widget_send_event(widget, (w_event*) &event,
			W_EVENT_SEND);
	if (w_widget_is_ok(widget) <= 0)
		return 0;
	if (doit) {
		_last->x = event.bounds.x;
		_last->y = event.bounds.y;
		if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
			_last->x = ClientWidth - size.width - _last->x;
		}
	}
	int flags = UPDATE_ALL;
	if (_W_WIDGET(widget)->style & W_SMOOTH) {
		flags |= UPDATE_FLUSH;
	}
	ppriv->update_0(W_CONTROL(parent), flags, ppriv);
	_w_sash_draw_band(widget, _last, &size);
	if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0) {
		pt.x = event.bounds.x;
		pt.y = _last->y;
		w_control_set_bounds(W_CONTROL(widget), &pt, &size);
	}
	return result;
}
gboolean _gtk_sash_realize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	_w_sash_set_cursor(W_CONTROL(widget), _W_CONTROL(widget)->cursor);
	return _gtk_control_realize(widget, e, priv);
}
wresult _w_sash_compute_size(w_widget *widget, struct w_event_compute_size *e,
		_w_control_priv *priv) {
	if (e->wHint != W_DEFAULT && e->wHint < 0)
		e->wHint = 0;
	if (e->hHint != W_DEFAULT && e->hHint < 0)
		e->hHint = 0;
	int border = W_CONTROL_GET_CLASS(widget)->get_border_width(
			W_CONTROL(widget));
	e->size->width = border * 2;
	e->size->height = border * 2;
	if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
		e->size->width += DEFAULT_WIDTH;
		e->size->height += 3;
	} else {
		e->size->width += 3;
		e->size->height += DEFAULT_HEIGHT;
	}
	if (e->wHint != W_DEFAULT)
		e->size->width = e->wHint + (border * 2);
	if (e->hHint != W_DEFAULT)
		e->size->height = e->hHint + (border * 2);
	return W_TRUE;
}
wresult _w_sash_dispose_class(struct _w_widget_class *clazz) {
	return W_TRUE;
}
void _w_sash_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_sash_class *clazz) {
	if (classId == _W_CLASS_SASH) {
		W_WIDGET_CLASS(clazz)->platformPrivate = &gtk_toolkit->class_sash_priv;
	}
	_w_control_class_init(toolkit, classId, W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SASH;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_sash_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_sash);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_sash);
	W_WIDGET_CLASS(clazz)->dispose_class = _w_sash_dispose_class;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_SASH) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		_W_WIDGET_PRIV(priv)->handle_top = _w_widget_h;
		_W_WIDGET_PRIV(priv)->create_handle = _w_sash_create_handle;
		_W_WIDGET_PRIV(priv)->check_style = _w_sash_check_style;
		_W_WIDGET_PRIV(priv)->hook_events = _w_sash_hook_events;
		_W_WIDGET_PRIV(priv)->compute_size = _w_sash_compute_size;
		priv->handle_fixed = _w_widget_h;
		/*
		 * signals
		 */
		_gtk_signal_fn *signals = priv->widget.signals;
		signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_sash_button_press_event;
		signals[SIGNAL_BUTTON_RELEASE_EVENT] = _gtk_sash_button_release_event;
		signals[SIGNAL_DRAW] = _gtk_sash_draw;
		signals[SIGNAL_FOCUS_IN_EVENT] = _gtk_sash_focus_in_event;
		signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_sash_key_press_event;
		signals[SIGNAL_MOTION_NOTIFY_EVENT] = _gtk_sash_motion_notify_event;
		signals[SIGNAL_REALIZE] = _gtk_sash_realize;
	}
}

