/*
 * progressbar.h
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */

#ifndef WIN32_CONTROLS_PROGRESSBAR_H_
#define WIN32_CONTROLS_PROGRESSBAR_H_
#include "../widgets/shell.h"
typedef struct _w_progressbar {
	_w_control control;
} _w_progressbar;

typedef struct _w_progressbar_priv {
	_w_control_priv control;
} _w_progressbar_priv;

#define _W_PROGRESSBAR(x) ((_w_progressbar*)x)
#define _W_PROGRESSBAR_PRIV(x) ((_w_progressbar_priv*)x)
void _w_progressbar_class_init(w_toolkit *toolkit, wushort classId,struct _w_progressbar_class *clazz);


#endif /* WIN32_CONTROLS_PROGRESSBAR_H_ */
