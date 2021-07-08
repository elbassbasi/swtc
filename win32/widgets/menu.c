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
HMENU _w_hmenu_get_parent(HMENU menu) {
	MENUINFO info;
	info.cbSize = sizeof(info);
	info.fMask = MIM_MENUDATA | MIM_HELPID;
	if (!GetMenuInfo(menu, &info)) {
		return 0;
	}
	if ((info.dwContextHelpID & _MENU_HELPID_BIT_CHECK) == 0)
		return 0;
	if ((info.dwContextHelpID & _MENU_HELPID_MASK) == 0) {
		return 0;
	}
	return (HMENU) info.dwMenuData;
}
w_menu* _w_hmenu_get_top(HMENU menu) {
	HMENU m = menu;
	MENUINFO info;
	info.cbSize = sizeof(info);
	info.fMask = MIM_MENUDATA | MIM_HELPID;
	while (1) {
		if (!GetMenuInfo(m, &info)) {
			return 0;
		}
		if ((info.dwContextHelpID & _MENU_HELPID_BIT_CHECK) == 0)
			return 0;
		if ((info.dwContextHelpID & _MENU_HELPID_MASK) == 0) {
			return (w_menu*) info.dwMenuData;
		}
		m = (HMENU) info.dwMenuData;
	}
	return 0;
}
int _w_menu_find_menu_ids(UINT id, HMENU *hmenu, _w_menu_id **menuid,
		_w_accel_id **acc, w_menu *menu, w_control *parent) {
	if (acc != 0)
		*acc = 0;
	if (id & _MENU_ID_ID) {
		int _id = id & _MENU_ID_MASK;
		w_menu *_menu = menu;
		if (id & _MENU_ID_ACCEL) {
			_w_accel_id *_acc;
			if (menu == 0)
				menu = (w_menu*) _w_hmenu_get_top(*hmenu);
			_acc = w_array_get(_W_CONTROL(parent)->ids, _id,
					sizeof(_w_accel_id));
			if (_acc == 0)
				return W_FALSE;
			if (acc != 0)
				*acc = _acc;
			if (_acc->flags & _MENU_FLAGS_ID) {
				_id = _acc->sub_id;
				menu = (w_menu*) _w_hmenu_get_top(_acc->hMenu);
			} else
				return W_FALSE;
		}
		if (menuid != 0) {
			if (menu == 0)
				menu = (w_menu*) _w_hmenu_get_top(*hmenu);
			w_array *_ids = _W_MENU(menu)->ids;
			*menuid = w_array_get(_ids, _id, sizeof(_w_menu_id));
		}
	}
	return W_TRUE;
}
/*
 * menuitem
 */
wresult _w_menuitem_copy(w_widgetdata *from, w_widgetdata *to) {
	_w_item_copy(from, to);
	_W_MENUITEM(to)->menu = _W_MENUITEM(from)->menu;
	return W_TRUE;
}
int _w_menuitem_find_index(HMENU parent_menu, HMENU menu) {
	if (menu == 0)
		return -1;
	int count = GetMenuItemCount(parent_menu);
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_SUBMENU;
	for (int i = 0; i < count; i++) {
		info.hSubMenu = 0;
		if (!GetMenuItemInfoW(parent_menu, i, TRUE, &info)) {
			return -1;
		}
		if (info.hSubMenu == menu) {
			return i;
		}
	}
	return -1;
}
HMENU _w_menuitem_get_submenu(w_menuitem *item) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	MENUITEMINFOW info;
	if (index == -1) {
		return _W_MENUITEM(item)->menu;
	} else {
		info.cbSize = sizeof(info);
		info.fMask = MIIM_SUBMENU;
		info.hSubMenu = 0;
		if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
			return info.hSubMenu;
		} else
			return 0;
	}
}
wresult _w_menuitem_get_data(w_item *item, void **data) {
	*data = 0;
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_DATA;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		*data = (void*) info.dwItemData;
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
void _w_menuitem_get_text_0(w_menuitem *item, WCHAR **str, size_t *size,
		size_t added) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	*str = 0;
	*size = 0;
	if (index < 0) {
		return;
	}
	*str = _w_toolkit_malloc_all(size);
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STRING;
	info.dwTypeData = *str;
	info.cch = *size;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		if (((info.cch + 1 + added) * sizeof(WCHAR)) >= *size) {
			_w_toolkit_free(*str, *size);
			*size = (info.cch + 1 + added) * sizeof(WCHAR);
			*str = _w_toolkit_malloc(*size);
			if (*str != 0) {
				info.dwTypeData = *str;
				info.cch = *size;
				if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
					return;
				}
			}
		} else
			return;
	}
}
wresult _w_menuitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	WCHAR *str;
	size_t size;
	char *string;
	size_t i;
	int ret = W_FALSE;
	_w_menuitem_get_text_0(W_MENUITEM(item), &str, &size, 0);
	if (str != 0) {
		i = 0;
		while (str[i] != 0) {
			if (str[i] == '\t')
				break;
			i++;
		}
		_win_text_set(str, i, alloc, user_data, enc);
	}
	_w_toolkit_free(str, size);
	return ret;
}
wresult _w_menuitem_set_data(w_item *item, void *data) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	wresult result = W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_DATA;
	info.dwItemData = (ULONG_PTR) data;
	if (SetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		result = W_TRUE;
	}
	return result;
}
wresult _w_menuitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	WCHAR *str, acc[30];
	size_t size, i, l = 0;
	int newlength;
	wresult result = W_FALSE;
	_w_menuitem_get_text_0(W_MENUITEM(item), &str, &size, 0);
	acc[0] = 0;
	if (str != 0) {
		i = 0;
		while (str[i] != 0) {
			if (str[i] == '\t')
				break;
			i++;
		}
		if (str[i] == '\t') {
			l = lstrlenW(&str[i]);
			l = WMIN(sizeof(acc) / sizeof(acc[0]) - 1, l);
			lstrcpynW(acc, &str[i], l);
			str[l] = 0;
		}
	}
	_w_toolkit_free(str, size);
	_win_text_fix_0(text, length, enc, l, &str, &newlength);
	if (str != 0) {
		i = 0;
		while (str[i] != 0) {
			if (str[i] == '\t')
				str[i] = ' ';
			i++;
		}
		lstrcatW(str, acc);
		MENUITEMINFOW info;
		info.cbSize = sizeof(info);
		info.fMask = MIIM_STRING;
		info.dwTypeData = str;
		info.cch = newlength;
		if (SetMenuItemInfoW(hMenu, index, TRUE, &info)) {
			result = W_TRUE;
		}
	}
	_win_text_free(text, str, newlength);
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
		info.wID = _MENU_ID_STYLE_CHECK;
		break;
	case W_RADIO:
		info.fType = MFT_RADIOCHECK;
		break;
	case W_CASCADE:
		_info.cbSize = sizeof(_info);
		_info.fMask = MIM_HELPID;
		GetMenuInfo(parentItem, &_info);
		submenu = CreatePopupMenu();
		_info.fMask = MIM_MENUDATA | MIM_STYLE | MIM_HELPID;
		_info.dwMenuData = (ULONG_PTR) parentItem;
		_info.dwStyle = MNS_NOTIFYBYPOS;
		_info.dwContextHelpID += _MENU_HELPID_ADD;
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
			W_WIDGETDATA(item)->clazz = W_WIDGETDATA(parent)->clazz;
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
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	w_widget *parent = _W_ITEM(item)->parent;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	info.fState = 0;
	wresult result = 0;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		_w_accel_id *id = 0;
		_w_menu_find_menu_ids(info.wID, &hMenu, 0, &id, W_MENU(parent), 0);
		if (id != 0)
			return id->accelerator;
	}
	return result;
}
wresult _w_menuitem_get_enabled(w_menuitem *item) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_TRUE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		return (info.fState & (MFS_DISABLED | MFS_GRAYED)) == 0;
	} else
		return W_TRUE;
}
wresult _w_menuitem_get_item(w_menuitem *item, wuint index,
		w_menuitem *sub_item) {
	HMENU hMenu = _w_menuitem_get_submenu(item);
	if (hMenu == 0)
		return W_ERROR_INVALID_ARGUMENT;
	int count = GetMenuItemCount(hMenu);
	if (index < count) {
		W_WIDGETDATA(sub_item)->clazz = W_WIDGETDATA(item)->clazz;
		_W_ITEM(sub_item)->parent = _W_ITEM(item)->parent;
		_W_ITEM(sub_item)->index = index;
		_W_MENUITEM(sub_item)->menu = hMenu;
		return W_TRUE;
	}
	return W_ERROR_INVALID_RANGE;
}
wresult _w_menuitem_get_item_count(w_menuitem *item) {
	HMENU hMenu = _w_menuitem_get_submenu(item);
	if (hMenu == 0)
		return 0;
	return GetMenuItemCount(hMenu);
}
typedef struct _w_menuitem_iterator {
	struct _w_iterator_class *clazz;
	w_widget *menu;
	HMENU hMenu;
	int count;
	int i;
} _w_menuitem_iterator;
wresult _w_menuitem_iterator_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_menuitem_iterator_reset(w_iterator *it) {
	_w_menuitem_iterator *_it = (_w_menuitem_iterator*) it;
	_it->i = 0;
	return W_TRUE;
}
wresult _w_menuitem_iterator_next(w_iterator *it, void *obj) {
	_w_menuitem_iterator *_it = (_w_menuitem_iterator*) it;
	if (_it->i < _it->count) {
		w_menuitem *item = (w_menuitem*) obj;
		W_WIDGETDATA(item)->clazz = _W_MENU_GET_ITEM_CLASS(_it->menu);
		_W_ITEM(item)->parent = _it->menu;
		_W_ITEM(item)->index = _it->i;
		_W_MENUITEM(item)->menu = _it->hMenu;
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _w_menuitem_iterator_remove(w_iterator *it) {
	_w_menuitem_iterator *_it = (_w_menuitem_iterator*) it;
	return W_FALSE;
}
size_t _w_menuitem_iterator_get_count(w_iterator *it) {
	_w_menuitem_iterator *_it = (_w_menuitem_iterator*) it;
	return ((struct _w_menuitem_iterator*) it)->count;
}
_w_iterator_class _w_menuitem_iterator_class = { //
		_w_menuitem_iterator_close, //
				_w_menuitem_iterator_next, //
				_w_menuitem_iterator_reset, //
				_w_menuitem_iterator_remove, //
				_w_menuitem_iterator_get_count //
		};
wresult _w_menuitem_get_items(w_menuitem *item, w_iterator *items) {
	HMENU hMenu = _w_menuitem_get_submenu(item);
	if (hMenu != 0) {
		_w_menuitem_iterator *it = (_w_menuitem_iterator*) items;
		it->clazz = &_w_menuitem_iterator_class;
		it->menu = _W_ITEM(item)->parent;
		it->hMenu = hMenu;
		it->i = 0;
		it->count = GetMenuItemCount(hMenu);
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_menuitem_get_id(w_menuitem *item, int mask) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		if (mask == 0) {
			w_widget *menu = _W_ITEM(item)->parent;
			mask = _W_MENU(menu)->id_mask & 0xFFFF;
		}
		return (info.wID >> 16) & mask;
	} else
		return 0;
}
wresult _w_menuitem_get_image(w_menuitem *item, w_image *image) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	w_widget *parent = _W_ITEM(item)->parent;
	if (index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID | MIIM_BITMAP;
	info.fState = 0;
	wresult result = W_FALSE;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		HBITMAP hbmpItem = info.hbmpItem;
		if (info.hbmpItem == HBMMENU_CALLBACK) {
			HMENU menu = hMenu;
			int index;
			_w_menu_id *idInfo = 0;
			_w_menu_find_menu_ids(info.wID, &hMenu, &idInfo, 0, W_MENU(parent),
					0);
			if (idInfo != 0) {
				hbmpItem = idInfo->image;
			}
		}
		if (hbmpItem != 0) {
			result = W_TRUE;
			_W_IMAGE(image)->handle = CopyImage(hbmpItem, IMAGE_BITMAP, 0, 0,
			LR_DEFAULTSIZE);
			_W_IMAGE(image)->type = _IMAGE_HBITMAP;
			_W_IMAGE(image)->nodispose = 0;
		}
	}
	return result;
}
wresult _w_menuitem_get_parent_item_0(w_widget *menu, HMENU hMenu,
		w_menuitem *parent) {
	MENUINFO _info;
	int index = -1;
	_info.cbSize = sizeof(_info);
	_info.fMask = MIM_MENUDATA | MIM_HELPID;
	_info.dwMenuData = 0;
	if (GetMenuInfo(hMenu, &_info)) {
		if ((_info.dwContextHelpID & _MENU_HELPID_BIT_CHECK) != 0
				&& (_info.dwContextHelpID & _MENU_HELPID_MASK) != 0) {
			index = _w_menuitem_find_index(hMenu, (HMENU) _info.dwMenuData);
			if (index < 0)
				_info.dwMenuData = 0;
		}
		if (_info.dwMenuData != 0) {
			W_WIDGETDATA(parent)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
			_W_ITEM(parent)->parent = menu;
			_W_ITEM(parent)->index = index;
			_W_MENUITEM(parent)->menu = (HMENU) _info.dwMenuData;
		}
	}
	return W_FALSE;
}
wresult _w_menuitem_get_parent_item(w_menuitem *item, w_menuitem *parentitem) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	w_widget *parent = _W_ITEM(item)->parent;
	return _w_menuitem_get_parent_item_0(parent, hMenu, parentitem);
}
wresult _w_menuitem_get_selection(w_menuitem *item) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		return (info.fState & MFS_CHECKED) != 0;
	} else
		return W_FALSE;
}
BOOL _w_menuitem_is_style_check(HMENU hMenu, UINT id) {
	if ((id & _MENU_ID_ID) != 0) {

	} else if ((id & _MENU_ID_STYLE_CHECK) != 0)
		return TRUE;
	return FALSE;
}
wresult _w_menuitem_get_style(w_menuitem *item) {
	HMENU parentItem = _w_menuitem_get_submenu(item);
	if (parentItem != 0)
		return W_CASCADE;
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_FTYPE | MIIM_ID;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		if (info.fType & MFT_SEPARATOR)
			return W_SEPARATOR;
		if (info.fType & MFT_RADIOCHECK)
			return W_RADIO;
		if (_w_menuitem_is_style_check(hMenu, info.wID))
			return W_CHECK;
		return W_PUSH;
	}
	return 0;
}
wresult _w_menuitem_is_enabled(w_menuitem *item) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_TRUE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		return (info.fState & MFS_ENABLED) != 0;
	} else
		return W_TRUE;
}
wresult _w_menuitem_remove(w_menuitem *item) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	w_widget *menu = _W_ITEM(item)->parent;
	w_event_menu ei;
	ei.event.type = W_EVENT_ITEM_DISPOSE;
	ei.event.platform_event = 0;
	ei.event.widget = menu;
	ei.item = item;
	_w_widget_send_event(W_WIDGET(menu), (w_event*) &ei, W_EVENT_SEND);
	W_WIDGETDATA(item)->clazz = 0;
	if (DeleteMenu(hMenu, index, MF_BYPOSITION)) {
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_menuitem_remove_item(w_menuitem *item, wuint index) {
	HMENU hMenu = _w_menuitem_get_submenu(item);
	if (hMenu == 0)
		return W_FALSE;
	w_widget *menu = _W_ITEM(item)->parent;
	_w_menuitem _item;
	W_WIDGETDATA(&_item)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
	_W_ITEM(&_item)->parent = menu;
	_W_ITEM(&_item)->index = index;
	_W_MENUITEM(&_item)->menu = hMenu;
	return _w_menuitem_remove(W_MENUITEM(&_item));
}
wresult _w_menuitem_fill_accel(ACCEL *accel, struct _w_accel_id *item) {
	accel->cmd = accel->key = accel->fVirt = 0;
	//if (accelerator == 0 || !getEnabled ()) return false;
	if ((item->accelerator & W_COMMAND) != 0)
		return W_FALSE;
	int fVirt = FVIRTKEY;
	int key = item->accelerator & W_KEY_MASK;
	int vKey = _w_untranslate_key(key);
	WCHAR tp[2];
	if (vKey != 0) {
		key = vKey;
	} else {
		switch (key) {
		/*
		 * Bug in Windows.  For some reason, VkKeyScan
		 * fails to map ESC to VK_ESCAPE and DEL to
		 * VK_DELETE.  The fix is to map these keys
		 * as a special case.
		 */
		case 27:
			key = VK_ESCAPE;
			break;
		case 127:
			key = VK_DELETE;
			break;
		default: {
			if (key == 0)
				return W_FALSE;
			vKey = VkKeyScanW((short) key);
			if (vKey == -1) {
				tp[0] = key;
				tp[1] = 0;
				CharUpperW(tp);
				if (key != tp[0]) {
					fVirt = 0;
				}
			} else {
				key = vKey & 0xFF;
			}
		}
		}
	}
	accel->key = (short) key;
	accel->fVirt = fVirt;
	if ((item->accelerator & W_ALT) != 0)
		accel->fVirt |= FALT;
	if ((item->accelerator & W_SHIFT) != 0)
		accel->fVirt |= FSHIFT;
	if ((item->accelerator & W_CTRL) != 0)
		accel->fVirt |= FCONTROL;
	return W_TRUE;
}
int _w_menuitem_accelerator_get_text(WCHAR *txt, wuint accelerator) {
	int i = 0;
	wresult __ADD = W_FALSE;
	if (accelerator & W_CTRL) {
		txt[i++] = 'C';
		txt[i++] = 't';
		txt[i++] = 'r';
		txt[i++] = 'l';
		__ADD = W_TRUE;
	}
	if (accelerator & W_ALT) {
		if (__ADD)
			txt[i++] = '+';
		txt[i++] = 'A';
		txt[i++] = 'l';
		txt[i++] = 't';
		__ADD = W_TRUE;
	}
	if (accelerator & W_SHIFT) {
		if (__ADD)
			txt[i++] = '+';
		txt[i++] = 'S';
		txt[i++] = 'h';
		txt[i++] = 'i';
		txt[i++] = 'f';
		txt[i++] = 't';
		__ADD = W_TRUE;
	}
	if ((accelerator & 0xFFFF) == 0) {
		txt[i] = 0;
	} else {
		if (__ADD)
			txt[i++] = '+';
		if (accelerator & W_KEYCODE_BIT) {
			//_w_untranslate_key()

		} else {
			txt[i++] = (accelerator & 0xFFFF);
			txt[i] = 0;
		}
	}
	return i;
}
wresult _w_menuitem_set_accelerator(w_menuitem *item, wuint accelerator) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	w_widget *parent = _W_ITEM(item)->parent;
	WCHAR txt[30], *str;
	size_t size;
	wresult result = W_FALSE;
	int count = _w_menuitem_accelerator_get_text(txt, accelerator);
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	info.fState = 0;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		_w_menuitem_get_text_0(item, &str, &size, count + 1);
		if (str != 0) {
			int i = 0;
			while (1) {
				if (str[i] == '\t')
					break;
				if (str[i] == 0)
					break;
				i++;
			}
			str[i++] = '\t';
			lstrcpyW(&str[i], txt);
			info.fMask = MIIM_STRING | MIIM_ID;
			info.dwTypeData = str;
			info.cch = i + count;
			_w_accel_id *acc = 0;
			_w_menu_find_menu_ids(info.wID, &hMenu, 0, &acc, W_MENU(parent), 0);
			if (acc == 0) {
				int _id = _w_control_new_id(_W_MENU(parent)->parent, &acc);
				if (acc != 0) {
					acc->flags = 0;
					if (info.wID & _MENU_ID_ID) {
						int last_id = info.wID & _MENU_ID_MASK;
						info.wID = (info.wID & 0xFFFF0000) | _MENU_ID_ID
								| _MENU_ID_ACCEL | (_id & _MENU_ID_MASK);
						acc->flags |= _MENU_FLAGS_ID;
						acc->sub_id = last_id;
					} else {
						if (info.wID & _MENU_ID_STYLE_CHECK) {
							acc->flags |= _MENU_FLAGS_CHECK;
						}
						info.wID = (info.wID & 0xFFFF0000) | _MENU_ID_ID
								| _MENU_ID_ACCEL | (_id & _MENU_ID_MASK);
						acc->sub_id = index;
					}
					acc->hMenu = hMenu;
				}
			}
			if (acc != 0) {
				acc->accelerator = accelerator;
				if (SetMenuItemInfoW(hMenu, index, TRUE, &info)) {
					result = W_TRUE;
				}
			}
		}
	}
	_w_toolkit_free(str, size);
	return result;
}
wresult _w_menuitem_set_enabled(w_menuitem *item, int enabled) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	info.fState = 0;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		int bits = MFS_DISABLED | MFS_GRAYED;
		if (enabled) {
			if ((info.fState & bits) == 0)
				return W_TRUE;
			info.fState &= ~bits;
		} else {
			if ((info.fState & bits) == bits)
				return W_TRUE;
			info.fState |= bits;
		}
		if (SetMenuItemInfoW(hMenu, index, TRUE, &info)) {
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_menuitem_set_id(w_menuitem *item, int mask, wushort id) {
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	HMENU hMenu = _W_MENUITEM(item)->menu;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	info.wID = 0;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		if (mask == 0) {
			w_widget *menu = _W_ITEM(item)->parent;
			mask = _W_MENU(menu)->id_mask & 0xFFFF;
		}
		int newid = ((info.wID >> 16) & ~mask) | (id & mask);
		info.wID = (info.wID & 0xFFFF) | (newid & 0xFFFF) << 16;
		if (SetMenuItemInfoW(hMenu, index, TRUE, &info)) {
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_menuitem_set_image(w_menuitem *item, w_image *image) {
	if (WIN32_VERSION < VERSION(4, 10))
		return W_FALSE;
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	wresult result = W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID | MIIM_BITMAP;
	HBITMAP hbmpItemLast = 0, hbmpItem = 0;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		hbmpItemLast = info.hbmpItem;
		if (WIN32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
			if (image != 0) {
				hbmpItem = _w_image_create_32bit_dib(image);
			}
			info.hbmpItem = hbmpItem;
		} else {
			info.hbmpItem = image != 0 ? HBMMENU_CALLBACK : 0;
		}
		if (info.hbmpItem == HBMMENU_CALLBACK) {
			/*w_shell *shell;
			 _w_menu_get_shell((w_menu*) _W_ITEM(item)->parent, &shell);
			 _w_accel_id *id = 0;
			 int _id = _w_shell_registre_menuitem_id(shell, item, &id);
			 info.wID = (info.wID & 0xFFFF8000) | ((_id + 1) & 0x7FFF);
			 id->image = hbmpItem;*/
		} else {
			info.fMask = MIIM_BITMAP;
		}
		if (SetMenuItemInfoW(hMenu, index, TRUE, &info)) {
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
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE | MIIM_ID;
	info.fState = 0;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		if (_w_menuitem_is_style_check(hMenu, info.wID)
				|| (info.fState & MFT_RADIOCHECK)) {
			info.fMask = MIIM_STATE;
			info.fState &= ~MFS_CHECKED;
			if (selected)
				info.fState |= MFS_CHECKED;
			if (SetMenuItemInfoW(hMenu, index, TRUE, &info)) {
				return W_TRUE;
			}
		}
	}
	return W_FALSE;
}
/*
 * menu
 */
wresult _w_menu_get_bounds(w_menu *menu, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_menu_get_root(w_menu *menu, w_menuitem *rootitem) {
	W_WIDGETDATA(rootitem)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
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
wresult _w_menu_get_shell(w_widget *menu, w_shell **shell) {
	w_control *parent = _W_MENU(menu)->parent;
	return w_widget_get_shell(W_WIDGET(parent), shell);
}
wresult _w_menu_get_visible(w_menu *menu) {
	return W_FALSE;
}
wresult _w_menu_is_visible(w_menu *menu) {
	return W_FALSE;
}
wresult _w_menu_set_location(w_menu *menu, w_point *location) {
	if ((_W_WIDGET(menu)->style & (W_BAR | W_DROP_DOWN)) != 0)
		return W_FALSE;
	_W_MENU(menu)->x = location->x;
	_W_MENU(menu)->y = location->y;
	_W_WIDGET(menu)->state |= STATE_MENU_HAS_LOCATION;
	return W_TRUE;
}
wresult _w_menu_set_orientation(w_menu *menu, int orientation) {
	return W_FALSE;
}
wresult _w_menu_set_visible(w_menu *menu, int visible) {
	wuint64 style = _W_WIDGET(menu)->style;
	if ((style & (W_BAR | W_DROP_DOWN)) != 0)
		return W_FALSE;
	w_control *parent = _W_MENU(menu)->parent;
	HWND hwndParent = _W_WIDGET(parent)->handle;
	if (visible) {
		int flags = TPM_LEFTBUTTON;
		if (GetKeyState(VK_LBUTTON) >= 0)
			flags |= TPM_RIGHTBUTTON;
		if ((style & W_RIGHT_TO_LEFT) != 0)
			flags |= TPM_RIGHTALIGN;
		if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
			flags &= ~TPM_RIGHTALIGN;
			if ((style & W_LEFT_TO_RIGHT) != 0)
				flags |= TPM_RIGHTALIGN;
		}
		int nX = _W_MENU(menu)->x, nY = _W_MENU(menu)->y;
		if ((_W_WIDGET(menu)->state & STATE_MENU_HAS_LOCATION) == 0) {
			int pos = GetMessagePos();
			nX = GET_X_LPARAM(pos);
			nY = GET_Y_LPARAM(pos);
		}
		_W_WIDGET(menu)->state &= ~STATE_MENU_HAS_LOCATION;
		/*
		 * Feature in Windows.  It is legal use TrackPopupMenu()
		 * to display an empty menu as long as menu items are added
		 * inside of WM_INITPOPUPMENU.  If no items are added, then
		 * TrackPopupMenu() fails and does not send an indication
		 * that the menu has been closed.  This is not strictly a
		 * bug but leads to unwanted behavior when application code
		 * assumes that every WM_INITPOPUPMENU will eventually result
		 * in a WM_MENUSELECT, wParam=MAKEWPARAM (0, 0xFFFF), lParam=0 to
		 * indicate that the menu has been closed.  The fix is to detect
		 * the case when TrackPopupMenu() fails and the number of items in
		 * the menu is zero and issue a fake WM_MENUSELECT.
		 */
		HMENU handle = _W_MENU(menu)->handle;
		BOOL success = TrackPopupMenu(handle, flags, nX, nY, 0, hwndParent,
		NULL);
		if (!success && GetMenuItemCount(handle) == 0) {
			SendMessageW(hwndParent, WM_MENUSELECT, MAKEWPARAM(0, 0xFFFF), 0);
		}
	} else {
		SendMessageW(hwndParent, WM_CANCELMODE, 0, 0);
	}
	/*
	 * Bug in Windows.  After closing a popup menu, the accessibility focus
	 * is not returned to the focus control.  This causes confusion for AT users.
	 * The fix is to explicitly set the accessibility focus back to the focus control.
	 */
	HWND hFocus = GetFocus();
	if (hFocus != 0) {
		NotifyWinEvent(EVENT_OBJECT_FOCUS, hFocus, OBJID_CLIENT, 0);
	}
	return W_TRUE;
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
	_info.dwContextHelpID = _MENU_HELPID_BIT_CHECK;
	SetMenuInfo(_W_MENU(widget)->handle, &_info);
	_W_MENU(widget)->parent = W_CONTROL(parent);
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_W_MENU(widget)->x = -1;
	_W_MENU(widget)->y = -1;
	_W_MENU(widget)->id_mask = 0xFFFF;
	return W_TRUE;
}
wresult _w_menu_post_event(w_widget *widget, w_event *e,int flags) {
	if (widget->post_event != 0) {
		widget->post_event(widget, e);
	}
	return W_FALSE;
}
void _w_menu_radio_select(HMENU hMenu, int i, int pas, MENUITEMINFOW *info) {
	BOOL stop = FALSE;
	while (TRUE) {
		info->fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID;
		if (GetMenuItemInfoW(hMenu, i, TRUE, info)) {
			if ((info->fType & MFT_RADIOCHECK) != 0) {
				info->fMask = MIIM_STATE;
				info->fState &= ~MFS_CHECKED;
				SetMenuItemInfoW(hMenu, i, TRUE, info);
			} else
				break;
		} else
			break;
		i += pas;
	}
}
void _w_menu_update(HMENU menu) {

}
wresult _MENU_WM_MENUCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HMENU hMenu = (HMENU) e->lparam;
	int index = e->wparam;
	w_menu *menu = _w_hmenu_get_top(hMenu);
	if (menu == 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID | MIIM_CHECKMARKS;
	if (!GetMenuItemInfoW(hMenu, index, TRUE, &info))
		return W_TRUE;
	if ((info.fType & MFT_RADIOCHECK) != 0) {
		UINT last_state = info.fState;
		_w_menu_radio_select(hMenu, index - 1, -1, &info);
		_w_menu_radio_select(hMenu, index + 1, 1, &info);
		info.fMask = MIIM_STATE;
		info.fState = last_state | MFS_CHECKED;
		SetMenuItemInfoW(hMenu, index, TRUE, &info);
	} else {
		if (_w_menuitem_is_style_check(hMenu, info.wID)) {
			if ((info.fState & MFS_CHECKED) != 0) {
				info.fState &= ~MFS_CHECKED;
			} else {
				info.fState |= MFS_CHECKED;
			}
			info.fMask = MIIM_STATE;
			SetMenuItemInfoW(hMenu, index, TRUE, &info);
		}
	}
	_w_menuitem item;
	w_event_menu ei;
	ei.event.type = W_EVENT_ITEM_SELECTION;
	ei.event.platform_event = _EVENT_PLATFORM(e);
	ei.event.widget = W_WIDGET(menu);
	ei.item = (w_menuitem*) &item;
	W_WIDGETDATA(&item)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
	_W_ITEM(&item)->parent = W_WIDGET(menu);
	_W_ITEM(&item)->index = index;
	_W_MENUITEM(&item)->menu = hMenu;
	_w_widget_send_event(W_WIDGET(menu), (w_event*) &ei, W_EVENT_SEND);
	e->result = FALSE;
	return W_TRUE;
}
wresult _MENU_WM_COMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = W_FALSE;
	/*
	 * When the WM_COMMAND message is sent from a
	 * menu, the HWND parameter in LPARAM is zero.
	 */
	WPARAM lastWparam = e->wparam;
	_w_accel_id *_acc = 0;
	HMENU hmenu = 0;
	_w_menu_find_menu_ids(LOWORD(e->wparam), &hmenu, 0, &_acc, 0,
			W_CONTROL(widget));
	if (_acc != 0) {
		e->lparam = (LPARAM) _acc->hMenu;
		e->wparam = _acc->sub_id;
		result = _MENU_WM_MENUCOMMAND(widget, e, priv);
	}
	e->wparam = lastWparam;
	e->lparam = 0;
	return result;
}
wresult _MENU_WM_INITMENUPOPUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {

	/* Ignore WM_INITMENUPOPUP for an accelerator */
	if (win_toolkit->accelKeyHit)
		return W_FALSE;

	_w_menuitem item;
	w_event_menu ei;

	/*
	 * If the high order word of LPARAM is non-zero,
	 * the menu is the system menu and we can ignore
	 * WPARAM.  Otherwise, use WPARAM to find the menu.
	 */
	w_shell *shell;
	w_widget_get_shell(widget, &shell);
	HMENU oldMenu = _W_CONTROL(shell)->activeMenu, newMenu = 0;
	if (HIWORD (e->lparam) == 0) {
		newMenu = (HMENU) e->wparam;
		if (newMenu != 0)
			_w_menu_update(newMenu);
	}
	HMENU menu = newMenu;
	while (menu != 0 && menu != oldMenu) {
		menu = _w_hmenu_get_parent(menu);
	}
	if (menu == 0) {
		menu = _W_CONTROL(shell)->activeMenu;
		while (menu != 0) {
			/*
			 * It is possible (but unlikely), that application
			 * code could have disposed the widget in the hide
			 * event.  If this happens, stop searching up the
			 * ancestor list because there is no longer a link
			 * to follow.
			 */
			w_menu *_menu = _w_hmenu_get_top(menu);
			if (_menu != 0) {
				if (_w_menuitem_get_parent_item_0(W_WIDGET(_menu), menu,
						W_MENUITEM(&item))) {
					ei.event.type = W_EVENT_ITEM_HIDE;
					ei.event.platform_event = _EVENT_PLATFORM(e);
					ei.event.widget = W_WIDGET(menu);
					ei.item = (w_menuitem*) &item;
					_w_widget_send_event(W_WIDGET(_menu), W_EVENT(&ei),
							W_EVENT_SEND);
				}
			}
			menu = _w_hmenu_get_parent(menu);
			HMENU ancestor = newMenu;
			while (ancestor != 0 && ancestor != menu) {
				ancestor = _w_hmenu_get_parent(ancestor);
			}
			if (ancestor != 0)
				break;
		}
	}

	/*
	 * The shell and the new menu may be disposed because of
	 * sending the hide event to the ancestor menus but setting
	 * a field to null in a disposed shell is not harmful.
	 */
	if (newMenu != 0)
		newMenu = 0;
	_W_CONTROL(shell)->activeMenu = newMenu;

	/* Send the show event */
	if (newMenu != 0 && newMenu != oldMenu) {
		w_menu *_menu = _w_hmenu_get_top(newMenu);
		if (_menu != 0) {
			if (_w_menuitem_get_parent_item_0(W_WIDGET(_menu), newMenu,
					W_MENUITEM(&item))) {
				ei.event.type = W_EVENT_ITEM_SHOW;
				ei.event.platform_event = _EVENT_PLATFORM(e);
				ei.event.widget = W_WIDGET(_menu);
				ei.item = (w_menuitem*) &item;
				_w_widget_send_event(W_WIDGET(_menu), W_EVENT(&ei),
						W_EVENT_SEND);
			}
		}
		// widget could be disposed at this point
	}
	return W_FALSE;
}
wresult _MENU_WM_UNINITMENUPOPUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HMENU hiddenMenu = (HMENU) e->wparam;
	if (hiddenMenu != 0) {
		_w_menuitem item;
		w_event_menu ei;
		w_shell *shell;
		w_widget_get_shell(widget, &shell);
		w_menu *_menu = _w_hmenu_get_top(hiddenMenu);
		if (_menu != 0) {
			if (_w_menuitem_get_parent_item_0(W_WIDGET(_menu), hiddenMenu,
					W_MENUITEM(&item))) {
				ei.event.type = W_EVENT_ITEM_HIDE;
				ei.event.platform_event = _EVENT_PLATFORM(e);
				ei.event.widget = W_WIDGET(_menu);
				ei.item = (w_menuitem*) &item;
				_w_widget_send_event(W_WIDGET(_menu), W_EVENT(&ei),
						W_EVENT_SEND);
			}
		}
		if (hiddenMenu == _W_CONTROL(shell)->activeMenu)
			_W_CONTROL(shell)->activeMenu = 0;
	}
	return W_FALSE;
}
wresult _MENU_WM_MENUCHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Feature in Windows.  When the user types Alt+<key>
	 * and <key> does not match a mnemonic in the System
	 * menu or the menu bar, Windows beeps.  This beep is
	 * unexpected and unwanted by applications that look
	 * for Alt+<key>.  The fix is to detect the case and
	 * stop Windows from beeping by closing the menu.
	 */
	int type = HIWORD(e->wparam);
	if (type == 0 || type == MF_SYSMENU) {
		win_toolkit->mnemonicKeyHit = FALSE;
		e->result = MAKELRESULT(0, MNC_CLOSE);
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _MENU_WM_MENUSELECT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int code = HIWORD(e->wparam);
	w_shell *shell;
	w_widget_get_shell(widget, &shell);
	_w_menuitem item;
	w_event_menu ei;
	//KillTimer (this.handle, Menu.ID_TOOLTIP_TIMER);
	/*if (_W_SHELL(shell)->activeMenu != 0)
	 activeMenu.hideCurrentToolTip ();*/
	if (code == 0xFFFF && e->lparam == 0) {
		HMENU menu = _W_CONTROL(shell)->activeMenu;
		while (menu != 0) {
			/*
			 * When the user cancels any menu that is not the
			 * menu bar, assume a mnemonic key was pressed to open
			 * the menu from WM_SYSCHAR.  When the menu was invoked
			 * using the mouse, this assumption is wrong but not
			 * harmful.  This variable is only used in WM_SYSCHAR
			 * and WM_SYSCHAR is only sent after the user has pressed
			 * a mnemonic.
			 */
			win_toolkit->mnemonicKeyHit = TRUE;
			/*
			 * It is possible (but unlikely), that application
			 * code could have disposed the widget in the hide
			 * event.  If this happens, stop searching up the
			 * parent list because there is no longer a link
			 * to follow.
			 */
			w_menu *_menu = _w_hmenu_get_top(menu);
			if (_menu != 0) {
				if (_w_menuitem_get_parent_item_0(W_WIDGET(_menu), menu,
						W_MENUITEM(&item))) {
					ei.event.type = W_EVENT_ITEM_HIDE;
					ei.event.platform_event = _EVENT_PLATFORM(e);
					ei.event.widget = W_WIDGET(_menu);
					ei.item = (w_menuitem*) &item;
					_w_widget_send_event(W_WIDGET(_menu), W_EVENT(&ei),
							W_EVENT_SEND);
				}
				if (w_widget_is_ok(W_WIDGET(_menu)) <= 0)
					break;
			}
			menu = _w_hmenu_get_parent(menu);
		}
		/*
		 * The shell may be disposed because of sending the hide
		 * event to the last active menu menu but setting a field
		 * to null in a destroyed widget is not harmful.
		 */
		_W_CONTROL(shell)->activeMenu = 0;
		return W_FALSE;
	}
	if ((code & MF_SYSMENU) != 0)
		return W_FALSE;
	if ((code & MF_HILITE) != 0) {
		if ((code & MF_POPUP) != 0) {
			int index = LOWORD(e->wparam);
			MENUITEMINFOW info;
			info.cbSize = sizeof(info);
			info.fMask = MIIM_SUBMENU;
			if (GetMenuItemInfoW((HMENU) e->lparam, index, TRUE, &info)) {
				HMENU newMenu = info.hSubMenu;
				if (newMenu != 0) {
					//item = newMenu.cascade;
					_W_CONTROL(widget)->activeMenu = newMenu;
					//_W_CONTROL(widget)->activeMenu.selectedMenuItem = newMenu.cascade;
					SetTimer(_W_WIDGET(widget)->handle, ID_TOOLTIP_TIMER,
					TTM_GETDELAYTIME, 0);
				}
			}
		} else {
			HMENU newMenu = (HMENU) e->lparam;
			/*if (newMenu != 0) {
			 int id = LOWORD (e->wparam);
			 item = display.getMenuItem (id);
			 }
			 _W_CONTROL(widget)->activeMenu = (newMenu == 0) ? menu : newMenu;
			 if (item != null && _W_CONTROL(widget)->activeMenu != 0) {
			 activeMenu.selectedMenuItem = item;
			 SetTimer (_W_WIDGET(widget)->handle, ID_TOOLTIP_TIMER, TTM_GETDELAYTIME, 0);
			 }*/
		}
		//if (item != null) item.sendEvent (SWT.Arm);
	}
	return W_FALSE;
}
wresult _MENU_WM_DRAWITEM(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	DRAWITEMSTRUCT *st = (DRAWITEMSTRUCT*) e->lparam;
	wresult result;
	if (st->CtlType == ODT_MENU) {
		HMENU hmenu = (HMENU) st->hwndItem;
		_w_accel_id *acc = 0;
		_w_menu_id *idInfo = 0;
		_w_menu_find_menu_ids(st->itemID, &hmenu, &idInfo, &acc, 0,
				W_CONTROL(widget));
		if (idInfo != 0 && idInfo->image != 0) {
			BITMAP bm;
			GetObjectW(idInfo->image, sizeof(bm), &bm);
			HDC srcHdc = CreateCompatibleDC(NULL);
			HBITMAP oldHBitmap = SelectObject(srcHdc, idInfo->image);
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
	wresult result;
	if (st->CtlType == ODT_MENU) {
		_w_accel_id *acc = 0;
		_w_menu_id *idInfo = 0;
		HMENU hmenu = 0;
		_w_menu_find_menu_ids(st->itemID, &hmenu, &idInfo, &acc, 0,
				W_CONTROL(widget));
		if (result <= 0)
			return W_FALSE;
		if (idInfo->image != 0) {
			BITMAP bm;
			GetObjectW(idInfo->image, sizeof(bm), &bm);
			st->itemWidth = bm.bmWidth;
			st->itemHeight = bm.bmHeight;
		}
	}
	return W_FALSE;
}
void _w_menu_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_menu_class *clazz) {
	if (classId == _W_CLASS_MENU) {
		W_WIDGET_CLASS(clazz)->platformPrivate = &win_toolkit->class_menu_priv;
	}
	_w_widget_class_init(toolkit, classId, W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_MENU;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_menu_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_menu);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_menu);
	/*
	 * functions of menu
	 */
	W_WIDGET_CLASS(clazz)->create = _w_menu_create;
	W_WIDGET_CLASS(clazz)->post_event = _w_menu_post_event;
	W_WIDGET_CLASS(clazz)->get_shell = _w_menu_get_shell;
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
	W_WIDGETDATA_CLASS(item)->toolkit = W_WIDGET_CLASS(clazz)->toolkit;
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
