/*
 * slider.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef WIN32_CONTROLS_SLIDER_H_
#define WIN32_CONTROLS_SLIDER_H_
#include "../widgets/shell.h"
#define STATE_SLIDER_IGNORE_SELECTION (1 << (STATE_CONTROL_END + 1))
typedef struct _w_slider {
	_w_control control;
	int increment;
	int pageIncrement;
} _w_slider;

typedef struct _w_slider_priv {
	_w_control_priv control;
	WNDPROC def_scale_proc;

} _w_slider_priv;
#define _W_SLIDER(x) ((_w_slider*)x)
#define _W_SLIDER_PRIV(x) ((_w_slider_priv*)x)


void _w_slider_class_init(struct _w_slider_class *clazz);



#endif /* WIN32_CONTROLS_SLIDER_H_ */
