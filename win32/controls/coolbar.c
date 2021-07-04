/*
 * coolbar.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "coolbar.h"
#include "../widgets/toolkit.h"
/*
 * CoolItem
 */
wresult _w_coolitem_compute_size(w_coolitem *coolitem, w_size *result,
		int wHint, int hHint) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	if (index < 0) {
		memset(result, 0, sizeof(w_size));
		return TRUE;
	}
	result->width = wHint;
	result->height = hHint;
	if (wHint == W_DEFAULT)
		result->width = 32;
	if (hHint == W_DEFAULT)
		result->height = 32;
	int margin = _w_coolitem_get_margin(W_COOLBAR(parent), index);
	if ((_W_WIDGET(parent)->style & W_VERTICAL) != 0) {
		result->height += margin;
	} else {
		result->width += margin;
	}
	return TRUE;
}
wresult _w_coolitem_get_bounds(w_coolitem *coolitem, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_coolitem_get_control(w_coolitem *coolitem, w_control **control) {
	*control = 0;
	return W_FALSE;
}
int _w_coolitem_get_margin(w_coolbar *coolbar, int index) {
	HWND handle = _W_WIDGET(coolbar)->handle;
	wuint64 style = _W_WIDGET(coolbar)->style;
	int margin = 0;
	if (_COMCTL32_VERSION >= VERSION(6, 0)) {
		MARGINS margins;
		SendMessageW(handle, RB_GETBANDMARGINS, 0, (LPARAM) &margins);
		margin += margins.cxLeftWidth + margins.cxRightWidth;
	}
	RECT rect;
	SendMessageW(handle, RB_GETBANDBORDERS, index, (LPARAM) &rect);
	if ((style & W_FLAT) != 0) {
		/*
		 * Bug in Windows.  When the style bit  RBS_BANDBORDERS is not set
		 * the rectangle returned by RBS_BANDBORDERS is four pixels too small.
		 * The fix is to add four pixels to the result.
		 */
		if ((style & W_VERTICAL) != 0) {
			margin += rect.top + 4;
		} else {
			margin += rect.left + 4;
		}
	} else {
		if ((style & W_VERTICAL) != 0) {
			margin += rect.top + rect.bottom;
		} else {
			margin += rect.left + rect.right;
		}
	}
	if ((style & W_FLAT) == 0) {
		if (!_w_coolitem_is_last_item_of_row(coolbar, index)) {
			margin += SEPARATOR_WIDTH;
		}
	}
	return margin;
}
wresult _w_coolitem_get_minimum_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_get_order(w_coolitem *coolitem) {
	return W_FALSE;
}
wresult _w_coolitem_get_preferred_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_get_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_get_wrap_indice(w_coolitem *coolitem) {
	return W_FALSE;
}
wresult _w_coolitem_is_last_item_of_row(w_coolbar *coolbar, int index) {
	int count = (int) SendMessageW(_W_WIDGET(coolbar)->handle, RB_GETBANDCOUNT,
			0, 0);
	if (index + 1 == count)
		return TRUE;
	REBARBANDINFOW rbBand;
	rbBand.cbSize = sizeof(REBARBANDINFOW);
	rbBand.fMask = RBBIM_STYLE;
	SendMessageW(_W_WIDGET(coolbar)->handle, RB_GETBANDINFO, index + 1,
			(LPARAM) &rbBand);
	return (rbBand.fStyle & RBBS_BREAK) != 0;
}
wresult _w_coolitem_set_control(w_coolitem *coolitem, w_control *control) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	if (control != 0) {
		if (!w_widget_is_ok(W_WIDGET(control)))
			return W_ERROR_INVALID_ARGUMENT;
		w_widget *p;
		w_widget_get_parent(W_WIDGET(control), &p);

		if (p != parent)
			return W_ERROR_INVALID_PARENT;
	}
	int index = _W_ITEM(coolitem)->index;
	if (index < 0)
		return W_FALSE;
	w_control *newControl = control, *oldControl;
	HWND hwnd = _W_WIDGET(parent)->handle;
	HWND hwndChild = 0;
	if (newControl != 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
		hwndChild = priv->handle_top(control);
	}
	REBARBANDINFOW rbBand;
	rbBand.cbSize = sizeof(REBARBANDINFOW);
	rbBand.fMask = RBBIM_CHILD;
	rbBand.hwndChild = 0;
	SendMessageW(hwnd, RB_GETBANDINFOW, index, (LPARAM) &rbBand);
	oldControl = (w_control*) GetWindowLongPtrW(rbBand.hwndChild,
	GWLP_USERDATA);
	rbBand.hwndChild = hwndChild;

	/*
	 * Feature in Windows.  When Windows sets the rebar band child,
	 * it makes the new child visible and hides the old child and
	 * moves the new child to the top of the Z-order.  The fix is
	 * to save and restore the visibility and Z-order.
	 */
	HWND hwndAbove = 0;
	if (newControl != 0) {
		hwndAbove = GetWindow(hwndChild, GW_HWNDPREV);
	}
	boolean hideNew = newControl != 0 && !w_control_get_visible(newControl);
	boolean showOld = oldControl != 0 && w_control_get_visible(oldControl);
	SendMessageW(hwnd, RB_SETBANDINFO, index, (LPARAM) &rbBand);
	if (hideNew)
		w_control_set_visible(newControl, FALSE);
	if (showOld)
		w_control_set_visible(oldControl, TRUE);
	if (hwndAbove != 0 && hwndAbove != hwndChild) {
		int flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
		SetWindowPos(hwndChild, hwndAbove, 0, 0, 0, 0, flags);
	}
	return W_TRUE;
}
wresult _w_coolitem_set_layout(w_coolitem *coolitem, int order, int wrapindices,
		w_size *sizes) {
	return W_FALSE;
}
wresult _w_coolitem_set_minimum_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_set_order(w_coolitem *coolitem, int order) {
	return W_FALSE;
}
wresult _w_coolitem_set_preferred_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_set_size(w_coolitem *coolitem, w_size *size) {
	int index = _W_ITEM(coolitem)->index;
	if (index == -1)
		return FALSE;
	int width = WMAX(0, size->width);
	int height = WMAX(0, size->height);
	w_coolbar *parent = (w_coolbar*) _W_ITEM(coolitem)->parent;
	HWND hwnd = _W_WIDGET(parent)->handle;
	int cx, cyChild, cxIdeal;
	if ((_W_WIDGET(parent)->style & W_VERTICAL) != 0) {
		cx = height;
		cyChild = width;
		cxIdeal = WMAX(0, height - _w_coolitem_get_margin(parent, index));
	} else {
		cx = width;
		cyChild = height;
		cxIdeal = WMAX(0, width - _w_coolitem_get_margin(parent, index));
	}
	REBARBANDINFOW rbBand;
	rbBand.cbSize = sizeof(REBARBANDINFOW);

	/* Get the child size fields first so we don't overwrite them. */
	rbBand.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
	SendMessageW(hwnd, RB_GETBANDINFO, index, (LPARAM) &rbBand);

	/* Set the size fields we are currently modifying. */
	/*if (!ideal)*/rbBand.cxIdeal = cxIdeal;
	/*if (!minimum)*/rbBand.cyMinChild = cyChild;
	rbBand.cyChild = cyChild;

	/*
	 * Do not set the size for the last item on the row.
	 */
	//if (!_w_coolitem_is_last_item_of_row(parent, index)) {
	if (_COMCTL32_VERSION >= VERSION(6, 0)) {
		MARGINS margins;
		SendMessageW(hwnd, RB_GETBANDMARGINS, 0, (LPARAM) &margins);
		cx -= margins.cxLeftWidth + margins.cxRightWidth;
	}
	int separator =
			(_W_WIDGET(parent)->style & W_FLAT) == 0 ? SEPARATOR_WIDTH : 0;
	rbBand.cx = cx - separator;
	rbBand.fMask |= RBBIM_SIZE;
	//}
	SendMessageW(hwnd, RB_SETBANDINFO, index, (LPARAM) &rbBand);
	return TRUE;
}
wresult _w_coolitem_set_wrap_indice(w_coolitem *coolitem, int wrap_indice) {
	return W_FALSE;
}
/*
 * CoolBar
 */
wuint64 _w_coolbar_check_style(w_widget *widget, wuint64 style) {
	style |= W_NO_FOCUS;
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_coolbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	HWND handle = _W_WIDGET(widget)->handle;
	int width = 0, height = 0;
	int border = _w_control_get_border_width(W_CONTROL(widget));
	int newWidth = e->wHint == W_DEFAULT ? 0x3FFF : e->wHint + (border * 2);
	int newHeight = e->hHint == W_DEFAULT ? 0x3FFF : e->hHint + (border * 2);
	int count = (int) SendMessageW(handle, RB_GETBANDCOUNT, 0, 0);
	if (count != 0) {
		//ignoreResize = true;
		_W_WIDGET(widget)->state |= STATE_IGNORE_RESIZE;
		int redraw = FALSE;
		if (IsWindowVisible(handle)) {
			if (_COMCTL32_VERSION >= VERSION(6, 0)) {
				redraw = TRUE;
				UpdateWindow(handle);
				DefWindowProcW(handle, WM_SETREDRAW, 0, 0);
			} else {
				redraw = _w_control_get_drawing(W_CONTROL(widget));
				if (redraw) {
					UpdateWindow(handle);
					SendMessageW(handle, WM_SETREDRAW, 0, 0);
				}
			}
		}
		RECT oldRect;
		GetWindowRect(handle, &oldRect);
		int oldWidth = oldRect.right - oldRect.left;
		int oldHeight = oldRect.bottom - oldRect.top;
		int flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER;
		SetWindowPos(handle, 0, 0, 0, newWidth, newHeight, flags);
		RECT rect;
		SendMessageW(handle, RB_GETRECT, count - 1, (LPARAM) &rect);
		height = WMAX(height, rect.bottom);
		SetWindowPos(handle, 0, 0, 0, oldWidth, oldHeight, flags);
		REBARBANDINFOW rbBand;
		rbBand.cbSize = sizeof(REBARBANDINFOW);
		rbBand.fMask = RBBIM_IDEALSIZE | RBBIM_STYLE;
		int rowWidth = 0;
		for (int i = 0; i < count; i++) {
			SendMessageW(handle, RB_GETBANDINFO, i, (LPARAM) &rbBand);
			if ((rbBand.fStyle & RBBS_BREAK) != 0) {
				width = WMAX(width, rowWidth);
				rowWidth = 0;
			}
			rowWidth += rbBand.cxIdeal
					+ _w_coolitem_get_margin(W_COOLBAR(widget), i);
		}
		width = WMAX(width, rowWidth);
		if (redraw) {
			if (_COMCTL32_VERSION >= VERSION(6, 0)) {
				DefWindowProcW(handle, WM_SETREDRAW, 1, 0);
			} else {
				SendMessageW(handle, WM_SETREDRAW, 1, 0);
			}
		}
		//ignoreResize = false;
		_W_WIDGET(widget)->state &= ~STATE_IGNORE_RESIZE;
	}
	if (width == 0)
		width = DEFAULT_COOLBAR_WIDTH;
	if (height == 0)
		height = DEFAULT_COOLBAR_HEIGHT;
	if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
		int tmp = width;
		width = height;
		height = tmp;
	}
	if (e->wHint != W_DEFAULT)
		width = e->wHint;
	if (e->hHint != W_DEFAULT)
		height = e->hHint;
	height += border * 2;
	width += border * 2;
	e->size->width = width;
	e->size->height = height;
	return TRUE;
}
wresult _w_coolbar_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result = _w_composite_create_handle(control, priv);
	if (result > 0) {
		_W_WIDGET(control)->state &= ~(STATE_CANVAS | STATE_THEME_BACKGROUND);
		HWND handle = _W_WIDGET(control)->handle;
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
		if ((_W_WIDGET(control)->style & W_VERTICAL) != 0) {
			_W_WIDGET(control)->style |= W_VERTICAL;
			int bits = GetWindowLongW(handle, GWL_STYLE);
			SetWindowLongW(handle, GWL_STYLE, bits | CCS_VERT);
		} else {
			_W_WIDGET(control)->style |= W_HORIZONTAL;
		}
	}
	return result;
}
wresult _w_coolbar_get_item(w_coolbar *coolbar, int index, w_coolitem *item) {
	return W_FALSE;
}
wresult _w_coolbar_get_item_count(w_coolbar *coolbar) {
	return W_FALSE;
}
wresult _w_coolbar_get_items(w_coolbar *coolbar, w_iterator *items) {
	return W_FALSE;
}
wresult _w_coolbar_get_locked(w_coolbar *coolbar) {
	return W_FALSE;
}
wresult _w_coolbar_insert_item(w_coolbar *coolbar, w_coolitem *item, int style,
		int index) {
	int count = (int) SendMessageW(_W_WIDGET(coolbar)->handle, RB_GETBANDCOUNT,
			0, 0);
	if (!(0 <= index && index <= count)) {
		index = count;
		//return W_ERROR_INVALID_RANGE;
	}
	REBARBANDINFOW rbBand;
	rbBand.cbSize = sizeof(REBARBANDINFOW);
	rbBand.fMask = RBBIM_TEXT | RBBIM_STYLE | RBBIM_ID;
	rbBand.fStyle = RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
	if ((style & W_DROP_DOWN) != 0) {
		rbBand.fStyle |= RBBS_USECHEVRON;
	}
	WCHAR lpText[2];
	lpText[0] = 0;
	rbBand.lpText = lpText;
	rbBand.wID = index;

	/*
	 * Feature in Windows.  When inserting an item at end of a row,
	 * sometimes, Windows will begin to place the item on the right
	 * side of the cool bar.  The fix is to resize the new items to
	 * the maximum size and then resize the next to last item to the
	 * ideal size.
	 */
	/*int lastIndex = getLastIndexOfRow (index - 1);
	 wresult fixLast = index == lastIndex + 1;
	 if (fixLast) {
	 rbBand.fMask |= RBBIM_SIZE;
	 rbBand.cx = MAX_WIDTH;
	 }*/

	/*
	 * Feature in Windows. Is possible that the item at index zero
	 * has the RBBS_BREAK flag set. When a new item is inserted at
	 * position zero, the previous item at position zero moves to
	 * a new line.  The fix is to detect this case and clear the
	 * RBBS_BREAK flag on the previous item before inserting the
	 * new item.
	 */
	if (index == 0 && count > 0) {
		//getItem (0).setWrap (false);
	}

	/* Insert the item */
	if (SendMessageW(_W_WIDGET(coolbar)->handle, RB_INSERTBAND, index,
			(LPARAM) &rbBand) == 0) {
		return W_ERROR_ITEM_NOT_ADDED;
	}

	/* Resize the next to last item to the ideal size */
	/*if (fixLast) {
	 resizeToPreferredWidth (lastIndex);
	 }*/
	if (item != 0) {
		W_WIDGETDATA(item)->clazz = _W_COOLBAR_GET_ITEM_CLASS(coolbar);
		_W_ITEM(item)->parent = W_WIDGET(coolbar);
		_W_ITEM(item)->index = index;
	}
	return W_TRUE;
}
wresult _w_coolbar_set_locked(w_coolbar *coolbar, int locked) {
	return W_FALSE;
}
DWORD _w_coolbar_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_composite_widget_style(control,
			priv) | CCS_NODIVIDER | CCS_NORESIZE;
	wuint64 style = _W_WIDGET(control)->style;
	bits |= RBS_VARHEIGHT | RBS_DBLCLKTOGGLE;
	if ((style & W_FLAT) == 0)
		bits |= RBS_BANDBORDERS;
	return bits;
}
WCHAR* _w_coolbar_window_class(w_control *control, _w_control_priv *priv) {
	return REBARCLASSNAMEW;
}
/*
 *	messages
 */
wresult _COOLBAR_WM_COMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Feature in Windows.  When the coolbar window
	 * proc processes WM_COMMAND, it forwards this
	 * message to its parent.  This is done so that
	 * children of this control that send this message
	 * type to their parent will notify not only
	 * this control but also the parent of this control,
	 * which is typically the application window and
	 * the window that is looking for the message.
	 * If the control did not forward the message,
	 * applications would have to subclass the control
	 * window to see the message. Because the control
	 * window is subclassed by SWT, the message
	 * is delivered twice, once by SWT and once when
	 * the message is forwarded by the window proc.
	 * The fix is to avoid calling the window proc
	 * for this control.
	 */
	wresult result = _CONTROL_WM_COMMAND(widget, e, priv);
	if (result)
		return result;
	e->result = 0;
	return W_TRUE;
}
wresult _COOLBAR_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_ERASEBKGND(widget, e, priv);
	/*
	 * Feature in Windows.  For some reason, Windows
	 * does not fully erase the area that the cool bar
	 * occupies when the size of the cool bar is larger
	 * than the space occupied by the cool bar items.
	 * The fix is to erase the cool bar background.
	 *
	 * NOTE: On versions of Windows prior to XP, for
	 * some reason, the cool bar draws separators in
	 * WM_ERASEBKGND.  Therefore it is essential to run
	 * the cool bar window proc after the background has
	 * been erased.
	 */
	if (_COMCTL32_VERSION < VERSION(6, 0) || !IsAppThemed()) {
		RECT r;
		GetClientRect(e->hwnd, &r);
		priv->draw_background(W_CONTROL(widget), (HDC) e->wparam, &r, -1, 0, 0,
				priv);
		return W_FALSE;
	}
	return result;
}
wresult _COOLBAR_WM_NOTIFY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Feature in Windows.  When the cool bar window
	 * proc processes WM_NOTIFY, it forwards this
	 * message to its parent.  This is done so that
	 * children of this control that send this message
	 * type to their parent will notify not only
	 * this control but also the parent of this control,
	 * which is typically the application window and
	 * the window that is looking for the message.
	 * If the control did not forward the message,
	 * applications would have to subclass the control
	 * window to see the message. Because the control
	 * window is subclassed by SWT, the message
	 * is delivered twice, once by SWT and once when
	 * the message is forwarded by the window proc.
	 * The fix is to avoid calling the window proc
	 * for this control.
	 */
	wresult result = _CONTROL_WM_NOTIFY(widget, e, priv);
	if (result)
		return result;
	e->result = 0;
	return W_TRUE;
}
wresult _COOLBAR_WM_SETREDRAW(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Feature in Windows.  When redraw is turned off, the rebar
	 * control does not call the default window proc.  This means
	 * that the rebar will redraw and children of the rebar will
	 * also redraw.  The fix is to call both the rebar window proc
	 * and the default window proc.
	 *
	 * NOTE: The rebar control can resize itself in WM_SETREDRAW.
	 * When redraw is turned off by the default window proc, this
	 * can leave pixel corruption in the parent.  The fix is to
	 * detect the size change and damage the previous area in the
	 * parent.
	 *
	 * NOTE:  In version 6.00 of COMCTL32.DLL, when WM_SETREDRAW
	 * is off, we cannot detect that the size has changed causing
	 * pixel corruption.  The fix is to disallow WM_SETREDRAW by
	 * not running the default window proc or the rebar window
	 * proc.
	 */
	if (_COMCTL32_VERSION >= VERSION(6, 0)) {
		e->result = 0;
		return W_TRUE;
	}
	w_rect rect, r;
	w_control_get_bounds(W_CONTROL(widget), &rect.pt, &rect.sz);
	priv->widget.call_window_proc(widget, e, priv);
	DefWindowProcW(e->hwnd, WM_SETREDRAW, e->wparam, e->lparam);
	w_control_get_bounds(W_CONTROL(widget), &r.pt, &r.sz);
	if (!w_rect_equals(&r, &rect)) {
		w_widget *parent = _W_WIDGET(widget)->parent;
		w_control_redraw(W_CONTROL(parent), &rect, W_TRUE);
	}
	return W_TRUE;
}
wresult _COOLBAR_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_WIDGET(widget)->state & STATE_IGNORE_RESIZE) {
		priv->widget.call_window_proc(widget, e, priv);
		return W_TRUE;
	}
	return _COMPOSITE_WM_SIZE(widget, e, priv);
}
wresult _COOLBAR_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMHDR *hdr = (NMHDR*) e->lparam;
	HWND handle = _W_WIDGET(widget)->handle;
	_w_item item;
	w_rect r;
	w_event_coolbar event;
	switch (hdr->code) {
	case RBN_BEGINDRAG: {
		int pos = GetMessagePos();
		POINT pt;
		POINTSTOPOINT(pt, pos);
		ScreenToClient(handle, &pt);
		int button = win_toolkit->lastButton != 0 ? win_toolkit->lastButton : 1;
		//if (!sendDragEvent (button, pt.x, pt.y)) return LRESULT.ONE;
		break;
	}
	case RBN_CHILDSIZE: {
		/*
		 * Bug in Windows.  When Windows sets the size of the rebar band
		 * child and the child is a combo box, the size of the drop down
		 * portion of the combo box is resized to zero.  The fix is to set
		 * the size of the control to the current size after the rebar has
		 * already resized it.  If the control is not a combo, this does
		 * nothing.  If the control is a combo, the drop down portion is
		 * recalculated.
		 */
		NMREBARCHILDSIZE *lprbcs = (NMREBARCHILDSIZE*) e->lparam;
		if (lprbcs->uBand != -1) {
			int index = lprbcs->wID & 0xFFFF;
			W_WIDGETDATA(&item)->clazz = _W_COOLBAR_GET_ITEM_CLASS(widget);
			_W_ITEM(&item)->parent = widget;
			_W_ITEM(&item)->index = index;
			w_control *control;
			_w_coolitem_get_control(W_COOLITEM(&item), &control);
			if (control != 0) {
				r.x = lprbcs->rcChild.left;
				r.y = lprbcs->rcChild.top;
				r.width = lprbcs->rcChild.right - lprbcs->rcChild.left;
				r.height = lprbcs->rcChild.bottom - lprbcs->rcChild.top;
				w_control_set_bounds(control, &r.pt, &r.sz);
			}
		}
		break;
	}
	case RBN_HEIGHTCHANGE: {
		if ((_W_WIDGET(widget)->state & STATE_IGNORE_RESIZE) == 0) {
			w_control_get_bounds(W_CONTROL(widget), 0, &r.sz);
			int border = w_control_get_border_width(W_CONTROL(widget));
			int barHeight = SendMessageW(handle, RB_GETBARHEIGHT, 0, 0);
			if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
				r.width = barHeight + 2 * border;
			} else {
				r.height = barHeight + 2 * border;
			}
			w_control_set_bounds(W_CONTROL(widget), 0, &r.sz);
		}
		break;
	}
	case RBN_CHEVRONPUSHED: {
		NMREBARCHEVRON *lpnm = (NMREBARCHEVRON*) e->lparam;
		int index = lpnm->wID & 0xFFFF;
		W_WIDGETDATA(&item)->clazz = _W_COOLBAR_GET_ITEM_CLASS(widget);
		_W_ITEM(&item)->parent = widget;
		_W_ITEM(&item)->index = index;
		event.event.type = W_EVENT_ITEM_SELECTION;
		event.event.widget = widget;
		event.event.platform_event = _EVENT_PLATFORM(e);
		event.event.time = 0;
		event.event.data = 0;
		event.detail = W_ARROW;
		if ((W_WIDGET(widget)->style & W_VERTICAL) != 0) {
			event.location.x = lpnm->rc.right;
			event.location.y = lpnm->rc.top;
		} else {
			event.location.x = lpnm->rc.left;
			event.location.y = lpnm->rc.bottom;
		}
		_w_widget_post_event(widget, W_EVENT(&event),W_EVENT_SEND);
		break;
	}
	case NM_CUSTOMDRAW: {
		/*
		 * Bug in Windows.  On versions of Windows prior to XP,
		 * drawing the background color in NM_CUSTOMDRAW erases
		 * the separators.  The fix is to draw the background
		 * in WM_ERASEBKGND.
		 */
		if (_COMCTL32_VERSION < VERSION(6, 0))
			break;
		w_control *control = priv->find_background_control(W_CONTROL(widget),
				priv);
		if (control != 0 || (_W_WIDGET(widget)->style & W_FLAT) != 0) {
			NMCUSTOMDRAW *nmcd = (NMCUSTOMDRAW*) e->lparam;
			switch (nmcd->dwDrawStage) {
			case CDDS_PREERASE:
				e->result = CDRF_SKIPDEFAULT | CDRF_NOTIFYPOSTERASE;
				return W_TRUE;
				break;
			case CDDS_POSTERASE:
				priv->draw_background(W_CONTROL(widget), nmcd->hdc, &nmcd->rc,
						-1, 0, 0, priv);
				break;
			}
		}
		break;
	}
	}
	return W_FALSE;
}
void _w_coolbar_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_coolbar_class *clazz) {
	if (classId == _W_CLASS_COOLBAR) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_coolbar_priv;
	}
	_w_composite_class_init(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_coolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_coolbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_coolbar);
	/*
	 * public function
	 */
	clazz->get_item = _w_coolbar_get_item;
	clazz->get_item_count = _w_coolbar_get_item_count;
	clazz->get_items = _w_coolbar_get_items;
	clazz->get_locked = _w_coolbar_get_locked;
	clazz->insert_item = _w_coolbar_insert_item;
	clazz->set_locked = _w_coolbar_set_locked;
	/*
	 * item
	 */
	struct _w_coolitem_class *item = clazz->class_coolitem;
	W_WIDGETDATA_CLASS(item)->toolkit = W_WIDGET_CLASS(clazz)->toolkit;
	_w_item_class_init(W_ITEM_CLASS(item));
	item->compute_size = _w_coolitem_compute_size;
	item->get_bounds = _w_coolitem_get_bounds;
	item->get_control = _w_coolitem_get_control;
	item->get_minimum_size = _w_coolitem_get_minimum_size;
	item->get_order = _w_coolitem_get_order;
	item->get_preferred_size = _w_coolitem_get_preferred_size;
	item->get_size = _w_coolitem_get_size;
	item->get_wrap_indice = _w_coolitem_get_wrap_indice;
	item->set_control = _w_coolitem_set_control;
	item->set_layout = _w_coolitem_set_layout;
	item->set_minimum_size = _w_coolitem_set_minimum_size;
	item->set_order = _w_coolitem_set_order;
	item->set_preferred_size = _w_coolitem_set_preferred_size;
	item->set_size = _w_coolitem_set_size;
	item->set_wrap_indice = _w_coolitem_set_wrap_indice;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_COOLBAR) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->check_style = _w_coolbar_check_style;
		priv->compute_size = _w_coolbar_compute_size;
		priv->create_handle = _w_coolbar_create_handle;
		priv->widget_style = _w_coolbar_widget_style;
		priv->window_class = _w_coolbar_window_class;
		/*
		 * messages
		 */
		dispatch_message *msg = priv->messages;
		msg[_WM_COMMAND] = _COOLBAR_WM_COMMAND;
		msg[_WM_ERASEBKGND] = _COOLBAR_WM_ERASEBKGND;
		msg[_WM_NOTIFY] = _COOLBAR_WM_NOTIFY;
		msg[_WM_SETREDRAW] = _COOLBAR_WM_SETREDRAW;
		msg[_WM_SIZE] = _COOLBAR_WM_SIZE;
		msg[_WM_NOTIFYCHILD] = _COOLBAR_WM_NOTIFYCHILD;
	}
}
