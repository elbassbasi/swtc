/*
 * Name:        treeview.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_CONTROLS_TREEVIEW_H_
#define GTK_CONTROLS_TREEVIEW_H_
#include "listview.h"
/*
 * treeitem
 */
typedef struct _w_treeitem {
	_w_item item;
	GtkTreeIter iter;
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
	int signal_changed_id;
	int signal_row_activated_id;
	int signal_test_expand_row_id;
	int signal_test_collapse_row_id;
	int signal_expand_collapse_cursor_row_id;
	int signal_row_has_child_toggled_id;
	int signal_start_interactive_search_id;
	int signal_row_inserted_id;
	int signal_row_deleted_id;
} _w_treeview_priv;
#define _W_TREEVIEW_PRIV(x) ((_w_treeview_priv*)x)
/*
 * functions
 */
void _w_treeitem_remove_children_flags(GtkTreeModel *modelHandle,
		GtkTreeIter *parent);
void _w_treeview_class_init(struct _w_treeview_class *clazz);
/*
 * messages
 */
#endif /* GTK_CONTROLS_TREEVIEW_H_ */
