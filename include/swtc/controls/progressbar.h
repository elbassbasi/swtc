/*
 * Name:        progressbar.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_PROGRESSBAR_H_
#define SWTC_CONTROLS_PROGRESSBAR_H_
#include "../widgets/control.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_progressbar {
	w_control control;
	void *handle[8];
} w_progressbar;
#define W_PROGRESSBAR(x) ((w_progressbar*)x)
struct _w_progressbar_class {
	struct _w_control_class control;
	wresult (*get_maximum)(w_progressbar *progressbar);
	wresult (*get_selection)(w_progressbar *progressbar);
	wresult (*get_state)(w_progressbar *progressbar);
	wresult (*set_maximum)(w_progressbar *progressbar, int value);
	wresult (*set_selection)(w_progressbar *progressbar, int value);
	wresult (*set_state)(w_progressbar *progressbar, int state);
};
SWT_PUBLIC wresult w_progressbar_create(w_progressbar *progressbar,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_progressbar* w_progressbar_new(struct w_toolkit *toolkit, w_composite *parent,
                                            wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_progressbar_get_maximum(w_progressbar *progressbar);
SWT_PUBLIC wresult w_progressbar_get_selection(w_progressbar *progressbar);
SWT_PUBLIC wresult w_progressbar_get_state(w_progressbar *progressbar);
SWT_PUBLIC wresult w_progressbar_set_maximum(w_progressbar *progressbar, int value);
SWT_PUBLIC wresult w_progressbar_set_selection(w_progressbar *progressbar, int value);
SWT_PUBLIC wresult w_progressbar_set_state(w_progressbar *progressbar, int state);

#define W_PROGRESSBAR_CLASS(x) ((struct _w_progressbar_class*)x)
#define W_PROGRESSBAR_GET_CLASS(x) ((struct _w_progressbar_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_PROGRESSBAR_H_ */
