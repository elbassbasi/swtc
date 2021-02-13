/*
 * scrollable.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
wresult w_scrollbar_get_parent(w_scrollbar *scrollbar, w_scrollable **parent) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_parent(scrollbar, parent);
	} else
		return result;
}
wresult w_scrollbar_get_enabled(w_scrollbar *scrollbar) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_enabled(scrollbar);
	} else
		return result;
}
wresult w_scrollbar_is_enabled(w_scrollbar *scrollbar) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->is_enabled(scrollbar);
	} else
		return result;
}
wresult w_scrollbar_set_enabled(w_scrollbar *scrollbar, int enabled) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->set_enabled(scrollbar, enabled);
	} else
		return result;
}
wresult w_scrollbar_get_visible(w_scrollbar *scrollbar) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_visible(scrollbar);
	} else
		return result;
}
wresult w_scrollbar_is_visible(w_scrollbar *scrollbar) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->is_visible(scrollbar);
	} else
		return result;
}
wresult w_scrollbar_set_visible(w_scrollbar *scrollbar, int visible) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->set_visible(scrollbar, visible);
	} else
		return result;
}
wresult w_scrollbar_get_values(w_scrollbar *scrollbar,
		w_scrollbar_value *values) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_values(scrollbar, values);
	} else {
		memset(values, 0, sizeof(w_scrollbar_value));
		return result;
	}
}
wresult w_scrollbar_get_increment(w_scrollbar *scrollbar) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_increment(scrollbar);
	} else
		return result;
}
wresult w_scrollbar_get_maximum(w_scrollbar *scrollbar) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_maximum(scrollbar);
	} else
		return result;
}
wresult w_scrollbar_get_minimum(w_scrollbar *scrollbar) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_minimum(scrollbar);
	} else
		return result;
}
wresult w_scrollbar_get_page_increment(w_scrollbar *scrollbar) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_page_increment(scrollbar);
	} else
		return result;
}
wresult w_scrollbar_get_selection(w_scrollbar *scrollbar) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_selection(scrollbar);
	} else
		return result;
}
wresult w_scrollbar_get_thumb(w_scrollbar *scrollbar) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_thumb(scrollbar);
	} else
		return result;
}
wresult w_scrollbar_set_values(w_scrollbar *scrollbar,
		w_scrollbar_value *values) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->set_values(scrollbar, values);
	} else
		return result;
}
wresult w_scrollbar_set_increment(w_scrollbar *scrollbar, int increment) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->set_increment(scrollbar,
				increment);
	} else
		return result;
}
wresult w_scrollbar_set_maximum(w_scrollbar *scrollbar, int maximum) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->set_maximum(scrollbar, maximum);
	} else
		return result;
}
wresult w_scrollbar_set_minimum(w_scrollbar *scrollbar, int minimum) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->set_minimum(scrollbar, minimum);
	} else
		return result;
}
wresult w_scrollbar_set_page_increment(w_scrollbar *scrollbar,
		int pageIncrement) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->set_page_increment(scrollbar,
				pageIncrement);
	} else
		return result;
}
wresult w_scrollbar_set_selection(w_scrollbar *scrollbar, int selection) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->set_selection(scrollbar,
				selection);
	} else
		return result;
}
wresult w_scrollbar_set_thumb(w_scrollbar *scrollbar, int thumb) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->set_thumb(scrollbar, thumb);
	} else
		return result;
}
wresult w_scrollbar_get_size(w_scrollbar *scrollbar, w_size *size) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_size(scrollbar, size);
	} else {
		memset(size, 0, sizeof(w_size));
		return result;
	}
}
wresult w_scrollbar_get_thumb_bounds(w_scrollbar *scrollbar, w_rect *rect) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_thumb_bounds(scrollbar,
				rect);
	} else {
		memset(rect, 0, sizeof(w_rect));
		return result;
	}
}
wresult w_scrollbar_get_thumb_track_bounds(w_scrollbar *scrollbar,
		w_rect *rect) {
	wresult result = W_WIDGETDATA_CHECK0(scrollbar);
	if (result >= 0) {
		return W_SCROLLBAR_GET_CLASS(scrollbar)->get_thumb_track_bounds(
				scrollbar, rect);
	} else {
		memset(rect, 0, sizeof(w_rect));
		return result;
	}
}
/*
 *
 */
wresult w_scrollable_compute_trim(w_scrollable *scrollable, w_rect *_result,
		w_rect *rect) {
	wresult result = W_WIDGET_CHECK0(scrollable);
	if (result >= 0) {
		w_event_compute_trim e;
		e.event.type = W_EVENT_COMPUTE_TRIM;
		e.event.platform_event = 0;
		e.event.data = 0;
		e.rect = rect;
		e.result = _result;
		return w_widget_send_event(W_WIDGET(scrollable), (w_event*) &e);
	} else {
		memset(_result, 0, sizeof(w_rect));
		return result;
	}
}
wresult w_scrollable_get_client_area(w_scrollable *scrollable, w_rect *rect) {
	wresult result = W_WIDGET_CHECK0(scrollable);
	if (result >= 0) {
		struct w_event_client_area e;
		e.event.type = W_EVENT_CLIENT_AREA;
		e.event.platform_event = 0;
		e.event.data = 0;
		e.rect = rect;
		return w_widget_send_event(W_WIDGET(scrollable), (w_event*) &e);
	} else {
		memset(rect, 0, sizeof(w_rect));
		return result;
	}
}
wresult w_scrollable_get_horizontal_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
	w_widgetdata_close(W_WIDGETDATA(scrollbar));
	wresult result = W_WIDGET_CHECK0(scrollable);
	if (result >= 0) {
		return W_SCROLLABLE_GET_CLASS(scrollable)->get_horizontal_bar(
				scrollable, scrollbar);
	} else
		return result;
}
wresult w_scrollable_get_scrollbars_mode(w_scrollable *scrollable) {
	wresult result = W_WIDGET_CHECK0(scrollable);
	if (result >= 0) {
		return W_SCROLLABLE_GET_CLASS(scrollable)->get_scrollbars_mode(
				scrollable);
	} else
		return result;
}
wresult w_scrollable_get_vertical_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
	w_widgetdata_close(W_WIDGETDATA(scrollbar));
	wresult result = W_WIDGET_CHECK0(scrollable);
	if (result >= 0) {
		return W_SCROLLABLE_GET_CLASS(scrollable)->get_vertical_bar(scrollable,
				scrollbar);
	} else
		return result;
}

