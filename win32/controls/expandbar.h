/*
 * expandbar.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef WIN32_CONTROLS_EXPANDBAR_H_
#define WIN32_CONTROLS_EXPANDBAR_H_
#include "../widgets/shell.h"
typedef struct _w_expandbar {
	_w_composite composite;
} _w_expandbar;

typedef struct _w_expandbar_priv {
	_w_composite_priv composite;

} _w_expandbar_priv;
#define _W_EXPANDBAR(x) ((_w_expandbar*)x)
#define _W_EXPANDBAR_PRIV(x) ((_w_expandbar_priv*)x)
/*
 * expanditem
 */
typedef struct _w_expanditem {
	_w_item item;
} _w_expanditem;
#define _W_EXPANDITEM(x) ((_w_expanditem*)x)

void _w_expandbar_class_init(struct _w_expandbar_class *clazz);



#endif /* WIN32_CONTROLS_EXPANDBAR_H_ */
