/*
 * coolbar.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_COOLBAR_H_
#define GTK_CONTROLS_COOLBAR_H_
#include "../widgets/shell.h"
typedef struct _w_coolitem_handle {
	w_rect itemBounds;
	w_size preferredSize;
	w_size minimumSize;
	int requestedWidth;
	unsigned ideal :1;
	unsigned wrap;
	unsigned newrow :1;
	w_control *control;
} _w_coolitem_handle;
typedef struct _w_coolitem_handles {
	size_t alloc;
	size_t count;
	_w_coolitem_handle items[0];
} _w_coolitem_handles;
typedef struct _w_coolbar {
	_w_composite composite;
	_w_coolitem_handles *items;
	w_cursor *hoverCursor;
	w_cursor *dragCursor;
	unsigned isLocked :1;
} _w_coolbar;

typedef struct _w_coolbar_priv {
	_w_composite_priv composite;

} _w_coolbar_priv;
#define _W_COOLBAR(x) ((_w_coolbar*)x)
#define _W_COOLBAR_PRIV(x) ((_w_coolbar_priv*)x)
#define _W_COOLBAR_GET_ITEM_CLASS(coolbar) (W_WIDGETDATA_CLASS(W_COOLBAR_GET_CLASS(coolbar)->class_coolitem))
#define _W_COOLBAR_GROW 4
/*
 * coolitem
 */
typedef struct _w_coolitem {
	_w_item item;
	_w_coolitem_handle *handle;
} _w_coolitem;
#define _W_COOLITEM(x) ((_w_coolitem*)x)
void _w_coolitem_update_chevron(w_coolbar *coolbar, _w_coolitem_handle *_item);
void _w_coolbar_fix_point(w_coolbar *coolbar, w_size *result, w_size *pt);
void _w_coolbar_fix_rectangle(w_coolbar *coolbar, w_rect *result, w_rect *rect);
int _w_coolbar_get_width(w_coolbar *coolbar);
wresult _w_coolbar_layout_items(w_coolbar *coolbar);
void _w_coolbar_relayout(w_coolbar *coolbar);
void _w_coolbar_wrap_items(w_coolbar *coolbar, int maxWidth);
void _w_coolbar_class_init(struct _w_coolbar_class *clazz);

#endif /* GTK_CONTROLS_COOLBAR_H_ */
