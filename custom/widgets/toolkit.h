/*
 * toolkit.h
 *
 *  Created on: 28 juin 2021
 *      Author: Azeddine
 */

#ifndef CUSTOM_WIDGETS_TOOLKIT_H_
#define CUSTOM_WIDGETS_TOOLKIT_H_
#include "canvas.h"
typedef struct c_toolkit {
	w_toolkit toolkit;
	struct _w_toolkit_class class_toolkit;
	w_toolkit *system_toolkit;
	w_theme *theme;
	struct _w_toolkit_classes classes;
} c_toolkit;
extern c_toolkit *ctoolkit;
CSWT_PUBLIC w_toolkit* w_toolkit_get_custom();
void w_toolkit_custom_init(c_toolkit *toolkit);
void w_toolkit_custom_init_clazz(c_toolkit *toolkit);
/*  */
void cw_button_init_class(w_toolkit *toolkit, wushort classId,
		struct _w_widget_class *clazz);
void cw_expandbar_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_expandbar_class *clazz);
#endif /* CUSTOM_WIDGETS_TOOLKIT_H_ */
