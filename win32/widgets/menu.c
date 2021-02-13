/*
 * Name:        menu.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "shell.h"
#include "menu.h"
#include "toolkit.h"
#define MENU_CHECK  1 << 31
#define MENU_HAS_ACCELERATOR  1 << 30
/*
 * menuitem
 */
wresult _w_menuitem_copy(w_widgetdata *from, w_widgetdata *to) {
	_w_item_copy(from, to);
	_W_MENUITEM(to)->menu = _W_MENUITEM(from)->menu;
	return W_TRUE;
}
HMENU _w_menuitem_get_submenu(w_menuitem *item) {
	MENUITEMINFOW info;
	if (_W_ITEM(item)->index == -1) {
		return _W_MENUITEM(item)->menu;
	} else {
		info.cbSize = sizeof(info);
		info.fMask = MIIM_SUBMENU;
		info.hSubMenu = 0;
		if (GetMenuItemInfoW(_W_MENUITEM(item)->menu,
		_W_ITEM(item)->index, TRUE, &info)) {
			return info.hSubMenu;
		} else
			return 0;
	}
}
wresult _w_menuitem_get_data(w_item *item, void **data) {
	return W_FALSE;
}
wresult _w_menuitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_menuitem_set_data(w_item *item, void *data) {
	return W_FALSE;
}
wresult _w_menuitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	if (_W_ITEM(item)->index == -1)
		return W_FALSE;
	size_t newlength;
	wresult result = W_FALSE;
	WCHAR *s = _win_text_fix(text, length, &newlength, enc);
	MENUITEMINFOW info;
	if (s != 0) {
		info.cbSize = sizeof(info);
		info.fMask = MIIM_STRING;
		info.dwTypeData = s;
		info.cch = newlength;
		if (SetMenuItemInfoW(_W_MENUITEM(item)->menu,
		_W_ITEM(item)->index, TRUE, &info)) {
			result = W_TRUE;
		}
	}
	_win_text_free(text, s, newlength);
	return result;
}
wresult _w_menuitem_insert(w_menuitem *parent, w_menuitem *item, int style,
		int index) {
	MENUITEMINFOW info;
	MENUINFO _info;
	HMENU parentItem = _w_menuitem_get_submenu(parent), submenu = 0;
	if (parentItem == 0)
		return W_ERROR_INVALID_ARGUMENT;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID | MIIM_TYPE;
	info.fType = 0;
	info.wID = 0;
	int bits = W_CHECK | W_RADIO | W_PUSH | W_SEPARATOR | W_CASCADE;
	switch (style & bits) {
	case W_SEPARATOR:
		info.fType = MFT_SEPARATOR;
		break;
	case W_CHECK:
		info.wID = MENU_CHECK;
		break;
	case W_RADIO:
		info.fType = MFT_RADIOCHECK;
		break;
	case W_CASCADE:
		submenu = CreatePopupMenu();
		_info.cbSize = sizeof(_info);
		_info.fMask = MIM_MENUDATA | MIM_STYLE | MIM_HELPID;
		_info.dwMenuData = (ULONG_PTR) parentItem;
		_info.dwStyle = MNS_NOTIFYBYPOS;
		_info.dwContextHelpID = 0;
		SetMenuInfo(submenu, &_info);
		info.fMask = MIIM_ID | MIIM_TYPE | MIIM_SUBMENU;
		info.hSubMenu = submenu;
		break;
	}
	info.dwTypeData = L"";
	info.cch = 0;
	if (index < 0)
		index = GetMenuItemCount(parentItem);
	BOOL succes = InsertMenuItemW(parentItem, index, TRUE, &info);
	if (succes) {
		if (item != 0) {
			_W_WIDGETDATA(item)->clazz = _W_WIDGETDATA(parent)->clazz;
			_W_ITEM(item)->parent = _W_ITEM(parent)->parent;
			_W_ITEM(item)->index = index;
			_W_MENUITEM(item)->menu = parentItem;
		}
		return W_TRUE;
	} else {
		if (submenu != 0)
			DestroyMenu(submenu);
		return W_ERROR_ITEM_NOT_ADDED;
	}
}
wresult _w_menuitem_get_accelerator(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_get_enabled(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_get_item(w_menuitem *item, wuint index,
		w_menuitem *sub_item) {
	return W_FALSE;
}
wresult _w_menuitem_get_item_count(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_get_items(w_menuitem *item, w_iterator *items) {
	return W_FALSE;
}
wresult _w_menuitem_get_id(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_get_image(w_menuitem *item, w_image *image) {
	return W_FALSE;
}
wresult _w_menuitem_get_parent_item(w_menuitem *item, w_menuitem *parent) {
	return W_FALSE;
}
wresult _w_menuitem_get_selection(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_get_style(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_is_enabled(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_remove(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_remove_item(w_menuitem *item, wuint index) {
	return W_FALSE;
}
wresult _w_menuitem_set_accelerator(w_menuitem *item, wuint accelerator) {
	return W_FALSE;
}
wresult _w_menuitem_set_enabled(w_menuitem *item, int enabled) {
	return W_FALSE;
}
wresult _w_menuitem_set_id(w_menuitem *item, wushort id) {
	return W_FALSE;
}
wresult _w_menuitem_set_image(w_menuitem *item, w_image *image) {
	return W_FALSE;
}
wresult _w_menuitem_set_selection(w_menuitem *item, int selected) {
	return W_FALSE;
}
/*
 * menu
 */
wresult _w_menu_get_bounds(w_menu *menu, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_menu_get_root(w_menu *menu, w_menuitem *rootitem) {
	_W_WIDGETDATA(rootitem)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
	_W_ITEM(rootitem)->parent = W_WIDGET(menu);
	_W_ITEM(rootitem)->index = -1;
	_W_MENUITEM(rootitem)->menu = _W_MENU(menu)->handle;
	return W_TRUE;
}
wresult _w_menu_get_orientation(w_menu *menu) {
	return W_FALSE;
}
wresult _w_menu_get_parent(w_menu *menu, w_control **parent) {
	return W_FALSE;
}
wresult _w_menu_get_shell(w_menu *menu, w_shell **shell) {
	return W_FALSE;
}
wresult _w_menu_get_visible(w_menu *menu) {
	return W_FALSE;
}
wresult _w_menu_is_visible(w_menu *menu) {
	return W_FALSE;
}
wresult _w_menu_set_location(w_menu *menu, w_point *location) {
	return W_FALSE;
}
wresult _w_menu_set_orientation(w_menu *menu, int orientation) {
	return W_FALSE;
}
wresult _w_menu_set_visible(w_menu *menu, int visible) {
	return W_FALSE;
}
wresult _w_menu_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (w_widget_class_id(parent) < _W_CLASS_CONTROL)
		return W_ERROR_INVALID_ARGUMENT;
	if ((style & W_BAR) != 0) {
		_W_MENU(widget)->handle = CreateMenu();
	} else {
		_W_MENU(widget)->handle = CreatePopupMenu();
	}
	if (_W_MENU(widget)->handle == 0)
		return W_ERROR_NO_HANDLES;
	MENUINFO _info;
	_info.cbSize = sizeof(_info);
	_info.fMask = MIM_MENUDATA | MIM_STYLE | MIM_HELPID;
	_info.dwMenuData = (ULONG_PTR) widget;
	_info.dwStyle = MNS_NOTIFYBYPOS;
	_info.dwContextHelpID = -1;
	SetMenuInfo(_W_MENU(widget)->handle, &_info);
	_W_MENU(widget)->parent = W_CONTROL(parent);
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_W_MENU(widget)->x = -1;
	_W_MENU(widget)->y = -1;
	return W_TRUE;
}
wresult _w_menu_post_event(w_widget *widget, w_event *e) {
	return W_FALSE;
}
void _w_menu_class_init(struct _w_menu_class *clazz) {
	_w_widget_class_init(W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_MENU;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_menu_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_menu);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_menu);
	/*
	 * functions of menu
	 */
	W_WIDGET_CLASS(clazz)->create = _w_menu_create;
	W_WIDGET_CLASS(clazz)->post_event = _w_menu_post_event;
	clazz->get_bounds = _w_menu_get_bounds;
	clazz->get_root = _w_menu_get_root;
	clazz->get_orientation = _w_menu_get_orientation;
	clazz->get_parent = _w_menu_get_parent;
	clazz->get_visible = _w_menu_get_visible;
	clazz->is_visible = _w_menu_is_visible;
	clazz->set_location = _w_menu_set_location;
	clazz->set_orientation = _w_menu_set_orientation;
	clazz->set_visible = _w_menu_set_visible;
	/*
	 * function of menu item
	 */
	struct _w_menuitem_class *item = clazz->class_menuitem;
	_w_item_class_init(W_ITEM_CLASS(item));
	W_WIDGETDATA_CLASS(item)->copy = _w_menuitem_copy;
	W_ITEM_CLASS(item)->get_data = _w_menuitem_get_data;
	W_ITEM_CLASS(item)->get_text = _w_menuitem_get_text;
	W_ITEM_CLASS(item)->set_data = _w_menuitem_set_data;
	W_ITEM_CLASS(item)->set_text = _w_menuitem_set_text;
	item->insert = _w_menuitem_insert;
	item->get_accelerator = _w_menuitem_get_accelerator;
	item->get_enabled = _w_menuitem_get_enabled;
	item->get_item = _w_menuitem_get_item;
	item->get_item_count = _w_menuitem_get_item_count;
	item->get_items = _w_menuitem_get_items;
	item->get_id = _w_menuitem_get_id;
	item->get_image = _w_menuitem_get_image;
	item->get_parent_item = _w_menuitem_get_parent_item;
	item->get_selection = _w_menuitem_get_selection;
	item->get_style = _w_menuitem_get_style;
	item->is_enabled = _w_menuitem_is_enabled;
	item->remove = _w_menuitem_remove;
	item->remove_item = _w_menuitem_remove_item;
	item->set_accelerator = _w_menuitem_set_accelerator;
	item->set_enabled = _w_menuitem_set_enabled;
	item->set_id = _w_menuitem_set_id;
	item->set_image = _w_menuitem_set_image;
	item->set_selection = _w_menuitem_set_selection;
}
