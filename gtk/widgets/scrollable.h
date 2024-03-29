/*
 * Name:        scrollable.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_WIDGETS_SCROLLABLE_H_
#define GTK_WIDGETS_SCROLLABLE_H_
#include "control.h"
/*
 * states
 */
#define STATE_VSCROLLBAR_DRAGSEND (1 << (STATE_CONTROL_END + 1))
#define STATE_HSCROLLBAR_DRAGSEND (1 << (STATE_CONTROL_END + 2))
#define STATE_SCROLLABLE_END (STATE_CONTROL_END + 2)
/*
 * scrollbar
 */
typedef struct _w_scrollbar {
	w_widgetdata widgetdata;
	w_scrollable *parent;
	GtkWidget *scrolledHandle;
	int style;
} _w_scrollbar;
#define _W_SCROLLBAR(x) ((_w_scrollbar*)x)
/*
 * scrollable
 */
typedef struct _w_scrollable {
	_w_control control;
	int vdetail;
	int hdetail;
} _w_scrollable;
#define _W_SCROLLABLE(x) ((_w_scrollable*)x)
/*
 * private
 */
typedef struct _w_scrollable_priv _w_scrollable_priv;
struct _w_scrollable_priv {
	_w_control_priv control;
	int (*apply_theme_background)(w_scrollable *scrollable,
			_w_control_priv *priv);
	GtkWidget* (*handle_scrolled)(w_widget *control, _w_control_priv *priv);
	void (*update_scrollbar_value)(w_scrollable *scrollable, wuint64 style,
			_w_control_priv *priv);
};
#define _W_SCROLLABLE_PRIV(x) ((_w_scrollable_priv*)x)
#define _W_SCROLLABLE_GET_PRIV(x) ((_w_scrollable_priv*)_W_WIDGET_GET_PRIV(x))
/*
 * functions
 */
int _w_scrollable_hscrollbar_width(w_scrollable *scrollable,
		_w_control_priv *priv);
int _w_scrollable_vscrollbar_width(w_scrollable *scrollable,
		_w_control_priv *priv);
wresult _w_scrollable_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv);
int _w_scrollable_apply_theme_background(w_scrollable *scrollable,
		_w_control_priv *priv);
void _w_scrollable_hook_events(w_widget *widget, _w_control_priv *priv);
wresult _w_scrollable_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv);
/*
 * signals
 */
wuint64 _gtk_scrollbar_style(GtkWidget *scrolledHandle, GtkWidget *adjustment);
gboolean _gtk_scrollbar_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_scrollable_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_scrollable_event_after(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _w_scrollable_dispose_class(struct _w_widget_class *clazz);
void _w_scrollable_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_scrollable_class *clazz);
#endif /* GTK_WIDGETS_SCROLLABLE_H_ */
