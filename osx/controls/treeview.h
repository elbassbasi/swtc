/*
 * Name:        treeview.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef OSX_CONTROLS_TREEVIEW_H_
#define OSX_CONTROLS_TREEVIEW_H_
#include "listview.h"
/*
 * treeitem
 */
typedef struct _w_treeitem {
	_w_item item;
	SWTTreeItem *handle;
	NSOutlineView *view;
} _w_treeitem;
#define _W_TREEITEM(x) ((_w_treeitem*)x)
/*
 * treeview
 */
typedef struct _w_treeview {
	_w_listviewbase base;
} _w_treeview;
#define _W_TREEVIEW(x) ((_w_tree*)x)
/*
 * private
 */
typedef struct _w_treeview_priv {
	_w_listviewbase_priv base;
} _w_treeview_priv;
#define _W_TREEVIEW_PRIV(x) ((_w_treeview_priv*)x)
/*
 * functions
 */
void _w_treeview_class_init(struct _w_treeview_class *clazz);
/*
 * messages
 */
#endif /* OSX_CONTROLS_TREEVIEW_H_ */
