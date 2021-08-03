/*
 * coolbar.h
 *
 *  Created on: 26 juil. 2021
 *      Author: azeddine
 */

#ifndef CUSTOM_CONTROLS_COOLBAR_H_
#define CUSTOM_CONTROLS_COOLBAR_H_
#include "../widgets/canvas.h"
typedef struct cw_coolitem {
	w_rect itemBounds;
	w_size preferredSize;
	w_size minimumSize;
	int requestedWidth;
	unsigned ideal :1;
	unsigned wrap :1;
	unsigned newrow :1;
	w_control *control;
} cw_coolitem;
typedef struct cw_coolbar_priv {
	w_array *items;
	w_cursor *hoverCursor;
	w_cursor *dragCursor;
	unsigned isLocked :1;
} cw_coolbar_priv;

void cw_coolbar_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_coolbar_class *clazz);
#endif /* CUSTOM_CONTROLS_COOLBAR_H_ */
