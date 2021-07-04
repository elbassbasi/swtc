/*
 * tabview.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "tabview.h"
#include "../widgets/toolkit.h"
#include <vsstyle.h>
wresult _w_tabitem_get_data(w_item *item, void **data) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	TCITEMW tcItem;
	tcItem.mask = TCIF_PARAM;
	tcItem.lParam = (LPARAM) 0;
	SendMessageW(handle, TCM_GETITEMW, index, (LPARAM) &tcItem);
	*data = (void*) tcItem.lParam;
	return W_TRUE;
}
wresult _w_tabitem_get_index(w_item *item) {
	return _W_ITEM(item)->index;
}
wresult _w_tabitem_get_text_unicode(HWND handle, int index, WCHAR **str,
		int *length, size_t *size) {
	*str = _w_toolkit_malloc_all(size);
	if (str != 0) {
		TC_ITEMW tci;
		tci.mask = TCIF_TEXT;
		tci.pszText = *str;
		tci.cchTextMax = *size / sizeof(WCHAR);
		tci.dwStateMask = TCIS_HIGHLIGHTED;
		if (!SendMessageW(handle, TCM_GETITEMW, index, (LPARAM) &tci)) {
			return FALSE;
		}
		*length = lstrlenW(*str);
		return W_TRUE;
	}
	*length = 0;
	return W_FALSE;
}
wresult _w_tabitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	WCHAR *str;
	int length;
	size_t size;
	wresult result = _w_tabitem_get_text_unicode(handle, index, &str, &length,
			&size);
	if (result > 0) {
		result = _win_text_set(str, length, alloc, user_data, enc);
	}
	_w_toolkit_free(str, size);
	return result;
}
wresult _w_tabitem_set_data(w_item *item, void *data) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	TCITEMW tcItem;
	tcItem.mask = TCIF_PARAM;
	tcItem.lParam = (LPARAM) data;
	SendMessageW(handle, TCM_SETITEMW, index, (LPARAM) &tcItem);
	return W_TRUE;
}
wresult _w_tabitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	TCITEMW tcItem;
	WCHAR *str;
	int newlength;
	wresult result = _win_text_fix(text, length, enc, &str, &newlength);
	if (result > 0) {
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = str;
		tcItem.cchTextMax = newlength;
		SendMessageW(handle, TCM_SETITEMW, index, (LPARAM) &tcItem);
	}
	_win_text_free(text, str, newlength);
	return result;
}
wresult _w_tabitem_get_bounds(w_tabitem *item, w_rect *rect) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	RECT itemRect;
	SendMessageW(handle, TCM_GETITEMRECT, index, (LPARAM) &itemRect);
	rect->x = itemRect.left;
	rect->y = itemRect.top;
	rect->width = itemRect.right - itemRect.left;
	rect->height = itemRect.bottom - itemRect.top;
	return W_TRUE;
}
wresult _w_tabitem_get_image(w_tabitem *item) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	TCITEMW tcItem;
	tcItem.mask = TCIF_IMAGE;
	tcItem.iImage = -1;
	SendMessageW(handle, TCM_GETITEMW, index, (LPARAM) &tcItem);
	return tcItem.iImage;
}
wresult _w_tabitem_remove(w_tabitem *item, int destroy_Control) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	w_control *control = w_tabitem_get_control(item);
	if (control != 0) {
		w_widget_dispose(W_WIDGET(control));
	}
	SendMessageW(handle, TCM_DELETEITEM, index, (LPARAM) 0);
	return W_TRUE;
}
wresult _w_tabitem_set_image(w_tabitem *item, int image) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	TCITEMW tcItem;
	tcItem.mask = TCIF_IMAGE;
	tcItem.iImage = image;
	SendMessageW(handle, TCM_SETITEMW, index, (LPARAM) &tcItem);
	return W_TRUE;
}
wresult _w_tabitem_pack(w_tabitem *item, w_control *control) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	int selectionIndex = SendMessageW(handle, TCM_GETCURSEL, 0, 0);
	if (index == selectionIndex) {
		w_rect r;
		w_scrollable_get_client_area(W_SCROLLABLE(parent), &r);
		w_control_set_bounds(control, &r.pt, &r.sz);
		w_control_set_visible(control, W_TRUE);
	}
	return W_TRUE;
}
/*
 * tabview
 */
wuint64 _w_tabview_check_style(w_widget *widget, wuint64 style) {
	/*
	 * When the W_TOP style has not been set, force the
	 * tabs to be on the bottom for tab folders on PPC.
	 */
	style = _w_widget_check_bits(style, W_TOP, W_BOTTOM, W_LEFT, W_RIGHT, 0, 0);

	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_tabview_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	wresult result = _w_composite_compute_size(widget, e, priv);
	HWND handle = _W_WIDGET(widget)->handle;
	RECT insetRect, itemRect;
	SendMessageW(handle, TCM_ADJUSTRECT, 0, (LPARAM) &insetRect);
	int width = insetRect.left - insetRect.right;
	int count = SendMessageW(handle, TCM_GETITEMCOUNT, 0, 0);
	if (count != 0) {
		SendMessageW(handle, TCM_GETITEMRECT, count - 1, (LPARAM) &itemRect);
		width = WMAX(width, itemRect.right - insetRect.right);
	}
	RECT rect;
	SetRect(&rect, 0, 0, width, e->size->height);
	SendMessageW(handle, TCM_ADJUSTRECT, 1, (LPARAM) &rect);
	int border = w_control_get_border_width(W_CONTROL(widget));
	rect.left -= border;
	rect.right += border;
	width = rect.right - rect.left;
	e->size->width = WMAX(width, e->size->width);
	return result;
}
wresult _w_tabview_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	HWND handle = _W_WIDGET(widget)->handle;
	RECT rect;
	SetRect(&rect, e->rect->x, e->rect->y, e->rect->x + e->rect->width,
			e->rect->y + e->rect->height);
	SendMessageW(handle, TCM_ADJUSTRECT, 1, (LPARAM) &rect);
	int border = w_control_get_border_width(W_CONTROL(widget));
	rect.left -= border;
	rect.right += border;
	rect.top -= border;
	rect.bottom += border;
	e->result->x = rect.left;
	e->result->y = rect.top;
	e->result->width = rect.right - rect.left;
	e->result->height = rect.bottom - rect.top;
	return W_TRUE;
}
wresult _w_tabview_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result = _w_composite_create_handle(control, priv);
	if (result > 0) {
		_W_TABVIEW(control)->_currentHover = -1;
		_W_WIDGET(control)->state &= ~(STATE_CANVAS | STATE_THEME_BACKGROUND);

		/*
		 * Feature in Windows.  Despite the fact that the
		 * tool tip text contains \r\n, the tooltip will
		 * not honour the new line unless TTM_SETMAXTIPWIDTH
		 * is set.  The fix is to set TTM_SETMAXTIPWIDTH to
		 * a large value.
		 */
		HWND handle = _W_WIDGET(control)->handle;
		HWND hwndToolTip = (HWND) SendMessageW(handle, TCM_GETTOOLTIPS, 0, 0);
		SendMessageW(hwndToolTip, TTM_SETMAXTIPWIDTH, 0, 0x7FFF);

		if (_W_WIDGET(control)->style & W_RIGHT_TO_LEFT) {
			_W_WIDGET(control)->state |= STATE_CREATE_AS_RTL;
		}
		if (_W_WIDGET(control)->style & W_CLOSE) {
			SendMessageW(handle, TCM_SETPADDING, 0,
					MAKELPARAM(TABITEM_CLOSE_WIDTH + 3, 3));
		}
	}
	return result;
}
wresult _w_tabview_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	HWND handle = _W_WIDGET(widget)->handle;
	RECT rect;
	GetClientRect(handle, &rect);
	SendMessageW(handle, TCM_ADJUSTRECT, 0, (LPARAM) &rect);
	e->rect->x = rect.left;
	e->rect->y = rect.top;
	e->rect->width = rect.right - rect.left;
	e->rect->height = rect.bottom - rect.top;
	return W_TRUE;
}
wresult _w_tabview_get_imagelist(w_tabview *tabview, w_imagelist **imagelist) {
	*imagelist = _W_TABVIEW(tabview)->imagelist;
	return W_TRUE;
}
wresult _w_tabview_get_item(w_tabview *tabview, int index, w_tabitem *item) {
	HWND handle = _W_WIDGET(tabview)->handle;
	int count = SendMessageW(handle, TCM_GETITEMCOUNT, 0, 0);
	if (0 <= index && index < count) {
		W_WIDGETDATA(item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
		_W_ITEM(item)->parent = W_WIDGET(tabview);
		_W_ITEM(item)->index = index;
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_tabview_get_item_p(w_tabview *tabview, w_point *point,
		w_tabitem *item) {
	HWND handle = _W_WIDGET(tabview)->handle;
	TCHITTESTINFO pinfo;
	pinfo.pt.x = point->x;
	pinfo.pt.y = point->y;
	int index = SendMessageW(handle, TCM_HITTEST, 0, (LPARAM) &pinfo);
	if (index != -1) {
		W_WIDGETDATA(item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
		_W_ITEM(item)->parent = W_WIDGET(tabview);
		_W_ITEM(item)->index = index;
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_tabview_get_item_count(w_tabview *tabview) {
	HWND handle = _W_WIDGET(tabview)->handle;
	return SendMessageW(handle, TCM_GETITEMCOUNT, 0, 0);
}
wresult _w_tabview_get_items(w_tabview *tabview, w_iterator *items) {
	return W_FALSE;
}
wresult _w_tabview_get_selection(w_tabview *tabview, w_tabitem *item) {
	HWND handle = _W_WIDGET(tabview)->handle;
	int index = SendMessageW(handle, TCM_GETCURSEL, 0, 0);
	if (index != -1) {
		W_WIDGETDATA(item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
		_W_ITEM(item)->parent = W_WIDGET(tabview);
		_W_ITEM(item)->index = index;
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_tabview_insert_item(w_tabview *tabview, w_tabitem *item, int index) {
	HWND handle = _W_WIDGET(tabview)->handle;
	int count = SendMessageW(handle, TCM_GETITEMCOUNT, 0, 0);
	if (index < 0 || index > count) {
		index = count;
	}
	TCITEMW tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = L"";
	tcItem.cchTextMax = 1;
	int i = SendMessageW(handle, TCM_INSERTITEMW, index, (LPARAM) &tcItem);
	if (i == -1) {
		return W_ERROR_ITEM_NOT_ADDED;
	}
	if (item != 0) {
		W_WIDGETDATA(item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
		_W_ITEM(item)->parent = W_WIDGET(tabview);
		_W_ITEM(item)->index = i;
	}
	return W_TRUE;
}
wresult _w_tabview_set_imagelist(w_tabview *tabview, w_imagelist *imagelist) {
	wresult result = TRUE;
	HIMAGELIST _imagelist = 0;
	if (imagelist == 0) {
		_imagelist = 0;
		_W_TABVIEW(tabview)->imagelist = 0;
	} else {
		if (_W_IMAGELIST(imagelist)->imagelist == 0) {
			_imagelist = 0;
			_W_TABVIEW(tabview)->imagelist = 0;
			result = W_ERROR_INVALID_ARGUMENT;
		} else {
			_imagelist = _W_IMAGELIST(imagelist)->imagelist;
			_W_TABVIEW(tabview)->imagelist = imagelist;
		}
	}
	HWND handle = _W_WIDGET(tabview)->handle;
	SendMessageW(handle, TCM_SETIMAGELIST, 0, (LPARAM) _imagelist);
	return result;
}
wresult _w_tabview_set_selection_0(w_tabview *tabview, int index,
		boolean notify) {
	HWND handle = _W_WIDGET(tabview)->handle;
	_w_tabitem item;
	int oldIndex = SendMessageW(handle, TCM_GETCURSEL, 0, 0);
	if (oldIndex == index)
		return W_TRUE;
	W_WIDGETDATA(&item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
	_W_ITEM(&item)->parent = W_WIDGET(tabview);
	if (oldIndex != -1) {
		_W_ITEM(&item)->index = oldIndex;
		w_control *control = w_tabitem_get_control(W_TABITEM(&item));
		if (control != 0 && w_widget_is_ok(W_WIDGET(control)) > 0) {
			w_control_set_visible(control, W_FALSE);
		}
	}
	SendMessageW(handle, TCM_SETCURSEL, index, 0);
	int newIndex = SendMessageW(handle, TCM_GETCURSEL, 0, 0);
	if (newIndex != -1) {
		_W_ITEM(&item)->index = newIndex;
		w_control *control = w_tabitem_get_control(W_TABITEM(&item));
		if (control != 0 && w_widget_is_ok(W_WIDGET(control)) > 0) {
			w_rect r;
			w_scrollable_get_client_area(W_SCROLLABLE(tabview), &r);
			w_control_set_bounds(control, &r.pt, &r.sz);
			w_control_set_visible(control, W_TRUE);
		}
		if (notify) {
			w_event_tabview _e;
			_e.event.type = W_EVENT_ITEM_SELECTION;
			_e.event.widget = W_WIDGET(tabview);
			_e.event.platform_event = 0;
			_e.event.data = 0;
			_e.item = W_TABITEM(&item);
			return _w_widget_post_event(W_WIDGET(tabview), (w_event*) &_e,W_EVENT_SEND);
		}
	}
	return W_TRUE;
}
wresult _w_tabview_set_selection(w_tabview *tabview, int index) {
	HWND handle = _W_WIDGET(tabview)->handle;
	int count = SendMessageW(handle, TCM_GETITEMCOUNT, 0, 0);
	if (!(0 <= index && index < count))
		return W_FALSE;
	return _w_tabview_set_selection_0(tabview, index, W_FALSE);
}
DWORD _w_tabview_widget_style(w_control *control, _w_control_priv *priv) {
	/*
	 * Bug in Windows.  Under certain circumstances,
	 * when TCM_SETITEM is used to change the text
	 * in a tab item, the tab folder draws on top
	 * of the client area.  The fix is ensure that
	 * this cannot happen by setting WS_CLIPCHILDREN.
	 */
	DWORD bits = _w_composite_widget_style(control, priv) | WS_CLIPCHILDREN;
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_NO_FOCUS) != 0)
		bits |= TCS_FOCUSNEVER;
	if ((style & W_BOTTOM) != 0)
		bits |= TCS_BOTTOM;
	if (style & W_CLOSE) {
		//bits |= TCS_OWNERDRAWFIXED;
		bits |= TCS_FORCEICONLEFT | TCS_FORCELABELLEFT;
	}
	return bits | TCS_TABS | TCS_TOOLTIPS;
}
WCHAR* _w_tabview_window_class(w_control *control, _w_control_priv *priv) {
	return WC_TABCONTROLW;
}
/*
 * messages
 */
void _w_tabview_get_close_rect(w_widget *widget, RECT *itemRect,
		RECT *closeRect) {
	closeRect->left = itemRect->right - TABITEM_CLOSE_WIDTH - 2;
	closeRect->top = (itemRect->top + itemRect->bottom - TABITEM_CLOSE_HEIGTH)
			/ 2;
	closeRect->right = itemRect->right - 2;
	closeRect->bottom =
			(itemRect->top + itemRect->bottom + TABITEM_CLOSE_HEIGTH) / 2;
}
wresult _TABVIEW_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_SIZE(widget, e, priv);
	TCITEMW tcItem;
	int index = SendMessageW(e->hwnd, TCM_GETCURSEL, 0, 0);
	if (index != -1) {
		tcItem.mask = TCIF_PARAM;
		tcItem.lParam = 0;
		SendMessageW(e->hwnd, TCM_GETITEMW, index, (LPARAM) &tcItem);
		w_control *control = (w_control*) tcItem.lParam;
		if (control != 0) {
			w_rect r;
			w_scrollable_get_client_area(W_SCROLLABLE(widget), &r);
			w_control_set_bounds(control, &r.pt, &r.sz);
		}
	}
	return result;
}
wresult _TABVIEW_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMHDR *hdr = (NMHDR*) e->lparam;
	switch (hdr->code) {
	case TCN_SELCHANGE:
	case TCN_SELCHANGING: {
		w_tabitem item;
		int index = SendMessageW(hdr->hwndFrom, TCM_GETCURSEL, 0, 0);
		if (index != -1) {
			W_WIDGETDATA(&item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(widget);
			_W_ITEM(&item)->parent = widget;
			_W_ITEM(&item)->index = index;
			w_control *control = (w_control*) w_tabitem_get_control(&item);
			if (control != 0) {
				if (hdr->code == TCN_SELCHANGE) {
					w_rect r;
					w_scrollable_get_client_area(W_SCROLLABLE(widget), &r);
					w_control_set_bounds(control, &r.pt, &r.sz);
				}
				wresult visible;
				if (hdr->code == TCN_SELCHANGE)
					visible = W_TRUE;
				else
					visible = W_FALSE;
				w_control_set_visible(control, visible);
			}
		}
		if (hdr->code == TCN_SELCHANGE) {
			w_event_tabview _e;
			W_WIDGETDATA(&item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(widget);
			_W_ITEM(&item)->parent = widget;
			_W_ITEM(&item)->index = index;
			_e.event.type = W_EVENT_ITEM_SELECTION;
			_e.event.widget = widget;
			_e.event.platform_event = _EVENT_PLATFORM(e);
			_e.event.data = 0;
			_e.item = &item;
			return _w_widget_post_event(widget, (w_event*) &_e,W_EVENT_SEND);
		}
	}
	}
	return W_TRUE;
}
wresult _TABVIEW_WM_LBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_LBUTTONUP(widget, e, priv);
	if (_W_WIDGET(widget)->style & W_CLOSE) {
		HWND handle = _W_WIDGET(widget)->handle;
		TCHITTESTINFO pinfo;
		POINT pt;
		pt.x = GET_X_LPARAM(e->lparam);
		pt.y = GET_Y_LPARAM(e->lparam);
		pinfo.pt.x = pt.x;
		pinfo.pt.y = pt.y;
		pinfo.flags = 0;
		int iTabNow = (int) SendMessageW(handle, TCM_HITTEST, 0,
				(LPARAM) &pinfo);
		if (iTabNow != -1) {
			RECT itemRect, closeRect;
			SendMessageW(handle, TCM_GETITEMRECT, iTabNow, (LPARAM) &itemRect);
			_w_tabview_get_close_rect(widget, &itemRect, &closeRect);
			if (PtInRect(&closeRect, pt)) {
				w_event_tabview _e;
				_w_tabitem item;
				W_WIDGETDATA(&item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(widget);
				_W_ITEM(&item)->parent = widget;
				_W_ITEM(&item)->index = iTabNow;
				_e.event.type = W_EVENT_ITEM_CLOSE;
				_e.event.widget = widget;
				_e.event.platform_event = _EVENT_PLATFORM(e);
				_e.event.data = 0;
				_e.item = W_TABITEM(&item);
				_e.control = 0;
				_w_widget_post_event(widget, (w_event*) &_e,W_EVENT_SEND);
			}
		}
	}
	return result;
}
wresult _TABVIEW_WM_MOUSEMOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_MOUSEMOVE(widget, e, priv);
	if (_W_WIDGET(widget)->style & W_CLOSE) {
		HWND handle = _W_WIDGET(widget)->handle;
		w_themedata data;
		memset(&data, 0, sizeof(data));
		BOOL isFromTabToTab = FALSE;
		TCHITTESTINFO pinfo;
		POINT pt;
		pt.x = GET_X_LPARAM(e->lparam);
		pt.y = GET_Y_LPARAM(e->lparam);
		pinfo.pt.x = pt.x;
		pinfo.pt.y = pt.y;
		pinfo.flags = 0;
		int iTabNow = (int) SendMessageW(handle, TCM_HITTEST, 0,
				(LPARAM) &pinfo);
		int _currentHoverTabItem = _W_TABVIEW(widget)->_currentHover;
		if (_currentHoverTabItem == iTabNow && _currentHoverTabItem != -1) {
			// mouse moves arround in the same tab
			// do nothing
		} else if (iTabNow == -1 && _currentHoverTabItem != -1) {
			// mouse is no more on any tab, set hover -1
			_currentHoverTabItem = -1;

			// send mouse leave notif
		} else if (iTabNow != -1 && _currentHoverTabItem == -1) {
			// mouse is just entered in a tab zone
			_currentHoverTabItem = iTabNow;
		} else if (iTabNow != -1 && _currentHoverTabItem != -1
				&& _currentHoverTabItem != iTabNow) {
			// mouse is being moved from a tab and entering into another tab
			isFromTabToTab = TRUE;

			// set current hovered
			_currentHoverTabItem = iTabNow;
		} else if (iTabNow == -1 && _currentHoverTabItem == -1) // mouse is already outside
				{
			// do nothing
		}

		/* draw tab close button */
		RECT currentHoverTabRectOld, _currentHoverTabRect, closeRect;
		SendMessageW(handle, TCM_GETITEMRECT, _currentHoverTabItem,
				(LPARAM) &currentHoverTabRectOld);
		BOOL isCloseHoverOld = (_W_WIDGET(widget)->state & STATE_CLOSE_HOVER)
				!= 0;
		BOOL _isCloseHover;

		if (_currentHoverTabItem != -1) // is hovering
				{
			SendMessageW(handle, TCM_GETITEMRECT, _currentHoverTabItem,
					(LPARAM) &_currentHoverTabRect);
			_w_tabview_get_close_rect(widget, &_currentHoverTabRect,
					&closeRect);
			if (PtInRect(&closeRect, pt)) {
				_W_WIDGET(widget)->state |= STATE_CLOSE_HOVER;
				_isCloseHover = TRUE;
			} else {
				_W_WIDGET(widget)->state &= ~STATE_CLOSE_HOVER;
				_isCloseHover = FALSE;
			}
		} else {
			SetRectEmpty(&_currentHoverTabRect);
			_W_WIDGET(widget)->state &= ~STATE_CLOSE_HOVER;
			_isCloseHover = FALSE;
		}
		if (isFromTabToTab || _isCloseHover != isCloseHoverOld) {
			if (isCloseHoverOld && (isFromTabToTab || !_isCloseHover)) {
				_w_tabview_get_close_rect(widget, &currentHoverTabRectOld,
						&closeRect);
				InvalidateRect(handle, &closeRect, FALSE);
			}
			if (_isCloseHover) {
				_w_tabview_get_close_rect(widget, &_currentHoverTabRect,
						&closeRect);
				InvalidateRect(handle, &closeRect, FALSE);
			}
		}
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = handle;
		tme.dwHoverTime = 0;
		tme.dwFlags = TME_LEAVE;
		TrackMouseEvent(&tme);
	}
	return result;
}
wresult _TABVIEW_WM_MOUSELEAVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_MOUSELEAVE(widget, e, priv);
	if (_W_WIDGET(widget)->style & W_CLOSE) {
		HWND handle = _W_WIDGET(widget)->handle;
		if ((_W_WIDGET(widget)->state & STATE_CLOSE_HOVER) != 0) {
			RECT r;
			SendMessageW(handle, TCM_GETITEMRECT,
			_W_TABVIEW(widget)->_currentHover, (LPARAM) &r);
			InvalidateRect(handle, &r, FALSE);
		}
		_W_TABVIEW(widget)->_currentHover = -1;
		_W_WIDGET(widget)->state &= ~ STATE_CLOSE_HOVER;
	}
	return result;
}
wresult _TABVIEW_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HWND handle = _W_WIDGET(widget)->handle;
	_w_control_priv *cpriv = _W_CONTROL_PRIV(priv);
	WPARAM lastWparam = e->wparam;
	PAINTSTRUCT ps;
	HDC hdc, hDCMem;
	HBITMAP hMemBmp, hOldBmp;
	RECT rcWnd;
	int width;
	int height;
	if (e->wparam == 0) {
		hdc = BeginPaint(handle, &ps);
	} else {
		hdc = (HDC) e->wparam;
	}
	GetClientRect(handle, &rcWnd);
	width = rcWnd.right - rcWnd.left;
	height = rcWnd.bottom - rcWnd.top;
	// create a new DC based on the target HDC
	hDCMem = CreateCompatibleDC(hdc);
	// create a bitmap that is compatible with the target DC
	hMemBmp = CreateCompatibleBitmap(hdc, width, height);
	// select the new bitmap in to the DC, saving the old bitmap
	hOldBmp = (HBITMAP) SelectObject(hDCMem, hMemBmp);
	e->wparam = (WPARAM) hDCMem;
	WNDPROC def_window_proc = *cpriv->get_def_window_proc(W_CONTROL(widget),
			cpriv);
	e->result = CallWindowProcW(def_window_proc, e->hwnd, e->msg, e->wparam,
			e->lparam);
	HTHEME hThemeWindow = OpenThemeData(NULL, L"WINDOW");
	HTHEME hThemeTool = OpenThemeData(NULL, L"TOOLBAR");
	HTHEME hThemeTab = OpenThemeData(NULL, L"TAB");
	if (hThemeWindow != 0) {
		int count = SendMessageW(handle, TCM_GETITEMCOUNT, 0, 0);
		int sel = SendMessageW(handle, TCM_GETCURSEL, 0, 0);
		int focus = SendMessageW(handle, TCM_GETCURFOCUS, 0, 0);
		RECT itemRect, closeRect;
		POINT pt;
		int iStateId;
		for (int i = 0; i < count; i++) {
			SendMessageW(handle, TCM_GETITEMRECT, i, (LPARAM) &itemRect);
			if (RectVisible(hdc, &itemRect)) {
				GetCursorPos(&pt);
				ScreenToClient(handle, &pt);
				/* draw item*/
				/*iStateId = TIS_NORMAL;
				 if (sel == i)
				 iStateId = TIS_SELECTED;
				 DrawThemeBackground(hThemeTab, hDCMem, TABP_TABITEM, iStateId,
				 &itemRect, NULL);*/
				/* draw close button */
				_w_tabview_get_close_rect(widget, &itemRect, &closeRect);
				iStateId = CBS_NORMAL;
				if (PtInRect(&closeRect, pt)) {
					iStateId = CBS_PUSHED;
					DrawThemeBackground(hThemeTool, hDCMem, TP_BUTTON, TS_HOT,
							&closeRect, NULL);
				}
				closeRect.left += 1;
				closeRect.top += 1;
				closeRect.right -= 1;
				closeRect.bottom -= 1;
				DrawThemeBackground(hThemeWindow, hDCMem, WP_SMALLCLOSEBUTTON,
						iStateId, &closeRect, NULL);
			}
		}
		CloseThemeData(hThemeTab);
		CloseThemeData(hThemeTool);
		CloseThemeData(hThemeWindow);
	}
	// copy all the bits from our new DC over to the target DC
	BitBlt(hdc, rcWnd.left, rcWnd.top, width, height, hDCMem, 0, 0,
	SRCCOPY);
	// select the original bitmap the DC came with
	SelectObject(hDCMem, hOldBmp);
	// delete our bitmap
	DeleteObject(hMemBmp);
	// delete the DC
	DeleteDC(hDCMem);
	if (lastWparam == 0)
		EndPaint(handle, &ps);
	e->wparam = lastWparam;
	return W_TRUE;
}
wresult _w_tabview_call_window_proc(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HWND handle = _W_WIDGET(widget)->handle;
	if (handle == 0) {
		e->result = 0;
		return W_FALSE;
	} else {
		_w_control_priv *cpriv = _W_CONTROL_PRIV(priv);
		WPARAM lastWparam = e->wparam;
		PAINTSTRUCT ps;
		HDC hdc, hDCMem;
		HBITMAP hMemBmp, hOldBmp;
		RECT rcWnd;
		int width;
		int height;
		if ((_W_WIDGET(widget)->style & W_CLOSE) != 0 && e->msg == WM_PAINT) {
			_TABVIEW_WM_PAINT(widget, e, priv);
		} else {
			WNDPROC def_window_proc = *cpriv->get_def_window_proc(
					W_CONTROL(widget), cpriv);
			e->result = CallWindowProcW(def_window_proc, e->hwnd, e->msg,
					e->wparam, e->lparam);
		}
		return W_TRUE;
	}
}
void _w_tabview_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_tabview_class *clazz) {
	if (classId == _W_CLASS_TABVIEW) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_tabview_priv;
	}
	_w_composite_class_init(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TABVIEW;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tabview_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tabview);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tabview);

	clazz->get_imagelist = _w_tabview_get_imagelist;
	clazz->get_item = _w_tabview_get_item;
	clazz->get_item_p = _w_tabview_get_item_p;
	clazz->get_item_count = _w_tabview_get_item_count;
	clazz->get_items = _w_tabview_get_items;
	clazz->get_selection = _w_tabview_get_selection;
	clazz->insert_item = _w_tabview_insert_item;
	clazz->set_imagelist = _w_tabview_set_imagelist;
	clazz->set_selection = _w_tabview_set_selection;
	/*
	 * class item
	 */
	struct _w_tabitem_class *item = clazz->class_tabitem;
	W_WIDGETDATA_CLASS(item)->toolkit = W_WIDGET_CLASS(clazz)->toolkit;
	_w_item_class_init(W_ITEM_CLASS(item));
	item->get_bounds = _w_tabitem_get_bounds;
	item->get_image = _w_tabitem_get_image;
	item->set_image = _w_tabitem_set_image;
	item->remove = _w_tabitem_remove;
	item->pack = _w_tabitem_pack;
	item->item.get_data = _w_tabitem_get_data;
	item->item.get_text = _w_tabitem_get_text;
	item->item.set_data = _w_tabitem_set_data;
	item->item.set_text = _w_tabitem_set_text;
	/*
	 * priv
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_TABVIEW) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->check_style = _w_tabview_check_style;
		priv->compute_size = _w_tabview_compute_size;
		priv->compute_trim = _w_tabview_compute_trim;
		priv->create_handle = _w_tabview_create_handle;
		priv->get_client_area = _w_tabview_get_client_area;
		priv->widget_style = _w_tabview_widget_style;
		priv->window_class = _w_tabview_window_class;
		priv->widget.call_window_proc = _w_tabview_call_window_proc;
		/*
		 * messages
		 */
		dispatch_message *msg = priv->messages;
		msg[_WM_SIZE] = _TABVIEW_WM_SIZE;
		msg[_WM_NOTIFYCHILD] = _TABVIEW_WM_NOTIFYCHILD;
		msg[_WM_MOUSEMOVE] = _TABVIEW_WM_MOUSEMOVE;
		msg[_WM_MOUSELEAVE] = _TABVIEW_WM_MOUSELEAVE;
		msg[_WM_LBUTTONUP] = _TABVIEW_WM_LBUTTONUP;
	}
}
