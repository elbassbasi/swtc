/*
 * listview.c
 *
 *  Created on: 5 févr. 2021
 *      Author: azeddine
 */
#include "treeview.h"
#include "../widgets/toolkit.h"
/*
 * listitem
 */
wresult _w_listitem_copy(w_widgetdata *from, w_widgetdata *to) {
	return W_FALSE;
}
wresult _w_listitem_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
	return W_FALSE;
}
wresult _w_listitem_get_data(w_item *item, void **data) {
	w_widget *parent = _W_ITEM(item)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	w_class_id class_id = clazz->class_id;
	wresult result = W_FALSE;
	HWND handle = _W_WIDGET(parent)->handle;
	UINT msg_set, msg_get;
	LPARAM *lParam;
	LISTITEM lItem;
	if (class_id == _W_CLASS_TREEVIEW) {
		lItem.tvItem.hItem = _W_TREEITEM(item)->htreeitem;
		lParam = &lItem.tvItem.lParam;
		msg_get = TVM_GETITEMW;
	} else {
		lItem.lvItem.iSubItem = _W_ITEM(item)->index;
		lParam = &lItem.lvItem.lParam;
		msg_get = LVM_GETITEMW;
	}
	lItem.tvItem.mask = TVIF_PARAM | TVIF_STATE;
	*lParam = 0;
	*data = 0;
	if (SendMessageW(handle, msg_get, 0, (LPARAM) &lItem)) {
		if (_W_WIDGET(parent)->style & W_VIRTUAL) {
			*data = (void*) *lParam;
		} else {
			UINT state =
					class_id == _W_CLASS_TREEVIEW ?
							lItem.tvItem.state : lItem.lvItem.state;
			if (state & ITEM_STATE_PARAMMASK) {
				w_array *arr = (w_array*) *lParam;
				if (arr != 0) {
					*data = arr->user_data;
				}
			} else {
				*data = (void*) *lParam;
			}
		}
		result = W_TRUE;
	}
	return result;
}
wresult _w_listitem_get_text_0(w_listitem *item, int index, w_alloc alloc,
		void *user_data, int enc) {
	w_item_attr attr;
	attr.alloc = alloc;
	attr.user_data = user_data;
	attr.enc = enc;
	return _w_listitem_get_attr(item, index, W_ITEM_ATTR_MASK_TEXT, &attr);
}
wresult _w_listitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return _w_listitem_get_text_0(W_LISTITEM(item), 0, alloc, user_data, enc);
}
wresult _w_listitem_set_data(w_item *item, void *data) {
	w_widget *parent = _W_ITEM(item)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	w_class_id class_id = clazz->class_id;
	HWND handle = _W_WIDGET(parent)->handle;
	wresult result = W_FALSE;
	UINT msg_set, msg_get;
	LPARAM *lParam;
	LISTITEM lItem;
	if (class_id == _W_CLASS_TREEVIEW) {
		lItem.tvItem.hItem = _W_TREEITEM(item)->htreeitem;
		lParam = &lItem.tvItem.lParam;
		msg_set = TVM_SETITEMW;
		msg_get = TVM_GETITEMW;
	} else {
		lItem.lvItem.iSubItem = _W_ITEM(item)->index;
		lParam = &lItem.lvItem.lParam;
		msg_set = LVM_SETITEMW;
		msg_get = LVM_GETITEMW;
	}
	if (_W_WIDGET(parent)->style & W_VIRTUAL) {
		lItem.tvItem.mask = TVIF_PARAM;
		*lParam = (LPARAM) data;
		if (SendMessageW(handle, msg_set, 0, (LPARAM) &lItem)) {
			result = W_TRUE;
		}
	} else {
		lItem.tvItem.mask = TVIF_PARAM | TVIF_STATE;
		if (SendMessageW(handle, msg_get, 0, (LPARAM) &lItem)) {
			lItem.tvItem.mask = TVIF_PARAM;
			UINT state =
					class_id == _W_CLASS_TREEVIEW ?
							lItem.tvItem.state : lItem.lvItem.state;
			if (state & ITEM_STATE_PARAMMASK) {
				w_array *arr = (w_array*) *lParam;
				if (arr != 0) {
					result = W_TRUE;
					arr->user_data = data;
				}
			} else {
				*lParam = (LPARAM) data;
				if (SendMessageW(handle, msg_set, 0, (LPARAM) &lItem)) {
					result = W_TRUE;
				}
			}
		}
	}
	return result;
}
wresult _w_listitem_set_text_0(w_listitem *item, int index, const char *text,
		int length, int enc) {
	w_item_attr attr;
	attr.text = (char*) text;
	attr.length = length;
	attr.enc = enc;
	return _w_listitem_set_attr(item, index, W_ITEM_ATTR_MASK_TEXT, &attr);
}
wresult _w_listitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	return _w_listitem_set_text_0(W_LISTITEM(item), 0, text, length, enc);
}
wresult _w_listitem_get_attr_0(w_listitem *item, int index, int mask,
		w_item_attr *attr, int flags) {
	w_widget *parent = _W_ITEM(item)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	w_class_id class_id = clazz->class_id;
	HWND handle = _W_WIDGET(parent)->handle;
	wresult result = W_FALSE;
	UINT msg_set, msg_get;
	LPARAM *lParam;
	LISTITEM lItem;
	if (class_id == _W_CLASS_TREEVIEW) {
		lItem.tvItem.hItem = _W_TREEITEM(item)->htreeitem;
		lItem.tvItem.stateMask = TVIS_STATEIMAGEMASK;
		lParam = &lItem.tvItem.lParam;
		msg_get = TVM_GETITEMW;
	} else {
		lItem.lvItem.iSubItem = _W_ITEM(item)->index;
		lParam = &lItem.lvItem.lParam;
		lItem.lvItem.stateMask = LVIS_STATEIMAGEMASK;
		msg_get = LVM_GETITEMW;
	}
	if (mask & W_ITEM_ATTR_MASK_FONT) {
		attr->font = 0;
	}
	if (mask & W_ITEM_ATTR_MASK_BACKGROUND) {
		attr->background = 0;
	}
	if (mask & W_ITEM_ATTR_MASK_FORGROUND) {
		attr->foreground = 0;
	}
	if (_W_WIDGET(parent)->style & W_VIRTUAL) {
		if (index == 0) {

		}
		w_event_list event;
		_w_item column;
		W_WIDGETDATA(&column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(parent);
		_W_ITEM(&column)->parent = parent;
		_W_ITEM(&column)->index = index;
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_GET_TEXT;
		event.event.widget = parent;
		event.event.platform_event = 0;
		event.detail = mask;
		event.item = item;
		event.column = W_COLUMNITEM(&column);
		event.textattr = attr;
		result = _w_widget_post_event(parent, W_EVENT(&event), W_EVENT_SEND);
	} else {
		lItem.tvItem.mask = TVIF_PARAM | TVIF_STATE;
		if (SendMessageW(handle, msg_get, 0, (LPARAM) &lItem)) {
			UINT state =
					class_id == _W_CLASS_TREEVIEW ?
							lItem.tvItem.state : lItem.lvItem.state;
			_w_item_list *_i = 0;
			if (state & ITEM_STATE_PARAMMASK) {
				w_array *arr = (w_array*) *lParam;
				_i = (_w_item_list*) w_array_get(arr, index,
						sizeof(_w_item_list));
			} else if (mask & W_ITEM_ATTR_MASK_TEXT) {
				if (!flags) {
					const int str_size = 0x200;
					WCHAR str[str_size];
					lItem.tvItem.mask = TVIF_TEXT;
					if (class_id == _W_CLASS_TREEVIEW) {
						lItem.tvItem.pszText = str;
						lItem.tvItem.cchTextMax = str_size;
					} else {
						lItem.lvItem.pszText = str;
						lItem.lvItem.cchTextMax = str_size;
					}
					str[0] = 0;
					if (SendMessageW(handle, msg_get, 0, (LPARAM) &lItem)) {
						str[str_size - 1] = 0;
						w_alloc_set_text(attr->alloc, attr->user_data,
								attr->enc, (char*) str, -1,
								W_ENCODING_PLATFORM);
					}
				}
			}
			if (_i != 0) {
				if (mask & W_ITEM_ATTR_MASK_TEXT) {
					_win_text_set_0(_i->text, -1, attr->alloc, attr->user_data,
							attr->enc);
				}
				if (mask & W_ITEM_ATTR_MASK_FONT) {
					attr->font = _i->font;
				}
				if (mask & W_ITEM_ATTR_MASK_BACKGROUND) {
					attr->background = _i->background;
				}
				if (mask & W_ITEM_ATTR_MASK_FORGROUND) {
					attr->foreground = _i->foreground;
				}
				result = W_TRUE;
			}
		}
	}
	return result;
}
wresult _w_listitem_get_attr(w_listitem *item, int index, int mask,
		w_item_attr *attr) {
	return _w_listitem_get_attr_0(item, index, mask, attr, 0);
}
wresult _w_listitem_get_bounds(w_listitem *item, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_listitem_get_bounds_index(w_listitem *item, int index,
		w_rect *bounds) {
	w_widget *parent = _W_ITEM(item)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	w_class_id class_id = clazz->class_id;
	if (class_id == _W_CLASS_TREEVIEW) {
		RECT rect;
		_w_treeitem_get_bounds_0(W_TREEITEM(item), &rect, 0, index,
				TREEVIEW_BOUNDS_GET_TEXT_CLIP | TREEVIEW_BOUNDS_GET_IMAGE);
		bounds->x = rect.left;
		bounds->y = rect.top;
		bounds->width = rect.right - rect.left;
		bounds->height = rect.bottom - rect.top;
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_listitem_get_checked_0(w_listitem *item, int mask) {
	w_widget *parent = _W_ITEM(item)->parent;
	if ((_W_WIDGET(parent)->style & W_CHECK) == 0)
		return W_FALSE;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	w_class_id class_id = clazz->class_id;
	HWND handle = _W_WIDGET(parent)->handle;
	wresult result = W_FALSE;
	UINT msg_set;
	LISTITEM lItem;
	if (class_id == _W_CLASS_TREEVIEW) {
		lItem.tvItem.hItem = _W_TREEITEM(item)->htreeitem;
		lItem.tvItem.mask = TVIF_HANDLE | TVIF_STATE;
		lItem.tvItem.stateMask = TVIS_STATEIMAGEMASK;
		SendMessageW(handle, TVM_GETITEM, 0, (LPARAM) &lItem.tvItem);
		result = (lItem.tvItem.state & mask) != 0;
		SendMessageW(handle, TVM_SETITEMW, 0, (LPARAM) &lItem.tvItem);
	} else {
		lItem.lvItem.iItem = _W_ITEM(item)->index;
		lItem.lvItem.mask = TVIF_STATE;
		lItem.lvItem.iSubItem = 0;
		lItem.lvItem.stateMask = TVIS_STATEIMAGEMASK;
		SendMessageW(handle, LVM_GETITEM, 0, (LPARAM) &lItem.lvItem);
		result = (lItem.lvItem.state & mask) != 0;
		SendMessageW(handle, LVM_SETITEMW, 0, (LPARAM) &lItem.lvItem);
	}
	return result;
}
wresult _w_listitem_get_checked(w_listitem *item) {
	return _w_listitem_get_checked_0(item, ITEM_STATE_CHECKEDNORMAL);
}
wresult _w_listitem_get_grayed(w_listitem *item) {
	return _w_listitem_get_checked_0(item, ITEM_STATE_MIXEDNORMAL);
}
wresult _w_listitem_get_image(w_listitem *item) {
	return W_FALSE;
}
wresult _w_listitem_set_attr(w_listitem *item, int index, int mask,
		w_item_attr *attr) {
	w_widget *parent = _W_ITEM(item)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	w_class_id class_id = clazz->class_id;
	HWND handle = _W_WIDGET(parent)->handle;
	wresult result = W_FALSE;
	UINT msg_set, msg_get;
	LPARAM *lParam;
	LISTITEM lItem;
	if (class_id == _W_CLASS_TREEVIEW) {
		lItem.tvItem.hItem = _W_TREEITEM(item)->htreeitem;
		lItem.tvItem.stateMask = TVIS_STATEIMAGEMASK;
		lParam = &lItem.tvItem.lParam;
		msg_set = TVM_SETITEMW;
		msg_get = TVM_GETITEMW;
	} else {
		lItem.lvItem.iSubItem = _W_ITEM(item)->index;
		lParam = &lItem.lvItem.lParam;
		lItem.lvItem.stateMask = LVIS_STATEIMAGEMASK;
		msg_set = LVM_SETITEMW;
		msg_get = LVM_GETITEMW;
	}
	if (_W_WIDGET(parent)->style & W_VIRTUAL) {
		if (index == 0) {

		}
		w_event_list event;
		_w_item column;
		W_WIDGETDATA(&column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(parent);
		_W_ITEM(&column)->parent = parent;
		_W_ITEM(&column)->index = index;
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_SET_TEXT;
		event.event.widget = parent;
		event.event.platform_event = 0;
		event.detail = mask;
		event.item = item;
		event.column = W_COLUMNITEM(&column);
		event.textattr = attr;
		result = _w_widget_post_event(parent, W_EVENT(&event), W_EVENT_SEND);
	} else {
		lItem.tvItem.mask = TVIF_PARAM | TVIF_STATE;
		if (SendMessageW(handle, msg_get, 0, (LPARAM) &lItem)) {
			UINT state =
					class_id == _W_CLASS_TREEVIEW ?
							lItem.tvItem.state : lItem.lvItem.state;
			int update = W_FALSE;
			_w_item_list *_i = 0;
			if (state & ITEM_STATE_PARAMMASK) {
				w_array *arr = (w_array*) *lParam;
				_i = (_w_item_list*) w_array_set(&arr, index,
						sizeof(_w_item_list));
				if (arr != (w_array*) *lParam) {
					*lParam = (LPARAM) arr;
					lItem.tvItem.mask = TVIF_PARAM;
					update = W_TRUE;
				}
			} else {
				int need_arr = index != 0 || (mask & W_ITEM_ATTR_MASK_FONT) != 0
						|| (mask & W_ITEM_ATTR_MASK_BACKGROUND) != 0
						|| (mask & W_ITEM_ATTR_MASK_FORGROUND) != 0;
				if (need_arr) {
					w_array *arr = 0;
					_i = (_w_item_list*) w_array_set(&arr, index,
							sizeof(_w_item_list));
					if (arr != 0) {
						arr->user_data = (void*) *lParam;
						if (index != 0
								|| (index == 0
										&& (mask & W_ITEM_ATTR_MASK_TEXT) == 0)) {
							_w_item_list *f_el = w_array_get(arr, index,
									sizeof(_w_item_list));
							const int str_size = 0x200;
							WCHAR str[str_size];
							lItem.tvItem.mask = TVIF_TEXT;
							if (class_id == _W_CLASS_TREEVIEW) {
								lItem.tvItem.pszText = str;
								lItem.tvItem.cchTextMax = str_size;
							} else {
								lItem.lvItem.pszText = str;
								lItem.lvItem.cchTextMax = str_size;
							}
							str[0] = 0;
							if (SendMessageW(handle, msg_get, 0,
									(LPARAM) &lItem)) {
								str[str_size] = 0;
								w_alloc_set_text(w_alloc_buffer_new,
										&f_el->text, W_ENCODING_UTF8,
										(char*) str, -1, W_ENCODING_PLATFORM);
							}
						}
						if (class_id == _W_CLASS_TREEVIEW) {
							lItem.tvItem.state |= ITEM_STATE_PARAMMASK;
							lItem.tvItem.pszText = LPSTR_TEXTCALLBACKW;
						} else {
							lItem.lvItem.state |= ITEM_STATE_PARAMMASK;
							lItem.tvItem.pszText = LPSTR_TEXTCALLBACKW;
						}
						*lParam = (LPARAM) arr;
						lItem.tvItem.mask = TVIF_PARAM | TVIF_STATE | TVIF_TEXT;
						update = W_TRUE;
					}
				} else {
					WCHAR *str;
					int newlength;
					_win_text_fix(attr->text, attr->length, attr->enc, &str,
							&newlength);
					if (str != 0) {
						lItem.tvItem.mask = TVIF_TEXT;
						if (class_id == _W_CLASS_TREEVIEW) {
							lItem.tvItem.pszText = str;
							lItem.tvItem.cchTextMax = newlength;
						} else {
							lItem.lvItem.pszText = str;
							lItem.lvItem.cchTextMax = newlength;
						}
						if (SendMessageW(handle, msg_set, 0, (LPARAM) &lItem)) {
							result = W_TRUE;
						}
					}
					_win_text_free(attr->text, str, newlength);
				}
			}
			if (_i != 0) {
				if (mask & W_ITEM_ATTR_MASK_TEXT) {
					_win_text_copy(&_i->text, attr->text, attr->length,
							attr->enc);
				}
				if (mask & W_ITEM_ATTR_MASK_FONT) {
					_i->font = attr->font;
				}
				if (mask & W_ITEM_ATTR_MASK_BACKGROUND) {
					_i->background = attr->background;
				}
				if (mask & W_ITEM_ATTR_MASK_FORGROUND) {
					_i->foreground = attr->foreground;
				}
			}
			if (update) {
				result = SendMessageW(handle, msg_set, 0, (LPARAM) &lItem);
			}
		}
	}
	return result;
}
wresult _w_listitem_set_checked(w_listitem *item, int checked) {
}
wresult _w_listitem_set_grayed(w_listitem *item, int grayed) {

}
wresult _w_listitem_set_image(w_listitem *item, int image) {
	w_widget *parent = _W_ITEM(item)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	w_class_id class_id = clazz->class_id;
	HWND handle = _W_WIDGET(parent)->handle;
	wresult result = W_FALSE;
	UINT msg_set;
	LISTITEM lItem;
	if (class_id == _W_CLASS_TREEVIEW) {
		lItem.tvItem.hItem = _W_TREEITEM(item)->htreeitem;
		lItem.tvItem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		lItem.tvItem.iImage = image;
		lItem.tvItem.iSelectedImage = image;
		msg_set = TVM_SETITEMW;
	} else {
		lItem.lvItem.iItem = _W_ITEM(item)->index;
		lItem.lvItem.mask = LVIF_IMAGE;
		lItem.lvItem.iSubItem = 0;
		msg_set = LVM_SETITEMW;
	}
	if (SendMessageW(handle, msg_set, 0, (LPARAM) &lItem)) {
		result = W_TRUE;
	}
	return result;
}
/*
 * listview
 */
wresult _w_listview_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result = _w_composite_create_handle(control, priv);
	if (result <= 0)
		return result;
	_W_WIDGET(control)->state &= ~(STATE_CANVAS | STATE_THEME_BACKGROUND);
	HWND handle = _W_WIDGET(control)->handle;

	/* Use the Explorer theme */
	if (win_toolkit->EXPLORER_THEME) {
		if (WIN32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
			_W_LISTVIEWBASE(control)->explorerTheme = TRUE;
			SetWindowTheme(handle, L"EXPLORER", NULL);
		}
	}

	/* Get the header window proc */
	/*if (HeaderProc == 0) {
	 HWND hwndHeader = SendMessage(handle, LVM_GETHEADER, 0, 0);
	 HeaderProc = GetWindowLongPtr(hwndHeader, GWLP_WNDPROC);
	 }*/

	/*
	 * Feature in Windows.  In version 5.8 of COMCTL32.DLL,
	 * if the font is changed for an item, the bounds for the
	 * item are not updated, causing the text to be clipped.
	 * The fix is to detect the version of COMCTL32.DLL, and
	 * if it is one of the versions with the problem, then
	 * use version 5.00 of the control (a version that does
	 * not have the problem).  This is the recommended work
	 * around from the MSDN.
	 */
	if (_COMCTL32_VERSION < VERSION(6, 0)) {
		SendMessageW(handle, CCM_SETVERSION, 5, 0);
	}

	/*
	 * This code is intentionally commented.  According to
	 * the documentation, setting the default item size is
	 * supposed to improve performance.  By experimentation,
	 * this does not seem to have much of an effect.
	 */
//	SendMessage (handle, LVM_SETITEMCOUNT, 1024 * 2, 0);
	/* Set the checkbox image list */
	/*if ((_W_WIDGET(control)->style & W_CHECK) != 0) {
	 LRESULT empty = SendMessageW(handle, LVM_APPROXIMATEVIEWRECT, 0, 0);
	 LRESULT oneItem = SendMessageW(handle, LVM_APPROXIMATEVIEWRECT, 1, 0);
	 int width = HIWORD (oneItem) - HIWORD(empty), height = width;
	 setCheckboxImageList(control, width, height, FALSE);
	 SendMessage(handle, LVM_SETCALLBACKMASK, LVIS_STATEIMAGEMASK, 0);
	 }*/

	/*
	 * Feature in Windows.  When the control is created,
	 * it does not use the default system font.  A new HFONT
	 * is created and destroyed when the control is destroyed.
	 * This means that a program that queries the font from
	 * this control, uses the font in another control and then
	 * destroys this control will have the font unexpectedly
	 * destroyed in the other control.  The fix is to assign
	 * the font ourselves each time the control is created.
	 * The control will not destroy a font that it did not
	 * create.
	 */
	/*HFONT hFont = GetStockObject(SYSTEM_FONT);
	 SendMessageW(handle, WM_SETFONT, (WPARAM) hFont, 0);*/

	/*
	 * Bug in Windows.  When the first column is inserted
	 * without setting the header text, Windows will never
	 * allow the header text for the first column to be set.
	 * The fix is to set the text to an empty string when
	 * the column is inserted.
	 */
	LVCOLUMNW lvColumn;
	WCHAR pszText[2];
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	pszText[0] = 0;
	lvColumn.pszText = pszText;
	lvColumn.cx = 100;
	SendMessageW(handle, LVM_INSERTCOLUMNW, 0, (LPARAM) &lvColumn);

	/* Set the extended style bits */
	DWORD bits1 = LVS_EX_LABELTIP;
	if ((_W_WIDGET(control)->style & W_FULL_SELECTION) != 0)
		bits1 |= LVS_EX_FULLROWSELECT;
	if ((_W_WIDGET(control)->style & W_CHECK) != 0)
		bits1 |= LVS_EX_CHECKBOXES;
	if (_COMCTL32_VERSION < VERSION(6, 0))
		bits1 |= LVS_EX_DOUBLEBUFFER;
	SendMessageW(handle, LVM_SETEXTENDEDLISTVIEWSTYLE, bits1, bits1);

	/*
	 * Feature in Windows.  Windows does not explicitly set the orientation of
	 * the header.  Instead, the orientation is inherited when WS_EX_LAYOUTRTL
	 * is specified for the table.  This means that when both WS_EX_LAYOUTRTL
	 * and WS_EX_NOINHERITLAYOUT are specified for the table, the header will
	 * not be oriented correctly.  The fix is to explicitly set the orientation
	 * for the header.
	 *
	 * NOTE: WS_EX_LAYOUTRTL is not supported on Windows NT.
	 */
	if (WIN32_VERSION >= VERSION(4, 10)) {
		if ((_W_WIDGET(control)->style & W_RIGHT_TO_LEFT) != 0) {
			HWND hwndHeader = (HWND) SendMessageW(handle, LVM_GETHEADER, 0, 0);
			int bits2 = GetWindowLongW(hwndHeader, GWL_EXSTYLE);
			SetWindowLongW(hwndHeader, GWL_EXSTYLE, bits2 | WS_EX_LAYOUTRTL);
			HWND hwndTooltop = (HWND) SendMessageW(handle, LVM_GETTOOLTIPS, 0,
					0);
			int bits3 = GetWindowLongW(hwndTooltop, GWL_EXSTYLE);
			SetWindowLongW(hwndTooltop, GWL_EXSTYLE, bits3 | WS_EX_LAYOUTRTL);
		}
	}
	return result;
}
DWORD _w_listview_widget_style(w_control *control, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	DWORD bits = _w_composite_widget_style(control, priv);
	bits |= LVS_SHAREIMAGELISTS;
	if ((style & W_HIDE_SELECTION) == 0)
		bits |= LVS_SHOWSELALWAYS;
	if ((style & W_SINGLE) != 0)
		bits |= LVS_SINGLESEL;
	/*
	 * This code is intentionally commented.  In the future,
	 * the FLAT bit may be used to make the header flat and
	 * unresponsive to mouse clicks.
	 */
//	if ((style & W_FLAT) != 0) bits |= LVS_NOSORTHEADER;
	bits |= LVS_REPORT | LVS_NOCOLUMNHEADER;
	if ((style & W_VIRTUAL) != 0)
		bits |= LVS_OWNERDATA;
	return bits;
}
WCHAR* _w_listview_window_class(w_control *control,
		_w_control_priv *priv) {
	return WC_LISTVIEWW;
}
wresult _w_listview_clear_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_clear_indices(w_listview *list, int *indices, int length) {
	return W_FALSE;
}
wresult _w_listview_clear_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_clear_range(w_listview *list, int start, int end) {
	return W_FALSE;
}
wresult _w_listview_deselect_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_deselect_indices(w_listview *table, int *indices,
		int length) {
	return W_FALSE;
}
wresult _w_listview_deselect_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_deselect_range(w_listview *list, int start, int end) {
	return W_FALSE;
}
wresult _w_listview_get_item(w_listview *list, int index, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_get_item_count(w_listview *list) {
	return W_FALSE;
}
wresult _w_listview_get_item_from_point(w_listview *list, w_point *point,
		w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_get_items(w_listview *list, w_iterator *items) {
	return W_FALSE;
}
wresult _w_listview_get_selection_index(w_listview *list) {
	return W_FALSE;
}
wresult _w_listview_get_top_index(w_listview *list) {
	return W_FALSE;
}
wresult _w_listview_get_top_item(w_listview *list, w_listitem *topitem) {
	return W_FALSE;
}
wresult _w_listview_is_selected(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_insert_item(w_listview *list, w_listitem *item, int index) {
	HWND handle = _W_WIDGET(list)->handle;
	int count = SendMessageW(handle, LVM_GETITEMCOUNT, 0, 0);
	if (!(0 <= index && index <= count))
		index = count;
	LVITEMW lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvItem.iItem = index;
	lvItem.iSubItem = 0;
	lvItem.cchTextMax = 0;
	lvItem.pszText = LPSTR_TEXTCALLBACKW;
	/*
	 * Bug in Windows.  Despite the fact that the image list
	 * index has never been set for the item, Windows always
	 * assumes that the image index for the item is valid.
	 * When an item is inserted, the image index is zero.
	 * Therefore, when the first image is inserted and is
	 * assigned image index zero, every item draws with this
	 * image.  The fix is to set the image index when the
	 * the item is created.
	 */
	lvItem.iImage = I_IMAGECALLBACK;

	/* Insert the item */
	//setDeferResize (true);
	//ignoreSelect = ignoreShrink = TRUE;
	int result = SendMessageW(handle, LVM_INSERTITEMW, 0, (LPARAM) &lvItem);
	//ignoreSelect = ignoreShrink = false;
	if (result == -1)
		return W_ERROR_ITEM_NOT_ADDED;
	//setDeferResize (false);

	/* Resize to show the first item */
	//if (count == 0) setScrollWidth (item, false);
	if (item != 0) {
		W_WIDGETDATA(item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(list);
		_W_ITEM(item)->parent = W_WIDGET(list);
		_W_ITEM(item)->index = result;
	}
	return W_TRUE;
}
wresult _w_listview_remove_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_remove_indices(w_listview *list, int *indices, int length) {
	return W_FALSE;
}
wresult _w_listview_remove_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_remove_range(w_listview *list, int start, int end) {
	return W_FALSE;
}
wresult _w_listview_select_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_select_indices(w_listview *list, int *indices, int length) {
	return W_FALSE;
}
wresult _w_listview_select_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_select_range(w_listview *list, int start, int end) {
	return W_FALSE;
}
wresult _w_listview_set_header_visible(w_listviewbase *list, int show) {
	return W_FALSE;
}
wresult _w_listview_set_insert_mark(w_listview *list, w_listitem *item,
		int before) {
	return W_FALSE;
}
wresult _w_listview_set_selection_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_set_selection_indices(w_listview *list, int *indices,
		int length) {
	return W_FALSE;
}
wresult _w_listview_set_selection_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_set_selection_range(w_listview *list, int start, int end) {
	return W_FALSE;
}
wresult _w_listview_set_top_index(w_listview *list, int index) {
	return W_FALSE;
}
wresult _w_listview_set_top_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
wresult _w_listview_show_item(w_listview *list, w_listitem *item) {
	return W_FALSE;
}
void _w_listview_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_listview_class *clazz) {
	if (classId == _W_CLASS_LISTVIEW) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_listview_priv;
	}
	_w_listviewbase_class_init(toolkit, classId, W_LISTVIEWBASE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LISTVIEW;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_listview_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_listview);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_listview);
	/*
	 * function
	 */
	clazz->clear_index = _w_listview_clear_index;
	clazz->clear_indices = _w_listview_clear_indices;
	clazz->clear_item = _w_listview_clear_item;
	clazz->clear_range = _w_listview_clear_range;
	clazz->deselect_index = _w_listview_deselect_index;
	clazz->deselect_indices = _w_listview_deselect_indices;
	clazz->deselect_item = _w_listview_deselect_item;
	clazz->deselect_range = _w_listview_deselect_range;
	clazz->get_item = _w_listview_get_item;
	clazz->get_item_count = _w_listview_get_item_count;
	clazz->get_item_from_point = _w_listview_get_item_from_point;
	clazz->get_items = _w_listview_get_items;
	clazz->get_selection_index = _w_listview_get_selection_index;
	clazz->get_top_index = _w_listview_get_top_index;
	clazz->get_top_item = _w_listview_get_top_item;
	clazz->is_selected = _w_listview_is_selected;
	clazz->insert_item = _w_listview_insert_item;
	clazz->remove_index = _w_listview_remove_index;
	clazz->remove_indices = _w_listview_remove_indices;
	clazz->remove_item = _w_listview_remove_item;
	clazz->remove_range = _w_listview_remove_range;
	clazz->select_index = _w_listview_select_index;
	clazz->select_indices = _w_listview_select_indices;
	clazz->select_item = _w_listview_select_item;
	clazz->select_range = _w_listview_select_range;
	clazz->set_insert_mark = _w_listview_set_insert_mark;
	clazz->set_selection_index = _w_listview_set_selection_index;
	clazz->set_selection_indices = _w_listview_set_selection_indices;
	clazz->set_selection_item = _w_listview_set_selection_item;
	clazz->set_selection_range = _w_listview_set_selection_range;
	clazz->set_top_index = _w_listview_set_top_index;
	clazz->set_top_item = _w_listview_set_top_item;
	clazz->show_item = _w_listview_show_item;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_LISTVIEW) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->create_handle = _w_listview_create_handle;
		priv->widget_style = _w_listview_widget_style;
		priv->window_class = _w_listview_window_class;
	}
}
