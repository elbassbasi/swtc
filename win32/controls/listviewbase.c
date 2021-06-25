/*
 * listviewbase.c
 *
 *  Created on: 5 févr. 2021
 *      Author: azeddine
 */
#include "treeview.h"
#include "../widgets/toolkit.h"
_w_column_list* _w_columns_list_get(w_widget *widget, int index, int create) {
	_w_columns_list *columns = _W_LISTVIEWBASE(widget)->columns;
	if (columns == 0) {
		if (create == TRUE) {
			int count = WMAX(4, index);
			columns = calloc(1,
					sizeof(_w_columns_list) + count * sizeof(_w_column_list));
			if (columns == 0)
				return 0;
			columns->alloc = count;
			_W_LISTVIEWBASE(widget)->columns = columns;
		}
	} else {
		if (columns->alloc <= index && create == TRUE) {
			int count = WMAX(columns->alloc + 4, index);
			columns = realloc(columns,
					sizeof(_w_columns_list) + count * sizeof(_w_column_list));
			if (columns == 0)
				return 0;
			int last = columns->alloc;
			memset(&columns->columns[last], 0,
					(count - last) * sizeof(_w_column_list));
			columns->alloc = count;
			_W_LISTVIEWBASE(widget)->columns = columns;
		}
	}
	if (columns != 0 && columns->alloc > index) {
		return &columns->columns[index];
	} else
		return 0;
}
_w_item_list* _w_items_list_get(_w_items_list **list, int index, int create) {
	_w_items_list *_list = *list;
	if (_list == 0) {
		if (create == TRUE) {
			int count = WMAX(4, index);
			_list = calloc(1,
					sizeof(_w_items_list) + count * sizeof(_w_item_list));
			if (_list == 0)
				return 0;
			_list->alloc = count;
			*list = _list;
		}
	} else {
		if (_list->alloc <= index && create == TRUE) {
			int count = WMAX(_list->alloc + 4, index);
			_list = realloc(_list,
					sizeof(_w_items_list) + count * sizeof(_w_item_list));
			if (_list == 0)
				return 0;
			int last = _list->alloc;
			memset(&_list->columns[last], 0,
					(count - last) * sizeof(_w_item_list));
			_list->alloc = count;
			*list = _list;
		}
	}
	if (_list != 0 && _list->alloc > index) {
		return &_list->columns[index];
	} else
		return 0;
}
/*
 * columnitem
 */
wresult _w_columnitem_get_data(w_item *item, void **data) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
	int index = _W_ITEM(item)->index;
	return W_FALSE;
}
wresult _w_columnitem_get_index(w_item *item) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
	int index = _W_ITEM(item)->index;
	return index;
}
wresult _w_columnitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
	int index = _W_ITEM(item)->index;
	return W_FALSE;
}
wresult _w_columnitem_set_data(w_item *item, void *data) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
	int index = _W_ITEM(item)->index;
	return W_FALSE;
}
wresult _w_columnitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	int index = _W_ITEM(item)->index;
	WCHAR *str;
	int newlength;
	/*
	 * Bug in Windows.  When a column header contains a
	 * mnemonic character, Windows does not measure the
	 * text properly.  This causes '...' to always appear
	 * at the end of the text.  The fix is to remove
	 * mnemonic characters and replace doubled mnemonics
	 * with spaces.
	 */
	int replace = WIN32_VERSION <= VERSION(4, 10);
	wresult result = _win_text_fix(text, length, enc, &str, &newlength);
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
		HDITEMW hdItem;
		hdItem.mask = HDI_TEXT;
		hdItem.pszText = str;
		hdItem.cchTextMax = newlength;
		SendMessageW(hwndHeader, HDM_SETITEMW, index, (LPARAM) &hdItem);
	} else {
		HWND handle = _W_WIDGET(parent)->handle;
		/*
		 * Bug in Windows.  For some reason, when the title
		 * of a column is changed after the column has been
		 * created, the alignment must also be reset or the
		 * text does not draw.  The fix is to query and then
		 * set the alignment.
		 */
		LVCOLUMNW lvColumn;
		lvColumn.mask = LVCF_FMT;
		SendMessageW(handle, LVM_GETCOLUMNW, index, (LPARAM) &lvColumn);

		lvColumn.mask |= LVCF_TEXT;
		lvColumn.pszText = str;
		lvColumn.cchTextMax = newlength;
		LRESULT _result = SendMessageW(handle, LVM_SETCOLUMNW, index,
				(LPARAM) &lvColumn);
		if (_result == 0)
			result = W_ERROR_CANNOT_SET_TEXT;
	}
	_win_text_free(text, str, newlength);
	return result;
}
wresult _w_columnitem_get_alignment(w_columnitem *column) {
	w_widget *parent = _W_ITEM(column)->parent;
	int index = _W_ITEM(column)->index;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	int alignment = 0;
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
		HDITEMW hdItem;
		hdItem.mask = HDI_FORMAT;
		hdItem.fmt = 0;
		SendMessageW(hwndHeader, HDM_GETITEMW, index, (LPARAM) &hdItem);
		if (hdItem.fmt & HDF_LEFT)
			alignment |= W_LEFT;
		if (hdItem.fmt & HDF_CENTER)
			alignment |= W_CENTER;
		if (hdItem.fmt & HDF_RIGHT)
			alignment |= W_RIGHT;
	} else {
		LVCOLUMNW lvColumn;
		lvColumn.mask = LVCF_FMT;
		SendMessageW(_W_WIDGET(parent)->handle, LVM_GETCOLUMNW, index,
				(LPARAM) &lvColumn);
		if (lvColumn.fmt & LVCFMT_LEFT)
			alignment |= W_LEFT;
		if (lvColumn.fmt & LVCFMT_CENTER)
			alignment |= W_CENTER;
		if (lvColumn.fmt & LVCFMT_RIGHT)
			alignment |= W_RIGHT;
	}
	return alignment;
}
wresult _w_columnitem_get_image(w_columnitem *column) {
	w_widget *parent = _W_ITEM(column)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	int index = _W_ITEM(column)->index;
	HWND hwndHeader;
	HWND handle = _W_WIDGET(parent)->handle;
	if (clazz->class_id
			== _W_CLASS_TREEVIEW|| _COMCTL32_VERSION < VERSION(6,0)) {
		if (clazz->class_id == _W_CLASS_TREEVIEW)
			hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
		else
			hwndHeader = (HWND) SendMessageW(handle, LVM_GETHEADER, 0, 0);
		HDITEMW hdItem;
		hdItem.mask = HDI_IMAGE;
		hdItem.iImage = -1;
		SendMessageW(hwndHeader, HDM_GETITEM, index, (LPARAM) &hdItem);
		return hdItem.iImage;
	} else {
		LVCOLUMNW lvColumn;
		lvColumn.mask = LVCF_IMAGE;
		SendMessageW(handle, LVM_GETCOLUMNW, index, (LPARAM) &lvColumn);
		return lvColumn.iImage;
	}
}
wresult _w_columnitem_get_moveable_0(w_widget *tree, int index) {
	if (index == 0)
		return W_FALSE;
	_w_columns_list *columns = _W_LISTVIEWBASE(tree)->columns;
	if (columns != 0 && columns->alloc > index) {
		return (columns->columns[index].flags & _W_TREECOLUMN_NOT_MOVEABLE) == 0;
	} else {
		return W_TRUE;
	}
}
wresult _w_columnitem_get_moveable(w_columnitem *column) {
	w_widget *parent = _W_ITEM(column)->parent;
	int index = _W_ITEM(column)->index;
	return _w_columnitem_get_moveable_0(parent, index);
}
wresult _w_columnitem_get_resizable_0(w_widget *tree, int index) {
	_w_columns_list *columns = _W_LISTVIEWBASE(tree)->columns;
	if (columns != 0 && columns->alloc > index) {
		return (columns->columns[index].flags & _W_TREECOLUMN_NOT_RESIZABLE)
				== 0;
	} else {
		return W_TRUE;
	}
}
wresult _w_columnitem_get_resizable(w_columnitem *column) {
	w_widget *parent = _W_ITEM(column)->parent;
	int index = _W_ITEM(column)->index;
	return _w_columnitem_get_resizable_0(parent, index);
}
wresult _w_columnitem_get_tooltip_text(w_columnitem *column, w_alloc alloc,
		void *user_data, int enc) {
	w_widget *parent = _W_ITEM(column)->parent;
	HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
	int index = _W_ITEM(column)->index;
	return W_FALSE;
}
wresult _w_columnitem_get_width(w_columnitem *column) {
	w_widget *parent = _W_ITEM(column)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	int index = _W_ITEM(column)->index;
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
		HDITEMW hdItem;
		hdItem.mask = HDI_WIDTH;
		hdItem.cxy = 0;
		SendMessageW(hwndHeader, HDM_GETITEMW, index, (LPARAM) &hdItem);
		return hdItem.cxy;
	} else {
		return SendMessageW(_W_WIDGET(parent)->handle, LVM_GETCOLUMNWIDTH,
				index, 0);
	}
}
wresult _w_columnitem_pack(w_columnitem *column) {
	w_widget *parent = _W_ITEM(column)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
		HWND handle = _W_WIDGET(parent)->handle;
		int index = _W_ITEM(column)->index;
		int columnWidth = 0;
		RECT headerRect, rect;
		TVITEMW tvItem;
		SendMessageW(hwndHeader, HDM_GETITEMRECT, index, (LPARAM) &headerRect);
		HDC hDC = GetDC(_W_WIDGET(parent)->handle);
		HFONT oldFont = 0, newFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0,
				0);
		if (newFont != 0)
			oldFont = SelectObject(hDC, newFont);
		w_event_list event;
		_w_graphics _gc;
		_w_treeitem item;
		w_rect _r;
		memset(&event, 0, sizeof(event));
		_w_graphics_init(W_GRAPHICS(&_gc), hDC);
		_W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(parent);
		_W_ITEM(&item)->parent = 0;
		tvItem.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_STATE;
		tvItem.hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
		TVGN_ROOT, 0);
		while (tvItem.hItem != 0) {
			SendMessageW(handle, TVM_GETITEMW, 0, (LPARAM) &tvItem);
			if (tvItem.hItem != 0) {
				_W_ITEM(&item)->index = -1;
				_W_TREEITEM(&item)->htreeitem = tvItem.hItem;
				RECT itemRect;
				HFONT hFont = INVALID_HANDLE_VALUE; //item.fontHandle(index);
				if (hFont != INVALID_HANDLE_VALUE)
					hFont = SelectObject(hDC, hFont);
				_w_treeitem_get_bounds_0(W_TREEITEM(&item), &itemRect, hDC,
						index,
						TREEVIEW_BOUNDS_GET_TEXT | TREEVIEW_BOUNDS_GET_IMAGE);
				_r.x = itemRect.left;
				_r.y = itemRect.top;
				_r.width = itemRect.right - itemRect.left;
				_r.height = itemRect.bottom - itemRect.top;
				int itemRight = 0;
				event.detail =
						(tvItem.state & TVIS_SELECTED) != 0 ? W_SELECTED : 0;
				event.textattr = 0;
				event.gc = W_GRAPHICS(&_gc);
				event.column = column;
				event.item = W_LISTITEM(&item);
				event.rect = &_r;
				event.event.widget = parent;
				event.event.platform_event = 0;
				event.event.type = W_EVENT_ITEM_MEASURE;
				wresult ret = w_widget_send_event(parent, (w_event*) &event);
				if (ret) {
					itemRight = _r.x + _r.width;
				} else {
					itemRight = itemRect.right;
				}
				if (hFont != INVALID_HANDLE_VALUE)
					SelectObject(hDC, hFont);
				columnWidth = WMAX(columnWidth, itemRight - headerRect.left);
			}
			tvItem.hItem = (HTREEITEM) SendMessageW(handle,
			TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM) tvItem.hItem);
		}
		size_t size;
		WCHAR *str = _w_toolkit_malloc_all(&size);
		tvItem.mask = HDI_TEXT;
		tvItem.pszText = str;
		tvItem.cchTextMax = size / sizeof(WCHAR);
		SendMessageW(hwndHeader, HDM_GETITEMW, index, (LPARAM) &tvItem);
		if (tvItem.cchTextMax >= size / sizeof(WCHAR)) {
			_w_toolkit_free(str, size);
			size = tvItem.cchTextMax * sizeof(WCHAR);
			str = _w_toolkit_malloc(size);
			if (str != 0) {
				tvItem.pszText = str;
				SendMessageW(hwndHeader,
				HDM_GETITEMW, index, (LPARAM) &tvItem);
			}
		}
		int flags = DT_CALCRECT | DT_NOPREFIX;
		DrawTextW(hDC, str, tvItem.cchTextMax, &rect, flags);
		int headerWidth = rect.right - rect.left + TREEVIEW_HEADER_MARGIN;
		if (IsAppThemed())
			headerWidth += TREEVIEW_HEADER_EXTRA;
		if (_W_LISTVIEWBASE(parent)->headerimagelist != 0
				|| _W_LISTVIEWBASE(parent)->sortColumn == index) {
			HDITEMW hdItem;
			hdItem.mask = HDI_IMAGE;
			hdItem.iImage = -1;
			SendMessageW(hwndHeader, HDM_GETITEM, index, (LPARAM) &hdItem);
			if (hdItem.iImage == -1
					|| _W_LISTVIEWBASE(parent)->sortColumn == index) {
				if (_W_LISTVIEWBASE(parent)->sortColumn == index
						&& _W_LISTVIEWBASE(parent)->sortDirection != W_NONE) {
					headerWidth += TREEVIEW_SORT_WIDTH;
				} else {
					w_size _sz;
					w_imagelist_get_size(
					_W_LISTVIEWBASE(parent)->headerimagelist, &_sz);
					headerWidth += _sz.width;
				}
				int margin = 0;
				if (hwndHeader != 0) {
					margin = SendMessageW(hwndHeader, HDM_GETBITMAPMARGIN, 0,
							0);
				} else {
					margin = GetSystemMetrics(SM_CXEDGE) * 3;
				}
				headerWidth += margin * 2;
			}
		}
		if (newFont != 0)
			SelectObject(hDC, oldFont);
		ReleaseDC(hwndHeader, hDC);
		_w_toolkit_free(str, size);
		//int gridWidth = parent.linesVisible ? Tree.GRID_WIDTH : 0;
		//setWidthInPixels (Math.max (headerWidth, columnWidth + gridWidth));
	} else {

	}
	return W_TRUE;
}
wresult _w_columnitem_set_alignment(w_columnitem *column, int alignment) {
	w_widget *parent = _W_ITEM(column)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(column)->index;
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(parent);
	HWND hwndHeader;
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
		HDITEMW hdItem;
		RECT rect, headerRect;
		hdItem.mask = HDI_FORMAT;
		SendMessageW(hwndHeader, HDM_GETITEMW, index, (LPARAM) &hdItem);
		hdItem.fmt &= ~HDF_JUSTIFYMASK;
		if ((alignment & W_LEFT) == W_LEFT)
			hdItem.fmt |= HDF_LEFT;
		if ((alignment & W_CENTER) == W_CENTER)
			hdItem.fmt |= HDF_CENTER;
		if ((alignment & W_RIGHT) == W_RIGHT)
			hdItem.fmt |= HDF_RIGHT;
		SendMessageW(hwndHeader, HDM_SETITEMW, index, (LPARAM) &hdItem);
		//_w_control_force_resize(_W_TREECOLUMN(column)->tree);
		GetClientRect(handle, &rect);
		SendMessageW(hwndHeader, HDM_GETITEMRECT, index, (LPARAM) &headerRect);
		rect.left = headerRect.left;
		rect.right = headerRect.right;
		InvalidateRect(handle, &rect, TRUE);
	} else {
		LVCOLUMNW lvColumn;
		lvColumn.mask = LVCF_FMT;
		SendMessageW(handle, LVM_GETCOLUMNW, index, (LPARAM) &lvColumn);
		lvColumn.fmt &= ~LVCFMT_JUSTIFYMASK;
		int fmt = 0;
		if ((alignment & W_LEFT) == W_LEFT)
			fmt = LVCFMT_LEFT;
		if ((alignment & W_CENTER) == W_CENTER)
			fmt = LVCFMT_CENTER;
		if ((alignment & W_RIGHT) == W_RIGHT)
			fmt = LVCFMT_RIGHT;
		lvColumn.fmt |= fmt;
		SendMessageW(handle, LVM_SETCOLUMNW, index, (LPARAM) &lvColumn);
		/*
		 * Bug in Windows.  When LVM_SETCOLUMN is used to change
		 * the alignment of a column, the column is not redrawn
		 * to show the new alignment.  The fix is to compute the
		 * visible rectangle for the column and redraw it.
		 */
		if (index != 0) {
			hwndHeader = (HWND) SendMessageW(handle, LVM_GETHEADER, 0, 0);
			//parent.forceResize ();
			RECT rect, headerRect;
			GetClientRect(handle, &rect);
			SendMessageW(hwndHeader, HDM_GETITEMRECT, index,
					(LPARAM) &headerRect);
			MapWindowPoints(hwndHeader, handle, (LPPOINT) &headerRect, 2);
			rect.left = headerRect.left;
			rect.right = headerRect.right;
			InvalidateRect(handle, &rect, TRUE);
		}
	}
	return W_TRUE;
}
wresult _w_columnitem_set_image(w_columnitem *column, int image) {
	w_widget *parent = _W_ITEM(column)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	int index = _W_ITEM(column)->index;
	HWND handle = _W_WIDGET(parent)->handle;
	if (clazz->class_id
			== _W_CLASS_TREEVIEW|| _COMCTL32_VERSION < VERSION(6,0)) {
		HWND hwndHeader;
		if (clazz->class_id == _W_CLASS_TREEVIEW)
			hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
		else
			hwndHeader = (HWND) SendMessageW(handle, LVM_GETHEADER, 0, 0);
		HDITEMW hdItem;
		hdItem.mask = HDI_IMAGE;
		hdItem.iImage = image;
		SendMessageW(hwndHeader, HDM_SETITEMW, index, (LPARAM) &hdItem);
	} else {
		LVCOLUMNW lvColumn;
		lvColumn.mask = LVCF_FMT | LVCF_IMAGE;
		SendMessageW(handle, LVM_GETCOLUMNW, index, (LPARAM) &lvColumn);
		if (image != -1) {
			lvColumn.fmt |= LVCFMT_IMAGE;
			lvColumn.iImage = image;
			//if (right) lvColumn.fmt |= LVCFMT_BITMAP_ON_RIGHT;
		} else {
			lvColumn.mask &= ~LVCF_IMAGE;
			lvColumn.fmt &= ~(LVCFMT_IMAGE | LVCFMT_BITMAP_ON_RIGHT);
		}
		SendMessageW(handle, LVM_SETCOLUMNW, index, (LPARAM) &lvColumn);
	}
	return W_TRUE;
}
wresult _w_columnitem_set_moveable(w_columnitem *column, int moveable) {
	w_widget *parent = _W_ITEM(column)->parent;
	HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
	int index = _W_ITEM(column)->index;
	if (index == 0)
		return W_FALSE;
	_w_column_list *col = _w_columns_list_get(parent, index, moveable != 0);
	if (col != 0) {
		if (moveable) {
			col->flags &= ~_W_TREECOLUMN_NOT_MOVEABLE;
		} else {
			col->flags |= _W_TREECOLUMN_NOT_MOVEABLE;
		}
	}
	return W_TRUE;
}
wresult _w_columnitem_set_resizable(w_columnitem *column, int resizable) {
	w_widget *parent = _W_ITEM(column)->parent;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
	int index = _W_ITEM(column)->index;
	_w_column_list *col = _w_columns_list_get(parent, index, resizable != 0);
	if (col != 0) {
		if (resizable) {
			col->flags &= ~_W_TREECOLUMN_NOT_RESIZABLE;
		} else {
			col->flags |= _W_TREECOLUMN_NOT_RESIZABLE;
		}
	}
	return W_TRUE;
}
wresult _w_columnitem_set_tooltip_text(w_columnitem *column, const char *text,
		int length, int enc) {
	w_widget *parent = _W_ITEM(column)->parent;
	HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
	int index = _W_ITEM(column)->index;
	return W_FALSE;
}
wresult _w_columnitem_set_width(w_columnitem *column, int width) {
	w_widget *parent = _W_ITEM(column)->parent;
	int index = _W_ITEM(column)->index;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(parent);
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		HWND hwndHeader = _W_TREEVIEW(parent)->hwndHeader;
		HWND handle = _W_WIDGET(parent)->handle;
		HDITEMW hdItem;
		RECT rect, headerRect;
		if (width < 0)
			return W_FALSE;
		hdItem.mask = HDI_WIDTH;
		hdItem.cxy = width;
		SendMessageW(hwndHeader, HDM_SETITEMW, index, (LPARAM) &hdItem);
		SendMessageW(hwndHeader, HDM_GETITEMRECT, index, (LPARAM) &headerRect);
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(parent);
		//priv->force_resize(W_CONTROL(parent),priv);
		GetClientRect(handle, &rect);
		rect.left = headerRect.left;
		InvalidateRect(handle, &rect, TRUE);
		_w_treeview_set_scroll_width(W_TREEVIEW(parent));
	} else {
		SendMessageW(_W_WIDGET(parent)->handle, LVM_SETCOLUMNWIDTH, index,
				width);
	}
	return W_TRUE;
}
wuint64 _w_listviewbase_check_style(w_widget *widget, wuint64 style) {
	/*
	 * Feature in Windows.  Even when WS_HSCROLL or
	 * WS_VSCROLL is not specified, Windows creates
	 * trees and tables with scroll bars.  The fix
	 * is to set H_SCROLL and V_SCROLL.
	 *
	 * NOTE: This code appears on all platforms so that
	 * applications have consistent scroll bar behavior.
	 */
	if ((style & W_NO_SCROLL) == 0) {
		style |= W_HSCROLL | W_VSCROLL;
	}
	/*
	 * Note: Windows only supports TVS_NOSCROLL and TVS_NOHSCROLL.
	 */
	if ((style & W_HSCROLL) != 0 && (style & W_VSCROLL) == 0) {
		style |= W_VSCROLL;
	}
	return _w_widget_check_bits(style, W_SINGLE, W_MULTI, 0, 0, 0, 0);
}
wresult _w_listviewbase_clear_all(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_deselect_all(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_column(w_listviewbase *list, int index,
		w_columnitem *column) {
	if (column == 0)
		return W_ERROR_INVALID_ARGUMENT;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(list);
	int columnCount;
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		if (_W_TREEVIEW(list)->hwndHeader == 0) {
			wresult result = _w_treeview_create_parent(W_TREEVIEW(list),
					_W_CONTROL_GET_PRIV(list));
			if (result <= 0)
				return result;
		}
		HWND hwndHeader = _W_TREEVIEW(list)->hwndHeader;
		int columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
	} else {

	}
	if (index >= columnCount)
		return W_ERROR_INVALID_RANGE;
	_W_WIDGETDATA(column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(list);
	_W_ITEM(column)->parent = W_WIDGET(list);
	_W_ITEM(column)->index = index;
	return W_TRUE;
}
wresult _w_listviewbase_get_column_count(w_listviewbase *list) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(list);
	HWND hwndHeader;
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		hwndHeader = _W_TREEVIEW(list)->hwndHeader;
	} else {
		hwndHeader = (HWND) SendMessageW(_W_WIDGET(list)->handle, LVM_GETHEADER,
				0, 0);
	}
	return SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
}
wresult _w_listviewbase_get_columns(w_listviewbase *list, w_iterator *columns) {
	return W_FALSE;
}
wresult _w_listviewbase_get_gridline_width(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_header_height(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_header_imagelist(w_listviewbase *list,
		w_imagelist **imagelist) {
	*imagelist = _W_LISTVIEWBASE(list)->headerimagelist;
	return W_TRUE;
}
wresult _w_listviewbase_get_header_visible(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_imagelist(w_listviewbase *list,
		w_imagelist **imagelist) {
	*imagelist = _W_LISTVIEWBASE(list)->imagelist;
	return W_TRUE;
}
wresult _w_listviewbase_get_item_height(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_lines_visible(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_selection(w_listviewbase *list,
		w_iterator *selection) {
	return W_FALSE;
}
wresult _w_listviewbase_get_selection_count(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_get_sort_column(w_listviewbase *list,
		w_columnitem *column) {
	return W_FALSE;
}
wresult _w_listviewbase_get_sort_direction(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_insert_column(w_listviewbase *list,
		w_columnitem *column, int index) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(list);
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(list);
	HWND handle = _W_WIDGET(list)->handle;
	HWND hwndHeader;
	int columnCount;
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		if (_W_TREEVIEW(list)->hwndHeader == 0) {
			wresult result = _w_treeview_create_parent(W_TREEVIEW(list), priv);
			if (result <= 0)
				return result;
		}
		hwndHeader = _W_TREEVIEW(list)->hwndHeader;
		columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
		if (index < 0 || index > columnCount)
			index = columnCount;
		HDITEMW hdItem;
		hdItem.mask = HDI_TEXT | HDI_WIDTH;
		hdItem.pszText = L"";
		hdItem.cchTextMax = 0;
		hdItem.cxy = 50;
		index = SendMessageW(hwndHeader, HDM_INSERTITEMW, index,
				(LPARAM) &hdItem);
		if (index < 0)
			return W_ERROR_ITEM_NOT_ADDED;

		/* When the first column is created, hide the horizontal scroll bar */
		if (columnCount == 0) {
//scrollWidth = 0;
			if ((_W_WIDGET(list)->style & W_HSCROLL) != 0) {
				int bits = GetWindowLongW(handle, GWL_STYLE);
				bits |= TVS_NOHSCROLL;
				SetWindowLongW(handle, GWL_STYLE, bits);
			}
			/*
			 * Bug in Windows.  When TVS_NOHSCROLL is set after items
			 * have been inserted into the tree, Windows shows the
			 * scroll bar.  The fix is to check for this case and
			 * explicitly hide the scroll bar explicitly.
			 */
			int count = SendMessageW(handle, TVM_GETCOUNT, 0, 0);
			if (count != 0) {
				ShowScrollBar(handle, SB_HORZ, FALSE);
			}
			/*createItemToolTips();
			 if (itemToolTipHandle != 0) {
			 SendMessage (itemToolTipHandle, TTM_SETDELAYTIME, TTDT_AUTOMATIC, -1);
			 }*/
		}
		_w_treeview_set_scroll_width(W_TREEVIEW(list));
		//updateImageList();
		_w_treeview_update_scrollbar(W_TREEVIEW(list));

		/* Redraw to hide the items when the first column is created */
		if (columnCount == 0 && SendMessageW(handle, TVM_GETCOUNT, 0, 0) != 0) {
			InvalidateRect(handle, NULL, TRUE);
		}
	} else {
		hwndHeader = (HWND) SendMessageW(handle, LVM_GETHEADER, 0, 0);
		int oldColumn = SendMessageW(handle, LVM_GETSELECTEDCOLUMN, 0, 0);
		if (oldColumn >= index) {
			SendMessageW(handle, LVM_SETSELECTEDCOLUMN, oldColumn + 1, 0);
		}
		columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
		if (index < 0 || index > columnCount)
			index = columnCount;

		/*
		 * Ensure that resize listeners for the table and for columns
		 * within the table are not called.  This can happen when the
		 * first column is inserted into a table or when a new column
		 * is inserted in the first position.
		 */
		//ignoreColumnResize = true;
		if (index == 0) {
			if (columnCount > 1) {
				LVCOLUMNW lvColumn;
				lvColumn.mask = LVCF_WIDTH;
				lvColumn.cx = 0;
				SendMessageW(handle, LVM_INSERTCOLUMNW, 1, (LPARAM) &lvColumn);
				SendMessageW(handle, LVM_GETCOLUMNW, 1, (LPARAM) &lvColumn);
				int width = lvColumn.cx;
				lvColumn.mask = LVCF_IMAGE | LVCF_WIDTH | LVCF_FMT;
				SendMessageW(handle, LVM_GETCOLUMNW, 0, (LPARAM) &lvColumn);
				SendMessageW(handle, LVM_SETCOLUMNW, 1, (LPARAM) &lvColumn);
				lvColumn.fmt = LVCFMT_IMAGE;
				lvColumn.cx = width;
				lvColumn.iImage = I_IMAGENONE;
				//lvColumn.pszText = lvColumn.cchTextMax = 0;
				SendMessageW(handle, LVM_SETCOLUMNW, 0, (LPARAM) &lvColumn);
				lvColumn.mask = LVCF_FMT;
				lvColumn.fmt = LVCFMT_LEFT;
				SendMessageW(handle, LVM_SETCOLUMNW, 0, (LPARAM) &lvColumn);
			} else {
				SendMessageW(handle, LVM_SETCOLUMNWIDTH, 0, 0);
			}
			/*
			 * Bug in Windows.  Despite the fact that every item in the
			 * table always has LPSTR_TEXTCALLBACK, Windows caches the
			 * bounds for the selected items.  This means that
			 * when you change the string to be something else, Windows
			 * correctly asks you for the new string but when the item
			 * is selected, the selection draws using the bounds of the
			 * previous item.  The fix is to reset LPSTR_TEXTCALLBACK
			 * even though it has not changed, causing Windows to flush
			 * cached bounds.
			 */
			/*if ((style & SWT.VIRTUAL) == 0) {
			 LVITEM lvItem = new LVITEM ();
			 lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
			 lvItem.pszText = LPSTR_TEXTCALLBACK;
			 lvItem.iImage = I_IMAGECALLBACK;
			 for (int i=0; i<itemCount; i++) {
			 lvItem.iItem = i;
			 SendMessage (handle, LVM_SETITEM, 0, lvItem);
			 }
			 }*/
		} else {
			LVCOLUMNW lvColumn;
			lvColumn.mask = LVCF_FMT;
			//lvColumn.cx = 10;
			lvColumn.fmt = LVCFMT_LEFT;
			SendMessageW(handle, LVM_INSERTCOLUMNW, index, (LPARAM) &lvColumn);
		}
		//ignoreColumnResize = false;

	}

	/* Add the tool tip item for the header */
	HWND headerToolTipHandle = _W_LISTVIEWBASE(list)->headerToolTipHandle;
	if (headerToolTipHandle != 0) {
		RECT rect;
		if (SendMessageW(hwndHeader, HDM_GETITEMRECT, index, (LPARAM) &rect)
				!= 0) {
			TOOLINFOW lpti;
			lpti.cbSize = sizeof(lpti);
			lpti.uFlags = TTF_SUBCLASS;
			lpti.hwnd = hwndHeader;
			lpti.uId = win_toolkit->nextToolTipId++;
			//column.id = lpti.uId;
			lpti.rect.left = rect.left;
			lpti.rect.top = rect.top;
			lpti.rect.right = rect.right;
			lpti.rect.bottom = rect.bottom;
			lpti.lpszText = LPSTR_TEXTCALLBACKW;
			SendMessageW(headerToolTipHandle, TTM_ADDTOOL, 0, (LPARAM) &lpti);
		}
	}
	if (column != 0) {
		_W_WIDGETDATA(column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(list);
		_W_ITEM(column)->parent = W_WIDGET(list);
		_W_ITEM(column)->index = index;

	}
	return W_TRUE;
}
wresult _w_listviewbase_remove_all(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_select_all(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_set_header_imagelist(w_listviewbase *list,
		w_imagelist *imagelist) {
	HIMAGELIST hImageList;
	wresult ret = W_TRUE;
	if (imagelist == 0) {
		hImageList = 0;
	} else {
		hImageList = _W_IMAGELIST(imagelist)->imagelist;
		if (hImageList == 0) {
			imagelist = 0;
			ret = W_ERROR_INVALID_ARGUMENT;
		}
	}
	_W_LISTVIEWBASE(list)->headerimagelist = imagelist;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(list);
	HWND hwndHeader;
	if (clazz->class_id == _W_CLASS_TREEVIEW)
		hwndHeader = _W_TREEVIEW(list)->hwndHeader;
	else
		hwndHeader = (HWND) SendMessageW(_W_WIDGET(list)->handle, LVM_GETHEADER,
				0, 0);
	SendMessageW(hwndHeader, HDM_SETIMAGELIST, 0, (LPARAM) hImageList);
	return ret;
}
wresult _w_listviewbase_set_header_visible(w_listviewbase *list, int show) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(list);
	wresult result = W_TRUE;
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		if (_W_TREEVIEW(list)->hwndHeader == 0) {
			wresult result = _w_treeview_create_parent(W_TREEVIEW(list),
					_W_CONTROL_GET_PRIV(list));
			if (result <= 0)
				return result;
		}
		HWND hwndHeader = _W_TREEVIEW(list)->hwndHeader;
		int bits = GetWindowLong(hwndHeader, GWL_STYLE);
		if (show) {
			if ((bits & HDS_HIDDEN) == 0)
				return W_TRUE;
			bits &= ~HDS_HIDDEN;
			SetWindowLongW(hwndHeader, GWL_STYLE, bits);
			ShowWindow(hwndHeader, SW_SHOW);
		} else {
			if ((bits & HDS_HIDDEN) != 0)
				return W_TRUE;
			bits |= HDS_HIDDEN;
			SetWindowLongW(hwndHeader, GWL_STYLE, bits);
			ShowWindow(hwndHeader, SW_HIDE);
		}
		_w_treeview_set_scroll_width(W_TREEVIEW(list));
		//updateHeaderToolTips();
		_w_treeview_update_scrollbar(W_TREEVIEW(list));
	} else {
		HWND handle = _W_WIDGET(list)->handle;
		int newBits = GetWindowLongW(handle, GWL_STYLE);
		newBits &= ~LVS_NOCOLUMNHEADER;
		if (!show)
			newBits |= LVS_NOCOLUMNHEADER;
		/*
		 * Feature in Windows.  Setting or clearing LVS_NOCOLUMNHEADER
		 * causes the table to scroll to the beginning.  The fix is to
		 * save and restore the top index causing the table to scroll
		 * to the new location.
		 */
		//int oldIndex = getTopIndex ();
		SetWindowLongW(handle, GWL_STYLE, newBits);
	}
	return result;
}
wresult _w_listviewbase_set_imagelist(w_listviewbase *list,
		w_imagelist *imagelist) {
	HIMAGELIST hImageList;
	wresult ret = W_TRUE;
	if (imagelist == 0) {
		hImageList = 0;
	} else {
		hImageList = _W_IMAGELIST(imagelist)->imagelist;
		if (hImageList == 0) {
			imagelist = 0;
			ret = W_ERROR_INVALID_ARGUMENT;
		}
	}
	_W_LISTVIEWBASE(list)->imagelist = imagelist;
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(list);
	UINT msg;
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		msg = TVM_SETIMAGELIST;
	} else {
		msg = LVM_SETIMAGELIST;
	}
	SendMessageW(_W_WIDGET(list)->handle, msg, LVSIL_NORMAL,
			(LPARAM) hImageList);
	SendMessageW(_W_WIDGET(list)->handle, msg, LVSIL_SMALL,
			(LPARAM) hImageList);
	return ret;
}
wresult _w_listviewbase_set_item_height(w_listviewbase *list, int itemHeight) {
	return W_FALSE;
}
wresult _w_listviewbase_set_lines_visible(w_listviewbase *list, int show) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(list);
	HWND handle = _W_WIDGET(list)->handle;
	if (clazz->class_id == _W_CLASS_TREEVIEW) {
		if (_W_TREEVIEW(list)->linesVisible == show)
			return W_TRUE;
		_W_TREEVIEW(list)->linesVisible = show;
		if (_W_TREEVIEW(list)->hwndParent == 0
				&& _W_TREEVIEW(list)->linesVisible)
			_W_TREEVIEW(list)->customDraw = TRUE;
		InvalidateRect(handle, NULL, TRUE);
	} else {
		DWORD newBits = show ? LVS_EX_GRIDLINES : 0;
		SendMessageW(handle, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_GRIDLINES,
				newBits);
		if (show) {
			int bits = GetWindowLongW(handle, GWL_STYLE);
			if ((bits & LVS_NOCOLUMNHEADER) == 0) {
				//fixItemHeight (TRUE);
			}
		}
	}
	return W_TRUE;
}
wresult _w_listviewbase_set_sort_column(w_listviewbase *list,
		w_columnitem *column) {
	return W_FALSE;
}
wresult _w_listviewbase_set_sort_direction(w_listviewbase *list,
		int direction) {
	return W_FALSE;
}
wresult _w_listviewbase_show_column(w_listviewbase *list,
		w_columnitem *column) {
	return W_FALSE;
}
wresult _w_listviewbase_show_selection(w_listviewbase *list) {
	return W_FALSE;
}
wresult _w_listviewbase_sort(w_listviewbase *list) {
	return W_FALSE;
}
void _w_listviewbase_class_init(struct _w_listviewbase_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	clazz->clear_all = _w_listviewbase_clear_all;
	clazz->deselect_all = _w_listviewbase_deselect_all;
	clazz->get_column = _w_listviewbase_get_column;
	clazz->get_column_count = _w_listviewbase_get_column_count;
	clazz->get_columns = _w_listviewbase_get_columns;
	clazz->get_gridline_width = _w_listviewbase_get_gridline_width;
	clazz->get_header_height = _w_listviewbase_get_header_height;
	clazz->get_header_imagelist = _w_listviewbase_get_header_imagelist;
	clazz->get_header_visible = _w_listviewbase_get_header_visible;
	clazz->get_imagelist = _w_listviewbase_get_imagelist;
	clazz->get_item_height = _w_listviewbase_get_item_height;
	clazz->get_lines_visible = _w_listviewbase_get_lines_visible;
	clazz->get_selection = _w_listviewbase_get_selection;
	clazz->get_selection_count = _w_listviewbase_get_selection_count;
	clazz->get_sort_column = _w_listviewbase_get_sort_column;
	clazz->get_sort_direction = _w_listviewbase_get_sort_direction;
	clazz->insert_column = _w_listviewbase_insert_column;
	clazz->remove_all = _w_listviewbase_remove_all;
	clazz->select_all = _w_listviewbase_select_all;
	clazz->set_header_imagelist = _w_listviewbase_set_header_imagelist;
	clazz->set_header_visible = _w_listviewbase_set_header_visible;
	clazz->set_imagelist = _w_listviewbase_set_imagelist;
	clazz->set_item_height = _w_listviewbase_set_item_height;
	clazz->set_lines_visible = _w_listviewbase_set_lines_visible;
	clazz->set_sort_column = _w_listviewbase_set_sort_column;
	clazz->set_sort_direction = _w_listviewbase_set_sort_direction;
	clazz->show_column = _w_listviewbase_show_column;
	clazz->show_selection = _w_listviewbase_show_selection;
	clazz->sort = _w_listviewbase_sort;
	/*
	 * list item
	 */
	struct _w_listitem_class *listitem = W_LISTITEM_CLASS(clazz->class_item);
	_w_item_class_init(W_ITEM_CLASS(listitem));
	W_ITEM_CLASS(listitem)->get_data = _w_listitem_get_data;
	W_ITEM_CLASS(listitem)->get_text = _w_listitem_get_text;
	W_ITEM_CLASS(listitem)->set_data = _w_listitem_set_data;
	W_ITEM_CLASS(listitem)->set_text = _w_listitem_set_text;
	listitem->get_attr = _w_listitem_get_attr;
	listitem->get_text = _w_listitem_get_text_0;
	listitem->get_bounds = _w_listitem_get_bounds;
	listitem->get_bounds_index = _w_listitem_get_bounds_index;
	listitem->get_checked = _w_listitem_get_checked;
	listitem->get_image = _w_listitem_get_image;
	listitem->set_checked = _w_listitem_set_checked;
	listitem->set_image = _w_listitem_set_image;
	listitem->set_attr = _w_listitem_set_attr;
	listitem->set_text = _w_listitem_set_text_0;
	/*
	 * column item
	 */
	struct _w_columnitem_class *columnitem = W_COLUMNITEM_CLASS(
			clazz->class_column);
	_w_item_class_init(W_ITEM_CLASS(columnitem));
	columnitem->get_alignment = _w_columnitem_get_alignment;
	//columnitem->get_id = _w_columnitem_get_order;
	columnitem->get_image = _w_columnitem_get_image;
	columnitem->get_moveable = _w_columnitem_get_moveable;
	columnitem->get_resizable = _w_columnitem_get_resizable;
	columnitem->get_tooltip_text = _w_columnitem_get_tooltip_text;
	columnitem->get_width = _w_columnitem_get_width;
	columnitem->pack = _w_columnitem_pack;
	columnitem->set_alignment = _w_columnitem_set_alignment;
	//columnitem->set_id = _w_columnitem_set_order;
	columnitem->set_image = _w_columnitem_set_image;
	columnitem->set_moveable = _w_columnitem_set_moveable;
	columnitem->set_resizable = _w_columnitem_set_resizable;
	columnitem->set_tooltip_text = _w_columnitem_set_tooltip_text;
	columnitem->set_width = _w_columnitem_set_width;
	columnitem->item.get_data = _w_columnitem_get_data;
	columnitem->item.get_index = _w_columnitem_get_index;
	columnitem->item.get_text = _w_columnitem_get_text;
	columnitem->item.set_data = _w_columnitem_set_data;
	columnitem->item.set_text = _w_columnitem_set_text;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->check_style = _w_listviewbase_check_style;
}
