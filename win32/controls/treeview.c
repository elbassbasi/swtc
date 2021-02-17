/*
 * Name:        treeview.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "treeview.h"
#include "../widgets/toolkit.h"
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
	HTREEITEM hNewItem = (HTREEITEM) SendMessageW(_W_WIDGET(tree)->handle,
	TVM_INSERTITEMW, 0, (LPARAM) &tvInsert);
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
wresult _w_treeitem_get_data(w_item *item, void **data) {
	w_widget *tree = _W_ITEM(item)->parent;
	TVITEMW tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.lParam = 0;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	if (SendMessageW(_W_WIDGET(tree)->handle, TVM_GETITEMW, 0,
			(LPARAM) &tvItem)) {
		*data = (void*) tvItem.lParam;
		return W_TRUE;
	} else {
		*data = 0;
		return W_FALSE;
	}
}
wresult _w_treeitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_treeitem_set_data(w_item *item, void *data) {
	w_widget *tree = _W_ITEM(item)->parent;
	TVITEMW tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.lParam = (LPARAM) data;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	HRESULT result = SendMessageW(_W_WIDGET(tree)->handle, TVM_SETITEMW, 0,
			(LPARAM) &tvItem);
	if (result == 0)
		return W_FALSE;
	else
		return W_TRUE;
}
wresult _w_treeitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	size_t newlength;
	wresult result = W_FALSE;
	WCHAR *s = _win_text_fix(text, length, &newlength, enc);
	if (s != 0) {
		TVITEMW tvItem;
		w_widget *tree = _W_ITEM(item)->parent;
		tvItem.mask = TVIF_TEXT;
		tvItem.cchTextMax = newlength;
		tvItem.pszText = s;
		tvItem.hItem = _W_TREEITEM(item)->htreeitem;
		if (SendMessageW(_W_WIDGET(tree)->handle, TVM_SETITEMW, 0,
				(LPARAM) &tvItem)) {
			result = W_TRUE;
		}
	}
	_win_text_free(text, s, newlength);
	return result;
}
wresult _w_treeitem_clear(w_treeitem *item, int index, int all) {
	return W_FALSE;
}
wresult _w_treeitem_clear_all(w_treeitem *item, int all) {
	return W_FALSE;
}
wresult _w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_treeitem_get_checked(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_expanded(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_first_child(w_treeitem *item, w_treeitem *child) {
	return W_FALSE;
}
wresult _w_treeitem_get_grayed(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem) {
	return W_FALSE;
}
wresult _w_treeitem_get_item_count(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_items(w_treeitem *item, w_iterator *items) {
	return W_FALSE;
}
wresult _w_treeitem_get_image(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_last_child(w_treeitem *item, w_treeitem *child) {
	return W_FALSE;
}
wresult _w_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next) {
	return W_FALSE;
}
wresult _w_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent) {
	return W_FALSE;
}
wresult _w_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev) {
	return W_FALSE;
}
wresult _w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		int index) {
	w_widget *tree = _W_ITEM(item)->parent;
	HTREEITEM i = _w_treeview_find_previous(_W_WIDGET(tree)->handle,
	_W_TREEITEM(item)->htreeitem, index);
	if (i == 0)
		return W_ERROR_INVALID_RANGE;
	return _w_treeitem_insert_item_0(item, subitem, i);
}
wresult _w_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
		w_treeitem *after) {
	return W_FALSE;
}
wresult _w_treeitem_remove_all(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_set_checked(w_treeitem *item, int checked) {
	return W_FALSE;
}
wresult _w_treeitem_set_expanded(w_treeitem *item, int expanded) {
	return W_FALSE;
}
wresult _w_treeitem_set_grayed(w_treeitem *item, int grayed) {
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
wresult _w_treeitem_set_image(w_treeitem *item, int image) {
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

	/* Use the Explorer theme */
	if (win_toolkit->EXPLORER_THEME) {
		if (WIN32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
			_W_LISTVIEWBASE(control)->explorerTheme = TRUE;
			SetWindowTheme(handle, L"EXPLORER", NULL);
			int bits = TVS_EX_DOUBLEBUFFER | TVS_EX_RICHTOOLTIP;
			if (win_toolkit->ENABLE_TVS_EX_FADEINOUTEXPANDOS)
				bits |= TVS_EX_FADEINOUTEXPANDOS;
			/*
			 * This code is intentionally commented.
			 */
//			if ((style & W_FULL_SELECTION) == 0) bits |= TVS_EX_AUTOHSCROLL;
			SendMessageW(handle, TVM_SETEXTENDEDSTYLE, 0, bits);
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
	//if ((_W_WIDGET(control)->style & W_CHECK) != 0) setCheckboxImageList ();
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
	bits |= TVS_SHOWSELALWAYS | TVS_LINESATROOT | TVS_HASBUTTONS
			| TVS_NONEVENHEIGHT;
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
wresult _w_treeview_deselect(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
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
wresult _w_treeview_remove(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_set_insert_mark(w_treeview *tree, w_treeitem *item,
		int before) {
	return W_FALSE;
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
/*
 * messages
 */
wresult _TREE_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_GETOBJECT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_HSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_LBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_MOUSEMOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_MOUSEWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_MOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_RBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_SETFONT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_SETREDRAW(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_SYSCOLORCHANGE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_VSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_TIMER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_CTLCOLORCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return _COMPOSITE_WM_NOTIFY(widget, e, priv);
}
/*
 * tree notification
 */
wresult _TREE_WM_NOTIFY_GETDISPINFO(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_CDDS_PREPAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_CDDS_ITEMPREPAINT(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_CDDS_POSTPAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_CUSTOMDRAW(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_DBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_ITEMCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_SELCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_SELCHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMTREEVIEWW *treeView = (NMTREEVIEWW*) e->lparam;
	TVITEMW tvItem;
	w_event_list event;
	w_treeitem item;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_ITEM_SELECTION;
	event.event.widget = widget;
	event.item = W_ITEM(&item);
	_W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
	_W_ITEM(&item)->parent = widget;
	_W_ITEM(&item)->index = -1;
	_W_TREEITEM(&item)->htreeitem = (HTREEITEM) treeView->itemNew.hItem;
	_w_widget_send_event(widget, (w_event*) &event);
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_ITEMEXPANDED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_ITEMEXPANDING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMTREEVIEW *treeView = (NMTREEVIEW*) e->lparam;
	wresult runExpanded = W_FALSE;
	w_event_list event;
	w_treeitem item;
	TVITEMW tvItem;
	memset(&event, 0, sizeof(event));
	event.event.widget = widget;
	event.item = W_ITEM(&item);
	_W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
	_W_ITEM(&item)->parent = widget;
	_W_ITEM(&item)->index = -1;
	_W_TREEITEM(&item)->htreeitem = (HTREEITEM) treeView->itemNew.hItem;
	switch (treeView->action) {
	case TVE_EXPAND:
		/*
		 * Bug in Windows.  When the numeric keypad asterisk
		 * key is used to expand every item in the tree, Windows
		 * sends TVN_ITEMEXPANDING to items in the tree that
		 * have already been expanded.  The fix is to detect
		 * that the item is already expanded and ignore the
		 * notification.
		 */
		if ((treeView->itemNew.state & TVIS_EXPANDED) == 0) {
			event.event.type = W_EVENT_ITEM_EXPAND;
			_w_widget_send_event(widget, (w_event*) &event);
			/**
			 * if tree item has no children remove + image
			 */
			HTREEITEM hFirstItem = (HTREEITEM) SendMessageW(
			_W_WIDGET(widget)->handle, TVM_GETNEXTITEM,
			TVGN_CHILD, (LPARAM) treeView->itemNew.hItem);
			if (hFirstItem == 0) {
				tvItem.mask = TVIF_CHILDREN;
				tvItem.cChildren = 0;
				tvItem.hItem = treeView->itemNew.hItem;
				HRESULT result = SendMessageW(_W_WIDGET(widget)->handle,
				TVM_SETITEMW, 0, (LPARAM) &tvItem);
			}
		}
		break;
	case TVE_COLLAPSE:
		event.event.type = W_EVENT_ITEM_COLLAPSE;
		_w_widget_send_event(widget, (w_event*) &event);
		break;
	}
	return W_FALSE;
}
wresult _TREE_WM_NOTIFY_BEGINDRAG(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREE_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	switch (((NMHDR*) e->lparam)->code) {
	case TVN_GETDISPINFOA:
	case TVN_GETDISPINFOW:
		return _TREE_WM_NOTIFY_GETDISPINFO(widget, e, priv);
		break;
	case NM_CUSTOMDRAW:
		return _TREE_WM_NOTIFY_CUSTOMDRAW(widget, e, priv);
		break;
	case NM_DBLCLK:
		return _TREE_WM_NOTIFY_DBLCLK(widget, e, priv);
		break;
		/*
		 * Bug in Windows.  On Vista, when TVM_SELECTITEM is called
		 * with TVGN_CARET in order to set the selection, for some
		 * reason, Windows deselects the previous two items that
		 * were selected.  The fix is to stop the selection from
		 * changing on all but the item that is supposed to be
		 * selected.
		 */
	case TVN_ITEMCHANGINGA:
	case TVN_ITEMCHANGINGW:
		return _TREE_WM_NOTIFY_ITEMCHANGING(widget, e, priv);
		break;
	case TVN_SELCHANGINGA:
	case TVN_SELCHANGINGW:
		return _TREE_WM_NOTIFY_SELCHANGING(widget, e, priv);
		break;
	case TVN_SELCHANGEDA:
	case TVN_SELCHANGEDW:
		return _TREE_WM_NOTIFY_SELCHANGED(widget, e, priv);
		break;
	case TVN_ITEMEXPANDINGA:
	case TVN_ITEMEXPANDINGW:
		return _TREE_WM_NOTIFY_ITEMEXPANDING(widget, e, priv);
		break;
	case TVN_ITEMEXPANDEDA:
	case TVN_ITEMEXPANDEDW:
		return _TREE_WM_NOTIFY_ITEMEXPANDED(widget, e, priv);
		break;
	case TVN_BEGINDRAGA:
	case TVN_BEGINDRAGW:
		if ((GetKeyState(VK_LBUTTON) >= 0)) {
			return _TREE_WM_NOTIFY_BEGINDRAG(widget, e, priv);
		}
		break;
	case TVN_BEGINRDRAGA:
	case TVN_BEGINRDRAGW:
		return _TREE_WM_NOTIFY_BEGINDRAG(widget, e, priv);
		break;
	}
	return W_FALSE;
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
	_w_item_class_init(W_ITEM_CLASS(treeitem));
	W_ITEM_CLASS(treeitem)->get_data = _w_treeitem_get_data;
	W_ITEM_CLASS(treeitem)->get_text = _w_treeitem_get_text;
	W_ITEM_CLASS(treeitem)->set_data = _w_treeitem_set_data;
	W_ITEM_CLASS(treeitem)->set_text = _w_treeitem_set_text;
	treeitem->clear = _w_treeitem_clear;
	treeitem->clear_all = _w_treeitem_clear_all;
	treeitem->get_bounds = _w_treeitem_get_bounds;
	treeitem->get_checked = _w_treeitem_get_checked;
	treeitem->get_expanded = _w_treeitem_get_expanded;
	treeitem->get_first_child = _w_treeitem_get_first_child;
	treeitem->get_grayed = _w_treeitem_get_grayed;
	treeitem->get_item = _w_treeitem_get_item;
	treeitem->get_item_count = _w_treeitem_get_item_count;
	treeitem->get_items = _w_treeitem_get_items;
	treeitem->get_image = _w_treeitem_get_image;
	treeitem->get_last_child = _w_treeitem_get_last_child;
	treeitem->get_next_sibling = _w_treeitem_get_next_sibling;
	treeitem->get_parent_item = _w_treeitem_get_parent_item;
	treeitem->get_prev_sibling = _w_treeitem_get_prev_sibling;
	treeitem->insert_item = _w_treeitem_insert_item;
	treeitem->insert_item_after = _w_treeitem_insert_item_after;
	treeitem->remove_all = _w_treeitem_remove_all;
	treeitem->set_checked = _w_treeitem_set_checked;
	treeitem->set_expanded = _w_treeitem_set_expanded;
	treeitem->set_grayed = _w_treeitem_set_grayed;
	treeitem->set_image = _w_treeitem_set_image;
	treeitem->set_has_children = _w_treeitem_set_has_children;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->create_handle = _w_treeview_create_handle;
	priv->widget_style = _w_treeview_widget_style;
	priv->window_class = _w_treeview_window_class;
	/*
	 * messages
	 */
	dispatch_message *msg = priv->messages;
	msg[_WM_CHAR] = _TREE_WM_CHAR;
	msg[_WM_ERASEBKGND] = _TREE_WM_ERASEBKGND;
	msg[_WM_GETOBJECT] = _TREE_WM_GETOBJECT;
	msg[_WM_HSCROLL] = _TREE_WM_HSCROLL;
	msg[_WM_KEYDOWN] = _TREE_WM_KEYDOWN;
	msg[_WM_KILLFOCUS] = _TREE_WM_KILLFOCUS;
	msg[_WM_LBUTTONDBLCLK] = _TREE_WM_LBUTTONDBLCLK;
	msg[_WM_LBUTTONDOWN] = _TREE_WM_LBUTTONDOWN;
	msg[_WM_MOUSEMOVE] = _TREE_WM_MOUSEMOVE;
	msg[_WM_MOUSEWHEEL] = _TREE_WM_MOUSEWHEEL;
	msg[_WM_MOVE] = _TREE_WM_MOVE;
	msg[_WM_RBUTTONDOWN] = _TREE_WM_RBUTTONDOWN;
	msg[_WM_PAINT] = _TREE_WM_PAINT;
	msg[_WM_SETCURSOR] = _TREE_WM_SETCURSOR;
	msg[_WM_SETFOCUS] = _TREE_WM_SETFOCUS;
	msg[_WM_SETFONT] = _TREE_WM_SETFONT;
	msg[_WM_SETREDRAW] = _TREE_WM_SETREDRAW;
	msg[_WM_SIZE] = _TREE_WM_SIZE;
	msg[_WM_SYSCOLORCHANGE] = _TREE_WM_SYSCOLORCHANGE;
	msg[_WM_VSCROLL] = _TREE_WM_VSCROLL;
	msg[_WM_TIMER] = _TREE_WM_TIMER;
	msg[_WM_CTLCOLORCHILD] = _TREE_WM_CTLCOLORCHILD;
	msg[_WM_NOTIFY] = _TREE_WM_NOTIFY;
	msg[_WM_NOTIFYCHILD] = _TREE_WM_NOTIFYCHILD;
}
