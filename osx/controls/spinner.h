/*
 * spinner.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef OSX_CONTROLS_SPINNER_H_
#define OSX_CONTROLS_SPINNER_H_
#include "../widgets/shell.h"
typedef struct _w_spinner {
	_w_composite composite;
} _w_spinner;

typedef struct _w_spinner_priv {
	_w_composite_priv composite;

} _w_spinner_priv;
#define _W_SPINNER(x) ((_w_spinner*)x)
#define _W_SPINNER_PRIV(x) ((_w_spinner_priv*)x)


void _w_spinner_class_init(struct _w_spinner_class *clazz);



#endif /* OSX_CONTROLS_SPINNER_H_ */
