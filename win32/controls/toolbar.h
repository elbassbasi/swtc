/*
 * toolbar.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef WIN32_CONTROLS_TOOLBAR_H_
#define WIN32_CONTROLS_TOOLBAR_H_
#include "../widgets/shell.h"
#define STATE_IGNORE_RESIZE (1 << (STATE_COMPOSITE_END + 0))
#define STATE_IGNORE_MOUSE (1 << (STATE_COMPOSITE_END + 1))
typedef struct _w_toolbar_id {
	char *tooltips;
} _w_toolbar_id;
typedef struct _w_toolbar_ids {
	int alloc;
	int count;
	_w_toolbar_id id[0];
} _w_toolbar_ids;
typedef struct _w_toolbar {
	_w_composite composite;
	w_imagelist *imagelist;
	_w_toolbar_ids *ids;
} _w_toolbar;

typedef struct _w_toolbar_priv {
	_w_composite_priv composite;

} _w_toolbar_priv;
#define _W_TOOLBAR(x) ((_w_toolbar*)x)
#define _W_TOOLBAR_PRIV(x) ((_w_toolbar_priv*)x)
#define _W_TOOLBAR_GET_ITEM_CLASS(toolbar) (W_WIDGETDATA_CLASS(W_TOOLBAR_GET_CLASS(toolbar)->class_toolitem))
/*
 * toolitem
 */
typedef struct _w_toolitem {
	_w_item item;
} _w_toolitem;
#define _W_TOOLITEM(x) ((_w_toolitem*)x)
void _w_toolitem_resize_control(w_toolitem *item, w_control *control);
void _w_toolbar_layout_items(w_toolbar *toolbar);
void _w_toolbar_set_drop_down_items(w_toolbar *toolbar, wresult set);
void _w_toolbar_set_row_count(w_toolbar *toolbar, int count);
DWORD _w_toolbar_widget_item_style(wuint64 style);
void _w_toolbar_class_init(struct _w_toolbar_class *clazz);

#endif /* WIN32_CONTROLS_TOOLBAR_H_ */
