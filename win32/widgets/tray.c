/*
 * Name:        tray.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "tray.h"
#include "toolkit.h"
void _w_trayitem_create_update_widget(w_trayitem *trayitem, int newIcon) {
	_w_trayitem *_trayitem = _W_TRAYITEM(trayitem);
	NOTIFYICONDATAW iconData;
	iconData.uID =
			newIcon ?
					(_trayitem->id = win_toolkit->nextTrayId++) : _trayitem->id;
	iconData.hWnd = win_toolkit->hwndMessage;
	iconData.uFlags = NIF_MESSAGE;
	iconData.uCallbackMessage = SWT_TRAYICONMSG;
	/*
	 * NIM_ADD message should be called only once in a TrayItem instance
	 * life-cycle i.e. in the TrayItem instance creation step only, else
	 * will lead to multiple TrayIcons entries on Win10 refer bug 488739.
	 */
	Shell_NotifyIconW((newIcon ? NIM_ADD : NIM_MODIFY), &iconData);
}
wresult _w_trayitem_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (w_widget_is_ok(parent) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (w_widget_class_id(parent) != _W_CLASS_TRAY)
		return W_ERROR_INVALID_ARGUMENT;
	_w_tray *_tray = _W_TRAY(parent);
	_w_trayitem *_item = _W_TRAYITEM(widget);
	_item->parent = parent;
	_W_WIDGET(widget)->post_event = post_event;
	_W_WIDGET(widget)->style = style;
	_w_trayitem_create_update_widget(W_TRAYITEM(widget), W_TRUE);
	_item->next = 0;
	if (_tray->items == 0) {
		_tray->items = _item;
		_item->prev = _item; //last
	} else {
		_w_trayitem *last = _tray->items->prev;
		last->next = _item;
		_item->prev = last;
		_tray->items->prev = _item;
	}
	_tray->count++;
	return W_TRUE;
}
wresult _w_trayitem_post_event(w_widget *widget, w_event *e,int flags) {
	return W_FALSE;
}

wresult _w_trayitem_get_tooltip(w_trayitem *trayitem, w_tooltip **tooltip) {
	*tooltip = _W_TRAYITEM(trayitem)->tooltip;
	return W_TRUE;
}
wresult _w_trayitem_get_tooltip_text(w_trayitem *trayitem, w_alloc alloc,
		void *user_data, int enc) {
	return _win_text_set_0(_W_TRAYITEM(trayitem)->tooltiptext, -1, alloc,
			user_data, enc);
}
wresult _w_trayitem_get_visible(w_trayitem *trayitem) {
	return _W_TRAYITEM(trayitem)->visible;
}
wresult _w_trayitem_set_icon(w_trayitem *trayitem, HICON icon) {
	_w_trayitem *_trayitem = _W_TRAYITEM(trayitem);
	NOTIFYICONDATAW iconData;
	iconData.cbSize = sizeof(iconData);
	iconData.uID = _trayitem->id;
	iconData.hWnd = win_toolkit->hwndMessage;
	iconData.hIcon = icon;
	iconData.uFlags = NIF_ICON;
	Shell_NotifyIconW(NIM_MODIFY, &iconData);
	return W_TRUE;
}
wresult _w_trayitem_set_image(w_trayitem *trayitem, w_image *image) {
	HICON hicon = 0;
	ICONINFO info;
	if (_W_IMAGE(image)->handle != 0) {
		switch (_W_IMAGE(image)->type) {
		case _IMAGE_HBITMAP:
			info.fIcon = TRUE;
			info.hbmColor = (HBITMAP) _W_IMAGE(image)->handle;
			info.hbmMask = 0;
			hicon = CreateIconIndirect(&info);
			break;
		case _IMAGE_ICON:
			hicon = CopyIcon((HICON) _W_IMAGE(image)->handle);
			break;
		case _IMAGE_GPBITMAP:
			GdipCreateHICONFromBitmap((GpImage*) _W_IMAGE(image)->handle,
					&hicon);
			break;
		}
	}
	_W_TRAYITEM(trayitem)->icon = hicon;
	return _w_trayitem_set_icon(trayitem, hicon);
}
wresult _w_trayitem_set_tooltip(w_trayitem *trayitem, w_tooltip *toolTip) {
	w_tooltip *oldTip = _W_TRAYITEM(trayitem)->tooltip, *newTip = toolTip;
	if (oldTip != 0)
		_W_TOOLTIP(oldTip)->item = 0;
	_W_TRAYITEM(trayitem)->tooltip = newTip;
	if (newTip != 0)
		_W_TOOLTIP(newTip)->item = trayitem;
	return W_TRUE;
}
wresult _w_trayitem_set_tooltip_text_0(w_trayitem *trayitem,
		const char *string) {
	NOTIFYICONDATAW iconData;
	if (string == 0) {
		iconData.szTip[0] = 0;
	} else {
		/*
		 * Note that the size of the szTip field is different in version 5.0 of shell32.dll.
		 */
		int length = win_toolkit->shell32_version < VERSION(5, 0) ? 64 : 128;
		w_utf8_to_utf16(string, -1, iconData.szTip, length);
		iconData.szTip[length - 1] = 0;
	}
	iconData.cbSize = sizeof(iconData);
	iconData.uID = _W_TRAYITEM(trayitem)->id;
	iconData.hWnd = win_toolkit->hwndMessage;
	iconData.uFlags = NIF_TIP;
	Shell_NotifyIconW(NIM_MODIFY, &iconData);
	return W_TRUE;
}
wresult _w_trayitem_set_tooltip_text(w_trayitem *trayitem, const char *string,
		int length, int enc) {
	wresult result = _win_text_copy(&_W_TRAYITEM(trayitem)->tooltiptext, string,
			length, enc);
	if (result > 0) {
		_w_trayitem_set_tooltip_text_0(trayitem,
				_W_TRAYITEM(trayitem)->tooltiptext);
	}
	return result;
}
wresult _w_trayitem_set_visible(w_trayitem *trayitem, int visible) {
	_w_trayitem *_trayitem = _W_TRAYITEM(trayitem);
	if (_trayitem->visible == visible)
		return W_TRUE;
	w_event e;
	if (visible) {
		/*
		 * It is possible (but unlikely), that application
		 * code could have disposed the widget in the show
		 * event.  If this happens, just return.
		 */
		e.type = W_EVENT_SHOW;
		e.widget = W_WIDGET(trayitem);
		e.time = 0;
		e.data = 0;
		e.platform_event = 0;
		_w_widget_post_event(W_WIDGET(trayitem), &e, W_EVENT_SEND);
		if (w_widget_is_ok(W_WIDGET(trayitem)) < 0)
			return W_FALSE;
	}
	_trayitem->visible = visible;
	NOTIFYICONDATAW iconData;
	iconData.cbSize = sizeof(iconData);
	iconData.uID = _trayitem->id;
	iconData.hWnd = win_toolkit->hwndMessage;
	if (win_toolkit->shell32_version < VERSION(5, 0)) {
		if (visible) {
			iconData.uFlags = NIF_MESSAGE;
			iconData.uCallbackMessage = SWT_TRAYICONMSG;
			Shell_NotifyIconW(NIM_MODIFY, &iconData);
			_w_trayitem_set_icon(trayitem, _trayitem->icon);
			_w_trayitem_set_tooltip_text_0(trayitem, _trayitem->tooltiptext);
		} else {
			Shell_NotifyIconW(NIM_DELETE, &iconData);
		}
	} else {
		iconData.uFlags = NIF_STATE;
		iconData.dwState = visible ? 0 : NIS_HIDDEN;
		iconData.dwStateMask = NIS_HIDDEN;
		Shell_NotifyIconW(NIM_MODIFY, &iconData);
	}
	if (!visible) {
		e.type = W_EVENT_HIDE;
		e.widget = W_WIDGET(trayitem);
		e.time = 0;
		e.data = 0;
		e.platform_event = 0;
		_w_widget_post_event(W_WIDGET(trayitem), &e, W_EVENT_SEND);
	}
	return W_TRUE;
}
LRESULT CALLBACK messageProc(HWND hwnd, UINT msg, WPARAM wParam,
		LPARAM lParam) {
	return 0;
}
/*
 * tray
 */
wresult _w_tray_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_FALSE;
}
wresult _w_tray_post_event(w_widget *widget, w_event *e,int flags) {
	return W_FALSE;
}

wresult _w_tray_get_item(w_tray *tray, int index, w_trayitem **item) {
	if (index >= 0 && index < _W_TRAY(tray)->count) {
		_w_trayitem *_item = _W_TRAY(tray)->items;
		int i = 0;
		while (_item != 0) {
			if (i == index) {
				*item = W_TRAYITEM(_item);
				return W_TRUE;
			}
			i++;
			_item = _item->next;
		}
	}
	return W_FALSE;
}
wresult _w_tray_get_item_count(w_tray *tray) {
	return _W_TRAY(tray)->count;
}
wresult _w_tray_get_items(w_tray *tray, w_iterator *items) {
	return W_FALSE;
}
void _w_tray_class_init(w_toolkit *toolkit, wushort classId,struct _w_tray_class *clazz) {
	_w_widget_class_init(toolkit,classId,W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TRAY;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tray_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tray);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tray);
	/*
	 * public function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_tray_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_tray_create;
	clazz->get_item = _w_tray_get_item;
	clazz->get_item_count = _w_tray_get_item_count;
	clazz->get_items = _w_tray_get_items;
}
void _w_trayitem_class_init(w_toolkit *toolkit, wushort classId,struct _w_trayitem_class *item) {
	_w_widget_class_init(toolkit,classId,W_WIDGET_CLASS(item));
	W_WIDGET_CLASS(item)->class_id = _W_CLASS_TRAYITEM;
	W_WIDGET_CLASS(item)->class_size = sizeof(struct _w_trayitem_class);
	W_WIDGET_CLASS(item)->object_total_size = sizeof(w_trayitem);
	W_WIDGET_CLASS(item)->object_used_size = sizeof(_w_trayitem);
	/*
	 * public function
	 */
	W_WIDGET_CLASS(item)->post_event = _w_trayitem_post_event;
	W_WIDGET_CLASS(item)->create = _w_trayitem_create;
	item->get_tooltip = _w_trayitem_get_tooltip;
	item->get_tooltip_text = _w_trayitem_get_tooltip_text;
	item->get_visible = _w_trayitem_get_visible;
	item->set_image = _w_trayitem_set_image;
	item->set_tooltip = _w_trayitem_set_tooltip;
	item->set_tooltip_text = _w_trayitem_set_tooltip_text;
	item->set_visible = _w_trayitem_set_visible;
}
