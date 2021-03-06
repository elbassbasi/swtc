/*
 * Name:        shell.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_WIDGETS_SHELL_H_
#define WIN32_WIDGETS_SHELL_H_
#include "canvas.h"
#include "tooltip.h"
/*
 * shell
 */
typedef struct _w_shell _w_shell;
struct _w_shell {
	_w_canvas canvas;
	_w_shell *next;
	_w_shell *prev;
	w_menu *menubar;
	w_button *defaultButton;
	w_button *saveDefault;
	_w_tooltip *tooltips;
	HWND toolTipHandle;
	HWND balloonTipHandle;
	HWND menuItemToolTipHandle;
	WNDPROC windowProc;
	WNDPROC ToolTipProc;
	WNDPROC DialogProc;
	char *toolTitle;
	unsigned fullScreen :1;
	unsigned modified :1;
	unsigned opened :1;
	unsigned moved :1;
	unsigned resized :1;
	unsigned center :1;
	unsigned wasMaximized :1;
	unsigned showWithParent :1;
	unsigned swFlags :4;
	int minWidth;
	int minHeight;
};
#define _W_SHELL(x) ((_w_shell*)x)
/*
 * private
 */
typedef struct _w_shell_priv _w_shell_priv;
struct _w_shell_priv {
	_w_canvas_priv canvas;
};
#define _W_SHELL_PRIV(x) ((_w_shell_priv*)x)
#define _W_SHELL_GET_PRIV(x) ((_w_shell_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
wresult _w_shell_create_tooltip(w_shell *shell, _w_tooltip *tooltip);
wresult _w_shell_balloontip_handle(w_shell *shell, HWND *handle);
wresult _w_shell_tooltip_handle(w_shell *shell, HWND *handle);
wresult _w_shell_menu_item_tooltip_handle(w_shell *shell, HWND *handle);
void _w_shell_set_tooltip_title(w_shell *shell, HWND hwndToolTip, char *text,
		int icon);
void _w_shell_bring_totop(w_shell *shell);
wresult _w_shell_is_visible(w_control *control);
void _w_shell_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_shell_class *clazz);
/*
 * messages
 */
wresult _SHELL_WM_CLOSE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_NCACTIVATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_QUERYOPEN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_SYSCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_ACTIVATE_0(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_ACTIVATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_DESTROY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_ENTERIDLE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_GETMINMAXINFO(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_MOUSEACTIVATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_MOVE_0(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_MOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_NCHITTEST(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_NCLBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_PALETTECHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_QUERYNEWPALETTE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_SHOWWINDOW(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_WINDOWPOSCHANGING_0(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _SHELL_WM_WINDOWPOSCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
#endif /* WIN32_WIDGETS_SHELL_H_ */
