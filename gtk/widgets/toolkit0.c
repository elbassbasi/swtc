/*
 * Name:        toolkit0.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
#include "dnd.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
_w_toolkit *gtk_toolkit = 0;
w_app *app = 0;
void w_app_init_thread(w_app *app) {
	app->app->platformToolkit = W_TOOLKIT(gtk_toolkit);
}
w_toolkit* w_app_get_platform_toolkit(w_app *app) {
	return app->app->platformToolkit;
}
wresult w_app_get_current_directory(w_app *app, w_alloc alloc, void *userdata,
		int enc) {
	/*if (app->currentdir == 0) {
	 app->app->currentdir = getcwd(NULL, 0);
	 }
	 return app->app->currentdir;*/
	return W_FALSE;
}
void w_app_create(w_app *_app, int argc, char **argv) {
	if (app != 0) {
		_app->app = 0;
		return;
	}
	if (gtk_toolkit == 0) {
		const int toolkit_size = sizeof(_w_toolkit);
		int page_size = sysconf(_SC_PAGE_SIZE);
		int total_size = (toolkit_size & ~(page_size - 1)) + 5 * page_size;
		const int tmp_total = total_size - toolkit_size;
		gtk_toolkit = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
		if (gtk_toolkit == MAP_FAILED || gtk_toolkit == 0) {
			fprintf(stderr, "Error : Do not initialize application\n");
			exit(EXIT_FAILURE);
			return;
		}
		gtk_toolkit->total_size = total_size;
		gtk_toolkit->tmp_reserved = page_size;
		gtk_toolkit->tmpaddr_sz_bits = (tmp_total - gtk_toolkit->tmp_reserved)
				/ _PAGES_SIZE;
		gtk_toolkit->tmpaddr_sz = gtk_toolkit->tmpaddr_sz_bits / 64;
		if (gtk_toolkit->tmpaddr_sz_bits % 64 != 0)
			gtk_toolkit->tmpaddr_sz++;
		gtk_toolkit->tmp_total = tmp_total
				- gtk_toolkit->tmpaddr_sz * sizeof(wuint64);
		_w_toolkit_init(gtk_toolkit);
		_app->app = &gtk_toolkit->app;
	}
	app = _app;
	_app->app->argc = argc;
	_app->app->argv = argv;
	w_app_init_thread(_app);
}
w_app* w_app_get() {
	return app;
}
void w_app_dispose(w_app *app) {
	w_app_dispose_all();
	_w_toolkit_dispose((w_disposable*) gtk_toolkit);
	munmap(gtk_toolkit, gtk_toolkit->total_size);
}
wresult _w_toolkit_check_widget(w_toolkit *toolkit, w_widget *widget) {
	if (pthread_equal(pthread_self(), (pthread_t)
	_W_TOOLKIT(toolkit)->thread.id)) {
		return W_TRUE;
	} else {
		return W_ERROR_THREAD_INVALID_ACCESS;
	}
}
wresult _w_toolkit_check_widgetdata(w_toolkit *toolkit,
		w_widgetdata *widgetdata) {
	if (pthread_equal(pthread_self(), (pthread_t)
	_W_TOOLKIT(toolkit)->thread.id)) {
		return W_TRUE;
	} else {
		return W_ERROR_THREAD_INVALID_ACCESS;
	}
}
wresult _w_toolkit_init_class(w_toolkit *toolkit, wushort clazz_id,
		struct _w_widget_class *clazz) {
	if (clazz_id < _W_CLASS_LAST) {
		w_widget_init_class fun = gtk_toolkit_classes_init[clazz_id];
		if (fun != 0) {
			fun(toolkit, clazz_id, clazz);
			return W_TRUE;
		}
	}
	return W_FALSE;
}
struct _w_widget_class* _w_toolkit_get_class(w_toolkit *toolkit,
		wushort clazz_id) {
	if (clazz_id >= _W_CLASS_LAST)
		return 0;
	else
		return _W_TOOLKIT(toolkit)->classes.classes[clazz_id];
}
w_theme* _w_toolkit_get_theme(w_toolkit *toolkit) {
	_w_toolkit *_toolkit = _W_TOOLKIT(toolkit);
	if (_toolkit->theme == 0) {
		return (w_theme*) &_toolkit->gtktheme;
	}
	return _toolkit->theme;
}
wresult _w_toolkit_beep(w_toolkit *toolkit) {
	gdk_beep();
	if (!_W_TOOLKIT(toolkit)->ISX11) {
		gdk_flush();
	} else {
		gdk_flush();
		/*void* xDisplay = gdk_x11_display_get_xdisplay(
		 gdk_display_get_default());
		 XFlush(xDisplay);*/
	}
	return W_TRUE;
}
w_shell* _w_toolkit_get_active_shell(w_toolkit *toolkit) {
	return _W_TOOLKIT(toolkit)->activeShell;
}
w_menu* _w_toolkit_get_menubar(w_toolkit *toolkit) {
	return 0;
}
wresult _w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *rect) {
	rect->x = 0;
	rect->y = 0;
	rect->width = gdk_screen_width();
	rect->height = gdk_screen_height();
	return W_TRUE;
}
wresult _w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea) {
	clientArea->x = 0;
	clientArea->y = 0;
	clientArea->width = gdk_screen_width();
	clientArea->height = gdk_screen_height();
	return W_TRUE;
}
w_control* _w_toolkit_get_cursor_control(w_toolkit *toolkit) {
	int x = 0, y = 0;
	GtkWidget *handle = 0;
	void *user_data = 0;
	GdkWindow *window;
#if GTK3
	GdkDisplay *display = gdk_display_get_default();
	GdkDeviceManager *device_manager = gdk_display_get_device_manager(display);
	GdkDevice *device = gdk_device_manager_get_client_pointer(device_manager);
	window = gdk_device_get_window_at_position(device, &x, &y);
#else
		window = gdk_window_at_pointer (&x, &y);
#endif
	if (window != 0) {
		gdk_window_get_user_data(window, &user_data);
		handle = (GtkWidget*) user_data;
	} else {
		/*
		 * Feature in GTK. gdk_window_at_pointer() will not return a window
		 * if the pointer is over a foreign embedded window. The fix is to use
		 * XQueryPointer to find the containing GDK window.
		 */
		if (!_W_TOOLKIT(toolkit)->ISX11)
			return 0;
		/*gdk_error_trap_push();
		 int unusedInt = 0;
		 void* unusedPtr = 0, buffer = 0;
		 void* xWindow, xParent = XDefaultRootWindow(xDisplay);
		 do {
		 if (XQueryPointer(xDisplay, xParent, unusedPtr, buffer,
		 unusedInt, unusedInt, unusedInt, unusedInt, unusedInt)
		 == 0) {
		 handle = 0;
		 break;
		 }
		 if ((xWindow = buffer[0]) != 0) {
		 xParent = xWindow;
		 GdkWindow* gdkWindow = 0;
		 if (GTK_VERSION >= VERSION (2, 24, 0)) {
		 gdkWindow = gdk_x11_window_lookup_for_display(
		 gdk_display_get_default(), xWindow);
		 } else {
		 gdkWindow = gdk_window_lookup(xWindow);
		 }
		 if (gdkWindow != 0) {
		 gdk_window_get_user_data(gdkWindow, user_data);
		 if (user_data[0] != 0)
		 handle = user_data[0];
		 }
		 }
		 } while (xWindow != 0);
		 gdk_error_trap_pop();*/
	}
	if (handle == 0)
		return 0;
	do {
		w_widget *widget = _w_widget_find_control(handle);
		if (widget != 0 && w_widget_class_id(widget) >= _W_CLASS_CONTROL) {
			if (w_control_is_enabled(W_CONTROL(widget)))
				return W_CONTROL(widget);
		}
	} while ((handle = gtk_widget_get_parent(handle)) != 0);
	return 0;
}
wresult _w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location) {
	_gdk_window_get_device_position(0, &location->x, &location->y, 0);
	return W_TRUE;
}
size_t _w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	if (sizes != 0) {
		if (length >= 1) {
			sizes[0].width = 16;
			sizes[0].height = 16;
		}
		if (length >= 2) {
			sizes[1].width = 32;
			sizes[1].height = 32;
		}
	}
	return 2;
}
int _w_toolkit_get_dismissal_alignment(w_toolkit *toolkit) {
	int buffer = 0;
	GtkSettings *settings = gtk_settings_get_default();
	g_object_get(settings, "gtk-alternative-button-order", &buffer, NULL);
	return buffer == 1 ? W_LEFT : W_RIGHT;
}
int _w_toolkit_get_double_click_time(w_toolkit *toolkit) {
	GtkSettings *settings = gtk_settings_get_default();
	int buffer = 0;
	g_object_get(settings, "gtk-double-click-time", &buffer, NULL);
	return buffer;
}
w_control* _w_toolkit_get_focus_control(w_toolkit *toolkit) {
	w_control *focusControl = _W_TOOLKIT(toolkit)->focusControl;
	if (focusControl != 0 && w_widget_is_ok(W_WIDGET(focusControl))) {
		return focusControl;
	}
	w_shell *activeShell = _W_TOOLKIT(toolkit)->activeShell;
	if (activeShell == 0)
		return 0;
	GtkWidget *shellHandle = _W_SHELL_HANDLE(activeShell);
	GtkWidget *handle = gtk_window_get_focus(GTK_WINDOW(shellHandle));
	if (handle == 0)
		return 0;
	do {
		w_widget *widget = g_object_get_qdata(G_OBJECT(handle),
		_W_TOOLKIT(toolkit)->quark[0]);
		if (widget != 0 && w_widget_class_id(widget) >= _W_CLASS_CONTROL) {
			return w_control_is_enabled(W_CONTROL(widget)) ?
					W_CONTROL(widget) : 0;
		}
	} while ((handle = gtk_widget_get_parent(handle)) != 0);
	return 0;
}
wresult _w_toolkit_get_high_contrast(w_toolkit *toolkit) {
	return W_FALSE;
}
int _w_toolkit_get_icon_depth(w_toolkit *toolkit) {
	if (GTK_VERSION >= VERSION(2, 22, 0)) {
		return gdk_visual_get_depth(gdk_visual_get_system());
	} else {
		GdkVisual *visual = gdk_visual_get_system();
		return gdk_visual_get_depth(visual);
	}
}
size_t _w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	if (sizes != 0) {
		if (length >= 1) {
			sizes[0].width = 16;
			sizes[0].height = 16;
		}
		if (length >= 2) {
			sizes[1].width = 32;
			sizes[1].height = 32;
		}
	}
	return 2;
}
void _w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells) {
	w_iterator_link_create(shells, &_W_SHELL(gtk_toolkit->shells)->shells_link,
			gtk_toolkit->shells, gtk_toolkit->shells_count, 0, 0);
}
wresult w_iterator_filter_shell_parent(void *userdata, void *in, void *out) {
	if (_W_WIDGET(in)->parent == W_WIDGET(userdata)) {
		if (out != 0) {
			*((w_shell**) out) = (w_shell*) in;
		}
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
void _w_toolkit_get_shells_from_parent(w_shell *shell, w_iterator *iterator) {
	w_iterator_link_create(iterator,
			&_W_SHELL(gtk_toolkit->shells)->shells_link, gtk_toolkit->shells,
			-1, w_iterator_filter_shell_parent, shell);
}
w_color _w_toolkit_get_system_color(w_toolkit *toolkit, wuint id) {
	w_theme *theme = (w_theme*) &_W_TOOLKIT(toolkit)->gtktheme;
	w_color color;
	w_theme_get_color(theme, id, &color);
	return color;
}
wuchar cursorshapes[] = { GDK_LEFT_PTR, GDK_WATCH, GDK_CROSS, 0,
		GDK_QUESTION_ARROW, GDK_FLEUR, GDK_SIZING, GDK_DOUBLE_ARROW, GDK_SIZING,
		GDK_SB_H_DOUBLE_ARROW, GDK_TOP_SIDE, GDK_BOTTOM_SIDE, GDK_RIGHT_SIDE,
		GDK_LEFT_SIDE, GDK_TOP_RIGHT_CORNER, GDK_BOTTOM_RIGHT_CORNER,
		GDK_BOTTOM_LEFT_CORNER, GDK_TOP_LEFT_CORNER, GDK_SB_UP_ARROW, GDK_XTERM,
		GDK_X_CURSOR, GDK_HAND2 };
w_cursor* _w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint style) {
	if (style <= W_CURSOR_HAND) {
		_w_cursor *cursor = &_W_TOOLKIT(toolkit)->cursors[style];
		if (cursor->handle == 0) {
			GdkCursorType shape = cursorshapes[style];
			if (shape == 0 && style == W_CURSOR_APPSTARTING) {
			} else {
				_W_CURSOR(cursor)->handle = gdk_cursor_new_for_display(
						gdk_display_get_default(), shape);
			}
		}
		return W_CURSOR(cursor);
	} else
		return 0;
}
w_font* _w_toolkit_get_system_font(w_toolkit *toolkit) {
	if (_W_TOOLKIT(toolkit)->system_font == 0) {
		/* Initialize the system font slot */
		PangoFontDescription *defaultFont = 0;
#if GTK3
		GtkStyleContext *context = gtk_widget_get_style_context(
		_W_TOOLKIT(toolkit)->gtktheme.handle[GTK_THEME_HANDLE_SHELL]);
		if ((GTK_VERSION < VERSION(3, 8, 0))) {
			defaultFont = (PangoFontDescription*) gtk_style_context_get_font(
					context, GTK_STATE_FLAG_NORMAL);
		} else if (GTK_VERSION >= VERSION(3, 18, 0)) {
			gtk_style_context_save(context);
			gtk_style_context_set_state(context, GTK_STATE_FLAG_NORMAL);
			gtk_style_context_get(context, GTK_STATE_FLAG_NORMAL, "font",
					&defaultFont, NULL);
			gtk_style_context_restore(context);
		} else {
			gtk_style_context_get(context, GTK_STATE_FLAG_NORMAL, "font",
					&defaultFont, NULL);
		}
#endif
		_W_TOOLKIT(toolkit)->system_font = defaultFont;
	}
	return (w_font*) (_W_TOOLKIT(toolkit)->system_font);
}

wresult _w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
		w_image **image) {
	const char *image_name = 0;
	_w_image *_image = 0;
	switch (id) {
	case W_ICON_ERROR:
		image_name = "gtk-dialog-error";
		_image = &_W_TOOLKIT(toolkit)->systemImages[0];
		break;
	case W_ICON_INFORMATION:
	case W_ICON_WORKING:
		image_name = "gtk-dialog-info";
		_image = &_W_TOOLKIT(toolkit)->systemImages[1];
		break;
	case W_ICON_QUESTION:
		image_name = "gtk-dialog-question";
		_image = &_W_TOOLKIT(toolkit)->systemImages[2];
		break;
	case W_ICON_WARNING:
		image_name = "gtk-dialog-warning";
		_image = &_W_TOOLKIT(toolkit)->systemImages[3];
		break;
	}
	return W_FALSE;
}
w_menu* _w_toolkit_get_system_menu(w_toolkit *toolkit) {
	return 0;
}
w_taskbar* _w_toolkit_get_system_taskbar(w_toolkit *toolkit) {
	return 0;
}
w_tray* _w_toolkit_get_system_tray(w_toolkit *toolkit) {
	w_tray *tray = &_W_TOOLKIT(toolkit)->tray;
	if (w_widget_is_ok(W_WIDGET(tray)))
		return tray;
	_w_widget_create(W_WIDGET(tray), toolkit, 0, 0, _W_CLASS_TRAY, 0);
	return tray;
}
w_thread* _w_toolkit_get_thread(w_toolkit *toolkit) {
	return &_W_TOOLKIT(toolkit)->thread;
}
wuint64 _w_toolkit_get_thread_id(w_toolkit *toolkit) {
	return _W_TOOLKIT(toolkit)->thread.id;
}
wresult _w_toolkit_get_touch_enabled(w_toolkit *toolkit) {
	return W_FALSE;
}
void _w_toolkit_get_window_origin(w_control *control, w_point *origin) {
	if (w_widget_class_id(W_WIDGET(control)) == _W_CLASS_SHELL) {
		if (!_W_SHELL(control)->mapped) {
			/*
			 * Special case: The handle attributes are not initialized until the
			 * shell is made visible, so gdk_window_get_origin () always returns {0, 0}.
			 *
			 * Once the shell is realized, gtk_window_get_position () includes
			 * window trims etc. from the window manager. That's why getLocation ()
			 * is not safe to use for coordinate mappings after the shell has been made visible.
			 */
			w_control_get_bounds(control, origin, 0);
			return;
		}
	}
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
	GdkWindow *window = priv->window_event(W_WIDGET(control), priv);
	int x = 0;
	int y = 0;
	gdk_window_get_origin(window, &x, &y);
	origin->x = x;
	origin->y = y;
	return;
}
wresult _w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
		w_point *result, w_point *point) {
	if (from != 0 && !w_widget_is_ok(W_WIDGET(from)))
		return W_ERROR_INVALID_ARGUMENT;
	if (to != 0 && !w_widget_is_ok(W_WIDGET(to)))
		return W_ERROR_INVALID_ARGUMENT;
	memcpy(result, point, sizeof(w_point));
	if (from == to)
		return TRUE;
	w_point origin;
	if (from != 0) {
		_w_toolkit_get_window_origin(from, &origin);
		if ((_W_WIDGET(from)->style & W_MIRRORED) != 0) {
			_w_control_priv *priv = _W_CONTROL_GET_PRIV(from);
			result->x = priv->get_client_width(from, priv) - result->x;
		}
		result->x += origin.x;
		result->y += origin.y;
	}
	if (to != 0) {
		_w_toolkit_get_window_origin(to, &origin);
		result->x -= origin.x;
		result->y -= origin.y;
		if ((_W_WIDGET(to)->style & W_MIRRORED) != 0) {
			_w_control_priv *priv = _W_CONTROL_GET_PRIV(to);
			result->x = priv->get_client_width(to, priv) - result->x;
		}
	}
	return TRUE;
}
wresult _w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
		w_rect *result, w_rect *rectangle) {
	if (from != 0 && !w_widget_is_ok(W_WIDGET(from)))
		return W_ERROR_INVALID_ARGUMENT;
	if (to != 0 && !w_widget_is_ok(W_WIDGET(to)))
		return W_ERROR_INVALID_ARGUMENT;
	memcpy(result, rectangle, sizeof(w_rect));
	if (from == to)
		return TRUE;
	int fromRTL = FALSE, toRTL = FALSE;
	w_point origin;
	if (from != 0) {
		_w_toolkit_get_window_origin(from, &origin);
		fromRTL = (_W_WIDGET(from)->style & W_MIRRORED) != 0;
		if (fromRTL) {
			_w_control_priv *priv = _W_CONTROL_GET_PRIV(from);
			result->x = priv->get_client_width(from, priv) - result->x;
		}
		result->x += origin.x;
		result->y += origin.y;
	}
	if (to != 0) {
		_w_toolkit_get_window_origin(to, &origin);
		result->x -= origin.x;
		result->y -= origin.y;
		toRTL = (_W_WIDGET(to)->style & W_MIRRORED) != 0;
		if (toRTL) {
			_w_control_priv *priv = _W_CONTROL_GET_PRIV(to);
			result->x = priv->get_client_width(to, priv) - result->x;
		}
	}
	if (fromRTL != toRTL)
		result->x -= result->width;
	return TRUE;
}
wresult _w_toolkit_post(w_toolkit *toolkit, w_event *event) {
	return W_TRUE;
}
wresult _w_toolkit_post_quit(w_toolkit *toolkit, int quit) {
	_W_TOOLKIT(toolkit)->exit_code = quit;
	_W_TOOLKIT(toolkit)->exit_loop = 1;
	return W_TRUE;
}
wresult _w_toolkit_dispatch(w_toolkit *toolkit) {
	if (gtk_toolkit->fds == 0) {
		gtk_toolkit->allocated_nfds = sizeof(gtk_toolkit->fds_tmp)
				/ sizeof(gtk_toolkit->fds_tmp[0]);
		gtk_toolkit->fds = gtk_toolkit->fds_tmp;
	}
	int max_priority = 0;
	int timeout = 0;
	GMainContext *context = g_main_context_default();
	gboolean result = FALSE;
	do {
		if (g_main_context_acquire(context)) {
			result = g_main_context_prepare(context, &max_priority);
			int nfds;
			while ((nfds = g_main_context_query(context, max_priority, &timeout,
					gtk_toolkit->fds, gtk_toolkit->allocated_nfds))
					> gtk_toolkit->allocated_nfds) {
				if (gtk_toolkit->fds != gtk_toolkit->fds_tmp) {
					g_free(gtk_toolkit->fds);
				}
				gtk_toolkit->allocated_nfds = nfds;
				gtk_toolkit->fds = g_malloc(nfds * sizeof(GPollFD));
			}
			GPollFunc poll = g_main_context_get_poll_func(context);
			if (poll != 0) {
				if (nfds > 0 || timeout != 0) {
					/*
					 * Bug in GTK. For some reason, g_main_context_wakeup() may
					 * fail to wake up the UI thread from the polling function.
					 * The fix is to sleep for a maximum of 50 milliseconds.
					 */
					if (timeout < 0)
						timeout = 50;
					gtk_toolkit->wake = FALSE;
					poll(gtk_toolkit->fds, nfds, timeout);
				}
			}
			g_main_context_check(context, max_priority, gtk_toolkit->fds, nfds);
			g_main_context_release(context);
		}
	} while (!result /*&& getMessageCount () == 0*/&& !gtk_toolkit->wake);
	gtk_toolkit->wake = FALSE;
	if (gtk_toolkit->fds != gtk_toolkit->fds_tmp) {
		g_free(gtk_toolkit->fds);
		gtk_toolkit->fds = 0;
	}
	return TRUE;
}
wresult _w_toolkit_event_free_memory(w_toolkit *toolkit) {
	w_event _e;
	w_widget *widget = ((_w_toolkit*) toolkit)->widget_free, *next;
	while (widget != 0) {
		w_widget_post_event_proc _proc =
				(w_widget_post_event_proc) widget->post_event;
		next = (w_widget*) widget->handle; //widget = widget->next
		if (_proc != 0) {
			_e.type = W_EVENT_FREE_MEMORY;
			_e.platform_event = 0;
			_e.data = 0;
			_e.widget = widget;
			_proc(widget, &_e);
		} else {
			free(widget);
		}
		widget = next; //widget = widget->next
	}
	((_w_toolkit*) toolkit)->widget_free = 0;
	return W_TRUE;
}
wresult _w_toolkit_read(w_toolkit *toolkit) {
	gdk_threads_leave();
	g_main_context_iteration(0, FALSE);
	return _w_toolkit_event_free_memory(toolkit);
}
wresult _w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *location) {
	GdkDisplay *gdkDisplay = gdk_display_get_default();
	GdkDeviceManager *gdkDeviceManager = gdk_display_get_device_manager(
			gdkDisplay);
	GdkScreen *gdkScreen = gdk_screen_get_default();
	GdkDevice *gdkPointer = gdk_device_manager_get_client_pointer(
			gdkDeviceManager);
	gdk_device_warp(gdkPointer, gdkScreen, location->x, location->y);
	return TRUE;
}
wresult _w_toolkit_set_theme(w_toolkit *toolkit, w_theme *theme) {
	_W_TOOLKIT(toolkit)->theme = theme;
	return W_TRUE;
}
int _w_toolkit_run(w_toolkit *toolkit, w_shell *shell) {
	_W_TOOLKIT(toolkit)->exit_loop = 0;
	do {
		_w_toolkit_read(toolkit);
		_w_toolkit_dispatch(toolkit);
		if (shell != 0) {
			if (w_widget_is_ok(W_WIDGET(shell)) <= 0) {
				_w_toolkit_event_free_memory(toolkit);
				break;
			}
		}
	} while (_W_TOOLKIT(toolkit)->exit_loop == 0);
	return _W_TOOLKIT(toolkit)->exit_code;
}
gboolean _w_toolkit_async_exec_GSourceFunc(gpointer user_data) {
	threads_idle *funcs = (threads_idle*) user_data;
	funcs->func(funcs->args);
	_w_toolkit_delete_pages(funcs, sizeof(threads_idle));
	return FALSE;
}
gboolean _w_toolkit_sync_exec_GSourceFunc(gpointer user_data) {
	threads_idle *funcs = (threads_idle*) user_data;
	funcs->func(funcs->args);
	funcs->signalled[0] = 1;
	_w_toolkit_delete_pages(funcs, sizeof(threads_idle));
	pthread_cond_broadcast(&gtk_toolkit->condition);
	return FALSE;
}
wresult _w_toolkit_exec(w_toolkit *toolkit, w_thread_start function, void *args,
		int sync, wuint ms) {
	pthread_t thread = pthread_self();
	if (pthread_equal(thread, _W_TOOLKIT(toolkit)->thread.id) && ms != -1) {
		function(args);
		return TRUE;
	}
	threads_idle *funcs = (threads_idle*) _w_toolkit_new_pages(
			sizeof(threads_idle));
	if (funcs == 0)
		return W_ERROR_NO_MEMORY;
	volatile wuint signalled = 0;
	funcs->func = function;
	funcs->args = args;
	funcs->signalled = &signalled;
	GSourceFunc sourceFunc;
	if (ms == -1) {
		if (sync) {
			sourceFunc = _w_toolkit_sync_exec_GSourceFunc;
		} else {
			sourceFunc = _w_toolkit_async_exec_GSourceFunc;
		}
		gdk_threads_add_idle(sourceFunc, funcs);
	} else {
		g_timeout_add(ms, _w_toolkit_async_exec_GSourceFunc, funcs);
	}
	if (sync) {
		while (signalled != 0) {
			pthread_cond_wait(&gtk_toolkit->condition,
					&gtk_toolkit->condition_mutex);
		}
	}
	return TRUE;
}
wresult _w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	return _w_toolkit_exec(toolkit, function, args, W_FALSE, -1);
}
wresult _w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	return _w_toolkit_exec(toolkit, function, args, W_TRUE, -1);
}
wresult _w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args) {
	return _w_toolkit_exec(toolkit, function, args, W_FALSE, milliseconds);
}
wresult _w_toolkit_update(w_toolkit *toolkit) {
	//flushExposes (0, true);
	/*
	 * Do not send expose events on GTK 3.16.0+
	 * It's worth checking whether can be removed on all GTK 3 versions.
	 */
	if (GTK_VERSION < VERSION(3, 16, 0)) {
		gdk_window_process_all_updates();
	}
	return W_TRUE;
}
wresult _w_toolkit_wake(w_toolkit *toolkit) {
	if (w_thread_get_current_id() == w_toolkit_get_thread_id(toolkit))
		return W_TRUE;
	g_main_context_wakeup(0);
	_W_TOOLKIT(toolkit)->wake = TRUE;
	return W_TRUE;
}
void _w_toolkit_class_init(_w_toolkit *toolkit) {
	struct _w_toolkit_class *clazz = &toolkit->class_toolkit;
	((w_disposable*) toolkit)->clazz = (struct w_disposable_class*) clazz;
	clazz->disposable.dispose = _w_toolkit_dispose;
	clazz->check_widget = _w_toolkit_check_widget;
	clazz->check_widgetdata = _w_toolkit_check_widgetdata;
	clazz->init_class = _w_toolkit_init_class;
	clazz->get_class = _w_toolkit_get_class;
	clazz->async_exec = _w_toolkit_async_exec;
	clazz->beep = _w_toolkit_beep;
	clazz->get_active_shell = _w_toolkit_get_active_shell;
	clazz->get_menubar = _w_toolkit_get_menubar;
	clazz->get_bounds = _w_toolkit_get_bounds;
	clazz->get_client_area = _w_toolkit_get_client_area;
	clazz->get_cursor_control = _w_toolkit_get_cursor_control;
	clazz->get_cursor_location = _w_toolkit_get_cursor_location;
	clazz->get_cursor_sizes = _w_toolkit_get_cursor_sizes;
	clazz->get_dismissal_alignment = _w_toolkit_get_dismissal_alignment;
	clazz->get_double_click_time = _w_toolkit_get_double_click_time;
	clazz->get_focus_control = _w_toolkit_get_focus_control;
	clazz->get_high_contrast = _w_toolkit_get_high_contrast;
	clazz->get_icon_depth = _w_toolkit_get_icon_depth;
	clazz->get_icon_sizes = _w_toolkit_get_icon_sizes;
	clazz->get_shells = _w_toolkit_get_shells;
	clazz->get_system_color = _w_toolkit_get_system_color;
	clazz->get_system_cursor = _w_toolkit_get_system_cursor;
	clazz->get_system_font = _w_toolkit_get_system_font;
	clazz->get_system_image = _w_toolkit_get_system_image;
	clazz->get_system_menu = _w_toolkit_get_system_menu;
	clazz->get_system_taskbar = _w_toolkit_get_system_taskbar;
	clazz->get_system_tray = _w_toolkit_get_system_tray;
	clazz->get_theme = _w_toolkit_get_theme;
	clazz->get_thread = _w_toolkit_get_thread;
	clazz->get_thread_id = _w_toolkit_get_thread_id;
	clazz->get_touch_enabled = _w_toolkit_get_touch_enabled;
	clazz->map_0 = _w_toolkit_map_0;
	clazz->map_1 = _w_toolkit_map_1;
	clazz->post = _w_toolkit_post;
	clazz->post_quit = _w_toolkit_post_quit;
	clazz->dispatch = _w_toolkit_dispatch;
	clazz->set_cursor_location = _w_toolkit_set_cursor_location;
	clazz->set_theme = _w_toolkit_set_theme;
	clazz->run = _w_toolkit_run;
	clazz->read = _w_toolkit_read;
	clazz->sync_exec = _w_toolkit_sync_exec;
	clazz->timer_exec = _w_toolkit_timer_exec;
	clazz->update = _w_toolkit_update;
	clazz->wake = _w_toolkit_wake;
	clazz->messagebox_open = _w_messagebox_open;
	clazz->open_clipboard = _w_toolkit_open_clipboard;
	clazz->register_type = _w_toolkit_register_type;
}
