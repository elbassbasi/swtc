/*
 * control.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
wresult w_control_get_graphics(w_control *control, w_graphics *gc) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_graphics(control, gc);
	} else
		return result;
}
wresult w_control_get_orientation(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_orientation(control);
	} else
		return result;
}
wresult w_control_set_orientation(w_control *control, int orientation) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_orientation(control,
				orientation);
	} else
		return result;
}
wresult w_control_get_text_direction(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_text_direction(control);
	} else
		return result;
}
wresult w_control_set_text_direction(w_control *control, int textDirection) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_text_direction(control,
				textDirection);
	} else
		return result;
}
wresult w_control_print(w_control *control, w_graphics *gc) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->print(control, gc);
	} else
		return result;
}
wresult w_control_compute_size(w_control *control, w_size *size, int wHint,
		int hHint) {
	return w_control_compute_size_0(control, size, wHint, hHint, W_TRUE);
}
wresult w_control_compute_size_0(w_control *control, w_size *size, int wHint,
		int hHint, int changed) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		w_event_compute_size e;
		e.event.type = W_EVENT_COMPUTE_SIZE;
		e.event.platform_event = 0;
		e.event.data = 0;
		e.event.widget = W_WIDGET(control);
		e.size = size;
		e.wHint = wHint;
		e.hHint = hHint;
		return w_widget_post_event(W_WIDGET(control), (w_event*) &e,
				W_EVENT_SEND);
	} else
		return result;
}
wresult w_control_get_accessible(w_control *control,
		w_accessible **accessible) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_accessible(control, accessible);
	} else {
		*accessible = 0;
		return result;
	}
}
wresult w_control_get_bounds(w_control *control, w_point *location,
		w_size *size) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_bounds(control, location, size);
	} else
		return result;
}
wresult w_control_set_bounds(w_control *control, w_point *location,
		w_size *size) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_bounds(control, location, size);
	} else
		return result;
}
wresult w_control_set_bounds_(w_control *control, int x, int y, int width,
		int heigth) {
	w_rect r;
	r.x = x;
	r.y = y;
	r.width = width;
	r.height = heigth;
	return w_control_set_bounds(control, &r.pt, &r.sz);
}
wresult w_control_get_region(w_control *control, w_region *region) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_region(control, region);
	} else
		return result;
}
wresult w_control_set_region(w_control *control, w_region *region) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_region(control, region);
	} else
		return result;
}
wresult w_control_move_above(w_control *control, w_control *_control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->move_above(control, _control);
	} else
		return result;
}
wresult w_control_move_below(w_control *control, w_control *_control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->move_below(control, _control);
	} else
		return result;
}
wresult w_control_pack(w_control *control, int changed) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->pack(control, changed);
	} else
		return result;
}
wresult w_control_to_control(w_control *control, w_point *_result,
		w_point *point) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->to_control(control, _result, point);
	} else
		return result;
}
wresult w_control_to_display(w_control *control, w_point *_result,
		w_point *point) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->to_display(control, _result, point);
	} else
		return result;
}
wresult w_control_drag_detect(w_control *control, w_event_mouse *event) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->drag_detect(control, event);
	} else
		return result;
}
wresult w_control_get_drag_detect(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_drag_detect(control);
	} else
		return result;
}
wresult w_control_set_drag_detect(w_control *control, int dragDetect) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_drag_detect(control,
				dragDetect);
	} else
		return result;
}
wresult w_control_set_focus(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_focus(control);
	} else
		return result;
}
wresult w_control_force_focus(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->force_focus(control);
	} else
		return result;
}
wresult w_control_is_focus_control(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->is_focus_control(control);
	} else
		return result;
}
wresult w_control_get_border_width(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_border_width(control);
	} else
		return result;
}
wresult w_control_get_cursor(w_control *control, w_cursor **cursor) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_cursor(control, cursor);
	} else {
		*cursor = 0;
		return result;
	}
}
wresult w_control_set_cursor(w_control *control, w_cursor *cursor) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_cursor(control, cursor);
	} else
		return result;
}
wresult w_control_get_enabled(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_enabled(control);
	} else
		return result;
}
wresult w_control_set_enabled(w_control *control, int enabled) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_enabled(control, enabled);
	} else
		return result;
}
wresult w_control_is_enabled(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->is_enabled(control);
	} else
		return result;
}
wresult w_control_get_font(w_control *control, w_font **font) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_font(control, font);
	} else {
		*font = 0;
		return result;
	}
}
wresult w_control_set_font(w_control *control, w_font *font) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_font(control, font);
	} else
		return result;
}
wresult w_control_get_background(w_control *control, w_color *background) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_background(control, background);
	} else {
		*background = 0xFF000000;
		return result;
	}
}
wresult w_control_set_background(w_control *control, w_color color) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_background(control, color);
	} else
		return result;
}
wresult w_control_get_foreground(w_control *control, w_color *foreground) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_foreground(control, foreground);
	} else {
		*foreground = 0xFF000000;
		return result;
	}
}
wresult w_control_set_foreground(w_control *control, w_color color) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_foreground(control, color);
	} else
		return result;
}
wresult w_control_get_layout_data(w_control *control, void **data) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_layout_data(control, data);
	} else {
		*data = 0;
		return result;
	}
}
wresult w_control_new_layout_data(w_control *control, void **data,
		size_t size) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->new_layout_data(control, data,
				size);
	} else {
		*data = 0;
		return result;
	}
}

wresult w_control_layout_set_data(w_control *control,
		const w_layout_data *data) {
	w_widget *parent;
	wresult result = w_widget_get_parent(W_WIDGET(control), &parent);
	if (result >= 0 && parent != 0) {
		w_layout *layout;
		result = w_composite_get_layout(W_COMPOSITE(parent), &layout);
		if (result >= 0 && layout != 0) {
			w_layout_set_data(layout, control, data);
		}
	}
	return result;
}
wresult w_control_layout_get_data(w_control *control, w_layout_data *data) {
	w_widget *parent;
	wresult result = w_widget_get_parent(W_WIDGET(control), &parent);
	if (result >= 0 && parent != 0) {
		w_layout *layout;
		result = w_composite_get_layout(W_COMPOSITE(parent), &layout);
		if (result >= 0 && layout != 0) {
			w_layout_get_data(layout, control, data);
		}
	}
	return result;
}
wresult w_control_request_layout(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->request_layout(control);
	} else
		return result;
}
wresult w_control_get_menu(w_control *control, w_menu **menu) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_menu(control, menu);
	} else {
		*menu = 0;
		return result;
	}
}
wresult w_control_set_menu(w_control *control, struct w_menu *menu) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_menu(control, menu);
	} else
		return result;
}
wresult w_control_set_parent(w_control *control, w_composite *parent) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_parent(control, parent);
	} else
		return result;
}
wresult w_control_is_reparentable(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->is_reparentable(control);
	} else
		return result;
}
wresult w_control_get_tooltip_text(w_control *control, w_alloc alloc,
		void *user_data, int enc) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_tooltip_text(control, alloc,
				user_data, enc);
	} else {
		return result;
	}
}
wresult w_control_set_tooltip_text(w_control *control, const char *text,
		int length, int enc) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_tooltip_text(control, text,
				length, enc);
	} else
		return result;
}
wresult w_control_get_touch_enabled(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_touch_enabled(control);
	} else
		return result;
}
wresult w_control_set_touch_enabled(w_control *control, int enabled) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_touch_enabled(control, enabled);
	} else
		return result;
}
wresult w_control_get_visible(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_visible(control);
	} else
		return result;
}
wresult w_control_set_visible(w_control *control, int visible) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_visible(control, visible);
	} else
		return result;
}
wresult w_control_is_visible(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->is_visible(control);
	} else
		return result;
}
wresult w_control_redraw(w_control *control, w_rect *rect, int all) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->redraw(control, rect, all);
	} else
		return result;
}
wresult w_control_set_redraw(w_control *control, int redraw) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_redraw(control, redraw);
	} else
		return result;
}
wresult w_control_set_capture(w_control *control, int capture) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_capture(control, capture);
	} else
		return result;
}
wresult w_control_traverse(w_control *control, int traversal,
		w_event_key *event) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->traverse(control, traversal, event);
	} else
		return result;
}
wresult w_control_update(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->update(control);
	} else
		return result;
}
wresult w_control_set_timer(w_control *control, wint64 ms, wushort id) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_timer(control, ms, id);
	} else
		return result;
}
wresult w_control_kill_timer(w_control *control, wushort id) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->kill_timer(control, id);
	} else
		return result;
}
wresult w_control_set_tab(w_control *control, int tab) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->set_tab(control, tab);
	} else
		return result;
}
wresult w_control_get_tab(w_control *control) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->get_tab(control);
	} else
		return result;
}
wresult w_control_create_dragsource(w_control *control,
		w_dragsource *dragsource, wuint64 style,
		w_widget_post_event_proc post_event) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->create_dragsource(control,
				dragsource, style, post_event);
	} else
		return result;
}
wresult w_control_create_droptarget(w_control *control,
		w_droptarget *droptarget, wuint64 style,
		w_widget_post_event_proc post_event) {
	wresult result = W_WIDGET_CHECK0(control);
	if (result > 0) {
		return W_CONTROL_GET_CLASS(control)->create_droptarget(control,
				droptarget, style, post_event);
	} else
		return result;
}
