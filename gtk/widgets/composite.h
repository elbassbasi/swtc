/*
 * Name:        composite.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_WIDGETS_COMPOSITE_H_
#define GTK_WIDGETS_COMPOSITE_H_
#include "scrollable.h"
/*
 * states
 */
#define STATE_COMPOSITE_END (STATE_SCROLLABLE_END + 0)
/*
 * composite
 */
typedef struct _w_composite {
	_w_scrollable scrollable;
	GtkIMContext *imHandle;
	w_layout *layout;
	int children_count;
	int layoutCount;
} _w_composite;
#define _W_COMPOSITE(x) ((_w_composite*)x)
/*
 * private
 */
typedef struct _w_composite_priv _w_composite_priv;
struct _w_composite_priv {
	_w_scrollable_priv scrollable;
	void (*add_child)(w_control *composite, w_widget *child,
			_w_control_priv *priv);
	w_composite* (*find_deferred_control)(w_control *composite,
			_w_control_priv *priv);
	GtkWidget* (*handle_parenting)(w_widget *control, _w_control_priv *priv);
	wresult (*has_border)(w_composite *composite, _w_control_priv *priv);
};
#define _W_COMPOSITE_PRIV(x) ((_w_composite_priv*)x)
#define _W_COMPOSITE_GET_PRIV(x) ((_w_composite_priv*)_W_WIDGET_GET_PRIV(x))
/*
 * functions
 */
void _w_composite_hook_events(w_widget *widget, _w_control_priv *priv);
wresult _w_composite_create_handle_0(w_widget *composite, GtkWidget **fixed,
		GtkWidget **scrolled, GtkWidget **handle, _w_control_priv *priv);
wresult _w_composite_create_handle(w_widget *widget, _w_control_priv *priv);
wresult _w_composite_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv);
wresult _w_composite_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv);
wresult _w_composite_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_priv *priv);
void _w_composite_add_child(w_control *composite, w_widget *child,
		_w_control_priv *priv);
/*
 * signals
 */
gboolean _gtk_composite_destroy(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_composite_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
gboolean _gtk_composite_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_composite_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_composite_focus(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_composite_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_composite_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_composite_map(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_composite_realize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_composite_scroll_child(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _w_composite_dispose_class(struct _w_widget_class *clazz);
void _w_composite_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_composite_class *clazz);
/*
 * messages
 */
#endif /* GTK_WIDGETS_COMPOSITE_H_ */
