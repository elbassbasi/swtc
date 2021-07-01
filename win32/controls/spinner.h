/*
 * spinner.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef WIN32_CONTROLS_SPINNER_H_
#define WIN32_CONTROLS_SPINNER_H_
#include "../widgets/shell.h"
#define STATE_SPINNER_IGNORE_CHARACTER (1 << (STATE_COMPOSITE_END + 0))
typedef struct _w_spinner {
	_w_composite composite;
	HWND hwndText;
	HWND hwndUpDown;
	int pageIncrement;
	int digits;
} _w_spinner;

typedef struct _w_spinner_priv {
	_w_composite_priv composite;
	WNDPROC EditProc;
	WNDPROC UpDownProc;

} _w_spinner_priv;
#define _W_SPINNER(x) ((_w_spinner*)x)
#define _W_SPINNER_PRIV(x) ((_w_spinner_priv*)x)

wresult _w_spinner_set_selection_0(w_spinner *spinner,int value, int setPos, int setText, int notify);
void _w_spinner_class_init(w_toolkit *toolkit, wushort classId,struct _w_spinner_class *clazz);



#endif /* WIN32_CONTROLS_SPINNER_H_ */
