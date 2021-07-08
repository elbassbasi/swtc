/*
 * tabview.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_TABVIEW_H_
#define GTK_CONTROLS_TABVIEW_H_
#include "../widgets/shell.h"

typedef struct _w_tabview {
	_w_composite composite;
	GtkWidget *image_close_button;
	w_imagelist *imagelist;
} _w_tabview;
enum {
	_W_TABVIEW_SIGNAL_SWITCH_PAGE, //
	_W_TABVIEW_SIGNAL_CLICKED, //
	_W_TABVIEW_SIGNAL_LAST
};
typedef struct _w_tabview_priv {
	_w_composite_priv composite;
	_gtk_signal signals[_W_TABVIEW_SIGNAL_LAST];
} _w_tabview_priv;
#define _W_TABVIEW(x) ((_w_tabview*)x)
#define _W_TABVIEW_PRIV(x) ((_w_tabview_priv*)x)
#define _W_TABVIEW_GET_ITEM_CLASS(tabview) (W_WIDGETDATA_CLASS(W_TABVIEW_GET_CLASS(tabview)->class_tabitem))
/*
 * tabitem
 */
typedef struct _w_tabitem {
	_w_item item;
} _w_tabitem;
#define _W_TABITEM(x) ((_w_tabitem*)x)

void _w_tabview_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_tabview_class *clazz);

#endif /* GTK_CONTROLS_TABVIEW_H_ */
