/*
 * Name:        widget.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_WIDGETS_WIDGET_H_
#define WIN32_WIDGETS_WIDGET_H_
#include "../graphics/gc.h"
/*
 * states
 */
#define STATE_DISPOSE_SENT (1 << 0)
#define STATE_DRAG_DETECT (1 << 1)
#define STATE_DISPOSED (1 << 2)
#define STATE_TRACK_MOUSE (1 << 3)
#define STATE_WIDGET_END (4)

/* Default size for widgets */
#define DEFAULT_WIDTH 64
#define DEFAULT_HEIGHT 64

/* Bidi UCC to enforce text direction */
#define LRE 0x202a
#define RLE 0x202b

/* Bidi flag and for auto text direction */
#define AUTO_TEXT_DIRECTION (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT)

/*
 * messages
 */
#define WM_NOTIFY_CHILD (WM_USER + 1)
#define WM_CTLCOLOR_CHILD (WM_USER + 2)
#define WM_COMMAND_CHILD (WM_USER + 3)
#define WM_SCROLL_CHILD (WM_USER + 4)
#define WM_ASYNCEXEC (WM_USER + 5)
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef WM_TOUCH
#define WM_TOUCH 0x240
#endif
enum {
	_WM_ACTIVATE = 1,
	_WM_CAPTURECHANGED,
	_WM_CHANGEUISTATE,
	_WM_CHAR,
	_WM_CLEAR,
	_WM_CLOSE,
	_WM_COMMAND,
	_WM_CONTEXTMENU,
	_WM_CTLCOLOR,
	_WM_CUT,
	_WM_CREATE,
	_WM_DESTROY,
	_WM_DRAWITEM,
	_WM_ENDSESSION,
	_WM_ENTERIDLE,
	_WM_ERASEBKGND,
	_WM_GESTURE,
	_WM_GETDLGCODE,
	_WM_GETFONT,
	_WM_GETOBJECT,
	_WM_GETMINMAXINFO,
	_WM_HELP,
	_WM_HSCROLL,
	_WM_IME_CHAR,
	_WM_IME_COMPOSITION,
	_WM_IME_COMPOSITION_START,
	_WM_IME_ENDCOMPOSITION,
	_WM_INITMENUPOPUP,
	_WM_INPUTLANGCHANGE,
	_WM_HOTKEY,
	_WM_KEYDOWN,
	_WM_KEYUP,
	_WM_KILLFOCUS,
	_WM_LBUTTONDBLCLK,
	_WM_LBUTTONDOWN,
	_WM_LBUTTONUP,
	_WM_MBUTTONDBLCLK,
	_WM_MBUTTONDOWN,
	_WM_MBUTTONUP,
	_WM_MEASUREITEM,
	_WM_MENUCHAR,
	_WM_MENUSELECT,
	_WM_MOUSEACTIVATE,
	_WM_MOUSEHOVER,
	_WM_MOUSELEAVE,
	_WM_MOUSEMOVE,
	_WM_MOUSEWHEEL,
	_WM_MOUSEHWHEEL,
	_WM_MOVE,
	_WM_NCACTIVATE,
	_WM_NCCALCSIZE,
	_WM_NCHITTEST,
	_WM_NCLBUTTONDOWN,
	_WM_NCPAINT,
	_WM_NOTIFY,
	_WM_PAINT,
	_WM_PALETTECHANGED,
	_WM_PARENTNOTIFY,
	_WM_PASTE,
	_WM_PRINT,
	_WM_PRINTCLIENT,
	_WM_QUERYENDSESSION,
	_WM_QUERYNEWPALETTE,
	_WM_QUERYOPEN,
	_WM_RBUTTONDBLCLK,
	_WM_RBUTTONDOWN,
	_WM_RBUTTONUP,
	_WM_SETCURSOR,
	_WM_SETFOCUS,
	_WM_SETFONT,
	_WM_SETTINGCHANGE,
	_WM_SETREDRAW,
	_WM_SHOWWINDOW,
	_WM_SIZE,
	_WM_SYSCHAR,
	_WM_SYSCOLORCHANGE,
	_WM_SYSCOMMAND,
	_WM_SYSKEYDOWN,
	_WM_SYSKEYUP,
	_WM_TABLET_FLICK,
	_WM_TIMER,
	_WM_TOUCH,
	_WM_UNDO,
	_WM_UNINITMENUPOPUP,
	_WM_UPDATEUISTATE,
	_WM_VSCROLL,
	_WM_WINDOWPOSCHANGED,
	_WM_WINDOWPOSCHANGING,
	_WM_XBUTTONDBLCLK,
	_WM_XBUTTONDOWN,
	_WM_XBUTTONUP,
	_WM_DPICHANGED,
	_WM_MENUCOMMAND,
	_WM_CTLCOLORCHILD,
	_WM_NOTIFYCHILD,
	_WM_COMMANDCHILD,
	_WM_DRAWCHILD,
	_WM_MEASURECHILD,
	_WM_SCROLLCHILD,
	_WM_LAST
};

typedef struct _w_event_platform {
	w_event event;
	UINT msg;
	LRESULT result;
	HWND hwnd;
	WPARAM wparam;
	LPARAM lparam;
} _w_event_platform;
#define _W_EVENT_PLATFORM(x) ((_w_event_platform*)x)

typedef struct _w_widget {
	void *v_table; //used in c++ as virtual table
	struct _w_widget_class *clazz;
	volatile int ref;
	wuint id;
	wuint64 style;
	wuint state0;
	wuint state;
	union {
		HWND handle;
		HMENU handleMenu;
	};
	w_widget_post_event_proc post_event;
	void *data[5];
} _w_widget;
#define _W_WIDGET(x) ((_w_widget*)x)
/*
 * private
 */
typedef struct _w_widget_priv _w_widget_priv;
typedef struct _w_control_priv _w_control_priv;
struct _w_widget_priv {
	void (*destroy)(w_widget *widget);
	wresult (*window_proc)(w_widget *widget, _w_event_platform *e,
			_w_control_priv *priv);
	wresult (*call_window_proc)(w_widget *widget, _w_event_platform *e,
			_w_control_priv *priv);
};
#define _W_WIDGET_PRIV(x) ((_w_widget_priv*)x)
#define _W_WIDGET_GET_PRIV(x) ((_w_widget_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
_w_widget_priv* _w_widget_get_priv(w_widget *widget);
w_widget* _w_widget_find_control(HWND hwnd);
wuint64 _w_widget_check_bits(wuint64 style, int int0, int int1, int int2,
		int int3, int int4, int int5);
wresult _w_widget_send_event(w_widget *widget, w_event *event);
int _w_translate_key(int key);
int _w_untranslate_key(int key);
void _w_widget_class_init(struct _w_widget_class *clazz);
/*
 * messages
 */
wresult _WIDGET_WM_CAPTURECHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_CONTEXTMENU(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_IME_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_KEYUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_SYSCHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_SYSKEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_SYSKEYUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_MOUSEMOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_LBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_LBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_MBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_MBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_MBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_MOUSEHOVER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_MOUSELEAVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_MOUSEWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_MOUSEHWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_RBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_RBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_RBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_XBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_XBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_XBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _WIDGET_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
#endif /* WIN32_WIDGETS_WIDGET_H_ */
