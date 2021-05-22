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
#define MENU_CHECK  (1 << 15)
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
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	info.fState = 0;
	wresult result = 0;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		int id = (info.wID & 0x7FFF) - 1;
		if (id >= 0) {
			w_shell *shell;
			_w_menu_get_shell((w_menu*) _W_ITEM(item)->parent, &shell);
			_w_menu_ids *acc = _W_SHELL(shell)->ids;
			if (acc != 0) {
				_w_menu_id *items = acc->id;
				if (id < acc->count) {
					result = items[id].accelerator;
				}
			}
		}
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
		_W_WIDGETDATA(sub_item)->clazz = _W_WIDGETDATA(item)->clazz;
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
		_W_WIDGETDATA(item)->clazz = _W_MENU_GET_ITEM_CLASS(_it->menu);
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
wresult _w_menuitem_get_id(w_menuitem *item) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		return (info.wID >> 16) & 0xFFFF;
	} else
		return 0;
}
wresult _w_menuitem_get_image(w_menuitem *item, w_image *image) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	int index = _W_ITEM(item)->index;
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
			int id = (info.wID & 0x7FFF) - 1;
			if (id >= 0) {
				w_shell *shell;
				_w_menu_get_shell((w_menu*) _W_ITEM(item)->parent, &shell);
				_w_menu_ids *acc = _W_SHELL(shell)->ids;
				if (acc != 0) {
					_w_menu_id *items = acc->id;
					if (id < acc->count) {
						hbmpItem = items[id].image;
					}
				}
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
wresult _w_menuitem_get_parent_item(w_menuitem *item, w_menuitem *parent) {
	HMENU hMenu = _W_MENUITEM(item)->menu;
	MENUINFO _info;
	int index = -1;
	_info.cbSize = sizeof(_info);
	_info.fMask = MIM_MENUDATA | MIM_HELPID;
	_info.dwMenuData = 0;
	if (GetMenuInfo(hMenu, &_info)) {
		if (_info.dwContextHelpID != -1) {
			index = _w_menuitem_find_index(hMenu, (HMENU) _info.dwMenuData);
			if (index < 0)
				_info.dwMenuData = 0;
		}
		if (_info.dwMenuData != 0) {
			_W_WIDGETDATA(parent)->clazz = _W_WIDGETDATA(item)->clazz;
			_W_ITEM(parent)->parent = _W_ITEM(item)->parent;
			_W_ITEM(parent)->index = index;
			_W_MENUITEM(parent)->menu = (HMENU) _info.dwMenuData;
		}
	}
	return W_FALSE;
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
		if ((info.wID & MENU_CHECK) != 0)
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
	_w_widget_send_event(W_WIDGET(menu), (w_event*) &ei);
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
	w_event_menu ei;
	ei.event.type = W_EVENT_ITEM_DISPOSE;
	ei.event.platform_event = 0;
	ei.event.widget = menu;
	ei.item = (w_menuitem*) &item;
	_W_WIDGETDATA(&_item)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
	_W_ITEM(&_item)->parent = menu;
	_W_ITEM(&_item)->index = index;
	_W_MENUITEM(&_item)->menu = hMenu;
	_w_widget_send_event(W_WIDGET(menu), (w_event*) &ei);
	if (DeleteMenu(hMenu, index, MF_BYPOSITION)) {
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_menuitem_fill_accel(ACCEL *accel, struct _w_menu_id *item) {
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
			w_shell *shell;
			_w_menu_get_shell((w_menu*) _W_ITEM(item)->parent, &shell);
			_w_menu_id *id = 0;
			int _id = _w_shell_registre_menuitem_id(shell, item, &id);
			info.wID = (info.wID & 0xFFFF8000) | ((_id + 1) & 0x7FFF);
			if (SetMenuItemInfoW(hMenu, index, TRUE, &info)) {
				if (id != 0)
					id->accelerator = accelerator;
				result = W_TRUE;
			} else {

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
wresult _w_menuitem_set_id(w_menuitem *item, wushort id) {
	int index = _W_ITEM(item)->index;
	if (index < 0)
		return W_FALSE;
	HMENU hMenu = _W_MENUITEM(item)->menu;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	info.wID = 0;
	if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
		info.wID = (info.wID & 0xFFFF) | (id & 0xFFFF) << 16;
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
			w_shell *shell;
			_w_menu_get_shell((w_menu*) _W_ITEM(item)->parent, &shell);
			_w_menu_id *id = 0;
			int _id = _w_shell_registre_menuitem_id(shell, item, &id);
			info.wID = (info.wID & 0xFFFF8000) | ((_id + 1) & 0x7FFF);
			id->image = hbmpItem;
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
		if ((info.wID & MENU_CHECK) || (info.fState & MFT_RADIOCHECK)) {
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
	w_control *parent = _W_MENU(menu)->parent;
	return w_control_get_shell(parent, shell);
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
wresult _MENU_WM_MENUCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HMENU hMenu = (HMENU) e->lparam;
	int index = e->wparam;
	w_menu *menu = _w_menu_get_top(hMenu);
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
		if (info.wID & MENU_CHECK) {
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
	_W_WIDGETDATA(&item)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
	_W_ITEM(&item)->parent = W_WIDGET(menu);
	_W_ITEM(&item)->index = index;
	_W_MENUITEM(&item)->menu = hMenu;
	_w_widget_send_event(W_WIDGET(menu), (w_event*) &ei);
	e->result = FALSE;
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
		int id = (st->itemID & 0x7FFF) - 1;
		w_shell *shell;
		w_control_get_shell(W_CONTROL(widget), &shell);
		_w_menu_ids *acc = _W_SHELL(shell)->ids;
		if (acc != 0 && id >= 0) {
			_w_menu_id *items = acc->id;
			if (id < acc->count && items[id].image != 0) {
				BITMAP bm;
				GetObjectW(items[id].image, sizeof(bm), &bm);
				HDC srcHdc = CreateCompatibleDC(NULL);
				HBITMAP oldHBitmap = SelectObject(srcHdc, items[id].image);
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
	}
	return W_FALSE;
}
wresult _MENU_WM_MEASUREITEM(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	MEASUREITEMSTRUCT *st = (MEASUREITEMSTRUCT*) e->lparam;
	if (st->CtlType == ODT_MENU) {
		int id = (st->itemID & 0x7FFF) - 1;
		w_shell *shell;
		w_control_get_shell(W_CONTROL(widget), &shell);
		_w_menu_ids *acc = _W_SHELL(shell)->ids;
		if (acc != 0 && id >= 0) {
			_w_menu_id *items = acc->id;
			if (id < acc->count && items[id].image != 0) {
				BITMAP bm;
				GetObjectW(items[id].image, sizeof(bm), &bm);
				st->itemWidth = bm.bmWidth;
				st->itemHeight = bm.bmHeight;
			}
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
