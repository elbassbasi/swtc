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
} _w_treeview_priv;
#define _W_TREEVIEW_PRIV(x) ((_w_treeview_priv*)x)
/*
 * functions
 */
void _w_treeitem_remove_children_flags(GtkTreeModel *modelHandle,
		GtkTreeIter *parent);
/*
 * signals
 */
wresult _gtk_treeview_get_focusitem(w_widget *widget, _w_treeitem *item,
		_w_control_priv *priv);
void _gtk_treeview_send_default_selection(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_treeview_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_treeview_row_activated(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_treeview_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_treeview_button_release_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_treeview_changed(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_treeview_event_after(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_treeview_expand_collapse_cursor_row(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_treeview_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_treeview_motion_notify_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_treeview_row_deleted(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_treeview_row_has_child_toggled(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_treeview_start_interactive_search(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_treeview_test_collapse_row(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_treeview_test_expand_row(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_treeview_toggled(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
void _w_treeview_class_init(struct _w_treeview_class *clazz);
/*
 * messages
 */
#endif /* GTK_CONTROLS_TREEVIEW_H_ */
