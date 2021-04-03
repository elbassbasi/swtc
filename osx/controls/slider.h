/*
 * slider.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef OSX_CONTROLS_SLIDER_H_
#define OSX_CONTROLS_SLIDER_H_
#include "../widgets/shell.h"
typedef struct _w_slider {
	_w_control control;
} _w_slider;

typedef struct _w_slider_priv {
	_w_control_priv control;

} _w_slider_priv;
#define _W_SLIDER(x) ((_w_slider*)x)
#define _W_SLIDER_PRIV(x) ((_w_slider_priv*)x)


void _w_slider_class_init(struct _w_slider_class *clazz);



#endif /* OSX_CONTROLS_SLIDER_H_ */
