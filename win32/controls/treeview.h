/*
 * Name:        treeview.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_CONTROLS_TREEVIEW_H_
#define WIN32_CONTROLS_TREEVIEW_H_
#include "listview.h"
#if NTDDI_VERSION < 0x06000000
#define TVS_EX_MULTISELECT 0x2
#define TVS_EX_DOUBLEBUFFER 0x4
#define TVS_EX_NOINDENTSTATE 0x8
#define TVS_EX_RICHTOOLTIP 0x10
#define TVS_EX_AUTOHSCROLL 0x20
#define TVS_EX_FADEINOUTEXPANDOS 0x40
#define TVS_EX_PARTIALCHECKBOXES 0x80
#define TVS_EX_EXCLUSIONCHECKBOXES 0x100
#define TVS_EX_DIMMEDCHECKBOXES 0x200
#define TVS_EX_DRAWIMAGEASYNC 0x400
#endif
/*
 * treeitem
 */
typedef struct _w_treeitem {
	_w_item item;
	HTREEITEM htreeitem;
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
#endif /* WIN32_CONTROLS_TREEVIEW_H_ */
