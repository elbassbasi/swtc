/*
 * Name:        control0.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "control.h"
#include "toolkit.h"
#include "dnd.h"
#include <sys/time.h>
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
int enterNotifyEventSignalId = 0;
GQuark GTK_POINTER_WINDOW = 0;
GQuark SWT_GRAB_WIDGET = 0;
void gtk_widget_set_align(GtkWidget *widget, GtkAlign hAlign, GtkAlign vAlign) {
	gtk_widget_set_halign(widget, hAlign);
	gtk_widget_set_valign(widget, vAlign);
}
void _w_control_check_background(w_control *control, _w_control_priv *priv) {
}
void _w_control_check_border(w_control *control, _w_control_priv *priv) {
}
void _w_control_check_buffered(w_control *control, _w_control_priv *priv) {
}
void _w_control_check_foreground(w_control *control, _w_control_priv *priv) {
}
void _w_control_check_mirrored(w_control *control, _w_control_priv *priv) {
}
wresult _w_control_check_subwindow(w_control *control, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_compute_native_size(w_widget *widget, GtkWidget *h,
		w_event_compute_size *e, _w_control_priv *priv) {
	int width = e->wHint, height = e->hHint;
#if GTK3
	if (e->wHint == W_DEFAULT && e->hHint == W_DEFAULT) {
		GtkRequisition requisition;
		gtk_widget_get_preferred_size(h, NULL, &requisition);
		width = requisition.width;
		height = requisition.height;
	} else if (e->wHint == W_DEFAULT || e->hHint == W_DEFAULT) {
		int natural_size;
		if (e->wHint == W_DEFAULT) {
			gtk_widget_get_preferred_width_for_height(h, height, NULL,
					&natural_size);
			width = natural_size;
		} else {
			gtk_widget_get_preferred_height_for_width(h, width, NULL,
					&natural_size);
			height = natural_size;
		}
	}
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
#else
#endif
}
wresult _w_control_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	return _w_control_compute_native_size(widget, handle, e, priv);
}
wresult _w_control_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_widget_is_ok(parent))
		return W_ERROR_INVALID_ARGUMENT;
	if (w_widget_class_id(parent) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_ARGUMENT;
	_w_widget_priv *ppriv = _W_WIDGET_GET_PRIV(parent);
	ppriv->check_open(parent, _W_CONTROL_PRIV(ppriv));
	_W_WIDGET(widget)->parent = parent;
	_W_WIDGET(widget)->post_event = post_event;
	_w_widget_priv *priv = _W_WIDGET_GET_PRIV(widget);
	_W_WIDGET(widget)->style = priv->check_style(widget, style);
	_W_WIDGET(widget)->state |= STATE_TAB_LIST;
	return priv->create_widget(widget, _W_CONTROL_PRIV(priv));
}
wresult _w_control_create_dragsource(w_control *control,
		w_dragsource *dragsource, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (control == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_widget_is_ok(W_WIDGET(control)))
		return W_ERROR_INVALID_ARGUMENT;
	if (_W_CONTROL(control)->dragsource != 0) {
		return W_ERROR_CANNOT_INIT_DRAG;
	}
	if (style == W_NONE) {
		style = W_DROP_MOVE;
	}
	w_widget_init(W_WIDGET(dragsource));
	struct _w_widget_class *clazz;
	clazz = (struct _w_widget_class*) w_toolkit_get_class(
			W_TOOLKIT(gtk_toolkit), _W_CLASS_DRAGSOURCE);
	if (clazz == 0)
		return W_ERROR_INVALID_SUBCLASS;
	if (clazz->class_id == 0)
		w_toolkit_init_class(W_TOOLKIT(gtk_toolkit), _W_CLASS_DRAGSOURCE,
				clazz);
	memset(&(dragsource->widget.clazz), 0, clazz->object_used_size);
	dragsource->widget.clazz = clazz;
	_W_CONTROL(control)->dragsource = W_DRAGSOURCE(dragsource);
	_W_DRAGSOURCE(dragsource)->control = W_CONTROL(control);
	_W_WIDGET(dragsource)->post_event = post_event;
	_W_WIDGET(dragsource)->style = style;
	_gtk_signal *signals = gtk_toolkit->signals;
	GtkWidget *handle = _W_WIDGET(control)->handle;
	_w_widget_connect(handle, &signals[SIGNAL_DRAG_DATA_GET],
	FALSE);
	_w_widget_connect(handle, &signals[SIGNAL_DRAG_END], FALSE);
	_w_widget_connect(handle, &signals[SIGNAL_DRAG_DATA_DELETE],
	FALSE);
	return W_TRUE;
}
wresult _w_control_create_droptarget(w_control *control,
		w_droptarget *droptarget, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (control == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_widget_is_ok(W_WIDGET(control)))
		return W_ERROR_INVALID_ARGUMENT;
	if (_W_CONTROL(control)->droptarget != 0) {
		return W_ERROR_CANNOT_INIT_DRAG;
	}
	if (style == W_NONE) {
		style = W_DROP_MOVE;
	}
	w_widget_init(W_WIDGET(droptarget));
	struct _w_widget_class *clazz;
	clazz = (struct _w_widget_class*) w_toolkit_get_class(
			W_TOOLKIT(gtk_toolkit), _W_CLASS_DROPTARGET);
	if (clazz == 0)
		return W_ERROR_INVALID_SUBCLASS;
	if (clazz->class_id == 0)
		w_toolkit_init_class(W_TOOLKIT(gtk_toolkit), _W_CLASS_DROPTARGET,
				clazz);
	memset(&(droptarget->widget.clazz), 0, clazz->object_used_size);
	droptarget->widget.clazz = clazz;
	_W_DROPTARGET(droptarget)->keyOperation = -1;
	_W_CONTROL(control)->droptarget = W_DROPTARGET(droptarget);
	_W_DROPTARGET(droptarget)->control = W_CONTROL(control);
	_W_WIDGET(droptarget)->post_event = post_event;
	_W_WIDGET(droptarget)->style = style;
	_gtk_signal *signals = gtk_toolkit->signals;
	GtkWidget *handle = _W_WIDGET(control)->handle;
	_W_DROPTARGET(droptarget)->drag_motion_handler = _w_widget_connect(handle,
			&signals[SIGNAL_DRAG_MOTION], FALSE);
	_W_DROPTARGET(droptarget)->drag_leave_handler = _w_widget_connect(handle,
			&signals[SIGNAL_DRAG_LEAVE], FALSE);
	_W_DROPTARGET(droptarget)->drag_data_received_handler = _w_widget_connect(
			handle, &signals[SIGNAL_DRAG_DATA_RECEIVED],
			FALSE);
	_W_DROPTARGET(droptarget)->drag_drop_handler = _w_widget_connect(handle,
			&signals[SIGNAL_DRAG_DROP], FALSE);
	/*_w_widget_connect(handle, &signals[SIGNAL_DRAG_MOTION], FALSE);
	 _w_widget_connect(handle, &signals[SIGNAL_DRAG_LEAVE], FALSE);
	 _w_widget_connect(handle, &signals[SIGNAL_DRAG_DATA_RECEIVED],
	 FALSE);
	 _w_widget_connect(handle, &signals[SIGNAL_DRAG_DROP], FALSE);*/
	return W_TRUE;
}
wresult _w_control_create_widget(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_DRAG_DETECT;
	w_widget *parent = _W_WIDGET(widget)->parent;

	_W_WIDGET_PRIV(priv)->check_orientation(widget, W_WIDGET(parent), priv);
	wresult ret = _w_widget_create_widget(widget, priv);
	if (ret < 0)
		return ret;
	struct _w_widget_class *clazz = _W_WIDGET(widget)->clazz;
	void **layout_data = ((void**) &((char*) widget)[clazz->object_used_size]);
	*layout_data = 0;
	priv->check_background(W_CONTROL(widget), priv);
	priv->check_foreground(W_CONTROL(widget), priv);
	if ((_W_WIDGET(widget)->state & STATE_PARENT_BACKGROUND) != 0)
		priv->set_parent_background(W_CONTROL(widget), priv);
	priv->check_buffered(W_CONTROL(widget), priv);
	if (parent != 0) {
		_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
		_W_COMPOSITE_PRIV(ppriv)->add_child(W_CONTROL(parent), widget, ppriv);
	}
	priv->show_widget(W_CONTROL(widget), priv);
	priv->set_initial_bounds(W_CONTROL(widget), priv);
	priv->set_zorder(W_CONTROL(widget), 0, ZORDER_FIX_CHILDREN, priv);
	priv->set_relations(W_CONTROL(widget), priv);
	priv->check_mirrored(W_CONTROL(widget), priv);
	priv->check_border(W_CONTROL(widget), priv);
	return ret;
}
w_font* _w_control_default_font(w_control *control) {
	return _w_toolkit_get_system_font(W_TOOLKIT(gtk_toolkit));
}
gboolean _w_control_drag_detect_2(w_control *control, int x, int y,
		gboolean filter, gboolean dragOnTimeout, gboolean *consume) {
	gboolean quit = FALSE, dragging = FALSE;

	//428852 DND workaround for GTk3.
	//Gtk3 no longer sends motion events on the same control during thread sleep
	//before a drag started. This is due to underlying gdk changes.
	//Thus for gtk3 we check mouse coords manually
	//Note, input params x/y are relative, the two points below are absolute coords.
	w_point startPos;
	w_point currPos;
#if GTK3
	startPos.x = startPos.y = 0;
	_w_toolkit_get_cursor_location(W_TOOLKIT(gtk_toolkit), &startPos);
#endif
	GtkWidget *handle = _W_WIDGET(control)->handle;
	while (!quit) {
		GdkEvent *eventPtr = 0;
		/*
		 * There should be an event on the queue already, but
		 * in cases where there isn't one, stop trying after
		 * half a second.
		 */
		struct timeval tp;
		gettimeofday(&tp, NULL);
		uint64_t timecurrent = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		uint64_t timeout = timecurrent + 500;
		while (timecurrent < timeout) {
			eventPtr = gdk_event_get();
			if (eventPtr != 0) {
				break;
			} else {
#if GTK3 //428852
				currPos.x = currPos.y = 0;
				_w_toolkit_get_cursor_location(W_TOOLKIT(gtk_toolkit),
						&currPos);
				dragging = gtk_drag_check_threshold(handle, startPos.x,
						startPos.y, currPos.x, currPos.y);
				if (dragging)
					break;
#endif
			}
			gettimeofday(&tp, 0);
			timecurrent = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		}
		if (dragging)
			return TRUE;  //428852
		if (eventPtr == 0)
			return dragOnTimeout;
		switch (eventPtr->type) {
		case GDK_MOTION_NOTIFY: {
			GdkEventMotion *gdkMotionEvent = (GdkEventMotion*) eventPtr;
			if ((gdkMotionEvent->state & GDK_BUTTON1_MASK) != 0) {
				if (gtk_drag_check_threshold(handle, x, y, gdkMotionEvent->x,
						gdkMotionEvent->y)) {
					dragging = TRUE;
					quit = TRUE;
				}
			} else {
				quit = TRUE;
			}
			int newX = 0, newY = 0;
			_gdk_window_get_device_position(gdkMotionEvent->window, &newX,
					&newY, 0);
			break;
		}
		case GDK_KEY_PRESS:
		case GDK_KEY_RELEASE: {
			GdkEventKey *gdkEvent = (GdkEventKey*) eventPtr;
			if (gdkEvent->keyval == GDK_KEY_Escape)
				quit = TRUE;
			break;
		}
		case GDK_BUTTON_RELEASE:
		case GDK_BUTTON_PRESS:
		case GDK_2BUTTON_PRESS:
		case GDK_3BUTTON_PRESS: {
			gdk_event_put(eventPtr);
			quit = TRUE;
			break;
		}
		default:
			gtk_main_do_event(eventPtr);
		}
		gdk_event_free(eventPtr);
	}
	return dragging;
}
wresult _w_control_drag_detect(w_control *control, w_event_mouse *event) {
	return W_FALSE;
}
wresult _w_control_draw_draw_gripper(w_control *control, w_graphics *gc,
		w_rect *rect, int vertical, _w_control_priv *priv) {
	GtkWidget *paintHandle = priv->handle_paint(W_WIDGET(control), priv);
	GdkWindow *window = gtk_widget_get_window(paintHandle);
	if (window == 0)
		return W_FALSE;
	int orientation =
			vertical ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL;
	int x = rect->x;
	if ((_W_WIDGET(control)->style & W_MIRRORED) != 0) {
		int clientWidth = priv->get_client_width(control, priv);
		x = clientWidth - rect->width - x;
	}
#if GTK3
	GtkStyleContext *context = gtk_widget_get_style_context(paintHandle);
	gtk_style_context_save(context);
	gtk_style_context_add_class(context, GTK_STYLE_CLASS_PANE_SEPARATOR);
	gtk_style_context_set_state(context, GTK_STATE_FLAG_NORMAL);
	gtk_render_handle(context, _W_GRAPHICS(gc)->cairo, x, rect->y, rect->width,
			rect->height);
	gtk_style_context_restore(context);
#endif
	return W_TRUE;
}
void _w_control_draw_widget(w_control *control, w_graphics *gc,
		_w_control_priv *priv) {

}
w_control* _w_control_find_background_control(w_control *control,
		_w_control_priv *priv) {
	return 0;
}
gboolean _w_control_force_focus_0(w_control *control, GtkWidget *focusHandle,
		_w_control_priv *priv) {
	if (gtk_widget_has_focus(focusHandle))
		return TRUE;
	gtk_widget_set_can_focus(focusHandle, TRUE);
	/* When the control is zero sized it must be realized */
	gtk_widget_realize(focusHandle);
	gtk_widget_grab_focus(focusHandle);
// widget could be disposed at this point
	if (!w_widget_is_ok(W_WIDGET(control)))
		return FALSE;
	w_shell *shell;
	w_widget_get_shell(W_WIDGET(control), &shell);
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	GtkWidget *handle = gtk_window_get_focus(GTK_WINDOW(shellHandle));
	while (handle != 0) {
		if (handle == focusHandle) {
			/* Cancel any previous ignoreFocus requests */
			gtk_toolkit->ignoreFocus = FALSE;
			return TRUE;
		}
		w_widget *widget = g_object_get_qdata(G_OBJECT(handle),
				gtk_toolkit->quark[0]);
		if (widget != 0 && w_widget_class_id(widget) >= _W_CLASS_CONTROL) {
			return widget == (w_widget*) control;
		}
		handle = gtk_widget_get_parent(handle);
	}
	return FALSE;
}
wresult _w_control_force_focus(w_control *control) {
	if (gtk_toolkit->focusEvent == W_EVENT_FOCUSOUT)
		return FALSE;
	w_shell *shell;
	w_widget_get_shell(W_WIDGET(control), &shell);
	_w_shell_set_saved_focus(shell, control);
	struct _w_control_class *clazz = W_CONTROL_GET_CLASS(control);
	if (clazz->is_enabled(control) <= 0 || clazz->is_visible(control) <= 0)
		return W_FALSE;
	_w_shell_bring_totop(shell, W_FALSE);
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	GtkWidget *focusHandle = priv->handle_focus(W_WIDGET(control), priv);
	return _w_control_force_focus_0(control, focusHandle, priv);
}
void _w_control_force_resize(w_control *control, _w_control_priv *priv) {
	/*
	 * Force size allocation on all children of this widget's
	 * topHandle.  Note that all calls to gtk_widget_size_allocate()
	 * must be preceded by a call to gtk_widget_size_request().
	 */
	GtkWidget *topHandle = priv->widget.handle_top(W_WIDGET(control), priv);
	GtkRequisition requisition;
	gtk_widget_size_request(topHandle, &requisition);
	GtkAllocation allocation;
	gtk_widget_get_allocation(topHandle, &allocation);
	gtk_widget_size_allocate(topHandle, &allocation);
}
wresult _w_control_get_accessible(w_control *control,
		w_accessible **accessible) {
	return W_FALSE;
}
wresult _w_control_get_background(w_control *control, w_color *background) {
	*background = W_COLOR_WHITE;
	return W_TRUE;
}
wresult _w_control_get_background_image(w_control *control, w_image *image) {
	return W_FALSE;
}
wresult _w_control_get_border_width(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_bounds(w_control *control, w_point *location,
		w_size *size) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	GtkWidget *topHandle = priv->widget.handle_top(W_WIDGET(control), priv);
	GtkAllocation allocation;
	gtk_widget_get_allocation(topHandle, &allocation);
	int width =
			(_W_WIDGET(control)->state & STATE_ZERO_WIDTH) != 0 ?
					0 : allocation.width;
	if (size != 0) {
		size->width = width;
		size->height =
				(_W_WIDGET(control)->state & STATE_ZERO_HEIGHT) != 0 ?
						0 : allocation.height;
	}
	if (location != 0) {
		location->x = allocation.x;
		location->y = allocation.y;
		w_control *parent = W_CONTROL(_W_WIDGET(control)->parent);
		if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
			_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
			location->x = ppriv->get_client_width(parent, ppriv) - width
					- location->x;
		}
	}
	return W_TRUE;
}
wresult _w_control_get_imcaret_pos(w_control *control, w_point *pos,
		_w_control_priv *priv) {
	pos->x = 0;
	pos->y = 0;
	return TRUE;
}
wresult _w_control_get_cursor(w_control *control, w_cursor **cursor) {
	return W_FALSE;
}
int _w_control_get_client_width(w_control *control, _w_control_priv *priv) {
	return 0;
}
wresult _w_control_get_drag_detect(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_enabled(w_control *control) {
	return (_W_WIDGET(control)->state & STATE_DISABLED) == 0;
}
wresult _w_control_get_font(w_control *control, w_font **font) {
	if (_W_CONTROL(control)->font != 0) {
		*font = (w_font*) _W_CONTROL(control)->font;
	} else {
		w_theme *theme;
		w_widget_get_theme(W_WIDGET(control), &theme);
		w_theme_get_font(theme, font);
	}
	return W_TRUE;
}
wresult _w_control_get_foreground(w_control *control, w_color *foreground) {
	*foreground = W_COLOR_BLACK;
	return W_TRUE;
}
wresult _w_control_get_graphics(w_control *control, w_graphics *gc) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	GdkWindow *window = priv->window_paint(W_WIDGET(control), priv);
	if (window == 0)
		return W_ERROR_NO_HANDLES;

#if USE_CAIRO
	cairo_t *cr;
	cr = gdk_cairo_create(window);
	_w_graphics_init(gc, cr);
	w_font *font;
	w_control_get_font(W_CONTROL(control), &font);
	w_graphics_set_font(gc, font);
	w_color background;
	w_control_get_background(W_CONTROL(control), &background);
	w_graphics_set_background(gc, background);
	w_color foreground;
	w_control_get_foreground(W_CONTROL(control), &foreground);
	w_graphics_set_foreground(gc, foreground);
#else
		cr = gdk_gc_new (window);
#endif
	return W_TRUE;
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
	*menu = _W_CONTROL(control)->menu;
	return W_TRUE;
}
wresult _w_control_get_orientation(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_region(w_control *control, w_region *region) {
	return W_FALSE;
}
wresult _w_control_get_shell(w_widget *control, w_shell **shell) {
	w_widget *p = _W_WIDGET(control)->parent;
	return w_widget_get_shell(W_WIDGET(p), shell);
}
wresult _w_control_get_tab(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_text_direction(w_control *control) {
	return W_FALSE;
}
void _w_control_get_thickness(GtkWidget *widget, w_point *thickness) {
#if GTK3
	thickness->x = 0, thickness->y = 0;
	GtkBorder tmp;
	GtkStyleContext *context = gtk_widget_get_style_context(widget);

	if (GTK_VERSION < VERSION(3, 18, 0)) {
		gtk_style_context_get_padding(context, GTK_STATE_FLAG_NORMAL, &tmp);
	} else {
		gtk_style_context_get_padding(context,
				gtk_widget_get_state_flags(widget), &tmp);
	}
	gtk_style_context_save(context);
	gtk_style_context_add_class(context, GTK_STYLE_CLASS_FRAME);
	thickness->x += tmp.left;
	thickness->y += tmp.top;
	if (GTK_VERSION < VERSION(3, 18, 0)) {
		gtk_style_context_get_border(context, GTK_STATE_FLAG_NORMAL, &tmp);
	} else {
		gtk_style_context_get_border(context,
				gtk_widget_get_state_flags(widget), &tmp);
	}
	thickness->x += tmp.left;
	thickness->y += tmp.top;
	gtk_style_context_restore(context);
	return;
#endif
}
wresult _w_control_get_tooltip_text(w_control *control, w_alloc alloc,
		void *user_data, int enc) {
	return _gtk_alloc_set_text(alloc, user_data,
	_W_CONTROL(control)->tooltiptext, -1, enc);
}
wresult _w_control_get_touch_enabled(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_visible(w_control *control) {
	return (_W_WIDGET(control)->state & STATE_HIDDEN) == 0;
}
wresult _w_control_init_themedata(w_widget *widget, w_themedata *data) {
	_w_widget_init_themedata(widget, data);
	data->attr.font = (w_font*) _W_CONTROL(widget)->font;
	data->attr.background = _W_CONTROL(widget)->background;
	data->attr.foreground = _W_CONTROL(widget)->foreground;
	return W_TRUE;
}
GtkWidget* _w_control_handle_enterexit(w_widget *control,
		_w_control_priv *priv) {
	return _W_WIDGET(control)->handle;
}
GtkWidget* _w_control_handle_top(w_widget *control, _w_control_priv *priv) {
	return _W_WIDGET(control)->handle;
}
GtkWidget* _w_control_handle_paint(w_widget *control, _w_control_priv *priv) {
	return _W_WIDGET(control)->handle;
}
GtkIMContext* _w_control_handle_im(w_widget *control, _w_control_priv *priv) {
	return 0;
}
wresult _w_control_has_focus(w_control *control, _w_control_priv *priv) {
	return control == _w_toolkit_get_focus_control(W_TOOLKIT(gtk_toolkit));
}
void _w_control_hook_events(w_widget *widget, _w_control_priv *priv) {
	_gtk_signal *signals = gtk_toolkit->signals;
	/* Connect the keyboard signals */
	GtkWidget *focusHandle = priv->handle_focus(widget, priv);
	int focusMask = GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK
			| GDK_FOCUS_CHANGE_MASK;
	gtk_widget_add_events(focusHandle, focusMask);
	_w_widget_connect(focusHandle, &signals[SIGNAL_POPUP_MENU], FALSE);
	_w_widget_connect(focusHandle, &signals[SIGNAL_SHOW_HELP], FALSE);
	_w_widget_connect(focusHandle, &signals[SIGNAL_KEY_PRESS_EVENT], FALSE);
	_w_widget_connect(focusHandle, &signals[SIGNAL_KEY_RELEASE_EVENT], FALSE);
	_w_widget_connect(focusHandle, &signals[SIGNAL_FOCUS], FALSE);
	_w_widget_connect(focusHandle, &signals[SIGNAL_FOCUS_IN_EVENT], FALSE);
	_w_widget_connect(focusHandle, &signals[SIGNAL_FOCUS_OUT_EVENT], FALSE);
	/* Connect the mouse signals */
	GtkWidget *eventHandle = priv->handle_event(widget, priv);
	int eventMask = GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK | GDK_SCROLL_MASK
			| GDK_SMOOTH_SCROLL_MASK;
	gtk_widget_add_events(eventHandle, eventMask);
	_w_widget_connect(eventHandle, &signals[SIGNAL_BUTTON_PRESS_EVENT], FALSE);
	_w_widget_connect(eventHandle, &signals[SIGNAL_BUTTON_RELEASE_EVENT],
	FALSE);
	_w_widget_connect(eventHandle, &signals[SIGNAL_MOTION_NOTIFY_EVENT], FALSE);
	_w_widget_connect(eventHandle, &signals[SIGNAL_SCROLL_EVENT], FALSE);

	/* Connect enter/exit signals */
	GtkWidget *enterExitHandle = priv->handle_enterexit(widget, priv);
	int enterExitMask = GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK;
	gtk_widget_add_events(enterExitHandle, enterExitMask);
	_w_widget_connect(enterExitHandle, &signals[SIGNAL_ENTER_NOTIFY_EVENT],
	FALSE);
	_w_widget_connect(enterExitHandle, &signals[SIGNAL_LEAVE_NOTIFY_EVENT],
	FALSE);
	/*
	 * Feature in GTK.  Events such as mouse move are propagate up
	 * the widget hierarchy and are seen by the parent.  This is the
	 * correct GTK behavior but not correct for SWT.  The fix is to
	 * hook a signal after and stop the propagation using a negative
	 * event number to distinguish this case.
	 *
	 * The signal is hooked to the fixedHandle to catch events sent to
	 * lightweight widgets.
	 */
	GtkWidget *fixedHandle = priv->handle_fixed(widget, priv);
	GtkWidget *blockHandle = fixedHandle != 0 ? fixedHandle : eventHandle;
	if (blockHandle != eventHandle) {
		_w_widget_connect(blockHandle, &signals[SIGNAL_BUTTON_PRESS_EVENT],
		TRUE);
		_w_widget_connect(blockHandle, &signals[SIGNAL_BUTTON_RELEASE_EVENT],
		TRUE);
		_w_widget_connect(blockHandle, &signals[SIGNAL_MOTION_NOTIFY_EVENT],
		TRUE);
	}
	/* Connect the event_after signal for both key and mouse */
	_w_widget_connect(eventHandle, &signals[SIGNAL_EVENT_AFTER], FALSE);
	if (focusHandle != eventHandle) {
		_w_widget_connect(focusHandle, &signals[SIGNAL_EVENT_AFTER], FALSE);
	}

	/* Connect the paint signal */
	GtkWidget *paintHandle = priv->handle_paint(widget, priv);
	int paintMask = GDK_EXPOSURE_MASK;
	gtk_widget_add_events(paintHandle, paintMask);
	_w_widget_connect(paintHandle, &signals[SIGNAL_EXPOSE_EVENT_INVERSE],
	FALSE);
	_w_widget_connect(paintHandle, &signals[SIGNAL_EXPOSE_EVENT], TRUE);
	/* Connect the Input Method signals */
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	_w_widget_connect(handle, &signals[SIGNAL_REALIZE], TRUE);
	_w_widget_connect(handle, &signals[SIGNAL_UNREALIZE], FALSE);
	GtkWidget *imHandle = (GtkWidget*) priv->handle_im(widget, priv);
	if (imHandle != 0) {
		_w_widget_connect(imHandle, &signals[SIGNAL_COMMIT], FALSE);
		_w_widget_connect(imHandle, &signals[SIGNAL_PREEDIT_CHANGED], FALSE);
	}
	_w_widget_connect(paintHandle, &signals[SIGNAL_STYLE_SET], FALSE);
	GtkWidget *topHandle = _W_WIDGET_PRIV(priv)->handle_top(widget, priv);
	_w_widget_connect(topHandle, &signals[SIGNAL_MAP], TRUE);
	_w_widget_connect(topHandle, &signals[SIGNAL_DESTROY], TRUE);
#if GTK3
	if (enterNotifyEventSignalId == 0 && GTK_VERSION < VERSION(3, 11, 9)) {
		enterNotifyEventSignalId = g_signal_lookup("enter-notify-event",
		GTK_TYPE_WIDGET);
		GTK_POINTER_WINDOW = g_quark_from_string("gtk-pointer-window");
		SWT_GRAB_WIDGET = g_quark_from_string("swt-grab-widget");
	}
#endif
}
wresult _w_control_is_enabled(w_control *control) {
	if (W_CONTROL_GET_CLASS(control)->get_enabled(control) > 0) {
		w_widget *parent = _W_WIDGET(control)->parent;
		return W_CONTROL_GET_CLASS(parent)->is_enabled(W_CONTROL(parent));
	}
	return W_FALSE;
}
wresult _w_control_is_focus_control(w_control *control) {
	return W_FALSE;
}
gboolean _w_control_is_focus_handle(w_control *control, GtkWidget *widget,
		_w_control_priv *priv) {
	return widget == priv->handle_focus(W_WIDGET(control), priv);
}
wresult _w_control_is_reparentable(w_control *control) {
	return W_FALSE;
}
wresult _w_control_is_visible(w_control *control) {
	if (W_CONTROL_GET_CLASS(control)->get_visible(control) > 0) {
		w_widget *parent = _W_WIDGET(control)->parent;
		return W_CONTROL_GET_CLASS(parent)->is_visible(W_CONTROL(parent));
	}
	return W_FALSE;
}
wuint* _w_control_find_timer(w_control *control, wushort id,
		wuint **firstFree) {
	if (id == 0)
		return W_FALSE;
	_w_control_timer *timer = &_W_CONTROL(control)->timer;
	wuint *_timer = 0, _t;
	int remove_timer = W_FALSE;
	for (int i = 0; i < sizeof(timer->id) / sizeof(timer->id[0]); i++) {
		_t = timer->id[i];
		if ((_t & _TIMER_ID_MASK) == id) {
			return &timer->id[i];
		}
		if (_timer == 0 && (_t & _TIMER_ID_MASK) == 0) {
			_t |= ((i & _TIMER_INDEX_MASK) << 16);
			_t &= ~ _TIMER_ARRAY;
			_timer = &timer->id[i];
			*_timer = _t;
		}
	}
	const int start_index = sizeof(void*) / sizeof(wuint);
	w_array *array = timer->ids;
	if (array != 0) {
		wuint *_arr_time;
		int count = w_array_get_count(array, (void**) &_arr_time);
		for (int i = start_index; i < count; i++) {
			_t = _arr_time[i];
			if ((_t & _TIMER_ID_MASK) == id) {
				return &_arr_time[i];
			}
			if (_timer == 0 && (_t & _TIMER_ID_MASK) == 0) {
				_t |= ((i & _TIMER_INDEX_MASK) << 16) | _TIMER_ARRAY;
				_timer = &_arr_time[i];
				*_timer = _t;
			}
		}
	}
	if (firstFree != 0) {
		*firstFree = _timer;
	}
	return 0;
}
void _w_control_kill_all_timer(w_control *control) {
	_w_control_timer *timer = &_W_CONTROL(control)->timer;
	wuint _t;
	int remove_timer = W_FALSE;
	for (int i = 0; i < sizeof(timer->id) / sizeof(timer->id[0]); i++) {
		_t = timer->id[i];
		if ((_t & _TIMER_ID_MASK) != 0) {
			g_source_remove_by_user_data(&timer->id[i]);
		}
	}
	const int start_index = sizeof(void*) / sizeof(wuint);
	w_array *array = timer->ids;
	if (array != 0) {
		wuint *_arr_time;
		int count = w_array_get_count(array, (void**) &_arr_time);
		for (int i = start_index; i < count; i++) {
			_t = _arr_time[i];
			if ((_t & _TIMER_ID_MASK) != 0) {
				g_source_remove_by_user_data(&_arr_time[i]);
			}
		}
		w_array_free(array, sizeof(wuint), 0);
	}
}
wresult _w_control_kill_timer(w_control *control, wushort id) {
	wuint *_t = _w_control_find_timer(control, id, 0);
	if (_t != 0) {
		*_t = 0;
		g_source_remove_by_user_data(_t);
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
void _w_control_mark_layout(w_control *control, int flags,
		_w_control_priv *priv) {

}
wresult _w_control_move_above(w_control *control, w_control *_control) {
	return W_FALSE;
}
wresult _w_control_move_below(w_control *control, w_control *_control) {
	return W_FALSE;
}
void _w_control_move_children(w_control *control, int oldWidth,
		_w_control_priv *priv) {

}
void _w_control_move_handle(w_control *control, w_point *position,
		_w_control_priv *priv) {
	GtkWidget *topHandle = _W_WIDGET_PRIV(priv)->handle_top(W_WIDGET(control),
			priv);
	_w_fixed_move(topHandle, position->x, position->y);
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
	w_size size;
	w_control_compute_size_0(control, &size, W_DEFAULT, W_DEFAULT, flags);
	return w_control_set_bounds(control, 0, &size);
}
wresult _w_control_print(w_control *control, w_graphics *gc) {
	return W_FALSE;
}
wresult _w_control_request_layout(w_control *control) {
	return W_FALSE;
}
wresult _w_control_redraw(w_control *control, w_rect *r, int all) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	GtkWidget *topHandle = priv->widget.handle_top(W_WIDGET(control), priv);
	if (!gtk_widget_get_visible(topHandle))
		return W_FALSE;
	int flags;
	if (all) {
		flags = REDRAW_ALL;
	} else
		flags = 0;
	if ((_W_WIDGET(control)->style & W_MIRRORED) != 0 && r != 0) {
		w_rect rect;
		memcpy(&rect, r, sizeof(w_rect));
		rect.x = priv->get_client_width(control, priv) - rect.width - rect.x;
		r = &rect;
	}
	priv->redraw_widget(control, r, flags, priv);
	return W_TRUE;
}
void _w_control_redraw_widget(w_control *control, w_rect *_rect, int flags,
		_w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(control)->handle;
	if (!gtk_widget_get_realized(handle))
		return;
	GdkWindow *window = priv->window_paint(W_WIDGET(control), priv);
	GdkRectangle rect, *r = &rect;
	if (flags & REDRAW_REDRAWALL) {
		rect.width = gdk_window_get_width(window);
		rect.height = gdk_window_get_height(window);
	} else {
		if (_rect != 0) {
			rect.x = _rect->x;
			rect.y = _rect->y;
			rect.width = WMAX(0, _rect->width);
			rect.height = WMAX(0, _rect->height);
		} else {
			r = 0;
		}
	}
	gdk_window_invalidate_rect(window, r, flags & REDRAW_ALL);
}
void _w_control_resize_handle(w_control *control, w_size *size,
		_w_control_priv *priv) {
	GtkWidget *topHandle = _W_WIDGET_PRIV(priv)->handle_top(W_WIDGET(control),
			priv);
	_w_fixed_resize(topHandle, size->width, size->height);
}
wresult _w_control_send_leave_notify(w_control *control,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_set_background(w_control *control, w_color color) {
	return W_FALSE;
}
wresult _w_control_set_background_pixmap(w_control *control, GdkPixbuf *image,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_set_background_image(w_control *control, w_image *image) {
	return W_FALSE;
}
wresult _w_control_set_bounds(w_control *control, w_point *location,
		w_size *size) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	wresult result = priv->set_bounds_0(control, location, size, priv);
	return result;
}
wresult _w_control_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_priv *priv) {
	wresult result = 0;
	w_point _location;
	w_size _size;
	GtkWidget *topHandle = _W_WIDGET_PRIV(priv)->handle_top(W_WIDGET(control),
			priv);
	GdkWindow *redrawWindow = priv->window_redraw(W_WIDGET(control), priv);
	GdkWindow *enableWindow = priv->window_enable(W_WIDGET(control), priv);
	int sendMove = location != 0;
	w_widget *parent = _W_WIDGET(control)->parent;
	GtkAllocation allocation;
	gtk_widget_get_allocation(topHandle, &allocation);
	if (location != 0) {
		memcpy(&_location, location, sizeof(w_point));
	} else
		memset(&_location, 0, sizeof(w_point));
	if (size != 0) {
		memcpy(&_size, size, sizeof(w_size));
	} else
		memset(&_size, 0, sizeof(w_size));
	if (parent != 0 && (_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
		_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
		int clientWidth = ppriv->get_client_width(W_CONTROL(parent), ppriv);
		int oldWidth =
				(_W_WIDGET(control)->state & STATE_ZERO_WIDTH) != 0 ?
						0 : allocation.width;
		int oldX = clientWidth - oldWidth - allocation.x;
		if (location != 0) {
			sendMove &= location->x != oldX;
			_location.x = clientWidth - (size != 0 ? size->width : oldWidth)
					- location->x;
		} else {
			location = (w_point*) control; // just for update location
			_location.x = clientWidth - (size != 0 ? size->width : oldWidth)
					- oldX;
			_location.y = allocation.y;
		}
	}
	int sameOrigin = W_TRUE, sameExtent = W_TRUE;
	if (location != 0) {
		int oldX = allocation.x;
		int oldY = allocation.y;
		sameOrigin = _location.x == oldX && _location.y == oldY;
		if (!sameOrigin) {
			if (enableWindow != 0) {
				gdk_window_move(enableWindow, _location.x, _location.y);
			}
			priv->move_handle(control, &_location, priv);
		}
	}
	int clientWidth = 0;
	if (size != 0) {
		int oldWidth =
				(_W_WIDGET(control)->state & STATE_ZERO_WIDTH) != 0 ?
						0 : allocation.width;
		int oldHeight =
				(_W_WIDGET(control)->state & STATE_ZERO_HEIGHT) != 0 ?
						0 : allocation.height;
		sameExtent = _size.width == oldWidth && _size.height == oldHeight;
		if (!sameExtent && (_W_WIDGET(control)->style & W_MIRRORED) != 0)
			clientWidth = priv->get_client_width(control, priv);
		if (!sameExtent && !(_size.width == 0 && _size.height == 0)) {
			w_size new_sz;
			new_sz.width = MAX(1, _size.width);
			new_sz.height = MAX(1, _size.height);
			if (redrawWindow != 0) {
				gdk_window_resize(redrawWindow, new_sz.width, new_sz.height);
			}
			if (enableWindow != 0) {
				gdk_window_resize(enableWindow, new_sz.width, new_sz.height);
			}
			priv->resize_handle(control, &new_sz, priv);
		}
	}
	if (!sameOrigin || !sameExtent) {
		/*
		 * Cause a size allocation this widget's topHandle.  Note that
		 * all calls to gtk_widget_size_allocate() must be preceded by
		 * a call to gtk_widget_size_request().
		 */
		GtkRequisition requisition;
#if GTK3
		gtk_widget_get_preferred_size(topHandle, &requisition, NULL);
#endif
#if GTK2
		gtk_widget_size_request(topHandle, &requisition);
#endif
		GtkAllocation _allocation;
		gtk_widget_get_allocation(topHandle, &_allocation);
		if (location != 0) {
			_allocation.x = _location.x;
			_allocation.y = _location.y;
		}
		if (size != 0) {
			_allocation.width = _size.width;
			_allocation.height = _size.height;
		}
		GtkWidget *handle = _W_WIDGET(control)->handle;
		if (GTK_VERSION >= VERSION(3, 8, 0)
				&& !gtk_widget_get_visible(handle)) {
			gtk_widget_show(handle);
#if GTK3
			gtk_widget_get_preferred_size(topHandle, &requisition, NULL);
#endif
#if GTK2
		gtk_widget_size_request(topHandle, &requisition);
#endif
			gtk_widget_size_allocate(topHandle, &_allocation);
			gtk_widget_hide(handle);
		} else {
			gtk_widget_size_allocate(topHandle, &_allocation);
		}
	}
	/*
	 * Bug in GTK.  Widgets cannot be sized smaller than 1x1.
	 * The fix is to hide zero-sized widgets and show them again
	 * when they are resized larger.
	 */
	if (!sameExtent) {

		_W_WIDGET(control)->state =
				(_size.width == 0) ?
						_W_WIDGET(control)->state | STATE_ZERO_WIDTH :
						_W_WIDGET(control)->state & ~STATE_ZERO_WIDTH;
		_W_WIDGET(control)->state =
				(_size.height == 0) ?
						_W_WIDGET(control)->state | STATE_ZERO_HEIGHT :
						_W_WIDGET(control)->state & ~STATE_ZERO_HEIGHT;
		if ((_W_WIDGET(control)->state & (STATE_ZERO_WIDTH | STATE_ZERO_HEIGHT))
				!= 0) {
			if (enableWindow != 0) {
				gdk_window_hide(enableWindow);
			}
			gtk_widget_hide(topHandle);
		} else {
			if ((_W_WIDGET(control)->state & STATE_HIDDEN) == 0) {
				if (enableWindow != 0) {
					gdk_window_show_unraised(enableWindow);
				}
				gtk_widget_show(topHandle);
			}
		}

		if ((_W_WIDGET(control)->style & W_MIRRORED) != 0)
			priv->move_children(control, clientWidth, priv);
	}
	if (location != 0 && !sameOrigin) {
		w_control *_control = priv->find_background_control(control, priv);
		if (_control != 0
				&& _W_CONTROL(_control)->backgroundImage.pixbuf != 0) {
			if (W_CONTROL_GET_CLASS(control)->is_visible(control) > 0) {
				w_rect tmp;
				memset(&tmp, 0, sizeof(w_rect));
				priv->redraw_widget(control, &tmp,
				REDRAW_ALL | REDRAW_REDRAWALL | REDRAW_TRIM, priv);
			}
		}
		if (sendMove) {
			w_event e;
			e.type = W_EVENT_MOVE;
			e.platform_event = 0;
			e.data = 0;
			e.widget = W_WIDGET(control);
			_w_widget_send_event(W_WIDGET(control), &e, W_EVENT_SEND);
		}
		result |= 1;
	}
	if (size != 0 && !sameExtent) {
		w_event e;
		e.type = W_EVENT_RESIZE;
		e.platform_event = 0;
		e.data = 0;
		e.widget = W_WIDGET(control);
		_w_widget_send_event(W_WIDGET(control), &e, W_EVENT_SEND);
		result |= 2;
	}
	return result;
}
wresult _w_control_set_capture(w_control *control, int capture) {
	return W_FALSE;
}
wresult _w_control_set_cursor_0(w_control *control, GdkCursor *cursor,
		_w_control_priv *priv) {
	GdkWindow *window = priv->window_event(W_WIDGET(control), priv);
	if (window != 0) {
		gdk_window_set_cursor(window, cursor);
		gdk_flush();
	}
	return W_TRUE;
}
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor) {
	if (cursor != 0 && _W_CURSOR(cursor)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	_W_CONTROL(control)->cursor = cursor;
	GdkCursor *hcursor = 0;
	if (cursor != 0) {
		hcursor = _W_CURSOR(cursor)->handle;
	}
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	return priv->set_cursor_0(control, hcursor, priv);
}
wresult _w_control_set_drag_detect(w_control *control, int dragDetect) {
	return W_FALSE;
}
wresult _w_control_set_enabled(w_control *control, int enabled) {
	return W_FALSE;
}
wresult _w_control_set_focus(w_control *control) {
	if ((_W_WIDGET(control)->style & W_NO_FOCUS) != 0)
		return W_FALSE;
	return _w_control_force_focus(control);
}
wresult _w_control_set_font(w_control *control, w_font *font) {
	_W_CONTROL(control)->font = (PangoFontDescription*) font;
	PangoFontDescription *fontDesc;
	if (font == 0) {
		fontDesc = (PangoFontDescription*) _w_control_default_font(control);
	} else {
		fontDesc = (PangoFontDescription*) font;
	}
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	priv->set_font_description(control, fontDesc, priv);
	return W_TRUE;
}
void _w_control_set_font_description(w_control *control,
		PangoFontDescription *font, _w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(control)->handle;
	_w_widget_set_font_description(W_WIDGET(control), handle, font, priv);
}
wresult _w_control_set_foreground(w_control *control, w_color color) {
	return W_FALSE;
}
void _w_control_set_initial_bounds(w_control *control, _w_control_priv *priv) {
}
wresult _w_control_set_menu(w_control *control, w_menu *menu) {
	_W_CONTROL(control)->menu = 0;
	if (w_widget_is_ok(W_WIDGET(menu))) {
		if ((_W_WIDGET(menu)->style & W_POP_UP) == 0) {
			return W_ERROR_MENU_NOT_POP_UP;
		}
		w_shell *shell1;
		w_widget_get_shell(W_WIDGET(menu), &shell1);
		w_shell *shell2;
		w_widget_get_shell(W_WIDGET(control), &shell2);
		if (shell1 != shell2) {
			return W_ERROR_INVALID_PARENT;
		}
		_W_CONTROL(control)->menu = menu;
		return W_TRUE;
	}
	return W_ERROR_INVALID_ARGUMENT;
}
wresult _w_control_set_orientation(w_control *control, int orientation) {
	return W_FALSE;
}
wresult _w_control_set_parent(w_control *control, w_composite *parent) {
	return W_FALSE;
}
void _w_control_set_parent_background(w_control *control,
		_w_control_priv *priv) {
}
wresult _w_control_set_redraw(w_control *control, int redraw) {
	return W_FALSE;
}
wresult _w_control_set_region(w_control *control, w_region *region) {
	if (region != 0 && w_region_is_ok(region) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	GtkWidget *topHandle = _W_WIDGET_PRIV(priv)->handle_top(W_WIDGET(control),
			priv);
	GdkWindow *window = gtk_widget_get_window(topHandle);
	cairo_region_t *shape_region =
			(region == 0) ? 0 : _W_REGION(region)->handle;
	gdk_window_shape_combine_region(window, shape_region, 0, 0);
	return W_TRUE;
}
void _w_control_set_relations(w_control *control, _w_control_priv *priv) {
}
wresult _w_control_set_tab(w_control *control, int tab) {
	return W_FALSE;
}
wresult _w_control_set_text_direction(w_control *control, int textDirection) {
	return W_FALSE;
}
gboolean _w_timer_listenner(gpointer user_data) {
	wuint _id = *((wuint*) user_data);
	w_control *control;
	if (_id & _TIMER_ARRAY) {
		control = *((w_control**) (((char*) user_data)
				- (((_id >> 16) & _TIMER_INDEX_MASK) * sizeof(wuint))));
	} else {
		control = (w_control*) (((char*) user_data)
				- (((_id >> 16) & _TIMER_INDEX_MASK) * sizeof(wuint))
				+ ((char*) user_data
						- (char*) &((_w_control*) (user_data))->timer));
	}
	w_event_time e;
	e.event.type = W_EVENT_TIMER;
	e.event.platform_event = 0;
	e.event.time = 0;
	e.event.widget = W_WIDGET(control);
	e.event.data = 0;
	_w_widget_send_event(W_WIDGET(control), (w_event*) &e, W_EVENT_SEND);
	return TRUE;
}
wresult _w_control_set_timer(w_control *control, wint64 ms, wushort id) {
	if (id == 0)
		return W_FALSE;
	_w_control_timer *timer = &_W_CONTROL(control)->timer;
	wuint *_timer = 0, *_t;
	_timer = _w_control_find_timer(control, id, &_t);
	if (_timer != 0) {
		g_source_remove_by_user_data(_timer);
	} else {
		_timer = _t;
	}
	if (_timer == 0) {
		int _index;
		if (timer->ids == 0) {
			_index = sizeof(void*) / sizeof(wuint);
			w_array_set(&timer->ids, _index, sizeof(wuint));
			if (timer->ids == 0)
				return W_FALSE;
			*((w_control**) timer->ids->data) = control;
			_timer = (wuint*) w_array_get(timer->ids, _index, sizeof(wuint));
		} else {
			_timer = w_array_add(&timer->ids, -1, sizeof(wuint), &_index);
			if (_timer == 0)
				return W_FALSE;
		}
		(*_timer) = ((_index & _TIMER_INDEX_MASK) << 16) | _TIMER_ARRAY;
	}
	(*_timer) |= id;
	g_timeout_add(ms, _w_timer_listenner, _timer);
	return W_TRUE;
}
wresult _w_control_set_tooltip_text(w_control *control, const char *text,
		int length, int enc) {
	return W_FALSE;
}
wresult _w_control_set_touch_enabled(w_control *control, int enabled) {
	return W_FALSE;
}
wresult _w_control_set_visible(w_control *control, int visible) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	GtkWidget *topHandle = _W_WIDGET_PRIV(priv)->handle_top(W_WIDGET(control),
			priv);
	if (visible) {
		gtk_widget_show(topHandle);
	} else {
		gtk_widget_hide(topHandle);
	}
	return W_TRUE;
}
wresult _w_control_set_zorder(w_control *control, w_control *sibling, int flags,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _w_control_show_menu(w_control *control, int x, int y, int detail) {
	w_event_menu_detect event;
	/*if (gdk_pointer_is_grabbed()) {
	 gdk_pointer_ungrab(0);
	 }*/
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_MENUDETECT;
	event.event.widget = W_WIDGET(control);
	event.location.x = x;
	event.location.y = y;
	event.detail = detail;
	int ret = _w_widget_send_event(W_WIDGET(control), (w_event*) &event,
			W_EVENT_SEND);
//widget could be disposed at this point
	if (!w_widget_is_ok(W_WIDGET(control)))
		return FALSE;
	if (ret && w_widget_is_ok(W_WIDGET(event.menu))) {
		/*boolean hooksKeys = hooks(SWT.KeyDown) || hooks(SWT.KeyUp);
		 menu.createIMMenu(hooksKeys ? imHandle() : 0);*/
		//if (event.location.x != x || event.location.y != y) {
		w_menu_set_location(event.menu, &event.location);
		//}
		w_menu_set_visible(event.menu, W_TRUE);
		return TRUE;
	}
	return FALSE;
}
void _w_control_show_widget(w_control *control, _w_control_priv *priv) {
	_W_WIDGET(control)->state |= STATE_ZERO_WIDTH | STATE_ZERO_HEIGHT;
	GtkWidget *topHandle = _W_WIDGET_PRIV(priv)->handle_top(W_WIDGET(control),
			priv);
	GtkWidget *fixedHandle = priv->handle_fixed(W_WIDGET(control), priv);
	GtkWidget *handle = _W_WIDGET(control)->handle;
	if (handle != 0 && handle != topHandle)
		gtk_widget_show(handle);
	if ((_W_WIDGET(control)->state & (STATE_ZERO_WIDTH | STATE_ZERO_HEIGHT))
			== 0) {
		if (fixedHandle != 0)
			gtk_widget_show(fixedHandle);
	}
}
wresult _w_control_to_control(w_control *control, w_point *result,
		w_point *point) {
	return W_FALSE;
}
wresult _w_control_to_display(w_control *control, w_point *result,
		w_point *point) {
	return W_FALSE;
}
gboolean _w_control_translate_mnemonic_0(w_control *_this, int keyval,
		GdkEventKey *gdkEvent, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_translate_traversal(w_control *control,
		GdkEventKey *keyEvent, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_traverse(w_control *control, int traversal,
		w_event_key *event) {
	return W_FALSE;
}
wresult _w_control_update_0(w_control *control, int flags,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_control_update(w_control *control) {
	return W_FALSE;
}
void _w_control_update_layout(w_control *control, int flags,
		_w_control_priv *priv) {

}
GdkWindow* _w_control_window_enable(w_widget *control, _w_control_priv *priv) {
	return 0;
}
GdkWindow* _w_control_window_event(w_widget *control, _w_control_priv *priv) {
	GtkWidget *eventHandle = priv->handle_event(control, priv);
	gtk_widget_realize(eventHandle);
	return gtk_widget_get_window(eventHandle);
}
GdkWindow* _w_control_window_paint(w_widget *control, _w_control_priv *priv) {
	GtkWidget *paintHandle = priv->handle_paint(control, priv);
	gtk_widget_realize(paintHandle);
	return gtk_widget_get_window(paintHandle);
}
GdkWindow* _w_control_window_redraw(w_widget *control, _w_control_priv *priv) {
	return 0;
}
wresult _w_control_dispose_class(struct _w_widget_class *clazz) {
	_w_widget_dispose_class(clazz);
	return W_TRUE;
}
void _w_control_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_control_class *clazz) {
	_w_widget_class_init(toolkit, classId, W_WIDGET_CLASS(clazz));
	/*
	 * functions
	 */
	W_WIDGET_CLASS(clazz)->dispose_class = _w_control_dispose_class;
	W_WIDGET_CLASS(clazz)->create = _w_control_create;
	W_WIDGET_CLASS(clazz)->get_shell = _w_control_get_shell;
	W_WIDGET_CLASS(clazz)->init_themedata = _w_control_init_themedata;
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
	clazz->get_region = _w_control_get_region;
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
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		_W_WIDGET_PRIV(priv)->handle_top = _w_control_handle_top;
		_W_WIDGET_PRIV(priv)->create_widget = _w_control_create_widget;
		_W_WIDGET_PRIV(priv)->hook_events = _w_control_hook_events;
		priv->draw_widget = _w_control_draw_widget;
		priv->handle_fixed = _w_widget_h;
		priv->handle_client = _w_widget_h;
		priv->handle_focus = _w_widget_h;
		priv->handle_event = _w_widget_h;
		priv->handle_enterexit = _w_control_handle_enterexit;
		priv->handle_paint = _w_control_handle_paint;
		priv->handle_im = _w_control_handle_im;
		priv->handle_font = _w_widget_h;
		priv->window_paint = _w_control_window_paint;
		priv->window_event = _w_control_window_event;
		priv->window_enable = _w_control_window_enable;
		priv->window_redraw = _w_control_window_redraw;
		priv->check_border = _w_control_check_border;
		priv->check_buffered = _w_control_check_buffered;
		priv->check_background = _w_control_check_background;
		priv->check_foreground = _w_control_check_foreground;
		priv->check_mirrored = _w_control_check_mirrored;
		priv->check_subwindow = _w_control_check_subwindow;
		priv->find_background_control = _w_control_find_background_control;
		priv->get_client_width = _w_control_get_client_width;
		priv->mark_layout = _w_control_mark_layout;
		priv->move_children = _w_control_move_children;
		priv->move_handle = _w_control_move_handle;
		priv->show_widget = _w_control_show_widget;
		priv->redraw_widget = _w_control_redraw_widget;
		priv->resize_handle = _w_control_resize_handle;
		priv->set_bounds_0 = _w_control_set_bounds_0;
		priv->set_font_description = _w_control_set_font_description;
		priv->set_parent_background = _w_control_set_parent_background;
		priv->set_initial_bounds = _w_control_set_initial_bounds;
		priv->set_relations = _w_control_set_relations;
		priv->set_zorder = _w_control_set_zorder;
		priv->update_layout = _w_control_update_layout;
		priv->set_cursor_0 = _w_control_set_cursor_0;
		priv->update_0 = _w_control_update_0;
		priv->force_resize = _w_control_force_resize;
		priv->has_focus = _w_control_has_focus;
		priv->get_imcaret_pos = _w_control_get_imcaret_pos;
		priv->translate_traversal = _w_control_translate_traversal;
		priv->send_leave_notify = _w_control_send_leave_notify;
		priv->is_focus_handle = _w_control_is_focus_handle;
		/*
		 * signals
		 */
		_gtk_signal_fn *signals = _W_WIDGET_PRIV(priv)->signals;
		signals[SIGNAL_DESTROY] = _gtk_control_destroy;
		signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_control_button_press_event;
		signals[SIGNAL_BUTTON_RELEASE_EVENT] =
				_gtk_control_button_release_event;
		signals[SIGNAL_COMMIT] = _gtk_control_commit;
		signals[SIGNAL_ENTER_NOTIFY_EVENT] = _gtk_control_enter_notify_event;
		signals[SIGNAL_EVENT_AFTER] = _gtk_control_event_after;
		signals[SIGNAL_DRAW] = _gtk_control_draw;
		//signals[SIGNAL_EXPOSE_EVENT_INVERSE] = _gtk_control_expose_inverse;
		signals[SIGNAL_FOCUS] = _gtk_control_focus;
		signals[SIGNAL_FOCUS_IN_EVENT] = _gtk_control_focus_in_event;
		signals[SIGNAL_FOCUS_OUT_EVENT] = _gtk_control_focus_out_event;
		signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_control_key_press_event;
		signals[SIGNAL_KEY_RELEASE_EVENT] = _gtk_control_key_release_event;
		signals[SIGNAL_LEAVE_NOTIFY_EVENT] = _gtk_control_leave_notify_event;
		signals[SIGNAL_MNEMONIC_ACTIVATE] = _gtk_control_mnemonic_activate;
		signals[SIGNAL_MOTION_NOTIFY_EVENT] = _gtk_control_motion_notify_event;
		signals[SIGNAL_POPUP_MENU] = _gtk_control_popup_menu;
		signals[SIGNAL_PREEDIT_CHANGED] = _gtk_control_preedit_changed;
		signals[SIGNAL_REALIZE] = _gtk_control_realize;
		signals[SIGNAL_SCROLL_EVENT] = _gtk_control_scroll_event;
		signals[SIGNAL_SHOW_HELP] = _gtk_control_show_help;
		signals[SIGNAL_STYLE_SET] = _gtk_control_style_set;
		signals[SIGNAL_UNREALIZE] = _gtk_control_unrealize;
	}
}
wresult _w_ccanvas_create_handle(w_widget *widget, _w_control_priv *priv) {
	GtkWidget *fixedHandle;
	fixedHandle = _w_fixed_new(0);
	if (fixedHandle == 0)
		return W_ERROR_NO_HANDLES;
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = fixedHandle;
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
}
void _w_ccanvas_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_ccanvas_class *clazz) {
	if (classId == _W_CLASS_CCANVAS) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&gtk_toolkit->class_ccanvas_priv;
	}
	_w_control_class_init(toolkit, classId, W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_CCANVAS;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_ccanvas_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_control);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_control);
	_w_widget_priv *priv = _W_WIDGET_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_CCANVAS) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		_W_WIDGET_PRIV(priv)->handle_top = _w_widget_h;
		_W_WIDGET_PRIV(priv)->create_handle = _w_ccanvas_create_handle;
		_W_CONTROL_PRIV(priv)->handle_fixed = _w_widget_h;
	}
}
