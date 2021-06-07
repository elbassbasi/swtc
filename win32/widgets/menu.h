/*
 * Name:        menu.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_WIDGETS_MENU_H_
#define WIN32_WIDGETS_MENU_H_
#include "item.h"
#define STATE_MENU_HAS_LOCATION (1 << (STATE_CONTROL_END + 1))
/*
 * menu
 */
enum {
	_MENU_FLAGS_ID = (1 << 1), //
	_MENU_FLAGS_CHECK = (1 << 2), //
	_MENU_ID_ID = (1 << 15),
	_MENU_ID_STYLE_CHECK = (1 << 14),
	_MENU_ID_ACCEL = (1 << 14),
	_MENU_ID_MASK = 0x3FFF,
	_MENU_HELPID_BIT_CHECK = (1 << 31),
	_MENU_HELPID_MASK = (0x7F << 23),
	_MENU_HELPID_ADD = (1 << 23),
	ID_TOOLTIP_TIMER = 110,
};
typedef struct _w_menu_id {
	wushort flags;
	wushort index;
	HMENU menu;
	HBITMAP image;
} _w_menu_id;
typedef struct _w_menu_ids {
	int alloc;
	int count;
	_w_menu_id id[0];
} _w_menu_ids;
typedef struct _w_menu _w_menu;
struct _w_menu {
	_w_widget widget;
	_w_menu *next;
	_w_menu *prev;
	w_control *parent;
	HMENU handle;
	_w_menu_ids *ids;
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
	HMENU menu;
} _w_menuitem;
#define _W_MENUITEM(x) ((_w_menuitem*)x)
/*
 * private
 */
typedef struct _w_menu_priv _w_menu_priv;
struct _w_menu_priv {
	_w_widget_priv widget;
};
#define _W_MENU_PRIV(x) ((_w_menu_priv*)x)
/*
 * functions
 */
wresult _w_menuitem_fill_accel(ACCEL *accel, struct _w_accel_id *item);
wresult _w_menu_get_shell(w_widget *menu, w_shell **shell);
void _w_menu_class_init(struct _w_menu_class *clazz);
/*
 * message
 */
wresult _MENU_WM_COMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _MENU_WM_MENUCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _MENU_WM_INITMENUPOPUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _MENU_WM_UNINITMENUPOPUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _MENU_WM_MENUCHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _MENU_WM_MENUSELECT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _MENU_WM_DRAWITEM(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _MENU_WM_MEASUREITEM(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
#endif /* WIN32_WIDGETS_MENU_H_ */
