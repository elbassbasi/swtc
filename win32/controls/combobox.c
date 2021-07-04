/*
 * combobox.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "combobox.h"
#include "../widgets/toolkit.h"
#define CBID_LIST 1000
#define CBID_EDIT 1001
/*
 *
 */
wresult _w_comboitem_get_data(w_item *item, void **data) {
	return W_FALSE;
}
wresult _w_comboitem_get_index(w_item *item) {
	return W_FALSE;
}
wresult _w_comboitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_comboitem_set_data(w_item *item, void *data) {
	wresult result = W_FALSE;
	w_widget *combo = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(combo)->handle;
	COMBOBOXEXITEMW cbei;
	cbei.mask = CBEIF_LPARAM;
	cbei.iItem = _W_ITEM(item)->index;
	cbei.lParam = (LPARAM) data;
	if (SendMessageW(handle, CBEM_SETITEMW, 0, (LPARAM) &cbei) != -1) {
		result = W_TRUE;
	}
	return result;
}
wresult _w_comboitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	w_widget *combo = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(combo)->handle;
	WCHAR *str;
	int newlength;
	wresult result = _win_text_fix(text, length, enc, &str, &newlength);
	if (result > 0) {
		COMBOBOXEXITEMW cbei;
		cbei.mask = CBEIF_TEXT;
		cbei.iItem = _W_ITEM(item)->index;
		cbei.pszText = str;
		cbei.cchTextMax = newlength;
		if (SendMessageW(handle, CBEM_SETITEMW, 0, (LPARAM) &cbei) != -1) {
			result = W_TRUE;
		}
	}
	_win_text_free(text, str, newlength);
	return result;
}
wresult _w_comboitem_get_image(w_comboitem *item) {
	return W_FALSE;
}
wresult _w_comboitem_set_image(w_comboitem *item, int image) {
	wresult result = W_FALSE;
	w_widget *combo = _W_ITEM(item)->parent;
	HWND handle = _W_WIDGET(combo)->handle;
	COMBOBOXEXITEMW cbei;
	cbei.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
	cbei.iItem = _W_ITEM(item)->index;
	cbei.iImage = image;
	cbei.iSelectedImage = image;
	if (SendMessageW(handle, CBEM_SETITEMW, 0, (LPARAM) &cbei) != -1) {
		result = W_TRUE;
	}
	return result;
}
/*
 *	combobox
 */
wuint64 _w_combobox_check_style(w_widget *widget, wuint64 style) {
	/*
	 * Feature in Windows.  It is not possible to create
	 * a combo box that has a border using Windows style
	 * bits.  All combo boxes draw their own border and
	 * do not use the standard Windows border styles.
	 * Therefore, no matter what style bits are specified,
	 * clear the BORDER bits so that the SWT style will
	 * match the Windows widget.
	 *
	 * The Windows behavior is currently implemented on
	 * all platforms.
	 */
	style &= ~W_BORDER;
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	style &= ~(W_HSCROLL | W_VSCROLL);
	style = _w_widget_check_bits(style, W_DROP_DOWN, W_SIMPLE, 0, 0, 0, 0);
	if ((style & W_SIMPLE) != 0)
		return style & ~W_READ_ONLY;
	return style;
}
wresult _w_combobox_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int width = 0, height = 0;
	wuint64 style = _W_WIDGET(widget)->style;
	int wHint = e->wHint;
	int hHint = e->hHint;
	HWND handle = _W_WIDGET(widget)->handle;
	if (wHint == W_DEFAULT) {
		HFONT newFont, oldFont = 0;
		HDC hDC = GetDC(handle);
		newFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
		if (newFont != 0)
			oldFont = SelectObject(hDC, newFont);
		int count = SendMessageW(handle, CB_GETCOUNT, 0, 0);
		RECT rect;
		int flags = DT_CALCRECT | DT_NOPREFIX;
		if ((style & W_READ_ONLY) == 0)
			flags |= DT_EDITCONTROL;
		int length = GetWindowTextLengthW(handle);
		WCHAR *buffer = _w_toolkit_malloc((length + 1) * sizeof(WCHAR));
		GetWindowTextW(handle, buffer, length + 1);
		DrawTextW(hDC, buffer, length, &rect, flags);
		_w_toolkit_free(buffer, (length + 1) * sizeof(WCHAR));
		width = WMAX(width, rect.right - rect.left);
		if ((style & W_HSCROLL) != 0) {
			width = WMAX(width, _W_COMBOBOX(widget)->scrollWidth);
		} else {
			for (int i = 0; i < count; i++) {
				length = SendMessageW(handle, CB_GETLBTEXTLEN, i, 0);
				if (length != CB_ERR) {
					WCHAR *buffer = _w_toolkit_malloc(
							(length + 1) * sizeof(WCHAR));
					int result = SendMessageW(handle, CB_GETLBTEXT, i,
							(LPARAM) buffer);
					if (result != CB_ERR) {
						DrawTextW(hDC, buffer, length, &rect, flags);
						width = WMAX(width, rect.right - rect.left);
					}
					_w_toolkit_free(buffer, (length + 1) * sizeof(WCHAR));
				}
			}
		}
		if (newFont != 0)
			SelectObject(hDC, oldFont);
		ReleaseDC(handle, hDC);
	}
	if (hHint == W_DEFAULT) {
		if ((style & W_SIMPLE) != 0) {
			int count = SendMessageW(handle, CB_GETCOUNT, 0, 0);
			int itemHeight = SendMessageW(handle, CB_GETITEMHEIGHT, 0, 0);
			height = count * itemHeight;
		}
	}
	if (width == 0)
		width = DEFAULT_WIDTH;
	if (height == 0)
		height = DEFAULT_HEIGHT;
	if (wHint != W_DEFAULT)
		width = wHint;
	if (hHint != W_DEFAULT)
		height = hHint;
	if ((style & W_READ_ONLY) != 0) {
		width += 8;
	} else {
		HWND hwndText = GetDlgItem(handle, CBID_EDIT);
		if (hwndText != 0) {
			LRESULT margins = SendMessageW(hwndText, EM_GETMARGINS, 0, 0);
			int marginWidth = LOWORD (margins) + HIWORD(margins);
			width += marginWidth + 3;
		}
	}
	COMBOBOXINFO pcbi;
	pcbi.cbSize = sizeof(pcbi);
	if (((style & W_SIMPLE) == 0) && GetComboBoxInfo(handle, &pcbi)) {
		width += pcbi.rcItem.left + (pcbi.rcButton.right - pcbi.rcButton.left);
		height = (pcbi.rcButton.bottom - pcbi.rcButton.top)
				+ pcbi.rcButton.top * 2;
	} else {
		int border = GetSystemMetrics(SM_CXEDGE);
		width += GetSystemMetrics(SM_CXVSCROLL) + border * 2;
		int textHeight = SendMessageW(handle, CB_GETITEMHEIGHT, -1, 0);
		if ((style & W_DROP_DOWN) != 0) {
			height = textHeight + 6;
		} else {
			height += textHeight + 10;
		}
	}
	if ((style & W_SIMPLE) != 0 && (style & W_HSCROLL) != 0) {
		height += GetSystemMetrics(SM_CYHSCROLL);
	}
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
}
wresult _w_combobox_clear_selection(w_combobox *combo) {
	return W_FALSE;
}
LRESULT CALLBACK CBTProc(int code, WPARAM wParam, LPARAM lParam) {
	return 0;
}
wresult _w_combobox_create_handle(w_control *control, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	HWND handle = _W_WIDGET(control)->handle;
	wresult result;
	/*
	 * Feature in Windows.  When the selection changes in a combo box,
	 * Windows draws the selection, even when the combo box does not
	 * have focus.  Strictly speaking, this is the correct Windows
	 * behavior because the combo box sets ES_NOHIDESEL on the text
	 * control that it creates.  Despite this, it looks strange because
	 * Windows also clears the selection and selects all the text when
	 * the combo box gets focus.  The fix is use the CBT hook to clear
	 * the ES_NOHIDESEL style bit when the text control is created.
	 */
	if ((style & (W_READ_ONLY | W_SIMPLE)) != 0) {
		result = _w_composite_create_handle(control, priv);
	} else {
		DWORD threadId = GetCurrentThreadId();
		HHOOK cbtHook = SetWindowsHookExW(WH_CBT, CBTProc, 0, threadId);
		result = _w_composite_create_handle(control, priv);
		if (cbtHook != 0)
			UnhookWindowsHookEx(cbtHook);
	}
	_W_WIDGET(control)->state &= ~(STATE_CANVAS | STATE_THEME_BACKGROUND);

	_w_combobox_priv *cpriv = _W_COMBOBOX_PRIV(priv);
	/* Get the text and list window procs */
	HWND hwndText = (HWND) SendMessageW(handle, CBEM_GETCOMBOCONTROL, 0, 0);
	if (hwndText != 0 && cpriv->EditProc == 0) {
		cpriv->EditProc = (WNDPROC) GetWindowLongPtrW(hwndText, GWLP_WNDPROC);
	}
	HWND hwndList = GetDlgItem(handle, CBID_LIST);
	if (hwndList != 0 && cpriv->ListProc == 0) {
		cpriv->ListProc = (WNDPROC) GetWindowLongPtrW(hwndList, GWLP_WNDPROC);
	}

	/*
	 * Bug in Windows.  If the combo box has the CBS_SIMPLE style,
	 * the list portion of the combo box is not drawn correctly the
	 * first time, causing pixel corruption.  The fix is to ensure
	 * that the combo box has been resized more than once.
	 */
	if ((style & W_SIMPLE) != 0) {
		int flags = SWP_NOZORDER | SWP_DRAWFRAME | SWP_NOACTIVATE;
		SetWindowPos(handle, 0, 0, 0, 0x3FFF, 0x3FFF, flags);
		SetWindowPos(handle, 0, 0, 0, 0, 0, flags);
	}
	return result;
}
wresult _w_combobox_copy(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_cut(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_deselect(w_combobox *combo, int index) {
	return W_FALSE;
}
wresult _w_combobox_deselect_all(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_get_imagelist(w_combobox *combo, w_imagelist **imagelist) {
	return W_FALSE;
}
wresult _w_combobox_get_item(w_combobox *combo, int index, w_comboitem *item) {
	return W_FALSE;
}
wresult _w_combobox_get_item_count(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_get_item_height(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_get_items(w_combobox *combo, w_iterator *items) {
	return W_FALSE;
}
wresult _w_combobox_get_list_visible(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_get_selection(w_combobox *combo, w_point *pt) {
	return W_FALSE;
}
wresult _w_combobox_get_selection_index(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_get_text(w_combobox *combo, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_combobox_get_text_height(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_get_text_limit(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_get_visible_item_count(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_indexof(w_combobox *combo, const char *string, int length,
		int enc, int start) {
	return W_FALSE;
}
wresult _w_combobox_insert_item(w_combobox *combo, w_comboitem *item,
		int index) {
	wresult result = W_FALSE;
	HWND handle = _W_WIDGET(combo)->handle;
	if (index < 0) {
		index = -1;
	}
	COMBOBOXEXITEMW cbei;
	ZeroMemory(&cbei, sizeof(cbei));
	cbei.mask = CBEIF_TEXT | CBEIF_INDENT;
	cbei.iItem = index;
	cbei.cchTextMax = 1;
	cbei.pszText = L" ";
	cbei.iIndent = 0;
	int newIndex = SendMessageW(handle, CBEM_INSERTITEMW, 0, (LPARAM) &cbei);
	if (newIndex != -1) {
		if (item != 0) {
			W_WIDGETDATA(item)->clazz = _W_COMBOBOX_GET_ITEM_CLASS(combo);
			_W_ITEM(item)->parent = W_WIDGET(combo);
			_W_ITEM(item)->index = newIndex;
		}
		result = W_TRUE;
	}
	return result;
}
wresult _w_combobox_paste(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_remove_index(w_combobox *combo, int index) {
	return W_FALSE;
}
wresult _w_combobox_remove_range(w_combobox *combo, int start, int end) {
	return W_FALSE;
}
wresult _w_combobox_remove_string(w_combobox *combo, const char *string,
		int length, int enc) {
	return W_FALSE;
}
wresult _w_combobox_remove_all(w_combobox *combo) {
	return W_FALSE;
}
wresult _w_combobox_select(w_combobox *combo, int index) {
	return W_FALSE;
}
wresult _w_combobox_set_imagelist(w_combobox *combo, w_imagelist *imagelist) {
	HWND handle = _W_WIDGET(combo)->handle;
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
	_W_COMBOBOX(combo)->imagelist = imagelist;
	SendMessageW(handle, CBEM_SETIMAGELIST, 0, (LPARAM) hImageList);
	return ret;
}
wresult _w_combobox_set_list_visible(w_combobox *combo, int visible) {
	return W_FALSE;
}
wresult _w_combobox_set_selection(w_combobox *combo, w_point *selection) {
	return W_FALSE;
}
wresult _w_combobox_set_text(w_combobox *combo, const char *string, int length,
		int enc) {
	return W_FALSE;
}
wresult _w_combobox_set_text_limit(w_combobox *combo, int limit) {
	return W_FALSE;
}
wresult _w_combobox_set_visible_item_count(w_combobox *combo, int count) {
	return W_FALSE;
}
DWORD _w_combobox_widget_extstyle(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_scrollable_widget_extstyle(control, priv);
	return bits & ~WS_EX_NOINHERITLAYOUT;
}

DWORD _w_combobox_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_composite_widget_style(control, priv);
	wuint64 style = _W_WIDGET(control)->style;
	bits |= CBS_AUTOHSCROLL | CBS_NOINTEGRALHEIGHT | WS_HSCROLL | WS_VSCROLL;
	if ((style & W_SIMPLE) != 0)
		return bits | CBS_SIMPLE;
	if ((style & W_READ_ONLY) != 0)
		return bits | CBS_DROPDOWNLIST;
	return bits | CBS_DROPDOWN;
}

WCHAR* _w_combobox_window_class(w_control *control,
		_w_control_priv *priv) {
	return WC_COMBOBOXEXW;
}

void _w_combobox_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_combobox_class *clazz) {
	if (classId == _W_CLASS_COMBOBOX) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_combobox_priv;
	}
	_w_composite_class_init(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMBOBOX;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_combobox_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_combobox);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_combobox);
	/*
	 * public
	 */
	clazz->clear_selection = _w_combobox_clear_selection;
	clazz->copy = _w_combobox_copy;
	clazz->cut = _w_combobox_cut;
	clazz->deselect = _w_combobox_deselect;
	clazz->deselect_all = _w_combobox_deselect_all;
	clazz->get_imagelist = _w_combobox_get_imagelist;
	clazz->get_item = _w_combobox_get_item;
	clazz->get_item_count = _w_combobox_get_item_count;
	clazz->get_item_height = _w_combobox_get_item_height;
	clazz->get_items = _w_combobox_get_items;
	clazz->get_list_visible = _w_combobox_get_list_visible;
	clazz->get_selection = _w_combobox_get_selection;
	clazz->get_selection_index = _w_combobox_get_selection_index;
	clazz->get_text = _w_combobox_get_text;
	clazz->get_text_height = _w_combobox_get_text_height;
	clazz->get_text_limit = _w_combobox_get_text_limit;
	clazz->get_visible_item_count = _w_combobox_get_visible_item_count;
	clazz->indexof = _w_combobox_indexof;
	clazz->insert_item = _w_combobox_insert_item;
	clazz->paste = _w_combobox_paste;
	clazz->remove_index = _w_combobox_remove_index;
	clazz->remove_range = _w_combobox_remove_range;
	clazz->remove_string = _w_combobox_remove_string;
	clazz->remove_all = _w_combobox_remove_all;
	clazz->select = _w_combobox_select;
	clazz->set_imagelist = _w_combobox_set_imagelist;
	clazz->set_list_visible = _w_combobox_set_list_visible;
	clazz->set_selection = _w_combobox_set_selection;
	clazz->set_text = _w_combobox_set_text;
	clazz->set_text_limit = _w_combobox_set_text_limit;
	clazz->set_visible_item_count = _w_combobox_set_visible_item_count;
	/*
	 * combo item
	 */
	struct _w_comboitem_class *item = W_COMBOITEM_CLASS(clazz->class_comboitem);
	W_WIDGETDATA_CLASS(item)->toolkit = W_WIDGET_CLASS(clazz)->toolkit;
	_w_item_class_init(W_ITEM_CLASS(item));
	W_ITEM_CLASS(item)->get_data = _w_comboitem_get_data;
	W_ITEM_CLASS(item)->get_text = _w_comboitem_get_text;
	W_ITEM_CLASS(item)->set_data = _w_comboitem_set_data;
	W_ITEM_CLASS(item)->set_text = _w_comboitem_set_text;
	item->get_image = _w_comboitem_get_image;
	item->set_image = _w_comboitem_set_image;
	/*
	 * priv
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_COMBOBOX) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->check_style = _w_combobox_check_style;
		priv->compute_size = _w_combobox_compute_size;
		priv->create_handle = _w_combobox_create_handle;
		priv->widget_style = _w_combobox_widget_style;
		priv->widget_extstyle = _w_combobox_widget_extstyle;
		priv->window_class = _w_combobox_window_class;
	}
}
