/*
 * Name:        treeview.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "treeview.h"
#include "../widgets/toolkit.h"
#include <vsstyle.h>
/*
 * treeitem
 */
HTREEITEM _w_treeview_find_item(HWND handle, HTREEITEM hFirstItem, int index) {
	if (hFirstItem == 0)
		return 0;
	int nextIndex = 0;
	HTREEITEM hNextItem = hFirstItem;
	while (hNextItem != 0 && nextIndex < index) {
		hNextItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
				(WPARAM) TVGN_NEXT, (LPARAM) hNextItem);
		nextIndex++;
	}
	if (index == nextIndex) {
		return hNextItem;
	}
	return 0;
}
HTREEITEM _w_treeview_find_previous(HWND handle, HTREEITEM parent, int index) {
	if (index < 0)
		return TVI_LAST;
	if (index == 0)
		return TVI_FIRST;
	HTREEITEM hFirstItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
			(WPARAM) TVGN_CHILD, (LPARAM) parent);
	HTREEITEM hItem = _w_treeview_find_item(handle, hFirstItem, index - 1);
	return hItem;
}
wresult _w_treeitem_insert_item_0(w_treeitem *item, w_treeitem *subitem,
		HTREEITEM after) {
	w_widget *tree = _W_ITEM(item)->parent;
	TV_INSERTSTRUCTW tvInsert;
	tvInsert.hParent = _W_TREEITEM(item)->htreeitem;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE
			| TVIF_STATE;
	tvInsert.item.iSelectedImage = I_IMAGECALLBACK;
	tvInsert.item.iImage = I_IMAGECALLBACK;
	tvInsert.hInsertAfter = after;
	tvInsert.item.pszText = LPSTR_TEXTCALLBACKW;
	if (_W_WIDGET(tree)->style & W_CHECK) {
		tvInsert.item.mask |= TVIF_STATE;
		tvInsert.item.state = 0x8000;
		tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
	}
	_W_TREEVIEW(tree)->ignoreGetDisp = TRUE;
	HTREEITEM hNewItem = (HTREEITEM) SendMessageW(_W_WIDGET(tree)->handle,
	TVM_INSERTITEMW, 0, (LPARAM) &tvInsert);
	_W_TREEVIEW(tree)->ignoreGetDisp = FALSE;
	if (hNewItem == 0)
		return W_ERROR_ITEM_NOT_ADDED;
	if (subitem != 0) {
		W_WIDGETDATA(subitem)->clazz = W_WIDGETDATA(item)->clazz;
		_W_ITEM(subitem)->parent = tree;
		_W_ITEM(subitem)->index = -1;
		_W_TREEITEM(subitem)->htreeitem = hNewItem;
	}
	return W_TRUE;
}
wresult _w_treeitem_clear(w_treeitem *item, int index, int all) {
	return W_FALSE;
}
wresult _w_treeitem_clear_all(w_treeitem *item, int all) {
	return W_FALSE;
}
void _w_treeitem_get_bounds_1(w_treeitem *item, _w_treeview_draw *info,
		RECT *rect, HDC hDC, int index, int flags) {
	memset(rect, 0, sizeof(RECT));
	if (!(flags & TREEVIEW_BOUNDS_GET_TEXT)
			&& !(flags & TREEVIEW_BOUNDS_GET_IMAGE))
		return;
	w_widget *tree = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(tree)->handle;
	/*if ((parent.style & W_VIRTUAL) == 0 && !cached && !parent.painted) {
	 TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_TEXT;
	 tvItem.hItem = handle;
	 tvItem.pszText = OS.LPSTR_TEXTCALLBACK;
	 parent.ignoreCustomDraw = true;
	 OS.SendMessage (hwnd, OS.TVM_SETITEM, 0, tvItem);
	 parent.ignoreCustomDraw = false;
	 }*/
	wresult firstColumn = index == 0;
	int columnCount = 0;
	HWND hwndHeader = _W_TREEVIEW(tree)->hwndHeader;
	if (hwndHeader != 0) {
		columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
		firstColumn = index == SendMessageW(hwndHeader, HDM_ORDERTOINDEX, 0, 0);
	}
	if (firstColumn) {
		wresult full = columnCount == 0 && (flags & TREEVIEW_BOUNDS_GET_TEXT)
				&& (flags & TREEVIEW_BOUNDS_GET_IMAGE)
				&& (flags & TREEVIEW_BOUNDS_FULLTEXT)
				&& (flags & TREEVIEW_BOUNDS_FULLIMAGE);
		*((HTREEITEM*) rect) = _W_TREEITEM(item)->htreeitem;
		if (!(WINBOOL) SendMessageW(handle, TVM_GETITEMRECT, !full,
				(LPARAM) rect)) {
			memset(rect, 0, sizeof(RECT));
			return;
		}
		if ((flags & TREEVIEW_BOUNDS_GET_IMAGE)
				&& !(flags & TREEVIEW_BOUNDS_FULLIMAGE)) {
			HIMAGELIST imagelist = (HIMAGELIST) SendMessageW(handle,
			TVM_GETIMAGELIST, TVSIL_NORMAL, 0);
			if (imagelist != 0) {
				int cx, cy;
				ImageList_GetIconSize(imagelist, &cx, &cy);
				rect->left -= cx + TREEVIEW_INSET;
				if (!(flags & TREEVIEW_BOUNDS_GET_TEXT))
					rect->right = rect->left + cx;
			} else {
				if (!(flags & TREEVIEW_BOUNDS_GET_TEXT))
					rect->right = rect->left;
			}
		}
		if ((flags & TREEVIEW_BOUNDS_FULLTEXT)
				|| (flags & TREEVIEW_BOUNDS_FULLIMAGE)
				|| (flags & TREEVIEW_BOUNDS_CLIP)) {
			if (hwndHeader != 0) {
				RECT headerRect;
				if (columnCount != 0) {
					if (SendMessageW(hwndHeader, HDM_GETITEMRECT, index,
							(LPARAM) &headerRect) == 0) {
						return;
					}
				} else {
					headerRect.right = _W_TREEVIEW(tree)->scrollWidth;
					if (headerRect.right == 0) {
						memcpy(&headerRect, rect, sizeof(RECT));
					}
				}
				if ((flags & TREEVIEW_BOUNDS_FULLTEXT)
						&& (flags & TREEVIEW_BOUNDS_CLIP))
					rect->right = headerRect.right;
				if ((flags & TREEVIEW_BOUNDS_FULLIMAGE))
					rect->left = headerRect.left;
				if ((flags & TREEVIEW_BOUNDS_CLIP)
						&& headerRect.right < rect->right) {
					rect->right = headerRect.right;
				}
			}
		}
	} else {
		if (!(0 <= index && index < columnCount))
			return;
		RECT headerRect;
		if (SendMessageW(hwndHeader, HDM_GETITEMRECT, index,
				(LPARAM) &headerRect) == 0) {
			return;
		}
		*((HTREEITEM*) rect) = _W_TREEITEM(item)->htreeitem;
		if (!SendMessageW(handle, TVM_GETITEMRECT, FALSE, (LPARAM) rect)) {
			return;
		}
		rect->left = headerRect.left;
		if ((flags & TREEVIEW_BOUNDS_FULLTEXT)
				&& (flags & TREEVIEW_BOUNDS_GET_IMAGE)
				&& (flags & TREEVIEW_BOUNDS_CLIP)) {
			rect->right = headerRect.right;
		} else {
			rect->right = headerRect.left;
			/*Image image = null;
			 if (index == 0) {
			 image = this.image;
			 } else {
			 if (images != null) image = images [index];
			 }
			 if (image != null) {
			 Point size = parent.getImageSize ();
			 rect.right += size.x;
			 }*/
			if ((flags & TREEVIEW_BOUNDS_GET_TEXT)) {
				if ((flags & TREEVIEW_BOUNDS_FULLTEXT)
						&& (flags & TREEVIEW_BOUNDS_CLIP)) {
					rect->left = rect->right + TREEVIEW_INSET;
					rect->right = headerRect.right;
				} else {
					const char *string = 0; /*index == 0 ? text : strings != null ? strings [index] : null;*/
					if (string != 0) {
						RECT textRect;
						WCHAR buffer = 0;/* new TCHAR (parent.getCodePage (), string, false);*/
						int length;
						int flags = DT_NOPREFIX | DT_SINGLELINE | DT_CALCRECT;
						HDC hNewDC = hDC;
						HFONT hFont = 0;
						if (hDC == 0) {
							hNewDC = GetDC(handle);
							/*hFont = fontHandle(index);
							 if (hFont == -1)
							 hFont = SendMessage(hwnd, WM_GETFONT, 0, 0);
							 hFont = SelectObject(hNewDC, hFont);*/
						}
						//DrawTextW(hNewDC, buffer, length, &textRect, flags);
						if (hDC == 0) {
							SelectObject(hNewDC, hFont);
							ReleaseDC(handle, hNewDC);
						}
						if ((flags & TREEVIEW_BOUNDS_GET_IMAGE)) {
							rect->right += textRect.right - textRect.left
									+ TREEVIEW_INSET * 3;
						} else {
							rect->left = rect->right + TREEVIEW_INSET;
							rect->right = rect->left
									+ (textRect.right - textRect.left)
									+ TREEVIEW_INSET;
						}
					}
				}
			}
			if ((flags & TREEVIEW_BOUNDS_CLIP)
					&& headerRect.right < rect->right) {
				rect->right = headerRect.right;
			}
		}
	}
	int gridWidth =
	_W_TREEVIEW(tree)->linesVisible && columnCount != 0 ?
	TREEVIEW_GRID_WIDTH :
															0;
	if ((flags & TREEVIEW_BOUNDS_GET_TEXT)
			|| !(flags & TREEVIEW_BOUNDS_GET_IMAGE)) {
		rect->right = WMAX(rect->left, rect->right - gridWidth);
	}
	rect->bottom = WMAX(rect->top, rect->bottom - gridWidth);
	return;
}
void _w_treeitem_get_bounds_0(w_treeitem *item, RECT *rect, HDC hDC, int index,
		int flags) {
	_w_treeview_draw info;
	w_widget *tree = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(tree)->handle;
	info.hwndHeader = _W_TREEVIEW(tree)->hwndHeader;
	if (info.hwndHeader != 0) {
		info.columnCount = SendMessageW(info.hwndHeader, HDM_GETITEMCOUNT, 0,
				0);
		info.firstColumn = SendMessageW(info.hwndHeader, HDM_ORDERTOINDEX, 0,
				0);
		int getheaderRect = TRUE;
		if (info.firstColumn == index) {
			if ((flags & TREEVIEW_BOUNDS_FULLTEXT)
					|| (flags & TREEVIEW_BOUNDS_FULLIMAGE)
					|| (flags & TREEVIEW_BOUNDS_CLIP)) {
				getheaderRect = TRUE;
			} else
				getheaderRect = FALSE;
		}
		if (getheaderRect) {
			if (SendMessageW(info.hwndHeader, HDM_GETITEMRECT, index,
					(LPARAM) &info.headerRect) == 0) {
				return;
			}
		}
	} else {
		info.columnCount = 1;
		info.firstColumn = 0;
	}
	wresult full;
	RECT *r;
	if (index == info.firstColumn) {
		full = info.columnCount == 0 && (flags & TREEVIEW_BOUNDS_GET_TEXT)
				&& (flags & TREEVIEW_BOUNDS_GET_IMAGE)
				&& (flags & TREEVIEW_BOUNDS_FULLTEXT)
				&& (flags & TREEVIEW_BOUNDS_FULLIMAGE);
	} else {
		full = FALSE;
	}
	if (full)
		r = &info.itemRect;
	else
		r = &info.textRect;
	*((HTREEITEM*) r) = _W_TREEITEM(item)->htreeitem;
	if (!(WINBOOL) SendMessageW(handle, TVM_GETITEMRECT, !full, (LPARAM) r)) {
		memset(rect, 0, sizeof(RECT));
		return;
	}
	_w_treeitem_get_bounds_1(item, &info, rect, hDC, index, flags);
}
wresult _w_treeitem_get_expanded(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem,
		int flags) {
	return W_FALSE;
}
wresult _w_treeitem_get_item_count(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_items(w_treeitem *item, w_iterator *items) {
	return W_FALSE;
}
wresult _w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		int index, w_treeitem *after, int flags) {
	w_widget *tree = _W_ITEM(item)->parent;
	HTREEITEM i = _w_treeview_find_previous(_W_WIDGET(tree)->handle,
	_W_TREEITEM(item)->htreeitem, index);
	if (i == 0)
		return W_ERROR_INVALID_RANGE;
	return _w_treeitem_insert_item_0(item, subitem, i);
}
wresult _w_treeitem_remove_all(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_set_expanded(w_treeitem *item, int expanded) {
	return W_FALSE;
}
wresult _w_treeitem_set_has_children(w_treeitem *item) {
	w_widget *tree = _W_ITEM(item)->parent;
	TVITEMW tvItem;
	tvItem.mask = TVIF_CHILDREN;
	tvItem.cChildren = 1;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	HRESULT result = SendMessageW(_W_WIDGET(tree)->handle, TVM_SETITEMW, 0,
			(LPARAM) &tvItem);
	if (result == 0)
		return W_FALSE;
	else
		return W_TRUE;
}
wresult _w_treeitem_set_item_count(w_treeitem *item, int count) {
	return W_FALSE;
}
/*
 * treeview
 */
wresult _w_treeview_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result = _w_composite_create_handle(control, priv);
	if (result <= 0)
		return result;
	_W_WIDGET(control)->state &= ~(STATE_CANVAS | STATE_THEME_BACKGROUND);
	HWND handle = _W_WIDGET(control)->handle;

	DWORD bits = 0;

	/* Use the Explorer theme */
	if (win_toolkit->EXPLORER_THEME) {
		if (WIN32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
			_W_LISTVIEWBASE(control)->explorerTheme = TRUE;
			SetWindowTheme(handle, L"EXPLORER", NULL);
			DWORD bits = TVS_EX_DOUBLEBUFFER | TVS_EX_RICHTOOLTIP;
			if (win_toolkit->ENABLE_TVS_EX_FADEINOUTEXPANDOS)
				bits |= TVS_EX_FADEINOUTEXPANDOS;
			/*
			 * This code is intentionally commented.
			 */
//			if ((style & W_FULL_SELECTION) == 0) bits |= TVS_EX_AUTOHSCROLL;
			/*
			 * Bug in Windows.  When the tree is using the explorer
			 * theme, it does not use COLOR_WINDOW_TEXT for the
			 * default foreground color.  The fix is to explicitly
			 * set the foreground.
			 */
			//setForegroundPixel (-1);
		}
	}

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

	/* Set the checkbox image list */
	if ((_W_WIDGET(control)->style & W_CHECK) != 0) {
		_w_treeview_set_checkbox_imagelist(W_TREEVIEW(control), priv);
	}
	if (bits != 0) {
		SendMessageW(handle, TVM_SETEXTENDEDSTYLE, 0, bits);
	}
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
	HFONT hFont = GetStockObject(SYSTEM_FONT);
	//SendMessageW(handle, WM_SETFONT, (WPARAM) hFont, 0);

	_W_LISTVIEWBASE(control)->createdAsRTL = (_W_WIDGET(control)->style
			& W_RIGHT_TO_LEFT) != 0;
	return result;
}
wresult _w_treeview_check_buffered(w_control *control, _w_control_priv *priv) {
	wresult result = _w_composite_check_buffered(control, priv);
	//if ((_W_WIDGET(control)->style & W_VIRTUAL) != 0) {
	_W_WIDGET(control)->style |= W_DOUBLE_BUFFERED;
	SendMessageW(_W_WIDGET(control)->handle, TVM_SETSCROLLTIME, 0, 0);
	//}
	if (win_toolkit->EXPLORER_THEME) {
		if (win_toolkit->IsAppThemed) {
			DWORD exStyle = SendMessageW(_W_WIDGET(control)->handle,
			TVM_GETEXTENDEDSTYLE, 0, 0);
			if ((exStyle & TVS_EX_DOUBLEBUFFER) != 0)
				_W_WIDGET(control)->style |= W_DOUBLE_BUFFERED;
		}
	}
	return result;
}
DWORD _w_treeview_widget_style(w_control *control, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	DWORD bits = _w_composite_widget_style(control, priv);
	bits |=
	TVS_SHOWSELALWAYS | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_NONEVENHEIGHT;
	if (win_toolkit->EXPLORER_THEME && WIN32_VERSION >= VERSION(6, 0)
			&& IsAppThemed()) {
		bits |= TVS_TRACKSELECT;
		if ((style & W_FULL_SELECTION) != 0)
			bits |= TVS_FULLROWSELECT;
	} else {
		if ((style & W_FULL_SELECTION) != 0) {
			bits |= TVS_FULLROWSELECT;
		} else {
			bits |= TVS_HASLINES;
		}
	}
	if ((style & (W_HSCROLL | W_VSCROLL)) == 0) {
		bits &= ~(WS_HSCROLL | WS_VSCROLL);
		bits |= TVS_NOSCROLL;
	} else {
		if ((style & W_HSCROLL) == 0) {
			bits &= ~WS_HSCROLL;
			bits |= TVS_NOHSCROLL;
		}
	}
//	bits |= TVS_NOTOOLTIPS | TVS_DISABLEDRAGDROP;
	return bits | TVS_DISABLEDRAGDROP;
}
WCHAR* _w_treeview_window_class(w_control *control, _w_control_priv *priv) {
	return WC_TREEVIEWW;
}
wresult _w_treeview_clear(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_create_parent(w_treeview *tree, _w_control_priv *priv) {
	//priv->force_resize(W_CONTROL(tree),priv);
	HWND handle = _W_WIDGET(tree)->handle;
	HWND parent = GetParent(handle);
	RECT rect;
	GetWindowRect(handle, &rect);
	MapWindowPoints(0, parent, (LPPOINT) &rect, 2);
	DWORD dwExStyle = priv->widget_extstyle(W_CONTROL(tree), priv);
	DWORD dwStyle = priv->widget_style(W_CONTROL(tree), priv);
	DWORD oldStyle = GetWindowLongW(handle, GWL_STYLE);
	DWORD newStyle = dwStyle & ~WS_VISIBLE;
	if ((oldStyle & WS_DISABLED) != 0)
		newStyle |= WS_DISABLED;
	if ((oldStyle & WS_VISIBLE) != 0)
		newStyle |= WS_VISIBLE;
	HWND hwndParent = CreateWindowExW(dwExStyle, WindowClass,
	NULL, newStyle, rect.left, rect.top, rect.right - rect.left,
			rect.bottom - rect.top, parent, 0, hinst,
			NULL);
	if (hwndParent == 0) {
		return W_ERROR_NO_HANDLES;
	}
	_W_TREEVIEW(tree)->hwndParent = hwndParent;
	SetWindowLongPtrW(hwndParent, GWLP_USERDATA, (LONG_PTR) tree);
	int bits = 0;
	if (WIN32_VERSION >= VERSION(4, 10)) {
		bits |= WS_EX_NOINHERITLAYOUT;
		if ((_W_WIDGET(tree)->style & W_RIGHT_TO_LEFT) != 0)
			bits |= WS_EX_LAYOUTRTL;
	}
	HWND hwndHeader = CreateWindowExW(bits, WC_HEADERW, NULL,
			HDS_BUTTONS | HDS_FULLDRAG | HDS_DRAGDROP | HDS_HIDDEN | WS_CHILD
					| WS_CLIPSIBLINGS, 0, 0, 0, 0, hwndParent, 0, hinst, NULL);
	if (hwndHeader == 0) {
		return W_ERROR_NO_HANDLES;
	}
	_W_TREEVIEW(tree)->hwndHeader = hwndHeader;
	if (_W_TREEVIEW_PRIV(priv)->HeaderProc == 0) {
		_W_TREEVIEW_PRIV(priv)->HeaderProc = (WNDPROC) GetWindowLongPtrW(
				hwndHeader,
				GWLP_WNDPROC);
	}
	/*SetWindowLongPtrW(hwndHeader, GWLP_WNDPROC,
	 (LONG_PTR) _w_control_window_proc);*/

	SetWindowLongPtrW(hwndHeader, GWLP_USERDATA, (LONG_PTR) tree);
	//if (IsDBLocale) {
	HIMC hIMC = ImmGetContext(handle);
	ImmAssociateContext(hwndParent, hIMC);
	ImmAssociateContext(hwndHeader, hIMC);
	ImmReleaseContext(handle, hIMC);
	//}
//This code is intentionally commented
//	if (!IsPPC) {
//		if ((style & W_BORDER) != 0) {
//			int oldExStyle = GetWindowLong (handle, GWL_EXSTYLE);
//			oldExStyle &= ~WS_EX_CLIENTEDGE;
//			SetWindowLong (handle, GWL_EXSTYLE, oldExStyle);
//		}
//	}
	_w_listviewbase_insert_column(W_LISTVIEWBASE(tree), 0, 0);
	HFONT hFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
	if (hFont != 0)
		SendMessageW(hwndHeader, WM_SETFONT, (WPARAM) hFont, 0);
	HWND hwndInsertAfter = GetWindow(handle, GW_HWNDPREV);
	int flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
	SetWindowPos(hwndParent, hwndInsertAfter, 0, 0, 0, 0, flags);
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_RANGE | SIF_PAGE;
	GetScrollInfo(hwndParent, SB_HORZ, &info);
	info.nPage = info.nMax + 1;
	SetScrollInfo(hwndParent, SB_HORZ, &info, TRUE);
	GetScrollInfo(hwndParent, SB_VERT, &info);
	info.nPage = info.nMax + 1;
	SetScrollInfo(hwndParent, SB_VERT, &info, TRUE);
	//customDraw = true;
	if ((oldStyle & WS_VISIBLE) != 0) {
		ShowWindow(hwndParent, SW_SHOW);
	}
	HWND hwndFocus = GetFocus();
	if (hwndFocus == handle)
		SetFocus(hwndParent);
	SetParent(handle, hwndParent);
	if (hwndFocus == handle)
		SetFocus(handle);
	return W_TRUE;
}
int _w_treeview_default_background(w_control *control, _w_control_priv *priv) {
	return GetSysColor(COLOR_WINDOW);
}
int _w_treeview_find_cell(w_treeview *tree, int x, int y, w_listitem *item,
		int *index, RECT *cellRect, RECT *itemRect) {
	return W_FALSE;
}
HTREEITEM _w_treeview_get_bottom_item(HWND handle) {
	HTREEITEM hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
	TVGN_FIRSTVISIBLE, 0);
	if (hItem == 0)
		return 0;
	int index = 0;
	int count = SendMessageW(handle, TVM_GETVISIBLECOUNT, 0, 0);
	while (index <= count) {
		HTREEITEM hNextItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
		TVGN_NEXTVISIBLE, (LPARAM) hItem);
		if (hNextItem == 0)
			return hItem;
		hItem = hNextItem;
		index++;
	}
	return hItem;
}
wresult _w_treeview_deselect(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
int _w_treeview_get_header_width(w_treeview *tree) {
	HWND hwndHeader = _W_TREEVIEW(tree)->hwndHeader;
	int total = 0;
	HDITEM hditem;
	hditem.mask = HDI_WIDTH;
	int hcnt = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, (WPARAM) 0,
			(LPARAM) 0);
	for (int i = 0; i < hcnt; i++) {
		if (SendMessageW(hwndHeader, HDM_GETITEMW, i, (LPARAM) &hditem) == TRUE) {
			total += hditem.cxy;
		}
	}
	return total;
}
wresult _w_treeview_get_item_from_point(w_treeview *tree, w_point *point,
		w_treeitem *item) {
	HWND handle = _W_WIDGET(tree)->handle;
	TVHITTESTINFO lpht;
	lpht.hItem = 0;
	lpht.pt.x = point->x;
	lpht.pt.y = point->y;
	SendMessageW(handle, TVM_HITTEST, 0, (LPARAM) &lpht);
	if (lpht.hItem != 0) {
		int flags = TVHT_ONITEM;
		if ((_W_WIDGET(tree)->style & W_FULL_SELECTION) != 0) {
			flags |= TVHT_ONITEMRIGHT | TVHT_ONITEMINDENT;
		} else {
			if (_W_WIDGET(tree)->style & W_CUSTOMDRAW) {
				lpht.flags &= ~(TVHT_ONITEMICON | TVHT_ONITEMLABEL);
				/*if (hitTestSelection (lpht.hItem, lpht.pt.x, lpht.pt.y)) {
				 lpht.flags |= TVHT_ONITEMICON | TVHT_ONITEMLABEL;
				 }*/
			}
		}
		if ((lpht.flags & flags) != 0 && lpht.hItem != 0) {
			W_WIDGETDATA(item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(tree);
			_W_ITEM(item)->parent = W_WIDGET(tree);
			_W_ITEM(item)->index = -1;
			_W_TREEITEM(item)->htreeitem = lpht.hItem;
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_treeview_get_root_item(w_treeview *tree, w_treeitem *root) {
	W_WIDGETDATA(root)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(tree);
	_W_ITEM(root)->parent = W_WIDGET(tree);
	_W_ITEM(root)->index = -1;
	_W_TREEITEM(root)->htreeitem = TVI_ROOT;
	return W_TRUE;
}
wresult _w_treeview_get_top_item(w_treeview *tree, w_treeitem *topitem) {
	HWND handle = _W_WIDGET(tree)->handle;
	HTREEITEM hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
	TVGN_FIRSTVISIBLE, 0);
	if (hItem != 0) {
		W_WIDGETDATA(topitem)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(tree);
		_W_ITEM(topitem)->parent = W_WIDGET(tree);
		_W_ITEM(topitem)->index = -1;
		_W_TREEITEM(topitem)->htreeitem = hItem;
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_treeview_remove(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_set_bounds(w_control *control, w_point *location,
		w_size *size) {
	wresult result = _w_control_set_bounds(control, location, size);
	_w_treeview_set_scroll_width(W_TREEVIEW(control));
	return result;
}
wuchar checkbox_imagelist_states_0[] = { CBS_UNCHECKEDNORMAL, CBS_CHECKEDNORMAL,
		CBS_UNCHECKEDNORMAL, CBS_MIXEDNORMAL };
wushort checkbox_imagelist_states_1[] = { DFCS_FLAT, DFCS_CHECKED | DFCS_FLAT,
DFCS_INACTIVE | DFCS_FLAT, DFCS_CHECKED | DFCS_INACTIVE | DFCS_FLAT };
void _w_treeview_set_checkbox_imagelist(w_treeview *tree,
		_w_control_priv *priv) {
	if ((_W_WIDGET(tree)->style & W_CHECK) == 0)
		return;
	HWND handle = _W_WIDGET(tree)->handle;
	int count = 16;
	/*if ((_W_WIDGET(tree)->style & W_RIGHT_TO_LEFT) != 0)
	 flags |= ILC_MIRROR;*/
	w_size size;
	size.height = SendMessageW(handle, TVM_GETITEMHEIGHT, 0, 0);
	size.width = size.height;
	w_imagelist imagelist;
	w_imagelist_init(&imagelist);
	w_imagelist_create(&imagelist, &size, count);
	HIMAGELIST hStateList = _W_IMAGELIST(&imagelist)->imagelist;
	HDC memDC = CreateCompatibleDC(NULL);
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth = size.width * 4;
	bmi.bmiHeader.biHeight = -size.height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	VOID *pBits;
	HBITMAP hBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pBits, NULL,
			0);
	if (hBitmap != 0) {
		HBITMAP hOldBitmap = SelectObject(memDC, hBitmap);
		RECT rect;
		rect.left = 0;
		rect.right = size.width;
		rect.top = 0;
		rect.bottom = size.height;
		if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
			HTHEME hTheme = OpenThemeData(NULL, L"BUTTON");
			for (int i = 0; i < 4; i++) {
				DrawThemeBackground(hTheme, memDC, BP_CHECKBOX,
						checkbox_imagelist_states_0[i % 4], &rect, NULL);
				rect.left += size.width;
				rect.right += size.width;
			}
			CloseThemeData(hTheme);
		} else {
			for (int i = 0; i < 4; i++) {
				DrawFrameControl(memDC, &rect, DFC_BUTTON,
				DFCS_BUTTONCHECK | checkbox_imagelist_states_1[i % 4]);
				rect.left += size.width;
				rect.right += size.width;
			}
		}
		SelectObject(memDC, hOldBitmap);
		int *colors = pBits;
		int length = size.width * 4 * size.height;
		for (int i = 0; i < length; i++) {
			if (colors[i] == 0xFF000000) {
				colors[i] = 0;
			}
		}
	}
	DeleteDC(memDC);
	for (int i = 0; i < 4; i++) {
		ImageList_Add(hStateList, hBitmap, 0);
	}
	DeleteObject(hBitmap);
	HIMAGELIST hOldStateList = (HIMAGELIST) SendMessageW(handle,
	TVM_GETIMAGELIST, TVSIL_STATE, 0);
	SendMessageW(handle, TVM_SETIMAGELIST, TVSIL_STATE, (LPARAM) hStateList);
	if (hOldStateList != 0)
		ImageList_Destroy(hOldStateList);
}
wresult _w_treeview_set_insert_mark(w_treeview *tree, w_treeitem *item,
		int before) {
	return W_FALSE;
}
void _w_treeview_set_scroll_width_0(w_treeview *tree, int width) {
	HWND hwndHeader = _W_TREEVIEW(tree)->hwndHeader;
	HWND hwndParent = _W_TREEVIEW(tree)->hwndParent;
	if (hwndHeader == 0 || hwndParent == 0)
		return;
	HWND handle = _W_WIDGET(tree)->handle;
	//TEMPORARY CODE
	_W_TREEVIEW(tree)->scrollWidth = width;
	int left = 0;
	RECT rect;
	SCROLLINFO info, _info;
	WINDOWPOS pos;
	info.cbSize = sizeof(info);
	info.fMask = SIF_RANGE | SIF_PAGE;
	int columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
	if (columnCount == 0 && width == 0) {
		GetScrollInfo(hwndParent, SB_HORZ, &info);
		info.nPage = info.nMax + 1;
		SetScrollInfo(hwndParent, SB_HORZ, &info, TRUE);
		GetScrollInfo(hwndParent, SB_VERT, &info);
		info.nPage = info.nMax + 1;
		SetScrollInfo(hwndParent, SB_VERT, &info, TRUE);
	} else {
		if ((_W_WIDGET(tree)->style & W_HSCROLL) != 0) {
			GetClientRect(hwndParent, &rect);
			GetScrollInfo(hwndParent, SB_HORZ, &info);
			info.nMax = width;
			info.nPage = rect.right - rect.left + 1;
			SetScrollInfo(hwndParent, SB_HORZ, &info, TRUE);
			info.fMask = SIF_POS;
			GetScrollInfo(hwndParent, SB_HORZ, &info);
			left = info.nPos;
		}
	}
	if ((_W_WIDGET(tree)->style & W_HSCROLL) != 0) {
		_info.cbSize = sizeof(_info);
		_info.fMask = SIF_RANGE | SIF_PAGE;
		GetScrollInfo(handle, SB_HORZ, &_info);
		_info.nPos += TREEVIEW_INCREMENT;
		_info.nPage = info.nPage;
		SetScrollInfo(handle, SB_HORZ, &_info, TRUE);
	}
	GetClientRect(hwndParent, &rect);
	HDLAYOUT playout;
	playout.prc = &rect;
	playout.pwpos = &pos;
	SendMessageW(hwndHeader, HDM_LAYOUT, 0, (LPARAM) &playout);
	int bits = GetWindowLongW(_W_WIDGET(tree)->handle, GWL_EXSTYLE);
	int b = (bits & WS_EX_CLIENTEDGE) != 0 ? GetSystemMetrics(SM_CXEDGE) : 0;
	int w = pos.cx
			+ (columnCount == 0 && width == 0 ?
					0 : GetSystemMetrics(SM_CXVSCROLL));
	int h = rect.bottom - rect.top - pos.y;
	w_imagelist *headerimagelist = _W_LISTVIEWBASE(tree)->headerimagelist;
	int cy = pos.cy;
	if (headerimagelist != 0) {
		w_size sz;
		w_imagelist_get_size(headerimagelist, &sz);
		cy = WMAX(pos.cy, sz.height + 2);
	}
	SetWindowPos(hwndHeader, HWND_TOP, pos.x - left, pos.y,/*pos.cx*/w + left,
			cy, SWP_NOACTIVATE);
	int oldIgnore = _W_TREEVIEW(tree)->ignoreResize;
	_W_TREEVIEW(tree)->ignoreResize = TRUE;
	SetWindowPos(handle, 0, pos.x - left - b, pos.y + cy - b, w + left + b * 2,
			h + b * 2, SWP_NOACTIVATE | SWP_NOZORDER);
	_W_TREEVIEW(tree)->ignoreResize = oldIgnore;
}
void _w_treeview_set_scroll_width(w_treeview *tree) {
	if (_W_TREEVIEW(tree)->hwndHeader == 0
			|| _W_TREEVIEW(tree)->hwndParent == 0)
		return;
	_w_treeview_set_scroll_width_0(tree, _w_treeview_get_header_width(tree));
}
wresult _w_treeview_set_selection(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_set_top_item(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_select(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_show_item(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
HWND _w_treeview_top_handle(w_control *control) {
	HWND hwndParent = _W_TREEVIEW(control)->hwndParent;
	if (hwndParent != 0)
		return hwndParent;
	else
		return _W_WIDGET(control)->handle;
}
void _w_treeview_update_scrollbar(w_treeview *tree) {
	HWND hwndHeader = _W_TREEVIEW(tree)->hwndHeader;
	HWND hwndParent = _W_TREEVIEW(tree)->hwndParent;
	if (hwndParent != 0) {
		int columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
		if (columnCount != 0) {
			SCROLLINFO info;
			info.cbSize = sizeof(info);
			info.fMask = SIF_ALL;
			int itemCount = SendMessageW(_W_WIDGET(tree)->handle, TVM_GETCOUNT,
					0, 0);
			if (itemCount == 0) {
				GetScrollInfo(hwndParent, SB_VERT, &info);
				info.nPage = info.nMax + 1;
				SetScrollInfo(hwndParent, SB_VERT, &info, TRUE);
			} else {
				GetScrollInfo(_W_WIDGET(tree)->handle, SB_VERT, &info);
				if (WIN32_VERSION >= VERSION(4, 10)) {
					if (info.nPage == 0) {
						SCROLLBARINFO psbi;
						psbi.cbSize = sizeof(psbi);
						GetScrollBarInfo(_W_WIDGET(tree)->handle,
						OBJID_VSCROLL, &psbi);
						if ((psbi.rgstate[0] & STATE_SYSTEM_INVISIBLE) != 0) {
							info.nPage = info.nMax + 1;
						}
					}
				}
				SetScrollInfo(hwndParent, SB_VERT, &info, TRUE);
			}
		}
	}
}
void _w_treeview_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_treeview_class *clazz) {
	if (classId == _W_CLASS_TREEVIEW) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_treeview_priv;
	}
	_w_listviewbase_class_init(toolkit, classId, W_LISTVIEWBASE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TREEVIEW;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_treeview_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_treeview);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_treeview);
	/*
	 * functions
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_treeview_post_event;
	W_CONTROL_CLASS(clazz)->set_bounds = _w_treeview_set_bounds;
	clazz->clear = _w_treeview_clear;
	clazz->deselect = _w_treeview_deselect;
	clazz->get_item_from_point = _w_treeview_get_item_from_point;
	clazz->get_root_item = _w_treeview_get_root_item;
	clazz->get_top_item = _w_treeview_get_top_item;
	clazz->remove = _w_treeview_remove;
	clazz->set_insert_mark = _w_treeview_set_insert_mark;
	clazz->set_selection = _w_treeview_set_selection;
	clazz->set_top_item = _w_treeview_set_top_item;
	clazz->select = _w_treeview_select;
	clazz->show_item = _w_treeview_show_item;
	/*
	 * tree item
	 */
	struct _w_treeitem_class *treeitem = W_TREEITEM_CLASS(
			clazz->base.class_item);
	treeitem->clear = _w_treeitem_clear;
	treeitem->clear_all = _w_treeitem_clear_all;
	treeitem->get_expanded = _w_treeitem_get_expanded;
	treeitem->get_item = _w_treeitem_get_item;
	treeitem->get_item_count = _w_treeitem_get_item_count;
	treeitem->get_items = _w_treeitem_get_items;
	treeitem->insert_item = _w_treeitem_insert_item;
	treeitem->remove_all = _w_treeitem_remove_all;
	treeitem->set_expanded = _w_treeitem_set_expanded;
	treeitem->set_has_children = _w_treeitem_set_has_children;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_TREEVIEW) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->create_handle = _w_treeview_create_handle;
		priv->widget_style = _w_treeview_widget_style;
		priv->window_class = _w_treeview_window_class;
		priv->widget.call_window_proc = _w_treeview_call_window_proc;
		priv->handle_top = _w_treeview_top_handle;
		priv->check_buffered = _w_treeview_check_buffered;
		_W_CONTROL_PRIV(priv)->default_background =
				_w_treeview_default_background;
		win_toolkit->EXPLORER_THEME = TRUE;
		/*
		 * messages
		 */
		dispatch_message *msg = priv->messages;
		msg[_WM_CHAR] = _TREEVIEW_WM_CHAR;
		msg[_WM_ERASEBKGND] = _TREEVIEW_WM_ERASEBKGND;
		msg[_WM_GETOBJECT] = _TREEVIEW_WM_GETOBJECT;
		msg[_WM_HSCROLL] = _TREEVIEW_WM_HSCROLL;
		msg[_WM_KEYDOWN] = _TREEVIEW_WM_KEYDOWN;
		msg[_WM_KILLFOCUS] = _TREEVIEW_WM_KILLFOCUS;
		msg[_WM_LBUTTONDBLCLK] = _TREEVIEW_WM_LBUTTONDBLCLK;
		msg[_WM_LBUTTONDOWN] = _TREEVIEW_WM_LBUTTONDOWN;
		msg[_WM_MOUSEMOVE] = _TREEVIEW_WM_MOUSEMOVE;
		msg[_WM_MOUSEWHEEL] = _TREEVIEW_WM_MOUSEWHEEL;
		msg[_WM_MOVE] = _TREEVIEW_WM_MOVE;
		msg[_WM_RBUTTONDOWN] = _TREEVIEW_WM_RBUTTONDOWN;
		msg[_WM_PAINT] = _TREEVIEW_WM_PAINT;
		msg[_WM_SETCURSOR] = _TREEVIEW_WM_SETCURSOR;
		msg[_WM_SETFOCUS] = _TREEVIEW_WM_SETFOCUS;
		msg[_WM_SETFONT] = _TREEVIEW_WM_SETFONT;
		msg[_WM_SETREDRAW] = _TREEVIEW_WM_SETREDRAW;
		msg[_WM_SIZE] = _TREEVIEW_WM_SIZE;
		msg[_WM_SYSCOLORCHANGE] = _TREEVIEW_WM_SYSCOLORCHANGE;
		msg[_WM_VSCROLL] = _TREEVIEW_WM_VSCROLL;
		msg[_WM_TIMER] = _TREEVIEW_WM_TIMER;
		msg[_WM_CTLCOLORCHILD] = _TREEVIEW_WM_CTLCOLORCHILD;
		msg[_WM_NOTIFY] = _TREEVIEW_WM_NOTIFY;
		msg[_WM_NOTIFYCHILD] = _TREEVIEW_WM_NOTIFYCHILD;
	}
}
