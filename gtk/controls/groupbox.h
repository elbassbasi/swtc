/*
 * groupbox.h
 *
 *  Created on: 17 mars 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_GROUPBOX_H_
#define GTK_CONTROLS_GROUPBOX_H_
#include "../widgets/shell.h"
typedef struct _w_groupbox {
	_w_composite composite;
} _w_groupbox;

typedef struct _w_groupbox_priv {
	_w_composite_priv composite;

} _w_groupbox_priv;
#define _W_GROUPBOX(x) ((_w_groupbox*)x)
#define _W_GROUPBOX_PRIV(x) ((_w_groupbox_priv*)x)
void _w_groupbox_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_groupbox_class *clazz);

#endif /* GTK_CONTROLS_GROUPBOX_H_ */
