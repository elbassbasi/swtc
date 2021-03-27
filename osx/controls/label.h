/*
 * label.h
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */

#ifndef OSX_CONTROLS_LABEL_H_
#define OSX_CONTROLS_LABEL_H_
#include "../widgets/shell.h"
typedef struct _w_label {
	_w_control control;
} _w_label;

typedef struct _w_label_priv {
	_w_control_priv control;

} _w_label_priv;
#define _W_LABEL(x) ((_w_label*)x)
#define _W_LABEL_PRIV(x) ((_w_label_priv*)x)

void _w_label_class_init(struct _w_label_class *clazz);


#endif /* OSX_CONTROLS_LABEL_H_ */
