/*
 * spinner.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_spinner_create(w_spinner *spinner, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(spinner), toolkit, W_WIDGET(parent), style,
			_W_CLASS_SPINNER, post_event);
}
w_spinner* w_spinner_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_SPINNER(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_SPINNER,post_event));
}
wresult w_spinner_copy(w_spinner *spinner) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->copy(spinner);
	} else
		return result;
}
wresult w_spinner_cut(w_spinner *spinner) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->cut(spinner);
	} else
		return result;
}
wresult w_spinner_get_digits(w_spinner *spinner) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->get_digits(spinner);
	} else
		return result;
}
wresult w_spinner_get_increment(w_spinner *spinner) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->get_increment(spinner);
	} else
		return result;
}
wresult w_spinner_get_maximum(w_spinner *spinner) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->get_maximum(spinner);
	} else
		return result;
}
wresult w_spinner_get_minimum(w_spinner *spinner) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->get_minimum(spinner);
	} else
		return result;
}
wresult w_spinner_get_page_increment(w_spinner *spinner) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->get_page_increment(spinner);
	} else
		return result;
}
wresult w_spinner_get_selection(w_spinner *spinner) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->get_selection(spinner);
	} else
		return result;
}
wresult w_spinner_get_text(w_spinner *spinner, w_alloc alloc, void *user_data,
		int enc) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->get_text(spinner, alloc, user_data,
				enc);
	} else
		return result;
}
wresult w_spinner_get_text_limit(w_spinner *spinner) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->get_text_limit(spinner);
	} else
		return result;
}
wresult w_spinner_get_values(w_spinner *spinner, w_spinner_value *value) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->get_values(spinner, value);
	} else
		return result;
}
wresult w_spinner_paste(w_spinner *spinner) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->paste(spinner);
	} else
		return result;
}
wresult w_spinner_set_digits(w_spinner *spinner, int value) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->set_digits(spinner, value);
	} else
		return result;
}
wresult w_spinner_set_increment(w_spinner *spinner, int value) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->set_increment(spinner, value);
	} else
		return result;
}
wresult w_spinner_set_maximum(w_spinner *spinner, int value) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->set_maximum(spinner, value);
	} else
		return result;
}
wresult w_spinner_set_minimum(w_spinner *spinner, int value) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->set_minimum(spinner, value);
	} else
		return result;
}
wresult w_spinner_set_page_increment(w_spinner *spinner, int value) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->set_page_increment(spinner, value);
	} else
		return result;
}
wresult w_spinner_set_selection(w_spinner *spinner, int value) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->set_selection(spinner, value);
	} else
		return result;
}
wresult w_spinner_set_text_limit(w_spinner *spinner, int limit) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->set_text_limit(spinner, limit);
	} else
		return result;
}
wresult w_spinner_set_values(w_spinner *spinner, w_spinner_value *value) {
	wresult result = W_WIDGET_CHECK0(spinner);
	if (result > 0) {
		return W_SPINNER_GET_CLASS(spinner)->set_values(spinner, value);
	} else
		return result;
}
