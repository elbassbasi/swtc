/*
 * datetime.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "datetime.h"
#include "../widgets/toolkit.h"
#ifndef DTM_GETIDEALSIZE
#define DTM_GETIDEALSIZE (DTM_FIRST+15)
#endif
#define  MARGIN 4
#define  MIN_YEAR 1752 // Gregorian switchover in North America: September 19, 1752
#define  MAX_YEAR 9999
wuint64 _w_datetime_check_style(w_widget *widget, wuint64 style) {
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	style &= ~(W_HSCROLL | W_VSCROLL);
	style = _w_widget_check_bits(style, W_DATE, W_TIME, W_CALENDAR, 0, 0, 0);
	style = _w_widget_check_bits(style, W_MEDIUM, W_SHORT, W_LONG, 0, 0, 0);
	if ((style & W_DATE) == 0)
		style &= ~W_DROP_DOWN;
	return style;
}
wresult _w_datetime_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	HWND handle = _W_WIDGET(widget)->handle;
	wuint64 style = _W_WIDGET(widget)->style;
	int wHint = e->wHint;
	int hHint = e->hHint;
	int width = 0, height = 0;
	if (wHint == W_DEFAULT || hHint == W_DEFAULT) {
		if ((style & W_CALENDAR) != 0) {
			RECT rect;
			SendMessageW(handle, MCM_GETMINREQRECT, 0, (LPARAM) &rect);
			width = rect.right;
			height = rect.bottom;
		} else {
			if (WIN32_VERSION >= VERSION(6, 0)) {
				/* Vista and later: use DTM_GETIDEALSIZE. */
				SIZE size;
				SendMessageW(handle, DTM_GETIDEALSIZE, 0, (LPARAM) &size);
				width = size.cx;
				height = size.cy;
			} else {
				HFONT newFont, oldFont = 0;
				HDC hDC = GetDC(handle);
				newFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
				if (newFont != 0)
					oldFont = SelectObject(hDC, newFont);
				RECT rect;
				WCHAR lpFormat[80];
				int size = 0;
				if ((style & W_DATE) != 0) {
					int dwFlags = 0;
					if ((style & W_SHORT) != 0) {
						size = GetLocaleInfoW(LOCALE_USER_DEFAULT,
						LOCALE_SYEARMONTH, lpFormat, 80);
						if (size == 0) {
							lstrcpyW(lpFormat, L"M/yyyy");
						}
					} else {
						dwFlags = (style & W_MEDIUM) != 0 ?
						DATE_SHORTDATE :
															DATE_LONGDATE;
					}
					int size = GetDateFormatW(LOCALE_USER_DEFAULT, dwFlags,
					NULL, lpFormat, NULL, 0);
					if (size > 0) {
						WCHAR *str = _w_toolkit_malloc(size * sizeof(WCHAR));
						if (str != 0) {
							GetDateFormatW(LOCALE_USER_DEFAULT, dwFlags, NULL,
									lpFormat, str, size);
							DrawTextW(hDC, str, size, &rect,
							DT_CALCRECT | DT_EDITCONTROL);
						}
						_w_toolkit_free(str, size * sizeof(WCHAR));
					}
				} else if ((style & W_TIME) != 0) {
					int dwFlags = 0;
					if ((style & W_SHORT) != 0) {
						dwFlags = TIME_NOSECONDS;
						size = _w_datetime_get_custom_short_time_format(
								lpFormat, 80);
					}
					int size = GetTimeFormatW(LOCALE_USER_DEFAULT, dwFlags,
					NULL, lpFormat, NULL, 0);
					if (size > 0) {
						WCHAR *str = _w_toolkit_malloc(size * sizeof(WCHAR));
						if (str != 0) {
							GetDateFormatW(LOCALE_USER_DEFAULT, dwFlags, NULL,
									lpFormat, str, size);
							DrawTextW(hDC, str, size, &rect,
							DT_CALCRECT | DT_EDITCONTROL);
						}
						_w_toolkit_free(str, size * sizeof(WCHAR));
					}
				}
				width = rect.right - rect.left;
				height = rect.bottom - rect.top;
				if (newFont != 0)
					SelectObject(hDC, oldFont);
				ReleaseDC(handle, hDC);
				int upDownWidth = GetSystemMetrics(SM_CXVSCROLL);
				width += upDownWidth + MARGIN;
			}
			int upDownHeight = GetSystemMetrics(SM_CYVSCROLL);
			if (WIN32_VERSION >= VERSION(6, 0)) {
				upDownHeight += 7;
			}
			height = WMAX(height, upDownHeight);
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
	int border = w_control_get_border_width(W_CONTROL(widget));
	width += border * 2;
	height += border * 2;
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
}
wresult _w_datetime_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result = _w_composite_create_handle(control, priv);
	if (result > 0) {
		_W_WIDGET(control)->state &= ~(STATE_CANVAS | STATE_THEME_BACKGROUND);
		HWND handle = _W_WIDGET(control)->handle;
		wuint64 style = _W_WIDGET(control)->style;
		if ((style & W_BORDER) == 0) {
			DWORD bits = GetWindowLongW(handle, GWL_EXSTYLE);
			bits &= ~(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
			SetWindowLongW(handle, GWL_EXSTYLE, bits);
		}
		if ((style & W_SHORT) != 0) {
			WCHAR lpFormat[80];
			int size = 0;
			if ((style & W_DATE) != 0) {
				size = GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_SYEARMONTH,
						lpFormat, 80);
				if (size == 0) {
					lstrcpyW(lpFormat, L"M/yyyy");
				}
			} else {
				size = _w_datetime_get_custom_short_time_format(lpFormat, 80);
			}
			SendMessageW(handle, DTM_SETFORMATW, 0, (LPARAM) lpFormat);
		}
	}
	return result;
}
int _w_datetime_get_custom_short_time_format(WCHAR *lpFormat, int _length) {
	int size = GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, lpFormat,
			_length);
	if (size == 0) {
		lstrcpyW(lpFormat, L"h:mm:ss tt");
	}
	int length = size;
	int inQuotes = FALSE;
	int start = 0, end = 0;
	while (start < length) {
		WCHAR ch = lpFormat[start];
		if (ch == '\'')
			inQuotes = !inQuotes;
		else if (ch == 's' && !inQuotes) {
			end = start + 1;
			while (end < length && lpFormat[end] == 's')
				end++;
			// skip the preceding separator
			while (start > 0 && lpFormat[start] != 'm')
				start--;
			start++;
			break;
		}
		start++;
	}
	if (start < end) {
		memcpy(&lpFormat[start], &lpFormat[end],
				(length - end) * sizeof(WCHAR));
		return length - end;
	}
	return length;
}
wresult _w_datetime_get_day(w_datetime *datetime) {
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	return systime.wDay;
}
WNDPROC* _w_datetime_get_def_window_proc(w_control *control,
		_w_control_priv *priv) {
	if (_W_WIDGET(control)->style & W_CALENDAR) {
		return &_W_DATETIME_PRIV(priv)->calendarProc;
	} else {
		return &priv->def_window_proc;
	}
}
wresult _w_datetime_get_hours(w_datetime *datetime) {
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	if ((style & W_CALENDAR) != 0)
		return _W_DATETIME(datetime)->wHour;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	return systime.wHour;
}
wresult _w_datetime_get_minutes(w_datetime *datetime) {
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	if ((style & W_CALENDAR) != 0)
		return _W_DATETIME(datetime)->wMinute;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	return systime.wMinute;
}
wresult _w_datetime_get_month(w_datetime *datetime) {
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	return systime.wMonth - 1;
}
wresult _w_datetime_get_seconds(w_datetime *datetime) {
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	if ((style & W_CALENDAR) != 0)
		return _W_DATETIME(datetime)->wSecond;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	return systime.wSecond;
}
wresult _w_datetime_get_year(w_datetime *datetime) {
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	return systime.wYear;
}
wresult _w_datetime_set_date(w_datetime *datetime, int year, int month,
		int day) {
	if (year < MIN_YEAR || year > MAX_YEAR)
		return W_FALSE;
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	msg = (style & W_CALENDAR) != 0 ? MCM_SETCURSEL : DTM_SETSYSTEMTIME;
	systime.wYear = year;
	systime.wMonth = (month + 1);
	systime.wDay = day;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	_W_WIDGET(datetime)->state &= ~STATE_DATETIME_LASTSYSTEMTIME;
	return W_TRUE;
}
wresult _w_datetime_set_day(w_datetime *datetime, int day) {
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	msg = (style & W_CALENDAR) != 0 ? MCM_SETCURSEL : DTM_SETSYSTEMTIME;
	systime.wDay = day;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	_W_WIDGET(datetime)->state &= ~STATE_DATETIME_LASTSYSTEMTIME;
	return W_TRUE;
}
wresult _w_datetime_set_hours(w_datetime *datetime, int hours) {
	if (hours < 0 || hours > 23)
		return W_FALSE;
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	msg = (style & W_CALENDAR) != 0 ? MCM_SETCURSEL : DTM_SETSYSTEMTIME;
	systime.wHour = hours;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	if ((style & W_CALENDAR) != 0 && hours >= 0 && hours <= 23)
		_W_DATETIME(datetime)->wHour = hours;
	return W_TRUE;
}
wresult _w_datetime_set_minutes(w_datetime *datetime, int minutes) {
	if (minutes < 0 || minutes > 59)
		return W_FALSE;
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	msg = (style & W_CALENDAR) != 0 ? MCM_SETCURSEL : DTM_SETSYSTEMTIME;
	systime.wMinute = minutes;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	if ((style & W_CALENDAR) != 0 && minutes >= 0 && minutes <= 59)
		_W_DATETIME(datetime)->wMinute = minutes;
	return W_TRUE;
}
wresult _w_datetime_set_month(w_datetime *datetime, int month) {
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	msg = (style & W_CALENDAR) != 0 ? MCM_SETCURSEL : DTM_SETSYSTEMTIME;
	systime.wMonth = (month + 1);
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	_W_WIDGET(datetime)->state &= ~STATE_DATETIME_LASTSYSTEMTIME;
	return W_TRUE;
}
wresult _w_datetime_set_seconds(w_datetime *datetime, int seconds) {
	if (seconds < 0 || seconds > 59)
		return W_FALSE;
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	msg = (style & W_CALENDAR) != 0 ? MCM_SETCURSEL : DTM_SETSYSTEMTIME;
	systime.wSecond = seconds;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	if ((style & W_CALENDAR) != 0 && seconds >= 0 && seconds <= 59)
		_W_DATETIME(datetime)->wSecond = seconds;
	return W_TRUE;
}
wresult _w_datetime_set_time(w_datetime *datetime, int hours, int minutes,
		int seconds) {
	if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0
			|| seconds > 59)
		return W_FALSE;
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	msg = (style & W_CALENDAR) != 0 ? MCM_SETCURSEL : DTM_SETSYSTEMTIME;
	systime.wHour = hours;
	systime.wMinute = minutes;
	systime.wSecond = seconds;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	if ((style & W_CALENDAR) != 0 && hours >= 0 && hours <= 23 && minutes >= 0
			&& minutes <= 59 && seconds >= 0 && seconds <= 59) {
		_W_DATETIME(datetime)->wHour = hours;
		_W_DATETIME(datetime)->wMinute = minutes;
		_W_DATETIME(datetime)->wSecond = seconds;
	}
	return W_TRUE;
}
wresult _w_datetime_set_year(w_datetime *datetime, int year) {
	if (year < MIN_YEAR || year > MAX_YEAR)
		return W_FALSE;
	HWND handle = _W_WIDGET(datetime)->handle;
	wuint64 style = _W_WIDGET(datetime)->style;
	SYSTEMTIME systime;
	int msg = (style & W_CALENDAR) != 0 ? MCM_GETCURSEL : DTM_GETSYSTEMTIME;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	msg = (style & W_CALENDAR) != 0 ? MCM_SETCURSEL : DTM_SETSYSTEMTIME;
	systime.wYear = year;
	SendMessageW(handle, msg, 0, (LPARAM) &systime);
	_W_WIDGET(datetime)->state &= ~STATE_DATETIME_LASTSYSTEMTIME;
	return W_TRUE;
}
DWORD _w_datetime_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_composite_widget_style(control, priv) | WS_TABSTOP;
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_CALENDAR) != 0)
		return bits | MCS_NOTODAY;
	/*
	 * Bug in Windows: When WS_CLIPCHILDREN is set in a
	 * Date and Time Picker, the widget draws on top of
	 * the updown control. The fix is to clear the bits.
	 */
	bits &= ~WS_CLIPCHILDREN;
	if ((style & W_TIME) != 0)
		bits |= DTS_TIMEFORMAT;
	if ((style & W_DATE) != 0) {
		bits |= ((style & W_MEDIUM) != 0 ?
		DTS_SHORTDATECENTURYFORMAT :
											DTS_LONGDATEFORMAT);
		if ((style & W_DROP_DOWN) == 0)
			bits |= DTS_UPDOWN;
	}
	return bits;
}
WCHAR* _w_datetime_window_class(w_control *control,
		_w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	return (style & W_CALENDAR) != 0 ? MONTHCAL_CLASSW : DATETIMEPICK_CLASSW;
}
/*
 * messages
 */
wresult _DATETIME_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	NMHDR *hdr = (NMHDR*) e->lparam;
	SYSTEMTIME systime, *lastSystemTime;
	w_event event;
	switch (hdr->code) {
	case DTN_CLOSEUP: {
		/*
		 * Feature in Windows.  When the user selects the drop-down button,
		 * the DateTimePicker runs a modal loop and consumes WM_LBUTTONUP.
		 * This is done without adding a mouse capture.  Since WM_LBUTTONUP
		 * is not delivered, the normal mechanism where a mouse capture is
		 * added on mouse down and removed when the mouse is released
		 * is broken, leaving an unwanted capture.  The fix is to avoid
		 * setting capture on mouse down right after WM_LBUTTONUP is consumed.
		 */
		win_toolkit->captureChanged = TRUE;
		break;
	}
	case MCN_SELCHANGE: {
		if (_W_WIDGET(widget)->state & STATE_DATETIME_IGNORESELECTION)
			break;
		SendMessageW(e->hwnd, MCM_GETCURSEL, 0, (LPARAM) &systime);
		event.type = W_EVENT_SELECTION;
		event.widget = widget;
		event.platform_event = _EVENT_PLATFORM(e);
		event.time = 0;
		event.data = 0;
		_w_widget_post_event(widget, &event, W_EVENT_SEND);
		break;
	}
	case DTN_DATETIMECHANGE: {
		SendMessageW(e->hwnd, DTM_GETSYSTEMTIME, 0, (LPARAM) &systime);
		lastSystemTime = &_W_DATETIME(widget)->lastSystemTime;
		if ((_W_WIDGET(widget)->state & STATE_DATETIME_LASTSYSTEMTIME) == 0
				|| systime.wDay != lastSystemTime->wDay
				|| systime.wMonth != lastSystemTime->wMonth
				|| systime.wYear != lastSystemTime->wYear) {
			event.type = W_EVENT_SELECTION;
			event.widget = widget;
			event.platform_event = _EVENT_PLATFORM(e);
			event.time = 0;
			event.data = 0;
			_w_widget_post_event(widget, &event, W_EVENT_SEND);
			if ((_W_WIDGET(widget)->style & W_TIME) == 0) {
				memcpy(&_W_DATETIME(widget)->lastSystemTime, &systime,
						sizeof(SYSTEMTIME));
				_W_WIDGET(widget)->state |= STATE_DATETIME_LASTSYSTEMTIME;
			}
		}
		break;
	}
	}
	return W_FALSE;
}
wresult _DATETIME_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_CHAR(widget, e, priv);
	if (result)
		return result;
	w_event event;
	/*
	 * Feature in Windows.  For some reason, when the
	 * user presses tab, return or escape, Windows beeps.
	 * The fix is to look for these keys and not call
	 * the window proc.
	 */
	switch (e->wparam) {
	case W_CR:
		event.type = W_EVENT_DEFAULTSELECTION;
		event.widget = widget;
		event.platform_event = _EVENT_PLATFORM(e);
		event.time = 0;
		event.data = 0;
		_w_widget_post_event(widget, &event, W_EVENT_SEND);
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
wresult _DATETIME_WM_LBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_LBUTTONDBLCLK(widget, e, priv);
	//if (isDisposed ()) return LRESULT.ZERO;
	if ((_W_WIDGET(widget)->style & W_CALENDAR) != 0) {
		MCHITTESTINFO pMCHitTest;
		pMCHitTest.cbSize = sizeof(pMCHitTest);
		POINT pt;
		pMCHitTest.pt.x = GET_X_LPARAM(e->lparam);
		pMCHitTest.pt.y = GET_Y_LPARAM(e->lparam);
		LRESULT code = SendMessageW(e->hwnd, MCM_HITTEST, 0,
				(LPARAM) &pMCHitTest);
		if ((code & MCHT_CALENDARDATE) == MCHT_CALENDARDATE)
			_W_WIDGET(widget)->state |= STATE_DATETIME_DOUBLECLICK;
	}
	return result;
}
wresult _DATETIME_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_LBUTTONDOWN(widget, e, priv);
	if (result && e->result == 0)
		return result;
	_W_WIDGET(widget)->state &= ~STATE_DATETIME_DOUBLECLICK;
	wuint64 style = _W_WIDGET(widget)->style;
	/*
	 * Feature in Windows. For some reason, the calendar control
	 * does not take focus on WM_LBUTTONDOWN.  The fix is to
	 * explicitly set focus.
	 */
	if ((style & W_CALENDAR) != 0) {
		if ((style & W_NO_FOCUS) == 0)
			SetFocus(e->hwnd);
	}
	return result;
}
wresult _DATETIME_WM_LBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_LBUTTONUP(widget, e, priv);
	if (w_widget_is_ok(widget) <= 0) {
		e->result = 0;
		return W_TRUE;
	}
	w_event event;
	if (_W_WIDGET(widget)->state & STATE_DATETIME_DOUBLECLICK) {
		event.type = W_EVENT_DEFAULTSELECTION;
		event.widget = widget;
		event.platform_event = _EVENT_PLATFORM(e);
		event.time = 0;
		event.data = 0;
		_w_widget_post_event(widget, &event, W_EVENT_SEND);
	}
	_W_WIDGET(widget)->state &= ~STATE_DATETIME_DOUBLECLICK;
	return result;
}
wresult _DATETIME_WM_TIMER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _CONTROL_WM_TIMER(widget, e, priv);
	if (result)
		return result;
	/*
	 * Feature in Windows. For some reason, Windows sends WM_NOTIFY with
	 * MCN_SELCHANGE at regular intervals. This is unexpected. The fix is
	 * to ignore MCN_SELCHANGE during WM_TIMER.
	 */
	_W_WIDGET(widget)->state |= STATE_DATETIME_IGNORESELECTION;
	e->result = priv->widget.call_window_proc(widget, e, priv);
	_W_WIDGET(widget)->state &= ~STATE_DATETIME_IGNORESELECTION;
	return W_TRUE;
}
void _w_datetime_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_datetime_class *clazz) {
	if (classId == _W_CLASS_DATETIME) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_datetime_priv;
	}
	_w_composite_class_init(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_DATETIME;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_datetime_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_datetime);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_datetime);
	/*
	 * public function
	 */
	clazz->get_day = _w_datetime_get_day;
	clazz->get_hours = _w_datetime_get_hours;
	clazz->get_minutes = _w_datetime_get_minutes;
	clazz->get_month = _w_datetime_get_month;
	clazz->get_seconds = _w_datetime_get_seconds;
	clazz->get_year = _w_datetime_get_year;
	clazz->set_date = _w_datetime_set_date;
	clazz->set_day = _w_datetime_set_day;
	clazz->set_hours = _w_datetime_set_hours;
	clazz->set_minutes = _w_datetime_set_minutes;
	clazz->set_month = _w_datetime_set_month;
	clazz->set_seconds = _w_datetime_set_seconds;
	clazz->set_time = _w_datetime_set_time;
	clazz->set_year = _w_datetime_set_year;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_DATETIME) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->check_style = _w_datetime_check_style;
		priv->compute_size = _w_datetime_compute_size;
		priv->create_handle = _w_datetime_create_handle;
		priv->widget_style = _w_datetime_widget_style;
		priv->window_class = _w_datetime_window_class;
		priv->get_def_window_proc = _w_datetime_get_def_window_proc;
		/*
		 * messages
		 */
		dispatch_message *msg = priv->messages;
		msg[_WM_NOTIFYCHILD] = _DATETIME_WM_NOTIFYCHILD;
		msg[_WM_CHAR] = _DATETIME_WM_CHAR;
		msg[_WM_LBUTTONDBLCLK] = _DATETIME_WM_LBUTTONDBLCLK;
		msg[_WM_LBUTTONDOWN] = _DATETIME_WM_LBUTTONDOWN;
		msg[_WM_LBUTTONUP] = _DATETIME_WM_LBUTTONUP;
		msg[_WM_TIMER] = _DATETIME_WM_TIMER;
	}
}
