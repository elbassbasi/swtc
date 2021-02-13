/*
 * Name:        composite.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef OSX_WIDGETS_COMPOSITE_H_
#define OSX_WIDGETS_COMPOSITE_H_
#include "scrollable.h"
/*
 * states
 */
#define STATE_FOREIGN_HANDLE (1 << (STATE_SCROLLABLE_END + 0))
#define STATE_COMPOSITE_END (STATE_SCROLLABLE_END + 1)
/*
 * composite
 */
typedef struct _w_composite {
	_w_scrollable scrollable;
	w_layout *layout;
	wushort layoutCount;
} _w_composite;
#define _W_COMPOSITE(x) ((_w_composite*)x)
/*
 * private
 */
typedef struct _w_composite_priv _w_composite_priv;
struct _w_composite_priv {
	_w_scrollable_priv scrollable;
	w_composite* (*find_deferred_control)(w_composite *composite,
			_w_composite_priv *priv);
};
#define _W_COMPOSITE_PRIV(x) ((_w_composite_priv*)x)
#define _W_COMPOSITE_GET_PRIV(x) ((_w_composite_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
wresult _w_composite_create_handle(w_widget *widget, _w_control_priv *priv);
void _w_composite_class_init(struct _w_composite_class *clazz);
/*
 * messages
 */
#endif /* OSX_WIDGETS_COMPOSITE_H_ */
