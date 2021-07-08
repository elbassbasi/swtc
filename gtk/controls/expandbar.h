/*
 * expandbar.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_EXPANDBAR_H_
#define GTK_CONTROLS_EXPANDBAR_H_
#include "../widgets/shell.h"
typedef struct _w_expandbar {
	_w_composite composite;
	w_imagelist *imagelist;
	int spacing;
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
	GtkWidget *handle;
} _w_expanditem;
#define _W_EXPANDITEM(x) ((_w_expanditem*)x)
wresult _w_expanditem_get_height(w_expanditem *expanditem);
void _w_expandbar_layout_items(w_expandbar *expandbar);
GtkWidget* _w_expandbar_handle_scrolled(w_widget *control,
		_w_control_priv *priv);
void _w_expandbar_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_expandbar_class *clazz);

#endif /* GTK_CONTROLS_EXPANDBAR_H_ */
