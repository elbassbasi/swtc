/*
 * Name:        shell.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef OSX_WIDGETS_SHELL_H_
#define OSX_WIDGETS_SHELL_H_
#include "canvas.h"
/*
 * state
 */
//#define STATE_FOREIGN_HANDLE (1 << (STATE_CANVAS_END + 2))
/*
 * shell
 */
typedef struct _w_shell _w_shell;
struct _w_shell {
	_w_canvas canvas;
	_w_shell *next;
	_w_shell *prev;
	w_menu *menubar;
	NSWindow *window;
	unsigned center :1;
	unsigned opened:1;
	unsigned moved:1;
	unsigned resized:1;
	unsigned minimized:1;
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
wresult _w_shell_force_active(w_shell *shell);
void _w_shell_make_key_and_order_front(w_shell *shell);
wresult _w_shell_set_active(w_shell *shell);
void _w_shell_class_init(struct _w_shell_class *clazz);
#endif /* OSX_WIDGETS_SHELL_H_ */
