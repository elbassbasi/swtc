/*
 * slider.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_SLIDER_H_
#define GTK_CONTROLS_SLIDER_H_
#include "../widgets/shell.h"
typedef struct _w_slider {
	_w_control control;
} _w_slider;

typedef struct _w_slider_priv {
	_w_control_priv control;
	_gtk_signal signals[3];
	guint signal_scale_value_changed;
	guint signal_value_changed;
	guint signal_change_value;

} _w_slider_priv;
#define _W_SLIDER(x) ((_w_slider*)x)
#define _W_SLIDER_PRIV(x) ((_w_slider_priv*)x)

void _w_slider_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_slider_class *clazz);

#endif /* GTK_CONTROLS_SLIDER_H_ */
