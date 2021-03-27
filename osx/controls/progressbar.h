/*
 * progressbar.h
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */

#ifndef OSX_CONTROLS_PROGRESSBAR_H_
#define OSX_CONTROLS_PROGRESSBAR_H_
#include "../widgets/shell.h"
typedef struct _w_progressbar {
	_w_control control;
} _w_progressbar;

typedef struct _w_progressbar_priv {
	_w_control_priv control;
} _w_progressbar_priv;

#define _W_PROGRESSBAR(x) ((_w_progressbar*)x)
#define _W_PROGRESSBAR_PRIV(x) ((_w_progressbar_priv*)x)
void _w_progressbar_class_init(struct _w_progressbar_class *clazz);


#endif /* OSX_CONTROLS_PROGRESSBAR_H_ */
