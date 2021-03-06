/*
 * button.h
 *
 *  Created on: Mar 5, 2021
 *      Author: azeddine
 */

#ifndef OSX_CONTROLS_BUTTON_H_
#define OSX_CONTROLS_BUTTON_H_
#include "../widgets/shell.h"
#define STATE_BUTTON_GRAYED (1 << (STATE_CONTROL_END + 1))
#define STATE_BUTTON_END (STATE_CONTROL_END + 1)
typedef struct _w_button {
	struct _w_control control;
} _w_button;

typedef struct _w_button_priv {
	_w_control_priv control;

}_w_button_priv;
#define _W_BUTTON(x) ((_w_button*)x)
#define _W_BUTTON_PRIV(x) ((_w_control_priv*)x)

int _w_button_check_style(int style);
/*
 * public function
 */
int _w_button_get_alignment(w_button *button);
wresult _w_button_get_grayed(w_button *button);
wresult _w_button_get_image(w_button *button, w_image *image);
wresult _w_button_get_selection(w_button *button);
wresult _w_button_get_text(w_button *button, w_alloc alloc,void* user_data);
wresult _w_button_set_alignment(w_button *button, int alignment);
wresult _w_button_set_grayed(w_button *button, int grayed);
wresult _w_button_set_image(w_button *button, w_image *image);
wresult _w_button_set_selection(w_button *button, int selected);
wresult _w_button_set_text(w_button *button, const char *text);
/*
 *
 */
void _w_button_class_init(struct _w_button_class *clazz);
#endif /* OSX_CONTROLS_BUTTON_H_ */
