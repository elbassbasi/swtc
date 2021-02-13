/*
 * Name:        control1.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "control.h"
#include "toolkit.h"
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
/*
 * signals
 */
gboolean _gtk_control_destroy(w_widget *widget, _w_event_platform *ee,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_button_press_event_0(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv, int sendMouseDown) {
	return W_FALSE;
}
gboolean _gtk_control_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_button_release_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_commit(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_event_after(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((_W_WIDGET(widget)->state & STATE_OBSCURED) != 0)
		return 0;
#if USE_CAIRO
	cairo_t *cairo = (cairo_t*) e->args[0];
	w_event_paint event;
	_w_graphics gc;
	_w_graphics_init(W_GRAPHICS(&gc), cairo);
	gdk_cairo_get_clip_rectangle(cairo, &gc.clipping);
	memset(&event, 0, sizeof(event));
	//event.count = 1;
	event.event.type = W_EVENT_PAINT;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.bounds.x = gc.clipping.x;
	event.bounds.y = gc.clipping.y;
	event.bounds.width = gc.clipping.width;
	event.bounds.height = gc.clipping.height;
	if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
		event.bounds.x = priv->get_client_width(W_CONTROL(widget), priv)
				- event.bounds.width - event.bounds.x;
	}
	/*
	 * Pass the region into the GCData so that GC.fill* methods can be aware of the region
	 * and clip themselves accordingly. Only relevant on GTK3.10+, see bug 475784.
	 */
	/*if (drawRegion)
	 data.regionSet = eventRegion;*/
//	data.damageRgn = gdkEvent.region;
	event.gc = W_GRAPHICS(&gc);
	/*w_graphics_set_clipping_rect(W_GRAPHICS(&gc), &event.bounds);
	 w_font *font;
	 w_control_get_font(W_CONTROL(widget), &font);
	 w_graphics_set_font(W_GRAPHICS(&gc), font);
	 w_color background;
	 w_control_get_background(W_CONTROL(widget), &background);
	 w_graphics_set_background(W_GRAPHICS(&gc), background);
	 w_color foreground;
	 w_control_get_foreground(W_CONTROL(widget), &foreground);
	 w_graphics_set_foreground(W_GRAPHICS(&gc), foreground);
	 priv->draw_widget(W_CONTROL(widget), W_GRAPHICS(&gc), priv);*/
	_w_widget_send_event(widget, (w_event*) &event);
	w_graphics_dispose(W_GRAPHICS(&gc));
#else
#endif
	return 0;
}
gboolean _gtk_control_expose_inverse(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_focus(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_key_release_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_mnemonic_activate(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_motion_notify_event(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_popup_menu(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_preedit_changed(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_realize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_scroll_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_show_help(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_style_set(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
gboolean _gtk_control_unrealize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
