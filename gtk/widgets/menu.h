/*
 * Name:        menu.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_WIDGETS_MENU_H_
#define GTK_WIDGETS_MENU_H_
#include "widget.h"
#include "item.h"
#define STATE_MENU_HAS_LOCATION (1 << (STATE_CONTROL_END + 1))
/*
 * menu
 */
typedef struct _w_menu _w_menu;
struct _w_menu {
	_w_widget widget;
	_w_menu *next;
	_w_menu *prev;
	w_control *parent;
	w_imagelist *imagelist;
	int x;
	int y;
};
#define _W_MENU(x) ((_w_menu*)x)
#define _W_MENU_GET_ITEM_CLASS(menu) (W_WIDGETDATA_CLASS(W_MENU_GET_CLASS(menu)->class_menuitem))
/*
 * menuitem
 */
typedef struct _w_menuitem {
	_w_item item;
	GtkWidget *widget;
} _w_menuitem;
#define _W_MENUITEM(x) ((_w_menuitem*)x)
/*
 * private
 */
typedef struct _w_menu_priv _w_menu_priv;
struct _w_menu_priv {
	_w_widget_priv widget;
	int signal_activate_id;
	int signal_select_id;
};
#define _W_MENU_PRIV(x) ((_w_menu_priv*)x)
#define _W_MENU_GET_PRIV(x) ((_w_menu_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
wresult _w_menu_get_shell(w_menu *menu, w_shell **shell);
void _w_menu_class_init(struct _w_menu_class *clazz);
#endif /* GTK_WIDGETS_MENU_H_ */
