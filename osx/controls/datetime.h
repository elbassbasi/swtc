/*
 * datetime.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef OSX_CONTROLS_DATETIME_H_
#define OSX_CONTROLS_DATETIME_H_
#include "../widgets/shell.h"

typedef struct _w_datetime {
	_w_composite composite;
} _w_datetime;

typedef struct _w_datetime_priv {
	_w_composite_priv composite;

} _w_datetime_priv;
#define _W_DATETIME(x) ((_w_datetime*)x)
#define _W_DATETIME_PRIV(x) ((_w_datetime_priv*)x)


void _w_datetime_class_init(struct _w_datetime_class *clazz);



#endif /* OSX_CONTROLS_DATETIME_H_ */
