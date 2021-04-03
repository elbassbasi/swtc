/*
 * combobox.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "combobox.h"
#include "../widgets/toolkit.h"


void _w_combobox_class_init(struct _w_combobox_class *clazz){
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
}
