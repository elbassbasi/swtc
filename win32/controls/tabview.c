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
	return W_FALSE;
}
wresult _w_tabview_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	return W_FALSE;
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
			w_themedata data;
			w_rect bounds, result;
			memset(&bounds, 0, sizeof(bounds));
			bounds.width = 300;
			bounds.height = 300;
			memset(&data, 0, sizeof(data));
			data.clazz = W_THEME_CLASS_TABVIEW;
			data.part = W_THEME_TABITEM_CLOSE;
			data.style = _W_WIDGET(control)->style;
			w_toolkit *toolkit = w_widget_get_toolkit(W_WIDGET(control));
			w_theme *theme = w_toolkit_get_theme(toolkit);
			w_theme_get_bounds(theme, &data, &bounds, &result);
			int width = result.width;
			SendMessageW(handle, TCM_SETPADDING, 0, MAKELPARAM(width, 3));
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
		_W_WIDGETDATA(item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
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
		_W_WIDGETDATA(item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
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
		_W_WIDGETDATA(item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
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
		_W_WIDGETDATA(item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
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
	_W_WIDGETDATA(&item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(tabview);
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
			_e.event.widget = tabview;
			_e.event.platform_event = 0;
			_e.event.data = 0;
			_e.item = W_TABITEM(&item);
			return _w_widget_send_event(W_WIDGET(tabview), (w_event*) &_e);
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
		bits |= TCS_OWNERDRAWFIXED;
		//bits &= ~WS_CLIPCHILDREN;
	}
	return bits | TCS_TABS | TCS_TOOLTIPS;
}
const char* _w_tabview_window_class(w_control *control, _w_control_priv *priv) {
	return WC_TABCONTROLA;
}
/*
 * messages
 */
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
			_W_WIDGETDATA(&item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(widget);
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
			_W_WIDGETDATA(&item)->clazz = _W_TABVIEW_GET_ITEM_CLASS(widget);
			_W_ITEM(&item)->parent = widget;
			_W_ITEM(&item)->index = index;
			_e.event.type = W_EVENT_ITEM_SELECTION;
			_e.event.widget = widget;
			_e.event.platform_event = _EVENT_PLATFORM(e);
			_e.event.data = 0;
			_e.item = &item;
			return _w_widget_send_event(widget, (w_event*) &_e);
		}
	}
	}
	return W_TRUE;
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
		w_point p;
		p.x = GET_X_LPARAM(e->lparam);
		p.y = GET_Y_LPARAM(e->lparam);
		pinfo.pt.x = p.x;
		pinfo.pt.y = p.y;
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
			//notify(TCN_MOUSELEAVING, -1);
		} else if (iTabNow != -1 && _currentHoverTabItem == -1) {
			// mouse is just entered in a tab zone
			_currentHoverTabItem = iTabNow;

			//notify(TCN_MOUSEHOVERING, _currentHoverTabItem);
		} else if (iTabNow != -1 && _currentHoverTabItem != -1
				&& _currentHoverTabItem != iTabNow) {
			// mouse is being moved from a tab and entering into another tab
			isFromTabToTab = TRUE;
			//_whichCloseClickDown = -1;

			// set current hovered
			_currentHoverTabItem = iTabNow;

			// send mouse enter notif
			//notify(TCN_MOUSEHOVERSWITCHING, _currentHoverTabItem);
		} else if (iTabNow == -1 && _currentHoverTabItem == -1) // mouse is already outside
				{
			// do nothing
		}

		/* draw tab close button */
		RECT currentHoverTabRectOld, _currentHoverTabRect;
		SendMessageW(handle, TCM_GETITEMRECT, _currentHoverTabItem,
				(LPARAM) &currentHoverTabRectOld);
		BOOL isCloseHoverOld = (_W_WIDGET(widget)->state & STATE_CLOSE_HOVER)
				!= 0;
		BOOL _isCloseHover;

		if (_currentHoverTabItem != -1) // is hovering
				{
			SendMessageW(handle, TCM_GETITEMRECT, _currentHoverTabItem,
					(LPARAM) &_currentHoverTabRect);
			w_toolkit *toolkit = w_widget_get_toolkit(widget);
			w_theme *theme = w_toolkit_get_theme(toolkit);
			w_rect bounds;
			data.clazz = W_THEME_CLASS_TABVIEW;
			data.part = W_THEME_TABITEM;
			data.style = _W_WIDGET(widget)->style;
			data.state = 0;
			bounds.x = _currentHoverTabRect.left;
			bounds.y = _currentHoverTabRect.top;
			bounds.width = _currentHoverTabRect.right
					- _currentHoverTabRect.left;
			bounds.height = _currentHoverTabRect.bottom
					- _currentHoverTabRect.top;
			int part_select = w_theme_hit_background(theme, &data, &p, &bounds);
			if (part_select == W_THEME_TABITEM_CLOSE) {
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
			if (isCloseHoverOld && (isFromTabToTab || !_isCloseHover))
				InvalidateRect(handle, &currentHoverTabRectOld, FALSE);

			if (_isCloseHover)
				InvalidateRect(handle, &_currentHoverTabRect, FALSE);
		}
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = handle;
		tme.dwHoverTime = 0;
		tme.dwFlags = TME_LEAVE;
		if (_isCloseHover) {
			// Mouse moves out from close zone will send WM_MOUSELEAVE message
			TrackMouseEvent(&tme);
		}
		// Mouse moves out from tab zone will send WM_MOUSELEAVE message
		// but it doesn't track mouse moving from a tab to another
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
		//_whichCloseClickDown = -1;
		_W_WIDGET(widget)->state &= ~ STATE_CLOSE_HOVER;

		//notify(TCN_MOUSELEAVING, _currentHoverTabItem);
	}
	return result;
}
wresult _TABVIEW_WM_DRAWCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_WIDGET(widget)->style & W_CLOSE) {
		DRAWITEMSTRUCT *lpDIS = (DRAWITEMSTRUCT*) e->lparam;
		HWND handle = _W_WIDGET(widget)->handle;
		int index = lpDIS->itemID;
		TC_ITEMW tci;
		_w_graphics gc;
		w_themedata data;
		w_rect bounds;
		RECT rect;
		tci.mask = TCIF_IMAGE | TCIF_STATE;
		tci.dwState = 0;
		SendMessageW(handle, TCM_GETITEMW, index, (LPARAM) &tci);
		SendMessageW(handle, TCM_GETITEMRECT, index, (LPARAM) &rect);
		bounds.x = rect.left;
		bounds.y = rect.top;
		bounds.width = rect.right - rect.left;
		bounds.height = rect.bottom - rect.top;
		if ((lpDIS->itemState & ODS_SELECTED) != 0) {
			bounds.x -= 1;
			bounds.y -= 1;
			bounds.width += 0;
			bounds.height += 0;
		} else {
			bounds.x += 0;
			bounds.y -= 1;
			bounds.width += 0;
			bounds.height += 4;
		}
		int nSavedDC = SaveDC(lpDIS->hDC);
		_w_control_init_graphics(W_CONTROL(widget), W_GRAPHICS(&gc), lpDIS->hDC,
				0);
		w_toolkit *toolkit = w_widget_get_toolkit(widget);
		w_theme *theme = w_toolkit_get_theme(toolkit);
		memset(&data, 0, sizeof(data));
		data.clazz = W_THEME_CLASS_TABVIEW;
		data.part = W_THEME_TABITEM;
		data.style = _W_WIDGET(widget)->style;
		data.state = 0;
		/* draw item background */
		if ((lpDIS->itemState & ODS_SELECTED) != 0)
			data.state |= W_THEME_SELECTED;
		if ((lpDIS->itemState & ODS_HOTLIGHT) != 0)
			data.state |= W_THEME_HOT;
		if ((lpDIS->itemState & ODS_FOCUS) != 0)
			data.state |= W_THEME_FOCUSED;
		if ((lpDIS->itemState & ODS_SELECTED) != 0)
			data.state |= W_THEME_SELECTED;
		if ((lpDIS->itemState & ODS_DISABLED) != 0)
			data.state |= W_THEME_DISABLED;
		w_graphics_set_clipping_rect(W_GRAPHICS(&gc), &bounds);
		w_theme_draw_background(theme, &data, W_GRAPHICS(&gc), &bounds);
		/* draw item image */
		w_imagelist *imagelist = _W_TABVIEW(widget)->imagelist;
		int imageWidth = 0;
		if (imagelist != 0 && tci.iImage >= 0) {
			w_size _sz;
			imageWidth = w_imagelist_get_size(imagelist, &_sz);
			imageWidth = _sz.width + 3;
			bounds.x += 3;
			w_theme_draw_image_index(theme, &data, W_GRAPHICS(&gc), &bounds,
					imagelist, tci.iImage,
					W_THEME_DRAW_LEFT | W_THEME_DRAW_VCENTER);
			bounds.x -= 3;
		}
		/* draw item text */
		size_t size = 0;
		WCHAR *str = 0;
		int length = 0;
		bounds.x += imageWidth + 2;
		_w_tabitem_get_text_unicode(handle, index, &str, &length, &size);
		w_theme_draw_text(theme, &data, W_GRAPHICS(&gc), &bounds,
				(const char*) str, length, W_ENCODING_UNICODE,
				W_THEME_DRAW_LEFT | W_THEME_DRAW_VCENTER);
		_w_toolkit_free(str, size);
		bounds.x -= imageWidth + 2;

		/* Draw item close button */
		w_rect rcCloseButton;
		data.part = W_THEME_TABITEM_CLOSE;
		w_theme_get_bounds(theme, &data, &bounds, &rcCloseButton);
		w_theme_draw_background(theme, &data, W_GRAPHICS(&gc), &rcCloseButton);
		RestoreDC(lpDIS->hDC, nSavedDC);
		return TRUE;
	}
	return FALSE;
}
void _w_tabview_class_init(struct _w_tabview_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
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
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->check_style = _w_tabview_check_style;
	priv->compute_size = _w_tabview_compute_size;
	priv->compute_trim = _w_tabview_compute_trim;
	priv->create_handle = _w_tabview_create_handle;
	priv->get_client_area = _w_tabview_get_client_area;
	priv->widget_style = _w_tabview_widget_style;
	priv->window_class = _w_tabview_window_class;
	/*
	 * messages
	 */
	dispatch_message *msg = priv->messages;
	msg[_WM_SIZE] = _TABVIEW_WM_SIZE;
	msg[_WM_NOTIFYCHILD] = _TABVIEW_WM_NOTIFYCHILD;
	msg[_WM_MOUSEMOVE] = _TABVIEW_WM_MOUSEMOVE;
	msg[_WM_MOUSELEAVE] = _TABVIEW_WM_MOUSELEAVE;
	msg[_WM_DRAWCHILD] = _TABVIEW_WM_DRAWCHILD;
}
