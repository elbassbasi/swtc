/*
 * toolbar.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_TOOLBAR_H_
#define GTK_CONTROLS_TOOLBAR_H_
#include "../widgets/shell.h"
typedef struct _w_toolbar {
	_w_composite composite;
	w_imagelist *imagelist;
} _w_toolbar;
enum {
	_W_TOOLBAR_SIGNAL_CLICKED,
	_W_TOOLBAR_SIGNAL_CLICKED_ARROW, //
	_W_TOOLBAR_SIGNAL_MENU_PROXY, //
	_W_TOOLBAR_LAST
};
typedef struct _w_toolbar_priv {
	_w_composite_priv composite;
	_gtk_signal signals[_W_TOOLBAR_LAST];

} _w_toolbar_priv;
#define _W_TOOLBAR(x) ((_w_toolbar*)x)
#define _W_TOOLBAR_PRIV(x) ((_w_toolbar_priv*)x)
#define _W_TOOLBAR_GET_ITEM_CLASS(toolbar) (W_WIDGETDATA_CLASS(W_TOOLBAR_GET_CLASS(toolbar)->class_toolitem))
/*
 * toolitem
 */
typedef struct _w_toolitem {
	_w_item item;
	GtkToolItem *toolItem;
} _w_toolitem;
#define _W_TOOLITEM(x) ((_w_toolitem*)x)
wresult _w_toolitem_get_control(w_toolitem *item, w_control **control);
void _w_toolitem_resize_handle(w_toolitem *item, int width, int height);
void _w_toolbar_relayout(w_toolbar *toolbar);
void _w_toolbar_class_init(struct _w_toolbar_class *clazz);

#endif /* GTK_CONTROLS_TOOLBAR_H_ */
