/*
 * tabview.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef WIN32_CONTROLS_TABVIEW_H_
#define WIN32_CONTROLS_TABVIEW_H_
#include "../widgets/shell.h"
#define STATE_CREATE_AS_RTL (1 << (STATE_COMPOSITE_END + 0))
#define STATE_CLOSE_HOVER (1 << (STATE_COMPOSITE_END + 1))

typedef struct _w_tabview {
	_w_composite composite;
	w_imagelist *imagelist;
	int _currentHover;
} _w_tabview;

typedef struct _w_tabview_priv {
	_w_composite_priv composite;

} _w_tabview_priv;
#define _W_TABVIEW(x) ((_w_tabview*)x)
#define _W_TABVIEW_PRIV(x) ((_w_tabview_priv*)x)
#define _W_TABVIEW_GET_ITEM_CLASS(tabview) (W_WIDGETDATA_CLASS(W_TABVIEW_GET_CLASS(tabview)->class_tabitem))
/*
 * tabitem
 */
typedef struct _w_tabitem {
	_w_item item;
} _w_tabitem;
#define _W_TABITEM(x) ((_w_tabitem*)x)

void _w_tabview_class_init(struct _w_tabview_class *clazz);

#endif /* WIN32_CONTROLS_TABVIEW_H_ */
