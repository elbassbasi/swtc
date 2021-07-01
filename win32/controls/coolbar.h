/*
 * coolbar.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef WIN32_CONTROLS_COOLBAR_H_
#define WIN32_CONTROLS_COOLBAR_H_
#include "../widgets/shell.h"
#define STATE_IGNORE_RESIZE (1 << (STATE_COMPOSITE_END + 0))


#define SEPARATOR_WIDTH 2
#define MAX_WIDTH 0x7FFF
#define DEFAULT_COOLBAR_WIDTH 0
#define DEFAULT_COOLBAR_HEIGHT 0
typedef struct _w_coolbar {
	_w_composite composite;
} _w_coolbar;

typedef struct _w_coolbar_priv {
	_w_composite_priv composite;

} _w_coolbar_priv;
#define _W_COOLBAR(x) ((_w_coolbar*)x)
#define _W_COOLBAR_PRIV(x) ((_w_coolbar_priv*)x)
#define _W_COOLBAR_GET_ITEM_CLASS(coolbar) (W_WIDGETDATA_CLASS(W_COOLBAR_GET_CLASS(coolbar)->class_coolitem))
/*
 * coolitem
 */
typedef struct _w_coolitem {
	_w_item item;
} _w_coolitem;
#define _W_COOLITEM(x) ((_w_coolitem*)x)
wresult _w_coolitem_is_last_item_of_row(w_coolbar *coolbar, int index);
int _w_coolitem_get_margin(w_coolbar *coolbar, int index);
void _w_coolbar_class_init(w_toolkit *toolkit, wushort classId,struct _w_coolbar_class *clazz);



#endif /* WIN32_CONTROLS_COOLBAR_H_ */
