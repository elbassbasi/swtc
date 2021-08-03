/*
 * Name:        shell.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_WIDGETS_SHELL_H_
#define GTK_WIDGETS_SHELL_H_
#include "canvas.h"
/*
 * states
 */
#define STATE_MODAL_SHELL (1 << (STATE_CANVAS_END + 1))
#define STATE_FOREIGN_HANDLE (1 << (STATE_CANVAS_END + 2))
#define STATE_SHELL_END (STATE_CANVAS_END + 2)
/*
 * const
 */
#define _W_SHELL_MAXIMUM_TRIM 128
#define _W_SHELL_BORDER 3
#define _W_SHELL_HANDLE(x) _w_widget_hppp(W_WIDGET(x),0)
#define _W_SHELL_VBOX(x) _w_widget_hpp(W_WIDGET(x),0)
#define ISCUSTOMRESIZE(style) ((style & W_NO_TRIM) == 0 && (style & (W_RESIZE | W_ON_TOP)) == (W_RESIZE | W_ON_TOP))
#define isUndecorated(style) ((style & (W_SHELL_TRIM | W_BORDER)) == W_NONE || \
		(style & (W_NO_TRIM | W_ON_TOP)) != 0)

#define GDK_NO_EXPOSE 30
/*
 * shell
 */
typedef struct _w_shell _w_shell;
struct _w_shell {
	_w_canvas canvas;
	w_link_0 shells_link;
	w_menu *menubar;
	GtkAccelGroup *accelGroup;
	GtkWindowGroup *group;
	w_control *lastActive;
	unsigned center :1;
	unsigned fullScreen :1;
	unsigned mapped :1;
	unsigned minimized :1;
	unsigned opened :1;
	unsigned moved :1;
	unsigned resized :1;
	unsigned showWithParent :1;
	unsigned ignoreFocusOut :1;
	int minWidth;
	int minHeight;
	w_rect oldbounds;
};
#define _W_SHELL(x) ((_w_shell*)x)
/*
 * private
 */
typedef struct _w_shell_priv _w_shell_priv;
struct _w_shell_priv {
	_w_canvas_priv canvas;
	_gtk_signal move_focus;
};
#define _W_SHELL_PRIV(x) ((_w_shell_priv*)x)
#define _W_SHELL_GET_PRIV(x) ((_w_shell_priv*)_W_WIDGET_GET_PRIV(x))
/*
 * functions
 */
void _w_shell_bring_totop(w_shell *shell, int force);
wresult _w_shell_close(w_shell *shell);
void _w_shell_close_widget(w_shell *shell, _w_event_platform *e);
GtkAccelGroup* _w_shell_create_accel_group(w_shell *shell);
void _w_shell_destroy_accel_group(w_shell *shell);
void _w_shell_set_active_control_0(w_shell *shell, w_control *control,
		int type);
void _w_shell_set_saved_focus(w_shell *shell, w_control *control);
void _w_shell_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_shell_class *clazz);
/*
 * signals
 */
gboolean _gtk_shell_destroy(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_configure_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_delete_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_focus(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_move_focus(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_motion_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_size_allocate(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_realize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_shell_window_state_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
#endif /* GTK_WIDGETS_SHELL_H_ */
