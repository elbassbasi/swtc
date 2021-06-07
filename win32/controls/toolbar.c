/*
 * toolbar.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "toolbar.h"
#include "../widgets/toolkit.h"
//#define TOOLITEM_RADIO (1 << 15)
#define TOOLITEM_SEP (1 << 15)
#define TOOLITEM_COMMAND_MASK 0x7FFF
wresult _w_toolitem_get_data(w_item *item, void **data) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_LPARAM | TBIF_BYINDEX;
	info.lParam = 0;
	SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info);
	*data = (void*) info.lParam;
	return W_TRUE;
}
wresult _w_toolitem_get_index(w_item *item) {
	return _W_ITEM(item)->index;
}
wresult _w_toolitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	WCHAR *str;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_COMMAND | TBIF_BYINDEX;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		int count = SendMessageW(handle, TB_GETBUTTONTEXTW, info.idCommand, 0);
		if (count > 0) {
			size_t size = count * sizeof(WCHAR);
			str = _w_toolkit_malloc(size);
			if (str != 0) {
				count = SendMessageW(handle, TB_GETBUTTONTEXTW, info.idCommand,
						(LPARAM) str);
				if (count > 0) {
					result = _win_text_set(str, count, alloc, user_data, enc);
				}
			}
			_w_toolkit_free(str, size);
		}
	}

	return result;
}
wresult _w_toolitem_set_data(w_item *item, void *data) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_LPARAM | TBIF_BYINDEX;
	info.lParam = (LPARAM) data;
	return SendMessageW(handle, TB_SETBUTTONINFOW, index, (LPARAM) &info);
}
wresult _w_toolitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_STYLE | TBIF_BYINDEX;
	SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info);
	info.dwMask |= TBIF_TEXT;
	info.fsStyle |= BTNS_AUTOSIZE;
	WCHAR *str;
	int newlength;
	wresult result = _win_text_fix(text, length, enc, &str, &newlength);
	if (newlength > 0) {
		info.fsStyle |= BTNS_SHOWTEXT;
	}
	info.pszText = str;
	info.cchText = newlength;
	SendMessageW(handle, TB_SETBUTTONINFOW, index, (LPARAM) &info);
	_win_text_free(text, str, newlength);
	return result;
}
wresult _w_toolitem_get_bounds(w_toolitem *item, w_rect *rect) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	RECT r;
	SendMessageW(handle, TB_GETITEMRECT, index, (LPARAM) &r);
	rect->x = r.left;
	rect->y = r.top;
	rect->width = r.right - r.left;
	rect->height = r.bottom - r.top;
	return W_TRUE;
}
wresult _w_toolitem_get_control(w_toolitem *item, w_control **control) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	*control = 0;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_STYLE | TBIF_STATE | TBIF_COMMAND | TBIF_BYINDEX;
	info.fsStyle = 0;
	info.idCommand = 0;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		if (info.idCommand & TOOLITEM_SEP) {
			w_event_toolbar event;
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_GET_CONTROL;
			event.event.widget = parent;
			event.item = item;
			event.control = 0;
			_w_widget_send_event(parent, W_EVENT(&event));
			*control = event.control;
			result = W_TRUE;
		}
	}
	return result;
}
wresult _w_toolitem_get_enabled(w_toolitem *item) {
	return W_FALSE;
}
wresult _w_toolitem_get_id(w_toolitem *item) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_COMMAND | TBIF_BYINDEX;
	info.idCommand = 0;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		result = (info.idCommand >> 16) & 0xFFFF;
	}
	return result;
}
wresult _w_toolitem_get_image(w_toolitem *item) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	wresult result = -1;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_IMAGE | TBIF_BYINDEX;
	info.iImage = -1;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		result = info.iImage;
		if (result < 0)
			result = -1;
	}
	return result;
}
wresult _w_toolitem_get_menu(w_toolitem *item, w_menu **menu) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	*menu = 0;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_STYLE | TBIF_BYINDEX;
	info.fsStyle = 0;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		if (info.fsStyle & BTNS_DROPDOWN) {
			w_event_toolbar event;
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_GET_MENU;
			event.event.widget = parent;
			event.item = item;
			event.menu = 0;
			_w_widget_send_event(parent, W_EVENT(&event));
			*menu = event.menu;
		}
	}
	return result;
}
wresult _w_toolitem_get_selection(w_toolitem *item) {
	return W_FALSE;
}
wresult _w_toolitem_get_style(w_toolitem *item) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_STYLE | TBIF_COMMAND | TBIF_BYINDEX;
	info.fsStyle = 0;
	info.idCommand = 0;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		if (info.idCommand & TOOLITEM_SEP) {
			return W_SEPARATOR;
		}
		if (info.fsStyle & BTNS_BUTTON)
			return W_PUSH;
		if (info.fsStyle & BTNS_CHECK) {
			if (info.fsStyle & BTNS_GROUP)
				return W_RADIO;
			else
				return W_CHECK;
		}
		if (info.fsStyle & BTNS_DROPDOWN)
			return W_DROP_DOWN;
	}
	return 0;
}
wresult _w_toolitem_get_tooltip_text(w_toolitem *item, w_alloc alloc,
		void *user_data, int enc) {
	return W_FALSE;
}
wresult _w_toolitem_get_width(w_toolitem *item) {
	return W_FALSE;
}
wresult _w_toolitem_is_enabled(w_toolitem *item) {
	return W_FALSE;
}
void _w_toolitem_resize_control(w_toolitem *item, w_control *control) {
	if (w_widget_is_ok(W_WIDGET(control)) > 0) {
		/*
		 * Set the size and location of the control
		 * separately to minimize flashing in the
		 * case where the control does not resize
		 * to the size that was requested.  This
		 * case can occur when the control is a
		 * combo box.
		 */
		w_rect itemRect, rect;
		_w_toolitem_get_bounds(item, &itemRect);
		w_control_set_bounds(control, 0, &itemRect.sz);
		w_control_get_bounds(control, 0, &rect.sz);
		rect.x = itemRect.x + (itemRect.width - rect.width) / 2;
		rect.y = itemRect.y + (itemRect.height - rect.height) / 2;
		w_control_set_bounds(control, &rect.pt, 0);
	}
}
wresult _w_toolitem_set_control(w_toolitem *item, w_control *control) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_STYLE | TBIF_STATE | TBIF_COMMAND | TBIF_BYINDEX;
	info.fsStyle = 0;
	info.idCommand = 0;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		if (info.idCommand & TOOLITEM_SEP) {
			w_event_toolbar event;
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_SET_CONTROL;
			event.event.widget = parent;
			event.item = item;
			event.control = control;
			_w_widget_send_event(parent, W_EVENT(&event));
			result = W_TRUE;
			/*
			 * Feature in Windows.  When a tool bar wraps, tool items
			 * with the style BTNS_SEP are used as wrap points.  This
			 * means that controls that are placed on top of separator
			 * items are not positioned properly.  Also, vertical tool
			 * bars are implemented using TB_SETROWS to set the number
			 * of rows.  When a control is placed on top of a separator,
			 * the height of the separator does not grow.  The fix in
			 * both cases is to change the tool item style from BTNS_SEP
			 * to BTNS_BUTTON, causing the item to wrap like a tool item
			 * button.  The new tool item button is disabled to avoid key
			 * traversal and the image is set to I_IMAGENONE to avoid
			 * getting the first image from the image list.
			 */
			if ((_W_WIDGET(parent)->style & (W_WRAP | W_VERTICAL)) != 0) {
				int changed = FALSE;
				if (control == 0) {
					if ((info.fsStyle & BTNS_SEP) == 0) {
						changed = TRUE;
						info.fsStyle &= ~(BTNS_BUTTON | BTNS_SHOWTEXT);
						info.fsStyle |= BTNS_SEP;
					}
				} else {
					if ((info.fsStyle & BTNS_SEP) != 0) {
						changed = TRUE;
						info.fsStyle &= ~BTNS_SEP;
						info.fsStyle |= BTNS_BUTTON | BTNS_SHOWTEXT;
						info.fsState &= ~TBSTATE_ENABLED;
						info.dwMask |= TBIF_IMAGE;
						info.iImage = I_IMAGENONE;
					}
				}
				if (changed) {
					SendMessageW(handle, TB_SETBUTTONINFOW, index,
							(LPARAM) &info);
					/*
					 * Bug in Windows.  When TB_SETBUTTONINFO changes the
					 * style of a tool item from BTNS_SEP to BTNS_BUTTON
					 * and the tool bar is wrapped, the tool bar does not
					 * redraw properly.  Windows uses separator items as
					 * wrap points and sometimes draws etching above or
					 * below and entire row.  The fix is to redraw the
					 * tool bar.
					 */
					if (SendMessageW(handle, TB_GETROWS, 0, 0) > 1) {
						InvalidateRect(handle, 0, TRUE);
					}
				}
			}
			_w_toolitem_resize_control(item, control);
		}
	}
	return result;
}
wresult _w_toolitem_set_enabled(w_toolitem *item, int enabled) {
	return W_FALSE;
}
wresult _w_toolitem_set_id(w_toolitem *item, wushort id) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_COMMAND | TBIF_BYINDEX;
	info.idCommand = 0;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		info.idCommand = (info.idCommand & 0xFFFF) | (id & 0xFFFF) << 16;
		result = SendMessageW(handle, TB_SETBUTTONINFOW, index, (LPARAM) &info);
	}
	return result;
}
wresult _w_toolitem_set_image(w_toolitem *item, int image) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(TBBUTTONINFOW);
	info.dwMask = TBIF_IMAGE | TBIF_SIZE | TBIF_STYLE | TBIF_BYINDEX;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		info.dwMask = TBIF_IMAGE | TBIF_BYINDEX;
		info.iImage = image;
		if (info.iImage < 0)
			info.iImage = I_IMAGENONE;
		/*
		 * Bug in Windows.  If the width of an item has already been
		 * calculated, the tool bar control will not recalculate it to
		 * include the space for the image.  The fix is to set the width
		 * to zero, forcing the control recalculate the width for the item.
		 */
		if ((info.fsStyle & BTNS_SEP) == 0) {
			info.dwMask |= TBIF_SIZE;
			info.cx = 0;
		}
		if (SendMessageW(handle, TB_SETBUTTONINFOW, index, (LPARAM) &info)) {
			result = W_TRUE;
			if ((info.fsStyle & BTNS_SEP) == 0) {
				HFONT hFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
				SendMessageW(handle, WM_SETFONT, (WPARAM) hFont, 0);
				_w_toolbar_layout_items(W_TOOLBAR(parent));
			}
		}
	}
	return result;
}
wresult _w_toolitem_set_menu(w_toolitem *item, w_menu *menu) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_STYLE | TBIF_BYINDEX;
	info.fsStyle = 0;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		if (info.fsStyle & BTNS_DROPDOWN) {
			w_event_toolbar event;
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_SET_MENU;
			event.event.widget = parent;
			event.item = item;
			event.menu = menu;
			_w_widget_send_event(parent, W_EVENT(&event));
		}
	}
	return result;
}
wresult _w_toolitem_set_selection(w_toolitem *item, int selected) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_STYLE | TBIF_STATE | TBIF_BYINDEX;
	info.idCommand = 0;
	if (SendMessageW(handle, TB_GETBUTTONINFOW, index, (LPARAM) &info) != -1) {
		if (info.fsStyle & BTNS_CHECK) {
			if (selected) {
				info.fsState |= TBSTATE_CHECKED;
			} else {
				info.fsState &= ~TBSTATE_CHECKED;
			}
			result = SendMessageW(handle, TB_SETBUTTONINFOW, index,
					(LPARAM) &info);
		}
	}
	return result;
}
wresult _w_toolitem_set_tooltip_text(w_toolitem *item, const char *string,
		int length, int enc) {
	return W_FALSE;
}
wresult _w_toolitem_set_width(w_toolitem *item, int width) {
	w_widget *parent = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(parent)->handle;
	int index = _W_ITEM(item)->index;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(TBBUTTONINFOW);
	info.dwMask = TBIF_SIZE | TBIF_BYINDEX;
	info.cx = width;
	return SendMessageW(handle, TB_SETBUTTONINFOW, index, (LPARAM) &info);
}
/*
 *
 */
wuint64 _w_toolbar_check_style(w_widget *widget, wuint64 style) {
	/*
	 * On Windows, only flat tool bars can be traversed.
	 */
	if ((style & W_FLAT) == 0)
		style |= W_NO_FOCUS;

	/*
	 * A vertical tool bar cannot wrap because TB_SETROWS
	 * fails when the toolbar has TBSTYLE_WRAPABLE.
	 */
	if ((style & W_VERTICAL) != 0)
		style &= ~W_WRAP;

	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_toolbar_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	int ret = _w_scrollable_compute_trim(widget, e, priv);
	int bits = GetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE);
	if ((bits & CCS_NODIVIDER) == 0) {
		e->result->height += 2;
	}
	return ret;
}
wresult _w_toolbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	int width = 0, height = 0;
	HWND handle = _W_WIDGET(widget)->handle;
	RECT rect, oldRect;
	TBBUTTONINFOW info;
	int count = (int) SendMessageW(handle, TB_BUTTONCOUNT, 0, 0);
	if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
		for (int i = 0; i < count; i++) {
			SendMessageW(handle, TB_GETITEMRECT, i, (LPARAM) &rect);
			height = WMAX(height, rect.bottom);
			info.cbSize = sizeof(TBBUTTONINFOW);
			info.dwMask = TBIF_SIZE | TBIF_BYINDEX | TBIF_STYLE;
			SendMessageW(handle, TB_GETBUTTONINFOW, i, (LPARAM) &info);
			if ((info.fsStyle & BTNS_SEP) != 0) {
				width = WMAX(width, info.cx);
			} else {
				width = WMAX(width, rect.right);
			}
		}
	} else {
		GetWindowRect(handle, &oldRect);
		int oldWidth = oldRect.right - oldRect.left;
		int oldHeight = oldRect.bottom - oldRect.top;
		int border = _w_control_get_border_width(W_CONTROL(widget));
		int newWidth = wHint == W_DEFAULT ? 0x3FFF : wHint + border * 2;
		int newHeight = hHint == W_DEFAULT ? 0x3FFF : hHint + border * 2;
		wresult redraw = _w_control_get_drawing(W_CONTROL(widget))
				&& IsWindowVisible(handle);
		//ignoreResize = true;
		_W_WIDGET(widget)->state |= STATE_IGNORE_RESIZE;
		if (redraw)
			UpdateWindow(handle);
		int flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER;
		SetWindowPos(handle, 0, 0, 0, newWidth, newHeight, flags);
		if (count != 0) {
			SendMessageW(handle, TB_GETITEMRECT, count - 1, (LPARAM) &rect);
			width = WMAX(width, rect.right);
			height = WMAX(height, rect.bottom);
		}
		SetWindowPos(handle, 0, 0, 0, oldWidth, oldHeight, flags);
		if (redraw)
			ValidateRect(handle, NULL);
		//ignoreResize = false;
		_W_WIDGET(widget)->state &= ~STATE_IGNORE_RESIZE;
	}

	/*
	 * From the Windows SDK for TB_SETBUTTONSIZE:
	 *
	 *   "If an application does not explicitly
	 *	set the button size, the size defaults
	 *	to 24 by 22 pixels".
	 */
	if (width == 0)
		width = DEFAULT_WIDTH;
	if (height == 0)
		height = DEFAULT_HEIGHT;
	if (wHint != W_DEFAULT)
		width = wHint;
	if (hHint != W_DEFAULT)
		height = hHint;
	w_event_compute_trim ee;
	w_rect r, trim;
	ee.rect = &r;
	ee.result = &trim;
	r.x = 0;
	r.y = 0;
	r.width = width;
	r.height = height;
	_w_toolbar_compute_trim(widget, &ee, priv);
	width = trim.width;
	height = trim.height;
	e->size->width = width;
	e->size->height = height;
	return TRUE;
}
wresult _w_toolbar_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result = _w_composite_create_handle(control, priv);
	if (result > 0) {
		HWND handle = _W_WIDGET(control)->handle;
		wuint64 style = _W_WIDGET(control)->style;
		_W_WIDGET(control)->state &= ~STATE_CANVAS;

		/*
		 * Feature in Windows.  When TBSTYLE_FLAT is used to create
		 * a flat toolbar, for some reason TBSTYLE_TRANSPARENT is
		 * also set.  This causes the toolbar to flicker when it is
		 * moved or resized.  The fix is to clear TBSTYLE_TRANSPARENT.
		 *
		 * NOTE:  This work around is unnecessary on XP.  There is no
		 * flickering and clearing the TBSTYLE_TRANSPARENT interferes
		 * with the XP theme.
		 */
		if ((style & W_FLAT) != 0) {
			if (_COMCTL32_VERSION < VERSION(6, 0) || !IsAppThemed()) {
				int bits = GetWindowLongW(handle, GWL_STYLE);
				bits &= ~TBSTYLE_TRANSPARENT;
				SetWindowLongW(handle, GWL_STYLE, bits);
			}
		}

		/*
		 * Feature in Windows.  Despite the fact that the
		 * tool tip text contains \r\n, the tooltip will
		 * not honour the new line unless TTM_SETMAXTIPWIDTH
		 * is set.  The fix is to set TTM_SETMAXTIPWIDTH to
		 * a large value.
		 */
		/*
		 * These lines are intentionally commented.  The tool
		 * bar currently sets this value to 300 so it is not
		 * necessary to set TTM_SETMAXTIPWIDTH.
		 */
//	HWND hwndToolTip = SendMessage (handle, TB_GETTOOLTIPS, 0, 0);
//	SendMessage (hwndToolTip, TTM_SETMAXTIPWIDTH, 0, 0x7FFF);
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
		SendMessageW(handle, WM_SETFONT, (WPARAM) hFont, 0);

		/* Set the button struct, bitmap and button sizes */
		SendMessageW(handle, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
		SendMessageW(handle, TB_SETBITMAPSIZE, 0, 0);
		SendMessageW(handle, TB_SETBUTTONSIZE, 0, 0);

		/* Set the extended style bits */
		DWORD bits = TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_MIXEDBUTTONS
				| TBSTYLE_EX_HIDECLIPPEDBUTTONS;
		if (_COMCTL32_VERSION >= VERSION(6, 0))
			bits |= TBSTYLE_EX_DOUBLEBUFFER;
		SendMessageW(handle, TB_SETEXTENDEDSTYLE, 0, bits);

		/*
		 *
		 */
		/*
		 * Ensure that either of HORIZONTAL or VERTICAL is set.
		 * NOTE: HORIZONTAL and VERTICAL have the same values
		 * as H_SCROLL and V_SCROLL so it is necessary to first
		 * clear these bits to avoid scroll bars and then reset
		 * the bits using the original style supplied by the
		 * programmer.
		 *
		 * NOTE: The CCS_VERT style cannot be applied when the
		 * widget is created because of this conflict.
		 */
		if ((style & W_VERTICAL) != 0) {
			int bits = GetWindowLong(handle, GWL_STYLE);
			/*
			 * Feature in Windows.  When a tool bar has the style
			 * TBSTYLE_LIST and has a drop down item, Window leaves
			 * too much padding around the button.  This affects
			 * every button in the tool bar and makes the preferred
			 * height too big.  The fix is to set the TBSTYLE_LIST
			 * when the tool bar contains both text and images.
			 *
			 * NOTE: Tool bars with CCS_VERT must have TBSTYLE_LIST
			 * set before any item is added or the tool bar does
			 * not lay out properly.  The work around does not run
			 * in this case.
			 */
			if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
				if ((style & W_RIGHT) != 0)
					bits |= TBSTYLE_LIST;
			}
			SetWindowLong(handle, GWL_STYLE, bits | CCS_VERT);
		}
	}
	return result;
}
wresult _w_toolbar_get_imagelist(w_toolbar *toolbar, w_imagelist **imagelist) {
	return W_FALSE;
}
wresult _w_toolbar_get_item(w_toolbar *toolbar, int index, w_toolitem *item) {
	return W_FALSE;
}
wresult _w_toolbar_get_item_from_point(w_toolbar *toolbar, w_point *point,
		w_toolitem *item) {
	HWND handle = _W_WIDGET(toolbar)->handle;
	RECT rect;
	POINT pt;
	pt.x = point->x;
	pt.y = point->y;
	int count = (int) SendMessageW(handle, TB_BUTTONCOUNT, 0, 0);
	for (int i = 0; i < count; i++) {
		SendMessageW(handle, TB_GETITEMRECT, i, (LPARAM) &rect);
		if (PtInRect(&rect, pt)) {
			_W_WIDGETDATA(item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(toolbar);
			_W_ITEM(item)->parent = W_WIDGET(toolbar);
			_W_ITEM(item)->index = i;
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_toolbar_get_item_count(w_toolbar *toolbar) {
	return W_FALSE;
}
wresult _w_toolbar_get_items(w_toolbar *toolbar, w_iterator *items) {
	return W_FALSE;
}
wresult _w_toolbar_get_row_count(w_toolbar *toolbar) {
	return W_FALSE;
}
wresult _w_toolbar_insert_item(w_toolbar *toolbar, w_toolitem *item, int style,
		int index) {
	HWND handle = _W_WIDGET(toolbar)->handle;
	int count = (int) SendMessageW(handle, TB_BUTTONCOUNT, 0, 0);
	if (!(0 <= index && index <= count)) {
		index = count;
	}
	int bits = _w_toolbar_widget_item_style(style);
	TBBUTTON lpButton;
	WCHAR *t = 0;
	memset(&lpButton, 0, sizeof(lpButton));
	lpButton.idCommand = (index & TOOLITEM_COMMAND_MASK);
	lpButton.fsStyle = (byte) bits;
	lpButton.fsState = (byte) TBSTATE_ENABLED;
	/*
	 * Bug in Windows.  Despite the fact that the image list
	 * index has never been set for the item, Windows always
	 * assumes that the image index for the item is valid.
	 * When an item is inserted, the image index is zero.
	 * Therefore, when the first image is inserted and is
	 * assigned image index zero, every item draws with this
	 * image.  The fix is to set the image index to none
	 * when the item is created.  This is not necessary in
	 * the case when the item has the BTNS_SEP style because
	 * separators cannot show images.
	 */
	if ((bits & BTNS_SEP) == 0)
		lpButton.iBitmap = I_IMAGENONE;
	else {
		lpButton.idCommand |= TOOLITEM_SEP;
	}
	LRESULT result = SendMessageW(_W_WIDGET(toolbar)->handle, TB_INSERTBUTTONW,
			index, (LPARAM) &lpButton);
	if (result == 0) {
		return W_ERROR_ITEM_NOT_ADDED;
	}
	if (item != 0) {
		_W_WIDGETDATA(item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(toolbar);
		_W_ITEM(item)->parent = W_WIDGET(toolbar);
		_W_ITEM(item)->index = index;
	}
	if ((_W_WIDGET(toolbar)->style & W_VERTICAL) != 0)
		_w_toolbar_set_row_count(toolbar, count + 1);
	_w_toolbar_layout_items(toolbar);
	return TRUE;
}
void _w_toolbar_layout_items(w_toolbar *toolbar) {
	HWND handle = _W_WIDGET(toolbar)->handle;
	_w_item item;
	_W_WIDGETDATA(&item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(toolbar);
	item.parent = W_WIDGET(toolbar);
	int count = (int) SendMessageW(handle, TB_BUTTONCOUNT, 0, 0);
	TBBUTTONINFOW info;
	WCHAR tmp[3];
	/*
	 * Feature in Windows.  When a tool bar has the style
	 * TBSTYLE_LIST and has a drop down item, Window leaves
	 * too much padding around the button.  This affects
	 * every button in the tool bar and makes the preferred
	 * height too big.  The fix is to set the TBSTYLE_LIST
	 * when the tool bar contains both text and images.
	 *
	 * NOTE: Tool bars with CCS_VERT must have TBSTYLE_LIST
	 * set before any item is added or the tool bar does
	 * not lay out properly.  The work around does not run
	 * in this case.
	 */
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		if ((_W_WIDGET(toolbar)->style & W_RIGHT) != 0
				&& (_W_WIDGET(toolbar)->style & W_VERTICAL) == 0) {
			wresult hasText = FALSE, hasImage = FALSE;
			for (int i = 0; i < count; i++) {
				info.cbSize = sizeof(TBBUTTONINFOW);
				info.dwMask = TBIF_IMAGE | TBIF_TEXT | TBIF_BYINDEX;
				info.iImage = 0;
				info.pszText = tmp;
				info.cchText = 2;
				tmp[0] = 0;
				if (SendMessageW(handle, TB_GETBUTTONINFOW, i, (LPARAM) &info)
						!= -1) {
					if (!hasText) {
						hasText = tmp[0] != 0;
					}
					if (!hasImage)
						hasImage = info.iImage > 0;
					if (hasText && hasImage)
						break;
				}
			}
			int oldBits = GetWindowLongW(handle, GWL_STYLE), newBits = oldBits;
			if (hasText && hasImage) {
				newBits |= TBSTYLE_LIST;
			} else {
				newBits &= ~TBSTYLE_LIST;
			}
			if (newBits != oldBits) {
				_w_toolbar_set_drop_down_items(toolbar, FALSE);
				SetWindowLongW(handle, GWL_STYLE, newBits);
				/*
				 * Feature in Windows.  For some reason, when the style
				 * is changed to TBSTYLE_LIST, Windows does not lay out
				 * the tool items.  The fix is to use WM_SETFONT to force
				 * the tool bar to redraw and lay out.
				 */
				HFONT hFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
				SendMessageW(handle, WM_SETFONT, (WPARAM) hFont, 0);
				_w_toolbar_set_drop_down_items(toolbar, TRUE);
			}
		}
	}

	if ((_W_WIDGET(toolbar)->style & W_WRAP) != 0) {
		SendMessageW(handle, TB_AUTOSIZE, 0, 0);
	}
	/*
	 *  When the tool bar is vertical, make the width of each button
	 *  be the width of the widest button in the tool bar.  Note that
	 *  when the tool bar contains a drop down item, it needs to take
	 *  into account extra padding.
	 */
	if ((_W_WIDGET(toolbar)->style & W_VERTICAL) != 0) {
		if (count > 1) {
			info.cbSize = sizeof(TBBUTTONINFOW);
			info.dwMask = TBIF_SIZE | TBIF_BYINDEX;
			LRESULT size = SendMessageW(handle, TB_GETBUTTONSIZE, 0, 0);
			int info_cx = LOWORD(size);
			int index = 0, extraPadding = 0;
			while (index < count) {
				info.dwMask = TBIF_STYLE | TBIF_BYINDEX;
				if (SendMessageW(handle, TB_GETBUTTONINFOW, index,
						(LPARAM) &info) != -1) {
					if ((info.fsStyle & BTNS_DROPDOWN) != 0) {
						/*
						 * Specifying 1 pixel extra padding to avoid truncation
						 * of widest item in the tool-bar when a tool-bar has
						 * SWT.VERTICAL style and any of the items in the
						 * tool-bar has SWT.DROP_DOWN style, Refer bug#437206
						 */
						extraPadding = 1;
						break;
					}
				}
				index++;
			}
			if (index < count) {
				LRESULT padding = SendMessageW(handle, TB_GETPADDING, 0, 0);
				info_cx += LOWORD (padding + extraPadding) * 2;
			}
			for (int i = 0; i < count; i++) {
				info.dwMask = TBIF_STYLE | TBIF_BYINDEX;
				if (SendMessageW(handle, TB_GETBUTTONINFOW, i, (LPARAM) &info)
						!= -1) {
					if ((info.fsStyle & BTNS_SEP) == 0) {
						info.dwMask = TBIF_SIZE;
						info.cx = info_cx;
						SendMessageW(handle, TB_SETBUTTONINFOW, i,
								(LPARAM) &info);
					}
				}
			}
		}
	}

	/*
	 * Feature on Windows. When SWT.WRAP or SWT.VERTICAL are set
	 * the separator items with control are implemented using BTNS_BUTTON
	 * instead of BTNS_SEP. When that is the case and TBSTYLE_LIST is
	 * set, the layout of the ToolBar recalculates the width for all
	 * BTNS_BUTTON based on the text and bitmap of the item.
	 * This is not strictly wrong, but the user defined width for the
	 * separators has to be respected if set.
	 * The fix is to detect this case and reset the cx width for the item.
	 */
	if ((_W_WIDGET(toolbar)->style & (W_WRAP | W_VERTICAL)) != 0) {
		int bits = GetWindowLongW(handle, GWL_STYLE);
		if ((bits & TBSTYLE_LIST) != 0) {
			info.cbSize = sizeof(TBBUTTONINFOW);
			info.dwMask = TBIF_SIZE;
			for (int i = 0; i < count; i++) {
				/*ToolItem item = items[i];
				 if (item != null && item.cx > 0) {
				 info.cx = item.cx;
				 SendMessageW(handle, TB_SETBUTTONINFOW, i,(LPARAM) &info);
				 }*/
			}
		}
	}
	w_control *control;
	for (int i = 0; i < count; i++) {
		item.index = i;
		_w_toolitem_get_control(W_TOOLITEM(&item), &control);
		_w_toolitem_resize_control(W_TOOLITEM(&item), control);
	}
}
void _w_toolbar_set_drop_down_items(w_toolbar *toolbar, wresult set) {
	HWND handle = _W_WIDGET(toolbar)->handle;
	int count = (int) SendMessageW(handle, TB_BUTTONCOUNT, 0, 0);
	TBBUTTONINFOW info;
	WCHAR tmp[3];
	info.cbSize = sizeof(TBBUTTONINFOW);
	/*
	 * Feature in Windows.  When the first button in a tool bar
	 * is a drop down item, Window leaves too much padding around
	 * the button.  This affects every button in the tool bar and
	 * makes the preferred height too big.  The fix is clear the
	 * BTNS_DROPDOWN before Windows lays out the tool bar and set
	 * the bit afterwards.
	 *
	 * NOTE:  This work around only runs when the tool bar contains
	 * only images.
	 */
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		wresult hasText = FALSE, hasImage = FALSE;
		for (int i = 0; i < count; i++) {
			info.dwMask = TBIF_IMAGE | TBIF_TEXT | TBIF_BYINDEX;
			info.iImage = 0;
			info.pszText = tmp;
			info.cchText = 2;
			tmp[0] = 0;
			if (SendMessageW(handle, TB_GETBUTTONINFOW, i, (LPARAM) &info)
					!= -1) {
				if (!hasText) {
					hasText = tmp[0] != 0;
				}
				if (!hasImage)
					hasImage = info.iImage > 0;
				if (hasText && hasImage)
					break;
			}
		}
		if (hasImage && !hasText) {
			for (int i = 0; i < count; i++) {
				info.dwMask = TBIF_STYLE | TBIF_BYINDEX;
				if (SendMessageW(handle, TB_GETBUTTONINFOW, i, (LPARAM) &info)
						!= -1) {
					if ((info.fsStyle & BTNS_DROPDOWN) != 0) {
						if (set) {
							info.fsStyle |= BTNS_DROPDOWN;
						} else {
							info.fsStyle &= ~BTNS_DROPDOWN;
						}
						SendMessageW(handle, TB_SETBUTTONINFO, i,
								(LPARAM) &info);
					}
				}
			}
		}
	}
}
wresult _w_toolbar_set_imagelist(w_toolbar *toolbar, w_imagelist *imagelist) {
	wresult result = TRUE;
	HIMAGELIST _imagelist = 0;
	if (imagelist == 0) {
		_imagelist = 0;
		_W_TOOLBAR(toolbar)->imagelist = 0;
	} else {
		if (_W_IMAGELIST(imagelist)->imagelist == 0) {
			_imagelist = 0;
			_W_TOOLBAR(toolbar)->imagelist = 0;
			result = W_ERROR_INVALID_ARGUMENT;
		} else {
			_imagelist = _W_IMAGELIST(imagelist)->imagelist;
			_W_TOOLBAR(toolbar)->imagelist = imagelist;
		}
	}
	if (result > 0) {
		HWND handle = _W_WIDGET(toolbar)->handle;
		SendMessageW(handle, TB_SETIMAGELIST, 0, (LPARAM) _imagelist);
		/*HFONT hFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
		 SendMessageW(handle, WM_SETFONT, (WPARAM) hFont, 0);
		 _w_toolbar_layout_items(toolbar);*/
	}
	return result;
}
void _w_toolbar_set_row_count(w_toolbar *toolbar, int count) {
	if ((_W_WIDGET(toolbar)->style & W_VERTICAL) != 0) {
		HWND handle = _W_WIDGET(toolbar)->handle;
		/*
		 * Feature in Windows.  When the TB_SETROWS is used to set the
		 * number of rows in a tool bar, the tool bar is resized to show
		 * the items.  This is unexpected.  The fix is to save and restore
		 * the current size of the tool bar.
		 */
		RECT rect;
		GetWindowRect(handle, &rect);
		w_composite *parent;
		w_control_get_parent(W_CONTROL(toolbar), &parent);
		MapWindowPoints(0, _W_WIDGET(parent)->handle, (LPPOINT) &rect, 2);
		_W_WIDGET(toolbar)->state |= STATE_IGNORE_RESIZE;
		/*
		 * Feature in Windows.  When the last button in a tool bar has the
		 * style BTNS_SEP and TB_SETROWS is used to set the number of rows
		 * in the tool bar, depending on the number of buttons, the toolbar
		 * will wrap items with the style BTNS_CHECK, even when the fLarger
		 * flags is used to force the number of rows to be larger than the
		 * number of items.  The fix is to set the number of rows to be two
		 * larger than the actual number of rows in the tool bar.  When items
		 * are being added, as long as the number of rows is at least one
		 * item larger than the count, the tool bar is laid out properly.
		 * When items are being removed, setting the number of rows to be
		 * one more than the item count has no effect.  The number of rows
		 * is already one more causing TB_SETROWS to do nothing.  Therefore,
		 * choosing two instead of one as the row increment fixes both cases.
		 */
		count += 2;
		SendMessageW(handle, TB_SETROWS, MAKEWPARAM(count, 1), 0);
		int flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER;
		SetWindowPos(handle, 0, 0, 0, rect.right - rect.left,
				rect.bottom - rect.top, flags);
		_W_WIDGET(toolbar)->state &= ~STATE_IGNORE_RESIZE;
	}
}
DWORD _w_toolbar_widget_item_style(wuint64 style) {
	if ((style & W_DROP_DOWN) != 0)
		return BTNS_DROPDOWN;
	if ((style & W_PUSH) != 0)
		return BTNS_BUTTON;
	if ((style & W_CHECK) != 0)
		return BTNS_CHECK;
	/*
	 * This code is intentionally commented.  In order to
	 * consistently support radio tool items across platforms,
	 * the platform radio behavior is not used.
	 */
	if ((style & W_RADIO) != 0)
		return BTNS_CHECKGROUP;
	if ((style & W_SEPARATOR) != 0)
		return BTNS_SEP;
	return BTNS_BUTTON;
}
DWORD _w_toolbar_widget_style(w_control *control, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	DWORD bits = _w_composite_widget_style(control, priv);
	bits |= CCS_NORESIZE | TBSTYLE_TOOLTIPS | TBSTYLE_CUSTOMERASE;
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed())
		bits |= TBSTYLE_TRANSPARENT;
	if ((style & W_SHADOW_OUT) == 0)
		bits |= CCS_NODIVIDER;
	if ((style & W_WRAP) != 0)
		bits |= TBSTYLE_WRAPABLE;
	if ((style & W_FLAT) != 0)
		bits |= TBSTYLE_FLAT;
	/*
	 * Feature in Windows.  When a tool bar has the style
	 * TBSTYLE_LIST and has a drop down item, Window leaves
	 * too much padding around the button.  This affects
	 * every button in the tool bar and makes the preferred
	 * height too big.  The fix is to set the TBSTYLE_LIST
	 * when the tool bar contains both text and images.
	 *
	 * NOTE: Tool bars with CCS_VERT must have TBSTYLE_LIST
	 * set before any item is added or the tool bar does
	 * not lay out properly.  The work around does not run
	 * in this case.
	 */
	if (_COMCTL32_VERSION >= VERSION(6, 0) || !IsAppThemed()) {
		if ((style & W_RIGHT) != 0)
			bits |= TBSTYLE_LIST;
	}
	return bits;
}
const char* _w_toolbar_window_class(w_control *control, _w_control_priv *priv) {
	return TOOLBARCLASSNAMEA;
}
/*
 *	messages
 */
wresult _TOOLBAR_WM_COMMANDCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int index = (e->wparam & TOOLITEM_COMMAND_MASK);
	_w_toolitem item;
	w_event_toolbar event;
	HWND handle = _W_WIDGET(widget)->handle;
	if (index >= 0) {
		RECT rect;
		memset(&event, 0, sizeof(event));
		SendMessageW(handle, TB_GETITEMRECT, index, (LPARAM) &rect);
		_W_WIDGETDATA(&item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(widget);
		_W_ITEM(&item)->parent = widget;
		_W_ITEM(&item)->index = index;
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_SELECTION;
		event.event.widget = widget;
		event.event.platform_event = _EVENT_PLATFORM(e);
		event.detail = 0;
		event.location.x = rect.left;
		event.location.y = rect.bottom;
		event.item = W_TOOLITEM(&item);
		_w_widget_send_event(widget, W_EVENT(&event));
	}
	return W_FALSE;
}
wresult _TOOLBAR_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_WIDGET(widget)->state & STATE_IGNORE_MOUSE)
		return W_FALSE;
	return _COMPOSITE_WM_LBUTTONDOWN(widget, e, priv);
}
wresult _TOOLBAR_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMHDR *hdr = (NMHDR*) e->lparam;
	switch (hdr->code) {
	case TBN_DROPDOWN: {
		NMTOOLBARW *lpnmtb = (NMTOOLBARW*) hdr;
		_w_toolitem item;
		w_event_toolbar event;
		HWND handle = _W_WIDGET(widget)->handle;
		RECT rect;
		int index = SendMessageW(handle, TB_COMMANDTOINDEX, lpnmtb->iItem, 0);
		if (index >= 0) {
			SendMessageW(handle, TB_GETITEMRECT, index, (LPARAM) &rect);
			_W_WIDGETDATA(&item)->clazz = _W_TOOLBAR_GET_ITEM_CLASS(widget);
			_W_ITEM(&item)->parent = widget;
			_W_ITEM(&item)->index = index;
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_SELECTION;
			event.event.widget = widget;
			event.event.platform_event = _EVENT_PLATFORM(e);
			event.detail = W_ARROW;
			event.location.x = rect.left;
			event.location.y = rect.bottom;
			event.item = W_TOOLITEM(&item);
			wresult result = _w_widget_send_event(widget, W_EVENT(&event));
			if (result == W_FALSE) {
				event.event.type = W_EVENT_ITEM_GET_MENU;
				event.menu = 0;
				wresult result = _w_widget_send_event(widget, W_EVENT(&event));
				if (result && event.menu) {
					w_point loc;
					w_control_to_display(W_CONTROL(widget), &loc,
							&event.location);
					w_menu_set_location(event.menu, &loc);
					w_menu_set_visible(event.menu, W_TRUE);
				}
			}
		}
	}
		break;
	}
	return W_FALSE;
}
wresult _TOOLBAR_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result;
	if (_W_WIDGET(widget)->state & STATE_IGNORE_RESIZE) {
		wresult result = priv->widget.call_window_proc(widget, e, priv);
	} else {
		result = _COMPOSITE_WM_SIZE(widget, e, priv);
		if (w_widget_is_ok(widget) <= 0)
			return result;
		/*
		 * Bug in Windows.  The code in Windows that determines
		 * when tool items should wrap seems to use the window
		 * bounds rather than the client area.  Unfortunately,
		 * tool bars with the style TBSTYLE_EX_HIDECLIPPEDBUTTONS
		 * use the client area.  This means that buttons which
		 * overlap the border are hidden before they are wrapped.
		 * The fix is to compute TBSTYLE_EX_HIDECLIPPEDBUTTONS
		 * and set it each time the tool bar is resized.
		 */
		wuint64 style = _W_WIDGET(widget)->style;
		HWND handle = _W_WIDGET(widget)->handle;
		if ((style & W_BORDER) != 0 && (style & W_WRAP) != 0) {
			RECT windowRect, rect;
			GetWindowRect(handle, &windowRect);
			int index = 0;
			int border = w_control_get_border_width(W_CONTROL(widget)) * 2;
			int count = SendMessageW(handle, TB_BUTTONCOUNT, 0, 0);
			while (index < count) {
				SendMessageW(handle, TB_GETITEMRECT, index, (LPARAM) &rect);
				MapWindowPoints(handle, 0, (LPPOINT) &rect, 2);
				if (rect.right > windowRect.right - border * 2)
					break;
				index++;
			}
			DWORD bits = SendMessageW(handle, TB_GETEXTENDEDSTYLE, 0, 0);
			if (index == count) {
				bits |= TBSTYLE_EX_HIDECLIPPEDBUTTONS;
			} else {
				bits &= ~TBSTYLE_EX_HIDECLIPPEDBUTTONS;
			}
			SendMessageW(handle, TB_SETEXTENDEDSTYLE, 0, bits);
		}
		_w_toolbar_layout_items(W_TOOLBAR(widget));
	}
	return result;
}
void _w_toolbar_class_init(struct _w_toolbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TOOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_toolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_toolbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_toolbar);
	/*
	 * public function
	 */
	clazz->get_imagelist = _w_toolbar_get_imagelist;
	clazz->get_item = _w_toolbar_get_item;
	clazz->get_item_from_point = _w_toolbar_get_item_from_point;
	clazz->get_item_count = _w_toolbar_get_item_count;
	clazz->get_items = _w_toolbar_get_items;
	clazz->get_row_count = _w_toolbar_get_row_count;
	clazz->insert_item = _w_toolbar_insert_item;
	clazz->set_imagelist = _w_toolbar_set_imagelist;
	/*
	 *
	 */
	struct _w_toolitem_class *item = clazz->class_toolitem;
	_w_item_class_init(W_ITEM_CLASS(item));
	W_ITEM_CLASS(item)->get_data = _w_toolitem_get_data;
	W_ITEM_CLASS(item)->get_text = _w_toolitem_get_text;
	W_ITEM_CLASS(item)->set_data = _w_toolitem_set_data;
	W_ITEM_CLASS(item)->set_text = _w_toolitem_set_text;
	item->get_bounds = _w_toolitem_get_bounds;
	item->get_control = _w_toolitem_get_control;
	item->get_enabled = _w_toolitem_get_enabled;
	item->get_id = _w_toolitem_get_id;
	item->get_image = _w_toolitem_get_image;
	item->get_menu = _w_toolitem_get_menu;
	item->get_selection = _w_toolitem_get_selection;
	item->get_style = _w_toolitem_get_style;
	item->get_tooltip_text = _w_toolitem_get_tooltip_text;
	item->get_width = _w_toolitem_get_width;
	item->is_enabled = _w_toolitem_is_enabled;
	item->set_control = _w_toolitem_set_control;
	item->set_enabled = _w_toolitem_set_enabled;
	item->set_id = _w_toolitem_set_id;
	item->set_image = _w_toolitem_set_image;
	item->set_menu = _w_toolitem_set_menu;
	item->set_selection = _w_toolitem_set_selection;
	item->set_tooltip_text = _w_toolitem_set_tooltip_text;
	item->set_width = _w_toolitem_set_width;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->create_handle = _w_toolbar_create_handle;
	priv->check_style = _w_toolbar_check_style;
	priv->widget_style = _w_toolbar_widget_style;
	priv->window_class = _w_toolbar_window_class;
	priv->compute_size = _w_toolbar_compute_size;
	priv->compute_trim = _w_toolbar_compute_trim;
	/*
	 * messages
	 */
	dispatch_message *msg = priv->messages;
	msg[_WM_COMMANDCHILD] = _TOOLBAR_WM_COMMANDCHILD;
	msg[_WM_LBUTTONDOWN] = _TOOLBAR_WM_LBUTTONDOWN;
	msg[_WM_NOTIFYCHILD] = _TOOLBAR_WM_NOTIFYCHILD;
	msg[_WM_SIZE] = _TOOLBAR_WM_SIZE;

}
