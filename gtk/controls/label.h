/*
 * label.h
 *
 *  Created on: 21 févr. 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_LABEL_H_
#define GTK_CONTROLS_LABEL_H_
#include "../widgets/shell.h"
typedef struct _w_hyperlink_id {
	w_range offset;
	w_range id;
	int mnemonic;
} _w_hyperlink_id;
typedef struct _w_hyperlink {
	PangoLayout *layout;
	char *text;
	int text_length;
	int ids_length;
	w_range selection;
	_w_hyperlink_id ids[0];
} _w_hyperlink;
typedef struct _w_label {
	_w_control control;
	_w_hyperlink *hyperlink;
} _w_label;

typedef struct _w_label_priv {
	_w_control_priv control;

} _w_label_priv;
#define _W_LABEL(x) ((_w_label*)x)
#define _W_LABEL_PRIV(x) ((_w_label_priv*)x)

void _w_label_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_label_class *clazz);
#endif /* GTK_CONTROLS_LABEL_H_ */
