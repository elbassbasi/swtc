/*
 * Name:        shell.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "shell.h"
#include "toolkit.h"
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
wresult _w_shell_close(w_shell *shell) {
	_w_shell_close_widget(shell, 0);
	return W_TRUE;
}
void _w_shell_close_widget(w_shell *shell, _w_event_platform *e) {
	wuint64 style = w_widget_get_style(W_WIDGET(shell));
	w_event event;
	event.type = W_EVENT_CLOSE;
	event.platform_event = (w_event_platform*) e;
	event.widget = W_WIDGET(shell);
	event.data = 0;
	_w_widget_send_event(W_WIDGET(shell), &event);
	if (style & W_DISPOSE_ON_CLOSE) {
		w_widget_dispose(W_WIDGET(shell));
	}
}
wuint64 _w_shell_check_style(w_widget *widget, wuint64 style) {
	if ((style & W_NO_TRIM) != 0) {
		style &= ~(W_CLOSE | W_MIN | W_MAX | W_RESIZE | W_BORDER);
	} else if ((style & W_NO_MOVE) != 0) {
		style |= W_TITLE;
	}
	if ((style & (W_MENU | W_MIN | W_MAX | W_CLOSE)) != 0) {
		style |= W_TITLE;
	}
	style &= ~W_TRANSPARENT;
	if ((style & W_ON_TOP) != 0)
		style &= ~(W_CLOSE | W_MIN | W_MAX);
	int mask = W_SYSTEM_MODAL | W_APPLICATION_MODAL | W_PRIMARY_MODAL;
	if ((style & W_SHEET) != 0) {
		style &= ~W_SHEET;
		w_composite *parent = _W_CONTROL(widget)->parent;
		style |= parent == 0 ? W_SHELL_TRIM : W_DIALOG_TRIM;
		if ((style & mask) == 0) {
			style |= parent == 0 ? W_APPLICATION_MODAL : W_PRIMARY_MODAL;
		}
	}
	int bits = style & ~mask;
	if ((style & W_SYSTEM_MODAL) != 0)
		return bits | W_SYSTEM_MODAL;
	if ((style & W_APPLICATION_MODAL) != 0)
		return bits | W_APPLICATION_MODAL;
	if ((style & W_PRIMARY_MODAL) != 0)
		return bits | W_PRIMARY_MODAL;
	return bits;
}
wresult _w_shell_create_embedded(w_widget *widget, w_widget *parent,
		wuint64 style, w_widget_post_event_proc post_event, void *handle,
		int embedded) {
	if (parent != 0) {
		if (!w_widget_is_ok(parent))
			return W_ERROR_INVALID_ARGUMENT;
		if (w_widget_class_id(parent) != _W_CLASS_SHELL)
			return W_ERROR_INVALID_ARGUMENT;
	}
	_W_CONTROL(widget)->parent = W_COMPOSITE(parent);
	_W_SHELL(widget)->center = parent != 0 && (style & W_SHEET) != 0;
	_W_WIDGET(widget)->post_event = post_event;
	_W_WIDGET(widget)->style = _w_shell_check_style(widget, style);
	if (handle != 0) {
		if (embedded) {
			_W_WIDGET(widget)->handle = handle;
		} else {
			_W_WIDGET(widget)->handle = handle;
			_W_WIDGET(widget)->state |= STATE_FOREIGN_HANDLE;
		}
	}
	_w_widget_priv *priv = _w_widget_get_priv(widget);
	wresult result = priv->create_widget(widget, _W_CONTROL_PRIV(priv));
	if (result > 0) {
		_w_toolkit_add_shell(_W_SHELL(widget));
	}
	return result;
}
wresult _w_shell_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_shell_create_embedded(widget, parent, style, post_event, 0,
			W_FALSE);
}
wresult _w_shell_create_handle(w_widget *widget, _w_control_priv *priv) {
	GtkWindowType type;
	GtkWidget *vboxHandle = 0, *scrolled = 0, *handle = 0;
	GtkWindow *shellHandle = 0;
	GtkWindowGroup *group = 0;
	wuint64 style = _W_WIDGET(widget)->style;
	if (_W_WIDGET(widget)->state & STATE_FOREIGN_HANDLE) {
		shellHandle = (GtkWindow*) _W_WIDGET(widget)->handle;
	}
	_w_widget_priv *ppriv;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_CANVAS;
	if (shellHandle == 0) {
		type = GTK_WINDOW_TOPLEVEL;
		if ((style & W_ON_TOP) != 0)
			type = GTK_WINDOW_POPUP;
		shellHandle = GTK_WINDOW(gtk_window_new(type));
		if (shellHandle == 0)
			goto _err;
		w_composite *parent = _W_CONTROL(widget)->parent;
		if (parent != 0) {
			ppriv = _W_WIDGET_GET_PRIV(parent);
			GtkWindow *pwindow = (GtkWindow*) ppriv->handle_top(
					W_WIDGET(parent), _W_CONTROL_PRIV(ppriv));
			gtk_window_set_transient_for(shellHandle, pwindow);
			gtk_window_set_destroy_with_parent(shellHandle, TRUE);
			// if child shells are minimizable, we want them to have a
			// taskbar icon, so they can be unminimized
			if ((style & W_MIN) == 0) {
				gtk_window_set_skip_taskbar_hint(shellHandle, TRUE);
			}

			/*
			 * For systems running Metacity, by applying the dialog type hint
			 * to a window only the close button can be placed on the title bar.
			 * The style hints for the minimize and maximize buttons are ignored.
			 * See bug 445456.
			 *
			 */
//			if (!isUndecorated ()) {
//				OS.gtk_window_set_type_hint (shellHandle, OS.GDK_WINDOW_TYPE_HINT_DIALOG);
//			}
		} else {
			if ((style & W_ON_TOP) != 0)
				gtk_window_set_keep_above(shellHandle, TRUE);
		}
		/*
		 * Feature in GTK.  The window size must be set when the window
		 * is created or it will not be allowed to be resized smaller that the
		 * initial size by the user.  The fix is to set the size to zero.
		 */
		if ((style & W_RESIZE) != 0) {
			//gtk_widget_set_size_request(GTK_WIDGET(shellHandle), 0, 0);
			gtk_window_set_resizable(shellHandle, TRUE);
		} else {
			gtk_window_set_resizable(shellHandle, FALSE);
		}
//gtk_window_set_title(shellHandle, "");
		if ((style & (W_NO_TRIM | W_BORDER | W_SHELL_TRIM)) == 0) {
			gtk_container_set_border_width(GTK_CONTAINER(shellHandle), 1);
		}
		if ((style & W_TOOL) != 0) {
			gtk_window_set_type_hint(shellHandle, GDK_WINDOW_TYPE_HINT_UTILITY);
		}
		if ((style & W_NO_TRIM) != 0) {
			gtk_window_set_decorated(shellHandle, FALSE);
		}

		if (ISCUSTOMRESIZE(style)) {
			gtk_container_set_border_width(GTK_CONTAINER(shellHandle),
			_W_SHELL_BORDER);
		}
	}
#if GTK3
	vboxHandle = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	if (vboxHandle == 0)
		goto _err;
	gtk_box_set_homogeneous(GTK_BOX(vboxHandle), FALSE);
#else
#endif
	if (_w_composite_create_handle_0(widget, 0, &scrolled, &handle, priv) < 0)
		goto _err;
	group = gtk_window_group_new();
	if (group == 0)
		goto _err;
	_W_SHELL(widget)->group = group;
	_W_WIDGET(widget)->handle = handle;
	gtk_container_add(GTK_CONTAINER(vboxHandle), scrolled);
	gtk_box_set_child_packing(GTK_BOX(vboxHandle), scrolled, TRUE, TRUE, 0,
			GTK_PACK_END);
	gtk_container_add(GTK_CONTAINER(shellHandle), vboxHandle);
	_w_widget_set_control(shellHandle, widget);
	_w_widget_set_control(vboxHandle, widget);
	gtk_widget_show_all(vboxHandle);
	/*
	 * Feature in GTK.  Realizing the shell triggers a size allocate event,
	 * which may be confused for a resize event from the window manager if
	 * received too late.  The fix is to realize the window during creation
	 * to avoid confusion.
	 */
	gtk_widget_realize(GTK_WIDGET(shellHandle));
	return W_TRUE;
	_err: if (vboxHandle != 0)
		gtk_widget_destroy(vboxHandle);
	if (shellHandle != 0
			&& !(_W_WIDGET(widget)->state & STATE_FOREIGN_HANDLE)) {
		gtk_widget_destroy(GTK_WIDGET(shellHandle));
	}
	if (scrolled != 0)
		gtk_widget_destroy(scrolled);
	if (handle != 0)
		gtk_widget_destroy(handle);
	if (group != 0)
		g_object_unref(G_OBJECT(group));
	return W_ERROR_NO_HANDLES;
}
w_composite* _w_shell_find_deferred_control(w_control *composite,
		_w_control_priv *priv) {
	return _W_COMPOSITE(composite)->layoutCount > 0 ? W_COMPOSITE(composite) : 0;
}
wresult _w_shell_force_active(w_shell *shell) {
	return W_FALSE;
}
void _w_shell_force_resize_0(w_control *control, int width, int height,
		_w_control_priv *priv) {
	int clientWidth = 0;
	int style = _W_WIDGET(control)->style;
#if GTK3
	if ((style & W_MIRRORED) != 0)
		clientWidth = priv->get_client_width(control, priv);
#endif
	GtkWidget *vboxHandle = _W_SHELL_VBOX(control);
	GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
	GtkRequisition minimumSize, naturalSize;
	GtkAllocation allocation;
	int border = gtk_container_get_border_width(GTK_CONTAINER(shellHandle));
	allocation.x = border;
	allocation.y = border;
	allocation.width = width;
	allocation.height = height;
	// Call gtk_widget_get_preferred_size() on GTK 3.20+ to prevent warnings.
	// See bug 486068.
	gtk_widget_get_preferred_size(vboxHandle, &minimumSize, &naturalSize);
	/*
	 * Bug 535075, 536153: On Wayland, we need to set the position of the GtkBox container
	 * relative to the shellHandle to prevent window contents rendered with offset.
	 */
	if (gtk_toolkit->ISWayland) {
		int dest_x;
		int dest_y;
		gtk_widget_translate_coordinates(vboxHandle, shellHandle, 0, 0, &dest_x,
				&dest_y);
		if (dest_x != -1 && dest_y != -1) {
			w_composite *parent = _W_CONTROL(control)->parent;
			wuint64 style = w_widget_get_style(W_WIDGET(parent));
			if (!(parent != 0 && (style & W_ON_TOP) != 0)) {
				allocation.x += dest_x;
				allocation.y += dest_y;
			}
		}
	}
#if GTK3
	gtk_widget_size_allocate(vboxHandle, &allocation);
#else
#endif

#if GTK3
	/*if ((style & W_MIRRORED) != 0)
	 _w_composite_move_children(W_COMPOSITE(control), clientWidth);*/
#endif
}
void _w_shell_force_resize(w_control *control, _w_control_priv *priv) {
	GtkAllocation allocation;
	gtk_widget_get_allocation(_W_SHELL_VBOX(control), &allocation);
	if (gtk_toolkit->ISWayland) {
		/*
		 * Bug 540163: We sometimes are getting the container's allocation
		 * before Shell is fully opened, which gets an incorrect allocation.
		 * Fix is to use the calculated box width/height if bounds have been set.
		 */
		GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
		int border = gtk_container_get_border_width(GTK_CONTAINER(shellHandle));
		int boxWidth = _W_SHELL(control)->oldbounds.width - 2 * border;
		int boxHeight = _W_SHELL(control)->oldbounds.height - 2 * border;
		if (boxWidth != allocation.width || boxHeight != allocation.height) {
			allocation.width = WMAX(boxWidth, allocation.width);
			allocation.height = WMAX(boxHeight, allocation.height);
		}
	}
	_w_shell_force_resize_0(control, allocation.width, allocation.height, priv);
}
wresult _w_shell_get_default_button(w_shell *shell, w_button **button) {
	return W_FALSE;
}
wresult _w_shell_get_alpha(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_full_screen(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_minimum_size(w_shell *shell, w_size *size) {
	return W_FALSE;
}
wresult _w_shell_get_modified(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_images(w_shell *shell, w_image *image, size_t length) {
	return W_FALSE;
}
wresult _w_shell_get_ime_input_mode(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_maximized(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_menu_bar(w_shell *shell, w_menu **menu) {
	return W_FALSE;
}
wresult _w_shell_get_minimized(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_shell(w_control *control, w_shell **shell) {
	*shell = W_SHELL(control);
	return W_TRUE;
}
wresult _w_shell_get_shells(w_shell *shell, w_iterator *iterator) {
	return W_FALSE;
}
wresult _w_shell_get_text(w_shell *shell, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_shell_get_toolbar(w_shell *shell, w_toolbar **toolbar) {
	return W_FALSE;
}
void _w_shell_hook_events(w_widget *shell, _w_control_priv *priv) {
	_w_composite_hook_events(shell, priv);
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	if (_W_SHELL_PRIV(priv)->move_focus_id == 0) {
		_W_SHELL_PRIV(priv)->move_focus_id = g_signal_lookup("move-focus",
				gtk_window_get_type());
	}
	_w_widget_connect(shellHandle, SIGNAL_KEY_PRESS_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_WINDOW_STATE_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_SIZE_ALLOCATE, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_CONFIGURE_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_DELETE_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_MAP_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_ENTER_NOTIFY_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_MOVE_FOCUS,
	_W_SHELL_PRIV(priv)->move_focus_id,
	FALSE);
#if GTK3
	_w_widget_connect(shellHandle, SIGNAL_FOCUS_IN_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_FOCUS_OUT_EVENT, 0, FALSE);
#else
#endif
	if (ISCUSTOMRESIZE(_W_WIDGET(shell)->style)) {
		int mask = GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK
				| GDK_BUTTON_PRESS_MASK | GDK_ENTER_NOTIFY_MASK
				| GDK_LEAVE_NOTIFY_MASK;
		gtk_widget_add_events(shellHandle, mask);
		_w_widget_connect(shellHandle, SIGNAL_EXPOSE_EVENT, 0, FALSE);
		_w_widget_connect(shellHandle, SIGNAL_LEAVE_NOTIFY_EVENT, 0, FALSE);
		_w_widget_connect(shellHandle, SIGNAL_MOTION_NOTIFY_EVENT, 0, FALSE);
		_w_widget_connect(shellHandle, SIGNAL_BUTTON_PRESS_EVENT, 0, FALSE);
	}
}
wresult _w_shell_open(w_shell *shell) {
	//_w_shell_bring_totop(shell, W_FALSE);
	w_composite *parent = _W_CONTROL(shell)->parent;
	if (w_widget_class_id(W_WIDGET(parent)) == _W_CLASS_SHELL
			&& !w_control_is_visible(W_CONTROL(parent))) {
		w_shell_open(W_SHELL(parent));
	}
	W_CONTROL_GET_CLASS(shell)->set_visible(W_CONTROL(shell), W_TRUE);
	return W_TRUE;
}
void _w_shell_resize_bounds(w_control *control, int width, int height,
		int notify, _w_control_priv *priv) {
	GtkWindow *shellHandle = GTK_WINDOW(_W_SHELL_HANDLE(control));
	GtkWidget *vboxHandle = _W_SHELL_VBOX(control);
	GdkWindow *redrawWindow = priv->window_redraw(W_WIDGET(control), priv);
	if (redrawWindow != 0) {
		gdk_window_resize(redrawWindow, width, height);
	}
	GdkWindow *enableWindow = priv->window_enable(W_WIDGET(control), priv);
	if (enableWindow != 0) {
		gdk_window_resize(enableWindow, width, height);
	}
	int border = gtk_container_get_border_width(GTK_CONTAINER(shellHandle));
	int boxWidth = width - 2 * border;
	int boxHeight = height - 2 * border;
#if GTK3
	if ((_W_WIDGET(control)->style & W_RESIZE) == 0) {
		gtk_widget_set_size_request(vboxHandle, boxWidth, boxHeight);
	}
#else
#endif
	_w_shell_force_resize_0(control, boxWidth, boxHeight, priv);
	if (notify) {
		_W_SHELL(control)->resized = 1;
		w_event _e;
		_e.type = W_EVENT_RESIZE;
		_e.widget = W_WIDGET(control);
		_e.time = 0;
		_e.platform_event = 0;
		_e.data = 0;
		_w_widget_send_event(W_WIDGET(control), &_e);
		if (w_widget_is_ok(W_WIDGET(control)) <= 0)
			return;
		priv->mark_layout(control, 0, priv);
		priv->update_layout(control, 0, priv);
	}
}
wresult _w_shell_set_active(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_set_alpha(w_shell *shell, int alpha) {
	return W_FALSE;
}
wresult _w_shell_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_priv *priv) {
	wresult result = _w_control_set_bounds_0(control, location, size, priv);
	if (result < 0)
		return result;
	w_event event;
	GtkWindow *shellHandle = GTK_WINDOW(_W_SHELL_HANDLE(control));
	if (location != 0) {
		int x_pos, y_pos;
		gtk_window_get_position(shellHandle, &x_pos, &y_pos);
		gtk_window_move(shellHandle, location->x, location->y);
		/*
		 * Bug in GTK: gtk_window_get_position () is not always up-to-date right after
		 * gtk_window_move (). The random delays cause problems like bug 445900.
		 *
		 * The workaround is to wait for the position change to be processed.
		 * The limit 1000 is an experimental value. I've seen cases where about 200
		 * iterations were necessary.
		 */
		for (int i = 0; i < 1000; i++) {
			int x2_pos, y2_pos;
			gtk_window_get_position(shellHandle, &x2_pos, &y2_pos);
			if (x2_pos == location->x && y2_pos == location->y) {
				break;
			}
		}
		if (x_pos != location->x || y_pos != location->y) {
			_W_SHELL(control)->moved = TRUE;
			_W_SHELL(control)->oldbounds.x = location->x;
			_W_SHELL(control)->oldbounds.y = location->y;
			event.type = W_EVENT_MOVE;
			event.widget = W_WIDGET(control);
			event.time = 0;
			event.platform_event = 0;
			event.data = 0;
			_w_widget_send_event(W_WIDGET(control), &event);
			if (w_widget_is_ok(W_WIDGET(control)) <= 0)
				return 0;
			result |= 1;
		}
	}
	if (size != 0) {
		int width = size->width;
		int height = size->height;
		//width = Math.max(1, Math.max(minWidth, width - trimWidth()));
		//height = Math.max(1, Math.max(minHeight, height - trimHeight()));
		/*
		 * If the shell is created without a RESIZE style bit, and the
		 * minWidth/minHeight has been set, allow the resize.
		 */
		if ((_W_WIDGET(control)->style & W_RESIZE) != 0
				|| (_W_SHELL(control)->minHeight != 0
						|| _W_SHELL(control)->minWidth != 0))
			gtk_window_resize(shellHandle, width, height);
		int changed = width != _W_SHELL(control)->oldbounds.width
				|| height != _W_SHELL(control)->oldbounds.height;
		if (changed) {
			_W_SHELL(control)->oldbounds.width = width;
			_W_SHELL(control)->oldbounds.height = height;
			result |= 2;		//resized
		}
		_w_shell_resize_bounds(control, width, height, changed, priv);
	}
	return result;
}
wresult _w_shell_set_default_button(w_shell *shell, w_button *button) {
	return W_FALSE;
}
wresult _w_shell_set_full_screen(w_shell *shell, int fullScreen) {
	return W_FALSE;
}
wresult _w_shell_set_images(w_shell *shell, w_image *image, size_t length) {
	return W_FALSE;
}
wresult _w_shell_set_ime_input_mode(w_shell *shell, int mode) {
	return W_FALSE;
}
wresult _w_shell_set_maximized(w_shell *shell, int maximized) {
	return W_FALSE;
}
wresult _w_shell_set_menu_bar(w_shell *shell, w_menu *menu) {
	return W_FALSE;
}
wresult _w_shell_set_minimized(w_shell *shell, int minimized) {
	return W_FALSE;
}
wresult _w_shell_set_minimum_size(w_shell *shell, w_size *size) {
	return W_FALSE;
}
wresult _w_shell_set_modified(w_shell *shell, int modified) {
	return W_FALSE;
}
wresult _w_shell_set_text(w_shell *shell, const char *string, size_t length,
		int enc) {
	return W_FALSE;
}
wresult _w_shell_set_visible(w_control *control, int visible) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
	w_composite *parent = _W_CONTROL(control)->parent;
	w_event e;
	int mask = W_PRIMARY_MODAL | W_APPLICATION_MODAL | W_SYSTEM_MODAL;
	if ((_W_WIDGET(control)->style & mask) != 0) {
		if (visible) {
			//_w_toolkit_set_modal_shell(gtk_toolkit, W_SHELL(control));
			gtk_window_set_modal(GTK_WINDOW(shellHandle), TRUE);
		} else {
			//_w_toolkit_clear_modal(gtk_toolkit, W_SHELL(control));
			gtk_window_set_modal(GTK_WINDOW(shellHandle), FALSE);
		}
		/*
		 * When in full-screen mode, the OS will always consider it to be the top of the display stack unless it is a dialog.
		 * This fix will give modal windows dialog-type priority if the parent is in full-screen, allowing it to be popped
		 * up in front of the full-screen window.
		 */
		if (parent != 0) {
			w_shell *shell;
			w_control_get_shell(W_CONTROL(parent), &shell);
			if (_W_SHELL(shell)->fullScreen)
				gtk_window_set_type_hint(GTK_WINDOW(shellHandle),
						GDK_WINDOW_TYPE_HINT_DIALOG);
		}
	} else {
		//_w_shell_update_modal(W_SHELL(control));
	}
	_W_SHELL(control)->showWithParent = visible;
	if (gtk_widget_get_mapped(shellHandle) == visible)
		return W_TRUE;
	if (visible) {
		if (parent != 0 && (_W_WIDGET(control)->style & W_SHEET)) {
			//_w_shell_center(control);
			if (!w_widget_is_ok(W_WIDGET(control)))
				return W_FALSE;
		}
		e.type = W_EVENT_SHOW;
		e.widget = W_WIDGET(control);
		e.platform_event = 0;
		e.data = 0;
		_w_widget_send_event(W_WIDGET(control), &e);
		if (!w_widget_is_ok(W_WIDGET(control)))
			return W_FALSE;

		gtk_widget_show(shellHandle);
		/*
		 * In order to ensure that the shell is visible
		 * and fully painted, dispatch events such as
		 * GDK_MAP and GDK_CONFIGURE, until the GDK_MAP
		 * event for the shell is received.
		 *
		 * Note that if the parent is minimized or withdrawn
		 * from the desktop, this should not be done since
		 * the shell not will be mapped until the parent is
		 * unminimized or shown on the desktop.
		 */
		_W_SHELL(control)->mapped = W_FALSE;
		gtk_widget_show(shellHandle);
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
		GdkWindow *enableWindow = priv->window_enable(W_WIDGET(control), priv);
		if (enableWindow != 0)
			gdk_window_raise(enableWindow);
		if (!w_widget_is_ok(W_WIDGET(control)))
			return W_FALSE;
		//if (!GTK_IS_PLUG (shellHandle)) {
		_w_toolkit_put_gdk_events(GDK_EXPOSE, GDK_FOCUS_CHANGE, GDK_CONFIGURE,
				GDK_MAP, GDK_UNMAP, GDK_NO_EXPOSE, GDK_WINDOW_STATE, -1);
		int iconic = W_FALSE;
		w_shell *shell;
		if (parent != 0) {
			w_control_get_shell(W_CONTROL(parent), &shell);
		} else
			shell = 0;
		do {
			/*
			 * This call to gdk_threads_leave() is a temporary work around
			 * to avoid deadlocks when gdk_threads_init() is called by native
			 * code outside of SWT (i.e AWT, etc). It ensures that the current
			 * thread leaves the GTK lock before calling the function below.
			 */
			gdk_threads_leave();
			g_main_context_iteration(0, FALSE);
			if (!w_widget_is_ok(W_WIDGET(control)))
				break;
			iconic = _W_SHELL(control)->minimized
					|| (shell != 0 && _W_SHELL(shell)->minimized);
		} while (!_W_SHELL(control)->mapped && !iconic);
		_w_toolkit_remove_gdk_events();
		if (!w_widget_is_ok(W_WIDGET(control)))
			return W_FALSE;
		if (!iconic) {
			//_w_control_update_0(control, W_TRUE, W_TRUE);
			if (!w_widget_is_ok(W_WIDGET(control)))
				return W_FALSE;
			//_w_shell_adjust_trim(W_SHELL(control));
		}
		//}//if (!GTK_IS_PLUG (shellHandle))
		_W_SHELL(control)->mapped = W_TRUE;

		if ((_W_WIDGET(control)->style & mask) != 0) {
			/*gdk_pointer_ungrab(gtk_widget_get_window(shellHandle),
			 GDK_CURRENT_TIME);*/
			gdk_pointer_ungrab(GDK_CURRENT_TIME);
		}
		_W_SHELL(control)->opened = W_TRUE;
		if (!_W_SHELL(control)->moved) {
			_W_SHELL(control)->moved = W_TRUE;
			w_rect location;
			W_CONTROL_GET_CLASS(control)->get_bounds(control, &location.pt,
					&location.sz);
			_W_SHELL(control)->oldbounds.x = location.x;
			_W_SHELL(control)->oldbounds.y = location.y;
			e.type = W_EVENT_MOVE;
			e.widget = W_WIDGET(control);
			e.platform_event = 0;
			e.data = 0;
			e.time = 0;
			_w_widget_send_event(W_WIDGET(control), &e);
			if (!w_widget_is_ok(W_WIDGET(control)))
				return W_FALSE;
		}
		if (!_W_SHELL(control)->resized) {
			_W_SHELL(control)->resized = 1;
			GtkAllocation allocation;
			GtkWidget *vboxHandle = _W_SHELL_VBOX(control);
			gtk_widget_get_allocation(vboxHandle, &allocation);
			int border = 0;
			if ((_W_WIDGET(control)->style
					& (W_NO_TRIM | W_BORDER | W_SHELL_TRIM)) == 0) {
				border = gtk_container_get_border_width(
						GTK_CONTAINER(shellHandle));
			}
			_W_SHELL(control)->oldbounds.width = allocation.width + 2 * border;
			_W_SHELL(control)->oldbounds.height = allocation.height
					+ 2 * border;
			e.type = W_EVENT_RESIZE;
			e.widget = W_WIDGET(control);
			e.platform_event = 0;
			e.data = 0;
			e.time = 0;
			_w_widget_send_event(W_WIDGET(control), &e);
			e.type = W_EVENT_LAYOUTDETECT;
			e.widget = W_WIDGET(control);
			e.platform_event = 0;
			e.data = 0;
			e.time = 0;
			_w_widget_send_event(W_WIDGET(control), &e);
		}
	} else {
		//_w_shell_fix_active_shell(W_SHELL(control));
		gtk_widget_hide(shellHandle);
		e.type = W_EVENT_HIDE;
		e.widget = W_WIDGET(control);
		e.platform_event = 0;
		e.data = 0;
		_w_widget_send_event(W_WIDGET(control), &e);
	}
	return W_TRUE;
}
void _w_shell_show_widget(w_control *control, _w_control_priv *priv) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
	if ((_W_WIDGET(control)->state & STATE_FOREIGN_HANDLE) != 0) {
		/*
		 * In case of foreign handles, activeShell might not be initialised as
		 * no focusIn events are generated on the window until the window loses
		 * and gain focus.
		 */
		if (gtk_window_is_active(GTK_WINDOW(shellHandle))) {
			gtk_toolkit->activeShell = W_SHELL(control);
			gtk_toolkit->activePending = W_TRUE;
		}
		GList *children = gtk_container_get_children(
				GTK_CONTAINER(shellHandle));
		GList *list = children;
		while (list != 0) {
			gtk_container_remove(GTK_CONTAINER(shellHandle),
					GTK_WIDGET(list->data));
			list = list->next;
		}
		g_list_free(children);
	}
	GtkWidget *vboxHandle = _W_SHELL_VBOX(control);
	gtk_widget_show_all(vboxHandle);
}
/*
 * signals
 */
gboolean _gtk_shell_destroy(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	_w_toolkit_remove_shell(_W_SHELL(widget));
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	gtk_widget_hide(shellHandle);
	if (gtk_toolkit->activeShell == W_SHELL(widget))
		gtk_toolkit->activeShell = 0;
	if (_W_SHELL(widget)->group != 0)
		g_object_unref(G_OBJECT(_W_SHELL(widget)->group));
	if (gtk_toolkit->shells == 0) {
		w_toolkit_post_quit(W_TOOLKIT(gtk_toolkit), EXIT_SUCCESS);
	}
	return W_FALSE;
}
gboolean _gtk_shell_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_configure_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_delete_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	//if (W_CONTROL_GET_CLASS(widget)->is_enabled(W_CONTROL(widget)) > 0) {
	_w_shell_close_widget(W_SHELL(widget), 0);
	//}
	return TRUE;
}
gboolean _gtk_shell_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_focus(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_move_focus(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_motion_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_size_allocate(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	GtkAllocation widgetAllocation;
	gtk_widget_get_allocation(shellHandle, &widgetAllocation);
	int width = widgetAllocation.width;
	int height = widgetAllocation.height;

//	Bug 474235: on Wayland gtk_size_allocate() is called more frequently, causing an
//  infinitely recursive resize call. This causes non-resizable Shells/Dialogs to
//  crash. Fix: only call resizeBounds() on resizable Shells.
#if GTK3
	if ((!(_W_SHELL(widget)->resized)
			|| _W_SHELL(widget)->oldbounds.width != width
			|| _W_SHELL(widget)->oldbounds.height != height)) { //Wayland
#endif
		_W_SHELL(widget)->oldbounds.width = width;
		_W_SHELL(widget)->oldbounds.height = height;
		_w_shell_resize_bounds(W_CONTROL(widget), width, height, W_TRUE, priv); //this is called to resize child widgets when the shell is resized.
	}
	return FALSE;
}
gboolean _gtk_shell_realize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_shell_window_state_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
void _w_shell_class_init(struct _w_shell_class *clazz) {
	_w_canvas_class_init(W_CANVAS_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SHELL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_shell_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_shell);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_shell);
	/*
	 * functions
	 */
	W_WIDGET_CLASS(clazz)->create = _w_shell_create;
	W_CONTROL_CLASS(clazz)->get_shell = _w_shell_get_shell;
	W_CONTROL_CLASS(clazz)->set_visible = _w_shell_set_visible;
	clazz->close = _w_shell_close;
	clazz->get_toolbar = _w_shell_get_toolbar;
	clazz->get_alpha = _w_shell_get_alpha;
	clazz->get_full_screen = _w_shell_get_full_screen;
	clazz->get_minimum_size = _w_shell_get_minimum_size;
	clazz->get_modified = _w_shell_get_modified;
	clazz->get_ime_input_mode = _w_shell_get_ime_input_mode;
	clazz->get_shells = _w_shell_get_shells;
	clazz->open = _w_shell_open;
	clazz->set_active = _w_shell_set_active;
	clazz->set_alpha = _w_shell_set_alpha;
	clazz->set_full_screen = _w_shell_set_full_screen;
	clazz->set_ime_input_mode = _w_shell_set_ime_input_mode;
	clazz->set_minimum_size = _w_shell_set_minimum_size;
	clazz->set_modified = _w_shell_set_modified;
	clazz->force_active = _w_shell_force_active;
	clazz->get_default_button = _w_shell_get_default_button;
	clazz->get_images = _w_shell_get_images;
	clazz->get_maximized = _w_shell_get_maximized;
	clazz->get_menu_bar = _w_shell_get_menu_bar;
	clazz->get_minimized = _w_shell_get_minimized;
	clazz->get_text = _w_shell_get_text;
	clazz->set_default_button = _w_shell_set_default_button;
	clazz->set_images = _w_shell_set_images;
	clazz->set_maximized = _w_shell_set_maximized;
	clazz->set_menu_bar = _w_shell_set_menu_bar;
	clazz->set_minimized = _w_shell_set_minimized;
	clazz->set_text = _w_shell_set_text;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->handle_top = _w_widget_hppp;
	_W_WIDGET_PRIV(priv)->create_handle = _w_shell_create_handle;
	_W_WIDGET_PRIV(priv)->check_style = _w_shell_check_style;
	_W_WIDGET_PRIV(priv)->hook_events = _w_shell_hook_events;
	priv->set_bounds_0 = _w_shell_set_bounds_0;
	priv->show_widget = _w_shell_show_widget;
	_W_COMPOSITE_PRIV(priv)->find_deferred_control =
			_w_shell_find_deferred_control;
	/*
	 * signals
	 */
	_gtk_signal *signals = _W_WIDGET_PRIV(priv)->signals;
	signals[SIGNAL_DESTROY] = _gtk_shell_destroy;
	signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_shell_button_press_event;
	signals[SIGNAL_CONFIGURE_EVENT] = _gtk_shell_configure_event;
	signals[SIGNAL_DELETE_EVENT] = _gtk_shell_delete_event;
	signals[SIGNAL_ENTER_NOTIFY_EVENT] = _gtk_shell_enter_notify_event;
	signals[SIGNAL_DRAW] = _gtk_shell_draw;
	signals[SIGNAL_FOCUS] = _gtk_shell_focus;
	signals[SIGNAL_FOCUS_IN_EVENT] = _gtk_shell_focus_in_event;
	signals[SIGNAL_FOCUS_OUT_EVENT] = _gtk_shell_focus_out_event;
	signals[SIGNAL_LEAVE_NOTIFY_EVENT] = _gtk_shell_leave_notify_event;
	signals[SIGNAL_MOVE_FOCUS] = _gtk_shell_move_focus;
	signals[SIGNAL_MOTION_NOTIFY_EVENT] = _gtk_shell_motion_notify_event;
	signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_shell_key_press_event;
	signals[SIGNAL_SIZE_ALLOCATE] = _gtk_shell_size_allocate;
	signals[SIGNAL_REALIZE] = _gtk_shell_realize;
	signals[SIGNAL_WINDOW_STATE_EVENT] = _gtk_shell_window_state_event;
}
