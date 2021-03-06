/*
 * groupbox.h
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */

#ifndef OSX_CONTROLS_GROUPBOX_H_
#define OSX_CONTROLS_GROUPBOX_H_
#include "../widgets/shell.h"
typedef struct _w_groupbox {
	_w_composite composite;
	unsigned ignoreResize :1;
	int hMargin;
	int vMargin;
} _w_groupbox;

typedef struct _w_groupbox_priv {
	_w_composite_priv composite;

} _w_groupbox_priv;
#define _W_GROUPBOX(x) ((_w_groupbox*)x)
#define _W_GROUPBOX_PRIV(x) ((_w_groupbox_priv*)x)
void _w_groupbox_class_init(struct _w_groupbox_class *clazz);

#endif /* OSX_CONTROLS_GROUPBOX_H_ */
