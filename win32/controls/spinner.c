/*
 * spinner.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "spinner.h"
#include "../widgets/toolkit.h"
wresult _w_spinner_call_window_proc(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_WIDGET(widget)->handle == 0)
		return W_FALSE;
	if (e->hwnd == _W_SPINNER(widget)->hwndText) {
		return CallWindowProcW(_W_SPINNER_PRIV(priv)->EditProc, e->hwnd, e->msg,
				e->wparam, e->lparam);
	}
	if (e->hwnd == _W_SPINNER(widget)->hwndUpDown) {
		return CallWindowProcW(_W_SPINNER_PRIV(priv)->UpDownProc, e->hwnd,
				e->msg, e->wparam, e->lparam);
	}
	return DefWindowProcW(_W_WIDGET(widget)->handle, e->msg, e->wparam,
			e->lparam);
}
wuint64 _w_spinner_check_style(w_widget *widget, wuint64 style) {
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_spinner_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	w_rect rect;
	memset(&rect, 0, sizeof(rect));
	HWND hwndText = _W_SPINNER(widget)->hwndText;
	HWND hwndUpDown = _W_SPINNER(widget)->hwndUpDown;
	if (wHint == W_DEFAULT || hHint == W_DEFAULT) {
		HFONT newFont, oldFont = 0;
		HDC hDC = GetDC(hwndText);
		newFont = (HFONT) SendMessageW(hwndText, WM_GETFONT, 0, 0);
		if (newFont != 0)
			oldFont = SelectObject(hDC, newFont);
		TEXTMETRICW tm;
		GetTextMetricsW(hDC, &tm);
		rect.height = tm.tmHeight;
		RECT _rect;
		int max = 0;
		SendMessageW(hwndUpDown, UDM_GETRANGE32, 0, (LPARAM) &max);
		int digits = _W_SPINNER(widget)->digits;
		int size = digits + 20;
		WCHAR *buffer = _w_toolkit_malloc(size);
		if (buffer != 0) {
			_itow(max, buffer, 10);
			int length = lstrlenW(buffer);
			if (digits > 0) {
				int newlength = digits - length;
				newlength += GetLocaleInfoW(LOCALE_USER_DEFAULT,
				LOCALE_SDECIMAL, &buffer[length], 4);
				buffer[newlength] = 0;
				while (length < newlength) {
					buffer[newlength] = '0';
					length++;
				}
				length = newlength;
			} else {
				for (int i = 0; i < 1; i++) {
					buffer[length++] = '0';
				}
				buffer[length] = 0;
			}
			int flags = DT_CALCRECT | DT_EDITCONTROL | DT_NOPREFIX;
			DrawTextW(hDC, buffer, length, &_rect, flags);
			rect.width = _rect.right - _rect.left;
		} else
			rect.width = 0;
		_w_toolkit_free(buffer, size);
		if (newFont != 0)
			SelectObject(hDC, oldFont);
		ReleaseDC(hwndText, hDC);
	}
	if (rect.width == 0)
		rect.width = DEFAULT_WIDTH;
	if (rect.height == 0)
		rect.height = DEFAULT_HEIGHT;
	if (wHint != W_DEFAULT)
		rect.width = wHint;
	if (hHint != W_DEFAULT)
		rect.height = hHint;
	w_event_compute_trim _e;
	w_rect trim;
	memset(&trim, 0, sizeof(w_rect));
	_e.event.type = W_EVENT_COMPUTE_TRIM;
	_e.event.platform_event = 0;
	_e.event.widget = widget;
	_e.event.time = 0;
	_e.rect = &rect;
	_e.result = &trim;
	_e.event.data = 0;
	priv->compute_trim(widget, &_e, priv);
	if (hHint == W_DEFAULT) {
		int border_width = w_control_get_border_width(W_CONTROL(widget));
		int upDownHeight = GetSystemMetrics(SM_CYVSCROLL) + 2 * border_width;
		if (WIN32_VERSION >= VERSION(6, 0)) {
			upDownHeight += (_W_WIDGET(widget)->style & W_BORDER) != 0 ? 1 : 3;
		}
		trim.height = WMAX(trim.height, upDownHeight);
	}
	e->size->width = trim.width;
	e->size->height = trim.height;
	return W_TRUE;
}
wresult _w_spinner_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {

	/* Get the trim of the text control */
	HWND hwndText = _W_SPINNER(widget)->hwndText;
	RECT rect;
	SetRect(&rect, e->rect->x, e->rect->y, e->rect->x + e->rect->width,
			e->rect->y + e->rect->height);
	int bits0 = GetWindowLongW(hwndText, GWL_STYLE);
	int bits1 = GetWindowLongW(hwndText, GWL_EXSTYLE);
	AdjustWindowRectEx(&rect, bits0, FALSE, bits1);
	e->result->width = rect.right - rect.left;
	e->result->height = rect.bottom - rect.top;

	/*
	 * The preferred height of a single-line text widget
	 * has been hand-crafted to be the same height as
	 * the single-line text widget in an editable combo
	 * box.
	 */
	LRESULT margins = SendMessageW(hwndText, EM_GETMARGINS, 0, 0);
	e->result->x -= LOWORD(margins);
	e->result->width += LOWORD (margins) + HIWORD(margins);
	if ((_W_WIDGET(widget)->style & W_BORDER) != 0) {
		e->result->x -= 1;
		e->result->y -= 1;
		e->result->width += 2;
		e->result->height += 2;
	}
	e->result->width += GetSystemMetrics(SM_CXVSCROLL);
	return W_TRUE;
}
wresult _w_spinner_copy(w_spinner *spinner) {
	HWND hwndText = _W_SPINNER(spinner)->hwndText;
	SendMessageW(hwndText, WM_COPY, 0, 0);
	return W_TRUE;
}
wresult _w_spinner_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result = _w_composite_create_handle(control, priv);
	if (result > 0) {
		HWND handle = _W_WIDGET(control)->handle;
		_W_WIDGET(control)->state &= ~(STATE_CANVAS | STATE_THEME_BACKGROUND);
		wuint64 style = _W_WIDGET(control)->style;
		DWORD textExStyle = (style & W_BORDER) != 0 ? WS_EX_CLIENTEDGE : 0;
		DWORD textStyle = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL
				| WS_CLIPSIBLINGS;
		if ((style & W_READ_ONLY) != 0)
			textStyle |= ES_READONLY;
		if (WIN32_VERSION >= VERSION(4, 10)) {
			if ((style & W_RIGHT_TO_LEFT) != 0)
				textExStyle |= WS_EX_LAYOUTRTL;
		}
		HWND hwndText = CreateWindowExW(textExStyle, WC_EDITW, NULL, textStyle,
				0, 0, 0, 0, handle, 0, hinst, NULL);
		if (hwndText != 0) {
			SetWindowLongPtrW(hwndText, GWLP_USERDATA, (LONG_PTR) control);
		} else {
			//error (W_ERROR_NO_HANDLES);
		}

		DWORD upDownStyle = WS_CHILD | WS_VISIBLE | UDS_AUTOBUDDY;
		if ((style & W_WRAP) != 0)
			upDownStyle |= UDS_WRAP;
		if ((style & W_BORDER) != 0) {
			if ((style & W_RIGHT_TO_LEFT) != 0) {
				upDownStyle |= UDS_ALIGNLEFT;
			} else {
				upDownStyle |= UDS_ALIGNRIGHT;
			}
		}
		HWND hwndUpDown = CreateWindowExW(0, UPDOWN_CLASSW, NULL, upDownStyle,
				0, 0, 0, 0, handle, 0, hinst, NULL);
		if (hwndUpDown != 0) {
			int flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
			SetWindowPos(hwndText, hwndUpDown, 0, 0, 0, 0, flags);
			SetWindowLongPtrW(hwndUpDown, GWLP_USERDATA, (LONG_PTR) control);
		} else {
			//error (W_ERROR_NO_HANDLES);
		}
		//if (IsDBLocale) {
		HIMC hIMC = ImmGetContext(handle);
		ImmAssociateContext(hwndText, hIMC);
		ImmAssociateContext(hwndUpDown, hIMC);
		ImmReleaseContext(handle, hIMC);
		//}
		SendMessageW(hwndUpDown, UDM_SETRANGE32, 0, 100);
		SendMessageW(hwndUpDown, UDM_SETPOS32, 0, 0);
		_W_SPINNER(control)->pageIncrement = 10;
		_W_SPINNER(control)->digits = 0;
		_W_SPINNER(control)->hwndText = hwndText;
		_W_SPINNER(control)->hwndUpDown = hwndUpDown;
		SetWindowTextW(hwndText, L"0");
	}
	return result;
}
wresult _w_spinner_cut(w_spinner *spinner) {
	if ((_W_WIDGET(spinner)->style & W_READ_ONLY) != 0)
		return W_FALSE;
	HWND hwndText = _W_SPINNER(spinner)->hwndText;
	SendMessageW(hwndText, WM_CUT, 0, 0);
	return W_TRUE;
}
wresult _w_spinner_get_digits(w_spinner *spinner) {
	return _W_SPINNER(spinner)->digits;
}
wresult _w_spinner_get_increment(w_spinner *spinner) {
	HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
	UDACCEL udaccel;
	SendMessageW(hwndUpDown, UDM_GETACCEL, 1, (LPARAM) &udaccel);
	return udaccel.nInc;
}
wresult _w_spinner_get_maximum(w_spinner *spinner) {
	HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
	int max = 0;
	SendMessageW(hwndUpDown, UDM_GETRANGE32, NULL, (LPARAM) (&max));
	return max;
}
wresult _w_spinner_get_minimum(w_spinner *spinner) {
	HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
	int min = 0;
	SendMessageW(hwndUpDown, UDM_GETRANGE32, (WPARAM) (&min), NULL);
	return min;
}
wresult _w_spinner_get_page_increment(w_spinner *spinner) {
	return _W_SPINNER(spinner)->pageIncrement;
}
wresult _w_spinner_get_selection(w_spinner *spinner) {
	HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
	return SendMessageW(hwndUpDown, UDM_GETPOS32, 0, 0);
}
wresult _w_spinner_get_selection_text(w_spinner *spinner, int *value) {
	HWND hwndText = _W_SPINNER(spinner)->hwndText;
	int length = GetWindowTextLengthW(hwndText);
	wresult result = W_FALSE;
	WCHAR *buffer = _w_toolkit_malloc((length + 1) * sizeof(WCHAR));
	if (buffer != 0) {
		result = W_TRUE;
		WCHAR decimalSeparator[5];
		WCHAR *tmp, *part, *_index, ch;
		GetWindowTextW(hwndText, buffer, length + 1);
		*value = 0;
		int digits = _W_SPINNER(spinner)->digits;
		if (digits > 0) {
			int _l = GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL,
					decimalSeparator, 4);
			decimalSeparator[_l] = 0;
			_index = wcsstr(buffer, decimalSeparator);
			if (_index != 0) {
				int wholeValue;
				int decimalValue;
				int index = (buffer - _index) / sizeof(WCHAR);
				int startIndex = buffer[0] == '+' || buffer[0] == '-' ? 1 : 0;
				if (startIndex != index) {
					ch = _index[0];
					_index[0] = 0;
					wholeValue = wcstol(&buffer[startIndex], &tmp, 10);
					_index[0] = ch;
				} else {
					wholeValue = 0;
				}
				if ((length - index) > digits) {
					ch = _index[digits + 1];
					_index[digits + 1] = 0;
					decimalValue = wcstol(&_index[1], &tmp, 10);
					_index[digits + 1] = ch;
				} else {
					decimalValue = wcstol(&_index[1], &tmp, 10);
				}
				for (int i = 0; i < digits; i++)
					wholeValue *= 10;
				int _value = wholeValue + decimalValue;
				if (buffer[0] == '-')
					_value = -_value;
				*value = _value;
			} else {
				int _value = wcstol(buffer, &tmp, 10);
				for (int i = 0; i < digits; i++)
					_value *= 10;
				*value = _value;
			}
		} else {
			*value = wcstol(buffer, &tmp, 10);
		}
		if (result > 0) {
			HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
			int max = 0, min = 0;
			SendMessageW(hwndUpDown, UDM_GETRANGE32, (WPARAM) &min,
					(LPARAM) &max);
			if (min <= *value && *value <= max) {
				result = W_TRUE;
			} else {
				result = W_FALSE;
			}
		}
	}
	_w_toolkit_free(buffer, (length + 1) * sizeof(WCHAR));
	return result;
}
wresult _w_spinner_get_text(w_spinner *spinner, w_alloc alloc, void *user_data,
		int enc) {
	HWND hwndText = _W_SPINNER(spinner)->hwndText;
	wresult result = W_FALSE;
	int length = GetWindowTextLengthW(hwndText);
	WCHAR *str = _w_toolkit_malloc((length + 1) * sizeof(WCHAR));
	if (str != 0) {
		GetWindowTextW(hwndText, str, length + 1);
		_win_text_set(str, length, alloc, user_data, enc);
	}
	_w_toolkit_free(str, (length + 1) * sizeof(WCHAR));
	return result;
}
wresult _w_spinner_get_text_limit(w_spinner *spinner) {
	HWND hwndText = _W_SPINNER(spinner)->hwndText;
	return SendMessageW(hwndText, EM_GETLIMITTEXT, 0, 0) & 0x7FFFFFFF;
}
wresult _w_spinner_get_values(w_spinner *spinner, w_spinner_value *value) {
	return W_FALSE;
}
wresult _w_spinner_paste(w_spinner *spinner) {
	if ((_W_WIDGET(spinner)->style & W_READ_ONLY) != 0)
		return W_FALSE;
	HWND hwndText = _W_SPINNER(spinner)->hwndText;
	SendMessageW(hwndText, WM_PASTE, 0, 0);
	return W_TRUE;
}
wresult _w_spinner_set_digits(w_spinner *spinner, int value) {
	if (value < 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (value == _W_SPINNER(spinner)->digits)
		return W_FALSE;
	_W_SPINNER(spinner)->digits = value;
	int pos;
	HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
	pos = SendMessageW(hwndUpDown, UDM_GETPOS32, 0, 0);
	return _w_spinner_set_selection_0(spinner, pos, W_FALSE, W_TRUE, W_FALSE);
}
wresult _w_spinner_set_increment(w_spinner *spinner, int value) {
	if (value < 1)
		return W_FALSE;
	HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
	int count = SendMessageW(hwndUpDown, UDM_GETACCEL, 0, (LPARAM) NULL);
	wresult result = W_FALSE;
	UDACCEL *udaccels = _w_toolkit_malloc(count * sizeof(UDACCEL));
	if (udaccels != 0) {
		SendMessageW(hwndUpDown, UDM_GETACCEL, count, (LPARAM) udaccels);
		int first = -1;
		UDACCEL *udaccel;
		for (int i = 0; i < count; i++) {
			if (first == -1)
				first = udaccels[i].nInc;
			udaccels[i].nInc = udaccels[i].nInc / first * value;
		}
		SendMessageW(hwndUpDown, UDM_SETACCEL, count, (LPARAM) udaccels);
	}
	_w_toolkit_free(udaccels, count * sizeof(UDACCEL));
	return result;
}
wresult _w_spinner_set_maximum(w_spinner *spinner, int value) {
	HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
	int min = 0;
	SendMessageW(hwndUpDown, UDM_GETRANGE32, (WPARAM) (&min), NULL);
	if (value < min)
		return W_FALSE;
	int pos;
	pos = SendMessageW(hwndUpDown, UDM_GETPOS32, 0, 0);
	SendMessageW(hwndUpDown, UDM_SETRANGE32, min, (LPARAM) value);
	wresult result = W_FALSE;
	if (pos > value)
		result = _w_spinner_set_selection_0(spinner, value, W_TRUE, W_TRUE,
				W_FALSE);
	return result;
}
wresult _w_spinner_set_minimum(w_spinner *spinner, int value) {
	HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
	int max = 0;
	SendMessageW(hwndUpDown, UDM_GETRANGE32, NULL, (LPARAM) (&max));
	if (value > max)
		return W_FALSE;
	int pos;
	pos = SendMessageW(hwndUpDown, UDM_GETPOS32, 0, 0);
	SendMessageW(hwndUpDown, UDM_SETRANGE32, value, max);
	wresult result = W_FALSE;
	if (pos < value)
		result = _w_spinner_set_selection_0(spinner, value, W_TRUE, W_TRUE,
				W_FALSE);
	return result;
}
wresult _w_spinner_set_page_increment(w_spinner *spinner, int value) {
	if (value < 1)
		return W_FALSE;
	_W_SPINNER(spinner)->pageIncrement = value;
	return W_TRUE;
}
wresult _w_spinner_set_selection(w_spinner *spinner, int value) {
	HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
	int max = 0, min = 0;
	SendMessageW(hwndUpDown, UDM_GETRANGE32, (WPARAM) &min, (LPARAM) &max);
	int _max = WMAX(min, value);
	value = WMIN(_max, max);
	return _w_spinner_set_selection_0(spinner, value, W_TRUE, W_TRUE, W_FALSE);
}
wresult _w_spinner_set_selection_0(w_spinner *spinner, int value, int setPos,
		int setText, int notify) {
	wresult result = W_FALSE;
	if (setPos) {
		HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
		SendMessageW(hwndUpDown, UDM_SETPOS32, 0, value);
		result = W_TRUE;
	}
	if (setText) {
		HWND hwndText = _W_SPINNER(spinner)->hwndText;
		int digits = _W_SPINNER(spinner)->digits;
		int size = digits + 20;
		WCHAR *buffer = _w_toolkit_malloc(size);
		if (buffer != 0) {
			_itow(value, buffer, 10);
			int length = lstrlenW(buffer);
			if (digits > 0) {
				int newlength = digits - length;
				if (newlength > 0) {
					newlength += GetLocaleInfoW(LOCALE_USER_DEFAULT,
					LOCALE_SDECIMAL, &buffer[length], 4);
					buffer[newlength] = 0;
					while (length < newlength) {
						buffer[newlength] = '0';
						length++;
					}
					length = newlength;
				} else {
					newlength += GetLocaleInfoW(LOCALE_USER_DEFAULT,
					LOCALE_SDECIMAL, &buffer[length], 4);
					buffer[newlength] = 0;
					while (length < newlength) {
						buffer[newlength] = '0';
						length++;
					}
					length = newlength;
				}
			}
			result = W_TRUE;
		}
		SetWindowTextW(hwndText, buffer);
		_w_toolkit_free(buffer, size);
		SendMessageW(hwndText, EM_SETSEL, 0, -1);
		NotifyWinEvent(EVENT_OBJECT_FOCUS, hwndText, OBJID_CLIENT, 0);
	}
	if (notify) {
		w_event e;
		e.type = W_EVENT_SELECTION;
		e.widget = W_WIDGET(spinner);
		e.platform_event = 0;
		e.time = 0;
		e.data = 0;
		_w_widget_send_event(W_WIDGET(spinner), &e);
	}
	return result;
}

wresult _w_spinner_set_text_limit(w_spinner *spinner, int limit) {
	if (limit == 0)
		return W_ERROR_CANNOT_BE_ZERO;
	HWND hwndText = _W_SPINNER(spinner)->hwndText;
	SendMessageW(hwndText, EM_SETLIMITTEXT, limit, 0);
	return W_TRUE;
}
wresult _w_spinner_set_values(w_spinner *spinner, w_spinner_value *value) {
	if (value->maximum < value->minimum)
		return W_FALSE;
	if (value->digits < 0)
		return W_FALSE;
	if (value->increment < 1)
		return W_FALSE;
	if (value->pageIncrement < 1)
		return W_FALSE;
	HWND hwndUpDown = _W_SPINNER(spinner)->hwndUpDown;
	int _max = WMAX(value->minimum, value->selection);
	int selection = WMIN(_max, value->maximum);
	_w_spinner_set_increment(spinner, value->increment);
	_W_SPINNER(spinner)->pageIncrement = value->pageIncrement;
	SendMessageW(hwndUpDown, UDM_SETRANGE32, value->minimum, value->maximum);
	return _w_spinner_set_selection_0(spinner, selection, W_TRUE, W_TRUE,
			W_FALSE);
}
wresult _w_spinner_subclass(w_control *control, _w_control_priv *priv) {
	wresult result = _w_control_subclass(control, priv);
	if (result > 0) {
		_w_spinner_priv *spriv = (_w_spinner_priv*) priv;
		if (spriv->EditProc == 0) {
			spriv->EditProc = (WNDPROC) GetWindowLongPtrW(
			_W_SPINNER(control)->hwndText,
			GWLP_WNDPROC);
		}
		SetWindowLongPtrW(_W_SPINNER(control)->hwndText, GWLP_WNDPROC,
				(LONG_PTR) _w_control_window_proc);
		if (spriv->UpDownProc == 0) {
			spriv->UpDownProc = (WNDPROC) GetWindowLongPtrW(
			_W_SPINNER(control)->hwndUpDown,
			GWLP_WNDPROC);
		}
		SetWindowLongPtrW(_W_SPINNER(control)->hwndUpDown, GWLP_WNDPROC,
				(LONG_PTR) _w_control_window_proc);
	}
	return result;
}
DWORD _w_spinner_widget_extstyle(w_control *control, _w_control_priv *priv) {
	return _w_scrollable_widget_extstyle(control, priv) & ~WS_EX_CLIENTEDGE;
}
/*
 * messages
 */

wresult _SPINNER_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	_COMPOSITE_WM_ERASEBKGND(widget, e, priv);
	//drawBackground (wParam);
	e->result = 1;
	return W_TRUE;;
}

/*wresult _SPINNER_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
 _w_control_priv *priv) {
 return W_FALSE;
 }*/

wresult _SPINNER_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HWND hwndText = _W_SPINNER(widget)->hwndText;
	SetFocus(hwndText);
	SendMessage(hwndText, EM_SETSEL, 0, -1);
	return W_FALSE;
}

wresult _SPINNER_WM_SETFONT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_SETFONT(widget, e, priv);
	if (result > 0)
		return result;
	HWND hwndText = _W_SPINNER(widget)->hwndText;
	SendMessageW(hwndText, WM_SETFONT, e->wparam, e->lparam);
	return result;
}

wresult _SPINNER_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult ret = _COMPOSITE_WM_SIZE(widget, e, priv);
	int width = LOWORD(e->lparam);
	int height = HIWORD(e->lparam);
	int upDownWidth = GetSystemMetrics(SM_CXVSCROLL);
	int textWidth = width - upDownWidth;
	int border = GetSystemMetrics(SM_CXEDGE);
	int flags = SWP_NOZORDER | SWP_DRAWFRAME | SWP_NOACTIVATE;
	SetWindowPos(_W_SPINNER(widget)->hwndText, 0, 0, 0, textWidth + border,
			height, flags);
	SetWindowPos(_W_SPINNER(widget)->hwndUpDown, 0, textWidth, 0, upDownWidth,
			height, flags);
	return ret;
}

wresult _SPINNER_IME_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {

	/* Process a DBCS character */
	win_toolkit->lastKey = 0;
	win_toolkit->lastAscii = e->wparam;
	win_toolkit->lastVirtual = win_toolkit->lastNull = win_toolkit->lastDead =
	FALSE;
	w_event_key event;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_KEYDOWN;
	event.event.widget = widget;
	event.event.platform_event = (struct w_event_platform*) e;
	event.event.time = win_toolkit->msg.time;
	if (!_w_send_key_event(&event)) {
		e->result = 0;
		return W_TRUE;
	}
	/*
	 * Feature in Windows.  The Windows text widget uses
	 * two 2 WM_CHAR's to process a DBCS key instead of
	 * using WM_IME_CHAR.  The fix is to allow the text
	 * widget to get the WM_CHAR's but ignore sending
	 * them to the application.
	 */
	_W_WIDGET(widget)->state |= STATE_SPINNER_IGNORE_CHARACTER;
	wresult result = priv->widget.call_window_proc(widget, e, priv);
	MSG msg;
	int flags = PM_REMOVE | PM_NOYIELD | PM_QS_INPUT | PM_QS_POSTMESSAGE;
	while (PeekMessageW(&msg, e->hwnd, WM_CHAR, WM_CHAR, flags)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	_W_WIDGET(widget)->state &= ~STATE_SPINNER_IGNORE_CHARACTER;
	event.event.type = W_EVENT_KEYUP;
	_w_send_key_event(&event);
	// widget could be disposed at this point
	win_toolkit->lastKey = win_toolkit->lastAscii = 0;
	return result;
}

wresult _SPINNER_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_WIDGET(widget)->state & STATE_SPINNER_IGNORE_CHARACTER)
		return W_FALSE;
	wresult result = _WIDGET_WM_CHAR(widget, e, priv);
	if (result)
		return result;
	/*
	 * Feature in Windows.  For some reason, when the
	 * widget is a single line text widget, when the
	 * user presses tab, return or escape, Windows beeps.
	 * The fix is to look for these keys and not call
	 * the window proc.
	 */
	w_event ee;
	switch (e->wparam) {
	case W_CR:
		ee.type = W_EVENT_DEFAULTSELECTION;
		ee.widget = widget;
		ee.platform_event = 0;
		ee.time = 0;
		ee.data = 0;
		_w_widget_send_event(widget, &ee);
		e->result = 0;
		return W_TRUE;
		break;
	case W_TAB:
	case W_ESC:
		e->result = 0;
		return W_TRUE;
		break;
	}
	return result;
}

wresult _SPINNER_WM_CLIPBOARD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((_W_WIDGET(widget)->style & W_READ_ONLY) != 0)
		return W_FALSE;
//	if (!hooks (SWT.Verify) && !filters (SWT.Verify)) return null;
	HWND hwndText = _W_SPINNER(widget)->hwndText;
	int call = W_FALSE;
	int length = 0;
	int start = 0, end = 0;
	WCHAR *newText = 0, *buffer = 0;
	HANDLE hMem;
	switch (e->msg) {
	case WM_CLEAR:
	case WM_CUT:
		SendMessageW(hwndText, EM_GETSEL, (WPARAM) &start, (LPARAM) &end);
		if (start != end) {
			newText = L"";
			call = W_TRUE;
		}
		break;
	case WM_PASTE:
		SendMessageW(hwndText, EM_GETSEL, (WPARAM) (&start), (LPARAM) (&end));
		if (OpenClipboard(0)) {
			HANDLE hMem = GetClipboardData(CF_UNICODETEXT);
			if (hMem != 0) {
				/* Ensure byteCount is a multiple of 2 bytes on UNICODE platforms */
				GlobalSize(hMem) / length;
				newText = GlobalLock(hMem);
			}
			break;
			case EM_UNDO:
			case WM_UNDO:
			if (SendMessageW(hwndText, EM_CANUNDO, 0, 0) != 0) {
				e->result = CallWindowProcW(_W_SPINNER_PRIV(priv)->EditProc,
						hwndText, e->msg, e->wparam, e->lparam);
				int length = GetWindowTextLengthW(hwndText);
				SendMessageW(hwndText, EM_GETSEL, (WPARAM) &start,
						(LPARAM) &end);
				if (length != 0 && start != end) {
					buffer = _w_toolkit_malloc((length + 1) * sizeof(WCHAR));
					if (buffer != 0) {
						GetWindowTextW(hwndText, buffer, length + 1);
						newText = &buffer[start];
					}
				} else {
					newText = L"";
				}
				e->result = CallWindowProcW(_W_SPINNER_PRIV(priv)->EditProc,
						hwndText, e->msg, e->wparam, e->lparam);
				SendMessageW(hwndText, EM_GETSEL, start, end);
			}
			break;
		}
	}
	if (newText != 0) {
		if (call) {
			e->result = CallWindowProcW(_W_SPINNER_PRIV(priv)->EditProc,
					hwndText, e->msg, e->wparam, e->lparam);
		}
		if (e->msg == WM_SETTEXT) {
			e->result = CallWindowProcW(_W_SPINNER_PRIV(priv)->EditProc,
					hwndText, e->msg, e->wparam, e->lparam);
		} else {
			//SendMessageW(hwndText, EM_REPLACESEL, 0, buffer);
			e->result = 0;
		}
	}
	switch (e->msg) {
	case WM_CLEAR:
	case WM_CUT:
		break;
	case WM_PASTE:
		if (hMem != 0)
			GlobalUnlock(hMem);
		CloseClipboard();
		break;
	case EM_UNDO:
	case WM_UNDO:
		_w_toolkit_free(newText, (length + 1) * sizeof(WCHAR));
		break;
	}
	return W_FALSE;
}

wresult _SPINNER_WM_COMMANDCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int code = HIWORD(e->wparam);
	int value;
	switch (code) {
	case EN_CHANGE:
		if (_w_spinner_get_selection_text(W_SPINNER(widget), &value)) {
			HWND hwndUpDown = _W_SPINNER(widget)->hwndUpDown;
			int pos;
			pos = SendMessageW(hwndUpDown, UDM_GETPOS32, 0, 0);
			if (pos != value)
				_w_spinner_set_selection_0(W_SPINNER(widget), value, W_TRUE,
						W_FALSE, W_TRUE);
		}
		/*sendEvent(SWT.Modify);
		 if (isDisposed())
		 return LRESULT.ZERO;*/
		break;
	}
	return W_FALSE;
}

wresult _SPINNER_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_WIDGET(widget)->state & STATE_SPINNER_IGNORE_CHARACTER)
		return W_FALSE;
	wresult result = _WIDGET_WM_KEYDOWN(widget, e, priv);
	if (result)
		return result;

	/* Increment the value */
	HWND hwndUpDown = _W_SPINNER(widget)->hwndUpDown;
	UDACCEL udaccel;
	SendMessageW(hwndUpDown, UDM_GETACCEL, 1, (LPARAM) &udaccel);
	int delta = 0;
	switch (e->wparam) {
	case VK_UP:
		delta = udaccel.nInc;
		break;
	case VK_DOWN:
		delta = -udaccel.nInc;
		break;
	case VK_PRIOR:
		delta = _W_SPINNER(widget)->pageIncrement;
		break;
	case VK_NEXT:
		delta = -_W_SPINNER(widget)->pageIncrement;
		break;
	}
	if (delta != 0) {
		int value;
		if (_w_spinner_get_selection_text(W_SPINNER(widget), &value)) {
			value = SendMessageW(hwndUpDown, UDM_GETPOS32, 0, 0);
		}
		int newValue = value + delta;
		int max = 0, min = 0;
		SendMessageW(hwndUpDown, UDM_GETRANGE32, (WPARAM) &min, (LPARAM) &max);
		if ((_W_WIDGET(widget)->style & W_WRAP) != 0) {
			if (newValue < min)
				newValue = max;
			if (newValue > max)
				newValue = min;
		}
		int _max = WMAX(min, newValue);
		newValue = WMIN(_max, max);
		if (value != newValue)
			_w_spinner_set_selection_0(W_SPINNER(widget), newValue, W_TRUE,
					W_TRUE, W_TRUE);
	}

	/*  Stop the edit control from moving the caret */
	switch (e->wparam) {
	case VK_UP:
	case VK_DOWN:
		e->result = 0;
		return W_TRUE;
	}
	return result;
}

wresult _SPINNER_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int value;
	if (_w_spinner_get_selection_text(W_SPINNER(widget), &value)) {
		HWND hwndUpDown = _W_SPINNER(widget)->hwndUpDown;
		value = SendMessageW(hwndUpDown, UDM_GETPOS32, 0, 0);
		_w_spinner_set_selection_0(W_SPINNER(widget), value, W_FALSE, W_TRUE,
				W_FALSE);
	}
	return _WIDGET_WM_KILLFOCUS(widget, e, priv);
}

wresult _SPINNER_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HWND hwndUpDown;
	NMUPDOWN *lpnmud = (NMUPDOWN*) e->lparam;
	switch (lpnmud->hdr.code) {
	case UDN_DELTAPOS:
		hwndUpDown = _W_SPINNER(widget)->hwndUpDown;
		int value = lpnmud->iPos + lpnmud->iDelta;
		int max = 0, min = 0;
		SendMessageW(hwndUpDown, UDM_GETRANGE32, (WPARAM) (&min),
				(LPARAM) (&max));
		if ((_W_WIDGET(widget)->style & W_WRAP) != 0) {
			if (value < min)
				value = max;
			if (value > max)
				value = min;
		}
		/*
		 * The SWT.Modify event is sent after the widget has been
		 * updated with the new state.  Rather than allowing
		 * the default updown window proc to set the value
		 * when the user clicks on the updown control, set
		 * the value explicitly and stop the window proc
		 * from running.
		 */
		int _max = WMAX(min, value);
		value = WMIN(_max, max);
		if (value != lpnmud->iPos) {
			_w_spinner_set_selection_0(W_SPINNER(widget), value, W_TRUE, W_TRUE,
					W_TRUE);
		}
		e->result = 1;
		return W_TRUE;
	}
	return _CONTROL_WM_NOTIFYCHILD(widget, e, priv);
}

wresult _SPINNER_WM_SCROLLCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int code = LOWORD(e->wparam);
	w_event ee;
	switch (code) {
	case SB_THUMBPOSITION:
		ee.type = W_EVENT_SELECTION;
		ee.widget = widget;
		ee.platform_event = 0;
		ee.time = 0;
		ee.data = 0;
		_w_widget_send_event(widget, &ee);
		break;
	}
	return W_FALSE;
}
void _w_spinner_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_spinner_class *clazz) {
	if (classId == _W_CLASS_SPINNER) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_spinner_priv;
	}
	_w_composite_class_init(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SPINNER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_spinner_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_spinner);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_spinner);
	/*
	 * public
	 */
	clazz->copy = _w_spinner_copy;
	clazz->cut = _w_spinner_cut;
	clazz->get_digits = _w_spinner_get_digits;
	clazz->get_increment = _w_spinner_get_increment;
	clazz->get_maximum = _w_spinner_get_maximum;
	clazz->get_minimum = _w_spinner_get_minimum;
	clazz->get_page_increment = _w_spinner_get_page_increment;
	clazz->get_selection = _w_spinner_get_selection;
	clazz->get_text = _w_spinner_get_text;
	clazz->get_text_limit = _w_spinner_get_text_limit;
	clazz->get_values = _w_spinner_get_values;
	clazz->paste = _w_spinner_paste;
	clazz->set_digits = _w_spinner_set_digits;
	clazz->set_increment = _w_spinner_set_increment;
	clazz->set_maximum = _w_spinner_set_maximum;
	clazz->set_minimum = _w_spinner_set_minimum;
	clazz->set_page_increment = _w_spinner_set_page_increment;
	clazz->set_selection = _w_spinner_set_selection;
	clazz->set_text_limit = _w_spinner_set_text_limit;
	clazz->set_values = _w_spinner_set_values;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_SPINNER) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->check_style = _w_spinner_check_style;
		priv->compute_size = _w_spinner_compute_size;
		priv->compute_trim = _w_spinner_compute_trim;
		priv->create_handle = _w_spinner_create_handle;
		priv->widget_extstyle = _w_spinner_widget_extstyle;
		priv->widget.call_window_proc = _w_spinner_call_window_proc;
		priv->subclass = _w_spinner_subclass;
		/*
		 * messages
		 */
		dispatch_message *msg = priv->messages;
		msg[_WM_ERASEBKGND] = _SPINNER_WM_ERASEBKGND;
		msg[_WM_KILLFOCUS] = _SPINNER_WM_KILLFOCUS;
		msg[_WM_SETFOCUS] = _SPINNER_WM_SETFOCUS;
		msg[_WM_SETFONT] = _SPINNER_WM_SETFONT;
		msg[_WM_SIZE] = _SPINNER_WM_SIZE;
		msg[_WM_IME_CHAR] = _SPINNER_IME_CHAR;
		msg[_WM_CHAR] = _SPINNER_WM_CHAR;
		//msg[_WM_CLIPBOARD] = _SPINNER_WM_CLIPBOARD;
		msg[_WM_COMMANDCHILD] = _SPINNER_WM_COMMANDCHILD;
		msg[_WM_KEYDOWN] = _SPINNER_WM_KEYDOWN;
		//msg[_WM_KILLFOCUS] = _SPINNER_WM_KILLFOCUS;
		msg[_WM_NOTIFYCHILD] = _SPINNER_WM_NOTIFYCHILD;
		msg[_WM_SCROLLCHILD] = _SPINNER_WM_SCROLLCHILD;
	}
}
