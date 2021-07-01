/*
 * button.h
 *
 *  Created on: 28 juin 2021
 *      Author: Azeddine
 */

#ifndef CUSTOM_CONTROLS_BUTTON_H_
#define CUSTOM_CONTROLS_BUTTON_H_
#include "../widgets/canvas.h"
typedef struct cw_button_priv {
	int state;
	char *text;
	w_image image;
}cw_button_priv;
void cw_button_init_class(w_toolkit *toolkit, wushort classId,struct _w_widget_class *clazz);

#endif /* CUSTOM_CONTROLS_BUTTON_H_ */
