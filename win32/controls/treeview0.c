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
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.iSelectedImage = I_IMAGECALLBACK;
	tvInsert.item.iImage = I_IMAGECALLBACK;
	tvInsert.hInsertAfter = after;
	tvInsert.item.pszText = LPSTR_TEXTCALLBACKW;
	if ((_W_WIDGET(tree)->style & W_CHECK) != 0) {
		tvInsert.item.mask = tvInsert.item.mask | TVIF_STATE;
		tvInsert.item.state = 1 << 12;
		tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
	}
	_W_TREEVIEW(tree)->ignoreGetDisp = TRUE;
	HTREEITEM hNewItem = (HTREEITEM) SendMessageW(_W_WIDGET(tree)->handle,
	TVM_INSERTITEMW, 0, (LPARAM) &tvInsert);
	_W_TREEVIEW(tree)->ignoreGetDisp = FALSE;
	if (hNewItem == 0)
		return W_ERROR_ITEM_NOT_ADDED;
	if (subitem != 0) {
		_W_WIDGETDATA(subitem)->clazz = _W_WIDGETDATA(item)->clazz;
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
void _w_treeitem_get_bounds_0(w_treeitem *item, RECT *rect, HDC hDC, int index,
		int flags) {
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
		columnCount = SendMessageW(hwndHeader,
		HDM_GETITEMCOUNT, 0, 0);
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
wresult _w_treeitem_get_bounds(w_listitem *item, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_treeitem_get_checked(w_listitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_expanded(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_grayed(w_listitem *item) {
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
wresult _w_treeitem_get_image(w_listitem *item) {
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
wresult _w_treeitem_set_checked(w_listitem *item, int checked) {
	return W_FALSE;
}
wresult _w_treeitem_set_expanded(w_treeitem *item, int expanded) {
	return W_FALSE;
}
wresult _w_treeitem_set_grayed(w_listitem *item, int grayed) {
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
wresult _w_treeitem_set_image(w_listitem *item, int image) {
	TVITEMW tvItem;
	tvItem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvItem.iImage = image;
	tvItem.iSelectedImage = image;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	HRESULT result = SendMessageW(_W_WIDGET(_W_ITEM(item)->parent)->handle,
	TVM_SETITEMW, 0, (LPARAM) &tvItem);
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
const char* _w_treeview_window_class(w_control *control,
		_w_control_priv *priv) {
	return WC_TREEVIEWA;
}
wresult _w_treeview_clear(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_create_parent(w_treeview *tree, _w_control_priv *priv) {
	//priv->force_resize(W_CONTROL(tree),priv);
	HWND handle = _W_WIDGET(tree)->handle;
	HWND parent = GetParent(handle);
	RECT rect;
	WCHAR tmp[30];
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
	w_utf8_to_utf16(WindowClass, -1, tmp, 30);
	HWND hwndParent = CreateWindowExW(dwExStyle, tmp,
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
	w_utf8_to_utf16(WC_HEADERA, -1, tmp, 30);
	HWND hwndHeader = CreateWindowExW(bits, tmp, NULL,
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
	_w_treeview_insert_column(W_LISTVIEWBASE(tree), 0, 0);
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
HTREEITEM _w_treeview_get_bottom_item(HWND handle) {
	HTREEITEM hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
			TVGN_FIRSTVISIBLE, 0);
	if (hItem == 0)
		return 0;
	int index = 0;
	int count = SendMessageW(handle, TVM_GETVISIBLECOUNT, 0, 0);
	while (index <= count) {
		HTREEITEM hNextItem =(HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
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
HWND _w_treeview_get_header(w_listviewbase *list, int create,
		_w_control_priv *priv) {
	if (create && _W_TREEVIEW(list)->hwndHeader == 0) {
		_w_treeview_create_parent(W_TREEVIEW(list), _W_CONTROL_GET_PRIV(list));
	}
	return _W_TREEVIEW(list)->hwndHeader;
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
	return W_FALSE;
}
wresult _w_treeview_get_parent_item(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_get_root_item(w_treeview *tree, w_treeitem *root) {
	_W_WIDGETDATA(root)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(tree);
	_W_ITEM(root)->parent = W_WIDGET(tree);
	_W_ITEM(root)->index = -1;
	_W_TREEITEM(root)->htreeitem = TVI_ROOT;
	return W_TRUE;
}
wresult _w_treeview_get_top_item(w_treeview *tree, w_treeitem *topitem) {
	return W_FALSE;
}
wresult _w_treeview_insert_column(w_listviewbase *tree, w_columnitem *column,
		int index) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(tree);
	if (_W_TREEVIEW(tree)->hwndHeader == 0) {
		wresult result = _w_treeview_create_parent(W_TREEVIEW(tree), priv);
		if (result <= 0)
			return result;
	}
	HWND hwndHeader = _W_TREEVIEW(tree)->hwndHeader;
	HWND handle = _W_WIDGET(tree)->handle;
	int columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
	if (index < 0)
		index = columnCount;
	else if (index > columnCount)
		return W_ERROR_INVALID_RANGE;
	HDITEMW hdItem;
	hdItem.mask = HDI_TEXT | HDI_WIDTH;
	hdItem.pszText = L"";
	hdItem.cchTextMax = 0;
	hdItem.cxy = 50;
	index = SendMessageW(hwndHeader, HDM_INSERTITEMW, index, (LPARAM) &hdItem);
	if (index < 0)
		return W_ERROR_ITEM_NOT_ADDED;
	if (column != 0) {
		_W_WIDGETDATA(column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(tree);
		_W_ITEM(column)->parent = W_WIDGET(tree);
		_W_ITEM(column)->index = index;

	}
	/* When the first column is created, hide the horizontal scroll bar */
	if (columnCount == 0) {
//scrollWidth = 0;
		if ((_W_WIDGET(tree)->style & W_HSCROLL) != 0) {
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
		 OS.SendMessage (itemToolTipHandle, OS.TTM_SETDELAYTIME, OS.TTDT_AUTOMATIC, -1);
		 }*/
	}
	_w_treeview_set_scroll_width(W_TREEVIEW(tree));
	//updateImageList();
	_w_treeview_update_scrollbar(W_TREEVIEW(tree));

	/* Redraw to hide the items when the first column is created */
	if (columnCount == 0 && SendMessageW(handle, TVM_GETCOUNT, 0, 0) != 0) {
		InvalidateRect(handle, NULL, TRUE);
	}

	/* Add the tool tip item for the header */
	HWND headerToolTipHandle = _W_LISTVIEWBASE(tree)->headerToolTipHandle;
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
	return W_TRUE;
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
void _w_treeview_set_checkbox_imagelist(w_treeview *tree,
		_w_control_priv *priv) {
	if ((_W_WIDGET(tree)->style & W_CHECK) == 0)
		return;
	HWND handle = _W_WIDGET(tree)->handle;
	int count = 5, flags = 0;
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		flags |= ILC_COLOR32;
	} else {
		HDC hDC = GetDC(handle);
		int bits = GetDeviceCaps(hDC, BITSPIXEL);
		int planes = GetDeviceCaps(hDC, PLANES);
		ReleaseDC(handle, hDC);
		int depth = bits * planes;
		switch (depth) {
		case 4:
			flags |= ILC_COLOR4;
			break;
		case 8:
			flags |= ILC_COLOR8;
			break;
		case 16:
			flags |= ILC_COLOR16;
			break;
		case 24:
			flags |= ILC_COLOR24;
			break;
		case 32:
			flags |= ILC_COLOR32;
			break;
		default:
			flags |= ILC_COLOR;
			break;
		}
		flags |= ILC_MASK;
	}
	if ((_W_WIDGET(tree)->style & W_RIGHT_TO_LEFT) != 0)
		flags |= ILC_MIRROR;
	int height = SendMessageW(handle, TVM_GETITEMHEIGHT, 0, 0);
	int width = height;
	HIMAGELIST hStateList = ImageList_Create(width, height, flags, count,
			count);
	HDC hDC = GetDC(handle);
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, width * count, height);
	HBITMAP hOldBitmap = SelectObject(memDC, hBitmap);
	RECT rect;
	SetRect(&rect, 0, 0, width * count, height);
	/*
	 * NOTE: DrawFrameControl() draws a black and white
	 * mask when not drawing a push button.  In order to
	 * make the box surrounding the check mark transparent,
	 * fill it with a color that is neither black or white.
	 */
	int clrBackground = 0;
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		w_control *control = priv->find_background_control(W_CONTROL(tree),
				priv);
		_w_control_priv *cpriv;
		if (control == 0) {
			control = W_CONTROL(tree);
			cpriv = priv;
		} else {
			cpriv = _W_CONTROL_GET_PRIV(control);
		}
		clrBackground = cpriv->get_background_pixel(control, cpriv);
	} else {
		clrBackground = 0x020000FF;
		if ((clrBackground & 0xFFFFFF) == GetSysColor(COLOR_WINDOW)) {
			clrBackground = 0x0200FF00;
		}
	}
	HBRUSH hBrush = CreateSolidBrush(clrBackground);
	FillRect(memDC, &rect, hBrush);
	DeleteObject(hBrush);
	HFONT oldFont = SelectObject(hDC,
			priv->default_font(W_CONTROL(tree), priv));
	TEXTMETRICW tm;
	GetTextMetricsW(hDC, &tm);
	SelectObject(hDC, oldFont);
	int itemWidth = WMIN(tm.tmHeight, width);
	int itemHeight = WMIN(tm.tmHeight, height);
	/*
	 * On Windows when OS level custom zoom is more than 150% then OS
	 * doesn't support auto-scaling of the native check-box images and hence
	 * the size of native check-box never goes more than 20px wide. So, to
	 * handle this special case in Table/Tree, need to apply the same upper
	 * cap to the size of custom drawn check-box images, so check-box images
	 * look in proper ratio. For more details refer bug 489828.
	 */
	itemWidth = WMIN(20, itemWidth);
	itemHeight = WMIN(20, itemHeight);
	int left = (width - itemWidth) / 2, top = (height - itemHeight) / 2 + 1;
	SetRect(&rect, left + width, top, left + width + itemWidth,
			top + itemHeight);
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		HTHEME hTheme = OpenThemeData(NULL, L"BUTTON");
		DrawThemeBackground(hTheme, memDC, BP_CHECKBOX, CBS_UNCHECKEDNORMAL,
				&rect, NULL);
		rect.left += width;
		rect.right += width;
		DrawThemeBackground(hTheme, memDC, BP_CHECKBOX, CBS_CHECKEDNORMAL,
				&rect, NULL);
		rect.left += width;
		rect.right += width;
		DrawThemeBackground(hTheme, memDC, BP_CHECKBOX, CBS_UNCHECKEDNORMAL,
				&rect, NULL);
		rect.left += width;
		rect.right += width;
		DrawThemeBackground(hTheme, memDC, BP_CHECKBOX, CBS_MIXEDNORMAL, &rect,
		NULL);
		CloseThemeData(hTheme);
	} else {
		DrawFrameControl(memDC, &rect, DFC_BUTTON,
		DFCS_BUTTONCHECK | DFCS_FLAT);
		rect.left += width;
		rect.right += width;
		DrawFrameControl(memDC, &rect, DFC_BUTTON,
		DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT);
		rect.left += width;
		rect.right += width;
		DrawFrameControl(memDC, &rect, DFC_BUTTON,
		DFCS_BUTTONCHECK | DFCS_INACTIVE | DFCS_FLAT);
		rect.left += width;
		rect.right += width;
		DrawFrameControl(memDC, &rect, DFC_BUTTON,
		DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_INACTIVE | DFCS_FLAT);
	}
	SelectObject(memDC, hOldBitmap);
	DeleteDC(memDC);
	ReleaseDC(handle, hDC);
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		ImageList_Add(hStateList, hBitmap, 0);
	} else {
		ImageList_AddMasked(hStateList, hBitmap, clrBackground);
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
	//int oldIgnore = ignoreResize;
	//ignoreResize = true;
	SetWindowPos(handle, 0, pos.x - left - b, pos.y + cy - b, w + left + b * 2,
			h + b * 2, SWP_NOACTIVATE | SWP_NOZORDER);
	//ignoreResize = oldIgnore;
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
void _w_treeview_class_init(struct _w_treeview_class *clazz) {
	_w_listviewbase_class_init(W_LISTVIEWBASE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TREEVIEW;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_treeview_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_treeview);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_treeview);
	/*
	 * functions
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_treeview_post_event;
	W_CONTROL_CLASS(clazz)->set_bounds = _w_treeview_set_bounds;
	W_LISTVIEWBASE_CLASS(clazz)->insert_column = _w_treeview_insert_column;
	clazz->clear = _w_treeview_clear;
	clazz->deselect = _w_treeview_deselect;
	clazz->get_item_from_point = _w_treeview_get_item_from_point;
	clazz->get_parent_item = _w_treeview_get_parent_item;
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
	W_LISTITEM_CLASS(treeitem)->get_bounds = _w_treeitem_get_bounds;
	W_LISTITEM_CLASS(treeitem)->get_checked = _w_treeitem_get_checked;
	W_LISTITEM_CLASS(treeitem)->get_grayed = _w_treeitem_get_grayed;
	W_LISTITEM_CLASS(treeitem)->get_image = _w_treeitem_get_image;
	W_LISTITEM_CLASS(treeitem)->set_checked = _w_treeitem_set_checked;
	W_LISTITEM_CLASS(treeitem)->set_grayed = _w_treeitem_set_grayed;
	W_LISTITEM_CLASS(treeitem)->set_image = _w_treeitem_set_image;
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
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->create_handle = _w_treeview_create_handle;
	priv->widget_style = _w_treeview_widget_style;
	priv->window_class = _w_treeview_window_class;
	priv->widget.call_window_proc = _w_treeview_call_window_proc;
	priv->handle_top = _w_treeview_top_handle;
	_W_LISTVIEWBASE_PRIV(priv)->get_header = _w_treeview_get_header;
	_W_CONTROL_PRIV(priv)->default_background = _w_treeview_default_background;
	//win_toolkit->EXPLORER_THEME = TRUE;
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
