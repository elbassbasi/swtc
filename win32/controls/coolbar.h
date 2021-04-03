/*
 * coolbar.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef WIN32_CONTROLS_COOLBAR_H_
#define WIN32_CONTROLS_COOLBAR_H_
#include "../widgets/shell.h"
typedef struct _w_coolbar {
	_w_composite composite;
} _w_coolbar;

typedef struct _w_coolbar_priv {
	_w_composite_priv composite;

} _w_coolbar_priv;
#define _W_COOLBAR(x) ((_w_coolbar*)x)
#define _W_COOLBAR_PRIV(x) ((_w_coolbar_priv*)x)
/*
 * coolitem
 */
typedef struct _w_coolitem {
	_w_item item;
} _w_coolitem;
#define _W_COOLITEM(x) ((_w_coolitem*)x)

void _w_coolbar_class_init(struct _w_coolbar_class *clazz);



#endif /* WIN32_CONTROLS_COOLBAR_H_ */
