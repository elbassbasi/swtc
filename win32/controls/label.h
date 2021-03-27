/*
 * label.h
 *
 *  Created on: 12 mars 2021
 *      Author: Azeddine
 */

#ifndef WIN32_CONTROLS_LABEL_H_
#define WIN32_CONTROLS_LABEL_H_
#include "../widgets/shell.h"
#define STATE_LABEL_IMAGE_AND_TEXT (1 << (STATE_CONTROL_END + 1))
typedef struct _w_label {
	_w_control control;
	HBITMAP image;
} _w_label;

typedef struct _w_label_priv {
	_w_control_priv control;
	WNDPROC def_link_proc;
} _w_label_priv;
#define _W_LABEL(x) ((_w_label*)x)
#define _W_LABEL_PRIV(x) ((_w_label_priv*)x)

void _w_label_class_init(struct _w_label_class *clazz);
#endif /* WIN32_CONTROLS_LABEL_H_ */
