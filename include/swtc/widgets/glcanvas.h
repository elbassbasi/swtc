/*
 * Name:        glcanvas.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_GLCANVAS_H_
#define SWT_WIDGETS_GLCANVAS_H_
#include "canvas.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_glcanvas {
	w_canvas canvas;
	void *handle[2];
} w_glcanvas;
#define W_GLCANVAS(x) ((w_glcanvas*)x)
typedef struct w_gldata {
	unsigned doubleBuffer :1;
	unsigned stereo :1;
	int redSize;
	int greenSize;
	int blueSize;
	int alphaSize;
	int depthSize;
	int stencilSize;
	int accumRedSize;
	int accumGreenSize;
	int accumBlueSize;
	int accumAlphaSize;
	int sampleBuffers;
	int samples;
	w_glcanvas *shareContext;
} w_gldata;
struct _w_glcanvas_class {
	struct _w_canvas_class canvas;
	wresult (*init_gldata)(w_glcanvas *glcanvas, w_gldata *data);
	wresult (*get_gldata)(w_glcanvas *glcanvas, w_gldata *data);
	wresult (*is_current)(w_glcanvas *glcanvas);
	wresult (*set_current)(w_glcanvas *glcanvas);
	wresult (*swap_buffers)(w_glcanvas *glcanvas);
};
SWT_PUBLIC wresult w_glcanvas_create(w_glcanvas *glcanvas,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_glcanvas* w_glcanvas_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_glcanvas_init_gldata(w_glcanvas *glcanvas, w_gldata *data);
SWT_PUBLIC wresult w_glcanvas_get_gldata(w_glcanvas *glcanvas, w_gldata *data);
SWT_PUBLIC wresult w_glcanvas_is_current(w_glcanvas *glcanvas);
SWT_PUBLIC wresult w_glcanvas_set_current(w_glcanvas *glcanvas);
SWT_PUBLIC wresult w_glcanvas_swap_buffers(w_glcanvas *glcanvas);

#define W_GLCANVAS_CLASS(x) ((struct _w_glcanvas_class*)x)
#define W_GLCANVAS_GET_CLASS(x) ((struct _w_glcanvas_class*)W_WIDGET_GET_CLASS(x))
#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_GLCANVAS_H_ */
