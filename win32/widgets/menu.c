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
#define MENU_CHECK  (1 << 31)
#define MENU_HAS_ACCELERATOR  (1 << 30)
#define MENU_CUSTOM_DATA  (1 << 29)
w_menu* _w_menu_get_top(HMENU menu) {
	HMENU m = menu;
	MENUINFO info;
	info.cbSize = sizeof(info);
	info.fMask = MIM_MENUDATA | MIM_HELPID;
	while (1) {
		if (!GetMenuInfo(m, &info)) {
			return 0;
		}
		if (info.dwContextHelpID == (DWORD) -1) {
			return (w_menu*) info.dwMenuData;
		}
		m = (HMENU) info.dwMenuData;
	}
	return 0;
}
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
	*data = 0;
	if (_W_ITEM(item)->index < 0)
		return 0;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_DATA | MIIM_ID;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->menu,
	_W_ITEM(item)->index, TRUE, &info)) {
		if (info.wID & MENU_CUSTOM_DATA) {
			if (info.dwItemData != 0) {
				*data = ((_w_menuitem_data*) info.dwItemData)->userdata;
			}
		} else {
			*data = (void*) info.dwItemData;
		}
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_menuitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_menuitem_set_data(w_item *item, void *data) {
	if (_W_ITEM(item)->index < 0)
		return W_FALSE;
	wresult result = W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_DATA | MIIM_ID;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->menu,
	_W_ITEM(item)->index, TRUE, &info)) {
		if (info.wID & MENU_CUSTOM_DATA) {
			if (info.dwItemData != 0) {
				((_w_menuitem_data*) info.dwItemData)->userdata = data;
				result = W_TRUE;
			}
		} else {
			info.fMask = MIIM_DATA;
			info.dwItemData = (ULONG_PTR) data;
			if (SetMenuItemInfoW(_W_MENUITEM(item)->menu,
			_W_ITEM(item)->index, TRUE, &info)) {
				result = W_TRUE;
			}
		}
	}
	return result;
}
wresult _w_menuitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	if (_W_ITEM(item)->index == -1)
		return W_FALSE;
	int newlength;
	wresult result = W_FALSE;
	WCHAR *s;
	_win_text_fix(text, length, enc, &s, &newlength);
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
	if (_W_ITEM(item)->index < 0)
		return 0;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->menu,
	_W_ITEM(item)->index, TRUE, &info)) {
		int id = info.wID & 0x1FFFF;
		if (id < 0x100)
			return 0;
		else
			return id - 0x100;
	} else
		return 0;
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
	if (_W_ITEM(item)->index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	info.wID = 0;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->menu,
	_W_ITEM(item)->index, TRUE, &info)) {
		int _id = (id & 0xFFFF) + 0x100;
		info.wID = (info.wID & 0xFFFE0000) | _id;
		if (SetMenuItemInfoW(_W_MENUITEM(item)->menu,
		_W_ITEM(item)->index, TRUE, &info)) {
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_menuitem_set_image(w_menuitem *item, w_image *image) {
	if (WIN32_VERSION < VERSION(4, 10))
		return W_FALSE;
	if (_W_ITEM(item)->index < 0)
		return W_FALSE;
	wresult result = W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_DATA | MIIM_ID | MIIM_BITMAP;
	HBITMAP hbmpItemLast = 0, hbmpItem = 0;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->menu,
	_W_ITEM(item)->index, TRUE, &info)) {
		hbmpItemLast = info.hbmpItem;
		if (WIN32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
			if (image != 0) {
				hbmpItem = _w_image_create_32bit_dib(image);
			}
			info.hbmpItem = hbmpItem;
		} else {
			info.hbmpItem = image != 0 ? HBMMENU_CALLBACK : 0;
		}
		_w_menuitem_data *d = 0;
		if (info.wID & MENU_CUSTOM_DATA) {
			if (info.dwItemData != 0) {
				info.fMask = MIIM_BITMAP;
				info.hbmpItem = HBMMENU_CALLBACK;
				d = (_w_menuitem_data*) info.dwItemData;
				d->image = hbmpItem;
				result = W_TRUE;
			}
		} else {
			if (info.hbmpItem == HBMMENU_CALLBACK) {
				d = malloc(sizeof(_w_menuitem_data));
				if (d == 0)
					return W_ERROR_NO_MEMORY;
				d->userdata = (void*) info.dwItemData;
				d->image = hbmpItem;
				info.fMask = MIIM_DATA | MIIM_ID | MIIM_BITMAP;
				info.dwItemData = (ULONG_PTR) d;
				info.wID |= MENU_CUSTOM_DATA;
				result = W_TRUE;
			} else {
				info.fMask = MIIM_BITMAP;
				result = W_TRUE;
			}
		}
		if (SetMenuItemInfoW(_W_MENUITEM(item)->menu,
		_W_ITEM(item)->index, TRUE, &info)) {
			result = W_TRUE;
		}
	}
	if (hbmpItemLast != 0) {
		DeleteObject(hbmpItemLast);
	}
	return result;
}
wresult _w_menuitem_set_image_index(w_menuitem *item, w_imagelist *imagelist,
		int index) {
	wresult result = W_FALSE;
	w_widget *menu = _W_ITEM(item)->parent;
	if (w_imagelist_is_ok(imagelist) > 0) {
		w_image img;
		w_image_init(&img);
		w_imagelist_get_image(imagelist, index, TRUE, &img);
		result = _w_menuitem_set_image(item, &img);
		w_image_dispose(&img);
	}
	return result;
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
	*parent = _W_MENU(menu)->parent;
	return W_TRUE;
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
wresult _MENU_WM_MENUCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_menu *menu = _w_menu_get_top((HMENU) e->lparam);
	if (menu == 0)
		return W_FALSE;
	_w_event_platform *ee = (_w_event_platform*) e;
	_w_menuitem item;
	w_event_menu ei;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID | MIIM_CHECKMARKS;
	WINBOOL success = GetMenuItemInfoW((HMENU) ee->lparam, ee->wparam,
	TRUE, &info);
	if (!success)
		return W_TRUE;
	if ((info.fType & MFT_RADIOCHECK) != 0) {
		UINT last_state = info.fState;
		int i = ee->wparam - 1;
		WINBOOL stop = FALSE;
		while (i >= 0 && !stop) {
			info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID;
			success = GetMenuItemInfoW((HMENU) ee->lparam, i,
			TRUE, &info);
			if (success) {
				if ((info.fType & MFT_RADIOCHECK) != 0) {
					info.fMask = MIIM_STATE;
					info.fState &= ~MFS_CHECKED;
					success = SetMenuItemInfoW((HMENU) ee->lparam, i,
					TRUE, &info);
				} else
					stop = TRUE;
			} else
				stop = TRUE;
			i--;
		}
		i = ee->wparam + 1;
		stop = FALSE;
		while (i >= 0 && !stop) {
			info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID;
			success = GetMenuItemInfoW((HMENU) ee->lparam, i,
			TRUE, &info);
			if (success) {
				if ((info.fType & MFT_RADIOCHECK) != 0) {
					info.fMask = MIIM_STATE;
					info.fState &= ~MFS_CHECKED;
					success = SetMenuItemInfoW((HMENU) ee->lparam, i,
					TRUE, &info);
				} else
					stop = TRUE;
			} else
				stop = TRUE;
			i++;
		}
		info.fMask = MIIM_STATE;
		info.fState = last_state | MFS_CHECKED;
		SetMenuItemInfoW((HMENU) ee->lparam, ee->wparam,
		TRUE, &info);
	} else {
		if (info.wID & MENU_CHECK) {
			if ((info.fState & MFS_CHECKED) != 0) {
				info.fState &= ~MFS_CHECKED;
			} else {
				info.fState |= MFS_CHECKED;
			}
			info.fMask = MIIM_STATE;
			success = SetMenuItemInfoW((HMENU) ee->lparam, ee->wparam,
			TRUE, &info);
		}
	}
	ei.event.type = W_EVENT_ITEM_SELECTION;
	ei.event.platform_event = _EVENT_PLATFORM(ee);
	ei.event.widget = W_WIDGET(menu);
	ei.item = (w_menuitem*) &item;
	_W_WIDGETDATA(&item)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
	_W_ITEM(&item)->parent = W_WIDGET(menu);
	_W_ITEM(&item)->index = ee->wparam;
	_W_MENUITEM(&item)->menu = (HMENU) ee->lparam;
	_w_widget_send_event(W_WIDGET(menu), (w_event*) &ei);
	ee->result = FALSE;
	return W_TRUE;
}
wresult _MENU_WM_INITMENUPOPUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _MENU_WM_UNINITMENUPOPUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _MENU_WM_DRAWITEM(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	DRAWITEMSTRUCT *st = (DRAWITEMSTRUCT*) e->lparam;
	if (st->CtlType == ODT_MENU) {
		_w_menuitem_data *d = (_w_menuitem_data*) st->itemData;
		if (d->image != 0) {
			BITMAP bm;
			GetObjectW(d->image, sizeof(bm), &bm);
			HDC srcHdc = CreateCompatibleDC(NULL);
			HBITMAP oldHBitmap = SelectObject(srcHdc, d->image);
			if (bm.bmBitsPixel == 32) {
				BLENDFUNCTION blend;
				blend.BlendOp = AC_SRC_OVER;
				blend.BlendFlags = 0;
				blend.SourceConstantAlpha = 0xFF;
				blend.AlphaFormat = AC_SRC_ALPHA;
				AlphaBlend(st->hDC, st->rcItem.left, st->rcItem.top,
						st->rcItem.right - st->rcItem.left,
						st->rcItem.bottom - st->rcItem.top, srcHdc, 0, 0,
						bm.bmWidth, bm.bmHeight, blend);
			} else {
				BitBlt(st->hDC, st->rcItem.left, st->rcItem.top,
						st->rcItem.bottom - st->rcItem.left,
						st->rcItem.right - st->rcItem.top, srcHdc, 0, 0,
						SRCCOPY);
			}
		}
	}
	return W_FALSE;
}
wresult _MENU_WM_MEASUREITEM(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	MEASUREITEMSTRUCT *st = (MEASUREITEMSTRUCT*) e->lparam;
	if (st->CtlType == ODT_MENU) {
		_w_menuitem_data *d = (_w_menuitem_data*) st->itemData;
		if (d->image != 0) {
			BITMAP bm;
			GetObjectW(d->image, sizeof(bm), &bm);
			st->itemWidth = bm.bmWidth;
			st->itemHeight = bm.bmHeight;
		}
	}
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
	item->set_image_index = _w_menuitem_set_image_index;
	item->set_selection = _w_menuitem_set_selection;
}
