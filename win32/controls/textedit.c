/*
 * textedit.c
 *
 *  Created on: 12 mars 2021
 *      Author: Azeddine
 */
#include "textedit.h"
#include "../widgets/toolkit.h"
#ifndef EM_GETSCROLLPOS
#define EM_GETSCROLLPOS (WM_USER + 221)
#define EM_SETSCROLLPOS (WM_USER + 222)
#endif
wresult _w_textedit_apply_segments(w_textedit *text) {
	return W_TRUE;
}
wresult _w_textedit_clear_segments(w_textedit *text, int m) {
	return W_TRUE;
}
wresult _w_textedit_append(w_textedit *text, const char *string, size_t length,
		int enc) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	WCHAR *str;
	int newlength;
	_win_text_fix(string, length, enc, &str, &newlength);
	if (str != 0) {
		HWND handle = _W_WIDGET(text)->handle;
		int length = GetWindowTextLengthW(handle);
		SendMessageW(handle, EM_SETSEL, length, length);
		_w_textedit_clear_segments(text, TRUE);
		/*
		 * Feature in Windows.  When an edit control with ES_MULTILINE
		 * style that does not have the WS_VSCROLL style is full (i.e.
		 * there is no space at the end to draw any more characters),
		 * EM_REPLACESEL sends a WM_CHAR with a backspace character
		 * to remove any further text that is added.  This is an
		 * implementation detail of the edit control that is unexpected
		 * and can cause endless recursion when EM_REPLACESEL is sent
		 * from a WM_CHAR handler.  The fix is to ignore calling the
		 * handler from WM_CHAR.
		 */
		_W_WIDGET(text)->state |= STATE_TEXTEDIT_IGNORE_CHARACTER;
		SendMessageW(handle, EM_REPLACESEL, 0, (LPARAM) str);
		_W_WIDGET(text)->state &= ~STATE_TEXTEDIT_IGNORE_CHARACTER;
		SendMessageW(handle, EM_SCROLLCARET, 0, 0);
		_w_textedit_apply_segments(text);
	}
	_win_text_free(string, str, newlength);
	return W_TRUE;
}
wresult _w_textedit_clear_selection(w_textedit *text) {
	HWND handle = _W_WIDGET(text)->handle;
	SendMessageW(handle, EM_SETSEL, -1, 0);
	return W_TRUE;
}
wresult _w_textedit_copy(w_textedit *text) {
	HWND handle = _W_WIDGET(text)->handle;
	SendMessageW(handle, WM_COPY, 0, 0);
	return W_TRUE;
}
wuint64 _w_textedit_check_style(w_widget *control, wuint64 style) {
	if ((style & W_SEARCH) != 0) {
		style |= W_SINGLE | W_BORDER;
		style &= ~W_PASSWORD;
		/*
		 * NOTE: ICON_CANCEL has the same value as H_SCROLL and
		 * ICON_SEARCH has the same value as V_SCROLL so they are
		 * cleared because W_SINGLE is set.
		 */
	}
	if ((style & W_SINGLE) != 0 && (style & W_MULTI) != 0) {
		style &= ~W_MULTI;
	}
	style = _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
	if ((style & W_SINGLE) != 0)
		style &= ~(W_HSCROLL | W_VSCROLL | W_WRAP);
	if ((style & W_WRAP) != 0) {
		style |= W_MULTI;
		style &= ~W_HSCROLL;
	}
	if ((style & W_MULTI) != 0)
		style &= ~W_PASSWORD;
	if ((style & (W_SINGLE | W_MULTI)) != 0)
		return style;
	if ((style & (W_HSCROLL | W_VSCROLL)) != 0)
		return style | W_MULTI;
	return style | W_SINGLE;
}
wresult _w_textedit_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	wresult result = _w_scrollable_compute_trim(widget, e, priv);
	/*
	 * The preferred height of a single-line text widget
	 * has been hand-crafted to be the same height as
	 * the single-line text widget in an editable combo
	 * box.
	 */
	WPARAM margins = SendMessageW(_W_WIDGET(widget)->handle, EM_GETMARGINS, 0,
			0);
	e->result->x -= LOWORD(margins);
	e->result->width += LOWORD (margins) + HIWORD(margins);
	if ((_W_WIDGET(widget)->style & W_HSCROLL) != 0)
		e->result->width++;
	if ((_W_WIDGET(widget)->style & W_BORDER) != 0) {
		e->result->x -= 1;
		e->result->y -= 1;
		e->result->width += 2;
		e->result->height += 2;
	}
	return result;
}
wresult _w_textedit_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	int width = 0, height = 0;
	if (wHint == W_DEFAULT || hHint == W_DEFAULT) {
		HFONT newFont, oldFont = 0;
		HDC hDC = GetDC(_W_WIDGET(widget)->handle);
		newFont = (HFONT) SendMessageW(_W_WIDGET(widget)->handle, WM_GETFONT, 0,
				0);
		if (newFont != 0)
			oldFont = SelectObject(hDC, newFont);
		TEXTMETRICW tm;
		GetTextMetricsW(hDC, &tm);
		int count =
				(_W_WIDGET(widget)->style & W_SINGLE) != 0 ?
						1 : SendMessageW(_W_WIDGET(widget)->handle,
						EM_GETLINECOUNT, 0, 0);
		height = count * tm.tmHeight;
		RECT rect;
		memset(&rect, 0, sizeof(rect));
		int flags = DT_CALCRECT | DT_EDITCONTROL | DT_NOPREFIX;
		boolean wrap = (_W_WIDGET(widget)->style & W_MULTI) != 0
				&& (_W_WIDGET(widget)->style & W_WRAP) != 0;
		if (wrap && wHint != W_DEFAULT) {
			flags |= DT_WORDBREAK;
			rect.right = wHint;
		}
		int length = GetWindowTextLengthW(_W_WIDGET(widget)->handle);
		if (length != 0) {
			WCHAR *buffer = _w_toolkit_malloc((length + 1) * sizeof(WCHAR));
			if (buffer != 0) {
				GetWindowTextW(_W_WIDGET(widget)->handle, buffer, length + 1);
				DrawTextW(hDC, buffer, length, &rect, flags);
			}
			_w_toolkit_free(buffer, (length + 1) * sizeof(WCHAR));
			width = rect.right - rect.left;
		}
		if (wrap && hHint == W_DEFAULT) {
			int newHeight = rect.bottom - rect.top;
			if (newHeight != 0)
				height = newHeight;
		}
		if ((_W_WIDGET(widget)->style & W_SINGLE) != 0) {
			WCHAR *str;
			int l, allocated;
			allocated = _w_textedit_get_message_0(W_TEXTEDIT(widget), &str, &l);
			SetRect(&rect, 0, 0, 0, 0);
			DrawTextW(hDC, str, l, &rect, flags);
			width = WMAX(width, rect.right - rect.left);
			if (allocated > 0) {
				_w_toolkit_free(str, allocated);
			}
		}
		if (newFont != 0)
			SelectObject(hDC, oldFont);
		ReleaseDC(_W_WIDGET(widget)->handle, hDC);
	}
	if (width == 0)
		width = DEFAULT_WIDTH;
	if (height == 0)
		height = DEFAULT_HEIGHT;
	if (wHint != W_DEFAULT)
		width = wHint;
	if (hHint != W_DEFAULT)
		height = hHint;
	struct w_event_compute_trim ee;
	w_rect rect;
	w_rect result;
	ee.result = &result;
	ee.rect = &rect;
	rect.x = rect.y = 0;
	rect.width = width;
	rect.height = height;
	_w_textedit_compute_trim(widget, &ee, priv);
	e->size->width = ee.result->width;
	e->size->height = ee.result->height;
	return W_TRUE;
}
wresult _w_textedit_cut(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_READ_ONLY) != 0)
		return W_FALSE;
	SendMessageW(_W_WIDGET(text)->handle, WM_CUT, 0, 0);
	return W_TRUE;
}
wresult _w_textedit_get_caret_line_number(w_textedit *text) {
	return SendMessageW(_W_WIDGET(text)->handle, EM_LINEFROMCHAR, -1, 0);
}
wresult _w_textedit_get_caret_location(w_textedit *text, w_point *location) {
	/*
	 * Bug in Windows.  For some reason, Windows is unable
	 * to return the pixel coordinates of the last character
	 * in the widget.  The fix is to temporarily insert a
	 * space, query the coordinates and delete the space.
	 * The selection is always an i-beam in this case because
	 * this is the only time the start of the selection can
	 * be equal to the last character position in the widget.
	 * If EM_POSFROMCHAR fails for any other reason, return
	 * pixel coordinates (0,0).
	 */
	int position = _w_textedit_get_caret_position(text, W_ENCODING_PLATFORM);
	HWND handle = _W_WIDGET(text)->handle;
	DWORD caretPos = SendMessageW(handle, EM_POSFROMCHAR, position, 0);
	if (caretPos == -1) {
		caretPos = 0;
		if (position >= GetWindowTextLengthW(handle)) {
			DWORD start = 0, end = 0;
			SendMessageW(handle, EM_GETSEL, (WPARAM) &start, (LPARAM) &end);
			SendMessageW(handle, EM_SETSEL, (WPARAM) &position,
					(LPARAM) &position);
			/*
			 * Feature in Windows.  When an edit control with ES_MULTILINE
			 * style that does not have the WS_VSCROLL style is full (i.e.
			 * there is no space at the end to draw any more characters),
			 * EM_REPLACESEL sends a WM_CHAR with a backspace character
			 * to remove any further text that is added.  This is an
			 * implementation detail of the edit control that is unexpected
			 * and can cause endless recursion when EM_REPLACESEL is sent
			 * from a WM_CHAR handler.  The fix is to ignore calling the
			 * handler from WM_CHAR.
			 */
			_W_WIDGET(text)->state |= STATE_TEXTEDIT_IGNORE_CHARACTER
					| STATE_TEXTEDIT_IGNORE_MODIFY;
			SendMessageW(handle, EM_REPLACESEL, 0, (LPARAM) L" ");
			caretPos = SendMessageW(handle, EM_POSFROMCHAR, position, 0);
			SendMessageW(handle, EM_SETSEL, position, position + 1);
			SendMessageW(handle, EM_REPLACESEL, 0, (LPARAM) L" ");
			_W_WIDGET(text)->state &= ~(STATE_TEXTEDIT_IGNORE_CHARACTER
					| STATE_TEXTEDIT_IGNORE_MODIFY);
			SendMessageW(handle, EM_SETSEL, start, start);
			SendMessageW(handle, EM_SETSEL, start, end);
		}
	}
	location->x = GET_X_LPARAM(caretPos);
	location->y = GET_Y_LPARAM(caretPos);
	return W_TRUE;
}
wresult _w_textedit_get_caret_position(w_textedit *text, int enc) {
	HWND handle = _W_WIDGET(text)->handle;
	DWORD start = 0, end = 0;
	SendMessageW(handle, EM_GETSEL, (WPARAM) &start, (LPARAM) &end);
	/*
	 * In Windows, there is no API to get the position of the caret
	 * when the selection is not an i-beam.  The best that can be done
	 * is to query the pixel position of the current caret and compare
	 * it to the pixel position of the start and end of the selection.
	 *
	 * NOTE:  This does not work when the i-beam belongs to another
	 * control.  In this case, guess that the i-beam is at the start
	 * of the selection.
	 */
	int caret = start;
	if (start != end) {
		int startLine = SendMessageW(handle, EM_LINEFROMCHAR, start, 0);
		int endLine = SendMessageW(handle, EM_LINEFROMCHAR, end, 0);
		if (startLine == endLine) {
			int idThread = GetWindowThreadProcessId(handle, NULL);
			GUITHREADINFO lpgui;
			lpgui.cbSize = sizeof(GUITHREADINFO);
			if (GetGUIThreadInfo(idThread, &lpgui)) {
				if (lpgui.hwndCaret == handle || lpgui.hwndCaret == 0) {
					POINT ptCurrentPos;
					if (GetCaretPos(&ptCurrentPos)) {
						int endPos = SendMessageW(handle, EM_POSFROMCHAR, end,
								0);
						if (endPos == -1) {
							int startPos = SendMessageW(handle, EM_POSFROMCHAR,
									start, 0);
							int startX = GET_X_LPARAM(startPos);
							if (ptCurrentPos.x > startX)
								caret = end;
						} else {
							int endX = GET_X_LPARAM(endPos);
							if (ptCurrentPos.x >= endX)
								caret = end;
						}
					}
				}
			}
		} else {
			int caretPos = SendMessageW(handle, EM_LINEINDEX, -1, 0);
			int caretLine = SendMessageW(handle, EM_LINEFROMCHAR, caretPos, 0);
			if (caretLine == endLine)
				caret = end;
		}
	}
	return caret;
}
wresult _w_textedit_get_char_count(w_textedit *text, int enc) {
	int length = GetWindowTextLengthW(_W_WIDGET(text)->handle);
	return length;
}
wresult _w_textedit_get_doubleclick_enabled(w_textedit *text) {
	return (_W_WIDGET(text)->state & STATE_TEXTEDIT_DOUBLE_CLICK) != 0;
}
wresult _w_textedit_get_echo_char(w_textedit *text) {
	int echo = SendMessageW(_W_WIDGET(text)->handle, EM_GETPASSWORDCHAR, 0, 0);
	return echo;
}
wresult _w_textedit_get_editable(w_textedit *text) {
	DWORD bits = GetWindowLong(_W_WIDGET(text)->handle, GWL_STYLE);
	return (bits & ES_READONLY) == 0;
}
wresult _w_textedit_get_line_count(w_textedit *text) {
	return SendMessageW(_W_WIDGET(text)->handle, EM_GETLINECOUNT, 0, 0);
}
wresult _w_textedit_get_line_delimiter(w_textedit *text) {
	return '\n';
}
wresult _w_textedit_get_line_height(w_textedit *text) {
	HFONT newFont, oldFont = 0;
	HDC hDC = GetDC(_W_WIDGET(text)->handle);
	newFont = (HFONT) SendMessageW(_W_WIDGET(text)->handle, WM_GETFONT, 0, 0);
	if (newFont != 0)
		oldFont = SelectObject(hDC, newFont);
	TEXTMETRICW tm;
	GetTextMetricsW(hDC, &tm);
	if (newFont != 0)
		SelectObject(hDC, oldFont);
	ReleaseDC(_W_WIDGET(text)->handle, hDC);
	return tm.tmHeight;
	return W_TRUE;
}
int _w_textedit_get_message_0(w_textedit *text, WCHAR **str, int *length) {
	*str = _W_TEXTEDIT(text)->message;
	int allocated = 0;
	if (WIN32_VERSION >= VERSION(6, 0)) {
		int bits = GetWindowLong(_W_WIDGET(text)->handle, GWL_STYLE);
		if ((bits & ES_MULTILINE) == 0) {
			allocated = _W_TEXTEDIT(text)->message_length;
			*str = _w_toolkit_malloc(allocated);
			if (*str == 0)
				allocated = 0;
			SendMessageW(_W_WIDGET(text)->handle, EM_GETCUEBANNER, (WPARAM) str,
					(LPARAM) allocated);
		}
	}
	return allocated;
}
wresult _w_textedit_get_message(w_textedit *text, w_alloc alloc,
		void *user_data, int enc) {
	WCHAR *str;
	int length;
	int allocated = _w_textedit_get_message_0(text, &str, &length);
	_win_text_set(str, length, alloc, user_data, enc);
	if (allocated > 0) {
		_w_toolkit_free(str, allocated);
	}
	return W_TRUE;
}
wresult _w_textedit_get_position(w_textedit *text, w_point *point, int enc) {
	LPARAM lParam = MAKELPARAM(point->x, point->y);
	int position = LOWORD(
			SendMessageW (_W_WIDGET(text)->handle, EM_CHARFROMPOS, 0, lParam));
	return position;
}
wresult _w_textedit_get_selection(w_textedit *text, w_range *range, int enc) {
	DWORD start, end;
	SendMessageW(_W_WIDGET(text)->handle, EM_GETSEL, (WPARAM) &start,
			(LPARAM) &end);
	range->start = start;
	range->end = end;
	return W_TRUE;
}
wresult _w_textedit_get_selection_count(w_textedit *text, int enc) {
	w_range selection;
	_w_textedit_get_selection(text, &selection, enc);
	return abs(selection.end - selection.start);
}
wresult _w_textedit_get_selection_text(w_textedit *text, w_alloc alloc,
		void *user_data, int enc) {
	int length = GetWindowTextLengthW(_W_WIDGET(text)->handle);
	DWORD start, end;
	SendMessageW(_W_WIDGET(text)->handle, EM_GETSEL, (WPARAM) &start,
			(LPARAM) &end);
	WCHAR *str = _w_toolkit_malloc((length + 1) * sizeof(WCHAR));
	if (str != 0) {
		GetWindowTextW(_W_WIDGET(text)->handle, str, length + 1);
		_win_text_set(str, length, alloc, user_data, enc);
	}
	_w_toolkit_free(str, (length + 1) * sizeof(WCHAR));
	return W_TRUE;
}
int _w_textedit_get_tab_width(w_textedit *text, int tabs) {
	HFONT oldFont = 0;
	RECT rect;
	HDC hDC = GetDC(_W_WIDGET(text)->handle);
	HFONT newFont = (HFONT) SendMessageW(_W_WIDGET(text)->handle, WM_GETFONT, 0,
			0);
	if (newFont != 0)
		oldFont = SelectObject(hDC, newFont);
	int flags = DT_CALCRECT | DT_SINGLELINE | DT_NOPREFIX;
	DrawTextW(hDC, L" ", 1, &rect, flags);
	if (newFont != 0)
		SelectObject(hDC, oldFont);
	ReleaseDC(_W_WIDGET(text)->handle, hDC);
	return (rect.right - rect.left) * tabs;
}
wresult _w_textedit_get_tabs(w_textedit *text) {
	return _W_TEXTEDIT(text)->tabs;
}
wresult _w_textedit_get_text(w_textedit *text, w_alloc alloc, void *user_data,
		int enc) {
	wresult result = W_FALSE;
	HWND handle = _W_WIDGET(text)->handle;
	int length = GetWindowTextLengthW(handle);
	WCHAR *str = _w_toolkit_malloc((length + 1) * sizeof(WCHAR));
	if (str != 0) {
		GetWindowTextW(handle, str, length + 1);
		_win_text_set(str, length, alloc, user_data, enc);
	}
	_w_toolkit_free(str, (length + 1) * sizeof(WCHAR));
	return result;
}
wresult _w_textedit_get_text_range(w_textedit *text, w_range *range,
		w_alloc alloc, void *user_data, int enc) {
	int start = range->start;
	int end = range->end;
	wresult result = W_FALSE;
	HWND handle = _W_WIDGET(text)->handle;
	int length = GetWindowTextLengthW(handle);
	end = WMIN(end, length - 1);
	start = WMAX(0, start);
	WCHAR *str = _w_toolkit_malloc((end + 1) * sizeof(WCHAR));
	if (str != 0) {
		GetWindowTextW(handle, str, end + 1);
		_win_text_set(&str[start], (end - start), alloc, user_data, enc);
	}
	_w_toolkit_free(str, (end + 1) * sizeof(WCHAR));
	return result;
}
wresult _w_textedit_get_text_limit(w_textedit *text, int enc) {
	int limit = SendMessageW(_W_WIDGET(text)->handle, EM_GETLIMITTEXT, 0, 0)
			& 0x7FFFFFFF;
	return limit;
}
wresult _w_textedit_get_top_index(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0)
		return 0;
	return SendMessageW(_W_WIDGET(text)->handle, EM_GETFIRSTVISIBLELINE, 0, 0);
}
wresult _w_textedit_get_top_pixel(w_textedit *text) {
	/*
	 * Note, EM_GETSCROLLPOS is implemented in Rich Edit 3.0
	 * and greater.  The plain text widget and previous versions
	 * of Rich Edit return zero.
	 */
	POINT buffer;
	LRESULT code = SendMessageW(_W_WIDGET(text)->handle, EM_GETSCROLLPOS, 0,
			(LPARAM) &buffer);
	if (code == 1)
		return buffer.x;
	int topindex = _w_textedit_get_top_index(text);
	int lineHeight = _w_textedit_get_line_height(text);
	return topindex * lineHeight;
}
wresult _w_textedit_insert(w_textedit *text, const char *string, size_t length,
		int enc) {
	_w_textedit_clear_segments(text, W_TRUE);
	WCHAR *str;
	int newlength;
	_win_text_fix(string, length, enc, &str, &newlength);
	if (str != 0) {
		/*
		 * Feature in Windows.  When an edit control with ES_MULTILINE
		 * style that does not have the WS_VSCROLL style is full (i.e.
		 * there is no space at the end to draw any more characters),
		 * EM_REPLACESEL sends a WM_CHAR with a backspace character
		 * to remove any further text that is added.  This is an
		 * implementation detail of the edit control that is unexpected
		 * and can cause endless recursion when EM_REPLACESEL is sent
		 * from a WM_CHAR handler.  The fix is to ignore calling the
		 * handler from WM_CHAR.
		 */
		_W_WIDGET(text)->state |= STATE_TEXTEDIT_IGNORE_CHARACTER;
		SendMessageW(_W_WIDGET(text)->handle, EM_REPLACESEL, 0, (LPARAM) str);
		_W_WIDGET(text)->state &= ~STATE_TEXTEDIT_IGNORE_CHARACTER;
	}
	_win_text_free(string, str, newlength);
	_w_textedit_apply_segments(text);
	return W_TRUE;
}
wresult _w_textedit_paste(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_READ_ONLY) != 0)
		return W_FALSE;
	SendMessageW(_W_WIDGET(text)->handle, WM_PASTE, 0, 0);
	return W_TRUE;
}
wresult _w_textedit_select_all(w_textedit *text) {
	SendMessageW(_W_WIDGET(text)->handle, EM_SETSEL, 0, -1);
	return W_TRUE;
}
wresult _w_textedit_set_doubleclick_enabled(w_textedit *text, int doubleClick) {
	if (doubleClick) {
		_W_WIDGET(text)->state |= STATE_TEXTEDIT_DOUBLE_CLICK;
	} else {
		_W_WIDGET(text)->state &= ~STATE_TEXTEDIT_DOUBLE_CLICK;
	}
	return W_TRUE;
}
wresult _w_textedit_set_echochar(w_textedit *text, int echo) {
	_W_WIDGET(text)->state |= STATE_TEXTEDIT_ALLOW_PASSWORD_CHAR;
	SendMessageW(_W_WIDGET(text)->handle, EM_SETPASSWORDCHAR, echo, 0);
	_W_WIDGET(text)->state &= ~STATE_TEXTEDIT_ALLOW_PASSWORD_CHAR;
	/*
	 * Bug in Windows.  When the password character is changed,
	 * Windows does not redraw to show the new password character.
	 * The fix is to force a redraw when the character is set.
	 */
	InvalidateRect(_W_WIDGET(text)->handle, 0, TRUE);
	return W_TRUE;
}
wresult _w_textedit_set_editable(w_textedit *text, int editable) {
	_W_WIDGET(text)->style &= ~W_READ_ONLY;
	if (!editable)
		_W_WIDGET(text)->style |= W_READ_ONLY;
	SendMessageW(_W_WIDGET(text)->handle, EM_SETREADONLY, editable ? 0 : 1, 0);
	return W_TRUE;
}
wresult _w_textedit_set_message(w_textedit *text, const char *message,
		size_t length, int enc) {
	int newlength;
	int copy = W_TRUE;
	WCHAR *str;
	_win_text_fix(message, length, enc, &str, &newlength);
	if (WIN32_VERSION >= VERSION(6, 0)) {
		int bits = GetWindowLong(_W_WIDGET(text)->handle, GWL_STYLE);
		if ((bits & ES_MULTILINE) == 0) {
			SendMessageW(_W_WIDGET(text)->handle, EM_SETCUEBANNER, 0,
					(LPARAM) str);
			copy = W_FALSE;
		}
	} else {
		InvalidateRect(_W_WIDGET(text)->handle, 0, TRUE);
	}
	if (copy) {
		_W_TEXTEDIT(text)->message = wcsdup(str);
	}
	_W_TEXTEDIT(text)->message_length = newlength;
	_win_text_free(message, str, newlength);
	return W_TRUE;
}
wresult _w_textedit_set_selection(w_textedit *text, int start, int enc) {
	SendMessageW(_W_WIDGET(text)->handle, EM_SETSEL, start, start);
	SendMessageW(_W_WIDGET(text)->handle, EM_SCROLLCARET, 0, 0);
	return W_TRUE;
}
wresult _w_textedit_set_selection_range(w_textedit *text, w_range *selection,
		int enc) {
	SendMessageW(_W_WIDGET(text)->handle, EM_SETSEL, selection->start,
			selection->end);
	SendMessageW(_W_WIDGET(text)->handle, EM_SCROLLCARET, 0, 0);
	return W_TRUE;
}
void _w_textedit_set_tab_stops(w_textedit *text, int tabs) {
	/*
	 * Feature in Windows.  Windows expects the tab spacing in
	 * dialog units so we must convert from space widths.  Due
	 * to round off error, the tab spacing may not be the exact
	 * number of space widths, depending on the font.
	 */
	DWORD width = (_w_textedit_get_tab_width(text, tabs) * 4)
			/ LOWORD(GetDialogBaseUnits());
	SendMessageW(_W_WIDGET(text)->handle, EM_SETTABSTOPS, 1, (LPARAM) &width);
}
wresult _w_textedit_set_tabs(w_textedit *text, int tabs) {
	if (tabs < 0)
		return W_FALSE;
	_w_textedit_set_tab_stops(text, tabs);
	return W_TRUE;
}
wresult _w_textedit_set_text(w_textedit *text, const char *string,
		size_t length, int enc) {
	WCHAR *str;
	int newlength;
	_win_text_fix(string, length, enc, &str, &newlength);
	if (str != 0) {
		HWND handle = _W_WIDGET(text)->handle;
		_w_textedit_clear_segments(text, TRUE);
		SetWindowTextW(handle, str);
		_w_textedit_apply_segments(text);
	}
	_win_text_free(string, str, newlength);
	return W_TRUE;
}
wresult _w_textedit_set_text_limit(w_textedit *text, int limit) {
	if (limit == 0)
		return W_ERROR_CANNOT_BE_ZERO;
	SendMessageW(_W_WIDGET(text)->handle, EM_SETLIMITTEXT, limit, 0);
	return W_TRUE;
}
wresult _w_textedit_set_top_index(w_textedit *text, int index) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0)
		return W_TRUE;
	int count = SendMessageW(_W_WIDGET(text)->handle, EM_GETLINECOUNT, 0, 0);
	int _index = WMAX(index, 0);
	index = WMIN(_index, count - 1);
	int topIndex = SendMessageW(_W_WIDGET(text)->handle, EM_GETFIRSTVISIBLELINE,
			0, 0);
	SendMessageW(_W_WIDGET(text)->handle, EM_LINESCROLL, 0, index - topIndex);
	return W_TRUE;
}
wresult _w_textedit_show_selection(w_textedit *text) {
	SendMessageW(_W_WIDGET(text)->handle, EM_SCROLLCARET, 0, 0);
	return W_TRUE;
}
DWORD _w_textedit_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_scrollable_widget_style(control, priv) | ES_AUTOHSCROLL;
	bits &= ~ WS_BORDER;
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_PASSWORD) != 0)
		bits |= ES_PASSWORD;
	if ((style & W_CENTER) != 0)
		bits |= ES_CENTER;
	if ((style & W_RIGHT) != 0)
		bits |= ES_RIGHT;
	if ((style & W_READ_ONLY) != 0)
		bits |= ES_READONLY;
	if ((style & W_SINGLE) != 0) {
		/*
		 * Feature in Windows.  When a text control is read-only,
		 * uses COLOR_3DFACE for the background .  If the text
		 * controls single-line and is within a tab folder or
		 * some other themed control, using WM_ERASEBKGND and
		 * WM_CTRCOLOR to draw the theme background results in
		 * pixel corruption.  The fix is to use an ES_MULTILINE
		 * text control instead.
		 * Refer Bug438901:- ES_MULTILINE doesn't apply for:
		 * W_PASSWORD | W_READ_ONLY style combination.
		 */
		if ((style & W_READ_ONLY) != 0) {
			if ((style & (W_BORDER | W_HSCROLL | W_VSCROLL | W_PASSWORD))
					== 0) {
				if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
					bits |= ES_MULTILINE;
				}
			}
		}
		return bits;
	}
	bits |= ES_MULTILINE | ES_NOHIDESEL | ES_AUTOVSCROLL;
	if ((style & W_WRAP) != 0)
		bits &= ~(WS_HSCROLL | ES_AUTOHSCROLL);
	return bits;
}
DWORD _w_textedit_widget_extstyle(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_scrollable_widget_extstyle(control, priv);
	wuint64 style = _W_WIDGET(control)->style;
	if (style & W_BORDER) {
		bits |= WS_EX_CLIENTEDGE;
	} else {
		bits &= ~ WS_EX_CLIENTEDGE;
	}
	return bits;
}
WCHAR* _w_textedit_window_class(w_control *control,
		_w_control_priv *priv) {
	return WC_EDITW;
}
void _w_textedit_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_textedit_class *clazz) {
	if (classId == _W_CLASS_TEXTEDIT) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_textedit_priv;
	}
	_w_scrollable_class_init(toolkit, classId, W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TEXTEDIT;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_textedit_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_textedit);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_textedit);
	/*
	 * public function
	 */
	clazz->append = _w_textedit_append;
	clazz->clear_selection = _w_textedit_clear_selection;
	clazz->copy = _w_textedit_copy;
	clazz->cut = _w_textedit_cut;
	clazz->get_caret_line_number = _w_textedit_get_caret_line_number;
	clazz->get_caret_location = _w_textedit_get_caret_location;
	clazz->get_caret_position = _w_textedit_get_caret_position;
	clazz->get_char_count = _w_textedit_get_char_count;
	clazz->get_doubleclick_enabled = _w_textedit_get_doubleclick_enabled;
	clazz->get_echo_char = _w_textedit_get_echo_char;
	clazz->get_editable = _w_textedit_get_editable;
	clazz->get_line_count = _w_textedit_get_line_count;
	clazz->get_line_delimiter = _w_textedit_get_line_delimiter;
	clazz->get_line_height = _w_textedit_get_line_height;
	clazz->get_message = _w_textedit_get_message;
	clazz->get_position = _w_textedit_get_position;
	clazz->get_selection = _w_textedit_get_selection;
	clazz->get_selection_count = _w_textedit_get_selection_count;
	clazz->get_selection_text = _w_textedit_get_selection_text;
	clazz->get_tabs = _w_textedit_get_tabs;
	clazz->get_text = _w_textedit_get_text;
	clazz->get_text_range = _w_textedit_get_text_range;
	clazz->get_text_limit = _w_textedit_get_text_limit;
	clazz->get_top_index = _w_textedit_get_top_index;
	clazz->get_top_pixel = _w_textedit_get_top_pixel;
	clazz->insert = _w_textedit_insert;
	clazz->paste = _w_textedit_paste;
	clazz->select_all = _w_textedit_select_all;
	clazz->set_doubleclick_enabled = _w_textedit_set_doubleclick_enabled;
	clazz->set_echochar = _w_textedit_set_echochar;
	clazz->set_editable = _w_textedit_set_editable;
	clazz->set_message = _w_textedit_set_message;
	clazz->set_selection = _w_textedit_set_selection;
	clazz->set_selection_range = _w_textedit_set_selection_range;
	clazz->set_tabs = _w_textedit_set_tabs;
	clazz->set_text = _w_textedit_set_text;
	clazz->set_text_limit = _w_textedit_set_text_limit;
	clazz->set_top_index = _w_textedit_set_top_index;
	clazz->show_selection = _w_textedit_show_selection;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_TEXTEDIT) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->check_style = _w_textedit_check_style;
		priv->compute_size = _w_textedit_compute_size;
		priv->compute_trim = _w_textedit_compute_trim;
		priv->window_class = _w_textedit_window_class;
		priv->widget_extstyle = _w_textedit_widget_extstyle;
		priv->widget_style = _w_textedit_widget_style;
	}
}

