/*
 * webview.c
 *
 *  Created on: 12 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_webview_create(w_webview *webview, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(webview), toolkit, W_WIDGET(parent), style,
			_W_CLASS_WEBVIEW, post_event);
}
w_webview* w_webview_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_WEBVIEW(
			_w_widget_new(toolkit, W_WIDGET(parent), style,_W_CLASS_WEBVIEW, post_event));
}
wresult w_webview_back(w_webview *webview) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->back(webview);
	} else
		return result;
}
wresult w_webview_execute(w_webview *webview, const char *script,int length, int enc) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->execute(webview, script,length,enc);
	} else
		return result;
}
wresult w_webview_close(w_webview *webview) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->close(webview);
	} else
		return result;
}
wresult w_webview_evaluate(w_webview *webview, const char *script, int length,
		int enc, w_value *ret) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->evaluate(webview, script, length,
				enc, ret);
	} else
		return result;
}
wresult w_webview_forward(w_webview *webview) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->forward(webview);
	} else
		return result;
}
wresult w_webview_get_webview_type(w_webview *webview, char **type) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->get_webview_type(webview, type);
	} else
		return result;
}
wresult w_webview_get_javascript_enabled(w_webview *webview) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->get_javascript_enabled(webview);
	} else
		return result;
}
wresult w_webview_get_text(w_webview *webview, w_alloc alloc, void *user_data,
		int enc) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->get_text(webview, alloc, user_data,
				enc);
	} else
		return result;
}
wresult w_webview_get_url(w_webview *webview, w_alloc alloc, void *user_data,
		int enc) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->get_url(webview, alloc, user_data,
				enc);
	} else
		return result;
}
wresult w_webview_is_back_enabled(w_webview *webview) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->is_back_enabled(webview);
	} else
		return result;
}
wresult w_webview_is_forward_enabled(w_webview *webview) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->is_forward_enabled(webview);
	} else
		return result;
}
wresult w_webview_refresh(w_webview *webview) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->refresh(webview);
	} else
		return result;
}
wresult w_webview_set_javascript_enabled(w_webview *webview, int enabled) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->set_javascript_enabled(webview,
				enabled);
	} else
		return result;
}
wresult w_webview_set_text(w_webview *webview, const char *html, int length,
		int enc, int trusted) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->set_text(webview, html, length,
				enc, trusted);
	} else
		return result;
}
wresult w_webview_set_url(w_webview *webview, const char *url, int length,
		int enc, const char *postData, const char *headers) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->set_url(webview, url, length, enc,
				postData, headers);
	} else
		return result;
}
wresult w_webview_stop(w_webview *webview) {
	wresult result = W_WIDGET_CHECK0(webview);
	if (result > 0) {
		return W_WEBVIEW_GET_CLASS(webview)->stop(webview);
	} else
		return result;
}

