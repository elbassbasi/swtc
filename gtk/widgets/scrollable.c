/*
 * Name:        scrollable.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "scrollable.h"
#include "toolkit.h"
void _w_scrollable_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_control_hook_events(widget, priv);
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_PRIV(priv)->handle_scrolled(widget, priv);
	if (scrolledHandle != 0) {
		if (gtk_toolkit->signal_id[SIGNAL_CHANGE_VALUE] == 0)
			gtk_toolkit->signal_id[SIGNAL_CHANGE_VALUE] = g_signal_lookup(
					"change-value", GTK_TYPE_SCROLLBAR);
		if (gtk_toolkit->signal_id[SIGNAL_VALUE_CHANGED] == 0)
			gtk_toolkit->signal_id[SIGNAL_VALUE_CHANGED] = g_signal_lookup(
					"value-changed", GTK_TYPE_ADJUSTMENT);
		GtkWidget *handle = gtk_scrolled_window_get_vscrollbar(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		GtkAdjustment *adjustmentHandle = gtk_scrolled_window_get_vadjustment(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		_w_widget_connect(handle, SIGNAL_CHANGE_VALUE, 0, FALSE);
		_w_widget_connect((GtkWidget*) adjustmentHandle, SIGNAL_VALUE_CHANGED,
				0, FALSE);
		_w_widget_connect(handle, SIGNAL_EVENT_AFTER, 0, FALSE);
		_w_widget_connect(handle, SIGNAL_BUTTON_PRESS_EVENT, 0, FALSE);
		handle = gtk_scrolled_window_get_hscrollbar(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		adjustmentHandle = gtk_scrolled_window_get_hadjustment(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		_w_widget_connect(handle, SIGNAL_CHANGE_VALUE, 0, FALSE);
		_w_widget_connect((GtkWidget*) adjustmentHandle, SIGNAL_VALUE_CHANGED,
				0, FALSE);
		_w_widget_connect(handle, SIGNAL_EVENT_AFTER, 0, FALSE);
		_w_widget_connect(handle, SIGNAL_BUTTON_PRESS_EVENT, 0, FALSE);
	}
}
wresult _w_scrollable_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	GtkAllocation allocation;
	//priv->force_resize(W_CONTROL(widget), priv);
	GtkWidget *clientHandle = priv->handle_client(widget, priv);
	gtk_widget_get_allocation(clientHandle, &allocation);
	e->rect->x = allocation.x;
	e->rect->y = allocation.y;
	e->rect->width = allocation.width;
	e->rect->height = allocation.height;
	return W_TRUE;
}
void _w_scrollable_class_init(struct _w_scrollable_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->get_client_area = _w_scrollable_get_client_area;
	_W_WIDGET_PRIV(priv)->hook_events = _w_scrollable_hook_events;
	_W_SCROLLABLE_PRIV(priv)->handle_scrolled = _w_widget_h0;
}
