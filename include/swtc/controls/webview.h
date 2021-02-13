/*
 * Name:        webview.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_WEBVIEW_H_
#define SWTC_CONTROLS_WEBVIEW_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_webview {
	w_composite composite;
	void *handle[0x10];
} w_webview;

#define W_WEBVIEW(x) ((w_webview*)x)

struct _w_webview_class {
	struct _w_composite_class composite;
	wresult (*back)(w_webview *webview);
	wresult (*execute)(w_webview *webview, const char *script, int length,
			int enc);
	wresult (*close)(w_webview *webview);
	wresult (*evaluate)(w_webview *webview, const char *script, int length,
			int enc, w_value *result);
	wresult (*forward)(w_webview *webview);
	wresult (*get_webview_type)(w_webview *webview, char **type);
	wresult (*get_javascript_enabled)(w_webview *webview);
	wresult (*get_text)(w_webview *webview, w_alloc alloc, void *user_data,
			int enc);
	wresult (*get_url)(w_webview *webview, w_alloc alloc, void *user_data,
			int enc);
	wresult (*is_back_enabled)(w_webview *webview);
	wresult (*is_forward_enabled)(w_webview *webview);
	wresult (*refresh)(w_webview *webview);
	wresult (*set_javascript_enabled)(w_webview *webview, int enabled);
	wresult (*set_text)(w_webview *webview, const char *html, int length,
			int enc, int trusted);
	wresult (*set_url)(w_webview *webview, const char *url, int length, int enc,
			const char *postData, const char *headers);
	wresult (*stop)(w_webview *webview);
};
SWT_PUBLIC wresult w_webview_create(w_webview *webview,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_webview* w_webview_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_webview_back(w_webview *webview);
SWT_PUBLIC wresult w_webview_execute(w_webview *webview, const char *script,
		int length, int enc);
SWT_PUBLIC wresult w_webview_close(w_webview *webview);
SWT_PUBLIC wresult w_webview_evaluate(w_webview *webview, const char *script,
		int length, int enc, w_value *result);
SWT_PUBLIC wresult w_webview_forward(w_webview *webview);
SWT_PUBLIC wresult w_webview_get_webview_type(w_webview *webview,char** type);
SWT_PUBLIC wresult w_webview_get_javascript_enabled(w_webview *webview);
SWT_PUBLIC wresult w_webview_get_text(w_webview *webview, w_alloc alloc,
		void *user_data, int enc);
SWT_PUBLIC wresult w_webview_get_url(w_webview *webview, w_alloc alloc,
		void *user_data, int enc);
SWT_PUBLIC wresult w_webview_is_back_enabled(w_webview *webview);
SWT_PUBLIC wresult w_webview_is_forward_enabled(w_webview *webview);
SWT_PUBLIC wresult w_webview_refresh(w_webview *webview);
SWT_PUBLIC wresult w_webview_set_javascript_enabled(w_webview *webview,
		int enabled);
SWT_PUBLIC wresult w_webview_set_text(w_webview *webview, const char *html,
		int length, int enc, int trusted);
SWT_PUBLIC wresult w_webview_set_url(w_webview *webview, const char *url,
		int length, int enc, const char *postData, const char *headers);
SWT_PUBLIC wresult w_webview_stop(w_webview *webview);

#define W_WEBVIEW_CLASS(x) ((struct _w_webview_class*)x)
#define W_WEBVIEW_GET_CLASS(x) ((struct _w_webview_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_WEBVIEW_H_ */
