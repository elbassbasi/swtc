/*
 * Name:        scrollable.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "scrollable.h"
#include "toolkit.h"
NSView* _w_scrollable_top_view(w_widget *widget) {
	return _W_WIDGET(widget)->handle;
}
wresult _w_scrollable_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	NSScrollView *scrollView =
			(NSScrollView*) _W_SCROLLABLE_PRIV(priv)->scroll_view(widget);
	w_rect *rect = e->rect;
	w_rect *result = e->result;
	if (scrollView != 0) {
		NSSize size;
		NSRect frame;
		size.width = rect->width;
		size.height = rect->height;
		int border =
				priv->has_border(widget, priv) ? NSBezelBorder : NSNoBorder;
		if (MAC_VERSION >= VERSION(10, 7, 0)) {
			// Always include the scroll bar in the trim even when the scroll style is overlay
		} else {
		}
		result->width = (int) size.width;
		result->height = (int) size.height;
		NSView *contentView = NSScrollView_contentView(scrollView);
		NSView_frame(contentView, &frame);
		result->x -= frame.x;
		result->y -= frame.y;
	} else {
		result->x = rect->x;
		result->y = rect->y;
		result->width = rect->width;
		result->height = rect->height;
	}
	return W_TRUE;
}
wresult _w_scrollable_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	NSView *scrollView = _W_SCROLLABLE_PRIV(priv)->scroll_view(widget);
	if (scrollView != 0) {
		return W_FALSE;
	} else {
		NSView *view = priv->get_view(widget);
		NSRect bounds;
		NSView_bounds(view, &bounds);
		e->rect->x = 0;
		e->rect->y = 0;
		e->rect->width = bounds.width;
		e->rect->height = bounds.height;
		return W_TRUE;
	}
}
NSView* _w_scrollable_get_view(w_widget *widget) {
	if (NSScrollView_isKindOfClass(_W_WIDGET(widget)->handle)) {
		return NSScrollView_documentView(
				(NSScrollView*) _W_WIDGET(widget)->handle);
	} else {
		return _W_WIDGET(widget)->handle;
	}
}
wresult _w_scrollable_set_zorder(w_widget *widget, _w_control_priv *priv) {
	wresult result = _w_control_set_zorder(widget, priv);
	/*NSView* scrollView = _W_SCROLLABLE_PRIV(priv)->scroll_view(widget);
	 if(scrollView != 0){
	 NSView* view = priv->get_view(widget);
	 NSScrollView_setDocumentView(NSSCROLLVIEW(scrollView), view);
	 }*/
	return result;
}
NSView* _w_scrollable_scroll_view(w_widget *widget) {
	if (NSScrollView_isKindOfClass(_W_WIDGET(widget)->handle)) {
		return _W_WIDGET(widget)->handle;
	} else {
		return 0;
	}
}
void _w_scrollable_class_init(struct _w_scrollable_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->get_client_area = _w_scrollable_get_client_area;
	_W_WIDGET_PRIV(priv)->compute_trim = _w_scrollable_compute_trim;
	priv->top_view = _w_scrollable_top_view;
	priv->set_zorder = _w_scrollable_set_zorder;
	priv->get_view = _w_scrollable_get_view;
	_W_SCROLLABLE_PRIV(priv)->scroll_view = _w_scrollable_scroll_view;
}
