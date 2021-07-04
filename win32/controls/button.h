/*
 * button.h
 *
 *  Created on: 7 mars 2021
 *      Author: Azeddine
 */

#ifndef WIN32_CONTROLS_BUTTON_H_
#define WIN32_CONTROLS_BUTTON_H_
#include "../widgets/shell.h"
#define STATE_BUTTON_GRAYED (1 << (STATE_CONTROL_END + 1))
#define STATE_BUTTON_IGNORE_MOUSE (1 << (STATE_CONTROL_END + 2))
#ifndef BS_COMMANDLINK
#define BS_COMMANDLINK 0x0000000EL
#endif
typedef struct _w_button {
	_w_control control;
} _w_button;

typedef struct _w_button_priv {
	_w_control_priv control;
	int CHECK_WIDTH;
	int CHECK_HEIGHT;
} _w_button_priv;
#define _W_BUTTON(x) ((_w_button*)x)
#define _W_BUTTON_PRIV(x) ((_w_button_priv*)x)
/*
 * functions
 */
void _w_button_update_selection(w_button *button, int flags);
void _w_button_class_init(w_toolkit *toolkit, wushort classId,struct _w_button_class *clazz);

#endif /* WIN32_CONTROLS_BUTTON_H_ */
