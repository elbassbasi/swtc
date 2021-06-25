/*
 * expandbar.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef WIN32_CONTROLS_EXPANDBAR_H_
#define WIN32_CONTROLS_EXPANDBAR_H_
#include "../widgets/shell.h"
typedef struct _w_expanditem_handle {
	unsigned expanded :1;
	unsigned hover :1;
	int image;
	w_rect rect;
	char *text;
	void *userdata;
	w_control *control;
} _w_expanditem_handle;
typedef struct _w_expanditem_handles {
	int alloc;
	int count;
	_w_expanditem_handle items[0];
} _w_expanditem_handles;
typedef struct _w_expandbar {
	_w_composite composite;
	_w_expanditem_handles *items;
	w_imagelist *imagelist;
	HFONT hFont;
	int spacing;
	int focusItem;
	int yCurrentScroll;
} _w_expandbar;

typedef struct _w_expandbar_priv {
	_w_composite_priv composite;

} _w_expandbar_priv;
#define _W_EXPANDBAR(x) ((_w_expandbar*)x)
#define _W_EXPANDBAR_PRIV(x) ((_w_expandbar_priv*)x)
#define _W_EXPANDBAR_GET_ITEM_CLASS(expandbar) (W_WIDGETDATA_CLASS(W_EXPANDBAR_GET_CLASS(expandbar)->class_expanditem))

/*
 * expanditem
 */
typedef struct _w_expanditem {
	_w_item item;
} _w_expanditem;
#define _W_EXPANDITEM(x) ((_w_expanditem*)x)
void _w_expanditem_redraw(w_expandbar *expandbar, _w_expanditem_handle *_item,
		int all);
int _w_expandbar_is_app_themed(w_expandbar *expandbar);
int _w_expandbar_get_band_height(w_expandbar *expandbar);
void _w_expandbar_layout_items(w_expandbar *expandbar, int index);
void _w_expandbar_set_scrollbar(w_expandbar *expandbar);
void _w_expandbar_show_item(w_expandbar *expandbar, int index,
		_w_expanditem_handle *_item);
void _w_expandbar_class_init(struct _w_expandbar_class *clazz);

#endif /* WIN32_CONTROLS_EXPANDBAR_H_ */
