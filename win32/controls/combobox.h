/*
 * combobox.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_COMBOBOX_H_
#define GTK_CONTROLS_COMBOBOX_H_
#include "../widgets/shell.h"
typedef struct _w_combobox {
	_w_composite composite;
	int scrollWidth;
	w_imagelist* imagelist;
} _w_combobox;

typedef struct _w_combobox_priv {
	_w_composite_priv composite;
	WNDPROC EditProc;
	WNDPROC ListProc;
} _w_combobox_priv;
#define _W_COMBOBOX(x) ((_w_combobox*)x)
#define _W_COMBOBOX_PRIV(x) ((_w_combobox_priv*)x)
#define _W_COMBOBOX_GET_ITEM_CLASS(combo) (W_WIDGETDATA_CLASS(W_COMBOBOX_GET_CLASS(combo)->class_comboitem))

/*
 * comboitem
 */
typedef struct _w_comboitem {
	_w_item item;
} _w_comboitem;
#define _W_COMBOITEM(x) ((_w_comboitem*)x)

void _w_combobox_class_init(w_toolkit *toolkit, wushort classId,struct _w_combobox_class *clazz);
#endif /* GTK_CONTROLS_COMBOBOX_H_ */
