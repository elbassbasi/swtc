/*
 * Name:        widget.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "widget.h"
#include "toolkit.h"
w_widget* _w_widget_find_control(HWND hwnd) {
	w_widget *widget = (w_widget*) GetWindowLongPtrW(hwnd, GWLP_USERDATA);
	return widget;
}
wuint64 _w_widget_check_bits(wuint64 style, int int0, int int1, int int2,
		int int3, int int4, int int5) {
	int mask = int0 | int1 | int2 | int3 | int4 | int5;
	if ((style & mask) == 0)
		style |= int0;
	if ((style & int0) != 0)
		style = (style & ~mask) | int0;
	if ((style & int1) != 0)
		style = (style & ~mask) | int1;
	if ((style & int2) != 0)
		style = (style & ~mask) | int2;
	if ((style & int3) != 0)
		style = (style & ~mask) | int3;
	if ((style & int4) != 0)
		style = (style & ~mask) | int4;
	if ((style & int5) != 0)
		style = (style & ~mask) | int5;
	return style;
}
void _w_widget_check_orientation(w_widget *widget, w_widget *parent) {
	_W_WIDGET(widget)->style &= ~W_MIRRORED;
	if ((_W_WIDGET(widget)->style & (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT)) == 0) {
		if (parent != 0) {
			if ((_W_WIDGET(parent)->style & W_LEFT_TO_RIGHT) != 0)
				_W_WIDGET(widget)->style |= W_LEFT_TO_RIGHT;
			if ((_W_WIDGET(parent)->style & W_RIGHT_TO_LEFT) != 0)
				_W_WIDGET(widget)->style |= W_RIGHT_TO_LEFT;
		}
	}
	_W_WIDGET(widget)->style = _w_widget_check_bits(_W_WIDGET(widget)->style,
			W_LEFT_TO_RIGHT, W_RIGHT_TO_LEFT, 0, 0, 0, 0);
}

void _w_widget_check_opened(w_widget *widget) {
	/* Do nothing */
}
wresult _w_widget_drag_detect(HWND hwnd, int x, int y, wresult filter,
		wresult *detect, wresult *consume) {
	if (consume != 0)
		*consume = W_FALSE;
	if (detect != 0)
		*detect = W_TRUE;
	POINT pt;
	pt.x = x;
	pt.y = y;
	ClientToScreen(hwnd, &pt);
	return DragDetect(hwnd, pt);
}
wresult _w_widget_is_ok(w_widget *obj) {
	return W_TRUE;
}
wresult _w_widget_dispose(w_widget *obj) {
	obj->clazz = 0;
	return W_TRUE;
}
wresult _w_widget_call_window_proc(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	e->result = 0;
	return W_TRUE;
}
wresult _w_widget_get_theme(w_widget *widget, w_theme **theme) {
	if (_W_WIDGET(widget)->theme != 0) {
		*theme = _W_WIDGET(widget)->theme;
		return W_TRUE;
	}
	w_toolkit *toolkit = w_widget_get_toolkit(widget);
	*theme = w_toolkit_get_theme(toolkit);
	return W_TRUE;
}
wresult _w_widget_set_theme(w_widget *widget, w_theme *theme) {
	_W_WIDGET(widget)->theme = theme;
	return W_TRUE;
}
wresult _w_widget_init_themedata(w_widget *widget, w_themedata *data) {
	w_themedata_init(data, 0, 0);
	data->style = w_widget_get_style(widget);
	return W_TRUE;
}
wresult _w_widget_get_parent(w_widget *widget, w_widget **parent) {
	*parent = _W_WIDGET(widget)->parent;
	return W_TRUE;
}
wresult _w_widget_get_first_child(w_widget *widget, w_widget **first_child,
		int filter) {
	*first_child = _W_WIDGET(widget)->first_child;
	return W_TRUE;
}
wresult _w_widget_get_next_sibling(w_widget *widget, w_widget **next_sibling,
		int filter) {
	*next_sibling = _W_WIDGET(widget)->next_sibling;
	return W_TRUE;
}
wresult _w_widget_get_prev_sibling(w_widget *widget, w_widget **prev_sibling,
		int filter) {
	*prev_sibling = _W_WIDGET(widget)->prev_sibling;
	return W_TRUE;
}
wresult _w_widget_send_event(w_widget *widget, w_event *event, int flags) {
	return W_WIDGET_GET_CLASS(widget)->post_event(widget, event, flags);
}
int _w_numpad_key(int key) {
	if (key >= VK_NUMPAD0) {
		if (key <= VK_NUMPAD9) {
			return '0' + key - VK_NUMPAD0;
		} else {
			switch (key) {
			case VK_MULTIPLY:
				return '*';
			case VK_ADD:
				return '+';
			case VK_SEPARATOR:
				return '\0';
			case VK_SUBTRACT:
				return '-';
			case VK_DECIMAL:
				return '.';
			case VK_DIVIDE:
				return '/';
			}
		}
	}
	return 0;
}
static int KeyTable[][2] = {
/* Keyboard and Mouse Masks */
{ VK_MENU, W_ALT }, //
		{ VK_SHIFT, W_SHIFT }, //
		{ VK_CONTROL, W_CTRL }, //
//		{VK_????,	W_COMMAND},

		/* NOT CURRENTLY USED */
//		{VK_LBUTTON, W_BUTTON1},
//		{VK_MBUTTON, W_BUTTON3},
//		{VK_RBUTTON, W_BUTTON2},
		/* Non-Numeric Keypad Keys */
		{ VK_UP, W_ARROW_UP }, //
		{ VK_DOWN, W_ARROW_DOWN }, //
		{ VK_LEFT, W_ARROW_LEFT }, //
		{ VK_RIGHT, W_ARROW_RIGHT }, //
		{ VK_PRIOR, W_PAGE_UP }, //
		{ VK_NEXT, W_PAGE_DOWN }, //
		{ VK_HOME, W_HOME }, //
		{ VK_END, W_END }, //
		{ VK_INSERT, W_INSERT }, //

		/* Virtual and Ascii Keys */
		{ VK_BACK, W_BS }, //
		{ VK_RETURN, W_CR }, //
		{ VK_DELETE, W_DEL }, //
		{ VK_ESCAPE, W_ESC }, //
		{ VK_RETURN, W_LF }, //
		{ VK_TAB, W_TAB }, //

		/* Functions Keys */
		{ VK_F1, W_F1 }, //
		{ VK_F2, W_F2 }, //
		{ VK_F3, W_F3 }, //
		{ VK_F4, W_F4 }, //
		{ VK_F5, W_F5 }, //
		{ VK_F6, W_F6 }, //
		{ VK_F7, W_F7 }, //
		{ VK_F8, W_F8 }, //
		{ VK_F9, W_F9 }, //
		{ VK_F10, W_F10 }, //
		{ VK_F11, W_F11 }, //
		{ VK_F12, W_F12 }, //
		{ VK_F13, W_F13 }, //
		{ VK_F14, W_F14 }, //
		{ VK_F15, W_F15 }, //
		{ VK_F16, W_F16 }, //
		{ VK_F17, W_F17 }, //
		{ VK_F18, W_F18 }, //
		{ VK_F19, W_F19 }, //
		{ VK_F20, W_F20 }, //

		/* Numeric Keypad Keys */
		{ VK_MULTIPLY, W_KEYPAD_MULTIPLY }, //
		{ VK_ADD, W_KEYPAD_ADD }, //
		{ VK_RETURN, W_KEYPAD_CR }, //
		{ VK_SUBTRACT, W_KEYPAD_SUBTRACT }, //
		{ VK_DECIMAL, W_KEYPAD_DECIMAL }, //
		{ VK_DIVIDE, W_KEYPAD_DIVIDE }, //
		{ VK_NUMPAD0, W_KEYPAD_0 }, //
		{ VK_NUMPAD1, W_KEYPAD_1 }, //
		{ VK_NUMPAD2, W_KEYPAD_2 }, //
		{ VK_NUMPAD3, W_KEYPAD_3 }, //
		{ VK_NUMPAD4, W_KEYPAD_4 }, //
		{ VK_NUMPAD5, W_KEYPAD_5 }, //
		{ VK_NUMPAD6, W_KEYPAD_6 }, //
		{ VK_NUMPAD7, W_KEYPAD_7 }, //
		{ VK_NUMPAD8, W_KEYPAD_8 }, //
		{ VK_NUMPAD9, W_KEYPAD_9 }, //
//		{VK_????,		W_KEYPAD_EQUAL},

		/* Other keys */
		{ VK_CAPITAL, W_CAPS_LOCK }, //
		{ VK_NUMLOCK, W_NUM_LOCK }, //
		{ VK_SCROLL, W_SCROLL_LOCK }, //
		{ VK_PAUSE, W_PAUSE }, //
		{ VK_CANCEL, W_BREAK }, //
		{ VK_SNAPSHOT, W_PRINT_SCREEN }, //
//		{VK_????,		W_HELP},

		};
int _w_translate_key(int key) {
	for (int i = 0; i < sizeof(KeyTable) / sizeof(KeyTable[0]); i++) {
		if (KeyTable[i][0] == key)
			return KeyTable[i][1];
	}
	return 0;
}
int _w_untranslate_key(int key) {
	for (int i = 0; i < sizeof(KeyTable) / sizeof(KeyTable[0]); i++) {
		if (KeyTable[i][1] == key)
			return KeyTable[i][0];
	}
	return 0;
}
int _w_ascii_key(int key) {
	WCHAR result[3];
	/* Get the current keyboard. */
	BYTE keyboard[0x100];
	memset(keyboard, 0, sizeof(keyboard));
	if (!GetKeyboardState(keyboard))
		return 0;

	/* Translate the key to ASCII or UNICODE using the virtual keyboard */
	if (ToUnicode(key, key, keyboard, result,
			sizeof(result) / sizeof(result[0]), 0) == 1)
		return result[0];
	return 0;
}
int _w_shifted_key(int key) {
	WCHAR result[3];
	/* Get the current keyboard. */
	BYTE keyboard[0x100];
	memset(keyboard, 0, sizeof(keyboard));
	keyboard[VK_SHIFT] |= 0x80;

	/* Translate the key to ASCII or UNICODE using the virtual keyboard */
	if (ToUnicode(key, key, keyboard, result,
			sizeof(result) / sizeof(result[0]), 0) == 1)
		return result[0];
	return 0;
}
int _w_control_key(int key) {
	WCHAR upper[3];
	upper[0] = key;
	upper[1] = 0;
	CharUpperW(upper);
	if (64 <= upper[0] && upper[0] <= 95)
		return upper[0] & 0xBF;
	return key;
}
void _w_set_input_state(w_event *event) {
	w_event_mouse *mouseevent = (w_event_mouse*) event;
	w_event_key *keyevent = (w_event_key*) event;
	if (GetKeyState(VK_MENU) < 0)
		mouseevent->detail |= W_ALT;
	if (GetKeyState(VK_SHIFT) < 0)
		mouseevent->detail |= W_SHIFT;
	if (GetKeyState(VK_CONTROL) < 0)
		mouseevent->detail |= W_CTRL;
	if (GetKeyState(VK_LBUTTON) < 0)
		mouseevent->detail |= W_BUTTON1;
	if (GetKeyState(VK_MBUTTON) < 0)
		mouseevent->detail |= W_BUTTON2;
	if (GetKeyState(VK_RBUTTON) < 0)
		mouseevent->detail |= W_BUTTON3;
	/*
	 * Bug in Windows.  On some machines that do not have XBUTTONs,
	 * the MK_XBUTTON1 and MK_XBUTTON2 bits are sometimes set,
	 * causing mouse capture to become stuck.  The fix is to test
	 * for the extra buttons only when they exist.
	 */
	if (win_toolkit->xMouse) {
		if (GetKeyState(VK_XBUTTON1) < 0)
			mouseevent->detail |= W_BUTTON4;
		if (GetKeyState(VK_XBUTTON2) < 0)
			mouseevent->detail |= W_BUTTON5;
	}
	switch (event->type) {
	case W_EVENT_MOUSEDOWN:
	case W_EVENT_MOUSEDOUBLECLICK:
		if (mouseevent->button == 1)
			mouseevent->detail &= ~W_BUTTON1;
		if (mouseevent->button == 2)
			mouseevent->detail &= ~W_BUTTON2;
		if (mouseevent->button == 3)
			mouseevent->detail &= ~W_BUTTON3;
		if (mouseevent->button == 4)
			mouseevent->detail &= ~W_BUTTON4;
		if (mouseevent->button == 5)
			mouseevent->detail &= ~W_BUTTON5;
		break;
	case W_EVENT_MOUSEUP:
		if (mouseevent->button == 1)
			mouseevent->detail |= W_BUTTON1;
		if (mouseevent->button == 2)
			mouseevent->detail |= W_BUTTON2;
		if (mouseevent->button == 3)
			mouseevent->detail |= W_BUTTON3;
		if (mouseevent->button == 4)
			mouseevent->detail |= W_BUTTON4;
		if (mouseevent->button == 5)
			mouseevent->detail |= W_BUTTON5;
		break;
	case W_EVENT_KEYDOWN:
	case W_EVENT_TRAVERSE:
		if (keyevent->keycode == W_ALT)
			mouseevent->detail &= ~W_ALT;
		if (keyevent->keycode == W_SHIFT)
			mouseevent->detail &= ~W_SHIFT;
		if (keyevent->keycode == W_CTRL)
			mouseevent->detail &= ~W_CTRL;
		break;
	case W_EVENT_KEYUP:
		if (keyevent->keycode == W_ALT)
			mouseevent->detail |= W_ALT;
		if (keyevent->keycode == W_SHIFT)
			mouseevent->detail |= W_SHIFT;
		if (keyevent->keycode == W_CTRL)
			mouseevent->detail |= W_CTRL;
		break;
	}
}
int _w_set_location_mask(w_event_key *event) {
	int location = W_NONE;
	if (win_toolkit->lastVirtual) {
		switch (win_toolkit->lastKey) {
		case VK_SHIFT:
			if (GetKeyState(VK_LSHIFT) < 0)
				location = W_LEFT;
			if (GetKeyState(VK_RSHIFT) < 0)
				location = W_RIGHT;
			break;
		case VK_NUMLOCK:
			location = W_KEYPAD;
			break;
		case VK_CONTROL:
		case VK_MENU:
			location =
					(_W_EVENT_PLATFORM(event->event.platform_event)->lparam
							& 0x1000000) == 0 ? W_LEFT : W_RIGHT;
			break;
		case VK_INSERT:
		case VK_DELETE:
		case VK_HOME:
		case VK_END:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
			if ((_W_EVENT_PLATFORM(event->event.platform_event)->lparam
					& 0x1000000) == 0) {
				location = W_KEYPAD;
			}
			break;
		}
		if (_w_numpad_key(win_toolkit->lastKey) != 0) {
			location = W_KEYPAD;
		}
	} else {
		if (win_toolkit->lastKey == W_KEYPAD_CR) {
			location = W_KEYPAD;
		}
	}
	event->keylocation = location;
	return location;
}
int _w_set_key_state(w_event_key *event) {
	/*
	 * Feature in Windows.  When the user presses Ctrl+Backspace
	 * or Ctrl+Enter, Windows sends a WM_CHAR with Delete (0x7F)
	 * and '\n' instead of '\b' and '\r'.  This is the correct
	 * platform behavior but is not portable.  The fix is to detect
	 * these cases and convert the character.
	 */
	switch (win_toolkit->lastAscii) {
	case W_DEL:
		if (win_toolkit->lastKey == W_BS)
			win_toolkit->lastAscii = W_BS;
		break;
	case W_LF:
		if (win_toolkit->lastKey == W_CR)
			win_toolkit->lastAscii = W_CR;
		break;
	}
	/*
	 * Feature in Windows.  When the user presses either the Enter
	 * key or the numeric keypad Enter key, Windows sends a WM_KEYDOWN
	 * with wParam=VK_RETURN in both cases.  In order to distinguish
	 * between the keys, the extended key bit is tested. If the bit
	 * is set, assume that the numeric keypad Enter was pressed.
	 */
	if (win_toolkit->lastKey == W_CR && win_toolkit->lastAscii == W_CR) {
		if ((_W_EVENT_PLATFORM(event->event.platform_event)->lparam & 0x1000000)
				!= 0)
			win_toolkit->lastKey = W_KEYPAD_CR;
	}
	_w_set_location_mask(event);
	if (win_toolkit->lastVirtual) {
		/*
		 * Feature in Windows.  The virtual key VK_DELETE is not
		 * treated as both a virtual key and an ASCII key by Windows.
		 * Therefore, we will not receive a WM_CHAR for this key.
		 * The fix is to treat VK_DELETE as a special case and map
		 * the ASCII value explicitly (Delete is 0x7F).
		 */
		if (win_toolkit->lastKey == VK_DELETE)
			win_toolkit->lastAscii = 0x7F;
		/*
		 * Feature in Windows.  When the user presses Ctrl+Pause, the
		 * VK_CANCEL key is generated and a WM_CHAR is sent with 0x03,
		 * possibly to allow an application to look for Ctrl+C and the
		 * the Break key at the same time.  This is unexpected and
		 * unwanted.  The fix is to detect the case and set the character
		 * to zero.
		 */
		if (win_toolkit->lastKey == VK_CANCEL)
			win_toolkit->lastAscii = 0x0;

		event->keycode = _w_translate_key(win_toolkit->lastKey);
	} else {
		event->keycode = win_toolkit->lastKey;
	}
	if (win_toolkit->lastAscii != 0 || win_toolkit->lastNull) {
		event->character = (char) win_toolkit->lastAscii;
	}
	if (event->keycode == 0 && event->character == 0) {
		if (!win_toolkit->lastNull)
			return W_FALSE;
	}
	_w_set_input_state((w_event*) event);
	return W_TRUE;
}
int _w_send_key_event(w_event_key *event) {
	if (!_w_set_key_state(event))
		return W_TRUE;
	return _w_widget_send_event(event->event.widget, (w_event*) event,
			W_EVENT_SEND);
}
int _w_widget_show_menu(w_widget *widget, int x, int y, int detail) {
	w_event_menu_detect event;
	event.event.type = W_EVENT_MENUDETECT;
	event.event.widget = widget;
	event.event.platform_event = 0;
	event.event.time = 0;
	event.event.data = 0;
	event.location.x = x;
	event.location.y = y;
	event.detail = detail;
	event.menu = 0;
	if (event.detail == W_MENU_KEYBOARD) {
		//updateMenuLocation(event);
	}
	_w_widget_send_event(widget, W_EVENT(&event), W_EVENT_SEND);
	// widget could be disposed at this point
	if (w_widget_is_ok(widget) <= 0)
		return W_FALSE;
	//if (!event.doit) return true;
	if (w_widget_is_ok(W_WIDGET(event.menu)) > 0) {
		if (x != event.location.x || y != event.location.y) {
			w_menu_set_location(event.menu, &event.location);
		}
		w_menu_set_visible(event.menu, W_TRUE);
		return W_TRUE;
	}
	return W_FALSE;
}
/*
 * messages
 */
wresult _WIDGET_WM_CAPTURECHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	win_toolkit->captureChanged = W_TRUE;
	return W_FALSE;
}
wresult _WIDGET_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_key event;
	memset(&event, 0, sizeof(event));
	win_toolkit->lastAscii = (int) e->wparam;
	win_toolkit->lastNull = e->wparam == 0;
	event.event.type = W_EVENT_KEYDOWN;
	event.event.widget = widget;
	event.event.platform_event = (struct w_event_platform*) e;
	event.event.time = win_toolkit->msg.time;
	_w_send_key_event(&event);
	if (event.doit) {
		e->result = 1;
		return W_TRUE;
	}
	// widget could be disposed at this point
	return W_FALSE;
}
wresult _WIDGET_WM_CONTEXTMENU(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((HWND) e->wparam != e->hwnd)
		return W_FALSE;

	/*
	 * Feature in Windows.  When the user presses  WM_NCRBUTTONUP,
	 * a WM_CONTEXTMENU message is generated.  This happens when
	 * the user releases the mouse over a scroll bar.  Normally,
	 * window displays the default scrolling menu but applications
	 * can process WM_CONTEXTMENU to display a different menu.
	 * Typically, an application does not want to supply a special
	 * scroll menu.  The fix is to look for a WM_CONTEXTMENU that
	 * originated from a mouse event and display the menu when the
	 * mouse was released in the client area.
	 */
	int x = 0, y = 0, detail = 0;
	if (e->lparam != -1) {
		POINT pt;
		POINTSTOPOINT(pt, e->lparam);
		x = pt.x;
		y = pt.y;
		detail = W_MENU_MOUSE;
		ScreenToClient(e->hwnd, &pt);
		RECT rect;
		GetClientRect(e->hwnd, &rect);
		if (!PtInRect(&rect, pt))
			return W_FALSE;
	} else {
		int pos = GetMessagePos();
		x = GET_X_LPARAM(pos);
		y = GET_Y_LPARAM(pos);
		detail = W_MENU_KEYBOARD;
	}

	/* Show the menu */
	if (_w_widget_show_menu(widget, x, y, detail)) {
		e->result = 0;
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _WIDGET_WM_IME_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	win_toolkit->lastKey = 0;
	win_toolkit->lastAscii = (int) e->wparam;
	win_toolkit->lastVirtual = win_toolkit->lastNull = win_toolkit->lastDead =
			W_FALSE;
	w_event_key event;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_KEYDOWN;
	event.event.widget = widget;
	event.event.platform_event = (struct w_event_platform*) e;
	event.event.time = win_toolkit->msg.time;
	if (!_w_send_key_event(&event)) {
		e->result = 1;
		return W_TRUE;
	}
	event.event.type = W_EVENT_KEYUP;
	_w_send_key_event(&event);
	// widget could be disposed at this point
	win_toolkit->lastKey = win_toolkit->lastAscii = 0;
	e->result = 1;
	return W_TRUE;
}
wresult _WIDGET_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_key event;
	WCHAR t[3];

	/* Ignore repeating modifier keys by testing key down state */
	switch ((int) e->wparam) {
	case VK_SHIFT:
	case VK_MENU:
	case VK_CONTROL:
	case VK_CAPITAL:
	case VK_NUMLOCK:
	case VK_SCROLL:
		if ((e->lparam & 0x40000000) != 0) {
			return W_FALSE;
		}
	}

	/* Clear last key and last ascii because a new key has been typed */
	win_toolkit->lastAscii = win_toolkit->lastKey = 0;
	win_toolkit->lastVirtual = win_toolkit->lastNull = win_toolkit->lastDead =
			W_FALSE;

	/* Map the virtual key */
	int mapKey = MapVirtualKeyW((int) e->wparam, 2);
	/*
	 * Feature in Windows.  For Devanagari and Bengali numbers,
	 * MapVirtualKey() returns the localized number instead of
	 * the ASCII equivalent.  For example, MapVirtualKey()
	 * maps VK_1 on the numbers keyboard to \u0967, which is
	 * the Devanagari digit '1', but not ASCII.
	 * The fix is to test for Devanagari and Bengali digits and
	 * map these explicitly.
	 *
	 * NOTE: VK_0 to VK_9 are the same as ASCII.
	 */
	if ((0x09e6 <= mapKey && mapKey <= 0x09ef)
			|| (0x0966 <= mapKey && mapKey <= 0x096f)) {
		mapKey = (int) e->wparam;
	}

	/*
	 * Bug in Windows 95 and NT.  When the user types an accent key such
	 * as ^ to get an accented character on a German keyboard, the accent
	 * key should be ignored and the next key that the user types is the
	 * accented key.  The fix is to detect the accent key stroke (called
	 * a dead key) by testing the high bit of the value returned by
	 * MapVirtualKey().
	 *
	 * When the user types an accent key that does not correspond to a
	 * virtual key, MapVirtualKey() won't set the high bit to indicate
	 * a dead key.  This happens when an accent key, such as '^' is the
	 * result of a modifier such as Shift key and MapVirtualKey() always
	 * returns the unshifted key.  The fix is to peek for a WM_DEADCHAR
	 * and avoid issuing the event.
	 */
	if ((mapKey & 0x80000000) != 0)
		return W_FALSE;

	MSG msg;
	int flags = PM_NOREMOVE | PM_NOYIELD | PM_QS_INPUT | PM_QS_POSTMESSAGE;
	if (PeekMessageW(&msg, e->hwnd, WM_DEADCHAR, WM_DEADCHAR, flags)) {
		win_toolkit->lastDead = W_TRUE;
		win_toolkit->lastVirtual = mapKey == 0;
		win_toolkit->lastKey =
				win_toolkit->lastVirtual ? (int) e->wparam : mapKey;
		return W_FALSE;
	}

	/*
	 *  Bug in Windows.  Somehow, the widget is becoming disposed after
	 *  calling PeekMessage().  In rare circumstances, it seems that
	 *  PeekMessage() can allow SWT listeners to run that might contain
	 *  application code that disposes the widget.  It is not exactly
	 *  clear how this can happen.  PeekMessage() is only looking for
	 *  WM_DEADCHAR.  It is not dispatching any message that it finds
	 *  or removing any message from the queue.  Cross-thread messages
	 *  are disabled.  The fix is to check for a disposed widget and
	 *  return without calling the window proc.
	 */
	/*if (isDisposed())
	 return LRESULT.ONE;*/

	/*
	 * If we are going to get a WM_CHAR, ensure that last key has
	 * the correct character value for the key down and key up
	 * events.  It is not sufficient to ignore the WM_KEYDOWN
	 * (when we know we are going to get a WM_CHAR) and compute
	 * the key in WM_CHAR because there is not enough information
	 * by the time we get the WM_CHAR.  For example, when the user
	 * types Ctrl+Shift+6 on a US keyboard, we get a WM_CHAR with
	 * wParam=30.  When the user types Ctrl+Shift+6 on a German
	 * keyboard, we also get a WM_CHAR with wParam=30.  On the US
	 * keyboard Shift+6 is ^, on the German keyboard Shift+6 is &.
	 * There is no way to map wParam=30 in WM_CHAR to the correct
	 * value.  Also, on international keyboards, the control key
	 * may be down when the user has not entered a control character.
	 *
	 * NOTE: On Windows 98, keypad keys are virtual despite the
	 * fact that a WM_CHAR is issued.  On Windows 2000 and XP,
	 * they are not virtual.  Therefore it is necessary to force
	 * numeric keypad keys to be virtual.
	 */
	win_toolkit->lastVirtual = mapKey == 0
			|| _w_numpad_key((int) e->wparam) != 0;
	if (win_toolkit->lastVirtual) {
		win_toolkit->lastKey = (int) e->wparam;
		/*
		 * Feature in Windows.  The virtual key VK_DELETE is not
		 * treated as both a virtual key and an ASCII key by Windows.
		 * Therefore, we will not receive a WM_CHAR for this key.
		 * The fix is to treat VK_DELETE as a special case and map
		 * the ASCII value explicitly (Delete is 0x7F).
		 */
		if (win_toolkit->lastKey == VK_DELETE)
			win_toolkit->lastAscii = 0x7F;

		/*
		 * It is possible to get a WM_CHAR for a virtual key when
		 * Num Lock is on.  If the user types Home while Num Lock
		 * is down, a WM_CHAR is issued with WPARM=55 (for the
		 * character 7).  If we are going to get a WM_CHAR we need
		 * to ensure that the last key has the correct value.  Note
		 * that Ctrl+Home does not issue a WM_CHAR when Num Lock is
		 * down.
		 */
		if (VK_NUMPAD0
				<= win_toolkit->lastKey&& win_toolkit->lastKey <= VK_DIVIDE) {
			/*
			 * Feature in Windows.  Calling to ToAscii() or ToUnicode(), clears
			 * the accented state such that the next WM_CHAR loses the accent.
			 * This makes is critical that the accent key is detected.  Also,
			 * these functions clear the character that is entered using the
			 * special Windows keypad sequence when NumLock is down (ie. typing
			 * ALT+0231 should gives 'c' with a cedilla when NumLock is down).
			 */
			if (_w_ascii_key(win_toolkit->lastKey) != 0)
				return W_FALSE;
			win_toolkit->lastAscii = _w_numpad_key(win_toolkit->lastKey);
		}
	} else {
		/*
		 * Convert LastKey to lower case because Windows non-virtual
		 * keys that are also ASCII keys, such as like VK_A, are have
		 * upper case values in WM_KEYDOWN despite the fact that the
		 * Shift was not pressed.
		 */
		t[0] = mapKey;
		t[1] = 0;
		CharLowerW(t);
		win_toolkit->lastKey = t[0];

		/*
		 * Feature in Windows. The virtual key VK_CANCEL is treated
		 * as both a virtual key and ASCII key by Windows.  This
		 * means that a WM_CHAR with WPARAM=3 will be issued for
		 * this key.  In order to distinguish between this key and
		 * Ctrl+C, mark the key as virtual.
		 */
		if (e->wparam == VK_CANCEL)
			win_toolkit->lastVirtual = W_TRUE;

		/*
		 * Some key combinations map to Windows ASCII keys depending
		 * on the keyboard.  For example, Ctrl+Alt+Q maps to @ on a
		 * German keyboard.  If the current key combination is special,
		 * the correct character is placed in wParam for processing in
		 * WM_CHAR.  If this is the case, issue the key down event from
		 * inside WM_CHAR.
		 */
		int asciiKey = _w_ascii_key((int) e->wparam);
		if (asciiKey != 0) {
			/*
			 * When the user types Ctrl+Space, ToAscii () maps this to
			 * Space.  Normally, ToAscii () maps a key to a different
			 * key if both a WM_KEYDOWN and a WM_CHAR will be issued.
			 * To avoid the extra SWT.KeyDown, look for a space and
			 * issue the event from WM_CHAR.
			 */
			if (asciiKey == ' ')
				return W_FALSE;
			if (asciiKey != (int) e->wparam)
				return W_FALSE;
			/*
			 * Feature in Windows. The virtual key VK_CANCEL is treated
			 * as both a virtual key and ASCII key by Windows.  This
			 * means that a WM_CHAR with WPARAM=3 will be issued for
			 * this key. To avoid the extra SWT.KeyDown, look for
			 * VK_CANCEL and issue the event from WM_CHAR.
			 */
			if (e->wparam == VK_CANCEL)
				return W_FALSE;
		}

		/*
		 * If the control key is not down at this point, then
		 * the key that was pressed was an accent key or a regular
		 * key such as 'A' or Shift+A.  In that case, issue the
		 * key event from WM_CHAR.
		 */
		if (GetKeyState(VK_CONTROL) >= 0)
			return W_FALSE;

		/*
		 * Get the shifted state or convert to lower case if necessary.
		 * If the user types Ctrl+A, LastAscii should be 'a', not 'A'.
		 * If the user types Ctrl+Shift+A, LastAscii should be 'A'.
		 * If the user types Ctrl+Shift+6, the value of LastAscii will
		 * depend on the international keyboard.
		 */
		if (GetKeyState(VK_SHIFT) < 0) {
			win_toolkit->lastAscii = _w_shifted_key((int) e->wparam);
			if (win_toolkit->lastAscii == 0)
				win_toolkit->lastAscii = mapKey;
		} else {
			t[0] = mapKey;
			t[1] = 0;
			CharLowerW(t);
			win_toolkit->lastAscii = t[0];
		}

		/* Note that Ctrl+'@' is ASCII NUL and is delivered in WM_CHAR */
		if (win_toolkit->lastAscii == '@')
			return W_FALSE;
		win_toolkit->lastAscii = _w_control_key(win_toolkit->lastAscii);
	}
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_KEYDOWN;
	event.event.widget = widget;
	event.event.platform_event = (struct w_event_platform*) e;
	event.event.time = win_toolkit->msg.time;
	if (_w_send_key_event(&event)) {
		e->result = 1;
		return W_TRUE;
	}
// widget could be disposed at this point
	return W_FALSE;
}
wresult _WIDGET_WM_KEYUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_key event;

	/*
	 * If the key up is not hooked, reset last key
	 * and last ascii in case the key down is hooked.
	 */
	/*if (!hooks(SWT.KeyUp) && !win_toolkit->filters(SWT.KeyUp)) {
	 win_toolkit->lastKey = win_toolkit->lastAscii = 0;
	 win_toolkit->lastVirtual = win_toolkit->lastNull =
	 win_toolkit->lastDead = false;
	 return null;
	 }*/

	/* Map the virtual key. */
	int mapKey = MapVirtualKeyW((int) e->wparam, 2);

	/*
	 * Bug in Windows 95 and NT.  When the user types an accent key such
	 * as ^ to get an accented character on a German keyboard, the accent
	 * key should be ignored and the next key that the user types is the
	 * accented key. The fix is to detect the accent key stroke (called
	 * a dead key) by testing the high bit of the value returned by
	 * MapVirtualKey ().
	 */
	if ((mapKey & 0x80000000) != 0)
		return W_FALSE;

	if (win_toolkit->lastDead)
		return W_FALSE;

	/*
	 * NOTE: On Windows 98, keypad keys are virtual despite the
	 * fact that a WM_CHAR is issued.  On Windows 2000 and XP,
	 * they are not virtual.  Therefore it is necessary to force
	 * numeric keypad keys to be virtual.
	 */
	win_toolkit->lastVirtual = mapKey == 0
			|| _w_numpad_key((int) e->wparam) != 0;
	if (win_toolkit->lastVirtual) {
		win_toolkit->lastKey = (int) e->wparam;
	} else {
		/*
		 * Feature in Windows. The virtual key VK_CANCEL is treated
		 * as both a virtual key and ASCII key by Windows.  This
		 * means that a WM_CHAR with WPARAM=3 will be issued for
		 * this key.  In order to distinguish between this key and
		 * Ctrl+C, mark the key as virtual.
		 */
		if (e->wparam == VK_CANCEL)
			win_toolkit->lastVirtual = W_TRUE;
		if (win_toolkit->lastKey == 0) {
			win_toolkit->lastAscii = 0;
			win_toolkit->lastNull = win_toolkit->lastDead = W_FALSE;
			return W_FALSE;
		}
	}
	wresult ret = W_FALSE;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_KEYUP;
	event.event.widget = widget;
	event.event.platform_event = (struct w_event_platform*) e;
	event.event.time = win_toolkit->msg.time;
	if (!_w_send_key_event(&event)) {
		e->result = 1;
		ret = W_TRUE;
	}
// widget could be disposed at this point
	win_toolkit->lastKey = win_toolkit->lastAscii = 0;
	win_toolkit->lastVirtual = win_toolkit->lastNull = win_toolkit->lastDead =
			W_FALSE;
	return ret;
}
wresult _WIDGET_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	win_toolkit->scrollRemainder = win_toolkit->scrollHRemainder = 0;
	priv->widget.call_window_proc(widget, e, priv);
	w_event event;
	memset(&event, 0, sizeof(event));
	event.type = W_EVENT_FOCUSOUT;
	event.widget = widget;
	event.platform_event = (struct w_event_platform*) e;
	_w_widget_send_event(widget, &event, W_EVENT_SEND);
// widget could be disposed at this point

	/*
	 * It is possible (but unlikely), that application
	 * code could have disposed the widget in the focus
	 * or deactivate events.  If this happens, end the
	 * processing of the Windows message by returning
	 * zero as the result of the window proc.
	 */
	/*if (isDisposed())
	 return LRESULT.ZERO;*/
	return W_TRUE;
}
wresult _WIDGET_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	priv->widget.call_window_proc(widget, e, priv);
	w_event event;
	memset(&event, 0, sizeof(event));
	event.type = W_EVENT_FOCUSIN;
	event.widget = widget;
	event.platform_event = (struct w_event_platform*) e;
	_w_widget_send_event(widget, &event, W_EVENT_SEND);
	// widget could be disposed at this point

	/*
	 * It is possible (but unlikely), that application
	 * code could have disposed the widget in the focus
	 * or activate events.  If this happens, end the
	 * processing of the Windows message by returning
	 * zero as the result of the window proc.
	 */
	/*if (isDisposed ()) return LRESULT.ZERO;
	 if (code == 0) return LRESULT.ZERO;
	 return new LRESULT (code);*/
	return W_TRUE;
}
wresult _WIDGET_WM_SYSCHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_key event;
	win_toolkit->lastAscii = (int) e->wparam;
	win_toolkit->lastNull = e->wparam == 0;

	/* Call the window proc to determine whether it is a system key or mnemonic */
	wresult oldKeyHit = win_toolkit->mnemonicKeyHit;
	win_toolkit->mnemonicKeyHit = W_TRUE;
	priv->widget.call_window_proc(widget, e, priv);
	wresult consumed = W_FALSE;
	if (!win_toolkit->mnemonicKeyHit) {
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_KEYDOWN;
		event.event.widget = widget;
		event.event.platform_event = (struct w_event_platform*) e;
		event.event.time = win_toolkit->msg.time;
		consumed = _w_send_key_event(&event);
		// widget could be disposed at this point
	}
	consumed |= win_toolkit->mnemonicKeyHit;
	win_toolkit->mnemonicKeyHit = oldKeyHit;
	if (consumed)
		e->result = 1;
	return W_TRUE;
}
wresult _WIDGET_WM_SYSKEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	WCHAR t[3];
	w_event_key event;
	/*
	 * Feature in Windows.  When WM_SYSKEYDOWN is sent,
	 * the user pressed ALT+<key> or F10 to get to the
	 * menu bar.  In order to issue events for F10 but
	 * ignore other key presses when the ALT is not down,
	 * make sure that either F10 was pressed or that ALT
	 * is pressed.
	 */
	if (e->wparam != VK_F10) {
		/* Make sure WM_SYSKEYDOWN was sent by ALT-<aKey>. */
		if ((e->lparam & 0x20000000) == 0)
			return W_FALSE;
	}

	/* Ignore well known system keys */
	switch ((int) e->wparam) {
	case VK_F4: {
		HWND hwndShell = e->hwnd;
		while (GetParent(hwndShell) != 0) {
			if (GetWindow(hwndShell, GW_OWNER) != 0)
				break;
			hwndShell = GetParent(hwndShell);
		}
		int bits = GetWindowLong(hwndShell, GWL_STYLE);
		if ((bits & WS_SYSMENU) != 0)
			return W_FALSE;
	}
	}

	/* Ignore repeating modifier keys by testing key down state */
	switch ((int) e->wparam) {
	case VK_SHIFT:
	case VK_MENU:
	case VK_CONTROL:
	case VK_CAPITAL:
	case VK_NUMLOCK:
	case VK_SCROLL:
		if ((e->lparam & 0x40000000) != 0)
			return W_FALSE;
	}

	/* Clear last key and last ascii because a new key has been typed */
	win_toolkit->lastAscii = win_toolkit->lastKey = 0;
	win_toolkit->lastVirtual = win_toolkit->lastNull = win_toolkit->lastDead =
			W_FALSE;

	/* If are going to get a WM_SYSCHAR, ignore this message. */
	int mapKey = MapVirtualKeyW((int) e->wparam, 2);

	win_toolkit->lastVirtual = mapKey == 0
			|| _w_numpad_key((int) e->wparam) != 0;
	if (win_toolkit->lastVirtual) {
		win_toolkit->lastKey = (int) e->wparam;
		/*
		 * Feature in Windows.  The virtual key VK_DELETE is not
		 * treated as both a virtual key and an ASCII key by Windows.
		 * Therefore, we will not receive a WM_SYSCHAR for this key.
		 * The fix is to treat VK_DELETE as a special case and map
		 * the ASCII value explicitly (Delete is 0x7F).
		 */
		if (win_toolkit->lastKey == VK_DELETE)
			win_toolkit->lastAscii = 0x7F;

		/* When a keypad key is typed, a WM_SYSCHAR is not issued */
		if (VK_NUMPAD0
				<= win_toolkit->lastKey&& win_toolkit->lastKey <= VK_DIVIDE) {
			/*
			 * A WM_SYSCHAR will be issued for '*', '+', '-', '.' and '/'
			 * on the numeric keypad.  Avoid issuing the key event twice
			 * by checking for these keys.  Note that calling to ToAscii()
			 * or ToUnicode(), clear the character that is entered using
			 * the special Windows keypad sequence when NumLock is down
			 * (ie. typing ALT+0231 should gives 'c' with a cedilla when
			 * NumLock is down).  Do not call either of these from here.
			 */
			switch (win_toolkit->lastKey) {
			case VK_MULTIPLY:
			case VK_ADD:
			case VK_SUBTRACT:
			case VK_DECIMAL:
			case VK_DIVIDE:
				return W_FALSE;
			}
			win_toolkit->lastAscii = _w_numpad_key(win_toolkit->lastKey);
		}
	} else {
		/*
		 * Convert LastKey to lower case because Windows non-virtual
		 * keys that are also ASCII keys, such as like VK_A, are have
		 * upper case values in WM_SYSKEYDOWN despite the fact that the
		 * Shift was not pressed.
		 */
		t[0] = mapKey;
		t[1] = 0;
		CharLowerW(t);
		win_toolkit->lastKey = t[0];
		return W_FALSE;
	}
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_KEYDOWN;
	event.event.widget = widget;
	event.event.platform_event = (struct w_event_platform*) e;
	event.event.time = win_toolkit->msg.time;

	if (!_w_send_key_event(&event)) {
		e->result = 1;
		return W_TRUE;
	}
	// widget could be disposed at this point
	return W_FALSE;
}
wresult _WIDGET_WM_SYSKEYUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return _WIDGET_WM_KEYUP(widget, e, priv);
}
wresult _WIDGET_WM_MOUSEMOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_mouse event;
	int pos = GetMessagePos();
	wresult result = W_FALSE;
	if (pos != win_toolkit->lastMouse || win_toolkit->captureChanged) {
#if !IsWinCE
		wresult trackMouse = (_W_WIDGET(widget)->state & STATE_TRACK_MOUSE)
				!= 0;
		if (trackMouse) {
			TRACKMOUSEEVENT lpEventTrack;
			lpEventTrack.cbSize = sizeof(lpEventTrack);
			lpEventTrack.dwFlags = TME_QUERY;
			lpEventTrack.hwndTrack = e->hwnd;
			TrackMouseEvent(&lpEventTrack);
			if (lpEventTrack.dwFlags == 0) {
				lpEventTrack.dwFlags = TME_LEAVE | TME_HOVER;
				lpEventTrack.hwndTrack = e->hwnd;
				TrackMouseEvent(&lpEventTrack);
				/*
				 * Force all outstanding WM_MOUSELEAVE messages to be dispatched before
				 * issuing a mouse enter.  This causes mouse exit events to be processed
				 * before mouse enter events.  Note that WM_MOUSELEAVE is posted to the
				 * event queue by TrackMouseEvent().
				 */
				MSG msg;
				int flags = PM_REMOVE | PM_NOYIELD | PM_QS_INPUT
						| PM_QS_POSTMESSAGE;
				while (PeekMessageW(&msg, 0, WM_MOUSELEAVE, WM_MOUSELEAVE,
						flags)) {
					TranslateMessage(&msg);
					DispatchMessageW(&msg);
				}
				event.event.type = W_EVENT_MOUSEENTER;
				event.event.time = 0;
				event.event.platform_event = (w_event_platform*) e;
				event.event.widget = widget;
				event.event.data = 0;
				event.button = 0;
				event.clickcount = 0;
				event.detail = 0;
				event.x = GET_X_LPARAM(e->lparam);
				event.y = GET_Y_LPARAM(e->lparam);
				_w_set_input_state((w_event*) &event);
				_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
			} else {
				lpEventTrack.dwFlags = TME_HOVER;
				TrackMouseEvent(&lpEventTrack);
			}
		}
	}
	if (pos != win_toolkit->lastMouse) {
		win_toolkit->lastMouse = pos;
		event.event.type = W_EVENT_MOUSEMOVE;
		event.event.time = 0;
		event.event.platform_event = (w_event_platform*) e;
		event.event.widget = widget;
		event.event.data = 0;
		event.button = 0;
		event.clickcount = 0;
		event.detail = 0;
		event.x = GET_X_LPARAM(e->lparam);
		event.y = GET_Y_LPARAM(e->lparam);
		_w_set_input_state((w_event*) &event);
		result = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	}
#endif
	win_toolkit->captureChanged = W_FALSE;
	return result;
}
wresult _WIDGET_WM_LBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Feature in Windows. Windows sends the following
	 * messages when the user double clicks the mouse:
	 *
	 *	WM_LBUTTONDOWN		- mouse down
	 *	WM_LBUTTONUP		- mouse up
	 *	WM_LBUTTONDBLCLK	- double click
	 *	WM_LBUTTONUP		- mouse up
	 *
	 * Applications that expect matching mouse down/up
	 * pairs will not see the second mouse down.  The
	 * fix is to send a mouse down event.
	 */
	w_event_mouse event;
	win_toolkit->captureChanged = W_FALSE;
	event.event.type = W_EVENT_MOUSEDOWN;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 1;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	event.event.type = W_EVENT_MOUSEDOUBLECLICK;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 1;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
	}
	if (!win_toolkit->captureChanged /*&& !isDisposed ()*/) {
		if (GetCapture() != e->hwnd)
			SetCapture(e->hwnd);
	}
	return ret;
}
int _w_get_last_event_time() {
#if IsWinCE
	return GetTickCount ();
#else
#endif
	return GetMessageTime();
}
int _w_get_click_count(int type, int button, HWND hwnd, LPARAM lParam) {
	if (type == W_EVENT_MOUSEDOWN) {
		int doubleClick = GetDoubleClickTime();
		int deltaTime = abs(win_toolkit->lastTime - _w_get_last_event_time());
		POINT pt;
		POINTSTOPOINT(pt, lParam);
		if (win_toolkit->lastClickHwnd == hwnd
				&& win_toolkit->lastButton == button
				&& (deltaTime <= doubleClick)
				&& PtInRect(&win_toolkit->clickRect, pt)) {
			win_toolkit->clickCount++;
		} else {
			win_toolkit->clickCount = 1;
		}
	}
	if (type == W_EVENT_MOUSEDOUBLECLICK || type == W_EVENT_MOUSEDOWN) {
		win_toolkit->lastButton = button;
		win_toolkit->lastClickHwnd = hwnd;
		win_toolkit->lastTime = _w_get_last_event_time();
		int xInset = GetSystemMetrics(SM_CXDOUBLECLK) / 2;
		int yInset = GetSystemMetrics(SM_CYDOUBLECLK) / 2;
		int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
		SetRect(&win_toolkit->clickRect, x - xInset, y - yInset, x + xInset,
				y + yInset);
	}
	if (type == W_EVENT_MOUSEUP || type == W_EVENT_MOUSEDOUBLECLICK
			|| type == W_EVENT_MOUSEDOWN) {
		return win_toolkit->clickCount;
	}
	return 0;
}
wresult dragDetect(HWND hwnd, int x, int y, wresult filter, wresult *detect,
		wresult *consume) {
	if (consume != 0)
		*consume = W_FALSE;
	if (detect != 0)
		*detect = W_TRUE;
	POINT pt;
	pt.x = x;
	pt.y = y;
	ClientToScreen(hwnd, &pt);
	return DragDetect(hwnd, pt);
}
wresult _WIDGET_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = W_FALSE;
	int x = GET_X_LPARAM(e->lparam);
	int y = GET_Y_LPARAM(e->lparam);
	wresult consume = FALSE, detect = FALSE;
	wresult dragging = FALSE, mouseDown = TRUE, dispatch;
	int count = _w_get_click_count(W_EVENT_MOUSEDOWN, 1, e->hwnd, e->lparam);
	w_event_mouse event;
	if (count == 1 && (_W_WIDGET(widget)->state & STATE_DRAG_DETECT) != 0) {
		/*
		 * Feature in Windows.  It's possible that the drag
		 * operation will not be started while the mouse is
		 * down, meaning that the mouse should be captured.
		 * This can happen when the user types the ESC key
		 * to cancel the drag.  The fix is to query the state
		 * of the mouse and capture the mouse accordingly.
		 */
		//dragging = dragDetect(e->hwnd, x, y, W_TRUE, &detect, &consume);
		//if (isDisposed ()) return LRESULT.ZERO;
		mouseDown = GetKeyState(VK_LBUTTON) < 0;
	}
	win_toolkit->captureChanged = W_FALSE;
	event.event.type = W_EVENT_MOUSEDOWN;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 1;
	event.clickcount = count;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	dispatch = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!dispatch && !consume) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
		result = W_TRUE;
	}
	if (mouseDown) {
		if (!win_toolkit->captureChanged /*&& !isDisposed ()*/) {
			if (GetCapture() != e->hwnd)
				SetCapture(e->hwnd);
		}
	}
	if (dragging) {
		event.event.type = W_EVENT_DRAGDETECT;
		event.event.time = 0;
		event.event.platform_event = (w_event_platform*) e;
		event.event.widget = widget;
		event.event.data = 0;
		event.button = 1;
		event.clickcount = 1;
		event.detail = 0;
		event.x = GET_X_LPARAM(e->lparam);
		event.y = GET_Y_LPARAM(e->lparam);
		_w_set_input_state((w_event*) &event);
		_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	} else {
		if (detect) {
			/*
			 * Feature in Windows.  DragDetect() captures the mouse
			 * and tracks its movement until the user releases the
			 * left mouse button, presses the ESC key, or moves the
			 * mouse outside the drag rectangle.  If the user moves
			 * the mouse outside of the drag rectangle, DragDetect()
			 * returns true and a drag and drop operation can be
			 * started.  When the left mouse button is released or
			 * the ESC key is pressed, these events are consumed by
			 * DragDetect() so that application code that matches
			 * mouse down/up pairs or looks for the ESC key will not
			 * function properly.  The fix is to send the missing
			 * events when the drag has not started.
			 *
			 * NOTE: For now, don't send a fake WM_KEYDOWN/WM_KEYUP
			 * events for the ESC key.  This would require computing
			 * wParam (the key) and lParam (the repeat count, scan code,
			 * extended-key flag, context code, previous key-state flag,
			 * and transition-state flag) which is non-trivial.
			 */
			if (GetKeyState(VK_ESCAPE) >= 0) {
				SendMessageW(e->hwnd, WM_LBUTTONUP, e->wparam, e->lparam);
			}
		}
	}
	return result;
}
wresult _WIDGET_WM_LBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEUP;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 1;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult result = _w_widget_send_event(widget, (w_event*) &event,
			W_EVENT_SEND);
	if (!result) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
	}
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
		if (GetCapture() == e->hwnd)
			ReleaseCapture();
	}
	return result;
}
wresult _WIDGET_WM_MBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Feature in Windows. Windows sends the following
	 * messages when the user double clicks the mouse:
	 *
	 *	WM_MBUTTONDOWN		- mouse down
	 *	WM_MBUTTONUP		- mouse up
	 *	WM_MLBUTTONDBLCLK	- double click
	 *	WM_MBUTTONUP		- mouse up
	 *
	 * Applications that expect matching mouse down/up
	 * pairs will not see the second mouse down.  The
	 * fix is to send a mouse down event.
	 */
	win_toolkit->captureChanged = W_FALSE;
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEDOWN;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 2;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	event.event.type = W_EVENT_MOUSEDOUBLECLICK;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 2;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
	}
	if (!win_toolkit->captureChanged /*&& !isDisposed()*/) {
		if (GetCapture() != e->hwnd)
			SetCapture(e->hwnd);
	}
	return W_TRUE;
}
wresult _WIDGET_WM_MBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	win_toolkit->captureChanged = W_FALSE;
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEDOWN;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 2;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
	}
	if (!win_toolkit->captureChanged /*&& !isDisposed()*/) {
		if (GetCapture() != e->hwnd)
			SetCapture(e->hwnd);
	}
	return W_TRUE;
}
wresult _WIDGET_WM_MBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEUP;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 2;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
	}
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
		if (GetCapture() == e->hwnd)
			ReleaseCapture();
	}
	return W_TRUE;
}
wresult _WIDGET_WM_MOUSEHOVER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEHOVER;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 0;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (ret) {
		e->result = 0;
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _WIDGET_WM_MOUSELEAVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int pos = GetMessagePos();
	POINT pt;
	POINTSTOPOINT(pt, pos);
	ScreenToClient(e->hwnd, &pt);
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEEXIT;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 2;
	event.clickcount = 0;
	event.detail = 0;
	event.x = pt.x;
	event.y = pt.y;
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (ret) {
		e->result = 0;
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _WIDGET_WM_MOUSEWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEWHEEL;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 0;
	event.detail = 0;
	int delta = GET_WHEEL_DELTA_WPARAM(e->wparam);
	int linesToScroll = 0;
	event.detail = 0;
	SystemParametersInfoW(SPI_GETWHEELSCROLLLINES, 0, &linesToScroll, 0);
	if (linesToScroll == WHEEL_PAGESCROLL) {
		event.detail = W_SCROLL_PAGE;
	} else {
		event.detail = W_SCROLL_LINE;
		delta *= linesToScroll;
	}
	/* Check if the delta and the remainder have the same direction (sign) */
	if ((delta ^ win_toolkit->scrollRemainder) >= 0)
		delta += win_toolkit->scrollRemainder;
	win_toolkit->scrollRemainder = delta % WHEEL_DELTA;
	event.clickcount = delta / WHEEL_DELTA;
	POINT pt;
	POINTSTOPOINT(pt, e->lparam);
	ScreenToClient(e->hwnd, &pt);
	event.x = pt.x;
	event.y = pt.y;
	_w_set_input_state((w_event*) &event);
	_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	return W_TRUE;
}
wresult _WIDGET_WM_MOUSEHWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEHORIZONTALWHEEL;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 0;
	event.detail = 0;
	int delta = GET_WHEEL_DELTA_WPARAM(e->wparam);
	event.detail = 0;
	if ((delta ^ win_toolkit->scrollHRemainder) >= 0)
		delta += win_toolkit->scrollHRemainder;
	win_toolkit->scrollHRemainder = delta % WHEEL_DELTA;

	delta = -delta;
	event.clickcount = delta / WHEEL_DELTA;
	POINT pt;
	POINTSTOPOINT(pt, e->lparam);
	ScreenToClient(e->hwnd, &pt);
	event.x = pt.x;
	event.y = pt.y;
	_w_set_input_state((w_event*) &event);
	_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	return W_TRUE;
}
wresult _WIDGET_WM_RBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Feature in Windows. Windows sends the following
	 * messages when the user double clicks the mouse:
	 *
	 *	WM_RBUTTONDOWN		- mouse down
	 *	WM_RBUTTONUP		- mouse up
	 *	WM_RBUTTONDBLCLK	- double click
	 *	WM_LBUTTONUP		- mouse up
	 *
	 * Applications that expect matching mouse down/up
	 * pairs will not see the second mouse down.  The
	 * fix is to send a mouse down event.
	 */
	win_toolkit->captureChanged = W_FALSE;
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEDOWN;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 3;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	event.event.type = W_EVENT_MOUSEDOUBLECLICK;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 3;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
	}
	if (!win_toolkit->captureChanged /*&& !isDisposed()*/) {
		if (GetCapture() != e->hwnd)
			SetCapture(e->hwnd);
	}
	return W_TRUE;
}
wresult _WIDGET_WM_RBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	win_toolkit->captureChanged = W_FALSE;
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEDOWN;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 3;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
	}
	if (!win_toolkit->captureChanged /*&& !isDisposed()*/) {
		if (GetCapture() != e->hwnd)
			SetCapture(e->hwnd);
	}
	return W_TRUE;
}
wresult _WIDGET_WM_RBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEUP;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 3;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		/* Call the DefWindowProc() to support WM_CONTEXTMENU */
		DefWindowProcW(e->hwnd, WM_RBUTTONUP, e->wparam, e->lparam);
		e->result = 0;
	}
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
		if (GetCapture() == e->hwnd)
			ReleaseCapture();
	}
	return W_TRUE;
}
wresult _WIDGET_WM_XBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Feature in Windows. Windows sends the following
	 * messages when the user double clicks the mouse:
	 *
	 *	WM_XBUTTONDOWN		- mouse down
	 *	WM_XBUTTONUP		- mouse up
	 *	WM_XLBUTTONDBLCLK	- double click
	 *	WM_XBUTTONUP		- mouse up
	 *
	 * Applications that expect matching mouse down/up
	 * pairs will not see the second mouse down.  The
	 * fix is to send a mouse down event.
	 */
	win_toolkit->captureChanged = W_FALSE;
	int button = HIWORD (e->wparam) == XBUTTON1 ? 4 : 5;
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEDOWN;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = button;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	_w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	event.event.type = W_EVENT_MOUSEDOUBLECLICK;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = 2;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
	}
	if (!win_toolkit->captureChanged /*&& !isDisposed()*/) {
		if (GetCapture() != e->hwnd)
			SetCapture(e->hwnd);
	}
	return W_TRUE;
}
wresult _WIDGET_WM_XBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	win_toolkit->captureChanged = W_FALSE;
	win_toolkit->xMouse = W_TRUE;
	int button = HIWORD (e->wparam) == XBUTTON1 ? 4 : 5;
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEDOWN;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = button;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
	}
	if (!win_toolkit->captureChanged /*&& !isDisposed()*/) {
		if (GetCapture() != e->hwnd)
			SetCapture(e->hwnd);
	}
	return W_FALSE;
}
wresult _WIDGET_WM_XBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int button = HIWORD (e->wparam) == XBUTTON1 ? 4 : 5;
	w_event_mouse event;
	event.event.type = W_EVENT_MOUSEUP;
	event.event.time = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.button = button;
	event.clickcount = 0;
	event.detail = 0;
	event.x = GET_X_LPARAM(e->lparam);
	event.y = GET_Y_LPARAM(e->lparam);
	_w_set_input_state((w_event*) &event);
	wresult ret = _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
	if (!ret) {
		priv->widget.call_window_proc(widget, e, priv);
	} else {
		e->result = 0;
	}
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
		if (GetCapture() == e->hwnd)
			ReleaseCapture();
	}
	return W_FALSE;
}
wresult _WIDGET_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HRGN rgn = CreateRectRgn(0, 0, 0, 0);
	GetUpdateRgn(e->hwnd, rgn, FALSE);
	_W_WIDGET_PRIV(priv)->call_window_proc(widget, _W_EVENT_PLATFORM(e), priv);
	w_graphics gc;
	_w_graphics_init(&gc, GetDC(e->hwnd));
	HideCaret(e->hwnd);
	RECT rect;
	GetRgnBox(rgn, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	wresult result = W_FALSE;
	if (width != 0 && height != 0) {
		HDC hDC = _W_GRAPHICS(&gc)->handle;
		SelectClipRgn(hDC, rgn);
		SetMetaRgn(hDC);
		w_event_paint event;
		event.event.type = W_EVENT_PAINT;
		event.event.platform_event = _EVENT_PLATFORM(e);
		event.event.widget = widget;
		event.event.data = 0;
		event.bounds.x = rect.left;
		event.bounds.y = rect.top;
		event.bounds.width = rect.right - rect.left;
		event.bounds.height = rect.bottom - rect.top;
		event.gc = &gc;
		result = _w_widget_send_event(widget, W_EVENT(&event), W_EVENT_SEND);
	}
	w_graphics_dispose(&gc);
	ShowCaret(e->hwnd);
	DeleteObject(rgn);
	return result;
}
void _w_widget_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_widget_class *clazz) {
	clazz->is_ok = _w_widget_is_ok;
	clazz->dispose = _w_widget_dispose;
	clazz->get_theme = _w_widget_get_theme;
	clazz->set_theme = _w_widget_set_theme;
	clazz->init_themedata = _w_widget_init_themedata;
	clazz->get_parent = _w_widget_get_parent;
	clazz->get_first_child = _w_widget_get_first_child;
	clazz->get_next_sibling = _w_widget_get_next_sibling;
	clazz->get_prev_sibling = _w_widget_get_prev_sibling;
	clazz->toolkit = toolkit;
	/*
	 * private
	 */
	_w_widget_priv *priv = _W_WIDGET_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (priv->init == 0) {
		priv->call_window_proc = _w_widget_call_window_proc;
	}
}
