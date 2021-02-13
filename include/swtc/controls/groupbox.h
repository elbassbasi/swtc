/*
 * Name:        group.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_GROUPBOX_H_
#define SWTC_CONTROLS_GROUPBOX_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_groupbox {
	w_composite composite;
	void *handle[0x10];
} w_groupbox;

#define W_GROUPBOX(x) ((w_groupbox*)x)

struct _w_groupbox_class {
	struct _w_composite_class composite;
	wresult (*get_text)(w_groupbox *group,w_alloc alloc,void* user_data,int enc);
	wresult (*set_text)(w_groupbox *group, const char *text,int length,int enc);
};
SWT_PUBLIC wresult w_groupbox_create(w_groupbox *group, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_groupbox* w_groupbox_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_groupbox_get_text(w_groupbox *group,w_alloc alloc,void* user_data,int enc);
SWT_PUBLIC wresult w_groupbox_set_text(w_groupbox *group, const char *text,int length,int enc);
#define W_GROUPBOX_CLASS(x) ((struct _w_groupbox_class*)x)
#define W_GROUPBOX_GET_CLASS(x) ((struct _w_groupbox_class*)W_WIDGET_GET_CLASS(x))
#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_GROUPBOX_H_ */
