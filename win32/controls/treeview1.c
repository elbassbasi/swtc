/*
 * treeview1.c
 *
 *  Created on: 11 juin 2021
 *      Author: Azeddine
 */
#include "treeview.h"
#include "../widgets/toolkit.h"
#include <vsstyle.h>
/*
 * messages
 */
wresult _w_treeview_post_event(w_widget *widget, w_event *ee,int flags) {
	if (ee->type == W_EVENT_PLATFORM) {
		_w_event_platform *e = (_w_event_platform*) ee;
		HWND hwndHeader;
		HWND hwndParent = _W_TREEVIEW(widget)->hwndParent;
		if (e->hwnd == hwndParent) {
			switch (e->msg) {
			case WM_SIZE: {
				_w_treeview_set_scroll_width(W_TREEVIEW(widget));
				_w_treeview_update_scrollbar(W_TREEVIEW(widget));
				//int total = _w_tree_get_header_width(W_TREE(widget));
				//_w_tree_update_size(W_TREE(widget), total);
				e->result = TRUE;
			}
				break;
			case WM_HSCROLL: {
				RECT rc;
				if (GetClientRect(hwndParent, &rc) != TRUE)
					return FALSE;
				int cx = rc.right - rc.left;
				int pos = GetScrollPos(hwndParent, SB_HORZ);
				int total = _w_treeview_get_header_width(W_TREEVIEW(widget));
				switch (LOWORD(e->wparam)) {
				case SB_LINELEFT:
					pos -= GetSystemMetrics(SM_CXVSCROLL);
					break;
				case SB_LINERIGHT:
					pos += GetSystemMetrics(SM_CXVSCROLL);
					break;
				case SB_PAGELEFT:
					pos -= cx;
					break;
				case SB_PAGERIGHT:
					pos += cx;
					break;
				case SB_LEFT:
					pos = 0;
					break;
				case SB_RIGHT:

					pos = total - cx;
					break;
				case SB_THUMBTRACK:
					pos = HIWORD(e->wparam);
					break;
				}
				if (pos < 0)
					pos = 0;
				else if (pos > total - cx)
					pos = total - cx;

				SetScrollPos(hwndParent, SB_HORZ, pos, FALSE);
				//_w_tree_update_size(W_TREE(widget),total);
				_w_treeview_set_scroll_width(W_TREEVIEW(widget));
				e->result = TRUE;
				return W_TRUE;
			}
				break;
			case WM_VSCROLL: {
				SCROLLINFO info;
				info.cbSize = sizeof(SCROLLINFO);
				info.fMask = SIF_ALL;
				GetScrollInfo(hwndParent, SB_VERT, &info);
				/*
				 * Update the nPos field to match the nTrackPos field
				 * so that the tree scrolls when the scroll bar of the
				 * parent is dragged.
				 *
				 * NOTE: For some reason, this code is only necessary
				 * on Windows Vista.
				 */
				if (/*!IsWinCE &&*/WIN32_VERSION >= VERSION(6, 0)) {
					if (LOWORD (e->wparam) == SB_THUMBTRACK) {
						info.nPos = info.nTrackPos;
					}
				}
				SetScrollInfo(_W_WIDGET(widget)->handle, SB_VERT, &info, TRUE);
				e->result = SendMessageW(_W_WIDGET(widget)->handle, WM_VSCROLL,
						e->wparam, e->lparam);
				GetScrollInfo(_W_WIDGET(widget)->handle, SB_VERT, &info);
				SetScrollInfo(hwndParent, SB_VERT, &info,
				TRUE);
				return W_TRUE;
			}
				break;
			case WM_NOTIFY: {
				NMHDR *hdr = (NMHDR*) e->lparam;
				if (hdr != 0) {
					hwndHeader = _W_TREEVIEW(widget)->hwndHeader;
					if (hdr->hwndFrom == hwndHeader) {
						_TREEVIEW_WM_NOTIFY_HEADER(widget, e,
								_W_CONTROL_GET_PRIV(widget));
					} else {
						e->result = SendMessageW(hdr->hwndFrom, WM_NOTIFY_CHILD,
								e->wparam, e->lparam);
					}
				}
			}
				break;
			case WM_DESTROY: {
				SetWindowLongPtrW(e->hwnd, GWLP_USERDATA, (LONG_PTR) 0);
				e->result = 0;
				return W_TRUE;
			}
			default:
				e->result = DefWindowProcW(e->hwnd, e->msg, e->wparam,
						e->lparam);
				return W_TRUE;
				break;
			}
		}
	}
	return _w_control_post_event(widget, ee,flags);
}
wresult _w_treeview_call_window_proc(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HWND handle = _W_WIDGET(widget)->handle;
	if (handle == 0)
		return W_FALSE;
	HWND hwndParent = _W_TREEVIEW(widget)->hwndParent;
	if (hwndParent != 0 && e->hwnd == hwndParent) {
		return DefWindowProcW(e->hwnd, e->msg, e->wparam, e->lparam);
	}
	HWND hwndHeader = _W_TREEVIEW(widget)->hwndHeader;
	if (hwndHeader != 0 && e->hwnd == hwndHeader) {
		return CallWindowProcW(_W_TREEVIEW_PRIV(priv)->HeaderProc, e->hwnd,
				e->msg, e->wparam, e->lparam);
	}
	switch (e->msg) {
	case WM_SETFOCUS: {
		/*
		 * Feature in Windows.  When a tree control processes WM_SETFOCUS,
		 * if no item is selected, the first item in the tree is selected.
		 * This is unexpected and might clear the previous selection.
		 * The fix is to detect that there is no selection and set it to
		 * the first visible item in the tree.  If the item was not selected,
		 * only the focus is assigned.
		 */
		if ((_W_WIDGET(widget)->style & W_SINGLE) != 0)
			break;
		HTREEITEM hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
		TVGN_CARET, 0);
		if (hItem == 0) {
			hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
			TVGN_FIRSTVISIBLE, 0);
			if (hItem != 0) {
				TVITEMW tvItem;
				tvItem.mask = TVIF_HANDLE | TVIF_STATE;
				tvItem.hItem = hItem;
				SendMessageW(handle, TVM_GETITEMW, 0, (LPARAM) &tvItem);
				_W_TREEVIEW(widget)->hSelect = hItem;
				_W_TREEVIEW(widget)->ignoreDeselect =
				_W_TREEVIEW(widget)->ignoreSelect =
				_W_TREEVIEW(widget)->lockSelection = TRUE;
				SendMessageW(handle, TVM_SELECTITEM, TVGN_CARET,
						(LPARAM) hItem);
				_W_TREEVIEW(widget)->ignoreDeselect =
				_W_TREEVIEW(widget)->ignoreSelect =
				_W_TREEVIEW(widget)->lockSelection = FALSE;
				_W_TREEVIEW(widget)->hSelect = 0;
				if ((tvItem.state & TVIS_SELECTED) == 0) {
					SendMessageW(handle, TVM_SETITEMW, 0, (LPARAM) &tvItem);
				}
			}
		}
		break;
	}
	}
	char exec[3];
	HTREEITEM hItem = 0;
	int redraw = FALSE;
	switch (e->msg) {
	/* Keyboard messages */
	case WM_KEYDOWN:
		//if (e->wparam == VK_CONTROL || e->wparam == VK_SHIFT) break;
		//FALL THROUGH
	case WM_CHAR:
	case WM_IME_CHAR:
	case WM_KEYUP:
	case WM_SYSCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		//FALL THROUGH

		/* Scroll messages */
	case WM_HSCROLL:
	case WM_VSCROLL:
		//FALL THROUGH

		/* Resize messages */
	case WM_SIZE:
		/*redraw = findImageControl () != null && getDrawing () && IsWindowVisible (handle);
		 if (redraw) DefWindowProc (handle, WM_SETREDRAW, 0, 0);*/
		//FALL THROUGH
		/* Mouse messages */
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEHOVER:
	case WM_MOUSELEAVE:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		//FALL THROUGH

		/* Other messages */
	case WM_SETFONT:
	case WM_TIMER: {
		/*if (findImageControl () != null) {
		 hItem = SendMessage (handle, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);
		 }*/

		break;
	}
	}
	e->result = CallWindowProcW(priv->def_window_proc, e->hwnd, e->msg,
			e->wparam, e->lparam);
	switch (e->msg) {
	/* Keyboard messages */
	case WM_KEYDOWN:
		if (e->wparam == VK_CONTROL || e->wparam == VK_SHIFT)
			break;
		/* FALLTHROUGH */
	case WM_CHAR:
	case WM_IME_CHAR:
	case WM_KEYUP:
	case WM_SYSCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		//FALL THROUGH

		/* Scroll messages */
	case WM_HSCROLL:
	case WM_VSCROLL:
		//FALL THROUGH

		/* Resize messages */
	case WM_SIZE:
		if (redraw) {
			DefWindowProcW(handle, WM_SETREDRAW, 1, 0);
			InvalidateRect(handle, NULL, TRUE);
			if (hwndHeader != 0)
				InvalidateRect(hwndHeader, NULL, TRUE);
		}
		/* FALLTHROUGH */

		/* Mouse messages */
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEHOVER:
	case WM_MOUSELEAVE:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		//FALL THROUGH

		/* Other messages */
	case WM_SETFONT:
	case WM_TIMER: {
		/*if (findImageControl () != null) {
		 if (hItem != SendMessageW (handle, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0)) {
		 InvalidateRect (handle, NULL, TRUE);
		 }
		 }
		 updateScrollBar ();*/
		break;
	}

	case WM_PAINT:
		_W_TREEVIEW(widget)->painted = TRUE;
		break;
	}
	return W_TRUE;
}
wresult _TREEVIEW_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_CHAR(widget, e, priv);
	if (result != 0)
		return result;
	HWND handle = _W_WIDGET(widget)->handle;
	_w_treeitem item;
	w_event_list event;
	/*
	 * Feature in Windows.  The tree control beeps
	 * in WM_CHAR when the search for the item that
	 * matches the key stroke fails.  This is the
	 * standard tree behavior but is unexpected when
	 * the key that was typed was ESC, CR or SPACE.
	 * The fix is to avoid calling the tree window
	 * proc in these cases.
	 */
	switch (e->wparam) {
	case ' ': {
		HTREEITEM hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
		TVGN_CARET, 0);
		if (hItem != 0) {
			_W_TREEVIEW(widget)->hAnchor = hItem;
			SendMessageW(handle, TVM_ENSUREVISIBLE, 0, (LPARAM) hItem);
			TVITEMW tvItem;
			tvItem.mask = TVIF_HANDLE | TVIF_STATE | TVIF_PARAM;
			tvItem.hItem = hItem;
			if ((_W_WIDGET(widget)->style & W_CHECK) != 0) {
				tvItem.stateMask = TVIS_STATEIMAGEMASK;
				SendMessageW(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
				ITEM_STATE_SELECTED(tvItem.state);
				SendMessageW(handle, TVM_SETITEMW, 0, (LPARAM) &tvItem);
				LONG_PTR id = (LONG_PTR) hItem;
				if (_COMCTL32_VERSION >= VERSION(6, 0)) {
					id = SendMessageW(handle, TVM_MAPHTREEITEMTOACCID,
							(WPARAM) hItem, 0);
				}
				NotifyWinEvent(EVENT_OBJECT_FOCUS, handle, OBJID_CLIENT, id);
			}
			tvItem.stateMask = TVIS_SELECTED;
			SendMessageW(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
			if ((_W_WIDGET(widget)->style & W_MULTI) != 0
					&& GetKeyState(VK_CONTROL) < 0) {
				if ((tvItem.state & TVIS_SELECTED) != 0) {
					tvItem.state &= ~TVIS_SELECTED;
				} else {
					tvItem.state |= TVIS_SELECTED;
				}
			} else {
				tvItem.state |= TVIS_SELECTED;
			}
			SendMessageW(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_SELECTION;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.item = W_LISTITEM(&item);
			W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
			_W_ITEM(&item)->parent = widget;
			_W_ITEM(&item)->index = -1;
			_W_TREEITEM(&item)->htreeitem = hItem;
			_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
			if ((_W_WIDGET(widget)->style & W_CHECK) != 0) {
				event.detail = W_CHECK;
				_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
			}
		}
		e->result = 0;
		return W_TRUE;
	}
	case W_CR: {
		/*
		 * Feature in Windows.  Windows sends NM_RETURN from WM_KEYDOWN
		 * instead of using WM_CHAR.  This means that application code
		 * that expects to consume the key press and therefore avoid a
		 * SWT.DefaultSelection event from WM_CHAR will fail.  The fix
		 * is to implement SWT.DefaultSelection in WM_CHAR instead of
		 * using NM_RETURN.
		 */
		HTREEITEM hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
		TVGN_CARET, 0);
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_DEFAULTSELECTION;
		event.event.widget = widget;
		event.event.platform_event = (w_event_platform*) e;
		if (hItem != 0) {
			event.event.type = W_EVENT_ITEM_DEFAULTSELECTION;
			W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
			_W_ITEM(&item)->parent = widget;
			_W_ITEM(&item)->index = -1;
			_W_TREEITEM(&item)->htreeitem = hItem;
			event.item = W_LISTITEM(&item);
		}
		_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
		e->result = 0;
		return W_TRUE;
	}
	case W_ESC:
		e->result = 0;
		return W_TRUE;
		break;

	}
	return result;
}
wresult _TREEVIEW_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_ERASEBKGND(widget, e, priv);
	if ((_W_WIDGET(widget)->style & W_DOUBLE_BUFFERED) != 0) {
		e->result = 1;
		return W_TRUE;
	}
	if (priv->find_image_control(W_CONTROL(widget), priv) != 0) {
		e->result = 1;
		return W_TRUE;
	}
	return result;
}
wresult _TREEVIEW_WM_GETOBJECT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREEVIEW_WM_HSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int fixScroll = FALSE;
	if ((_W_WIDGET(widget)->style & W_DOUBLE_BUFFERED) != 0) {
		fixScroll = (_W_WIDGET(widget)->style & (W_VIRTUAL | W_CUSTOMDRAW))
				!= 0;
	}
	HWND handle = _W_WIDGET(widget)->handle;
	if (fixScroll) {
		_W_WIDGET(widget)->style &= ~W_DOUBLE_BUFFERED;
		if (_W_TREEVIEW(widget)->explorerTheme) {
			SendMessageW(handle, TVM_SETEXTENDEDSTYLE, TVS_EX_DOUBLEBUFFER, 0);
		}
	}
	wresult result = _SCROLLABLE_WM_HSCROLL(widget, e, priv);
	if (fixScroll) {
		_W_WIDGET(widget)->style |= W_DOUBLE_BUFFERED;
		if (_W_TREEVIEW(widget)->explorerTheme) {
			SendMessageW(handle, TVM_SETEXTENDEDSTYLE, TVS_EX_DOUBLEBUFFER,
			TVS_EX_DOUBLEBUFFER);
		}
	}
	return result;
}
wresult _TREEVIEW_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_KEYDOWN(widget, e, priv);
	if (result)
		return result;
	HWND handle = _W_WIDGET(widget)->handle;
	HWND lastHwnd;
	switch (e->wparam) {
	case VK_LEFT:
	case VK_RIGHT: {
		/*
		 * Bug in Windows. The behavior for the left and right keys is not
		 * changed if the orientation changes after the control was created.
		 * The fix is to replace VK_LEFT by VK_RIGHT and VK_RIGHT by VK_LEFT
		 * when the current orientation differs from the orientation used to
		 * create the control.
		 */
		int isRTL = (_W_WIDGET(widget)->style & W_RIGHT_TO_LEFT) != 0;
		if (isRTL != _W_LISTVIEWBASE(widget)->createdAsRTL) {
			WPARAM last = e->wparam;
			e->wparam = last == VK_RIGHT ? VK_LEFT : VK_RIGHT;
			lastHwnd = e->hwnd;
			e->hwnd = handle;
			priv->widget.call_window_proc(widget, e, priv);
			e->hwnd = lastHwnd;
			e->wparam = last;
			return W_TRUE;
		}
	}
		break;
	case VK_SPACE:
		/*
		 * Ensure that the window proc does not process VK_SPACE
		 * so that it can be handled in WM_CHAR.  This allows the
		 * application to cancel an operation that is normally
		 * performed in WM_KEYDOWN from WM_CHAR.
		 */
		e->result = 0;
		return W_TRUE;
		break;
	case VK_ADD:
		if (GetKeyState(VK_CONTROL) < 0) {
			HWND hwndHeader = _W_TREEVIEW(widget)->hwndHeader;
			if (hwndHeader != 0) {
				_w_item column;
				W_WIDGETDATA(&column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(
						widget);
				_W_ITEM(&column)->parent = widget;
				int columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0,
						0);
				for (int i = 0; i < columnCount; i++) {
					_W_ITEM(&column)->index = i;
					if (w_columnitem_get_resizable(W_COLUMNITEM(&column)) > 0) {
						w_columnitem_pack(W_COLUMNITEM(&column));
					}
				}
			}
		}
		break;
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_HOME:
	case VK_END: {
		SendMessageW(handle, WM_CHANGEUISTATE, UIS_INITIALIZE, 0);
		/*if (itemToolTipHandle != 0)
		 ShowWindow(itemToolTipHandle, SW_HIDE);*/
		if ((_W_WIDGET(widget)->style & W_SINGLE) != 0)
			break;
		if (GetKeyState(VK_SHIFT) < 0) {
			HTREEITEM hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
			TVGN_CARET, 0);
			if (hItem != 0) {
				if (_W_TREEVIEW(widget)->hAnchor == 0)
					_W_TREEVIEW(widget)->hAnchor = hItem;
				_W_TREEVIEW(widget)->ignoreSelect =
				_W_TREEVIEW(widget)->ignoreDeselect = TRUE;
				lastHwnd = e->hwnd;
				e->hwnd = handle;
				priv->widget.call_window_proc(widget, e, priv);
				e->hwnd = lastHwnd;
				_W_TREEVIEW(widget)->ignoreSelect =
				_W_TREEVIEW(widget)->ignoreDeselect = FALSE;
				HTREEITEM hNewItem = (HTREEITEM) SendMessageW(handle,
				TVM_GETNEXTITEM, TVGN_CARET, 0);
				TVITEMW tvItem;
				tvItem.mask = TVIF_HANDLE | TVIF_STATE;
				tvItem.stateMask = TVIS_SELECTED;
				HTREEITEM hDeselectItem = hItem;
				RECT rect1;
				if (!TreeView_GetItemRect(handle, _W_TREEVIEW(widget)->hAnchor,
						&rect1, FALSE)) {
					_W_TREEVIEW(widget)->hAnchor = hItem;
					TreeView_GetItemRect(handle, _W_TREEVIEW(widget)->hAnchor,
							&rect1, FALSE);
				}
				RECT rect2;
				TreeView_GetItemRect(handle, hDeselectItem, &rect2, FALSE);
				int flags = rect1.top < rect2.top ?
				TVGN_PREVIOUSVISIBLE :
													TVGN_NEXTVISIBLE;
				while (hDeselectItem != _W_TREEVIEW(widget)->hAnchor) {
					tvItem.hItem = hDeselectItem;
					SendMessage(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
					hDeselectItem = (HTREEITEM) SendMessageW(handle,
					TVM_GETNEXTITEM, flags, (LPARAM) hDeselectItem);
				}
				HTREEITEM hSelectItem = _W_TREEVIEW(widget)->hAnchor;
				TreeView_GetItemRect(handle, hNewItem, &rect1, FALSE);
				TreeView_GetItemRect(handle, hSelectItem, &rect2, FALSE);
				tvItem.state = TVIS_SELECTED;
				flags = rect1.top < rect2.top ? TVGN_PREVIOUSVISIBLE :
				TVGN_NEXTVISIBLE;
				while (hSelectItem != hNewItem) {
					tvItem.hItem = hSelectItem;
					SendMessageW(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
					hSelectItem = (HTREEITEM) SendMessageW(handle,
					TVM_GETNEXTITEM, flags, (LPARAM) hSelectItem);
				}
				tvItem.hItem = hNewItem;
				SendMessageW(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
				tvItem.mask = TVIF_HANDLE | TVIF_PARAM;
				tvItem.hItem = hNewItem;
				SendMessageW(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
				w_event_list event;
				_w_treeitem item;
				memset(&event, 0, sizeof(event));
				event.event.type = W_EVENT_ITEM_SELECTION;
				event.event.widget = widget;
				event.event.platform_event = (w_event_platform*) e;
				event.item = W_LISTITEM(&item);
				W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(
						widget);
				_W_ITEM(&item)->parent = widget;
				_W_ITEM(&item)->index = -1;
				_W_TREEITEM(&item)->htreeitem = hNewItem;
				_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
				return W_TRUE;
			}
		}
		if (GetKeyState(VK_CONTROL) < 0) {
			HTREEITEM hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
			TVGN_CARET, 0);
			if (hItem != 0) {
				TVITEM tvItem;
				tvItem.mask = TVIF_HANDLE | TVIF_STATE;
				tvItem.stateMask = TVIS_SELECTED;
				tvItem.hItem = hItem;
				SendMessageW(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
				boolean oldSelected = (tvItem.state & TVIS_SELECTED) != 0;
				HTREEITEM hNewItem = 0;
				switch (e->wparam) {
				case VK_UP:
					hNewItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
					TVGN_PREVIOUSVISIBLE, (LPARAM) &hItem);
					break;
				case VK_DOWN:
					hNewItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
					TVGN_NEXTVISIBLE, (LPARAM) &hItem);
					break;
				case VK_HOME:
					hNewItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
					TVGN_ROOT, 0);
					break;
				case VK_PRIOR:
					hNewItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
					TVGN_FIRSTVISIBLE, 0);
					if (hNewItem == hItem) {
						SendMessageW(handle, WM_VSCROLL, SB_PAGEUP, 0);
						hNewItem = (HTREEITEM) SendMessageW(handle,
						TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);
					}
					break;
				case VK_NEXT: {
					RECT rect, clientRect;
					GetClientRect(handle, &clientRect);
					hNewItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
					TVGN_FIRSTVISIBLE, 0);
					do {
						HTREEITEM hVisible = (HTREEITEM) SendMessageW(handle,
						TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM) hNewItem);
						if (hVisible == 0)
							break;
						if (!TreeView_GetItemRect(handle, hVisible, &rect,
								FALSE))
							break;
						if (rect.bottom > clientRect.bottom)
							break;
						if ((hNewItem = hVisible) == hItem) {
							SendMessage(handle, WM_VSCROLL, SB_PAGEDOWN, 0);
						}
					} while (hNewItem != 0);
				}
					break;
				case VK_END:
					hNewItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
					TVGN_LASTVISIBLE, 0);
					break;
				}
				if (hNewItem != 0) {
					SendMessageW(handle, TVM_ENSUREVISIBLE, 0,
							(LPARAM) hNewItem);
					tvItem.hItem = hNewItem;
					SendMessageW(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
					int newSelected = (tvItem.state & TVIS_SELECTED) != 0;
					int redraw = !newSelected /*&& getDrawing()*/
					&& IsWindowVisible(handle);
					if (redraw) {
						UpdateWindow(handle);
						DefWindowProcW(handle, WM_SETREDRAW, 0, 0);
					}
					_W_TREEVIEW(widget)->hSelect = hNewItem;
					_W_TREEVIEW(widget)->ignoreSelect = TRUE;
					SendMessageW(handle, TVM_SELECTITEM, TVGN_CARET,
							(LPARAM) &hNewItem);
					_W_TREEVIEW(widget)->ignoreSelect = FALSE;
					_W_TREEVIEW(widget)->hSelect = 0;
					if (oldSelected) {
						tvItem.state = TVIS_SELECTED;
						tvItem.hItem = hItem;
						SendMessageW(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
					}
					if (!newSelected) {
						tvItem.state = 0;
						tvItem.hItem = hNewItem;
						SendMessageW(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
					}
					if (redraw) {
						RECT rect1, rect2;
						int fItemRect = (_W_WIDGET(widget)->style
								& W_FULL_SELECTION) == 0;
						if (_W_WIDGET(widget)->style & W_CUSTOMDRAW)
							fItemRect = FALSE;
						if (WIN32_VERSION >= VERSION(6, 0))
							fItemRect = FALSE;
						TreeView_GetItemRect(handle, hItem, &rect1, fItemRect);
						TreeView_GetItemRect(handle, hNewItem, &rect2,
								fItemRect);
						DefWindowProcW(handle, WM_SETREDRAW, 1, 0);
						InvalidateRect(handle, &rect1, TRUE);
						InvalidateRect(handle, &rect2, TRUE);
						UpdateWindow(handle);
					}
					e->result = 0;
					return W_TRUE;
				}
			}
		}
		lastHwnd = e->hwnd;
		e->hwnd = handle;
		priv->widget.call_window_proc(widget, e, priv);
		e->hwnd = lastHwnd;
		_W_TREEVIEW(widget)->hAnchor = (HTREEITEM) SendMessageW(handle,
		TVM_GETNEXTITEM, TVGN_CARET, 0);
		return W_TRUE;
	}
	}
	return result;
}
wresult _TREEVIEW_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Bug in Windows.  When a tree item that has an image
	 * with alpha is expanded or collapsed, the area where
	 * the image is drawn is not erased before it is drawn.
	 * This means that the image gets darker each time.
	 * The fix is to redraw the selection.
	 *
	 * Feature in Windows.  When multiple item have
	 * the TVIS_SELECTED state, Windows redraws only
	 * the focused item in the color used to show the
	 * selection when the tree loses or gains focus.
	 * The fix is to force Windows to redraw the
	 * selection when focus is gained or lost.
	 */
	int redraw = (_W_WIDGET(widget)->style & W_MULTI) != 0;
	if (!redraw) {
		if (_COMCTL32_VERSION >= VERSION(6, 0)) {
			if (_W_LISTVIEWBASE(widget)->imagelist != 0) {
				int bits = GetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE);
				if ((bits & TVS_FULLROWSELECT) == 0) {
					redraw = TRUE;
				}
			}
		}
	}
	//if (redraw) redrawSelection ();
	return _WIDGET_WM_KILLFOCUS(widget, e, priv);
}
wresult _TREEVIEW_WM_LBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result;
	HWND handle = _W_WIDGET(widget)->handle;
	w_event_mouse mouseevent;
	w_event_list event;
	_w_treeitem item;
	TVHITTESTINFO lpht;
	lpht.pt.x = GET_X_LPARAM(e->lparam);
	lpht.pt.y = GET_Y_LPARAM(e->lparam);
	SendMessageW(handle, TVM_HITTEST, 0, (LPARAM) &lpht);
	if (lpht.hItem != 0) {
		if ((_W_WIDGET(widget)->style & W_CHECK) != 0) {
			if ((lpht.flags & TVHT_ONITEMSTATEICON) != 0) {
				win_toolkit->captureChanged = FALSE;
				mouseevent.event.type = W_EVENT_MOUSEDOWN;
				mouseevent.event.time = 0;
				mouseevent.event.platform_event = (w_event_platform*) e;
				mouseevent.event.widget = widget;
				mouseevent.event.data = 0;
				mouseevent.button = 1;
				mouseevent.clickcount = 0;
				mouseevent.detail = 0;
				mouseevent.x = GET_X_LPARAM(e->lparam);
				mouseevent.y = GET_Y_LPARAM(e->lparam);
				_w_set_input_state((w_event*) &mouseevent);
				_w_widget_post_event(widget, (w_event*) &mouseevent,W_EVENT_SEND);
				mouseevent.event.type = W_EVENT_MOUSEDOUBLECLICK;
				mouseevent.event.time = 0;
				mouseevent.event.platform_event = (w_event_platform*) e;
				mouseevent.event.widget = widget;
				mouseevent.event.data = 0;
				mouseevent.button = 1;
				mouseevent.clickcount = 0;
				mouseevent.detail = 0;
				mouseevent.x = GET_X_LPARAM(e->lparam);
				mouseevent.y = GET_Y_LPARAM(e->lparam);
				_w_set_input_state((w_event*) &mouseevent);
				result = _w_widget_post_event(widget, (w_event*) &mouseevent,W_EVENT_SEND);
				if (!result) {
					if (!win_toolkit->captureChanged
							&& w_widget_is_ok(widget) > 0) {
						if (GetCapture() != handle)
							SetCapture(handle);
					}
					e->result = 0;
					return W_TRUE;
				}
				if (!win_toolkit->captureChanged
						&& w_widget_is_ok(widget) > 0) {
					if (GetCapture() != handle)
						SetCapture(handle);
				}
				SetFocus(handle);
				TVITEM tvItem;
				tvItem.hItem = lpht.hItem;
				tvItem.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_STATE;
				tvItem.stateMask = TVIS_STATEIMAGEMASK;
				SendMessageW(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
				ITEM_STATE_SELECTED(tvItem.state);
				SendMessageW(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
				LONG_PTR id = (LONG_PTR) tvItem.hItem;
				if (_COMCTL32_VERSION >= VERSION(6, 0)) {
					id = SendMessageW(handle, TVM_MAPHTREEITEMTOACCID,
							(WPARAM) tvItem.hItem, 0);
				}
				NotifyWinEvent(EVENT_OBJECT_FOCUS, handle, OBJID_CLIENT, id);
				memset(&event, 0, sizeof(event));
				event.event.type = W_EVENT_ITEM_SELECTION;
				event.event.widget = widget;
				event.event.platform_event = (w_event_platform*) e;
				event.detail = W_CHECK;
				event.item = W_LISTITEM(&item);
				W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(
						widget);
				_W_ITEM(&item)->parent = widget;
				_W_ITEM(&item)->index = -1;
				_W_TREEITEM(&item)->htreeitem = tvItem.hItem;
				_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
				e->result = 0;
				return W_TRUE;
			}
		}
	}
	result = _WIDGET_WM_LBUTTONDBLCLK(widget, e, priv);
	if (result == W_TRUE && e->result == 0)
		return result;
	if (lpht.hItem != 0) {
		int flags = TVHT_ONITEM;
		if ((_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0) {
			flags |= TVHT_ONITEMRIGHT | TVHT_ONITEMINDENT;
		} else {
			if ((_W_WIDGET(widget)->style & W_CUSTOMDRAW) != 0) {
				lpht.flags &= ~(TVHT_ONITEMICON | TVHT_ONITEMLABEL);
				/*if (hitTestSelection (lpht.hItem, lpht.pt.x, lpht.pt.y)) {
				 lpht.flags |= TVHT_ONITEMICON | TVHT_ONITEMLABEL;
				 }*/
			}
		}
		if ((lpht.flags & flags) != 0) {
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_DEFAULTSELECTION;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.detail = 0;
			event.item = W_LISTITEM(&item);
			W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
			_W_ITEM(&item)->parent = widget;
			_W_ITEM(&item)->index = -1;
			_W_TREEITEM(&item)->htreeitem = lpht.hItem;
			_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
		}
	}
	return result;
}
wresult _TREEVIEW_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result;
	HWND handle = _W_WIDGET(widget)->handle;
	wuint64 style = _W_WIDGET(widget)->style;
	w_event_mouse mouseevent;
	w_event_list event;
	_w_treeitem item;
	/*
	 * In a multi-select tree, if the user is collapsing a subtree that
	 * contains selected items, clear the selection from these items and
	 * issue a selection event.  Only items that are selected and visible
	 * are cleared.  This code also runs in the case when the white space
	 * below the last item is selected.
	 */
	TVHITTESTINFO lpht;
	lpht.pt.x = GET_X_LPARAM(e->lparam);
	lpht.pt.y = GET_Y_LPARAM(e->lparam);
	SendMessageW(handle, TVM_HITTEST, 0, (LPARAM) &lpht);
	if (lpht.hItem == 0 || (lpht.flags & TVHT_ONITEMBUTTON) != 0) {
		win_toolkit->captureChanged = FALSE;
		mouseevent.event.type = W_EVENT_MOUSEDOWN;
		mouseevent.event.time = 0;
		mouseevent.event.platform_event = (w_event_platform*) e;
		mouseevent.event.widget = widget;
		mouseevent.event.data = 0;
		mouseevent.button = 1;
		mouseevent.clickcount = 0;
		mouseevent.detail = 0;
		mouseevent.x = GET_X_LPARAM(e->lparam);
		mouseevent.y = GET_Y_LPARAM(e->lparam);
		_w_set_input_state((w_event*) &mouseevent);
		result = _w_widget_post_event(widget, (w_event*) &mouseevent,W_EVENT_SEND);
		if (result) {
			if (!win_toolkit->captureChanged && w_widget_is_ok(widget) > 0) {
				if (GetCapture() != handle)
					SetCapture(handle);
			}
			e->result = 0;
			return W_TRUE;
		}
		int fixSelection = FALSE, deselected = FALSE;
		HTREEITEM hOldSelection = (HTREEITEM) SendMessageW(handle,
		TVM_GETNEXTITEM,
		TVGN_CARET, 0);
		if (lpht.hItem != 0 && (style & W_MULTI) != 0) {
			if (hOldSelection != 0) {
				TVITEM tvItem;
				tvItem.mask = TVIF_HANDLE | TVIF_STATE;
				tvItem.hItem = lpht.hItem;
				SendMessageW(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
				if ((tvItem.state & TVIS_EXPANDED) != 0) {
					fixSelection = TRUE;
					tvItem.stateMask = TVIS_SELECTED;
					HTREEITEM hNext = (HTREEITEM) SendMessageW(handle,
					TVM_GETNEXTITEM,
					TVGN_NEXTVISIBLE, (LPARAM) lpht.hItem);
					while (hNext != 0) {
						if (hNext == _W_TREEVIEW(widget)->hAnchor)
							_W_TREEVIEW(widget)->hAnchor = 0;
						tvItem.hItem = hNext;
						SendMessage(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
						if ((tvItem.state & TVIS_SELECTED) != 0)
							deselected = TRUE;
						tvItem.state = 0;
						SendMessage(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
						HTREEITEM hItem = hNext = (HTREEITEM) SendMessage(
								handle,
								TVM_GETNEXTITEM, TVGN_NEXTVISIBLE,
								(LPARAM) hNext);
						while (hItem != 0 && hItem != lpht.hItem) {
							hItem = (HTREEITEM) SendMessageW(handle,
							TVM_GETNEXTITEM,
							TVGN_PARENT, (LPARAM) hItem);
						}
						if (hItem == 0)
							break;
					}
				}
			}
		}
		_W_TREEVIEW(widget)->dragStarted =
		_W_TREEVIEW(widget)->gestureCompleted = FALSE;
		if (fixSelection) {
			_W_TREEVIEW(widget)->hSelect = lpht.hItem;
			_W_TREEVIEW(widget)->ignoreDeselect =
			_W_TREEVIEW(widget)->ignoreSelect =
			_W_TREEVIEW(widget)->lockSelection = TRUE;
		}
		HWND lastHWND = e->hwnd;
		e->hwnd = handle;
		result = priv->widget.call_window_proc(widget, e, priv);
		e->hwnd = lastHWND;
		if (WIN32_VERSION >= VERSION(6, 0)) {
			if (GetFocus() != handle)
				SetFocus(handle);
		}
		if (fixSelection) {
			_W_TREEVIEW(widget)->hSelect = 0;
			_W_TREEVIEW(widget)->ignoreDeselect =
			_W_TREEVIEW(widget)->ignoreSelect =
			_W_TREEVIEW(widget)->lockSelection = FALSE;
		}
		HTREEITEM hNewSelection = (HTREEITEM) SendMessage(handle,
		TVM_GETNEXTITEM,
		TVGN_CARET, 0);
		if (hOldSelection != hNewSelection)
			_W_TREEVIEW(widget)->hAnchor = hNewSelection;
		if (_W_TREEVIEW(widget)->dragStarted) {
			if (!win_toolkit->captureChanged && w_widget_is_ok(widget) > 0) {
				if (GetCapture() != handle)
					SetCapture(handle);
			}
		}
		/*
		 * Bug in Windows.  When a tree has no images and an item is
		 * expanded or collapsed, for some reason, Windows changes
		 * the size of the selection.  When the user expands a tree
		 * item, the selection rectangle is made a few pixels larger.
		 * When the user collapses an item, the selection rectangle
		 * is restored to the original size but the selection is not
		 * redrawn, causing pixel corruption.  The fix is to detect
		 * this case and redraw the item.
		 */
		if ((lpht.flags & TVHT_ONITEMBUTTON) != 0) {
			int bits = GetWindowLong(handle, GWL_STYLE);
			if ((bits & TVS_FULLROWSELECT) == 0) {
				if (SendMessage(handle, TVM_GETIMAGELIST, TVSIL_NORMAL, 0)
						== 0) {
					HTREEITEM hItem = (HTREEITEM) SendMessage(handle,
					TVM_GETNEXTITEM,
					TVGN_CARET, 0);
					if (hItem != 0) {
						RECT rect;
						if (TreeView_GetItemRect(handle, hItem, &rect, FALSE)) {
							InvalidateRect(handle, &rect, TRUE);
						}
					}
				}
			}
		}
		if (deselected) {
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_SELECTION;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.detail = 0;
			event.item = W_LISTITEM(&item);
			W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
			_W_ITEM(&item)->parent = widget;
			_W_ITEM(&item)->index = -1;
			_W_TREEITEM(&item)->htreeitem = lpht.hItem;
			_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
		}
		return result;
	}

	/* Look for check/uncheck */
	if ((style & W_CHECK) != 0) {
		if ((lpht.flags & TVHT_ONITEMSTATEICON) != 0) {
			win_toolkit->captureChanged = FALSE;
			mouseevent.event.type = W_EVENT_MOUSEDOWN;
			mouseevent.event.time = 0;
			mouseevent.event.platform_event = (w_event_platform*) e;
			mouseevent.event.widget = widget;
			mouseevent.event.data = 0;
			mouseevent.button = 1;
			mouseevent.clickcount = 0;
			mouseevent.detail = 0;
			mouseevent.x = GET_X_LPARAM(e->lparam);
			mouseevent.y = GET_Y_LPARAM(e->lparam);
			_w_set_input_state((w_event*) &mouseevent);
			result = _w_widget_post_event(widget, (w_event*) &mouseevent,W_EVENT_SEND);
			if (result) {
				if (!win_toolkit->captureChanged
						&& w_widget_is_ok(widget) > 0) {
					if (GetCapture() != handle)
						SetCapture(handle);
				}
				e->result = 0;
				return W_TRUE;
			}
			if (!win_toolkit->captureChanged && w_widget_is_ok(widget) > 0) {
				if (GetCapture() != handle)
					SetCapture(handle);
			}
			SetFocus(handle);
			TVITEM tvItem;
			tvItem.hItem = lpht.hItem;
			tvItem.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_STATE;
			tvItem.stateMask = TVIS_STATEIMAGEMASK;
			SendMessage(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
			ITEM_STATE_SELECTED(tvItem.state);
			SendMessage(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
			LONG_PTR id = (LONG_PTR) tvItem.hItem;
			if (_COMCTL32_VERSION >= VERSION(6, 0)) {
				id = SendMessageW(handle, TVM_MAPHTREEITEMTOACCID,
						(WPARAM) tvItem.hItem, 0);
			}
			NotifyWinEvent(EVENT_OBJECT_FOCUS, handle, OBJID_CLIENT, id);
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_SELECTION;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.detail = W_CHECK;
			event.item = W_LISTITEM(&item);
			W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
			_W_ITEM(&item)->parent = widget;
			_W_ITEM(&item)->index = -1;
			_W_TREEITEM(&item)->htreeitem = tvItem.hItem;
			_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
			e->result = 0;
			return W_TRUE;
		}
	}

	/*
	 * Feature in Windows.  When the tree has the style
	 * TVS_FULLROWSELECT, the background color for the
	 * entire row is filled when an item is painted,
	 * drawing on top of any custom drawing.  The fix
	 * is to emulate TVS_FULLROWSELECT.
	 */
	boolean selected = FALSE;
	boolean fakeSelection = FALSE;
	if (lpht.hItem != 0) {
		if ((style & W_FULL_SELECTION) != 0) {
			int bits = GetWindowLong(handle, GWL_STYLE);
			if ((bits & TVS_FULLROWSELECT) == 0)
				fakeSelection = TRUE;
		} else {
			if (style & W_CUSTOMDRAW) {
				/*selected = hitTestSelection (lpht.hItem, lpht.x, lpht.y);
				 if (selected) {
				 if ((lpht.flags & TVHT_ONITEM) == 0) fakeSelection = true;
				 }*/
			}
		}
	}

	/* Process the mouse when an item is not selected */
	if (!selected && (style & W_FULL_SELECTION) == 0) {
		if ((lpht.flags & TVHT_ONITEM) == 0) {
			win_toolkit->captureChanged = FALSE;
			mouseevent.event.type = W_EVENT_MOUSEDOWN;
			mouseevent.event.time = 0;
			mouseevent.event.platform_event = (w_event_platform*) e;
			mouseevent.event.widget = widget;
			mouseevent.event.data = 0;
			mouseevent.button = 1;
			mouseevent.clickcount = 0;
			mouseevent.detail = 0;
			mouseevent.x = GET_X_LPARAM(e->lparam);
			mouseevent.y = GET_Y_LPARAM(e->lparam);
			_w_set_input_state((w_event*) &mouseevent);
			result = _w_widget_post_event(widget, (w_event*) &mouseevent,W_EVENT_SEND);
			if (result) {
				if (!win_toolkit->captureChanged
						&& w_widget_is_ok(widget) > 0) {
					if (GetCapture() != handle)
						SetCapture(handle);
				}
				e->result = 0;
				return W_TRUE;
			}
			HWND lastHWND = e->hwnd;
			e->hwnd = handle;
			result = priv->widget.call_window_proc(widget, e, priv);
			e->hwnd = lastHWND;
			if (WIN32_VERSION >= VERSION(6, 0)) {
				if (GetFocus() != handle)
					SetFocus(handle);
			}
			if (!win_toolkit->captureChanged && w_widget_is_ok(widget) > 0) {
				if (GetCapture() != handle)
					SetCapture(handle);
			}
			return result;
		}
	}

	/* Get the selected state of the item under the mouse */
	TVITEM tvItem;
	tvItem.mask = TVIF_HANDLE | TVIF_STATE;
	tvItem.stateMask = TVIS_SELECTED;
	boolean hittestSelected = FALSE;
	if ((style & W_MULTI) != 0) {
		tvItem.hItem = lpht.hItem;
		SendMessage(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
		hittestSelected = (tvItem.state & TVIS_SELECTED) != 0;
	}

	/* Get the selected state of the last selected item */
	HTREEITEM hOldItem = (HTREEITEM) SendMessage(handle, TVM_GETNEXTITEM,
	TVGN_CARET, 0);
	if ((style & W_MULTI) != 0) {
		tvItem.hItem = hOldItem;
		SendMessage(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);

		/* Check for CONTROL or drag selection */
		if (hittestSelected || (e->wparam & MK_CONTROL) != 0) {
			/*
			 * Feature in Windows.  When the tree is not drawing focus
			 * and the user selects a tree item while the CONTROL key
			 * is down, the tree window proc sends WM_UPDATEUISTATE
			 * to the top level window, causing controls within the shell
			 * to redraw.  When drag detect is enabled, the tree window
			 * proc runs a modal loop that allows WM_PAINT messages to be
			 * delivered during WM_LBUTTONDOWN.  When WM_SETREDRAW is used
			 * to disable drawing for the tree and a WM_PAINT happens for
			 * a parent of the tree (or a sibling that overlaps), the parent
			 * will draw on top of the tree.  If WM_SETREDRAW is turned back
			 * on without redrawing the entire tree, pixel corruption occurs.
			 * This case only seems to happen when the tree has been given
			 * focus from WM_MOUSEACTIVATE of the shell.  The fix is to
			 * force the WM_UPDATEUISTATE to be sent before disabling
			 * the drawing.
			 *
			 * NOTE:  Any redraw of a parent (or sibling) will be dispatched
			 * during the modal drag detect loop.  This code only fixes the
			 * case where the tree causes a redraw from WM_UPDATEUISTATE.
			 * In SWT, the InvalidateRect() that caused the pixel corruption
			 * is found in Composite.WM_UPDATEUISTATE().
			 */
			int uiState = (int) /*64*/SendMessage(handle, WM_QUERYUISTATE, 0,
					0);
			if ((uiState & UISF_HIDEFOCUS) != 0) {
				SendMessage(handle, WM_CHANGEUISTATE, UIS_INITIALIZE, 0);
			}
			UpdateWindow(handle);
			DefWindowProc(handle, WM_SETREDRAW, 0, 0);
		} else {
			//deselectAll ();
		}
	}

	/* Do the selection */
	win_toolkit->captureChanged = FALSE;
	mouseevent.event.type = W_EVENT_MOUSEDOWN;
	mouseevent.event.time = 0;
	mouseevent.event.platform_event = (w_event_platform*) e;
	mouseevent.event.widget = widget;
	mouseevent.event.data = 0;
	mouseevent.button = 1;
	mouseevent.clickcount = 0;
	mouseevent.detail = 0;
	mouseevent.x = GET_X_LPARAM(e->lparam);
	mouseevent.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &mouseevent);
	result = _w_widget_post_event(widget, (w_event*) &mouseevent,W_EVENT_SEND);
	if (result) {
		if (!win_toolkit->captureChanged && w_widget_is_ok(widget) > 0) {
			if (GetCapture() != handle)
				SetCapture(handle);
		}
		e->result = 0;
		return W_TRUE;
	}
	_W_TREEVIEW(widget)->hSelect = lpht.hItem;
	_W_TREEVIEW(widget)->dragStarted = _W_TREEVIEW(widget)->gestureCompleted =
	FALSE;
	_W_TREEVIEW(widget)->ignoreDeselect = _W_TREEVIEW(widget)->ignoreSelect =
	TRUE;
	HWND lastHWND = e->hwnd;
	e->hwnd = handle;
	result = priv->widget.call_window_proc(widget, e, priv);
	e->hwnd = lastHWND;
	if (WIN32_VERSION >= VERSION(6, 0)) {
		if (GetFocus() != handle)
			SetFocus(handle);
	}
	HTREEITEM hNewItem = (HTREEITEM) SendMessage(handle, TVM_GETNEXTITEM,
	TVGN_CARET, 0);
	if (fakeSelection) {
		if (hOldItem == 0 || (hNewItem == hOldItem && lpht.hItem != hOldItem)) {
			SendMessage(handle, TVM_SELECTITEM, TVGN_CARET,
					(LPARAM) lpht.hItem);
			hNewItem = (HTREEITEM) SendMessage(handle, TVM_GETNEXTITEM,
			TVGN_CARET, 0);
		}
		if (!_W_TREEVIEW(widget)->dragStarted
				&& (_W_WIDGET(widget)->state & STATE_DRAG_DETECT) != 0) {
			//_W_TREEVIEW(widget)->dragStarted = dragDetect (handle, lpht.x, lpht.y, false, null, null);
		}
	}
	_W_TREEVIEW(widget)->ignoreDeselect = _W_TREEVIEW(widget)->ignoreSelect =
	FALSE;
	_W_TREEVIEW(widget)->hSelect = 0;
	if (_W_TREEVIEW(widget)->dragStarted) {
		if (!win_toolkit->captureChanged && w_widget_is_ok(widget) > 0) {
			if (GetCapture() != handle)
				SetCapture(handle);
		}
	}

	/*
	 * Feature in Windows.  When the old and new focused item
	 * are the same, Windows does not check to make sure that
	 * the item is actually selected, not just focused.  The
	 * fix is to force the item to draw selected by setting
	 * the state mask.  This is only necessary when the tree
	 * is single select.
	 */
	if ((style & W_SINGLE) != 0) {
		if (hOldItem == hNewItem) {
			tvItem.mask = TVIF_HANDLE | TVIF_STATE;
			tvItem.state = TVIS_SELECTED;
			tvItem.stateMask = TVIS_SELECTED;
			tvItem.hItem = hNewItem;
			SendMessage(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
		}
	}

	/* Reselect the last item that was unselected */
	if ((style & W_MULTI) != 0) {

		/* Check for CONTROL and reselect the last item */
		if (hittestSelected || (e->wparam & MK_CONTROL) != 0) {
			if (hOldItem == hNewItem && hOldItem == lpht.hItem) {
				if ((e->wparam & MK_CONTROL) != 0) {
					tvItem.state ^= TVIS_SELECTED;
					if (_W_TREEVIEW(widget)->dragStarted)
						tvItem.state = TVIS_SELECTED;
					SendMessage(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
				}
			} else {
				if ((tvItem.state & TVIS_SELECTED) != 0) {
					tvItem.state = TVIS_SELECTED;
					SendMessage(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
				}
				if ((e->wparam & MK_CONTROL) != 0
						&& !_W_TREEVIEW(widget)->dragStarted) {
					if (hittestSelected) {
						tvItem.state = 0;
						tvItem.hItem = lpht.hItem;
						SendMessage(handle, TVM_SETITEM, 0, (LPARAM) &tvItem);
					}
				}
			}
			RECT rect1, rect2;
			boolean fItemRect = (style & W_FULL_SELECTION) == 0;
			if (style & W_CUSTOMDRAW)
				fItemRect = FALSE;
			if (WIN32_VERSION >= VERSION(6, 0))
				fItemRect = FALSE;
			TreeView_GetItemRect(handle, hOldItem, &rect1, fItemRect);
			TreeView_GetItemRect(handle, hNewItem, &rect2, fItemRect);
			DefWindowProc(handle, WM_SETREDRAW, 1, 0);
			InvalidateRect(handle, &rect1, TRUE);
			InvalidateRect(handle, &rect2, TRUE);
			UpdateWindow(handle);
		}

		/* Check for SHIFT or normal select and deselect/reselect items */
		if ((e->wparam & MK_CONTROL) == 0) {
			if (!hittestSelected || !_W_TREEVIEW(widget)->dragStarted) {
				/*tvItem.state = 0;
				 WNDPROC oldProc = GetWindowLongPtr (handle, GWLP_WNDPROC);
				 SetWindowLongPtr (handle, GWLP_WNDPROC, TreeProc);
				 if ((style & W_VIRTUAL) != 0) {
				 HTREEITEM hItem = SendMessage (handle, TVM_GETNEXTITEM, TVGN_ROOT, 0);
				 deselect (hItem, tvItem, hNewItem);
				 } else {
				 for (int i=0; i<items.length; i++) {
				 TreeItem item = items [i];
				 if (item != null && item.handle != hNewItem) {
				 tvItem.hItem = item.handle;
				 SendMessage (handle, TVM_SETITEM, 0,(LPARAM) &tvItem);
				 }
				 }
				 }
				 tvItem.hItem = hNewItem;
				 tvItem.state = TVIS_SELECTED;
				 SendMessageW (handle, TVM_SETITEM, 0,(LPARAM) &tvItem);
				 SetWindowLongPtr (handle, GWLP_WNDPROC,(LONG_PTR) oldProc);*/
				if ((e->wparam & MK_SHIFT) != 0) {
					RECT rect1;
					if (_W_TREEVIEW(widget)->hAnchor == 0)
						_W_TREEVIEW(widget)->hAnchor = hNewItem;
					if (TreeView_GetItemRect(handle,
							_W_TREEVIEW(widget)->hAnchor, &rect1, FALSE)) {
						RECT rect2;
						if (TreeView_GetItemRect(handle, hNewItem, &rect2,
								FALSE)) {
							int flags =
									rect1.top < rect2.top ?
											TVGN_NEXTVISIBLE :
											TVGN_PREVIOUSVISIBLE;
							tvItem.state = TVIS_SELECTED;
							HTREEITEM hItem = tvItem.hItem =
							_W_TREEVIEW(widget)->hAnchor;
							SendMessageW(handle, TVM_SETITEMW, 0,
									(LPARAM) &tvItem);
							while (hItem != hNewItem) {
								tvItem.hItem = hItem;
								SendMessageW(handle, TVM_SETITEMW, 0,
										(LPARAM) &tvItem);
								hItem = (HTREEITEM) SendMessageW(handle,
								TVM_GETNEXTITEM, flags, (LPARAM) hItem);
							}
						}
					}
				}
			}
		}
	}
	if ((e->wparam & MK_SHIFT) == 0)
		_W_TREEVIEW(widget)->hAnchor = hNewItem;

	/* Issue notification */
	if (!_W_TREEVIEW(widget)->gestureCompleted) {
		tvItem.hItem = hNewItem;
		tvItem.mask = TVIF_HANDLE | TVIF_PARAM;
		SendMessage(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_SELECTION;
		event.event.widget = widget;
		event.event.platform_event = (w_event_platform*) e;
		event.detail = 0;
		event.item = W_LISTITEM(&item);
		W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
		_W_ITEM(&item)->parent = widget;
		_W_ITEM(&item)->index = -1;
		_W_TREEITEM(&item)->htreeitem = tvItem.hItem;
		_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
	}
	_W_TREEVIEW(widget)->gestureCompleted = FALSE;

	/*
	 * Feature in Windows.  Inside WM_LBUTTONDOWN and WM_RBUTTONDOWN,
	 * the widget starts a modal loop to determine if the user wants
	 * to begin a drag/drop operation or marquee select.  Unfortunately,
	 * this modal loop eats the corresponding mouse up.  The fix is to
	 * detect the cases when the modal loop has eaten the mouse up and
	 * issue a fake mouse up.
	 */
	if (_W_TREEVIEW(widget)->dragStarted) {
		//sendDragEvent (1, GET_X_LPARAM (e->lparam), GET_Y_LPARAM (e->lparam));
	} else {
		int bits = GetWindowLong(handle, GWL_STYLE);
		if ((bits & TVS_DISABLEDRAGDROP) == 0) {
			//sendMouseEvent (SWT.MouseUp, 1, handle, WM_LBUTTONUP, wParam, lParam);
		}
	}
	_W_TREEVIEW(widget)->dragStarted = FALSE;
	return result;
}
wresult _TREEVIEW_WM_MOUSEMOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_MOUSEMOVE(widget, e, priv);
	if (result)
		return result;
	HWND itemToolTipHandle = _W_TREEVIEW(widget)->itemToolTipHandle;
	if (itemToolTipHandle != 0) {
		/*
		 * Bug in Windows.  On some machines that do not have XBUTTONs,
		 * the MK_XBUTTON1 and MK_XBUTTON2 bits are sometimes set,
		 * causing mouse capture to become stuck.  The fix is to test
		 * for the extra buttons only when they exist.
		 */
		int mask = MK_LBUTTON | MK_MBUTTON | MK_RBUTTON;
		if (win_toolkit->xMouse)
			mask |= MK_XBUTTON1 | MK_XBUTTON2;
		if ((e->wparam & mask) == 0) {
			int x = GET_X_LPARAM(e->lparam);
			int y = GET_Y_LPARAM(e->lparam);
			int index = -1;
			w_listitem item;
			RECT cellRect, itemRect;
			if (_w_treeview_find_cell(W_TREEVIEW(widget), x, y, &item, &index,
					&cellRect, &itemRect)) {
				/*
				 * Feature in Windows.  When the new tool rectangle is
				 * set using TTM_NEWTOOLRECT and the tooltip is visible,
				 * Windows draws the tooltip right away and the sends
				 * WM_NOTIFY with TTN_SHOW.  This means that the tooltip
				 * shows first at the wrong location and then moves to
				 * the right one.  The fix is to hide the tooltip window.
				 */
				if (SendMessage(itemToolTipHandle, TTM_GETCURRENTTOOL, 0, 0)
						== 0) {
					if (IsWindowVisible(itemToolTipHandle)) {
						ShowWindow(itemToolTipHandle, SW_HIDE);
					}
				}
				HWND handle = _W_WIDGET(widget)->handle;
				TOOLINFOW lpti;
				lpti.cbSize = sizeof(lpti);
				lpti.hwnd = handle;
				//lpti.uId = handle;
				lpti.uFlags = TTF_SUBCLASS | TTF_TRANSPARENT;
				lpti.rect.left = cellRect.left;
				lpti.rect.top = cellRect.top;
				lpti.rect.right = cellRect.right;
				lpti.rect.bottom = cellRect.bottom;
				SendMessageW(itemToolTipHandle, TTM_NEWTOOLRECT, 0,
						(LPARAM) &lpti);
			}
		}
	}
	return result;
}
wresult _TREEVIEW_WM_MOUSEWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _SCROLLABLE_WM_MOUSEWHEEL(widget, e, priv);
	if (_W_TREEVIEW(widget)->itemToolTipHandle != 0)
		ShowWindow(_W_TREEVIEW(widget)->itemToolTipHandle, SW_HIDE);
	return result;
}
wresult _TREEVIEW_WM_MOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_TREEVIEW(widget)->itemToolTipHandle != 0)
		ShowWindow(_W_TREEVIEW(widget)->itemToolTipHandle, SW_HIDE);
	if (_W_TREEVIEW(widget)->ignoreResize)
		return W_FALSE;
	return _CONTROL_WM_MOVE(widget, e, priv);
}
wresult _TREEVIEW_WM_RBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result;
	HWND handle = _W_WIDGET(widget)->handle;
	wuint64 style = _W_WIDGET(widget)->style;
	w_event_mouse mouseevent;
	/*
	 * Feature in Windows.  The receiver uses WM_RBUTTONDOWN
	 * to initiate a drag/drop operation depending on how the
	 * user moves the mouse.  If the user clicks the right button,
	 * without moving the mouse, the tree consumes the corresponding
	 * WM_RBUTTONUP.  The fix is to avoid calling the window proc for
	 * the tree.
	 */
	win_toolkit->captureChanged = FALSE;
	mouseevent.event.type = W_EVENT_MOUSEDOWN;
	mouseevent.event.time = 0;
	mouseevent.event.platform_event = (w_event_platform*) e;
	mouseevent.event.widget = widget;
	mouseevent.event.data = 0;
	mouseevent.button = 3;
	mouseevent.clickcount = 0;
	mouseevent.detail = 0;
	mouseevent.x = GET_X_LPARAM(e->lparam);
	mouseevent.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &mouseevent);
	result = _w_widget_post_event(widget, (w_event*) &mouseevent,W_EVENT_SEND);
	if (!result) {
		if (!win_toolkit->captureChanged && w_widget_is_ok(widget) > 0) {
			if (GetCapture() != handle)
				SetCapture(handle);
		}
		e->result = 0;
		return W_TRUE;
	}
	/*
	 * This code is intentionally commented.
	 */
//	if (GetCapture () != handle) SetCapture (handle);
	if (GetFocus() != handle)
		SetFocus(handle);

	/*
	 * Feature in Windows.  When the user selects a tree item
	 * with the right mouse button, the item remains selected
	 * only as long as the user does not release or move the
	 * mouse.  As soon as this happens, the selection snaps
	 * back to the previous selection.  This behavior can be
	 * observed in the Explorer but is not instantly apparent
	 * because the Explorer explicitly sets the selection when
	 * the user chooses a menu item.  If the user cancels the
	 * menu, the selection snaps back.  The fix is to avoid
	 * calling the window proc and do the selection ourselves.
	 * This behavior is consistent with the table.
	 */
	TVHITTESTINFO lpht;
	lpht.pt.x = GET_X_LPARAM(e->lparam);
	lpht.pt.y = GET_Y_LPARAM(e->lparam);
	SendMessage(handle, TVM_HITTEST, 0, (LPARAM) &lpht);
	if (lpht.hItem != 0) {
		boolean fakeSelection = (style & W_FULL_SELECTION) != 0;
		if (!fakeSelection) {
			if (style & W_CUSTOMDRAW) {
				//fakeSelection = hitTestSelection (lpht.hItem, lpht.x, lpht.y);
			} else {
				int flags = TVHT_ONITEMICON | TVHT_ONITEMLABEL;
				fakeSelection = (lpht.flags & flags) != 0;
			}
		}
		if (fakeSelection) {
			if ((e->wparam & (MK_CONTROL | MK_SHIFT)) == 0) {
				TVITEM tvItem;
				tvItem.mask = TVIF_HANDLE | TVIF_STATE;
				tvItem.stateMask = TVIS_SELECTED;
				tvItem.hItem = lpht.hItem;
				SendMessage(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
				if ((tvItem.state & TVIS_SELECTED) == 0) {
					_W_TREEVIEW(widget)->ignoreSelect = TRUE;
					SendMessage(handle, TVM_SELECTITEM, TVGN_CARET, 0);
					_W_TREEVIEW(widget)->ignoreSelect = FALSE;
					SendMessage(handle, TVM_SELECTITEM, TVGN_CARET,
							(LPARAM) lpht.hItem);
				}
			}
		}
	}
	e->result = 0;
	return W_TRUE;
}
wresult _TREEVIEW_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((_W_WIDGET(widget)->state & STATE_DISPOSE_SENT) != 0) {
		e->result = 0;
		return W_TRUE;
	}
	if ((_W_WIDGET(widget)->style & W_DOUBLE_BUFFERED) != 0
			|| priv->find_image_control(W_CONTROL(widget), priv) != 0) {
		HWND handle = _W_WIDGET(widget)->handle;
		int doubleBuffer = TRUE;
		if (_W_TREEVIEW(widget)->explorerTheme) {
			DWORD exStyle = SendMessageW(handle, TVM_GETEXTENDEDSTYLE, 0, 0);
			if ((exStyle & TVS_EX_DOUBLEBUFFER) != 0)
				doubleBuffer = FALSE;
		}
		if (doubleBuffer) {
			_w_graphics gc;
			HDC paintDC = 0;
			PAINTSTRUCT ps;
			int hooksPaint = TRUE;
			if (hooksPaint) {
				paintDC = BeginPaint(handle, &ps);
				_w_graphics_init(W_GRAPHICS(&gc), paintDC);
				_W_GRAPHICS(&gc)->hwnd = handle;
				_W_GRAPHICS(&gc)->ps = &ps;
			} else {
				paintDC = BeginPaint(handle, &ps);
			}
			int width = ps.rcPaint.right - ps.rcPaint.left;
			int height = ps.rcPaint.bottom - ps.rcPaint.top;
			if (width != 0 && height != 0) {
				HDC hDC = CreateCompatibleDC(paintDC);
				POINT lpPoint1, lpPoint2;
				SetWindowOrgEx(hDC, ps.rcPaint.left, ps.rcPaint.top, &lpPoint1);
				SetBrushOrgEx(hDC, ps.rcPaint.left, ps.rcPaint.top, &lpPoint2);
				HBITMAP hBitmap = CreateCompatibleBitmap(paintDC, width,
						height);
				HBITMAP hOldBitmap = SelectObject(hDC, hBitmap);
				priv->draw_background(W_CONTROL(widget), hDC, &ps.rcPaint, -1,
						0, 0, priv);
				WPARAM lastParam = e->wparam;
				e->wparam = (WPARAM) hDC;
				priv->widget.call_window_proc(widget, e, priv);
				e->wparam = lastParam;
				SetWindowOrgEx(hDC, lpPoint1.x, lpPoint1.y, NULL);
				SetBrushOrgEx(hDC, lpPoint2.x, lpPoint2.y, NULL);
				BitBlt(paintDC, ps.rcPaint.left, ps.rcPaint.top, width, height,
						hDC, 0, 0, SRCCOPY);
				SelectObject(hDC, hOldBitmap);
				DeleteObject(hBitmap);
				DeleteObject(hDC);
				if (hooksPaint) {
					w_event_paint event;
					event.event.type = W_EVENT_PAINT;
					event.event.platform_event = (w_event_platform*) e;
					event.event.widget = widget;
					event.event.data = 0;
					event.bounds.x = ps.rcPaint.left;
					event.bounds.y = ps.rcPaint.top;
					event.bounds.width = ps.rcPaint.right - ps.rcPaint.left;
					event.bounds.height = ps.rcPaint.bottom - ps.rcPaint.top;
					event.gc = W_GRAPHICS(&gc);
					_w_widget_post_event(widget, (w_event*) &event,W_EVENT_SEND);
				}
			}
			if (hooksPaint) {
				w_graphics_dispose(W_GRAPHICS(&gc));
				EndPaint(handle, &ps);
			} else {
				EndPaint(handle, &ps);
			}
			e->result = 0;
			return W_TRUE;
		}
	}
	return _COMPOSITE_WM_PAINT(widget, e, priv);
}
wresult _TREEVIEW_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _CONTROL_WM_SETCURSOR(widget, e, priv);
	if (result)
		return result;

	/*
	 * Feature in Windows. On Windows 7, the tree control show the
	 * hand cursor when the mouse is over an item.  This is the
	 * correct Windows 7 behavior but not correct for SWT. The fix
	 * is to always ensure a cursor is set.
	 */
	if (WIN32_VERSION >= VERSION(6, 1)) {
		if (e->wparam == (WPARAM) _W_WIDGET(widget)->handle) {
			int hitTest = LOWORD(e->lparam);
			if (hitTest == HTCLIENT) {
				SetCursor(LoadCursor(0, IDC_ARROW));
				e->result = 1;
				return W_TRUE;
			}
		}
	}
	return W_FALSE;
}
wresult _TREEVIEW_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Bug in Windows.  When a tree item that has an image
	 * with alpha is expanded or collapsed, the area where
	 * the image is drawn is not erased before it is drawn.
	 * This means that the image gets darker each time.
	 * The fix is to redraw the selection.
	 *
	 * Feature in Windows.  When multiple item have
	 * the TVIS_SELECTED state, Windows redraws only
	 * the focused item in the color used to show the
	 * selection when the tree loses or gains focus.
	 * The fix is to force Windows to redraw the
	 * selection when focus is gained or lost.
	 */
	int redraw = (_W_WIDGET(widget)->style & W_MULTI) != 0;
	if (!redraw) {
		if (_COMCTL32_VERSION >= VERSION(6, 0)) {
			if (_W_LISTVIEWBASE(widget)->imagelist != 0) {
				int bits = GetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE);
				if ((bits & TVS_FULLROWSELECT) == 0) {
					redraw = TRUE;
				}
			}
		}
	}
	//if (redraw) redrawSelection ();
	return _WIDGET_WM_SETFOCUS(widget, e, priv);
}
wresult _TREEVIEW_WM_SETFONT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_SETFONT(widget, e, priv);
	if (result)
		return result;
	HWND hwndHeader = _W_TREEVIEW(widget)->hwndHeader;
	if (_W_TREEVIEW(widget)->hwndHeader != 0) {
		/*
		 * Bug in Windows.  When a header has a sort indicator
		 * triangle, Windows resizes the indicator based on the
		 * size of the n-1th font.  The fix is to always make
		 * the n-1th font be the default.  This makes the sort
		 * indicator always be the default size.
		 */
		SendMessage(hwndHeader, WM_SETFONT, 0, e->lparam);
		SendMessage(hwndHeader, WM_SETFONT, e->wparam, e->lparam);
	}
	HWND itemToolTipHandle = _W_TREEVIEW(widget)->itemToolTipHandle;
	if (itemToolTipHandle != 0) {
		ShowWindow(itemToolTipHandle, SW_HIDE);
		SendMessage(itemToolTipHandle, WM_SETFONT, e->wparam, e->lparam);
	}
	HWND headerToolTipHandle = _W_LISTVIEWBASE(widget)->headerToolTipHandle;
	if (headerToolTipHandle != 0) {
		SendMessage(headerToolTipHandle, WM_SETFONT, e->wparam, e->lparam);
		//updateHeaderToolTips ();
	}
	return result;
}
wresult _TREEVIEW_WM_SETREDRAW(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HWND itemToolTipHandle = _W_TREEVIEW(widget)->itemToolTipHandle;
	if (itemToolTipHandle != 0)
		ShowWindow(itemToolTipHandle, SW_HIDE);
	/*
	 * Bug in Windows.  Under certain circumstances, when
	 * WM_SETREDRAW is used to turn off drawing and then
	 * TVM_GETITEMRECT is sent to get the bounds of an item
	 * that is not inside the client area, Windows segment
	 * faults.  The fix is to call the default window proc
	 * rather than the default tree proc.
	 *
	 * NOTE:  This problem is intermittent and happens on
	 * Windows Vista running under the theme manager.
	 */
	if (WIN32_VERSION >= VERSION(6, 0)) {
		e->result = DefWindowProcW(_W_WIDGET(widget)->handle, WM_SETREDRAW,
				e->wparam, e->lparam);
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _TREEVIEW_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HWND itemToolTipHandle = _W_TREEVIEW(widget)->itemToolTipHandle;
	if (itemToolTipHandle != 0)
		ShowWindow(itemToolTipHandle, SW_HIDE);
	HWND handle = _W_WIDGET(widget)->handle;
	/*
	 * Bug in Windows.  When TVS_NOHSCROLL is set when the
	 * size of the tree is zero, the scroll bar is shown the
	 * next time the tree resizes.  The fix is to hide the
	 * scroll bar every time the tree is resized.
	 */
	int bits = GetWindowLong(handle, GWL_STYLE);
	if ((bits & TVS_NOHSCROLL) != 0) {
		ShowScrollBar(handle, SB_HORZ, FALSE);
	}
	/*
	 * Bug in Windows.  On Vista, when the Explorer theme
	 * is used with a full selection tree, when the tree
	 * is resized to be smaller, the rounded right edge
	 * of the selected items is not drawn.  The fix is the
	 * redraw the entire tree.
	 */
	if (_W_TREEVIEW(widget)->explorerTheme
			&& (_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0) {
		InvalidateRect(handle, 0, FALSE);
	}
	if (_W_TREEVIEW(widget)->ignoreResize)
		return W_FALSE;
	return _COMPOSITE_WM_SIZE(widget, e, priv);
}
wresult _TREEVIEW_WM_SYSCOLORCHANGE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_SYSCOLORCHANGE(widget, e, priv);
	if (result)
		return result;
	/*
	 * Bug in Windows.  When the tree is using the explorer
	 * theme, it does not use COLOR_WINDOW_TEXT for the
	 * default foreground color.  The fix is to explicitly
	 * set the foreground.
	 */
	if (_W_TREEVIEW(widget)->explorerTheme) {
		//if (_W_CONTROL(widget)->foreground == 0) setForegroundPixel (-1);
	}
	//if ((_W_WIDGET(widget)->style & W_CHECK) != 0) setCheckboxImageList ();
	return result;
}
wresult _TREEVIEW_WM_VSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int fixScroll = FALSE;
	if ((_W_WIDGET(widget)->style & W_DOUBLE_BUFFERED) != 0) {
		int code = LOWORD(e->wparam);
		switch (code) {
		case SB_TOP:
		case SB_BOTTOM:
		case SB_LINEDOWN:
		case SB_LINEUP:
		case SB_PAGEDOWN:
		case SB_PAGEUP:
			fixScroll = (_W_WIDGET(widget)->style & (W_VIRTUAL | W_CUSTOMDRAW))
					!= 0;
			break;
		}
	}
	HWND handle = _W_WIDGET(widget)->handle;
	if (fixScroll) {
		_W_WIDGET(widget)->style &= ~W_DOUBLE_BUFFERED;
		if (_W_TREEVIEW(widget)->explorerTheme) {
			SendMessage(handle, TVM_SETEXTENDEDSTYLE, TVS_EX_DOUBLEBUFFER, 0);
		}
	}
	wresult result = _SCROLLABLE_WM_VSCROLL(widget, e, priv);
	if (fixScroll) {
		_W_WIDGET(widget)->style |= W_DOUBLE_BUFFERED;
		if (_W_TREEVIEW(widget)->explorerTheme) {
			SendMessage(handle, TVM_SETEXTENDEDSTYLE, TVS_EX_DOUBLEBUFFER,
			TVS_EX_DOUBLEBUFFER);
		}
	}
	return result;
}
wresult _TREEVIEW_WM_TIMER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _CONTROL_WM_TIMER(widget, e, priv);
	if (result)
		return result;

	/* Bug in Windows. When the expandos are visible (or in process of fading away)
	 * and the tree control is hidden the animation timer does not stop calling the
	 * window proc till the tree is visible again. This can cause performance problems
	 * specially in cases there the application has several tree controls in this state.
	 * The fix is to detect a timer that repeats itself several times when the control
	 * is not visible and stop it. The timer is stopped by sending a fake mouse move event.
	 *
	 * Note: Just killing the timer could cause some internal clean up task related to the
	 * animation not to run.
	 */
	HWND handle = _W_WIDGET(widget)->handle;
	DWORD bits = SendMessageW(handle, TVM_GETEXTENDEDSTYLE, 0, 0);
	if ((bits & TVS_EX_FADEINOUTEXPANDOS) != 0) {
		if (!IsWindowVisible(handle)) {
			/*if (lastTimerID == wParam) {
			 lastTimerCount++;
			 } else {
			 lastTimerCount = 0;
			 }
			 lastTimerID = wParam;
			 if (lastTimerCount >= TIMER_MAX_COUNT) {
			 CallWindowProc (TreeProc, handle, WM_MOUSEMOVE, 0, 0);
			 lastTimerID = -1;
			 lastTimerCount = 0;
			 }*/
		} else {
			/*lastTimerID = -1;
			 lastTimerCount = 0;*/
		}
	}
	return result;
}
wresult _TREEVIEW_WM_CTLCOLORCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (priv->find_image_control(W_CONTROL(widget), priv) != 0) {
		if (_COMCTL32_VERSION < VERSION(6, 0)) {
			//return super.wmColorChild (wParam, lParam);
			return W_FALSE;
		}
		e->result = (LRESULT) GetStockObject(NULL_BRUSH);
	}
	/*
	 * Feature in Windows.  Tree controls send WM_CTLCOLOREDIT
	 * to allow application code to change the default colors.
	 * This is undocumented and conflicts with TVM_SETTEXTCOLOR
	 * and TVM_SETBKCOLOR, the documented way to do this.  The
	 * fix is to ignore WM_CTLCOLOREDIT messages from trees.
	 */
	return W_FALSE;
}
wresult _TREEVIEW_WM_NOTIFY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMHDR *hdr = (NMHDR*) e->lparam;
	if (hdr->hwndFrom == _W_TREEVIEW(widget)->itemToolTipHandle) {
		wresult result = /*wmNotifyToolTip (hdr, wParam, lParam)*/0;
		if (result)
			return result;
	}
	if (hdr->hwndFrom == _W_TREEVIEW(widget)->hwndHeader) {
		wresult result = /*wmNotifyHeader (hdr, wParam, lParam)*/0;
		if (result)
			return result;
	}
	return _COMPOSITE_WM_NOTIFY(widget, e, priv);
}
/*
 * tree notification
 */
wresult _w_treeview_is_item_selected(w_widget *widget, NMTVCUSTOMDRAW *nmcd) {
	wresult selected = W_FALSE;
	HWND handle = _W_WIDGET(widget)->handle;
	if (IsWindowEnabled(handle)) {
		TVITEM tvItem;
		tvItem.mask = TVIF_HANDLE | TVIF_STATE;
		tvItem.hItem = (HTREEITEM) nmcd->nmcd.dwItemSpec;
		SendMessageW(handle, TVM_GETITEM, 0, (LPARAM) &tvItem);
		if ((tvItem.state & (TVIS_SELECTED | TVIS_DROPHILITED)) != 0) {
			selected = W_TRUE;
			/*
			 * Feature in Windows.  When the mouse is pressed and the
			 * selection is first drawn for a tree, the previously
			 * selected item is redrawn but the the TVIS_SELECTED bits
			 * are not cleared.  When the user moves the mouse slightly
			 * and a drag and drop operation is not started, the item is
			 * drawn again and this time with TVIS_SELECTED is cleared.
			 * This means that an item that contains colored cells will
			 * not draw with the correct background until the mouse is
			 * moved.  The fix is to test for the selection colors and
			 * guess that the item is not selected.
			 *
			 * NOTE: This code does not work when the foreground and
			 * background of the tree are set to the selection colors
			 * but this does not happen in a regular application.
			 */
			if (handle == GetFocus()) {
				if (GetTextColor(nmcd->nmcd.hdc)
						!= GetSysColor(COLOR_HIGHLIGHTTEXT)) {
					selected = W_FALSE;
				} else {
					if (GetBkColor(nmcd->nmcd.hdc)
							!= GetSysColor(COLOR_HIGHLIGHT)) {
						selected = W_FALSE;
					}
				}
			}
		} else {
			if (nmcd->nmcd.dwDrawStage == CDDS_ITEMPOSTPAINT) {
				/*
				 * Feature in Windows.  When the mouse is pressed and the
				 * selection is first drawn for a tree, the item is drawn
				 * selected, but the TVIS_SELECTED bits for the item are
				 * not set.  When the user moves the mouse slightly and
				 * a drag and drop operation is not started, the item is
				 * drawn again and this time TVIS_SELECTED is set.  This
				 * means that an item that is in a tree that has the style
				 * TVS_FULLROWSELECT and that also contains colored cells
				 * will not draw the entire row selected until the user
				 * moves the mouse.  The fix is to test for the selection
				 * colors and guess that the item is selected.
				 *
				 * NOTE: This code does not work when the foreground and
				 * background of the tree are set to the selection colors
				 * but this does not happen in a regular application.
				 */
				if (GetTextColor(nmcd->nmcd.hdc)
						== GetSysColor(COLOR_HIGHLIGHTTEXT)) {
					if (GetBkColor(nmcd->nmcd.hdc)
							== GetSysColor(COLOR_HIGHLIGHT)) {
						selected = W_TRUE;
					}
				}
			}
		}
	}
	return selected;
}
wresult _w_treeview_send_measure_item_event(w_widget *widget,
		w_event_list *event, w_rect *bounds, int detail) {
	_w_event_platform *e = (_w_event_platform*) event->event.platform_event;
	NMTVCUSTOMDRAW *nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	HDC hDC = nmcd->nmcd.hdc;
	RECT itemRect;
	int index = _W_ITEM(event->column)->index;
	_w_treeitem_get_bounds_0(W_TREEITEM(event->item), &itemRect, hDC, index,
	TREEVIEW_BOUNDS_GET_TEXT | TREEVIEW_BOUNDS_GET_IMAGE);

	int nSavedDC = SaveDC(hDC);
	w_graphics_init(event->gc);
	if (event->textattr->font != 0) {
		w_graphics_set_font(event->gc, event->textattr->font);
	}
	bounds->x = itemRect.left;
	bounds->y = itemRect.top;
	bounds->width = itemRect.right - itemRect.left;
	bounds->height = itemRect.bottom - itemRect.top;
	event->detail = detail;
	event->textattr = 0;
	event->rect = bounds;
	event->event.type = W_EVENT_ITEM_MEASURE;
	wresult result = w_widget_post_event(widget, (w_event*) &event,W_EVENT_SEND);
	w_graphics_dispose(event->gc);
	RestoreDC(hDC, nSavedDC);
	if (w_widget_is_ok(widget) <= 0)
		return W_FALSE;
	if (result) {
		HWND handle = _W_WIDGET(widget)->handle;
		HWND hwndHeader = _W_TREEVIEW(widget)->hwndHeader;
		if (hwndHeader != 0) {
			int columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
			if (columnCount == 0) {
				if (bounds->x + bounds->width
						> _W_TREEVIEW(widget)->scrollWidth) {
					_W_TREEVIEW(widget)->scrollWidth = bounds->x
							+ bounds->width;
					_w_treeview_set_scroll_width_0(W_TREEVIEW(widget),
					_W_TREEVIEW(widget)->scrollWidth);
				}
			}
		}
		if (bounds->height > SendMessageW(handle, TVM_GETITEMHEIGHT, 0, 0)) {
			SendMessageW(handle, TVM_SETITEMHEIGHT, bounds->height, 0);
		}
	}
	return result;
}
wresult _TREEVIEW_WM_NOTIFY_GETDISPINFO(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMTVDISPINFOW *lptvdi = (NMTVDISPINFOW*) e->lparam;
	int result = W_FALSE;
	HWND handle = _W_WIDGET(widget)->handle;
	_w_treeitem item;
	if ((lptvdi->item.mask & TVIF_TEXT)
			!= 0&& _W_TREEVIEW(widget)->ignoreGetDisp == FALSE) {
		W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
		_W_ITEM(&item)->parent = widget;
		_W_ITEM(&item)->index = -1;
		_W_TREEITEM(&item)->htreeitem = (HTREEITEM) lptvdi->item.hItem;
		w_alloc_buffer buffer;
		buffer.buffer = lptvdi->item.pszText;
		buffer.total_size = 0;
		buffer.size = lptvdi->item.cchTextMax;
		w_item_attr attr;
		attr.alloc = w_alloc_buffer_copy;
		attr.user_data = &buffer;
		attr.enc = W_ENCODING_UNICODE;
		return _w_listitem_get_attr_0(W_LISTITEM(&item), 0,
				W_ITEM_ATTR_MASK_TEXT, &attr, 1);
	}
	return result;
}
wresult _TREEVIEW_WM_NOTIFY_CDDS_PREPAINT(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	if (_W_TREEVIEW(widget)->explorerTheme) {
		HWND handle = _W_WIDGET(widget)->handle;
		int EraseItem = W_TRUE;
		if ((IsWindowEnabled(handle) && EraseItem)
				|| priv->find_image_control(W_CONTROL(widget), priv) != 0) {
			NMTVCUSTOMDRAW *nmcd = (NMTVCUSTOMDRAW*) e->lparam;
			priv->draw_background(W_CONTROL(widget), nmcd->nmcd.hdc,
					&nmcd->nmcd.rc, -1, 0, 0, priv);
		}
	}
	e->result = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
	return W_TRUE;
}
void _w_treeview_draw_init_draw(w_widget *widget, _w_treeview_draw *draw,
		_w_event_platform *e, _w_control_priv *priv) {
	draw->nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	/*
	 * init tree item
	 */
	w_listitem *item = W_LISTITEM(&draw->item);
	W_WIDGETDATA(item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
	_W_ITEM(item)->parent = widget;
	_W_ITEM(item)->index = -1;
	_W_TREEITEM(item)->htreeitem = (HTREEITEM) draw->nmcd->nmcd.dwItemSpec;
	/*
	 * init tree column
	 */
	w_columnitem *column = W_COLUMNITEM(&draw->column);
	W_WIDGETDATA(column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(widget);
	_W_ITEM(column)->parent = widget;
	_W_ITEM(column)->index = 0;
	/*
	 * init event
	 */
	w_event_list *event = &draw->event;
	memset(event, 0, sizeof(w_event_list));
	event->event.type = 0;
	event->event.widget = widget;
	event->event.platform_event = (w_event_platform*) e;
	event->item = W_LISTITEM(item);
	event->column = column;
}
wresult _w_treeview_draw_send_event(w_widget *widget, _w_treeview_draw *draw,
		int type, _w_control_priv *priv) {
	_w_treeview *_tree = _W_TREEVIEW(widget);
	w_graphics *gc = W_GRAPHICS(&draw->gc);
	HDC hDC = draw->hDC;
	w_item_attr *attr = &draw->attr;
	NMTVCUSTOMDRAW *nmcd = draw->nmcd;
	HWND handle = _W_WIDGET(widget)->handle;
	int index = draw->index;
	int is_cdd_prepaint = index == 0 && type == W_EVENT_ITEM_ERASE;
	RECT cellRect;
	_w_treeitem_get_bounds_0(W_TREEITEM(&draw->item), &cellRect, hDC,
			draw->index, TREEVIEW_BOUNDS_ALL);
	if (is_cdd_prepaint) {
		if (draw->clrSortBk != 0) {
			priv->draw_background(W_CONTROL(widget), hDC, &cellRect,
					draw->clrSortBk, 0, 0, priv);
		} else {
			if (IsWindowEnabled(handle)
					|| priv->find_image_control(W_CONTROL(widget), priv) != 0) {
				priv->draw_background(W_CONTROL(widget), hDC, &nmcd->nmcd.rc,
						-1, 0, 0, priv);
			} else {
				priv->fill_background(W_CONTROL(widget), hDC, GetBkColor(hDC),
						&nmcd->nmcd.rc);
			}
		}
	}
	int nSavedDC = SaveDC(hDC);
	_w_graphics_init(W_GRAPHICS(&draw->gc), hDC);
	if (draw->selected && _tree->explorerTheme && is_cdd_prepaint) {
		w_graphics_set_foreground(W_GRAPHICS(&draw->gc),
				GetSysColor(COLOR_WINDOWTEXT));
	} else {
		w_graphics_set_foreground(W_GRAPHICS(gc), GetTextColor(hDC));
	}
	w_graphics_set_background(W_GRAPHICS(&draw->gc), GetBkColor(hDC));
	if (!draw->selected || (!draw->fullSelection)) {
		if (draw->clrText != 0)
			w_graphics_set_foreground(W_GRAPHICS(&draw->gc), draw->clrText);
		if (draw->clrTextBk != 0)
			w_graphics_set_background(W_GRAPHICS(&draw->gc), draw->clrTextBk);
	} else {
		if (_tree->selectionForeground != 0 && type == W_EVENT_ITEM_PAINT)
			w_graphics_set_foreground(W_GRAPHICS(gc),
					_tree->selectionForeground);
	}
	int uiState = SendMessageW(handle, WM_QUERYUISTATE, 0, 0);
	if (uiState & UISF_HIDEFOCUS) {
		_W_GRAPHICS(&draw->gc)->state |= GRAPHICS_STATE_UISF_HIDEFOCUS;
	}
	if (uiState & UISF_HIDEACCEL) {
		_W_GRAPHICS(&draw->gc)->state |= GRAPHICS_STATE_UISF_HIDEACCEL;
	}
	if (draw->attr.font != 0) {
		w_graphics_set_font(W_GRAPHICS(&draw->gc), draw->attr.font);
	}
	draw->event.gc = W_GRAPHICS(&draw->gc);
	draw->event.detail = W_FOREGROUND;
	if (draw->clrTextBk != 0)
		draw->event.detail |= W_BACKGROUND;
	if (index == 0 || draw->fullSelection) {
		if (draw->hot)
			draw->event.detail |= W_HOT;
		if (draw->selected)
			draw->event.detail |= W_SELECTED;
		//if ((nmcd.uItemState & CDIS_FOCUS) != 0) {
		if (!_tree->explorerTheme) {
			if (SendMessageW(handle, TVM_GETNEXTITEM, TVGN_CARET, 0)
					== nmcd->nmcd.dwItemSpec) {
				if (handle == GetFocus()) {
					int uiState = SendMessageW(handle, WM_QUERYUISTATE, 0, 0);
					if ((uiState & UISF_HIDEFOCUS) == 0) {
						if (!_tree->explorerTheme || !draw->selected) {
							draw->focused = TRUE;
							draw->event.detail |= W_FOCUSED;
						}
					}
				}
			}
		}
	}
	draw->r.x = cellRect.left;
	draw->r.y = cellRect.top;
	draw->r.width = cellRect.right - cellRect.left;
	draw->r.height = cellRect.bottom - cellRect.top;
	draw->event.rect = &draw->r;
	//w_graphics_set_clipping_rect(W_GRAPHICS(&draw->gc), &draw->r);
	draw->event.event.type = type;
	draw->event.doit = 1;
	wresult result = w_widget_post_event(widget, W_EVENT(&draw->event),W_EVENT_SEND);
	int newTextClr = w_graphics_get_foreground(W_GRAPHICS(&draw->gc));
	w_graphics_dispose(W_GRAPHICS(&draw->gc));
	RestoreDC(hDC, nSavedDC);
	if (type == W_EVENT_ITEM_ERASE) {
		if (result) {
			if (index == 0)
				_tree->hooksEraseItem = TRUE;
			//if (isDisposed () || item.isDisposed ()) return null;
			if (draw->event.doit) {
				_tree->ignoreDrawForeground =
						(draw->event.detail & W_FOREGROUND) == 0;
				_tree->ignoreDrawBackground =
						(draw->event.detail & W_BACKGROUND) == 0;
				if (index == 0 || draw->fullSelection) {
					_tree->ignoreDrawSelection = (draw->event.detail
							& W_SELECTED) == 0;
					_tree->ignoreDrawFocus = (draw->event.detail & W_FOCUSED)
							== 0;
					_tree->ignoreDrawHot = (draw->event.detail & W_HOT) == 0;
				}
			} else {
				_tree->ignoreDrawForeground = TRUE;
				_tree->ignoreDrawBackground = TRUE;
				_tree->ignoreDrawSelection = TRUE;
				_tree->ignoreDrawFocus = TRUE;
				_tree->ignoreDrawHot = TRUE;
			}
			if (draw->selected && _tree->ignoreDrawSelection)
				_tree->ignoreDrawHot = TRUE;
			if (draw->fullSelection || index == 0) {
				int _draw = !draw->selected && !draw->hot;
				if (index == 0 && !_tree->ignoreDrawBackground
						&& draw->clrTextBk != 0) {
					if (!_tree->explorerTheme && draw->selected)
						_draw = !_tree->ignoreDrawSelection;
					if (_draw) {
						RECT *__r, textRect;
						if (_tree->hwndHeader == 0) {
							if (draw->fullSelection) {
								__r = &draw->rect;
							} else {
								_w_treeitem_get_bounds_0(
										W_TREEITEM(&draw->item), &textRect, hDC,
										draw->index,
										TREEVIEW_BOUNDS_GET_TEXT_CLIP);
								if (draw->measureEvent != 0) {
									textRect.right = WMIN(cellRect.right,
											draw->bounds.x
													+ draw->bounds.width);
								}
								__r = &textRect;
							}
						} else {
							__r = &cellRect;
						}
						priv->fill_background(W_CONTROL(widget), hDC,
								draw->clrTextBk, __r);
					}
				}
				if (_tree->ignoreDrawSelection)
					_tree->ignoreFullSelection = TRUE;
				if (!_tree->ignoreDrawSelection || !_tree->ignoreDrawHot) {
					if (_draw) {
						_tree->selectionForeground = GetSysColor(
						COLOR_HIGHLIGHTTEXT);
						if (index == 0)
							draw->clrText = _tree->selectionForeground;
					} else if (index != 0) {
						if (!_tree->explorerTheme) {
							draw->drawBackground = TRUE;
							_tree->ignoreDrawBackground = FALSE;
							HIGHCONTRASTW pvParam;
							pvParam.cbSize = sizeof(pvParam);
							SystemParametersInfoW(SPI_GETHIGHCONTRAST, 0,
									&pvParam, 0);
							if ((handle == GetFocus()
									|| (pvParam.dwFlags & HCF_HIGHCONTRASTON)
											!= 0) && IsWindowEnabled(handle)) {
								draw->clrTextBk = GetSysColor(COLOR_HIGHLIGHT);
							} else {
								draw->clrTextBk = GetSysColor(COLOR_3DFACE);
							}
							if (!_tree->ignoreFullSelection
									&& index == draw->columnCount - 1) {
								SetRect(&draw->tmpRect,
										draw->backgroundRect->left,
										draw->backgroundRect->top,
										nmcd->nmcd.rc.right,
										draw->backgroundRect->bottom);
								draw->backgroundRect = &draw->tmpRect;
							}
						} else {
							RECT pRect;
							SetRect(&pRect, nmcd->nmcd.rc.left,
									nmcd->nmcd.rc.top, nmcd->nmcd.rc.right,
									nmcd->nmcd.rc.bottom);
							if (draw->hwndHeader != 0) {
								if (draw->totalWidth
										> draw->clientRect.right
												- draw->clientRect.left) {
									pRect.left = 0;
									pRect.right = draw->totalWidth;
								} else {
									pRect.left = draw->clientRect.left;
									pRect.right = draw->clientRect.right;
								}
								if (index == draw->columnCount - 1) {
									SetRect(&draw->tmpRect,
											draw->backgroundRect->left,
											draw->backgroundRect->top,
											pRect.right,
											draw->backgroundRect->bottom);
									draw->backgroundRect = &draw->tmpRect;
								}
							}
							HTHEME hTheme = OpenThemeData(handle, L"TREEVIEW");
							int iStateId =
									draw->selected ? TREIS_SELECTED : TREIS_HOT;
							if (GetFocus() != handle && draw->selected
									&& !draw->hot)
								iStateId = TREIS_SELECTEDNOTFOCUS;
							DrawThemeBackground(hTheme, hDC, TVP_TREEITEM,
									iStateId, &pRect, draw->backgroundRect);
							CloseThemeData(hTheme);
						}
					}
					if (index == 0) {
						if (_tree->explorerTheme) {
							if (!draw->fullSelection) {
								RECT pRect, pClipRect;
								_w_treeitem_get_bounds_0(
										W_TREEITEM(&draw->item), &pRect, hDC,
										draw->index,
										TREEVIEW_BOUNDS_GET);
								_w_treeitem_get_bounds_0(
										W_TREEITEM(&draw->item), &pClipRect,
										hDC, draw->index,
										TREEVIEW_BOUNDS_GET_CLIP
												| TREEVIEW_BOUNDS_FULLTEXT);
								if (draw->measureEvent != 0) {
									pRect.right = WMIN(pClipRect.right,
											draw->bounds.x
													+ draw->bounds.width);
								} else {
									pRect.right += TREEVIEW_EXPLORER_EXTRA;
									pClipRect.right += TREEVIEW_EXPLORER_EXTRA;
								}
								pRect.left -= TREEVIEW_EXPLORER_EXTRA;
								pClipRect.left -= TREEVIEW_EXPLORER_EXTRA;
								HTHEME hTheme = OpenThemeData(handle,
										L"TREEVIEW");
								int iStateId =
										draw->selected ?
												TREIS_SELECTED : TREIS_HOT;
								if (GetFocus() != handle && draw->selected
										&& !draw->hot)
									iStateId = TREIS_SELECTEDNOTFOCUS;
								DrawThemeBackground(hTheme, hDC, TVP_TREEITEM,
										iStateId, &pRect, &pClipRect);
								CloseThemeData(hTheme);
							}
						} else {
							/*
							 * Feature in Windows.  When the tree has the style
							 * TVS_FULLROWSELECT, the background color for the
							 * entire row is filled when an item is painted,
							 * drawing on top of any custom drawing.  The fix
							 * is to emulate TVS_FULLROWSELECT.
							 */
							RECT *__r, textRect;
							if (draw->fullSelection) {
								if (_tree->hwndHeader == 0) {
									__r = &draw->rect;
								} else {
									__r = &cellRect;
								}
							} else {
								_w_treeitem_get_bounds_0(
										W_TREEITEM(&draw->item), &textRect, hDC,
										draw->index,
										TREEVIEW_BOUNDS_GET_TEXT_CLIP);
								if (draw->measureEvent != 0) {
									textRect.right = WMIN(cellRect.right,
											draw->bounds.x
													+ draw->bounds.width);
								}
								__r = &textRect;
							}
							priv->fill_background(W_CONTROL(widget), hDC,
									GetBkColor(hDC), __r);
						}
					}
				} else {
					if (draw->selected || draw->hot) {
						_tree->selectionForeground = newTextClr;
						if (index == 0) {
							draw->clrText = newTextClr;
							_tree->ignoreDrawSelection = TRUE;
							_tree->ignoreDrawHot = TRUE;
						}
					}
					if (!_tree->explorerTheme && index != 0) {
						if (draw->clrTextBk == 0 && IsWindowEnabled(handle)) {
							w_control *control = priv->find_background_control(
									W_CONTROL(widget), priv);
							_w_control_priv *cpriv;
							if (control == 0) {
								control = W_CONTROL(widget);
								cpriv = priv;
							} else {
								cpriv = _W_CONTROL_GET_PRIV(control);
							}
							draw->clrTextBk = cpriv->get_background_pixel(
									control, cpriv);
						}
					}
					if (_tree->explorerTheme && index == 0) {
						nmcd->nmcd.uItemState |= CDIS_DISABLED;
						/*
						 * Feature in Windows.  On Vista only, when the text
						 * color is unchanged and an item is asked to draw
						 * disabled, it uses the disabled color.  The fix is
						 * to modify the color so that is it no longer equal.
						 */
						int newColor;
						if (draw->clrText == 0) {
							newColor = priv->get_foreground_pixel(
									W_CONTROL(widget), priv);
						} else {
							newColor = (draw->clrText & 0x00FFFFFF);
						}
						if (nmcd->clrText == newColor) {
							nmcd->clrText |= 0x20000000;
							if (nmcd->clrText == newColor)
								nmcd->clrText &= ~0x20000000;
						} else {
							nmcd->clrText = newColor;
						}
					}
				}
				if (index == 0) {
					if (draw->focused && !_tree->ignoreDrawFocus
							&& !draw->fullSelection) {
						int flags = TREEVIEW_BOUNDS_GET_TEXT_CLIP;
						if (_tree->explorerTheme)
							flags |= TREEVIEW_BOUNDS_GET_IMAGE;
						_w_treeitem_get_bounds_0(W_TREEITEM(&draw->item),
								&draw->focusRect, hDC, draw->index, flags);
						if (draw->measureEvent != 0) {
							draw->focusRect.right = WMIN(cellRect.right,
									draw->bounds.x + draw->bounds.width);
						}
						nmcd->nmcd.uItemState &= ~CDIS_FOCUS;
						draw->hasFocusRect = TRUE;
					}
					if (_tree->explorerTheme) {
						if (draw->selected
								|| (draw->hot && _tree->ignoreDrawHot))
							nmcd->nmcd.uItemState &= ~CDIS_HOT;
					}
					RECT itemRect;
					_w_treeitem_get_bounds_0(W_TREEITEM(&draw->item), &itemRect,
							hDC, draw->index,
							TREEVIEW_BOUNDS_GET);
					SaveDC(hDC);
					SelectClipRgn(hDC, 0);
					if (_tree->explorerTheme) {
						itemRect.left -= TREEVIEW_EXPLORER_EXTRA;
						itemRect.right += TREEVIEW_EXPLORER_EXTRA;
					}
					//bug in Windows selection or SWT itemRect
					/*if (selected)*/itemRect.right++;
					if (_tree->linesVisible)
						itemRect.bottom++;
					RECT *clipRect = draw->clipRect;
					if (clipRect != 0) {
						IntersectClipRect(hDC, clipRect->left, clipRect->top,
								clipRect->right, clipRect->bottom);
					}
					ExcludeClipRect(hDC, itemRect.left, itemRect.top,
							itemRect.right, itemRect.bottom);
					_w_event_platform *e =
							(_w_event_platform*) draw->event.event.platform_event;
					e->result = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
					return W_TRUE;
				}
			}
		}
	}
	return W_FALSE;
}
wresult _TREEVIEW_WM_NOTIFY_CDDS_ITEMPREPAINT(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	_w_treeview *_tree = _W_TREEVIEW(widget);
	_tree->hooksEraseItem = FALSE;
	NMTVCUSTOMDRAW *nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	/*
	 * Feature in Windows.  When a new tree item is inserted
	 * using TVM_INSERTITEM and the tree is using custom draw,
	 * a NM_CUSTOMDRAW is sent before TVM_INSERTITEM returns
	 * and before the item is added to the items array.  The
	 * fix is to check for null.
	 *
	 * NOTE: This only happens on XP with the version 6.00 of
	 * COMCTL32.DLL,
	 */
	if (nmcd->nmcd.dwItemSpec == 0)
		return W_FALSE;
	HWND hwndHeader = _tree->hwndHeader;
	HWND handle = _W_WIDGET(widget)->handle;
	int i;
	wresult result;
	_w_treeview_draw draw;
	memset(&draw, 0, sizeof(draw));

	draw.fullSelection = (_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0;
	draw.customDraw = (_W_WIDGET(widget)->style & W_CUSTOMDRAW) != 0;

	int explorerTheme = _tree->explorerTheme;
	_w_treeview_draw_init_draw(widget, &draw, e, priv);
	/*
	 * get column count
	 */
	int columnCount = 1;
	if (hwndHeader != 0) {
		columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
	}
	draw.hDC = nmcd->nmcd.hdc;
	int index = 0;
	if (hwndHeader != 0) {
		index = SendMessageW(hwndHeader, HDM_ORDERTOINDEX, 0, 0);
		_W_ITEM(&draw.column)->index = index;
		draw.hdItem.mask = HDI_WIDTH;
		SendMessageW(hwndHeader, HDM_GETITEMW, index, (LPARAM) &draw.hdItem);
	}
	_w_listitem_get_attr(W_LISTITEM(&draw.item), _W_ITEM(&draw.column)->index,
			W_ITEM_ATTR_MASK_ALL_NO_TEXT, &draw.attr);
	if (draw.attr.font) {
		SelectObject(draw.hDC, (HFONT) draw.attr.font);
	}
	if (_tree->ignoreCustomDraw || nmcd->nmcd.rc.left == nmcd->nmcd.rc.right) {
		if (draw.attr.font != 0) {
			e->result = CDRF_NEWFONT;
		} else {
			e->result = CDRF_DODEFAULT;
		}
		return W_TRUE;
	}
	RECT *clipRect = 0;
	if (hwndHeader != 0) {
		wresult clip = !_tree->printClient;
		if (WIN32_VERSION >= VERSION(6, 0)) {
			clip = W_TRUE;
		}
		if (clip) {
			clipRect = &draw.rect;
			SetRect(clipRect, nmcd->nmcd.rc.left, nmcd->nmcd.rc.top,
					nmcd->nmcd.rc.left + draw.hdItem.cxy, nmcd->nmcd.rc.bottom);
		}
	}
	draw.clrText = 0;
	draw.clrTextBk = 0;
	if (IsWindowEnabled(handle)) {
		draw.clrText = draw.attr.foreground;
		draw.clrTextBk = draw.attr.background;
	}
	draw.clrSortBk = 0;
	if (win_toolkit->IsAppThemed) {
		if (_W_LISTVIEWBASE(widget)->sortColumn != -1
				&& _W_LISTVIEWBASE(widget)->sortDirection != W_NONE) {
			if (priv->find_image_control(W_CONTROL(widget), priv) == 0) {
				if (_W_LISTVIEWBASE(widget)->sortColumn == index) {
					/*clrSortBk = getSortColumnPixel();
					 if (clrTextBk == 0)
					 clrTextBk = clrSortBk;*/
				}
			}
		}
	}
	draw.selected = _w_treeview_is_item_selected(widget, nmcd);
	draw.hot = explorerTheme && (nmcd->nmcd.uItemState & CDIS_HOT) != 0;
	draw.focused = explorerTheme && (nmcd->nmcd.uItemState & CDIS_FOCUS) != 0;
	if (IsWindowVisible(handle) && nmcd->nmcd.rc.left < nmcd->nmcd.rc.right
			&& nmcd->nmcd.rc.top < nmcd->nmcd.rc.bottom) {
		if (draw.attr.font)
			SelectObject(draw.hDC, (HFONT) draw.attr.font);
		if (_tree->linesVisible) {
			DrawEdge(draw.hDC, &nmcd->nmcd.rc, BDR_SUNKENINNER, BF_BOTTOM);
		}
		wresult measureEvent = W_FALSE;
		if (draw.customDraw) {
			draw.event.gc = W_GRAPHICS(&draw.gc);
			draw.event.textattr = &draw.attr;
			result = _w_treeview_send_measure_item_event(widget, &draw.event,
					&draw.bounds, draw.selected ? W_SELECTED : 0);
			draw.event.gc = 0;
			measureEvent = result;
		}
		_tree->selectionForeground = 0;
		_tree->ignoreDrawForeground = _tree->ignoreDrawBackground =
				_tree->ignoreDrawSelection = _tree->ignoreDrawFocus =
						_tree->ignoreDrawHot = _tree->ignoreFullSelection =
								W_FALSE;
		if (draw.customDraw) { // EraseItem
			result = _w_treeview_draw_send_event(widget, &draw,
					W_EVENT_ITEM_ERASE, priv);
			if (result)
				return result;
		} //if (hooks (SWT.EraseItem))
		/*
		 * Feature in Windows.  When the tree has the style
		 * TVS_FULLROWSELECT, the background color for the
		 * entire row is filled when an item is painted,
		 * drawing on top of any custom drawing.  The fix
		 * is to emulate TVS_FULLROWSELECT.
		 */
		if (draw.fullSelection) {
			int bits = GetWindowLongW(handle, GWL_STYLE);
			if ((bits & TVS_FULLROWSELECT) == 0) {
				if (draw.selected) {
					priv->fill_background(W_CONTROL(widget), draw.hDC,
							GetBkColor(draw.hDC), &nmcd->nmcd.rc);
				} else {
					if (IsWindowEnabled(handle)) {
						priv->draw_background(W_CONTROL(widget), draw.hDC,
								&nmcd->nmcd.rc, -1, 0, 0, priv);
					}
				}
				nmcd->nmcd.uItemState &= ~CDIS_FOCUS;
			}
		}
	}
	result = W_FALSE;
	if (draw.clrText == 0 && draw.clrTextBk == 0 && draw.attr.font == 0) {
		e->result = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
		result = W_TRUE;
	} else {
		result = W_TRUE;
		e->result = CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT;
		if (draw.attr.font != 0)
			SelectObject(draw.hDC, draw.attr.font);
		if (IsWindowEnabled(handle) && IsWindowVisible(handle)) {
			/*
			 * Feature in Windows.  Windows does not fill the entire cell
			 * with the background color when TVS_FULLROWSELECT is not set.
			 * The fix is to fill the cell with the background color.
			 */
			if (draw.clrTextBk != 0) {
				int bits = GetWindowLongW(handle, GWL_STYLE);
				if ((bits & TVS_FULLROWSELECT) == 0) {
					if (hwndHeader != 0) {
						SetRect(&draw.rect, nmcd->nmcd.rc.left,
								nmcd->nmcd.rc.top,
								nmcd->nmcd.rc.left + draw.hdItem.cxy,
								nmcd->nmcd.rc.bottom);
						if (_COMCTL32_VERSION < VERSION(6, 0)
								|| !IsAppThemed()) {
							RECT itemRect;
							*((HTREEITEM*) &itemRect) =
							_W_TREEITEM(&draw.item)->htreeitem;
							if (SendMessageW(handle, TVM_GETITEMRECT, TRUE,
									(LPARAM) &itemRect)) {
								draw.rect.left = WMIN(itemRect.left,
										draw.rect.right);
							}
						}
						if (draw.fullSelection) {
							if (!draw.selected) {
								priv->fill_background(W_CONTROL(widget),
										draw.hDC, draw.clrTextBk, &draw.rect);
							}
						} else {
							priv->fill_background(W_CONTROL(widget), draw.hDC,
									draw.clrTextBk, &draw.rect);
						}
					} else {
						if (draw.fullSelection) {
							if (!draw.selected) {
								priv->fill_background(W_CONTROL(widget),
										draw.hDC, draw.clrTextBk,
										&nmcd->nmcd.rc);
							}
						}
					}
				}
			}
			if (!draw.selected) {
				nmcd->clrText =
						draw.clrText == 0 ?
								priv->get_foreground_pixel(W_CONTROL(widget),
										priv) :
								(draw.clrText & 0x00FFFFFF);
				nmcd->clrTextBk =
						draw.clrTextBk == 0 ?
								priv->get_background_pixel(W_CONTROL(widget),
										priv) :
								(draw.clrTextBk & 0x00FFFFFF);
			}
		}
	}
	if (IsWindowEnabled(handle)) {
		/*
		 * On Vista only, when an item is asked to draw disabled,
		 * the background of the text is not filled with the
		 * background color of the tree.  This is true for both
		 * regular and full selection trees.  In order to draw a
		 * background image, mark the item as disabled using
		 * CDIS_DISABLED (when not selected) and set the text
		 * to the regular text color to avoid drawing disabled.
		 */
		if (explorerTheme) {
			if (priv->find_image_control(W_CONTROL(widget), priv) != 0) {
				if (!draw.selected
						&& (nmcd->nmcd.uItemState & (CDIS_HOT | CDIS_SELECTED))
								== 0) {
					nmcd->nmcd.uItemState |= CDIS_DISABLED;
					/*
					 * Feature in Windows.  On Vista only, when the text
					 * color is unchanged and an item is asked to draw
					 * disabled, it uses the disabled color.  The fix is
					 * to modify the color so it is no longer equal.
					 */
					int newColor =
							draw.clrText == 0 ?
									priv->get_foreground_pixel(
											W_CONTROL(widget), priv) :
									(draw.clrText & 0x00FFFFFF);
					if (nmcd->clrText == newColor) {
						nmcd->clrText |= 0x20000000;
						if (nmcd->clrText == newColor)
							nmcd->clrText &= ~0x20000000;
					} else {
						nmcd->clrText = newColor;
					}
					if (draw.clrTextBk != 0) {
						if (draw.fullSelection) {
							RECT rect;
							if (columnCount != 0) {
								SetRect(&rect, nmcd->nmcd.rc.left,
										nmcd->nmcd.rc.top,
										nmcd->nmcd.rc.left + draw.hdItem.cxy,
										nmcd->nmcd.rc.bottom);
							} else {
								SetRect(&rect, nmcd->nmcd.rc.left,
										nmcd->nmcd.rc.top, nmcd->nmcd.rc.right,
										nmcd->nmcd.rc.bottom);
							}
							priv->fill_background(W_CONTROL(widget), draw.hDC,
									draw.clrTextBk, &rect);
						} else {
							RECT textRect;
							_w_treeitem_get_bounds_0(W_TREEITEM(&draw.item),
									&textRect, draw.hDC, index,
									TREEVIEW_BOUNDS_GET_TEXT_CLIP
											| TREEVIEW_BOUNDS_FULLTEXT);
							priv->fill_background(W_CONTROL(widget), draw.hDC,
									draw.clrTextBk, &textRect);
						}
					}
				}
			}
		}
	} else {
		/*
		 * Feature in Windows.  When the tree is disabled, it draws
		 * with a gray background over the sort column.  The fix is
		 * to fill the background with the sort column color.
		 */
		if (draw.clrSortBk != -1) {
			SetRect(&draw.rect, nmcd->nmcd.rc.left, nmcd->nmcd.rc.top,
					nmcd->nmcd.rc.left + draw.hdItem.cxy, nmcd->nmcd.rc.bottom);
			priv->fill_background(W_CONTROL(widget), draw.hDC, draw.clrSortBk,
					&draw.rect);
		}
	}
	SaveDC(draw.hDC);
	if (clipRect != 0) {
		HRGN hRgn = CreateRectRgn(clipRect->left, clipRect->top,
				clipRect->right, clipRect->bottom);
		POINT lpPoint;
		GetWindowOrgEx(draw.hDC, &lpPoint);
		OffsetRgn(hRgn, -lpPoint.x, -lpPoint.y);
		SelectClipRgn(draw.hDC, hRgn);
		DeleteObject(hRgn);
	}
	return result;
}
wresult _TREEVIEW_WM_NOTIFY_CDDS_ITEMPOSTPAINT(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	_w_treeview *_tree = _W_TREEVIEW(widget);
	if (_tree->ignoreCustomDraw)
		return W_FALSE;
	NMTVCUSTOMDRAW *nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	if (nmcd->nmcd.rc.left == nmcd->nmcd.rc.right) {
		e->result = CDRF_DODEFAULT;
		return W_TRUE;
	}
	RestoreDC(nmcd->nmcd.hdc, -1);

	/*
	 * Feature in Windows.  When a new tree item is inserted
	 * using TVM_INSERTITEM and the tree is using custom draw,
	 * a NM_CUSTOMDRAW is sent before TVM_INSERTITEM returns
	 * and before the item is added to the items array.  The
	 * fix is to check for null.
	 *
	 * NOTE: This only happens on XP with the version 6.00 of
	 * COMCTL32.DLL,
	 */
	if (nmcd->nmcd.dwItemSpec == 0)
		return W_FALSE;

	/*
	 * Feature in Windows.  Under certain circumstances, Windows
	 * sends CDDS_ITEMPOSTPAINT for an empty rectangle.  This is
	 * not a problem providing that graphics do not occur outside
	 * the rectangle.  The fix is to test for the rectangle and
	 * draw nothing.
	 *
	 * NOTE:  This seems to happen when both I_IMAGECALLBACK
	 * and LPSTR_TEXTCALLBACK are used at the same time with
	 * TVM_SETITEM.
	 */
	if (nmcd->nmcd.rc.left >= nmcd->nmcd.rc.right
			|| nmcd->nmcd.rc.top >= nmcd->nmcd.rc.bottom)
		return W_FALSE;
	HWND hwndHeader = _tree->hwndHeader;
	HWND handle = _W_WIDGET(widget)->handle;
	int i;
	wresult result;

	if (!IsWindowVisible(handle))
		return W_FALSE;

	_w_treeview_draw draw;
	memset(&draw, 0, sizeof(draw));
	draw.fullSelection = (_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0;
	draw.customDraw = (_W_WIDGET(widget)->style & W_CUSTOMDRAW) != 0;
	draw.hDC = nmcd->nmcd.hdc;
	_w_treeview_draw_init_draw(widget, &draw, e, priv);

	GetClientRect(priv->handle_top(W_CONTROL(widget)), &draw.clientRect);

	/*
	 * get column count
	 */
	draw.columnCount = 1;
	draw.totalWidth = draw.clientRect.right - draw.clientRect.left;
	if (hwndHeader != 0) {
		draw.columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
		draw.totalWidth = 0;
		draw.hdItem.mask = HDI_WIDTH;
		for (int j = 0; j < draw.columnCount; j++) {
			SendMessageW(hwndHeader, HDM_GETITEMW, j, (LPARAM) &draw.hdItem);
			draw.totalWidth += draw.hdItem.cxy;
		}
	}
	draw.hdItem.fmt = HDF_LEFT;
	int explorerTheme = _tree->explorerTheme;

	draw.selected = _w_treeview_is_item_selected(widget, nmcd);
	draw.hot = explorerTheme && (nmcd->nmcd.uItemState & CDIS_HOT) != 0;

	int x = 0;
	for (int i = 0; i < draw.columnCount; i++) {
		draw.index = i;
		_W_ITEM(&draw.column)->index = draw.index;
		int width = nmcd->nmcd.rc.right - nmcd->nmcd.rc.left;
		if (hwndHeader != 0) {
			draw.hdItem.mask = HDI_WIDTH | HDI_FORMAT;
			SendMessageW(hwndHeader, HDM_GETITEMW, i, (LPARAM) &draw.hdItem);
			SendMessageW(hwndHeader, HDM_GETITEMRECT, draw.index,
					(LPARAM) &draw.rect);
			width = draw.rect.right - draw.rect.left;
			x = draw.rect.left;
		}
		if (x > draw.clientRect.right)
			continue;
		draw.draw = W_FALSE;
		draw.clear = W_FALSE;
		draw.drawItem = W_FALSE;
		draw.drawText = W_FALSE;
		draw.drawImage = W_FALSE;
		draw.drawBackground = W_FALSE;
		draw.drawForeground = W_FALSE;
		draw.measureEvent = W_FALSE;

		draw.clrText = 0;
		draw.clrTextBk = 0;
		draw.sortIndex = -1;
		draw.clrSortBk = 0;
		memset(&draw.attr, 0, sizeof(draw.attr));
		_w_listitem_get_attr(W_LISTITEM(&draw.item), draw.index,
				W_ITEM_ATTR_MASK_ALL_NO_TEXT, &draw.attr);
		if (i == 0) {
			if (draw.fullSelection) {
				draw.clear = !explorerTheme && !_tree->ignoreDrawSelection
						&& priv->find_image_control(W_CONTROL(widget), priv)
								== 0;
				if (draw.clear || (draw.selected && !_tree->ignoreDrawSelection)
						|| (draw.hot && !_tree->ignoreDrawHot)) {
					draw.draw = TRUE;
					RECT pClipRect;
					SetRect(&pClipRect, width, nmcd->nmcd.rc.top,
							nmcd->nmcd.rc.right, nmcd->nmcd.rc.bottom);
					if (explorerTheme) {
						if (draw.customDraw) {
							RECT itemRect;
							_w_treeitem_get_bounds_0(W_TREEITEM(&draw.item),
									&itemRect, draw.hDC, draw.index,
									TREEVIEW_BOUNDS_GET_CLIP);
							itemRect.left -= TREEVIEW_EXPLORER_EXTRA;
							itemRect.right += TREEVIEW_EXPLORER_EXTRA + 1;
							pClipRect.left = itemRect.left;
							pClipRect.right = itemRect.right;
							if (hwndHeader != 0) {
								pClipRect.right = WMIN(pClipRect.right,
										nmcd->nmcd.rc.left + draw.hdItem.cxy);
							}
						}
						RECT pRect;
						SetRect(&pRect, nmcd->nmcd.rc.left, nmcd->nmcd.rc.top,
								nmcd->nmcd.rc.right, nmcd->nmcd.rc.bottom);
						if (draw.columnCount > 0 && hwndHeader != 0) {
							if (draw.totalWidth
									> draw.clientRect.right
											- draw.clientRect.left) {
								pRect.left = 0;
								pRect.right = draw.totalWidth;
							} else {
								pRect.left = draw.clientRect.left;
								pRect.right = draw.clientRect.right;
							}
						}
						draw.draw = FALSE;
						HTHEME hTheme = OpenThemeData(handle, L"TREEVIEW");
						int iStateId =
								draw.selected ? TREIS_SELECTED : TREIS_HOT;
						if (GetFocus() != handle && draw.selected && !draw.hot)
							iStateId = TREIS_SELECTEDNOTFOCUS;
						DrawThemeBackground(hTheme, draw.hDC, TVP_TREEITEM,
								iStateId, &pRect, &pClipRect);
						CloseThemeData(hTheme);
					}
					if (draw.draw) {
						priv->fill_background(W_CONTROL(widget), draw.hDC,
								GetBkColor(draw.hDC), &pClipRect);
					}
				}
			}
		} //if (i == 0)
		if (x + width > draw.clientRect.left) {
			draw.backgroundRect = 0;
			draw.drawItem = TRUE;
			draw.drawText = TRUE;
			draw.drawImage = TRUE;
			draw.drawBackground = FALSE;
			if (i == 0) {
				draw.drawItem = draw.drawImage = draw.drawText = FALSE;
				if (priv->find_image_control(W_CONTROL(widget), priv) != 0) {
					if (explorerTheme) {
						if (IsWindowEnabled(handle) && !draw.customDraw) {
							int image = -1;
							if (draw.index == 0) {
								//image = item.image;
							} else {
								/*Image [] images  = item.images;
								 if (images != null) image = images [index];*/
							}
							if (image != -1) {
								HIMAGELIST himagelist =
										(HIMAGELIST) SendMessageW(handle,
										TVM_GETIMAGELIST, TVSIL_NORMAL, 0);
								int cx = 0, cy = 0;
								if (himagelist != 0)
									ImageList_GetIconSize(himagelist, &cx, &cy);
								if (!_W_TREEVIEW(widget)->ignoreDrawForeground) {
									RECT iconRect;
									_w_treeitem_get_bounds_0(
											W_TREEITEM(&draw.item), &iconRect,
											draw.hDC, draw.index,
											TREEVIEW_BOUNDS_GET_IMAGE_CLIP);
									ImageList_DrawEx(himagelist, image,
											draw.hDC, iconRect.left,
											iconRect.top,
											iconRect.right - iconRect.left,
											iconRect.bottom - iconRect.top, -1,
											-1, ILD_NORMAL);
									SelectClipRgn(draw.hDC, 0);
								}
							}
						}
					} else {
						draw.drawItem = draw.drawText = draw.drawBackground =
						TRUE;
						_w_treeitem_get_bounds_0(W_TREEITEM(&draw.item),
								&draw.rect, draw.hDC, draw.index,
								TREEVIEW_BOUNDS_GET_TEXT_CLIP);
						if (_W_TREEVIEW(widget)->linesVisible) {
							draw.rect.right++;
							draw.rect.bottom++;
						}
					}
				}
				if (draw.selected && !_tree->ignoreDrawSelection
						&& !_tree->ignoreDrawBackground) {
					if (!explorerTheme) {
						priv->fill_background(W_CONTROL(widget), draw.hDC,
								GetBkColor(draw.hDC), &draw.rect);
					}
					draw.drawBackground = FALSE;
				}
				draw.backgroundRect = &draw.rect;
				if (draw.customDraw) {
					if (_tree->hooksEraseItem) {
						draw.drawItem = draw.drawText = draw.drawImage = TRUE;
						_w_treeitem_get_bounds_0(W_TREEITEM(&draw.item),
								&draw.rect, draw.hDC, draw.index,
								TREEVIEW_BOUNDS_GET_CLIP);
						if (draw.fullSelection) {
							draw.backgroundRect = &draw.rect;
						} else {
							_w_treeitem_get_bounds_0(W_TREEITEM(&draw.item),
									draw.backgroundRect, draw.hDC, draw.index,
									TREEVIEW_BOUNDS_GET_TEXT_CLIP);
						}
					} // if (hooks (SWT.EraseItem))
				}
			} else {
				_tree->selectionForeground = 0;
				_tree->ignoreDrawForeground = FALSE;
				_tree->ignoreDrawBackground = FALSE;
				_tree->ignoreDrawSelection = FALSE;
				_tree->ignoreDrawFocus = FALSE;
				_tree->ignoreDrawHot = FALSE;
				SetRect(&draw.rect, x, nmcd->nmcd.rc.top, x + width,
						nmcd->nmcd.rc.bottom);
				draw.backgroundRect = &draw.rect;
			}
			draw.clrText = 0, draw.clrTextBk = 0;
			if (_tree->selectionForeground != 0)
				draw.clrText = _tree->selectionForeground;
			if (IsWindowEnabled(handle)) {
				draw.drawForeground = FALSE;
				if (draw.selected) {
					if (i != 0 && !draw.fullSelection) {
						SetTextColor(draw.hDC,
								priv->get_foreground_pixel(W_CONTROL(widget),
										priv));
						SetBkColor(draw.hDC,
								priv->get_background_pixel(W_CONTROL(widget),
										priv));
						draw.drawForeground = draw.drawBackground = TRUE;
					}
				} else {
					draw.drawForeground = draw.drawBackground = TRUE;
				}
				if (draw.drawForeground) {
					draw.clrText = draw.attr.foreground;
				}
				if (draw.drawBackground) {
					draw.clrTextBk = draw.attr.background;
					if (draw.clrTextBk == 0 && draw.index == draw.sortIndex)
						draw.clrTextBk = draw.clrSortBk;
				}
			} else {
				if (draw.clrTextBk == 0 && draw.index == draw.sortIndex) {
					draw.drawBackground = TRUE;
					draw.clrTextBk = draw.clrSortBk;
				}
			}
			if (explorerTheme) {
				if (draw.selected || (nmcd->nmcd.uItemState & CDIS_HOT) != 0) {
					if (draw.fullSelection) {
						draw.drawBackground = FALSE;
					} else {
						if (i == 0) {
							draw.drawBackground = FALSE;
							if (!draw.customDraw)
								draw.drawText = FALSE;
						}
					}
				}
			}
			if (draw.drawItem) {
				if (draw.index != 0) {
					if (draw.customDraw) {
						draw.event.textattr = &draw.attr;
						draw.measureEvent = _w_treeview_send_measure_item_event(
								widget, &draw.event, &draw.bounds,
								draw.selected ? W_SELECTED : 0);
					}
					if (draw.customDraw) {
						_w_treeview_draw_send_event(widget, &draw,
								W_EVENT_ITEM_ERASE, priv);
					}/*if (hooks (SWT.EraseItem))*/
					if (_tree->selectionForeground != 0)
						draw.clrText = _tree->selectionForeground;
				}
				if (!_tree->ignoreDrawBackground) {
					if (draw.clrTextBk != 0) {
						if (draw.drawBackground) {
							priv->fill_background(W_CONTROL(widget), draw.hDC,
									draw.clrTextBk, draw.backgroundRect);
						}
					} else {
						/*Control control = findImageControl ();
						 if (control != null) {
						 if (i == 0) {
						 int right = Math.min (rect.right, width);
						 SetRect (rect, rect.left, rect.top, right, rect.bottom);
						 if (drawBackground) fillImageBackground (hDC, control, rect, 0, 0);
						 } else {
						 if (drawBackground) fillImageBackground (hDC, control, rect, 0, 0);
						 }
						 }*/
					}
				}
				draw.rect.left += TREEVIEW_INSET - 1;
				if (draw.drawImage) {
					int image = -1;
					if (draw.index == 0) {
						TV_ITEMW tvi;
						tvi.mask = TVIF_IMAGE | TVIF_HANDLE;
						tvi.hItem = (HTREEITEM) nmcd->nmcd.dwItemSpec;
						tvi.iImage = -1;
						SendMessageW(handle, TVM_GETITEMW, 0, (WPARAM) &tvi);
						image = tvi.iImage;
					} else {
						/*Image [] images  = item.images;
						 if (images != null) image = images [index];*/
					}
					HIMAGELIST himagelist = (HIMAGELIST) SendMessageW(handle,
					TVM_GETIMAGELIST, TVSIL_NORMAL, 0);
					int cx = 0, cy = 0;
					if (himagelist != 0)
						ImageList_GetIconSize(himagelist, &cx, &cy);
					int inset = draw.index != 0 ? TREEVIEW_INSET : 0;
					int offset = draw.index != 0 ?
					TREEVIEW_INSET :
													TREEVIEW_INSET + 2;
					if (image != -1) {
						//Rectangle bounds = image.getBounds (); // Points
						//if (size == null) size = DPIUtil.autoScaleDown (getImageSize ()); // To Points
						if (!_tree->ignoreDrawForeground) {
							//int y1 = rect.top + (index == 0 ? (getItemHeight () - size.y) / 2 : 0);
							int y1 = draw.rect.top;
							int x1 = WMAX(draw.rect.left,
									draw.rect.left - inset + 1);
							if (himagelist != 0)
								ImageList_DrawEx(himagelist, image, draw.hDC,
										x1, y1, cx, cy, -1, -1, ILD_NORMAL);
							/*GC gc = GC.win32_new (hDC, data);
							 gc.setClipping (DPIUtil.autoScaleDown(new Rectangle(x1, rect.top, rect.right - x1, rect.bottom - rect.top)));
							 gc.drawImage (image, 0, 0, bounds.width, bounds.height, DPIUtil.autoScaleDown(x1), DPIUtil.autoScaleDown(y1), size.x, size.y);
							 SelectClipRgn (hDC, 0);
							 gc.dispose ();*/
						}
						SetRect(&draw.rect, draw.rect.left + cx + offset,
								draw.rect.top, draw.rect.right - inset,
								draw.rect.bottom);
					} else {
						if (draw.index == 0) {
							if (himagelist) {
								//if (size == null) size = getImageSize ();
								draw.rect.left = WMIN(
										draw.rect.left + cx + offset,
										draw.rect.right);
							}
						} else {
							SetRect(&draw.rect, draw.rect.left + offset,
									draw.rect.top, draw.rect.right - inset,
									draw.rect.bottom);
						}
					}
				}
				if (draw.drawText) {
					/*
					 * Bug in Windows.  When DrawText() is used with DT_VCENTER
					 * and DT_ENDELLIPSIS, the ellipsis can draw outside of the
					 * rectangle when the rectangle is empty.  The fix is avoid
					 * all text drawing for empty rectangles.
					 */
					if (draw.rect.left < draw.rect.right) {
						_win32_toolkit_alloc alloc;
						alloc.text = 0;
						alloc.size = 0;
						draw.attr.alloc = _win32_toolkit_alloc_fn;
						draw.attr.user_data = &alloc;
						draw.attr.enc = W_ENCODING_UNICODE;
						_w_listitem_get_attr(W_LISTITEM(&draw.item), draw.index,
								W_ITEM_ATTR_MASK_TEXT, &draw.attr);
						if (alloc.text != 0) {
							HFONT lastfont;
							COLORREF oldClrText;
							COLORREF oldClrTextBk;
							int oldBkMode;
							HDC hDC = nmcd->nmcd.hdc;
							if (draw.attr.font != 0)
								lastfont = SelectObject(hDC,
										(HFONT) draw.attr.font);
							if (draw.clrText != 0)
								oldClrText = SetTextColor(hDC,
										draw.clrText & 0x00FFFFFF);
							if (draw.clrTextBk != 0) {
								oldClrTextBk = SetBkColor(hDC,
										draw.clrTextBk & 0x00FFFFFF);
							} else {
								oldBkMode = SetBkMode(hDC, TRANSPARENT);
							}
							int flags = DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER;
							if (draw.index != 0)
								flags |= DT_END_ELLIPSIS;
							if (draw.hdItem.fmt & HDF_LEFT)
								flags |= DT_LEFT;
							if (draw.hdItem.fmt & HDF_CENTER)
								flags |= DT_CENTER;
							if (draw.hdItem.fmt & HDF_RIGHT)
								flags |= DT_RIGHT;
							if (!_tree->ignoreDrawForeground)
								DrawTextW(hDC, alloc.text, alloc.size / 2,
										&draw.rect, flags);
							DrawTextW(hDC, alloc.text, alloc.size / 2,
									&draw.rect, flags | DT_CALCRECT);
							if (draw.attr.font != 0)
								SelectObject(hDC, lastfont);
							if (draw.clrText != 0)
								SetTextColor(hDC, oldClrText);
							if (draw.clrTextBk != 0)
								SetBkColor(hDC, oldClrTextBk);
							else {
								SetBkMode(hDC, oldBkMode);
							}
						}
						_w_toolkit_free(alloc.text, alloc.size);
					}
				}
			}
			if (_tree->selectionForeground != 0)
				draw.clrText = _tree->selectionForeground;
			HDC hDC = nmcd->nmcd.hdc;
			if (draw.customDraw) {
				_w_treeview_draw_send_event(widget, &draw, W_EVENT_ITEM_PAINT,
						priv);
			}
		}
	}/*end loop */
	if (_tree->linesVisible) {
		if (draw.fullSelection) {
			if (hwndHeader != 0) {
				SetRect(&draw.rect, nmcd->nmcd.rc.left + draw.hdItem.cxy,
						nmcd->nmcd.rc.top, nmcd->nmcd.rc.right,
						nmcd->nmcd.rc.bottom);
				DrawEdge(draw.hDC, &draw.rect, BDR_SUNKENINNER, BF_BOTTOM);
			}
		}
		DrawEdge(draw.hDC, &nmcd->nmcd.rc, BDR_SUNKENINNER, BF_BOTTOM);
	}
	if (!_tree->ignoreDrawFocus && draw.hasFocusRect != 0) {
		DrawFocusRect(draw.hDC, &draw.focusRect);
		draw.hasFocusRect = FALSE;
	} else {
		if (!explorerTheme) {
			if (handle == GetFocus()) {
				int uiState = SendMessageW(handle, WM_QUERYUISTATE, 0, 0);
				if ((uiState & UISF_HIDEFOCUS) == 0) {
					HTREEITEM hItem = (HTREEITEM) SendMessageW(handle,
					TVM_GETNEXTITEM, TVGN_CARET, 0);
					if (hItem == (HTREEITEM) nmcd->nmcd.dwItemSpec) {
						if (!_tree->ignoreDrawFocus
								&& priv->find_image_control(W_CONTROL(widget),
										priv) != 0) {
							if (draw.fullSelection) {
								SetRect(&draw.focusRect, 0, nmcd->nmcd.rc.top,
										draw.clientRect.right + 1,
										nmcd->nmcd.rc.bottom);
								DrawFocusRect(draw.hDC, &draw.focusRect);
							} else {
								int index = SendMessageW(hwndHeader,
								HDM_ORDERTOINDEX, 0, 0);
								RECT clipRect;
								_w_treeitem_get_bounds_0(W_TREEITEM(&draw.item),
										&draw.focusRect, draw.hDC, index,
										TREEVIEW_BOUNDS_GET_TEXT);
								_w_treeitem_get_bounds_0(W_TREEITEM(&draw.item),
										&clipRect, draw.hDC, index,
										TREEVIEW_BOUNDS_GET_TEXT_CLIP);
								IntersectClipRect(draw.hDC, clipRect.left,
										clipRect.top, clipRect.right,
										clipRect.bottom);
								DrawFocusRect(draw.hDC, &draw.focusRect);
								SelectClipRgn(draw.hDC, 0);
							}
						}
					}
				}
			}
		}
	}
	e->result = CDRF_DODEFAULT;
	return W_TRUE;
}
wresult _TREEVIEW_WM_NOTIFY_CDDS_POSTPAINT(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	_w_treeview *_tree = _W_TREEVIEW(widget);
	if (_tree->ignoreCustomDraw)
		return W_FALSE;
	NMTVCUSTOMDRAW *nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	HWND handle = _W_WIDGET(widget)->handle;
	HWND hwndHeader = _tree->hwndHeader;
	RECT rect;
	HDITEMW hdItem;
	if (IsWindowVisible(handle)) {
		if (win_toolkit->IsAppThemed) {
			if (_W_LISTVIEWBASE(widget)->sortColumn != -1
					&& _W_LISTVIEWBASE(widget)->sortDirection != W_NONE) {
				if (priv->find_image_control(W_CONTROL(widget), priv) == 0) {
					int index = _W_LISTVIEWBASE(widget)->sortColumn;
					if (index != -1) {
						int top = nmcd->nmcd.rc.top;
						/*
						 * Bug in Windows.  For some reason, during a collapse,
						 * when TVM_GETNEXTITEM is sent with TVGN_LASTVISIBLE
						 * and the collapse causes the item being collapsed
						 * to become the last visible item in the tree, the
						 * message takes a long time to process.  In order for
						 * the slowness to happen, the children of the item
						 * must have children.  Times of up to 11 seconds have
						 * been observed with 23 children, each having one
						 * child.  The fix is to use the bottom partially
						 * visible item rather than the last possible item
						 * that could be visible.
						 *
						 * NOTE: This problem only happens on Vista during
						 * WM_NOTIFY with NM_CUSTOMDRAW and CDDS_POSTPAINT.
						 */
						HTREEITEM hItem = 0;
						if (WIN32_VERSION >= VERSION(6, 0)) {
							hItem = _w_treeview_get_bottom_item(handle);
						} else {
							hItem = (HTREEITEM) SendMessageW(handle,
							TVM_GETNEXTITEM, TVGN_LASTVISIBLE, 0);
						}
						if (hItem != 0) {
							RECT rect;
							*((HTREEITEM*) &rect) = hItem;
							if (SendMessageW(handle, TVM_GETITEMRECT, FALSE,
									(LPARAM) &rect)) {
								top = rect.bottom;
							}
						}
						RECT rect;
						SetRect(&rect, nmcd->nmcd.rc.left, top,
								nmcd->nmcd.rc.right, nmcd->nmcd.rc.bottom);
						RECT headerRect;
						SendMessageW(hwndHeader, HDM_GETITEMRECT, index,
								(LPARAM) &headerRect);
						rect.left = headerRect.left;
						rect.right = headerRect.right;
						//priv->fill_background(W_CONTROL(widget),nmcd->nmcd.hdc,getSortColumnPixel (), &rect);
					}
				}
			}
		}
		if (_tree->linesVisible) {
			HDC hDC = nmcd->nmcd.hdc;
			if (hwndHeader != 0) {
				int x = 0;
				/*
				 * get column count
				 */
				int columnCount = 1;
				if (hwndHeader != 0) {
					columnCount = SendMessageW(hwndHeader,
					HDM_GETITEMCOUNT, 0, 0);
				}
				for (int i = 0; i < columnCount; i++) {
					RECT _r;
					SendMessageW(hwndHeader, HDM_GETITEMRECT, i, (LPARAM) &_r);
					SetRect(&rect, _r.left, nmcd->nmcd.rc.top, _r.right,
							nmcd->nmcd.rc.bottom);
					DrawEdge(hDC, &rect, BDR_SUNKENINNER, BF_RIGHT);
				}
			}
			int height = 0;
			/*
			 * Bug in Windows.  For some reason, during a collapse,
			 * when TVM_GETNEXTITEM is sent with TVGN_LASTVISIBLE
			 * and the collapse causes the item being collapsed
			 * to become the last visible item in the tree, the
			 * message takes a long time to process.  In order for
			 * the slowness to happen, the children of the item
			 * must have children.  Times of up to 11 seconds have
			 * been observed with 23 children, each having one
			 * child.  The fix is to use the bottom partially
			 * visible item rather than the last possible item
			 * that could be visible.
			 *
			 * NOTE: This problem only happens on Vista during
			 * WM_NOTIFY with NM_CUSTOMDRAW and CDDS_POSTPAINT.
			 */
			HTREEITEM hItem = 0;
			if (WIN32_VERSION >= VERSION(6, 0)) {
				hItem = _w_treeview_get_bottom_item(handle);
			} else {
				hItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
				TVGN_LASTVISIBLE, 0);
			}
			if (hItem != 0) {
				*((HTREEITEM*) &rect) = hItem;
				if (SendMessageW(handle, TVM_GETITEMRECT, FALSE,
						(LPARAM) &rect)) {
					height = rect.bottom - rect.top;
				}
			}
			if (height == 0) {
				height = SendMessageW(handle, TVM_GETITEMHEIGHT, 0, 0);
				GetClientRect(handle, &rect);
				SetRect(&rect, rect.left, rect.top, rect.right,
						rect.top + height);
				DrawEdge(hDC, &rect, BDR_SUNKENINNER, BF_BOTTOM);
			}
			if (height != 0) {
				while (rect.bottom < nmcd->nmcd.rc.bottom) {
					int top = rect.top + height;
					SetRect(&rect, rect.left, top, rect.right, top + height);
					DrawEdge(hDC, &rect, BDR_SUNKENINNER, BF_BOTTOM);
				}
			}
		}
	}
	e->result = CDRF_DODEFAULT;
	return W_TRUE;
}
wresult _TREEVIEW_WM_NOTIFY_CUSTOMDRAW(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMTVCUSTOMDRAW *nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	HWND hwndHeader = _W_TREEVIEW(widget)->hwndHeader;
	if (nmcd->nmcd.hdr.hwndFrom == hwndHeader)
		return W_FALSE;
	switch (nmcd->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		return _TREEVIEW_WM_NOTIFY_CDDS_PREPAINT(widget, e, priv);
	case CDDS_ITEMPREPAINT:
		return _TREEVIEW_WM_NOTIFY_CDDS_ITEMPREPAINT(widget, e, priv);
	case CDDS_ITEMPOSTPAINT:
		return _TREEVIEW_WM_NOTIFY_CDDS_ITEMPOSTPAINT(widget, e, priv);
	case CDDS_POSTPAINT:
		return _TREEVIEW_WM_NOTIFY_CDDS_POSTPAINT(widget, e, priv);
	}
	return W_FALSE;
}
wresult _TREEVIEW_WM_NOTIFY_DBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREEVIEW_WM_NOTIFY_ITEMCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREEVIEW_WM_NOTIFY_SELCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREEVIEW_WM_NOTIFY_SELCHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMTREEVIEWW *treeView = (NMTREEVIEWW*) e->lparam;
	TVITEMW tvItem;
	w_event_list event;
	w_treeitem item;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_ITEM_SELECTION;
	event.event.widget = widget;
	event.item = W_LISTITEM(&item);
	W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
	_W_ITEM(&item)->parent = widget;
	_W_ITEM(&item)->index = -1;
	_W_TREEITEM(&item)->htreeitem = (HTREEITEM) treeView->itemNew.hItem;
	_w_widget_post_event(widget, (w_event*) &event,W_EVENT_SEND);
	return W_FALSE;
}
wresult _TREEVIEW_WM_NOTIFY_ITEMEXPANDED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREEVIEW_WM_NOTIFY_ITEMEXPANDING(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	NMTREEVIEW *treeView = (NMTREEVIEW*) e->lparam;
	wresult runExpanded = W_FALSE;
	w_event_list event;
	w_treeitem item;
	TVITEMW tvItem;
	memset(&event, 0, sizeof(event));
	event.event.widget = widget;
	event.item = W_LISTITEM(&item);
	W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
	_W_ITEM(&item)->parent = widget;
	_W_ITEM(&item)->index = -1;
	_W_TREEITEM(&item)->htreeitem = (HTREEITEM) treeView->itemNew.hItem;
	HWND handle = _W_WIDGET(widget)->handle;
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
			_w_widget_post_event(widget, (w_event*) &event,W_EVENT_SEND);
			/**
			 * if tree item has no children remove + image
			 */
			HTREEITEM hFirstItem = (HTREEITEM) SendMessageW(handle,
			TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM) treeView->itemNew.hItem);
			if (hFirstItem == 0) {
				tvItem.mask = TVIF_CHILDREN;
				tvItem.cChildren = 0;
				tvItem.hItem = treeView->itemNew.hItem;
				HRESULT result = SendMessageW(handle, TVM_SETITEMW, 0,
						(LPARAM) &tvItem);
			}
		}
		break;
	case TVE_COLLAPSE:
		event.event.type = W_EVENT_ITEM_COLLAPSE;
		_w_widget_post_event(widget, (w_event*) &event,W_EVENT_SEND);
		break;
	}
	return W_FALSE;
}
wresult _TREEVIEW_WM_NOTIFY_BEGINDRAG(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _TREEVIEW_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	switch (((NMHDR*) e->lparam)->code) {
	case TVN_GETDISPINFOA:
	case TVN_GETDISPINFOW:
		return _TREEVIEW_WM_NOTIFY_GETDISPINFO(widget, e, priv);
		break;
	case NM_CUSTOMDRAW:
		return _TREEVIEW_WM_NOTIFY_CUSTOMDRAW(widget, e, priv);
		break;
	case NM_DBLCLK:
		return _TREEVIEW_WM_NOTIFY_DBLCLK(widget, e, priv);
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
		return _TREEVIEW_WM_NOTIFY_ITEMCHANGING(widget, e, priv);
		break;
	case TVN_SELCHANGINGA:
	case TVN_SELCHANGINGW:
		return _TREEVIEW_WM_NOTIFY_SELCHANGING(widget, e, priv);
		break;
	case TVN_SELCHANGEDA:
	case TVN_SELCHANGEDW:
		return _TREEVIEW_WM_NOTIFY_SELCHANGED(widget, e, priv);
		break;
	case TVN_ITEMEXPANDINGA:
	case TVN_ITEMEXPANDINGW:
		return _TREEVIEW_WM_NOTIFY_ITEMEXPANDING(widget, e, priv);
		break;
	case TVN_ITEMEXPANDEDA:
	case TVN_ITEMEXPANDEDW:
		return _TREEVIEW_WM_NOTIFY_ITEMEXPANDED(widget, e, priv);
		break;
	case TVN_BEGINDRAGA:
	case TVN_BEGINDRAGW:
		if ((GetKeyState(VK_LBUTTON) >= 0)) {
			return _TREEVIEW_WM_NOTIFY_BEGINDRAG(widget, e, priv);
		}
		break;
	case TVN_BEGINRDRAGA:
	case TVN_BEGINRDRAGW:
		return _TREEVIEW_WM_NOTIFY_BEGINDRAG(widget, e, priv);
		break;
	}
	return W_FALSE;
}
/*
 *
 */
wresult _TREEVIEW_WM_NOTIFY_HEADER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMHEADERW *phdn = (NMHEADERW*) e->lparam;
	HWND hwndHeader = _W_TREEVIEW(widget)->hwndHeader;
	HWND handle = _W_WIDGET(widget)->handle;
	w_event_list event;
	w_columnitem column;
	W_WIDGETDATA(&column)->clazz = _W_LISTVIEWBASE_GET_COLUMN_CLASS(widget);
	_W_ITEM(&column)->parent = widget;
	switch (phdn->hdr.code) {
	case HDN_BEGINTRACKW:
	case HDN_BEGINTRACKA:
	case HDN_DIVIDERDBLCLICKW:
	case HDN_DIVIDERDBLCLICKA: {
		if (phdn->iItem >= 0) {
			_W_ITEM(&column)->index = phdn->iItem;
			wresult resizable = w_columnitem_get_resizable(&column);
			if (!resizable) {
				e->result = 1;
				return W_TRUE;
			}
		}
		_W_TREEVIEW(widget)->ignoreColumnMove = W_TRUE;
		switch (phdn->hdr.code) {
		case HDN_DIVIDERDBLCLICKW:
		case HDN_DIVIDERDBLCLICKA:
			if (phdn->iItem >= 0)
				w_columnitem_pack(&column);
		}
		break;
	}
	case NM_RELEASEDCAPTURE: {
		if (!_W_TREEVIEW(widget)->ignoreColumnMove) {
			int columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
			for (int i = 0; i < columnCount; i++) {
				_W_ITEM(&column)->index = i;
				//column.updateToolTip(i);
			}
			//updateImageList();
		}
		_W_TREEVIEW(widget)->ignoreColumnMove = FALSE;
		break;
	}
	case HDN_BEGINDRAG: {
		if (_W_TREEVIEW(widget)->ignoreColumnMove) {
			e->result = 1;
			return W_TRUE;
		}
		if (phdn->iItem != -1) {
			_W_ITEM(&column)->index = phdn->iItem;
			wresult moveable = w_columnitem_get_moveable(&column);
			if (!moveable) {
				_W_TREEVIEW(widget)->ignoreColumnMove = W_TRUE;
				e->result = 1;
				return W_TRUE;
			}
		}
		break;
	}
	case HDN_ENDDRAG: {
		if (phdn->iItem != -1 && phdn->pitem != 0) {
			if ((phdn->pitem->mask & HDI_ORDER) != 0
					&& phdn->pitem->iOrder != -1) {
				int columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT, 0,
						0);
				int *order = _w_toolkit_malloc(columnCount * sizeof(int));
				SendMessageW(hwndHeader, HDM_GETORDERARRAY, columnCount,
						(LPARAM) order);
				int index = 0;
				while (index < columnCount) {
					if (order[index] == phdn->iItem)
						break;
					index++;
				}
				if (index == columnCount)
					index = 0;
				if (index != phdn->pitem->iOrder) {
					int start = WMIN(index, phdn->pitem->iOrder);
					int end = WMAX(index, phdn->pitem->iOrder);
					RECT rect, headerRect;
					GetClientRect(handle, &rect);
					SendMessageW(hwndHeader, HDM_GETITEMRECT, order[start],
							(LPARAM) &headerRect);
					rect.left = WMAX(rect.left, headerRect.left);
					SendMessageW(hwndHeader, HDM_GETITEMRECT, order[end],
							(LPARAM) &headerRect);
					rect.right = WMIN(rect.right, headerRect.right);
					InvalidateRect(handle, &rect, W_TRUE);
					_W_TREEVIEW(widget)->ignoreColumnMove = FALSE;
					for (int i = start; i <= end; i++) {
						_W_ITEM(&column)->index = order[i];
						memset(&event, 0, sizeof(event));
						event.event.type = W_EVENT_ITEM_MOVE;
						event.event.widget = widget;
						event.event.platform_event = _EVENT_PLATFORM(e);
						event.column = W_COLUMNITEM(&column);
						_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
					}
				}
				_w_toolkit_free(order, columnCount * sizeof(int));
			}
		}
		break;
	}
	case HDN_ITEMCHANGINGW:
	case HDN_ITEMCHANGINGA: {
		if (phdn->pitem != 0) {
			if ((phdn->pitem->mask & HDI_WIDTH) != 0) {
				RECT rect;
				GetClientRect(_W_WIDGET(widget)->handle, &rect);
				HDITEMW oldItem;
				oldItem.mask = HDI_WIDTH;
				SendMessageW(hwndHeader, HDM_GETITEMW, phdn->iItem,
						(LPARAM) &oldItem);
				int deltaX = phdn->pitem->cxy - oldItem.cxy;
				RECT headerRect;
				SendMessageW(hwndHeader, HDM_GETITEMRECT, phdn->iItem,
						(LPARAM) &headerRect);
				int gridWidth =
				_W_TREEVIEW(widget)->linesVisible ?
				TREEVIEW_GRID_WIDTH :
													0;
				rect.left = headerRect.right - gridWidth;
				int newX = rect.left + deltaX;
				rect.right = WMAX(rect.right, rect.left + abs(deltaX));
				if (_W_TREEVIEW(widget)->explorerTheme
						|| (_W_WIDGET(widget)->style & W_CUSTOMDRAW) != 0
						|| (priv->find_image_control(W_CONTROL(widget), priv)
								!= 0)) {
					rect.left -= GetSystemMetrics(SM_CXFOCUSBORDER);
					InvalidateRect(handle, &rect, W_TRUE);
					OffsetRect(&rect, deltaX, 0);
					InvalidateRect(handle, &rect, W_TRUE);
				} else {
					int flags = SW_INVALIDATE | SW_ERASE;
					ScrollWindowEx(handle, deltaX, 0, &rect, NULL, 0, NULL,
							flags);
				}
				if (SendMessageW(hwndHeader, HDM_ORDERTOINDEX, phdn->iItem, 0)
						!= 0) {
					rect.left = headerRect.left;
					rect.right = newX;
					InvalidateRect(handle, &rect, TRUE);
				}
				_w_treeview_set_scroll_width(W_TREEVIEW(widget));
			}
		}
		break;
	}
	case HDN_ITEMCHANGEDW:
	case HDN_ITEMCHANGEDA: {
		if (phdn->pitem != 0) {
			if ((phdn->pitem->mask & HDI_WIDTH) != 0) {
				if (_W_TREEVIEW(widget)->ignoreColumnMove) {
					if (WIN32_VERSION >= VERSION(6, 0)) {
						int flags = RDW_UPDATENOW | RDW_ALLCHILDREN;
						RedrawWindow(handle, NULL, 0, flags);
					} else {
						if ((_W_WIDGET(widget)->style & W_DOUBLE_BUFFERED)
								== 0) {
							wuint64 oldStyle = _W_WIDGET(widget)->style;
							_W_WIDGET(widget)->style |= W_DOUBLE_BUFFERED;
							UpdateWindow(handle);
							_W_WIDGET(widget)->style = oldStyle;
						}
					}
				}
				if (phdn->iItem >= 0) {
					_W_ITEM(&column)->index = phdn->iItem;
					//column.updateToolTip(phdn.iItem);
					memset(&event, 0, sizeof(event));
					event.event.type = W_EVENT_ITEM_RESIZE;
					event.event.widget = widget;
					event.event.platform_event = _EVENT_PLATFORM(e);
					event.column = W_COLUMNITEM(&column);
					_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
					if (w_widget_is_ok(widget) < 0) {
						e->result = 0;
						return W_TRUE;
					}
					int columnCount = SendMessageW(hwndHeader, HDM_GETITEMCOUNT,
							0, 0);
					int *order = _w_toolkit_malloc(columnCount * sizeof(int));
					SendMessageW(hwndHeader, HDM_GETORDERARRAY, columnCount,
							(LPARAM) order);
					int moved = FALSE;
					for (int i = 0; i < columnCount; i++) {
						int nextColumn = order[i];
						if (moved) {
							_W_ITEM(&column)->index = order[i];
							//nextColumn.updateToolTip(order[i]);
							memset(&event, 0, sizeof(event));
							event.event.type = W_EVENT_ITEM_MOVE;
							event.event.widget = widget;
							event.event.platform_event = _EVENT_PLATFORM(e);
							event.column = W_COLUMNITEM(&column);
							_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
						}
						if (nextColumn == phdn->iItem)
							moved = W_TRUE;
					}
					_w_toolkit_free(order, columnCount * sizeof(int));
				}
			}
			_w_treeview_set_scroll_width(W_TREEVIEW(widget));
		}
		break;
	}
	case HDN_ITEMCLICKW:
	case HDN_ITEMCLICKA: {
		if (phdn->iItem >= 0) {
			_W_ITEM(&column)->index = phdn->iItem;
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_SELECTION;
			event.event.widget = widget;
			event.event.platform_event = _EVENT_PLATFORM(e);
			event.column = W_COLUMNITEM(&column);
			_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
		}
		break;
	}
	case HDN_ITEMDBLCLICKW:
	case HDN_ITEMDBLCLICKA: {
		if (phdn->iItem >= 0) {
			_W_ITEM(&column)->index = phdn->iItem;
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_DEFAULTSELECTION;
			event.event.widget = widget;
			event.event.platform_event = _EVENT_PLATFORM(e);
			event.column = W_COLUMNITEM(&column);
			_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
		}
		break;
	}
	}
	return W_FALSE;
}

