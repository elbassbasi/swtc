/*
 * button.h
 *
 *  Created on: 21 févr. 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_BUTTON_H_
#define GTK_CONTROLS_BUTTON_H_
#include "../widgets/shell.h"
#define STATE_BUTTON_GRAYED (1 << (STATE_CONTROL_END + 1))
#define STATE_BUTTON_SELECTED (1 << (STATE_CONTROL_END + 2))
#define STATE_BUTTON_END (STATE_CONTROL_END + 1)
typedef struct _w_button {
	_w_control control;
} _w_button;

typedef struct _w_button_priv {
	_w_control_priv control;
	guint signal_clicked_id;

} _w_button_priv;
#define _W_BUTTON(x) ((_w_button*)x)
#define _W_BUTTON_PRIV(x) ((_w_button_priv*)x)

wuint64 _w_button_check_style(w_widget* widget,wuint64 style);
void _w_button_select_radio_0(_w_fixed *t, _w_event_platform *e, int next);
void _w_button_select_radio(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
/*
 * public function
 */
wresult _w_button_get_alignment(w_button *button);
wresult _w_button_get_grayed(w_button *button);
wresult _w_button_get_image(w_button *button, w_image *image);
wresult _w_button_get_selection(w_button *button);
wresult _w_button_get_text(w_button *button, w_alloc alloc, void *user_data,int enc);
wresult _w_button_set_alignment(w_button *button, int alignment);
wresult _w_button_set_grayed(w_button *button, int grayed);
wresult _w_button_set_image(w_button *button, w_image *image);
wresult _w_button_set_selection(w_button *button, int selected);
wresult _w_button_set_text(w_button *button, const char *text, int length,int enc);
/*
 *
 */
gboolean _gtk_button_clicked(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
void _w_button_hook_events(w_widget *widget, _w_control_priv *priv);
wresult _w_button_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event);
void _w_button_get_border(GtkBorder *gtkBorder, const char *border,
		GtkWidget *handle, int defaultBorder);
wresult _w_button_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv);
void _w_button_class_init(struct _w_button_class *clazz);

#endif /* GTK_CONTROLS_BUTTON_H_ */
