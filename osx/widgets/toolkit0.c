/*
 * Name:        toolkit0.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
#include <pthread.h>
_w_toolkit *mac_toolkit = 0;
w_toolkit* w_toolkit_get_platform(w_app *app) {
	if (mac_toolkit == 0) {
		const int total_size = 0x10000;
		const int toolkit_size = sizeof(_w_toolkit);
		const int tmp_size = total_size - toolkit_size;
		mac_toolkit = calloc(1, total_size);
		if (mac_toolkit == 0) {
			fprintf(stderr, "Error : Do not initialize toolkit\n");
			exit(EXIT_FAILURE);
			return 0;
		}
		mac_toolkit->tmp_alloc = tmp_size;
		_w_toolkit_init(mac_toolkit);
	}
	return (w_toolkit*) mac_toolkit;
}
wresult _w_toolkit_check_widget(w_toolkit *toolkit, w_widget *widget) {
	if (NSThread_isMainThread()) {
		return W_TRUE;
	} else {
		if (pthread_equal(pthread_self(), (pthread_t)
		_W_TOOLKIT(toolkit)->thread.id)) {
			return W_TRUE;
		} else {
			return W_ERROR_THREAD_INVALID_ACCESS;
		}
	}
}
wresult _w_toolkit_check_widgetdata(w_toolkit *toolkit,
		w_widgetdata *widgetdata) {
	if (NSThread_isMainThread()) {
		return W_TRUE;
	} else {
		if (pthread_equal(pthread_self(), (pthread_t)
		_W_TOOLKIT(toolkit)->thread.id)) {
			return W_TRUE;
		} else {
			return W_ERROR_THREAD_INVALID_ACCESS;
		}
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
	return 0;
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
	_w_shell *s = mac_toolkit->shells;
	while (s != 0) {
		if (_W_CONTROL(s)->parent == W_COMPOSITE(parent))
			return s;
		s = s->next;
	}
	return 0;
}
wresult _w_shells_iterator_next(w_iterator *it, void *obj) {
	if (_W_SHELLS_ITERATOR(it)->current != 0) {
		*((_w_shell**) obj) = _W_SHELLS_ITERATOR(it)->current;
		_W_SHELLS_ITERATOR(it)->current = _w_shells_iterator_find_next(
		_W_SHELLS_ITERATOR(it)->parent,
		_W_SHELLS_ITERATOR(it)->current);
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_shells_iterator_reset(w_iterator *it) {
	if (_W_SHELLS_ITERATOR(it)->parent == 0)
		_W_SHELLS_ITERATOR(it)->current = mac_toolkit->shells;
	else
		_W_SHELLS_ITERATOR(it)->current = _w_shells_iterator_find_first(
		_W_SHELLS_ITERATOR(it)->parent);
	return W_TRUE;
}
wresult _w_shells_iterator_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_shells_iterator_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_shells_iterator_count(w_iterator *it) {
	if (_W_SHELLS_ITERATOR(it)->parent == 0)
		return mac_toolkit->shells_count;
	else {
		if (_W_SHELLS_ITERATOR(it)->count == -1) {
			_w_shell *s = mac_toolkit->shells;
			size_t count = 0;
			while (s != 0) {
				if (_W_CONTROL(s)->parent
						== (w_composite*) _W_SHELLS_ITERATOR(it)->parent)
					count++;
				s = s->next;
			}
			_W_SHELLS_ITERATOR(it)->count = count;
		}
		return _W_SHELLS_ITERATOR(it)->count;
	}
}
_w_iterator_class _w_shells_iterator_class = { //
		_w_shells_iterator_close, //
				_w_shells_iterator_next, //
				_w_shells_iterator_reset, //
				_w_shells_iterator_remove, //
				_w_shells_iterator_count };
void _w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells) {
	_W_SHELLS_ITERATOR(shells)->base.clazz = &_w_shells_iterator_class;
	_W_SHELLS_ITERATOR(shells)->current = mac_toolkit->shells;
	_W_SHELLS_ITERATOR(shells)->parent = 0;
	_W_SHELLS_ITERATOR(shells)->count = -1;
}
void _w_toolkit_get_shells_from_parent(w_shell *shell, w_iterator *iterator) {
	_W_SHELLS_ITERATOR(iterator)->base.clazz = &_w_shells_iterator_class;
	_W_SHELLS_ITERATOR(iterator)->current = _w_shells_iterator_find_first(
	_W_SHELLS_ITERATOR(iterator)->parent);
	_W_SHELLS_ITERATOR(iterator)->parent = _W_SHELL(shell);
	_W_SHELLS_ITERATOR(iterator)->count = -1;
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
	return (w_font*)&_W_TOOLKIT(toolkit)->systemFont;
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
	return &_W_TOOLKIT(toolkit)->thread;
}
wuint64 _w_toolkit_get_thread_id(w_toolkit *toolkit) {
	return _W_TOOLKIT(toolkit)->thread.id;
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
    NSRunLoop* loop = NSRunLoop_currentRunLoop();
    NSRunLoop_runMode(loop, NSDefaultRunLoopMode, NSDate_distantFuture());
    return W_TRUE;
}
wresult _w_toolkit_read(w_toolkit *toolkit) {
    NSEvent* event = NSApplication_nextEventMatchingMask(_W_TOOLKIT(toolkit)->application, NSAnyEventMask, 0, NSDefaultRunLoopMode, W_TRUE);
        if ((event != 0) && (_W_TOOLKIT(toolkit)->application != 0)) {
            NSApplication_sendEvent(_W_TOOLKIT(toolkit)->application, event);
        }
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
