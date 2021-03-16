/*
 * sash.h
 *
 *  Created on: 7 mars 2021
 *      Author: Azeddine
 */

#ifndef WIN32_CONTROLS_SASH_H_
#define WIN32_CONTROLS_SASH_H_
#include "../widgets/shell.h"
#define STATE_SASH_DRAGGING (1 << (STATE_CONTROL_END + 1))
typedef struct _w_sash {
	_w_control control;
	w_point start;
	w_point last;
} _w_sash;

typedef struct _w_sash_priv {
	_w_control_priv control;

} _w_sash_priv;
#define _W_SASH(x) ((_w_sash*)x)
#define _W_SASH_PRIV(x) ((_w_sash_priv*)x)
void _w_sash_class_init(struct _w_sash_class *clazz);
#endif /* WIN32_CONTROLS_SASH_H_ */
