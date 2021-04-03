/*
 * toolbar.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef OSX_CONTROLS_TOOLBAR_H_
#define OSX_CONTROLS_TOOLBAR_H_
#include "../widgets/shell.h"
typedef struct _w_toolbar {
	_w_composite composite;
} _w_toolbar;

typedef struct _w_toolbar_priv {
	_w_composite_priv composite;

} _w_toolbar_priv;
#define _W_TOOLBAR(x) ((_w_toolbar*)x)
#define _W_TOOLBAR_PRIV(x) ((_w_toolbar_priv*)x)
/*
 * toolitem
 */
typedef struct _w_toolitem {
	_w_item item;
} _w_toolitem;
#define _W_TOOLITEM(x) ((_w_toolitem*)x)

void _w_toolbar_class_init(struct _w_toolbar_class *clazz);

#endif /* OSX_CONTROLS_TOOLBAR_H_ */
