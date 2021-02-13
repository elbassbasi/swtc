/*
 * Name:        slider.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_SLIDER_H_
#define SWTC_CONTROLS_SLIDER_H_
#include "../widgets/control.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef w_scrollbar_value w_slider_value;
typedef struct w_slider {
	w_control control;
	void *handle[8];
} w_slider;
#define W_SLIDER(x) ((w_slider*)x)
struct _w_slider_class {
	struct _w_control_class control;
	wresult (*get_increment)(w_slider *slider);
	wresult (*get_maximum)(w_slider *slider);
	wresult (*get_minimum)(w_slider *slider);
	wresult (*get_page_increment)(w_slider *slider);
	wresult (*get_selection)(w_slider *slider);
	wresult (*get_thumb)(w_slider *slider);
	wresult (*get_values)(w_slider *slider, w_slider_value *value);
	wresult (*set_increment)(w_slider *slider, int value);
	wresult (*set_maximum)(w_slider *slider, int value);
	wresult (*set_minimum)(w_slider *slider, int value);
	wresult (*set_page_increment)(w_slider *slider, int value);
	wresult (*set_selection)(w_slider *slider, int value);
	wresult (*set_thumb)(w_slider *slider, int value);
	wresult (*set_values)(w_slider *slider, w_slider_value *value);
};
SWT_PUBLIC wresult w_slider_create(w_slider *slider, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_slider* w_slider_new(struct w_toolkit *toolkit, w_composite *parent,
                                  wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_slider_get_increment(w_slider *slider);
SWT_PUBLIC wresult w_slider_get_maximum(w_slider *slider);
SWT_PUBLIC wresult w_slider_get_minimum(w_slider *slider);
SWT_PUBLIC wresult w_slider_get_page_increment(w_slider *slider);
SWT_PUBLIC wresult w_slider_get_selection(w_slider *slider);
SWT_PUBLIC wresult w_slider_get_thumb(w_slider *slider);
SWT_PUBLIC wresult w_slider_get_values(w_slider *slider, w_slider_value *value);
SWT_PUBLIC wresult w_slider_set_increment(w_slider *slider, int value);
SWT_PUBLIC wresult w_slider_set_maximum(w_slider *slider, int value);
SWT_PUBLIC wresult w_slider_set_minimum(w_slider *slider, int value);
SWT_PUBLIC wresult w_slider_set_page_increment(w_slider *slider, int value);
SWT_PUBLIC wresult w_slider_set_selection(w_slider *slider, int value);
SWT_PUBLIC wresult w_slider_set_thumb(w_slider *slider, int value);
SWT_PUBLIC wresult w_slider_set_values(w_slider *slider, w_slider_value *value);

#define W_SLIDER_CLASS(x) ((struct _w_slider_class*)x)
#define W_SLIDER_GET_CLASS(x) ((struct _w_slider_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_SLIDER_H_ */
