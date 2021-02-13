/*
 * Name:        listview.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_CONTROLS_LISTVIEW_H_
#define WIN32_CONTROLS_LISTVIEW_H_
#include "../widgets/shell.h"
/*
 * state
 */
#define STATE_TABLE_END (STATE_COMPOSITE_END + 1)
/*
 * listitem
 */
typedef struct _w_listitem {
	_w_item item;
} _w_listitem;
#define _W_LISTITEM(x) ((_w_listitem*)x)
/*
 * listview
 */
typedef struct _w_listviewbase {
	_w_composite composite;
	unsigned explorerTheme :1;
	unsigned createdAsRTL :1;
	w_imagelist *imagelist;
} _w_listviewbase;
#define _W_LISTVIEWBASE(x) ((_w_listviewbase*)x)
#define _W_LISTVIEWBASE_GET_ITEM_CLASS(list) (W_WIDGETDATA_CLASS(W_LISTVIEWBASE_GET_CLASS(list)->class_item))
#define _W_LISTVIEWBASE_GET_COLUMN_CLASS(list) (W_WIDGETDATA_CLASS(W_LISTVIEWBASE_GET_CLASS(list)->class_column))
typedef struct _w_listview {
	_w_listviewbase base;
} _w_listview;
#define _W_LISTVIEW(x) ((_w_listview*)x)
/*
 * private
 */
typedef struct _w_listviewbase_priv {
	_w_composite_priv composite;
}_w_listviewbase_priv;
#define _W_LISTVIEWBASE_PRIV(x) ((_w_listviewbase_priv*)x)
#define _W_LISTVIEWBASE_GET_PRIV(x) ((_w_listviewbase_priv*)_w_widget_get_priv(W_WIDGET(x)))
void _w_listviewbase_class_init(struct _w_listviewbase_class *clazz);
typedef struct _w_listview_priv {
	_w_listviewbase_priv base;
} _w_listview_priv;
#define _W_LISTVIEW_PRIV(x) ((_w_listview_priv*)x)
void _w_listview_class_init(struct _w_listview_class *clazz);
#endif /* WIN32_CONTROLS_TABLE_H_ */
