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
	w_event e;
	if (!w_widget_is_ok(widget))
		return TRUE;
	if (gtk_toolkit->currentControl == W_CONTROL(widget)) {
		gtk_toolkit->currentControl = 0;
	}
	if (gtk_toolkit->focusControl == W_CONTROL(widget)) {
		gtk_toolkit->focusControl = 0;
	}
	if (gtk_toolkit->imControl == W_CONTROL(widget)) {
		gtk_toolkit->imControl = 0;
	}
	if (gtk_toolkit->mnemonicControl == W_CONTROL(widget)) {
		gtk_toolkit->mnemonicControl = 0;
	}
	w_dragsource *dragsource = _W_CONTROL(widget)->dragsource;
	if (dragsource != 0) {
		w_widget_dispose(W_WIDGET(dragsource));
	}
	w_droptarget *droptarget = _W_CONTROL(widget)->droptarget;
	if (droptarget != 0) {
		w_widget_dispose(W_WIDGET(droptarget));
	}
	w_widget *p = _W_WIDGET(widget)->parent;
	if (p != 0) {
		w_link_unlink_0(&_W_WIDGET(widget)->sibling, widget,
				(void**) &_W_WIDGET(p)->first_child);
		_W_WIDGET(p)->children_count--;
	}
	wuint64 style = w_widget_get_style(widget);
	_w_control_kill_all_timer(W_CONTROL(widget));
	e.type = W_EVENT_DISPOSE;
	e.widget = widget;
	e.data = 0;
	e.time = 0;
	e.platform_event = (w_event_platform*) ee;
	_w_widget_send_event(widget, &e, W_EVENT_SEND);
	widget->clazz = 0;
	if (style & W_FREE_MEMORY) {
		_w_toolkit_registre_free(widget);
	}
	return FALSE;
}
gboolean _gtk_control_button_press_event_0(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv, int sendMouseDown) {
	GdkEventButton *gdkEvent = e->args[0];
	w_event_mouse event;
	_W_CONTROL(widget)->lastInput.x = gdkEvent->x;
	_W_CONTROL(widget)->lastInput.y = gdkEvent->y;
	if (_w_control_contained_in_region(widget, &_W_CONTROL(widget)->lastInput,
			priv))
		return FALSE;
	if (gdkEvent->type == GDK_3BUTTON_PRESS)
		return FALSE;
	/*
	 * When a shell is created with SWT.ON_TOP and SWT.NO_FOCUS,
	 * do not activate the shell when the user clicks on the
	 * the client area or on the border or a control within the
	 * shell that does not take focus.
	 */
	w_shell *shell;
	w_widget_get_shell(W_WIDGET(widget), &shell);
	wuint64 shellStyle = _W_WIDGET(shell)->style;
	if (((shellStyle & W_ON_TOP) != 0)
			&& (((shellStyle & W_NO_FOCUS) == 0)
					|| ((shellStyle & W_NO_FOCUS) == 0))) {
		w_shell_force_active(shell);
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
		//if (gtk_toolkit->ISX11) { // Wayland
		if ((_W_WIDGET(widget)->state & STATE_DRAG_DETECT) != 0) {
			if (gdkEvent->button == 1) {
				gboolean consume = FALSE;
				if (_w_control_drag_detect_2(W_CONTROL(widget), gdkEvent->x,
						gdkEvent->y, TRUE,
						TRUE, &consume)) {
					dragging = TRUE;
					if (consume)
						result = 1;
				}
				if (!w_widget_is_ok(widget))
					return TRUE;
			}
		}
		//}
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
			event.detail = _w_widget_set_input_state(gdkEvent->state);
			result = _w_widget_send_event(widget, (w_event*) &event,
					W_EVENT_SEND);
		}
		if (!w_widget_is_ok(widget))
			return TRUE;
		/*
		 * Feature in GTK: DND detection for X.11 & Wayland support is done through motion notify event
		 * instead of mouse click event. See Bug 503431.
		 */
		//if (gtk_toolkit->ISX11) { // Wayland
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
			event.detail = _w_widget_set_input_state(gdkEvent->state);
			_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
			if (!w_widget_is_ok(widget))
				return TRUE;
			if (_W_CONTROL(widget)->dragsource != 0) {
				event.event.widget = W_WIDGET(_W_CONTROL(widget)->dragsource);
				_w_widget_send_event(W_WIDGET(_W_CONTROL(widget)->dragsource),
						(w_event*) &event, W_EVENT_SEND);
				if (!w_widget_is_ok(widget))
					return TRUE;
			}
		}
		//}
		/*
		 * Pop up the context menu in the button press event for widgets
		 * that have default operating system menus in order to stop the
		 * operating system from displaying the menu if necessary.
		 */
		if ((_W_WIDGET(widget)->state & STATE_MENU) != 0) {
			if (gdkEvent->button == 3) {
				if (_w_control_show_menu(W_CONTROL(widget), gdkEvent->x_root,
						gdkEvent->y_root, W_MENU_MOUSE)) {
					result = TRUE;
				}
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
		event.detail = _w_widget_set_input_state(gdkEvent->state);
		result = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);

		if (!w_widget_is_ok(widget))
			return TRUE;
	}
	if (w_widget_is_ok(W_WIDGET(shell))) {
		_w_shell_set_active_control_0(shell, W_CONTROL(widget),
				W_EVENT_MOUSEDOWN);
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
	event.detail = _w_widget_set_input_state(gdkEvent->state);
	_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	return FALSE;
}
gboolean _gtk_control_commit(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	char *text = (char*) e->args[0];
	if (text == 0)
		return FALSE;
	int length = strlen(text);
	if (length == 0)
		return FALSE;
	_w_widget_send_IM_key_event(widget, e, W_EVENT_KEYDOWN, 0, text, length);
	return FALSE;
}
gboolean _gtk_control_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Feature in GTK. Children of a shell will inherit and display the shell's
	 * tooltip if they do not have a tooltip of their own. The fix is to use the
	 * new tooltip API in GTK 2.12 to null the shell's tooltip when the control
	 * being entered does not have any tooltip text set.
	 */
	char *buffer = 0;
	char *toolTipText = _W_CONTROL(widget)->tooltiptext;
	int newlength, mnemonic;
	buffer = _gtk_text_fix(toolTipText, -1, W_ENCODING_UTF8, &newlength,
			&mnemonic);
	w_shell *shell;
	w_widget_get_shell(widget, &shell);
	GtkWidget *toolHandle = _W_WIDGET(shell)->handle;
	gtk_widget_set_tooltip_text(toolHandle, buffer);
	_gtk_text_free(toolTipText, buffer, newlength);

	if (gtk_toolkit->currentControl == W_CONTROL(widget))
		return FALSE;
	GdkEventCrossing *gdkEvent = (GdkEventCrossing*) e->args[0];
	w_point *_lastInput = &_W_CONTROL(widget)->lastInput;
	_lastInput->x = (int) gdkEvent->x;
	_lastInput->y = (int) gdkEvent->y;
	if (_w_control_contained_in_region(widget, _lastInput, priv))
		return FALSE;

	/*
	 * It is possible to send out too many enter/exit events if entering a
	 * control through a subwindow. The fix is to return without sending any
	 * events if the GdkEventCrossing subwindow field is set and the control
	 * requests to check the field.
	 */
	if (gdkEvent->subwindow != 0
			&& priv->check_subwindow(W_CONTROL(widget), priv))
		return FALSE;
	if (gdkEvent->mode != GDK_CROSSING_NORMAL
			&& gdkEvent->mode != GDK_CROSSING_UNGRAB)
		return FALSE;
	if ((gdkEvent->state
			& (GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK)) != 0)
		return FALSE;
	w_event_mouse event;
	if (w_widget_is_ok(W_WIDGET(gtk_toolkit->currentControl))) {
		//display.removeMouseHoverTimeout(display.currentControl.handle);
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_MOUSEEXIT;
		event.event.widget = widget;
		event.event.platform_event = (w_event_platform*) e;
		event.event.time = gdkEvent->time;
		event.button = 0;
		event.clickcount = gtk_toolkit->clickCount;
		event.x = gdkEvent->x_root;
		event.y = gdkEvent->y_root;
		event.detail = _w_widget_set_input_state(gdkEvent->state);
		_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	}
	if (w_widget_is_ok(widget)) {
		gtk_toolkit->currentControl = W_CONTROL(widget);
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_MOUSEENTER;
		event.event.widget = widget;
		event.event.platform_event = (w_event_platform*) e;
		event.event.time = gdkEvent->time;
		event.button = 0;
		event.clickcount = gtk_toolkit->clickCount;
		event.x = gdkEvent->x_root;
		event.y = gdkEvent->y_root;
		event.detail = _w_widget_set_input_state(gdkEvent->state);
		_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	}
	return FALSE;
}
gboolean _gtk_control_event_after(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GdkEvent *event = (GdkEvent*) e->args[0];
	switch (event->type) {
	case GDK_BUTTON_PRESS: {
		if (e->widget != priv->handle_event(widget, priv))
			break;
		/*
		 * Pop up the context menu in the event_after signal to allow
		 * the widget to process the button press.  This allows widgets
		 * such as GtkTreeView to select items before a menu is shown.
		 */
		if ((_W_WIDGET(widget)->state & STATE_MENU) == 0) {
			GdkEventButton *gdkEventButton = (GdkEventButton*) event;
			if (gdkEventButton->button == 3) {
				_w_control_show_menu(W_CONTROL(widget), gdkEventButton->x_root,
						gdkEventButton->y_root, W_MENU_MOUSE);
			}
		}
		break;
	}
	case GDK_FOCUS_CHANGE: {
		if (priv->is_focus_handle(W_CONTROL(widget), e->widget, priv) <= 0)
			break;
		GdkEventFocus *gdkEventFocus = (GdkEventFocus*) event;

		/*
		 * Feature in GTK. The GTK combo box popup under some window managers
		 * is implemented as a GTK_MENU.  When it pops up, it causes the combo
		 * box to lose focus when focus is received for the menu.  The
		 * fix is to check the current grab handle and see if it is a GTK_MENU
		 * and ignore the focus event when the menu is both shown and hidden.
		 *
		 * NOTE: This code runs for all menus.
		 */
		if (gdkEventFocus->in != 0) {
			if (gtk_toolkit->ignoreFocus) {
				gtk_toolkit->ignoreFocus = FALSE;
				break;
			}
		} else {
			gtk_toolkit->ignoreFocus = FALSE;
			GtkWidget *grabHandle = gtk_grab_get_current();
			if (grabHandle != 0) {
				if (G_OBJECT_TYPE (grabHandle) == GTK_TYPE_MENU) {
					gtk_toolkit->ignoreFocus = TRUE;
					break;
				}
			}
		}
		w_event event;
		if (gdkEventFocus->in != 0) {
			event.type = W_EVENT_FOCUSIN;
		} else {
			event.type = W_EVENT_FOCUSOUT;
		}
		event.widget = widget;
		event.time = 0;
		event.platform_event = (w_event_platform*) e;
		event.data = 0;
		_w_widget_send_event(widget, &event, W_EVENT_SEND);
		break;
	}
	}
	return FALSE;
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
	w_graphics_set_clipping_rect(W_GRAPHICS(&gc), &event.bounds);
	w_font *font;
	w_control_get_font(W_CONTROL(widget), &font);
	w_graphics_set_font(W_GRAPHICS(&gc), font);
	w_color background;
	w_control_get_background(W_CONTROL(widget), &background);
	w_graphics_set_background(W_GRAPHICS(&gc), background);
	w_color foreground;
	w_control_get_foreground(W_CONTROL(widget), &foreground);
	w_graphics_set_foreground(W_GRAPHICS(&gc), foreground);
	priv->draw_widget(W_CONTROL(widget), W_GRAPHICS(&gc), priv);
	_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
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
	/* Stop GTK traversal for every widget */
	return TRUE;
}
gboolean _gtk_control_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	// widget could be disposed at this point
	if (_W_WIDGET(widget)->handle != 0) {
		w_control *oldControl = gtk_toolkit->imControl;
		if (oldControl != W_CONTROL(widget)) {
			if (w_widget_is_ok(W_WIDGET(oldControl))) {
				_w_control_priv *oldPriv = _W_CONTROL_GET_PRIV(oldControl);
				GtkIMContext *oldIMHandle = oldPriv->handle_im(
						W_WIDGET(oldControl), oldPriv);
				if (oldIMHandle != 0)
					gtk_im_context_reset(oldIMHandle);
			}
		}
		//if (hooks(SWT.KeyDown) || hooks(SWT.KeyUp)) {
		GtkIMContext *imHandle = priv->handle_im(widget, priv);
		if (imHandle != 0)
			gtk_im_context_focus_in(imHandle);
		//}
	}
	return 0;
}
gboolean _gtk_control_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	// widget could be disposed at this point
	if (_W_WIDGET(widget)->handle != 0) {
		GtkIMContext *imHandle = priv->handle_im(widget, priv);
		if (imHandle != 0) {
			gtk_im_context_focus_out(imHandle);
		}
	}
	return 0;
}
gboolean _w_control_filter_key(w_control *control, int keyval,
		GdkEventKey *event, _w_control_priv *priv) {
	GtkIMContext *imHandle = priv->handle_im(W_WIDGET(control), priv);
	if (imHandle != 0) {
		return gtk_im_context_filter_keypress(imHandle, event);
	}
	return FALSE;
}
gboolean _gtk_control_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GdkEventKey *gdkEvent = (GdkEventKey*) e->args[0];
	if (!priv->has_focus(W_CONTROL(widget), priv)) {
		/*
		 * Feature in GTK.  On AIX, the IME window deactivates the current shell and even
		 * though the widget receiving the key event is not in focus, it should filter the input in
		 * order to get it committed.  The fix is to detect that the widget shell is not active
		 * and call filterKey() only.
		 */
		if (_w_toolkit_get_active_shell(W_TOOLKIT(gtk_toolkit)) == 0) {
			if (_w_control_filter_key(W_CONTROL(widget), gdkEvent->keyval,
					gdkEvent, priv))
				return TRUE;
		}
		return FALSE;
	}

	if (_w_control_translate_mnemonic_0(W_CONTROL(widget), gdkEvent->keyval,
			gdkEvent, priv))
		return TRUE;
	// widget could be disposed at this point
	if (!w_widget_is_ok(widget))
		return FALSE;

	if (_w_control_filter_key(W_CONTROL(widget), gdkEvent->keyval, gdkEvent,
			priv))
		return TRUE;
	// widget could be disposed at this point
	if (!w_widget_is_ok(widget))
		return FALSE;

	if (priv->translate_traversal(W_CONTROL(widget), gdkEvent, priv))
		return TRUE;
	// widget could be disposed at this point
	if (!w_widget_is_ok(widget))
		return FALSE;
	return priv->widget.send_key_event(widget, e, priv);
}
gboolean _gtk_control_key_release_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (!priv->has_focus(W_CONTROL(widget), priv))
		return FALSE;
	GtkIMContext *imHandle = priv->handle_im(widget, priv);
	if (imHandle != 0) {
		if (gtk_im_context_filter_keypress(imHandle, (GdkEventKey*) e->args[0]))
			return TRUE;
	}
	return priv->widget.send_key_event(widget, e, priv);
}
gboolean _gtk_control_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_mouse event;
	if (gtk_toolkit->currentControl != W_CONTROL(widget))
		return FALSE;
	GdkEventCrossing *gdkEvent = (GdkEventCrossing*) e->args[0];
	w_point *_lastInput = &_W_CONTROL(widget)->lastInput;
	_lastInput->x = gdkEvent->x;
	_lastInput->y = gdkEvent->y;
	if (_w_control_contained_in_region(widget, _lastInput, priv))
		return FALSE;
	//display.removeMouseHoverTimeout(handle);
	gboolean result = FALSE;
	if (priv->send_leave_notify(W_CONTROL(widget), priv) > 0
			|| _w_toolkit_get_cursor_control(W_TOOLKIT(gtk_toolkit)) == 0) {
		if (gdkEvent->mode != GDK_CROSSING_NORMAL
				&& gdkEvent->mode != GDK_CROSSING_UNGRAB)
			return FALSE;
		if ((gdkEvent->state
				& (GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK))
				!= 0)
			return FALSE;
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_MOUSEEXIT;
		event.event.widget = widget;
		event.event.platform_event = (w_event_platform*) e;
		event.event.time = gdkEvent->time;
		event.button = 0;
		event.clickcount = gtk_toolkit->clickCount;
		event.x = gdkEvent->x_root;
		event.y = gdkEvent->y_root;
		event.detail = _w_widget_set_input_state(gdkEvent->state);
		_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
		gtk_toolkit->currentControl = 0;
	}
	return result;
}
gboolean _gtk_control_mnemonic_activate(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = FALSE;
	GdkEventKey *keyEvent = (GdkEventKey*) gtk_get_current_event();
	if (keyEvent != 0) {
		if (keyEvent->type == GDK_KEY_PRESS) {
			w_control *focusControl = _w_toolkit_get_focus_control(
					W_TOOLKIT(gtk_toolkit));
			GtkWidget *focusHandle = 0;
			if (w_widget_is_ok(W_WIDGET(focusControl))) {
				_w_control_priv *fpriv = _W_CONTROL_GET_PRIV(focusControl);
				focusHandle = fpriv->handle_focus(W_WIDGET(focusControl),
						fpriv);
			}
			if (focusHandle != 0) {
				gtk_toolkit->mnemonicControl = W_CONTROL(widget);
				gtk_widget_event(focusHandle, (GdkEvent*) keyEvent);
				gtk_toolkit->mnemonicControl = 0;
			}
			result = TRUE;
		}
		gdk_event_free((GdkEvent*) keyEvent);
	}
	return result;
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
			if (_w_control_drag_detect_2(W_CONTROL(widget), gdkEvent->x,
					gdkEvent->y, TRUE, TRUE, &consume)) {
				dragging = TRUE;
				if (consume)
					result = TRUE;
				if (!w_widget_is_ok(widget))
					return TRUE;
			} else {
			}
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
			event.detail = _w_widget_set_input_state(gdkEvent1->state);
			if (_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND)) {
				return TRUE;
			}
			if (!w_widget_is_ok(widget))
				return TRUE;
			if (_W_CONTROL(widget)->dragsource != 0) {
				event.event.widget = W_WIDGET(_W_CONTROL(widget)->dragsource);
				_w_widget_send_event(W_WIDGET(_W_CONTROL(widget)->dragsource),
						(w_event*) &event, W_EVENT_SEND);
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
			event.detail = _w_widget_set_input_state(state);
			_w_widget_send_event(W_WIDGET(gtk_toolkit->currentControl),
					(w_event*) &event, W_EVENT_SEND);
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
			event.detail = _w_widget_set_input_state(state);
			_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
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
	event.detail = _w_widget_set_input_state(state);
	result = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	return result;
}
gboolean _gtk_control_popup_menu(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (!_w_control_has_focus(W_CONTROL(widget), priv))
		return FALSE;
	int x, y;
	GdkModifierType mask;
	_gdk_window_get_device_position(0, &x, &y, &mask);
	return _w_control_show_menu(W_CONTROL(widget), x, y, W_MENU_KEYBOARD);
}
gboolean _gtk_control_preedit_changed(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	//display.showIMWindow(this);
	return FALSE;
}
gboolean _gtk_control_realize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GtkIMContext *imHandle = priv->handle_im(widget, priv);
	if (imHandle != 0) {
		GtkWidget *paintHandle = priv->handle_paint(widget, priv);
		GdkWindow *window = gtk_widget_get_window(paintHandle);
		gtk_im_context_set_client_window(GTK_IM_CONTEXT(imHandle), window);
	}
	GdkPixbuf *backgroundImage = _W_CONTROL(widget)->backgroundImage.pixbuf;
	if (backgroundImage != 0) {
		_w_control_set_background_pixmap(W_CONTROL(widget), backgroundImage,
				priv);
	}
	return FALSE;
}
gboolean _gtk_control_scroll_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GdkEventScroll *gdkEvent = (GdkEventScroll*) e->args[0];
	w_point *lastInput = &_W_CONTROL(widget)->lastInput;
	lastInput->x = gdkEvent->x;
	lastInput->y = gdkEvent->y;
	if (_w_control_contained_in_region(widget, lastInput, priv))
		return FALSE;
	w_event_mouse event;
	event.event.type = 0;
	switch (gdkEvent->direction) {
	case GDK_SCROLL_UP:
		event.event.type = W_EVENT_MOUSEVERTICALWHEEL;
		event.clickcount = 3;
		event.detail = W_SCROLL_LINE;
		break;
	case GDK_SCROLL_DOWN:
		event.event.type = W_EVENT_MOUSEVERTICALWHEEL;
		event.clickcount = -3;
		event.detail = W_SCROLL_LINE;
		break;
	case GDK_SCROLL_LEFT:
		event.event.type = W_EVENT_MOUSEHORIZONTALWHEEL;
		event.clickcount = 3;
		event.detail = 0;
		break;
	case GDK_SCROLL_RIGHT:
		event.event.type = W_EVENT_MOUSEHORIZONTALWHEEL;
		event.clickcount = -3;
		event.detail = 0;
		break;
	case GDK_SCROLL_SMOOTH: {
		double delta_x = 0, delta_y = 0;
		if (gdk_event_get_scroll_deltas((GdkEvent*) gdkEvent, &delta_x,
				&delta_y)) {
			if (delta_x != 0) {
				event.event.type = W_EVENT_MOUSEHORIZONTALWHEEL;
				event.clickcount = -3 * delta_x;
				event.detail = 0;
			}
			if (delta_y != 0) {
				event.event.type = W_EVENT_MOUSEVERTICALWHEEL;
				event.clickcount = -3 * delta_y;
				event.detail = W_SCROLL_LINE;
			}
		}
	}
		break;
	}
	if (event.event.type != 0) {
		event.event.widget = widget;
		event.event.platform_event = (w_event_platform*) e;
		event.event.time = gdkEvent->time;
		event.event.data = 0;
		event.button = 0;
		event.x = gdkEvent->x_root;
		event.y = gdkEvent->y_root;
		GdkWindow *window = priv->window_event(widget, priv);
		int origin_x = 0, origin_y = 0;
		gdk_window_get_origin(window, &origin_x, &origin_y);
		event.x = event.x - origin_x;
		event.y = event.y - origin_y;
		if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
			int client_width = priv->get_client_width(W_CONTROL(widget), priv);
			event.x = client_width - event.x;
		}
		event.detail = _w_widget_set_input_state(gdkEvent->state);
		return _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	}
	return FALSE;
}
gboolean _gtk_control_show_help(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (!priv->has_focus(W_CONTROL(widget), priv))
		return FALSE;
	w_event event;
	w_control *control = W_CONTROL(widget);
	while (control != 0) {
		event.type = W_EVENT_HELP;
		event.platform_event = (w_event_platform*) e;
		event.time = 0;
		event.widget = W_WIDGET(control);
		event.data = 0;
		if (_w_widget_send_event(W_WIDGET(control), &event, W_EVENT_SEND)) {
			return TRUE;
		}
		control = W_CONTROL(_W_WIDGET(control)->parent);
	}
	return FALSE;
}
gboolean _gtk_control_style_set(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GdkPixbuf *backgroundImage = _W_CONTROL(widget)->backgroundImage.pixbuf;
	if (backgroundImage != 0) {
		_w_control_set_background_pixmap(W_CONTROL(widget), backgroundImage,
				priv);
	}
	return FALSE;
}
gboolean _gtk_control_unrealize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GtkIMContext *imHandle = priv->handle_im(widget, priv);
	if (imHandle != 0)
		gtk_im_context_set_client_window(GTK_IM_CONTEXT(imHandle), 0);
	return FALSE;
}
