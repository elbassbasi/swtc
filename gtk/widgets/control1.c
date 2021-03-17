/*
 * Name:        control1.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "control.h"
#include "toolkit.h"
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
gboolean _w_control_contained_in_region(w_widget *widget, w_point *input,
		_w_control_priv *priv) {
	GtkWidget *topHandle = priv->widget.handle_top(widget, priv);
	GdkWindow *window = gtk_widget_get_window(topHandle);
	/* if (drawRegion && eventRegion != 0) {
	 return cairo_region_contains_point(eventRegion, x, y);
	 }*/
	return FALSE;
}
/*
 * signals
 */
gboolean _gtk_control_destroy(w_widget *widget, _w_event_platform *ee,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_button_press_event_0(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv, int sendMouseDown) {
	GdkEventButton *gdkEvent = e->args[0];
	w_event_mouse event;
	_W_CONTROL(widget)->lastInput.x = (int) gdkEvent->x;
	_W_CONTROL(widget)->lastInput.y = (int) gdkEvent->y;
	if (_w_control_contained_in_region(widget, &_W_CONTROL(widget)->lastInput,
			priv))
		return FALSE;
	if (gdkEvent->type == GDK_3BUTTON_PRESS)
		return 0;
	/*
	 * When a shell is created with SWT.ON_TOP and SWT.NO_FOCUS,
	 * do not activate the shell when the user clicks on the
	 * the client area or on the border or a control within the
	 * shell that does not take focus.
	 */
	w_shell *shell;
	w_control_get_shell(W_CONTROL(widget), &shell);
	if (((_W_WIDGET(shell)->style & W_ON_TOP) != 0)
			&& (((_W_WIDGET(shell)->style & W_NO_FOCUS) == 0)
					|| ((_W_WIDGET(shell)->style & W_NO_FOCUS) == 0))) {
		//_w_shell_force_active(shell);
	}
	gboolean result = 0;
	if (gdkEvent->type == GDK_BUTTON_PRESS) {
		gboolean dragging = FALSE;
		gtk_toolkit->clickCount = 1;
		GdkEvent *nextEvent = gdk_event_peek();
		if (nextEvent != 0) {
			int eventType = nextEvent->type;
			if (eventType == GDK_2BUTTON_PRESS)
				gtk_toolkit->clickCount = 2;
			if (eventType == GDK_3BUTTON_PRESS)
				gtk_toolkit->clickCount = 3;
			gdk_event_free(nextEvent);
		}
		/*
		 * Feature in GTK: DND detection for X.11 & Wayland support is done through motion notify event
		 * instead of mouse click event. See Bug 503431.
		 */
		if (gtk_toolkit->ISX11) { // Wayland
			if ((_W_WIDGET(widget)->state & STATE_DRAG_DETECT) != 0) {
				if (gdkEvent->button == 1) {
					gboolean consume = FALSE;
					/*if (_w_control_drag_detect_2(W_CONTROL(widget), gdkEvent->x,
					 gdkEvent->y, TRUE,
					 TRUE, &consume)) {
					 dragging = TRUE;
					 if (consume)
					 result = 1;
					 }
					 if (!w_widget_is_ok(widget))
					 return TRUE;*/
				}
			}
		}
		if (sendMouseDown) {
			event.event.type = W_EVENT_MOUSEDOWN;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.event.time = gdkEvent->time;
			event.event.data = 0;
			event.button = gdkEvent->button;
			event.clickcount = gtk_toolkit->clickCount;
			event.x = gdkEvent->x;
			if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
				event.x = priv->get_client_width(W_CONTROL(widget), priv)
						- event.x;
			}
			event.y = gdkEvent->y;
			event.detail = 0;
			event.statemask = _w_widget_set_input_state(gdkEvent->state);
			result = _w_widget_send_event(widget, (w_event*) &event);
		}
		if (!w_widget_is_ok(widget))
			return TRUE;
		/*
		 * Feature in GTK: DND detection for X.11 & Wayland support is done through motion notify event
		 * instead of mouse click event. See Bug 503431.
		 */
		if (gtk_toolkit->ISX11) { // Wayland
			if (dragging) {
				event.event.type = W_EVENT_DRAGDETECT;
				event.event.widget = widget;
				event.event.platform_event = (w_event_platform*) e;
				event.event.time = gdkEvent->time;
				event.event.data = 0;
				event.button = gdkEvent->button;
				event.clickcount = gtk_toolkit->clickCount;
				event.x = gdkEvent->x;
				if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
					event.x = priv->get_client_width(W_CONTROL(widget), priv)
							- event.x;
				}
				event.y = gdkEvent->y;
				event.detail = 0;
				event.statemask = _w_widget_set_input_state(gdkEvent->state);
				_w_widget_send_event(widget, (w_event*) &event);
				if (!w_widget_is_ok(widget))
					return TRUE;
				if (_W_CONTROL(widget)->dragsource != 0) {
					event.event.widget = W_WIDGET(
							_W_CONTROL(widget)->dragsource);
					_w_widget_send_event(
							W_WIDGET(_W_CONTROL(widget)->dragsource),
							(w_event*) &event);
					if (!w_widget_is_ok(widget))
						return TRUE;
				}
			}
		}
		/*
		 * Pop up the context menu in the button press event for widgets
		 * that have default operating system menus in order to stop the
		 * operating system from displaying the menu if necessary.
		 */
		if ((_W_WIDGET(widget)->state & STATE_MENU) != 0) {
			if (gdkEvent->button == 3) {
				/*if (_w_control_show_menu(W_CONTROL(widget),
				 (int) gdkEvent->x_root, (int) gdkEvent->y_root,
				 W_MENU_MOUSE)) {
				 result = TRUE;
				 }*/
			}
		}
	} else {
		gtk_toolkit->clickCount = 2;
		event.event.type = W_EVENT_MOUSEDOUBLECLICK;
		event.event.widget = widget;
		event.event.platform_event = (w_event_platform*) e;
		event.event.time = gdkEvent->time;
		event.button = gdkEvent->button;
		event.clickcount = gtk_toolkit->clickCount;
		event.x = gdkEvent->x_root;
		event.y = gdkEvent->y_root;
		event.detail = 0;
		event.statemask = _w_widget_set_input_state(gdkEvent->state);
		result = _w_widget_send_event(widget, (w_event*) &event);

		if (!w_widget_is_ok(widget))
			return TRUE;
	}
	if (w_widget_is_ok(W_WIDGET(shell))) {
		/*_w_shell_set_active_control_0(shell, W_CONTROL(widget),
		 W_EVENT_MOUSEDOWN);*/
	}
	return result;
}
gboolean _gtk_control_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return _gtk_control_button_press_event_0(widget, e, priv, TRUE);
}
gboolean _gtk_control_button_release_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	GdkEventButton *gdkEvent = e->args[0];
	w_event_mouse event;
	_W_CONTROL(widget)->lastInput.x = (int) gdkEvent->x;
	_W_CONTROL(widget)->lastInput.y = (int) gdkEvent->y;
	if (_w_control_contained_in_region(widget, &_W_CONTROL(widget)->lastInput,
			priv))
		return FALSE;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_MOUSEUP;
	event.event.widget = widget;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = gdkEvent->time;
	event.button = gdkEvent->button;
	event.clickcount = gtk_toolkit->clickCount;
	event.x = gdkEvent->x;
	event.y = gdkEvent->y;
	event.detail = 0;
	event.statemask = _w_widget_set_input_state(gdkEvent->state);
	_w_widget_send_event(widget, (w_event*) &event);
	return FALSE;
}
gboolean _gtk_control_commit(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_event_after(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((_W_WIDGET(widget)->state & STATE_OBSCURED) != 0)
		return 0;
#if USE_CAIRO
	cairo_t *cairo = (cairo_t*) e->args[0];
	w_event_paint event;
	_w_graphics gc;
	_w_graphics_init(W_GRAPHICS(&gc), cairo);
	gdk_cairo_get_clip_rectangle(cairo, &gc.clipping);
	memset(&event, 0, sizeof(event));
	//event.count = 1;
	event.event.type = W_EVENT_PAINT;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.bounds.x = gc.clipping.x;
	event.bounds.y = gc.clipping.y;
	event.bounds.width = gc.clipping.width;
	event.bounds.height = gc.clipping.height;
	if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
		event.bounds.x = priv->get_client_width(W_CONTROL(widget), priv)
				- event.bounds.width - event.bounds.x;
	}
	/*
	 * Pass the region into the GCData so that GC.fill* methods can be aware of the region
	 * and clip themselves accordingly. Only relevant on GTK3.10+, see bug 475784.
	 */
	/*if (drawRegion)
	 data.regionSet = eventRegion;*/
//	data.damageRgn = gdkEvent.region;
	event.gc = W_GRAPHICS(&gc);
	/*w_graphics_set_clipping_rect(W_GRAPHICS(&gc), &event.bounds);
	 w_font *font;
	 w_control_get_font(W_CONTROL(widget), &font);
	 w_graphics_set_font(W_GRAPHICS(&gc), font);
	 w_color background;
	 w_control_get_background(W_CONTROL(widget), &background);
	 w_graphics_set_background(W_GRAPHICS(&gc), background);
	 w_color foreground;
	 w_control_get_foreground(W_CONTROL(widget), &foreground);
	 w_graphics_set_foreground(W_GRAPHICS(&gc), foreground);*/
	priv->draw_widget(W_CONTROL(widget), W_GRAPHICS(&gc), priv);
	_w_widget_send_event(widget, (w_event*) &event);
	w_graphics_dispose(W_GRAPHICS(&gc));
#else
#endif
	return 0;
}
gboolean _gtk_control_expose_inverse(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_focus(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_key_release_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_mnemonic_activate(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_motion_notify_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	w_event_mouse event;
	int result;
	GdkEventMotion *gdkEvent = (GdkEventMotion*) e->args[0];
	_W_CONTROL(widget)->lastInput.x = (int) gdkEvent->x;
	_W_CONTROL(widget)->lastInput.y = (int) gdkEvent->y;
	if (_w_control_contained_in_region(widget, &_W_CONTROL(widget)->lastInput,
			priv))
		return FALSE;
	/*
	 * Feature in GTK: DND detection for X.11 & Wayland support is done through motion notify event
	 * instead of mouse click event. See Bug 503431.
	 */
	if (gtk_toolkit->ISWayland) { // Wayland
		gboolean dragging = FALSE;
		if ((_W_WIDGET(widget)->state & STATE_DRAG_DETECT) != 0) {
			gboolean consume = FALSE;
			/*if (_w_control_drag_detect_2(W_CONTROL(widget), gdkEvent->x,
			 gdkEvent->y, TRUE, TRUE, &consume)) {
			 dragging = TRUE;
			 if (consume)
			 result = TRUE;
			 if (!w_widget_is_ok(widget))
			 return TRUE;
			 } else {
			 }*/
		}
		if (dragging) {
			//gtk_event_controller_handle_event(dragGesture, event);
			GdkEventButton *gdkEvent1 = (GdkEventButton*) e->args[0];
			if (gdkEvent1->type == GDK_3BUTTON_PRESS)
				return FALSE;
			event.event.type = W_EVENT_DRAGDETECT;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.event.time = gdkEvent1->time;
			event.event.data = 0;
			event.button = gdkEvent1->button;
			event.clickcount = gtk_toolkit->clickCount;
			event.x = gdkEvent1->x;
			if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
				event.x = priv->get_client_width(W_CONTROL(widget), priv)
						- event.x;
			}
			event.y = gdkEvent1->y;
			event.detail = 0;
			event.statemask = _w_widget_set_input_state(gdkEvent1->state);
			if (_w_widget_send_event(widget, (w_event*) &event)) {
				return TRUE;
			}
			if (!w_widget_is_ok(widget))
				return TRUE;
			if (_W_CONTROL(widget)->dragsource != 0) {
				event.event.widget = W_WIDGET(_W_CONTROL(widget)->dragsource);
				_w_widget_send_event(W_WIDGET(_W_CONTROL(widget)->dragsource),
						(w_event*) &event);
			}
		}
	}
	if (W_CONTROL(widget) == gtk_toolkit->currentControl) {
		//display.addMouseHoverTimeout(handle);
	}
	double x = gdkEvent->x_root, y = gdkEvent->y_root;
	int state = gdkEvent->state;
	if (gdkEvent->is_hint != 0) {
		int pointer_x, pointer_y;
		GdkModifierType mask;
		GdkWindow *window = priv->window_event(widget, priv);
		_gdk_window_get_device_position(window, &pointer_x, &pointer_y, &mask);
		x = pointer_x;
		y = pointer_y;
		state = mask;
	}
#if GTK3
	if (W_CONTROL(widget) != gtk_toolkit->currentControl) {
		if (gtk_toolkit->currentControl != 0
				&& w_widget_is_ok(W_WIDGET(gtk_toolkit->currentControl))) {
			//display.removeMouseHoverTimeout(display.currentControl.handle);
			w_point pt, tmp;
			tmp.x = x;
			tmp.y = y;
			_w_toolkit_map_0(W_TOOLKIT(gtk_toolkit), W_CONTROL(widget),
					gtk_toolkit->currentControl, &pt, &tmp);
			event.event.type = W_EVENT_MOUSEEXIT;
			event.event.widget = W_WIDGET(gtk_toolkit->currentControl);
			event.event.platform_event = (w_event_platform*) e;
			event.event.time = gdkEvent->time;
			event.event.data = 0;
			event.button = 0;
			event.clickcount = gtk_toolkit->clickCount;
			w_control *currentControl = gtk_toolkit->currentControl;
			_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(currentControl);
			if (gdkEvent->is_hint) {
				event.x = pt.x;
				event.y = pt.y;
			} else {
				GdkWindow *window = cpriv->window_event(
						W_WIDGET(currentControl), cpriv);
				int origin_x = 0, origin_y = 0;
				gdk_window_get_origin(window, &origin_x, &origin_y);
				event.x = pt.x - origin_x;
				event.y = pt.y - origin_y;
			}
			if ((_W_WIDGET(gtk_toolkit->currentControl)->style & W_MIRRORED)
					!= 0) {
				event.x = cpriv->get_client_width(currentControl, priv)
						- event.x;
			}
			event.detail = 0;
			event.statemask = _w_widget_set_input_state(state);
			_w_widget_send_event(W_WIDGET(gtk_toolkit->currentControl),
					(w_event*) &event);
		}
		if (w_widget_is_ok(widget)) {
			gtk_toolkit->currentControl = W_CONTROL(widget);
			event.event.type = W_EVENT_MOUSEENTER;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.event.time = gdkEvent->time;
			event.event.data = 0;
			event.button = 0;
			event.clickcount = gtk_toolkit->clickCount;
			if (gdkEvent->is_hint) {
				event.x = x;
				event.y = y;
			} else {
				GdkWindow *window = priv->window_event(widget, priv);
				int origin_x = 0, origin_y = 0;
				gdk_window_get_origin(window, &origin_x, &origin_y);
				event.x = x - origin_x;
				event.y = y - origin_y;
			}
			if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
				event.x = priv->get_client_width(W_CONTROL(widget), priv)
						- event.x;
			}
			event.detail = 0;
			event.statemask = _w_widget_set_input_state(state);
			_w_widget_send_event(widget, (w_event*) &event);
		}
	}
#endif
	event.event.type = W_EVENT_MOUSEMOVE;
	event.event.widget = widget;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = gdkEvent->time;
	event.event.data = 0;
	event.button = 0;
	event.clickcount = gtk_toolkit->clickCount;
	if (gdkEvent->is_hint) {
		event.x = x;
		event.y = y;
	} else {
		GdkWindow *window = priv->window_event(widget, priv);
		int origin_x = 0, origin_y = 0;
		gdk_window_get_origin(window, &origin_x, &origin_y);
		event.x = x - origin_x;
		event.y = y - origin_y;
	}
	if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
		event.x = priv->get_client_width(W_CONTROL(widget), priv) - event.x;
	}
	event.detail = 0;
	event.statemask = _w_widget_set_input_state(state);
	result = _w_widget_send_event(widget, (w_event*) &event);
	return result;
}
gboolean _gtk_control_popup_menu(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_preedit_changed(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_realize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GtkIMContext *imHandle = priv->handle_im(widget, priv);
	if (imHandle != 0) {
		GtkWidget *paintHandle = priv->handle_paint(widget, priv);
		GdkWindow *window = gtk_widget_get_window(paintHandle);
		gtk_im_context_set_client_window(GTK_IM_CONTEXT(imHandle), window);
	}
	if (_W_CONTROL(widget)->backgroundImage != 0) {
		/*_w_control_set_background_pixmap(W_CONTROL(widget),
		 _W_CONTROL(widget)->backgroundImage, priv);*/
	}
	return FALSE;
}
gboolean _gtk_control_scroll_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_show_help(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_style_set(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_unrealize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
