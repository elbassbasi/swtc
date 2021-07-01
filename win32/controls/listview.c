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
	if (class_id == _W_CLASS_TREEVIEW) {
		TVITEMW tvItem;
		tvItem.mask = TVIF_PARAM;
		tvItem.lParam = 0;
		tvItem.hItem = _W_TREEITEM(item)->htreeitem;
		*data = 0;
		if (SendMessageW(handle, TVM_GETITEMW, 0, (LPARAM) &tvItem)) {
			if ((_W_WIDGET(parent)->style & W_VIRTUAL) != 0) {
				*data = (void*) tvItem.lParam;
			} else {
				w_array *arr = (w_array*) tvItem.lParam;
				if (arr != 0) {
					*data = arr->user_data;
				}
			}
			return W_TRUE;
		} else {
			return W_FALSE;
		}
	} else {

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
	if (class_id == _W_CLASS_TREEVIEW) {
		TVITEMW tvItem;
		tvItem.hItem = _W_TREEITEM(item)->htreeitem;
		if (_W_WIDGET(parent)->style & W_VIRTUAL) {
			tvItem.mask = TVIF_PARAM;
			tvItem.lParam = (LPARAM) data;
			if (SendMessageW(handle, TVM_SETITEMW, 0, (LPARAM) &tvItem)) {
				result = W_TRUE;
			}
		} else {
			tvItem.mask = TVIF_PARAM;
			tvItem.lParam = 0;
			if (SendMessageW(handle, TVM_GETITEMW, 0, (LPARAM) &tvItem)) {
				w_array *arr = (w_array**) tvItem.lParam;
				w_array *lastarr = arr;
				w_array_set(&arr, 0, sizeof(_w_item_list));
				if (arr != 0) {
					arr->user_data = data;
					if (lastarr != arr) {
						tvItem.mask = TVIF_PARAM;
						tvItem.lParam = (LPARAM) arr;
						if (SendMessageW(handle, TVM_SETITEMW, 0,
								(LPARAM) &tvItem)) {
							result = W_TRUE;
						}
					} else
						result = W_TRUE;
				}
			}
		}
	} else {
		LVITEMW lvItem;

	}
	return result;
}
wresult _w_listitem_set_text_0(w_listitem *item, int index, const char *text,
		int length, int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	w_class_id class_id = clazz->class_id;
	HWND handle = _W_WIDGET(parent)->handle;
	wresult result = W_FALSE;
	LRESULT lresult;
	WCHAR *s;
	int newlength = 0;
	LISTITEM lItem;
	if (class_id == _W_CLASS_TREEVIEW) {
		lItem.tvItem.mask = TVIF_TEXT;
		lItem.tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	} else {
		lItem.lvItem.mask = LVIF_TEXT;
		lItem.lvItem.iSubItem = index;
	}
	if ((_W_WIDGET(parent)->style & W_VIRTUAL) == 0) {
		w_array *arr;
		if (class_id == _W_CLASS_TREEVIEW) {
			lItem.tvItem.mask = TVIF_PARAM;
			lresult = SendMessageW(handle, TVM_GETITEMW, 0,
					(LPARAM) &lItem.tvItem);
			arr = (w_array*) lItem.tvItem.lParam;
		} else {
			lItem.lvItem.mask = LVIF_PARAM;
			lresult = SendMessageW(handle, LVM_GETITEMW, 0,
					(LPARAM) &lItem.lvItem);
			arr = (w_array*) lItem.lvItem.lParam;
		}
		if (lresult) {
			_w_item_list *_i = (_w_item_list*) w_array_set(&arr, index,
					sizeof(_w_item_list));
			if (_i != 0) {
				_win_text_copy(&_i->text, text, length, enc);
			}
			if (class_id == _W_CLASS_TREEVIEW) {
				lItem.tvItem.lParam = (LPARAM) arr;
				lItem.tvItem.mask = TVIF_TEXT | TVIF_PARAM;
			} else {
				lItem.lvItem.lParam = (LPARAM) arr;
				lItem.lvItem.mask = LVIF_TEXT | LVIF_PARAM;
			}
		}
		s = LPSTR_TEXTCALLBACKW;
	} else {
		if (index == 0) {
			if (text == 0)
				s = LPSTR_TEXTCALLBACKW;
			else
				result = _win_text_fix(text, length, enc, &s, &newlength);
		} else {

		}
	}
	if (s != 0) {
		if (class_id == _W_CLASS_TREEVIEW) {
			lItem.tvItem.cchTextMax = newlength;
			lItem.tvItem.pszText = s;
			if (SendMessageW(handle, TVM_SETITEMW, 0, (LPARAM) &lItem.tvItem)) {
				result = W_TRUE;
			}
		} else {
			lItem.lvItem.cchTextMax = newlength;
			lItem.lvItem.pszText = s;
			if (SendMessageW(handle, LVM_SETITEMW, 0, (LPARAM) &lItem.lvItem)) {
				result = W_TRUE;
			}
		}
	}
	if (s != LPSTR_TEXTCALLBACKW)
		_win_text_free(text, s, newlength);
	return result;
}
wresult _w_listitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	return _w_listitem_set_text_0(W_LISTITEM(item), 0, text, length, enc);
}
wresult _w_listitem_get_attr(w_listitem *item, int index, int mask,
		w_item_attr *attr) {
	w_widget *parent = _W_ITEM(item)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	w_class_id class_id = clazz->class_id;
	HWND handle = _W_WIDGET(parent)->handle;
	wresult result = W_FALSE;
	LRESULT lresult;
	TVITEMW tvItem;
	LVITEMW lvItem;
	if (class_id == _W_CLASS_TREEVIEW) {
		tvItem.mask = TVIF_TEXT;
		tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	} else {
		lvItem.mask = LVIF_TEXT;
		lvItem.iSubItem = index;
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
	if ((_W_WIDGET(parent)->style & W_VIRTUAL) == 0) {
		w_array *arr;
		if (class_id == _W_CLASS_TREEVIEW) {
			tvItem.mask = TVIF_PARAM;
			lresult = SendMessageW(handle, TVM_GETITEMW, 0, (LPARAM) &tvItem);
			arr = (w_array*) tvItem.lParam;
		} else {
			lvItem.mask = LVIF_PARAM;
			lresult = SendMessageW(handle, LVM_GETITEMW, 0, (LPARAM) &lvItem);
			arr = (w_array*) lvItem.lParam;
		}
		if (lresult) {
			_w_item_list *_i = (_w_item_list*) w_array_get(arr, index,
					sizeof(_w_item_list));
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
	} else {
		w_event_list event;
		_w_item column;
		W_WIDGETDATA(&column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(parent);
		_W_ITEM(&column)->parent = parent;
		_W_ITEM(&column)->index = index;
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_GET_TEXT;
		event.event.widget = parent;
		event.event.platform_event = 0;
		event.item = item;
		event.column = W_COLUMNITEM(&column);
		event.textattr = attr;
		result = _w_widget_send_event(parent, W_EVENT(&event));
	}
	return result;
}
wresult _w_listitem_get_bounds(w_listitem *item, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_listitem_get_bounds_index(w_listitem *item, int index,
		w_rect *bounds) {
	return W_FALSE;
}
wresult _w_listitem_get_checked(w_listitem *item) {
	return W_FALSE;
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
	LRESULT lresult;
	TVITEMW tvItem;
	LVITEMW lvItem;
	if (class_id == _W_CLASS_TREEVIEW) {
		tvItem.mask = TVIF_TEXT;
		tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	} else {
		lvItem.mask = LVIF_TEXT;
		lvItem.iSubItem = index;
	}
	if ((_W_WIDGET(parent)->style & W_VIRTUAL) == 0) {
		w_array *arr, *lastarr;
		if (class_id == _W_CLASS_TREEVIEW) {
			tvItem.mask = TVIF_PARAM;
			lresult = SendMessageW(handle, TVM_GETITEMW, 0, (LPARAM) &tvItem);
			arr = (w_array*) tvItem.lParam;
		} else {
			lvItem.mask = LVIF_PARAM;
			lresult = SendMessageW(handle, LVM_GETITEMW, 0, (LPARAM) &lvItem);
			arr = (w_array*) lvItem.lParam;
		}
		if (lresult) {
			lastarr = arr;
			_w_item_list *_i = (_w_item_list*) w_array_set(&arr, index,
					sizeof(_w_item_list));
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
				if (lastarr != arr) {
					if (class_id == _W_CLASS_TREEVIEW) {
						tvItem.lParam = (LPARAM) arr;
						lresult = SendMessageW(handle, TVM_SETITEMW, 0,
								(LPARAM) &tvItem);
					} else {
						lvItem.lParam = (LPARAM) arr;
						lresult = SendMessageW(handle, LVM_SETITEMW, 0,
								(LPARAM) &lvItem);
					}
				}
				result = W_TRUE;
			}
		}
	} else {
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
		event.item = item;
		event.column = W_COLUMNITEM(&column);
		event.textattr = attr;
		result = _w_widget_send_event(parent, W_EVENT(&event));
	}
	return result;
}
wresult _w_listitem_set_checked(w_listitem *item, int checked) {
	return W_FALSE;
}
wresult _w_listitem_set_image(w_listitem *item, int image) {
	wresult result = W_FALSE;
	LVITEMW lvItem;
	lvItem.mask = LVIF_IMAGE;
	lvItem.iImage = image;
	lvItem.iItem = _W_ITEM(item)->index;
	lvItem.iSubItem = 0;
	w_widget *list = _W_ITEM(item)->parent;
	if (SendMessageW(_W_WIDGET(list)->handle, LVM_SETITEMW, 0,
			(LPARAM) &lvItem)) {
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
const char* _w_listview_window_class(w_control *control,
		_w_control_priv *priv) {
	return WC_LISTVIEWA;
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
