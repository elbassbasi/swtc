/*
 * Name:        canvas.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef OSX_WIDGETS_CANVAS_H_
#define OSX_WIDGETS_CANVAS_H_
#include "composite.h"
/*
 * state
 */
#define STATE_CANVAS_END (STATE_COMPOSITE_END + 0)
/*
 * canvas
 */
typedef struct _w_canvas {
	_w_composite composite;
} _w_canvas;
#define _W_CANVAS(x) ((_w_canvas*)x)
/*
 * private
 */
typedef struct _w_canvas_priv _w_canvas_priv;
struct _w_canvas_priv {
	_w_composite_priv composite;
};
#define _W_CANVAS_PRIV(x) ((_w_canvas_priv*)x)
#define _W_CANVAS_GET_PRIV(x) ((_w_canvas_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
void _w_canvas_class_init(struct _w_canvas_class *clazz);
/*
 * messages
 */
#endif /* OSX_WIDGETS_CANVAS_H_ */
