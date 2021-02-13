/*
 * Name:        toolkit0.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
_w_toolkit *gtk_toolkit = 0;
w_toolkit* w_toolkit_get_platform(w_app *app) {
	if (gtk_toolkit == 0) {
		const int total_size = 0x10000;
		const int toolkit_size = sizeof(_w_toolkit);
		const int tmp_size = total_size - toolkit_size;
		gtk_toolkit = calloc(1, total_size);
		if (gtk_toolkit == 0) {
			fprintf(stderr, "Error : Do not initialize toolkit\n");
			exit(EXIT_FAILURE);
			return 0;
		}
		gtk_toolkit->tmp_alloc = tmp_size;
		_w_toolkit_init(gtk_toolkit);
	}
	return (w_toolkit*) gtk_toolkit;
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
struct _w_widget_class* _w_toolkit_get_class(w_toolkit *toolkit,
		wushort clazz_id) {
	if (clazz_id >= _W_CLASS_LAST)
		return 0;
	else
		return _W_TOOLKIT(toolkit)->classes[clazz_id];
}
w_theme* _w_toolkit_get_theme(w_toolkit *toolkit) {
	return _W_TOOLKIT(toolkit)->theme;
}
wresult _w_toolkit_beep(w_toolkit *toolkit) {
	return W_FALSE;
}
w_shell* _w_toolkit_get_active_shell(w_toolkit *toolkit) {
	return W_FALSE;
}
w_menu* _w_toolkit_get_menubar(w_toolkit *toolkit) {
	return 0;
}
wresult _w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *rect) {
	return W_TRUE;
}
wresult _w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea) {
	return W_TRUE;
}
w_control* _w_toolkit_get_cursor_control(w_toolkit *toolkit) {
	return 0;
}
wresult _w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location) {
	return W_TRUE;
}
size_t _w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	if (sizes != 0 && length > 0) {
	}
	return 1;
}
int _w_toolkit_get_dismissal_alignment(w_toolkit *toolkit) {
	return W_LEFT;
}
int _w_toolkit_get_double_click_time(w_toolkit *toolkit) {
	return W_FALSE;
}
w_control* _w_toolkit_get_focus_control(w_toolkit *toolkit) {
	return 0;
}
wresult _w_toolkit_get_high_contrast(w_toolkit *toolkit) {
	return W_FALSE;
}
int _w_toolkit_get_icon_depth(w_toolkit *toolkit) {
	return W_FALSE;
}
size_t _w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	if (sizes != 0) {
		if (length >= 1) {
		}
		if (length >= 2) {
		}
	}
	return 2;
}
_w_shell* _w_shells_iterator_find_next(_w_shell *parent, _w_shell *current) {
	if (parent == 0)
		return current->next;
	else {
		_w_shell *s = current->next;
		while (s != 0) {
			if (_W_CONTROL(s)->parent == W_COMPOSITE(parent))
				return s;
			s = s->next;
		}
		return 0;
	}
}
_w_shell* _w_shells_iterator_find_first(_w_shell *parent) {
	_w_shell *s = gtk_toolkit->shells;
	while (s != 0) {
		if (_W_CONTROL(s)->parent == W_COMPOSITE(parent))
			return s;
		s = s->next;
	}
	return 0;
}
wresult _w_shells_iterator_next(w_iterator *it, void *obj) {
	_w_shells_iterator *iter = (_w_shells_iterator*) it;
	if (iter->current != 0) {
		*((_w_shell**) obj) = iter->current;
		iter->current = _w_shells_iterator_find_next(iter->parent,
				iter->current);
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_shells_iterator_reset(w_iterator *it) {
	_w_shells_iterator *iter = (_w_shells_iterator*) it;
	if (iter->parent == 0)
		iter->current = gtk_toolkit->shells;
	else
		iter->current = _w_shells_iterator_find_first(iter->parent);
	return W_TRUE;
}
wresult _w_shells_iterator_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_shells_iterator_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_shells_iterator_count(w_iterator *it) {
	_w_shells_iterator *iter = (_w_shells_iterator*) it;
	if (iter->parent == 0)
		return gtk_toolkit->shells_count;
	else {
		if (iter->count == -1) {
			_w_shell *s = gtk_toolkit->shells;
			size_t count = 0;
			while (s != 0) {
				if (_W_CONTROL(s)->parent == W_COMPOSITE(iter->parent))
					count++;
				s = s->next;
			}
			iter->count = count;
		}
		return iter->count;
	}
}
_w_iterator_class _w_shells_iterator_class = { //
		_w_shells_iterator_close, //
				_w_shells_iterator_next, //
				_w_shells_iterator_reset, //
				_w_shells_iterator_remove, //
				_w_shells_iterator_count };
void _w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells) {
	_w_shells_iterator *iter = (_w_shells_iterator*) shells;
	iter->base.clazz = &_w_shells_iterator_class;
	iter->current = gtk_toolkit->shells;
	iter->parent = 0;
	iter->count = -1;
}
void _w_toolkit_get_shells_from_parent(w_shell *shell, w_iterator *iterator) {
	_w_shells_iterator *iter = (_w_shells_iterator*) iterator;
	iter->base.clazz = &_w_shells_iterator_class;
	iter->current = _w_shells_iterator_find_first(iter->parent);
	iter->parent = _W_SHELL(shell);
	iter->count = -1;
}
w_color _w_toolkit_get_system_color(w_toolkit *toolkit, wuint id) {
	w_theme *theme = _W_TOOLKIT(toolkit)->theme;
	return w_theme_get_color(theme, id);
}
w_cursor* _w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint style) {
	if (style <= W_CURSOR_HAND) {
		return W_CURSOR(&_W_TOOLKIT(toolkit)->cursors[style]);
	} else
		return 0;
}
w_font* _w_toolkit_get_system_font(w_toolkit *toolkit) {
	w_theme *theme = _W_TOOLKIT(toolkit)->theme;
	return w_theme_get_font(theme);
}
wresult _w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
		w_image **image) {
	return W_FALSE;
}
w_menu* _w_toolkit_get_system_menu(w_toolkit *toolkit) {
	return 0;
}
w_taskbar* _w_toolkit_get_system_taskbar(w_toolkit *toolkit) {
	return 0;
}
w_tray* _w_toolkit_get_system_tray(w_toolkit *toolkit) {
	return 0;
}
w_thread* _w_toolkit_get_thread(w_toolkit *toolkit) {
	return 0;
}
wuint64 _w_toolkit_get_thread_id(w_toolkit *toolkit) {
	return 0;
}
wresult _w_toolkit_get_touch_enabled(w_toolkit *toolkit) {
	return W_FALSE;
}
wresult _w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
		w_point *result, w_point *point) {
	return W_FALSE;
}
wresult _w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
		w_rect *result, w_rect *rectangle) {
	return W_FALSE;
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
wresult _w_toolkit_read(w_toolkit *toolkit) {
	gdk_threads_leave();
	g_main_context_iteration(0, FALSE);
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
wresult _w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *point) {
	return W_FALSE;
}
wresult _w_toolkit_set_theme(w_toolkit *toolkit, w_theme *theme) {
	return W_TRUE;
}
int _w_toolkit_run(w_toolkit *toolkit) {
	_W_TOOLKIT(toolkit)->exit_loop = 0;
	while (_W_TOOLKIT(toolkit)->exit_loop == 0) {
		_w_toolkit_read(toolkit);
		_w_toolkit_dispatch(toolkit);
	}
	return _W_TOOLKIT(toolkit)->exit_code;
}
wresult _w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	return W_FALSE;
}
wresult _w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	return W_TRUE;
}
wresult _w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args) {
	return W_TRUE;
}
wresult _w_toolkit_update(w_toolkit *toolkit) {
	return W_TRUE;
}
wresult _w_toolkit_wake(w_toolkit *toolkit) {
	return W_FALSE;
}
void _w_toolkit_class_init(_w_toolkit *toolkit) {
	struct _w_toolkit_class *clazz = &toolkit->class_toolkit;
	((w_disposable*) toolkit)->clazz = (struct w_disposable_class*) clazz;
	clazz->disposable.dispose = _w_toolkit_dispose;
	clazz->check_widget = _w_toolkit_check_widget;
	clazz->check_widgetdata = _w_toolkit_check_widgetdata;
	clazz->get_class = _w_toolkit_get_class;
	clazz->get_theme = _w_toolkit_get_theme;
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
}
