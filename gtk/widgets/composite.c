/*
 * Name:        composite.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "composite.h"
#include "toolkit.h"
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
void _w_composite_add_child(w_control *composite, w_widget *child,
		_w_control_priv *priv) {
	_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(child);
	GtkWidget *topHandle = _W_WIDGET_PRIV(cpriv)->handle_top(W_WIDGET(child),
			cpriv);
	GtkWidget *parentHandle = _W_COMPOSITE_PRIV(priv)->handle_parenting(
			W_WIDGET(composite), priv);
	w_link_linklast_0(&_W_WIDGET(child)->sibling, child,
			(void**) &_W_WIDGET(composite)->first_child);
	gtk_container_add(GTK_CONTAINER(parentHandle), topHandle);
	_W_WIDGET(composite)->children_count++;
}
wresult _w_composite_create_handle_0(w_widget *composite, GtkWidget **fixed,
		GtkWidget **scrolled, GtkWidget **handle, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(composite)->style;
	GtkAdjustment *vadj = 0, *hadj = 0;
	GtkWidget *socketHandle = 0;
	GtkIMContext *imHandle = 0;
	*handle = 0;
	if (fixed != 0) {
		*fixed = _w_fixed_new(0);
		if (*fixed == 0)
			goto _err;
		gtk_widget_set_has_window(*fixed, TRUE);
		_w_widget_set_control(*fixed, composite);
	}
	if (scrolled != 0) {
		vadj = gtk_adjustment_new(0, 0, 100, 1, 10, 10);
		if (vadj == 0)
			goto _err;
		hadj = gtk_adjustment_new(0, 0, 100, 1, 10, 10);
		if (hadj == 0)
			goto _err;
		*scrolled = gtk_scrolled_window_new(hadj, vadj);
		if (*scrolled == 0)
			goto _err;
		_w_widget_set_control(vadj, composite);
		_w_widget_set_control(hadj, composite);
		_w_widget_set_control(*scrolled, composite);
		gtk_widget_show_all(*scrolled);
	}
	*handle = (GtkWidget*) _w_fixed_new(composite);
	if (*handle == 0)
		goto _err;
	_w_widget_set_control(*handle, composite);
	gtk_widget_set_has_window(*handle, TRUE);
	gtk_widget_set_can_focus(*handle, TRUE);
	if ((_W_WIDGET(composite)->state & STATE_CANVAS) != 0) {
		imHandle = gtk_im_multicontext_new();
		if (imHandle == 0)
			goto _err;
	}
	if (scrolled) {
		if (fixed)
			_w_fixed_set_child(*fixed, *scrolled);
		/*
		 * Force the scrolledWindow to have a single child that is
		 * not scrolled automatically.  Calling gtk_container_add()
		 * seems to add the child correctly but cause a warning.
		 */
		//setWarnings(false);
		gtk_container_add(GTK_CONTAINER(*scrolled), *handle);
		//setWarnings(warnings);

		GtkPolicyType hsp =
				(style & W_HSCROLL) != 0 ? GTK_POLICY_ALWAYS : GTK_POLICY_NEVER;
		GtkPolicyType vsp =
				(style & W_VSCROLL) != 0 ? GTK_POLICY_ALWAYS : GTK_POLICY_NEVER;
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(*scrolled), hsp,
				vsp);
		if (_W_COMPOSITE_PRIV(priv)->has_border(W_COMPOSITE(composite), priv)) {
			gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(*scrolled),
					GTK_SHADOW_ETCHED_IN);
		}
	}
	if ((style & W_NO_REDRAW_RESIZE) != 0 && (style & W_RIGHT_TO_LEFT) == 0) {
		gtk_widget_set_redraw_on_allocate(*handle,
		FALSE);
	}
	/*
	 * Bug in GTK.  When a widget is double buffered and the back
	 * pixmap is null, the double buffer pixmap is filled with the
	 * background of the widget rather than the current contents of
	 * the screen.  If nothing is drawn during an expose event,
	 * the pixels are altered.  The fix is to clear double buffering
	 * when NO_BACKGROUND is set and DOUBLE_BUFFERED
	 * is not explicitly set.
	 */
	if ((style & W_DOUBLE_BUFFERED) == 0 && (style & W_NO_BACKGROUND) != 0) {
		gtk_widget_set_double_buffered(*handle,
		FALSE);
	}
	_W_COMPOSITE(composite)->imHandle = imHandle;
	return W_TRUE;
	_err: if (vadj != 0)
		g_object_unref(vadj);
	if (hadj != 0)
		g_object_unref(hadj);
	if (scrolled && *scrolled != 0)
		g_object_unref(*scrolled);
	if (fixed && *fixed != 0)
		g_object_unref(*fixed);
	if (*handle != 0)
		g_object_unref(*handle);
	if (imHandle != 0) {
		g_object_unref(imHandle);
	}
	return W_ERROR_NO_HANDLES;
}
wresult _w_composite_create_handle(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_CANVAS
			| STATE_CHECK_SUBWINDOW;
	int scrolled = (_W_WIDGET(widget)->style & (W_HSCROLL | W_VSCROLL)) != 0;
	if (!scrolled)
		_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
	GtkWidget *fixedHandle, *scrolledHandle, *handle, **scrolled_ = 0;
	if (scrolled || (_W_WIDGET(widget)->style & W_BORDER) != 0) {
		scrolled_ = &scrolledHandle;
	}
	if (scrolled_ == 0) {
		handle = _w_fixed_new(widget);
		if (handle == 0)
			return W_ERROR_NO_HANDLES;
		fixedHandle = handle;
		gtk_widget_set_has_window(handle, TRUE);
		gtk_widget_set_can_focus(handle, TRUE);
	} else {
		int ret = _w_composite_create_handle_0(widget, &fixedHandle, scrolled_,
				&handle, priv);
		if (ret < 0)
			return ret;
		if (scrolled_ == 0) {
			gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
		}
		_w_widget_set_control(handle, widget);
		if (scrolled_ != 0) {
			_w_widget_set_control(*scrolled_, widget);
		}
	}
	_w_widget_set_control(fixedHandle, widget);
	_W_WIDGET(widget)->handle = handle;
	return W_TRUE;
}
void _w_composite_minimum_size(w_composite *composite, w_size *size, int wHint,
		int hHint, int changed) {
	w_iterator children;
	w_iterator_init(&children);
	w_composite_get_children(composite, &children);
	/*
	 * Since getClientArea can be overridden by subclasses, we cannot
	 * call getClientAreaInPixels directly.
	 */
	w_rect clientArea, rect;
	w_scrollable_get_client_area(W_SCROLLABLE(composite), &clientArea);
	int width = 0, height = 0;
	w_control *child = 0;
	while (w_iterator_next(&children, &child)) {
		if (child != 0) {
			w_control_get_bounds(child, &rect.pt, &rect.sz);
			width = WMAX(width, rect.x - clientArea.x + rect.width);
			height = WMAX(height, rect.y - clientArea.y + rect.height);
		}
	}
	size->width = width;
	size->height = height;
}
wresult _w_composite_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	//display.runSkin();
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	if (_W_COMPOSITE(widget)->layout != 0) {
		if (wHint == W_DEFAULT || hHint == W_DEFAULT) {
			//changed |= (state & LAYOUT_CHANGED) != 0;
			w_layout_compute_size(_W_COMPOSITE(widget)->layout,
					W_COMPOSITE(widget), e->size, wHint, hHint, e->changed);
			//state &= ~LAYOUT_CHANGED;
		} else {
			e->size->width = wHint;
			e->size->height = hHint;
		}
	} else {
		_w_composite_minimum_size(W_COMPOSITE(widget), e->size, wHint, hHint,
		TRUE);
		if (e->size->width == 0)
			e->size->width = DEFAULT_WIDTH;
		if (e->size->height == 0)
			e->size->height = DEFAULT_HEIGHT;
	}
	if (wHint != W_DEFAULT)
		e->size->width = wHint;
	if (hHint != W_DEFAULT)
		e->size->height = hHint;
	w_rect rect, trim;
	rect.x = 0;
	rect.y = 0;
	rect.width = e->size->width;
	rect.height = e->size->height;
	w_scrollable_compute_trim(W_SCROLLABLE(widget), &trim, &rect);
	e->size->width = trim.width;
	e->size->height = trim.height;
	return TRUE;
}
w_composite* _w_composite_find_deferred_control(w_control *composite,
		_w_control_priv *priv) {
	if ( _W_COMPOSITE(composite)->layoutCount > 0) {
		return W_COMPOSITE(composite);
	} else {
		w_widget *parent = _W_WIDGET(composite)->parent;
		_w_control_priv *ppriv = _W_CONTROL_GET_PRIV(parent);
		return _W_COMPOSITE_PRIV(ppriv)->find_deferred_control(
				W_CONTROL(parent), ppriv);
	}
}
void _w_composite_fix_zorder(w_composite *composite, _w_control_priv *priv) {
	if ((_W_WIDGET(composite)->state & STATE_CANVAS) != 0)
		return;
	GtkWidget *parentHandle = _W_COMPOSITE_PRIV(priv)->handle_parenting(
			W_WIDGET(composite), priv);
	GdkWindow *parentWindow = gtk_widget_get_window(parentHandle);
	if (parentWindow == 0)
		return;
	GdkWindow *redrawWindow = priv->window_redraw(W_WIDGET(composite), priv);
	void *userData;
	GList *windows = gdk_window_peek_children(parentWindow);
	while (windows != 0) {
		GdkWindow *window = (GdkWindow*) windows->data;
		if (window != redrawWindow) {
			gdk_window_get_user_data(window, &userData);
			if (userData == 0 || !_W_IS_FIXED(userData)) {
				gdk_window_lower(window);
			}
		}
		windows = windows->next;
	}
}
GtkWidget* _w_composite_handle_parenting(w_widget *control,
		_w_control_priv *priv) {
	if ((_W_WIDGET(control)->state & STATE_CANVAS) != 0)
		return _W_WIDGET(control)->handle;
	GtkWidget *fixedHandle = priv->handle_fixed(control, priv);
	return fixedHandle != 0 ? fixedHandle : _W_WIDGET(control)->handle;
}
GtkWidget* _w_composite_handle_scrolled(w_widget *control,
		_w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	int scrolled = (style & (W_HSCROLL | W_VSCROLL)) != 0;
	if (scrolled || (style & W_BORDER) != 0) {
		GtkWidget *handle = _W_WIDGET(control)->handle;
		return gtk_widget_get_parent(handle);
	} else
		return 0;
}
GtkWidget* _w_composite_handle_fixed(w_widget *control, _w_control_priv *priv) {
	GtkWidget *scrolledHandle = _W_SCROLLABLE_PRIV(priv)->handle_scrolled(
			control, priv);
	if (scrolledHandle == 0)
		return _W_WIDGET(control)->handle;
	else
		return gtk_widget_get_parent(scrolledHandle);
}
wresult _w_composite_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
		if ((_W_WIDGET(widget)->state & STATE_ZERO_WIDTH) != 0
				&& (_W_WIDGET(widget)->state & STATE_ZERO_HEIGHT) != 0) {
			memset(e->rect, 0, sizeof(w_rect));
			return W_TRUE;
		}
		priv->force_resize(W_CONTROL(widget), priv);
		GtkWidget *clientHandle = priv->handle_client(widget, priv);
		GtkAllocation allocation;
		gtk_widget_get_allocation(clientHandle, &allocation);
		int width =
				(_W_WIDGET(widget)->state & STATE_ZERO_WIDTH) != 0 ?
						0 : allocation.width;
		int height =
				(_W_WIDGET(widget)->state & STATE_ZERO_HEIGHT) != 0 ?
						0 : allocation.height;
		e->rect->x = 0;
		e->rect->y = 0;
		e->rect->width = width;
		e->rect->height = height;
		return W_TRUE;
	} else {
		return _w_scrollable_get_client_area(widget, e, priv);
	}
}
/*
 * iterator of children control
 */
typedef struct _w_composite_children {
	w_basic_iterator base;
	w_composite *composite;
	w_widget *current;
	size_t count;
	int tablist;
} _w_composite_children;
wresult _w_composite_children_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_composite_children_next(w_iterator *it, void *obj) {
	_w_composite_children *iter = (_w_composite_children*) it;
	if (iter->current != 0) {
		w_widget *w = iter->current;
		int tablist = iter->tablist;
		while (w != 0) {
			if (w->clazz->class_id >= _W_CLASS_CONTROL) {
				if (!tablist || (_W_WIDGET(w)->state & STATE_TAB_LIST) != 0)
					break;
			}
			w = _W_WIDGET(w)->sibling.next;
		}
		*((w_widget**) obj) = w;
		iter->current = _W_WIDGET(w)->sibling.next;
		return W_TRUE;
	} else {
		*((w_widget**) obj) = 0;
		return W_FALSE;
	}
}
wresult _w_composite_children_reset(w_iterator *it) {
	_w_composite_children *iter = (_w_composite_children*) it;
	iter->current = _W_WIDGET(iter->composite)->first_child;
	return W_TRUE;
}
wresult _w_composite_children_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;

}
size_t _w_composite_children_get_count(w_iterator *it) {
	_w_composite_children *iter = (_w_composite_children*) it;
	return iter->count;
}
_w_iterator_class _w_composite_children_class = { //
		_w_composite_children_close, //
				_w_composite_children_next, //
				_w_composite_children_reset, //
				_w_composite_children_remove, //
				_w_composite_children_get_count //
		};
wresult _w_composite_for_all_children(w_composite *composite,
		w_widget_callback callback, void *user_data, int flags) {
	w_widget *w = _W_WIDGET(composite)->first_child;
	w_widget *next;
	wresult result;
	while (w != 0) {
		next = _W_WIDGET(w)->sibling.next;
		if (w->clazz->class_id >= _W_CLASS_CONTROL) {
			result = callback(W_WIDGET(composite), w, user_data);
		}
		if (result)
			break;
		w = next;
	}
	return W_TRUE;
}
wresult _w_composite_get_children(w_composite *composite, w_iterator *it) {
	_w_composite_children *iter = (_w_composite_children*) it;
	iter->base.clazz = &_w_composite_children_class;
	iter->composite = composite;
	iter->current = _W_WIDGET(composite)->first_child;
	iter->count = _W_WIDGET(composite)->children_count;
	iter->tablist = 0;
	return W_TRUE;
}
int _w_composite_get_children_count(w_composite *composite) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(composite);
	_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_PRIV(priv)->handle_parenting(
			W_WIDGET(composite), priv);
	return fixed->count;
}
wresult _w_composite_get_layout(w_composite *composite, w_layout **layout) {
	*layout = _W_COMPOSITE(composite)->layout;
	return W_TRUE;
}
wresult _w_composite_get_layout_deferred(w_composite *composite) {
	return W_FALSE;
}
wresult _w_composite_get_tab_list(w_composite *composite, w_iterator *it) {
	return W_FALSE;
}
wresult _w_composite_has_border(w_composite *composite, _w_control_priv *priv) {
	return (_W_WIDGET(composite)->style & W_BORDER) != 0;
}
void _w_composite_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_scrollable_hook_events(widget, priv);
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
		GtkWidget *handle = _W_WIDGET(widget)->handle;
		gtk_widget_add_events(handle, GDK_POINTER_MOTION_HINT_MASK);
		GtkWidget *scrolledHandle =
		_W_SCROLLABLE_PRIV(priv)->handle_scrolled(widget, priv);
		if (scrolledHandle != 0) {
			_w_widget_connect(scrolledHandle,
					&gtk_toolkit->signals[SIGNAL_SCROLL_CHILD],
					FALSE);
		}
	}
}
wresult _w_composite_is_layout_deferred(w_composite *composite) {
	return W_FALSE;
}
wresult _w_composite_do_layout(w_composite *composite, int changed, int all) {
	return W_FALSE;
}
wresult _w_composite_layout_changed(w_composite *_this, w_control **changed,
		size_t length, int flags) {
	return W_FALSE;
}
wresult _w_composite_mark_layout_callback(w_widget *widget, void *child,
		void *user_data) {
	_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(child);
	cpriv->mark_layout(W_CONTROL(child), (intptr_t) user_data, cpriv);
	return W_FALSE;
}
void _w_composite_mark_layout(w_control *control, int flags,
		_w_control_priv *priv) {
	//if (layout != null) {
	_W_WIDGET(control)->state |= STATE_LAYOUT_NEEDED;
	if (flags & W_CHANGED)
		_W_WIDGET(control)->state |= STATE_LAYOUT_CHANGED;
	//}
	if (flags & W_ALL) {
		_w_composite_for_all_children(W_COMPOSITE(control),
				_w_composite_mark_layout_callback, (void*) ((intptr_t) flags),
				0);
	}
}
wresult _w_composite_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_priv *priv) {
	int result = _w_control_set_bounds_0(control, location, size, priv);
	if (result < 0)
		return result;
	if (result & 2) { // widget is resized
		priv->mark_layout(control, 0, priv);
		priv->update_layout(control, 0, priv);
	}
	return result;
}
wresult _w_composite_set_layout(w_composite *composite, w_layout *layout) {
	_W_COMPOSITE(composite)->layout = layout;
	return W_TRUE;
}
wresult _w_composite_set_layout_deferred(w_composite *composite, int defer) {
	return W_FALSE;
}
wresult _w_composite_update_layout_callback(w_widget *widget, void *child,
		void *user_data) {
	_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(child);
	cpriv->update_layout(W_CONTROL(child), (intptr_t) user_data, cpriv);
	return W_FALSE;
}
void _w_composite_update_layout(w_control *control, int flags,
		_w_control_priv *priv) {
	w_composite *parent =
	_W_COMPOSITE_PRIV(priv)->find_deferred_control(control, priv);
	if (parent != 0) {
		_W_WIDGET(parent)->state |= STATE_LAYOUT_CHILD;
		return;
	}
	if ((_W_WIDGET(control)->state & STATE_LAYOUT_NEEDED) != 0) {
		int changed = (_W_WIDGET(control)->state & STATE_LAYOUT_CHANGED) != 0;
		_W_WIDGET(control)->state &= ~(STATE_LAYOUT_NEEDED
				| STATE_LAYOUT_CHANGED);
		w_event _e;
		_e.type = W_EVENT_LAYOUTDETECT;
		_e.platform_event = 0;
		_e.widget = W_WIDGET(control);
		_e.data = 0;
		_w_widget_send_event(W_WIDGET(control), &_e, W_EVENT_SEND);
	}
	if (flags & W_ALL) {
		_W_WIDGET(control)->state &= ~STATE_LAYOUT_CHILD;
		_w_composite_for_all_children(W_COMPOSITE(control),
				_w_composite_update_layout_callback, (void*) ((intptr_t) flags),
				0);
	}
}
/*
 * signals
 */
gboolean _gtk_composite_destroy(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_COMPOSITE(widget)->imHandle != 0) {
		g_object_unref(_W_COMPOSITE(widget)->imHandle);
		_W_COMPOSITE(widget)->imHandle = 0;
	}
	return _gtk_control_destroy(widget, e, priv);

}

gboolean _gtk_composite_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	gboolean result = _gtk_scrollable_button_press_event(widget, e, priv);
	if (result != 0)
		return result;
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
		if ((_W_WIDGET(widget)->style & W_NO_FOCUS) == 0) {
			GdkEventButton *gdkEvent = (GdkEventButton*) e->args[0];
			if (gdkEvent->button == 1) {
				if (_w_composite_get_children_count(W_COMPOSITE(widget)) == 0)
					W_CONTROL_GET_CLASS(widget)->set_focus(W_CONTROL(widget));
			}
		}
	}
	return result;
}
gboolean _gtk_composite_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
#if USE_CAIRO
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
#else
#endif
}

gboolean _gtk_composite_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_key_press_event(widget, e, priv);
	if (result != 0)
		return result;
	/*
	 * Feature in GTK.  The default behavior when the return key
	 * is pressed is to select the default button.  This is not the
	 * expected behavior for Composite and its subclasses.  The
	 * fix is to avoid calling the default handler.
	 */
	GtkWidget *socketHandle = 0;
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0 && socketHandle == 0) {
		GdkEventKey *keyEvent = (GdkEventKey*) e->args[0];
		int key = keyEvent->keyval;
		switch (key) {
		case GDK_KEY_Return:
		case GDK_KEY_KP_Enter:
			return TRUE;
		}
	}
	return result;
}

gboolean _gtk_composite_focus(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GtkWidget *socketHandle = 0;
	if (e->widget == socketHandle)
		return 0;
	return _gtk_control_focus(widget, e, priv);
}

gboolean _gtk_composite_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_focus_in_event(widget, e, priv);
	return (_W_WIDGET(widget)->state & STATE_CANVAS) != 0 ? TRUE : result;
}

gboolean _gtk_composite_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_focus_out_event(widget, e, priv);
	return (_W_WIDGET(widget)->state & STATE_CANVAS) != 0 ? TRUE : result;
}

gboolean _gtk_composite_map(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	_w_composite_fix_zorder(W_COMPOSITE(widget), priv);
	return FALSE;
}

gboolean _gtk_composite_realize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_realize(widget, e, priv);
	;
	if ((_W_WIDGET(widget)->style & W_NO_BACKGROUND) != 0) {
		GtkWidget *paintHandle = priv->handle_paint(widget, priv);
		GdkWindow *window = gtk_widget_get_window(paintHandle);
		if (window != 0) {
#if GTK3
			gdk_window_set_background_pattern(window, 0);
#endif
		}
	}
	return result;
}

gboolean _gtk_composite_scroll_child(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/* Stop GTK scroll child signal for canvas */
	g_signal_stop_emission(e->widget,
			gtk_toolkit->signals[SIGNAL_SCROLL_CHILD].id, 0);
	return TRUE;
}

gboolean _gtk_composite_style_set(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_style_set(widget, e, priv);
	if ((_W_WIDGET(widget)->style & W_NO_BACKGROUND) != 0) {
		GtkWidget *paintHandle = priv->handle_paint(widget, priv);
		GdkWindow *window = gtk_widget_get_window(paintHandle);
#if GTK3
		/*if (window != 0)
		 gdk_window_set_back_pixmap(window, 0, FALSE);*/
#endif
	}
	return result;
}
wresult _w_composite_dispose_class(struct _w_widget_class *clazz){
	_w_scrollable_dispose_class(clazz);
	return W_TRUE;
}
void _w_composite_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_composite_class *clazz) {
	if (classId == _W_CLASS_COMPOSITE) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&gtk_toolkit->class_composite_priv;
	}
	_w_scrollable_class_init(toolkit, classId, W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMPOSITE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_composite_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_composite);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_composite);
	/*
	 * functions
	 */
	W_WIDGET_CLASS(clazz)->dispose_class = _w_composite_dispose_class;
	clazz->for_all_children = _w_composite_for_all_children;
	clazz->get_children = _w_composite_get_children;
	clazz->get_layout = _w_composite_get_layout;
	clazz->get_layout_deferred = _w_composite_get_layout_deferred;
	clazz->get_tab_list = _w_composite_get_tab_list;
	clazz->is_layout_deferred = _w_composite_is_layout_deferred;
	clazz->do_layout = _w_composite_do_layout;
	clazz->layout_changed = _w_composite_layout_changed;
	clazz->set_layout = _w_composite_set_layout;
	clazz->set_layout_deferred = _w_composite_set_layout_deferred;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_COMPOSITE) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		_W_WIDGET_PRIV(priv)->create_handle = _w_composite_create_handle;
		_W_WIDGET_PRIV(priv)->get_client_area = _w_composite_get_client_area;
		_W_WIDGET_PRIV(priv)->hook_events = _w_composite_hook_events;
		_W_WIDGET_PRIV(priv)->handle_top = _w_composite_handle_fixed;
		_W_WIDGET_PRIV(priv)->compute_size = _w_composite_compute_size;
		priv->handle_fixed = _w_composite_handle_fixed;
		priv->mark_layout = _w_composite_mark_layout;
		priv->update_layout = _w_composite_update_layout;
		priv->set_bounds_0 = _w_composite_set_bounds_0;
		_W_SCROLLABLE_PRIV(priv)->handle_scrolled =
				_w_composite_handle_scrolled;
		_W_COMPOSITE_PRIV(priv)->add_child = _w_composite_add_child;
		_W_COMPOSITE_PRIV(priv)->has_border = _w_composite_has_border;
		_W_COMPOSITE_PRIV(priv)->handle_parenting =
				_w_composite_handle_parenting;
		_W_COMPOSITE_PRIV(priv)->find_deferred_control =
				_w_composite_find_deferred_control;
		/*
		 * signals
		 */
		_gtk_signal_fn *signals = _W_WIDGET_PRIV(priv)->signals;
		signals[SIGNAL_DESTROY] = _gtk_composite_destroy;
		signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_composite_button_press_event;
		signals[SIGNAL_DRAW] = _gtk_composite_draw;
		signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_composite_key_press_event;
		signals[SIGNAL_FOCUS] = _gtk_composite_focus;
		signals[SIGNAL_FOCUS_IN_EVENT] = _gtk_composite_focus_in_event;
		signals[SIGNAL_FOCUS_OUT_EVENT] = _gtk_composite_focus_out_event;
		signals[SIGNAL_MAP] = _gtk_composite_map;
		signals[SIGNAL_REALIZE] = _gtk_composite_realize;
		signals[SIGNAL_SCROLL_CHILD] = _gtk_composite_scroll_child;
	}
}
