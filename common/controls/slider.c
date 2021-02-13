/*
 * slider.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_slider_create(w_slider *slider, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(slider), toolkit, W_WIDGET(parent), style,
			_W_CLASS_SLIDER, post_event);
}
w_slider* w_slider_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_SLIDER(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_SLIDER,post_event));
}
wresult w_slider_get_increment(w_slider *slider) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->get_increment(slider);
	} else
		return result;
}
wresult w_slider_get_maximum(w_slider *slider) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->get_maximum(slider);
	} else
		return result;
}
wresult w_slider_get_minimum(w_slider *slider) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->get_minimum(slider);
	} else
		return result;
}
wresult w_slider_get_page_increment(w_slider *slider) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->get_page_increment(slider);
	} else
		return result;
}
wresult w_slider_get_selection(w_slider *slider) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->get_selection(slider);
	} else
		return result;
}
wresult w_slider_get_thumb(w_slider *slider) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->get_thumb(slider);
	} else
		return result;
}
wresult w_slider_get_values(w_slider *slider, w_slider_value *value) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->get_values(slider,value);
	} else
		return result;
}
wresult w_slider_set_increment(w_slider *slider, int value) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->set_increment(slider, value);
	} else
		return result;
}
wresult w_slider_set_maximum(w_slider *slider, int value) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->set_maximum(slider, value);
	} else
		return result;
}
wresult w_slider_set_minimum(w_slider *slider, int value) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->set_minimum(slider, value);
	} else
		return result;
}
wresult w_slider_set_page_increment(w_slider *slider, int value) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->set_page_increment(slider, value);
	} else
		return result;
}
wresult w_slider_set_selection(w_slider *slider, int value) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->set_selection(slider, value);
	} else
		return result;
}
wresult w_slider_set_thumb(w_slider *slider, int value) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->set_thumb(slider, value);
	} else
		return result;
}
wresult w_slider_set_values(w_slider *slider, w_slider_value *value) {
	wresult result = W_WIDGET_CHECK0(slider);
	if (result > 0) {
		return W_SLIDER_GET_CLASS(slider)->set_values(slider, value);
	} else
		return result;
}
