/*
 * widget.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
wresult w_widget_check(w_widget *widget) {
	if (widget != 0 && widget->clazz != 0) {
		w_toolkit *toolkit = widget->clazz->toolkit;
		return W_TOOLKIT_GET_CLASS(toolkit)->check_widget(toolkit, widget);
	} else
		return W_ERROR_NO_HANDLES;
}

void w_widget_init(w_widget *widget) {
	widget->clazz = 0;
	widget->ref = -1;
}
int w_widget_class_id(w_widget *widget) {
	if (widget != 0 && widget->clazz != 0)
		return widget->clazz->class_id;
	else
		return 0;
}
struct _w_widget_class* w_widget_get_class(w_widget *widget) {
	if (widget != 0)
		return widget->clazz;
	else
		return 0;
}
wresult w_widget_is_ok(w_widget *widget) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return widget->clazz->is_ok(widget);
	} else
		return W_FALSE;
}
wresult w_widget_init_themedata(w_widget *widget, w_themedata *data) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return widget->clazz->init_themedata(widget, data);
	} else
		return result;
}
wresult w_widget_dispose(w_widget *widget) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		widget->clazz->dispose(widget);
		widget->clazz = 0;
	}
	return W_TRUE;
}
void w_widget_free(w_widget *widget) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		w_widget_post_event_proc post_event = widget->post_event;
		widget->clazz->dispose(widget);
		w_event e;
		e.type = W_EVENT_FREE_MEMORY;
		e.widget = widget;
		e.platform_event = 0;
		e.data = 0;
		if (post_event != 0) {
			post_event(widget, &e);
		} else
			free(widget);
	}
}
void w_widget_ref_create(w_widget *widget) {
	widget->ref = 0;
}
void w_widget_ref_inc(w_widget *widget) {
	if (widget->ref > 0) {
		watomic_int_fetch_add(&widget->ref, 1);
	}
}
w_widget* w_widget_ref_dec(w_widget *widget) {
	if (widget->ref > 0) {
		watomic_int_fetch_sub(&widget->ref, 1);
		if (widget->ref <= 0) {
			w_widget_post_event_proc post_event = widget->post_event;
			if (widget->clazz != 0) {
				widget->clazz->dispose(widget);
			}
			w_event e;
			e.type = W_EVENT_FREE_MEMORY;
			e.widget = widget;
			e.platform_event = 0;
			e.data = 0;
			if (post_event != 0) {
				post_event(widget, &e);
			} else
				free(widget);
		}
	}
	return widget;
}
wresult w_widget_get_shell(w_widget *widget, w_shell **shell) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return W_WIDGET_GET_CLASS(widget)->get_shell(widget, shell);
	} else {
		*shell = 0;
		return result;
	}
}
w_toolkit* w_widget_get_toolkit(w_widget *widget) {
	if (w_widget_is_ok(widget) > 0) {
		return widget->clazz->toolkit;
	} else
		return w_app_get_default_toolkit(w_app_get());
}
wresult w_widget_get_theme(w_widget *widget, w_theme **theme) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return W_WIDGET_GET_CLASS(widget)->get_theme(widget, theme);
	} else {
		*theme = 0;
		return result;
	}
}
wresult w_widget_get_next_sibling(w_widget *widget, w_widget **next_sibling,
		int filter) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return W_WIDGET_GET_CLASS(widget)->get_next_sibling(widget,
				next_sibling, filter);
	} else {
		*next_sibling = 0;
		return result;
	}
}
wresult w_widget_get_parent(w_widget *widget, w_widget **parent) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return W_WIDGET_GET_CLASS(widget)->get_parent(widget, parent);
	} else {
		*parent = 0;
		return result;
	}
}
wresult w_widget_get_prev_sibling(w_widget *widget, w_widget **prev_sibling,
		int filter) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return W_WIDGET_GET_CLASS(widget)->get_prev_sibling(widget,
				prev_sibling, filter);
	} else {
		*prev_sibling = 0;
		return result;
	}
}
w_widget_post_event_proc w_widget_get_post_event(w_widget *widget) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return widget->post_event;
	} else
		return 0;
}
w_widget_post_event_proc w_widget_set_post_event(w_widget *widget,
		w_widget_post_event_proc post_event) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		w_widget_post_event_proc last = widget->post_event;
		widget->post_event = post_event;
		return last;
	} else
		return 0;
}
wresult w_widget_set_id(w_widget *widget, wuint id) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		widget->id = id;
		return W_TRUE;
	} else
		return result;
}
wuint w_widget_get_id(w_widget *widget) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return widget->id;
	} else
		return 0;
}
wresult w_widget_set_theme(w_widget *widget, w_theme *theme) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return W_WIDGET_GET_CLASS(widget)->set_theme(widget, theme);
	} else
		return result;
}
wuint64 w_widget_get_style(w_widget *widget) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return widget->style;
	} else
		return 0;
}
void* w_widget_get_data(w_widget *widget, wuint index) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0 && index < sizeof(widget->data) / sizeof(widget->data[0])) {
		return widget->data[index];
	} else
		return 0;
}
void* w_widget_set_data(w_widget *widget, wuint index, void *data) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0 && index < sizeof(widget->data) / sizeof(widget->data[0])) {
		void *last = widget->data[index];
		widget->data[index] = data;
		return last;
	} else
		return 0;
}
wresult w_widget_post_event(w_widget *widget, w_event *event, int flags) {
	wresult result = W_WIDGET_CHECK0(widget);
	if (result > 0) {
		return W_WIDGET_GET_CLASS(widget)->post_event(widget, event, flags);
	} else
		return result;
}
wresult _w_widget_create(w_widget *widget, w_toolkit *toolkit, w_widget *parent,
		wuint64 style, wuint class_id, w_widget_post_event_proc post_event) {
	struct _w_widget_class *clazz;
	w_event e;
	if (parent != 0 && !w_widget_is_ok(W_WIDGET(parent))) {
		return W_ERROR_INVALID_ARGUMENT;
	}
	w_widget_init(W_WIDGET(widget));
	if (toolkit == 0) {
		if (parent != 0) {
			toolkit = w_widget_get_toolkit(W_WIDGET(parent));
		} else {
			toolkit = w_app_get_default_toolkit(w_app_get());
		}
	}
	clazz = (struct _w_widget_class*) w_toolkit_get_class(toolkit, class_id);
	if (clazz == 0)
		return W_ERROR_INVALID_SUBCLASS;
	if (clazz->class_id == 0)
		w_toolkit_init_class(toolkit, class_id, clazz);
	memset(&(widget->clazz), 0, clazz->object_used_size);
	widget->clazz = clazz;
	wresult ret = clazz->create(widget, parent, style, post_event);
	if (ret < 0) {
		widget->clazz = 0;
		if ((style & W_FREE_MEMORY) && post_event != 0) {
			e.type = W_EVENT_FREE_MEMORY;
			e.widget = widget;
			e.platform_event = 0;
			e.data = 0;
			post_event(widget, &e);
		}
	}
	return ret;
}
w_widget* _w_widget_new(w_toolkit *toolkit, w_widget *parent, wuint64 style,
		wuint class_id, w_widget_post_event_proc post_event) {
	struct _w_widget_class *clazz;
	w_toolkit *_t = toolkit;
	if (_t == 0) {
		if (parent != 0) {
			_t = w_widget_get_toolkit(parent);
			if (_t == 0) {
				return 0;
			}
		} else {
			_t = w_app_get_default_toolkit(w_app_get());
		}
	}
	clazz = (struct _w_widget_class*) w_toolkit_get_class(_t, class_id);
	if (clazz == 0)
		return 0;
	if (clazz->class_id == 0)
		w_toolkit_init_class(toolkit, class_id, clazz);
	w_widget *widget = (w_widget*) malloc(clazz->object_total_size);
	if (widget == 0)
		return 0;
	memset(&(widget->clazz), 0, clazz->object_used_size + sizeof(void*));
	widget->clazz = clazz;
	int ret = clazz->create(widget, parent, style, post_event);
	if (ret < 0) {
		free(widget);
		return 0;
	}
	return widget;
}
