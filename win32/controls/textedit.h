/*
 * textedit.h
 *
 *  Created on: 12 mars 2021
 *      Author: Azeddine
 */

#ifndef WIN32_CONTROLS_TEXTEDIT_H_
#define WIN32_CONTROLS_TEXTEDIT_H_
#include "../widgets/shell.h"
#define STATE_TEXTEDIT_DOUBLE_CLICK (1 << (STATE_SCROLLABLE_END + 1))
#define STATE_TEXTEDIT_IGNORE_CHARACTER (1 << (STATE_SCROLLABLE_END + 2))
#define STATE_TEXTEDIT_IGNORE_MODIFY (1 << (STATE_SCROLLABLE_END + 3))
#define STATE_TEXTEDIT_ALLOW_PASSWORD_CHAR (1 << (STATE_SCROLLABLE_END + 4))
typedef struct _w_textedit {
	_w_scrollable scrollable;
	WCHAR* message;
	int message_length;
	int tabs;
} _w_textedit;

typedef struct _w_textedit_priv {
	_w_scrollable_priv scrollable;

} _w_textedit_priv;
#define _W_TEXTEDIT(x) ((_w_textedit*)x)
#define _W_TEXTEDIT_PRIV(x) ((_w_textedit_priv*)x)
wresult _w_textedit_get_caret_position(w_textedit *text, int enc);
int _w_textedit_get_message_0(w_textedit *text,WCHAR** str,int* length);
void _w_textedit_class_init(struct _w_textedit_class *clazz);
#endif /* WIN32_CONTROLS_TEXTEDIT_H_ */
