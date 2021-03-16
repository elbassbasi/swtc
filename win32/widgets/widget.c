/*
 * Name:        widget.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "widget.h"
#include "toolkit.h"
_w_widget_priv* _w_widget_get_priv(w_widget *widget) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(widget);
	while (clazz->toolkit != (struct w_toolkit*) win_toolkit) {
		clazz = clazz->next_class;
	}
	return _W_WIDGET_PRIV(clazz->reserved[0]);
}
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
		_w_widget_priv *priv) {
	e->result = 0;
	return W_TRUE;
}
wresult _w_widget_send_event(w_widget *widget, w_event *event) {
	if (widget->post_event != 0) {
		return widget->post_event(widget, event);
	} else {
		return w_widget_default_post_event(widget, event);
	}
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
		mouseevent->statemask |= W_ALT;
	if (GetKeyState(VK_SHIFT) < 0)
		mouseevent->statemask |= W_SHIFT;
	if (GetKeyState(VK_CONTROL) < 0)
		mouseevent->statemask |= W_CTRL;
	if (GetKeyState(VK_LBUTTON) < 0)
		mouseevent->statemask |= W_BUTTON1;
	if (GetKeyState(VK_MBUTTON) < 0)
		mouseevent->statemask |= W_BUTTON2;
	if (GetKeyState(VK_RBUTTON) < 0)
		mouseevent->statemask |= W_BUTTON3;
	/*
	 * Bug in Windows.  On some machines that do not have XBUTTONs,
	 * the MK_XBUTTON1 and MK_XBUTTON2 bits are sometimes set,
	 * causing mouse capture to become stuck.  The fix is to test
	 * for the extra buttons only when they exist.
	 */
	if (win_toolkit->xMouse) {
		if (GetKeyState(VK_XBUTTON1) < 0)
			mouseevent->statemask |= W_BUTTON4;
		if (GetKeyState(VK_XBUTTON2) < 0)
			mouseevent->statemask |= W_BUTTON5;
	}
	switch (event->type) {
	case W_EVENT_MOUSEDOWN:
	case W_EVENT_MOUSEDOUBLECLICK:
		if (mouseevent->button == 1)
			mouseevent->statemask &= ~W_BUTTON1;
		if (mouseevent->button == 2)
			mouseevent->statemask &= ~W_BUTTON2;
		if (mouseevent->button == 3)
			mouseevent->statemask &= ~W_BUTTON3;
		if (mouseevent->button == 4)
			mouseevent->statemask &= ~W_BUTTON4;
		if (mouseevent->button == 5)
			mouseevent->statemask &= ~W_BUTTON5;
		break;
	case W_EVENT_MOUSEUP:
		if (mouseevent->button == 1)
			mouseevent->statemask |= W_BUTTON1;
		if (mouseevent->button == 2)
			mouseevent->statemask |= W_BUTTON2;
		if (mouseevent->button == 3)
			mouseevent->statemask |= W_BUTTON3;
		if (mouseevent->button == 4)
			mouseevent->statemask |= W_BUTTON4;
		if (mouseevent->button == 5)
			mouseevent->statemask |= W_BUTTON5;
		break;
	case W_EVENT_KEYDOWN:
	case W_EVENT_TRAVERSE:
		if (keyevent->keycode == W_ALT)
			mouseevent->statemask &= ~W_ALT;
		if (keyevent->keycode == W_SHIFT)
			mouseevent->statemask &= ~W_SHIFT;
		if (keyevent->keycode == W_CTRL)
			mouseevent->statemask &= ~W_CTRL;
		break;
	case W_EVENT_KEYUP:
		if (keyevent->keycode == W_ALT)
			mouseevent->statemask |= W_ALT;
		if (keyevent->keycode == W_SHIFT)
			mouseevent->statemask |= W_SHIFT;
		if (keyevent->keycode == W_CTRL)
			mouseevent->statemask |= W_CTRL;
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
	return _w_widget_send_event(event->event.widget, (w_event*) event);
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
	return W_FALSE;
}
wresult _WIDGET_WM_IME_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_KEYUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_SYSCHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_SYSKEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_SYSKEYUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_MOUSEMOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_LBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_LBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_MBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_MBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_MBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_MOUSEHOVER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_MOUSELEAVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_MOUSEWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_MOUSEHWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_RBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_RBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_RBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_XBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_XBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_XBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _WIDGET_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	HRGN rgn = CreateRectRgn(0, 0, 0, 0);
	GetUpdateRgn(e->hwnd, rgn, FALSE);
	_W_WIDGET_PRIV(priv)->call_window_proc(widget, _W_EVENT_PLATFORM(e),
			_W_WIDGET_PRIV(priv));
	w_graphics gc;
	_w_graphics_init(&gc, GetDC(e->hwnd));
	HideCaret(e->hwnd);
	RECT rect;
	GetRgnBox(rgn, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
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
		_w_widget_send_event(widget, W_EVENT(&event));
	}
	w_graphics_dispose(&gc);
	ShowCaret(e->hwnd);
	DeleteObject(rgn);
	return W_TRUE;
}
void _w_widget_class_init(struct _w_widget_class *clazz) {
	clazz->is_ok = _w_widget_is_ok;
	clazz->dispose = _w_widget_dispose;
	clazz->toolkit = (w_toolkit*) win_toolkit;
	/*
	 * private
	 */
	_w_widget_priv *priv = _W_WIDGET_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->call_window_proc = _w_widget_call_window_proc;
}
