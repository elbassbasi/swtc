/*
 * Name:        label.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_LABEL_H_
#define SWTC_CONTROLS_LABEL_H_
#include "../widgets/control.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_label {
	w_control control;
	void *handle[8];
} w_label;
#define W_LABEL(x) ((w_label*)x)
struct _w_label_class {
	struct _w_control_class control;
	wresult (*get_image)(w_label *label, w_image *image);
	wresult (*get_text)(w_label *label,w_alloc alloc,void* user_data,int enc);
	wresult (*set_image)(w_label *label, w_image *image);
	wresult (*set_text)(w_label *label, const char *string,int length,int enc);
};
SWT_PUBLIC wresult w_label_create(w_label *label, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_label* w_label_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_label_get_image(w_label *label, w_image *image);
SWT_PUBLIC wresult w_label_get_text(w_label *label,w_alloc alloc,void* user_data,int enc);
SWT_PUBLIC wresult w_label_set_image(w_label *label, w_image *image);
SWT_PUBLIC wresult w_label_set_text(w_label *label, const char *string,int length,int enc);

#define W_LABEL_CLASS(x) ((struct _w_label_class*)x)
#define W_LABEL_GET_CLASS(x) ((struct _w_label_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_LABEL_H_ */
