/*
 * Name:        scrollable.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "scrollable.h"
#include "toolkit.h"
int _w_scrollable_hscrollbar_width(w_scrollable *scrollable,
		_w_control_priv *priv) {
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_PRIV(priv)->handle_scrolled(W_WIDGET(scrollable), priv);
	if (scrolledHandle != 0) {
		GtkWidget *hBarHandle = 0;
		hBarHandle = gtk_scrolled_window_get_hscrollbar(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		if (hBarHandle == 0)
			return 0;
		GtkRequisition requisition;
#if GTK3
		gtk_widget_get_preferred_size(hBarHandle, &requisition, NULL);
#else
	gtk_widget_size_request(hBarHandle, &requisition);
#endif
		int padding;
		gtk_widget_style_get(scrolledHandle, "scrollbar-spacing", &padding,
		NULL);
		int spacing = padding;
		return requisition.height + spacing;
	} else
		return 0;
}
int _w_scrollable_vscrollbar_width(w_scrollable *scrollable,
		_w_control_priv *priv) {
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_PRIV(priv)->handle_scrolled(W_WIDGET(scrollable), priv);
	if (scrolledHandle != 0) {
		GtkWidget *vBarHandle = 0;
		vBarHandle = gtk_scrolled_window_get_vscrollbar(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		if (vBarHandle == 0)
			return 0;
		GtkRequisition requisition;
#if GTK3
		gtk_widget_get_preferred_size(vBarHandle, &requisition, NULL);
#else
	gtk_widget_size_request(vBarHandle, &requisition);
#endif
		int padding;
		gtk_widget_style_get(scrolledHandle, "scrollbar-spacing", &padding,
		NULL);
		int spacing = padding;
		return requisition.width + spacing;
	} else
		return 0;
}
int _w_scrollable_apply_theme_background(w_scrollable *scrollable,
		_w_control_priv *priv) {
	return (_W_CONTROL(scrollable)->backgroundAlpha == 0) ? 1 : 0;
}
wresult _w_scrollable_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	GtkWidget *fixedHandle = priv->handle_fixed(widget, priv);
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_PRIV(priv)->handle_scrolled(W_WIDGET(widget), priv);
	int border = 0;
	if (fixedHandle != 0)
		border += gtk_container_get_border_width(GTK_CONTAINER(fixedHandle));
	if (scrolledHandle != 0)
		border += gtk_container_get_border_width(GTK_CONTAINER(scrolledHandle));
	e->result->x = e->rect->x - border;
	e->result->y = e->rect->y - border;
	e->result->width = e->rect->width + (border * 2);
	e->result->height = e->rect->height + (border * 2);
	e->result->height += _w_scrollable_hscrollbar_width(W_SCROLLABLE(widget),
			priv);
	e->result->width += _w_scrollable_vscrollbar_width(W_SCROLLABLE(widget),
			priv);
	if (scrolledHandle != 0) {
		if (gtk_scrolled_window_get_shadow_type(
				GTK_SCROLLED_WINDOW(scrolledHandle)) != GTK_SHADOW_NONE) {
			w_point thickness;
			_w_control_get_thickness(scrolledHandle, &thickness);
			int xthickness = thickness.x;
			int ythickness = thickness.y;
			e->result->x -= xthickness;
			e->result->y -= ythickness;
			e->result->width += xthickness * 2;
			e->result->height += ythickness * 2;
		}
	}
	return W_TRUE;
}
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
/*
 * signals
 */
wuint64 _gtk_scrollbar_style(GtkWidget *scrolledHandle, GtkWidget *adjustment) {
	GtkWidget *scrollbar = gtk_scrolled_window_get_hscrollbar(
			GTK_SCROLLED_WINDOW(scrolledHandle));
	if (adjustment == scrollbar) {
		return W_VSCROLL;
	} else {
		return W_HSCROLL;
	}
}
gboolean _gtk_scrollbar_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	GtkWidget *scrolledHandle = _W_SCROLLABLE_PRIV(priv)->handle_scrolled(
			widget, priv);
	wuint64 style = _gtk_scrollbar_style(scrolledHandle, e->widget);
	if (style & W_VSCROLL) {
		_W_SCROLLABLE(widget)->vdetail = GTK_SCROLL_NONE;
		_W_WIDGET(widget)->state &= ~ STATE_VSCROLLBAR_DRAGSEND;
	} else {
		_W_SCROLLABLE(widget)->hdetail = GTK_SCROLL_NONE;
		_W_WIDGET(widget)->state &= ~ STATE_HSCROLLBAR_DRAGSEND;
	}
	return FALSE;
}
gboolean _gtk_scrollable_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	if (GTK_IS_SCROLLBAR(e->widget)) {
		return _gtk_scrollbar_button_press_event(widget, e, priv);
	} else
		return _gtk_control_button_press_event(widget, e, priv);
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
	_W_SCROLLABLE_PRIV(priv)->apply_theme_background =
			_w_scrollable_apply_theme_background;
}
