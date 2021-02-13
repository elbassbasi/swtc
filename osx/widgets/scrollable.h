/*
 * Name:        scrollable.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef OSX_WIDGETS_SCROLLABLE_H_
#define OSX_WIDGETS_SCROLLABLE_H_
#include "control.h"
/*
 * states
 */
#define STATE_CANVAS (1 << (STATE_CONTROL_END + 0))
#define STATE_SCROLLABLE_END (STATE_CONTROL_END + 1)
/*
 * scrollable
 */
typedef struct _w_scrollable {
	_w_control control;
} _w_scrollable;
#define _W_SCROLLABLE(x) ((_w_scrollable*)x)
/*
 * private
 */
typedef struct _w_scrollable_priv _w_scrollable_priv;
struct _w_scrollable_priv {
	_w_control_priv control;
	NSView* (*scroll_view)(w_widget* widget);
};
#define _W_SCROLLABLE_PRIV(x) ((_w_scrollable_priv*)x)
#define _W_SCROLLABLE_GET_PRIV(x) ((_w_scrollable_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
void _w_scrollable_class_init(struct _w_scrollable_class *clazz);
/*
 * messages
 */
#endif /* OSX_WIDGETS_SCROLLABLE_H_ */
