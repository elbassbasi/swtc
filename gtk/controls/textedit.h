/*
 * textedit.h
 *
 *  Created on: 21 févr. 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_TEXTEDIT_H_
#define GTK_CONTROLS_TEXTEDIT_H_
#include "../widgets/shell.h"
#define STATE_TEXTEDIT_DOUBLE_CLICK (1 << (STATE_SCROLLABLE_END + 1))
typedef struct _w_textedit {
	_w_scrollable scrollable;
	char *message;
	double cachedAdjustment;
	double currentAdjustment;
	GtkTextMark *indexMark;
} _w_textedit;

typedef struct _w_textedit_priv {
	_w_scrollable_priv scrollable;

} _w_textedit_priv;
#define _W_TEXTEDIT(x) ((_w_textedit*)x)
#define _W_TEXTEDIT_PRIV(x) ((_w_textedit_priv*)x)

void _w_textedit_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_textedit_class *clazz);
#endif /* GTK_CONTROLS_TEXTEDIT_H_ */
