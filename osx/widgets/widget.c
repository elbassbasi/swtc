/*
 * Name:        widget.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "widget.h"
#include "toolkit.h"
_w_widget_priv* _w_widget_get_priv(w_widget *widget) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(widget);
	while (clazz->toolkit != W_TOOLKIT(mac_toolkit)) {
		clazz = clazz->next_class;
	}
	return _W_WIDGET_PRIV(clazz->reserved[0]);
}
w_widget* _w_widget_find_control_0(NSView *view, int *index) {
	Class clazz = NSObject_class(NSOBJECT(view));
	Class *nsclasses = mac_toolkit->nsclasses;
	for (int i = 0; i < _NS_CLASS_LAST; i++) {
		if (nsclasses[i] == clazz) {
			ns_get_widget funct = mac_toolkit->_ns_get_widget[i];
			if (index != 0)
				*index = i;
			return funct(view);
		}
	}
	return 0;
}
w_widget* _w_widget_find_control(NSView *view) {
	return _w_widget_find_control_0(view, 0);
}
void _w_event_platform_init(_w_event_platform *e, w_widget *widget,
		void *handle, int msg) {
	e->event.widget = widget;
	e->event.type = W_EVENT_PLATFORM;
	e->event.platform_event = 0;
	e->event.time = 0;
	e->event.data = 0;
	e->handle = handle;
	e->msg = msg;
	e->flags = 0;
	e->result._ptr = 0;
	memset(e->args, 0, sizeof(e->args));
}
wresult _w_widget_send_msg(_w_event_platform *e) {
	return _w_widget_send_event(e->event.widget, (w_event*) &e);
}
wuint64 _w_widget_check_bits(wuint64 style, int int0, int int1, int int2,
		int int3, int int4, int int5) {
	int mask = int0 | int1 | int2 | int3 | int4 | int5;
	if ((style & mask) == 0)
		style |= int0;
	if ((style & int0) != 0)
		style = (style & ~mask) | int0;
	if ((style & int1) != 0)
		style = (style & ~mask) | int1;
	if ((style & int2) != 0)
		style = (style & ~mask) | int2;
	if ((style & int3) != 0)
		style = (style & ~mask) | int3;
	if ((style & int4) != 0)
		style = (style & ~mask) | int4;
	if ((style & int5) != 0)
		style = (style & ~mask) | int5;
	return style;
}
wresult _w_widget_is_ok(w_widget *obj) {
	return W_TRUE;
}
wresult _w_widget_dispose(w_widget *obj) {
	obj->clazz = 0;
	return W_TRUE;
}
wresult _w_widget_send_event(w_widget *widget, w_event *event) {
    if(widget == 0) return W_FALSE;
	if (widget->post_event != 0) {
		return widget->post_event(widget, event);
	} else {
		return w_widget_default_post_event(widget, event);
	}
}
wuint64 _w_widget_check_style(w_widget *control, wuint64 style) {
	return style;
}
wresult _w_widget_create_widget(w_widget *widget, _w_control_priv *priv) {
	wresult ret = _W_WIDGET_PRIV(priv)->create_handle(widget, priv);
	if (ret <= 0)
		return ret;
	_W_WIDGET_PRIV(priv)->set_orientation(widget, priv);
	return ret;
}
wresult _w_widget_create_handle(w_widget *widget, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_widget_set_orientation(w_widget *widget, _w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_widget_post_event(w_widget *widget, w_event *ee) {
	_w_widget_priv *priv;
	switch (ee->type) {
	case W_EVENT_PLATFORM: {
		priv = _w_widget_get_priv(widget);
		return priv->msgs[_W_EVENT_PLATFORM(ee)->msg](widget,
				_W_EVENT_PLATFORM(ee), priv);
	}
		break;
	case W_EVENT_COMPUTE_SIZE:
		priv = _w_widget_get_priv(widget);
		return priv->compute_size(widget, (w_event_compute_size*) ee, priv);
		break;
	case W_EVENT_CLIENT_AREA:
		priv = _w_widget_get_priv(widget);
		return priv->get_client_area(widget, (w_event_client_area*) ee, priv);
		break;
	case W_EVENT_COMPUTE_TRIM:
		priv = _w_widget_get_priv(widget);
		return priv->compute_trim(widget, (w_event_compute_trim*) ee, priv);
		break;
	case W_EVENT_LAYOUTDETECT: {
		w_layout *layout;
		w_composite_get_layout(W_COMPOSITE(widget), &layout);
		w_layout_do_layout(layout, W_COMPOSITE(widget), W_TRUE);
		return W_TRUE;
	}
		break;
	}
	return W_FALSE;
}
wresult _w_widget_get_theme(w_widget *widget, w_theme **theme) {
	if (_W_WIDGET(widget)->theme != 0) {
		*theme = _W_WIDGET(widget)->theme;
		return W_TRUE;
	}
	w_toolkit *toolkit = w_widget_get_toolkit(widget);
	*theme = w_toolkit_get_theme(toolkit);
	return W_TRUE;
}
wresult _w_widget_set_theme(w_widget *widget, w_theme *theme) {
	_W_WIDGET(widget)->theme = theme;
	return W_TRUE;
}
/*
 * messages
 */
wresult _w_dispatch_message_null(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*if ((e->flags & _W_EVENT_PLATFORM_INVERSE) == 0) {
		e->flags |= _W_EVENT_PLATFORM_SUPER;
	}*/
	return W_FALSE;
}
wresult _ns_widget_drawRect(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((e->flags & _W_EVENT_PLATFORM_INVERSE) == 0) {
		e->flags |= _W_EVENT_PLATFORM_SUPER;
		NSMutableArray *isPainting = mac_toolkit->isPainting;
		NSMutableArray_addObject(isPainting, NSOBJECT(e->handle));
		NSGraphicsContext *context = NSGraphicsContext_currentContext();
		e->reserved[0] = context;
		NSGraphicsContext_saveGraphicsState(context);
	} else {
		NSGraphicsContext *context = (NSGraphicsContext*) e->reserved[0];
		if (w_widget_is_ok(widget) > 0) {
			/*
			 * Feature in Cocoa. There are widgets that draw outside of the UI thread,
			 * such as the progress bar and default button.  The fix is to draw the
			 * widget but not send paint events.
			 */
            priv->draw_widget(widget, e->handle, context,(NSRect*) e->args[0]._ptr, priv);
		}
		NSGraphicsContext_restoreGraphicsState(context);
		NSMutableArray *isPainting = mac_toolkit->isPainting;
		NSMutableArray_removeObjectIdenticalTo(isPainting, NSOBJECT(e->handle));
	}
	return W_TRUE;
}
wresult _ns_widget_setNeedsDisplay(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((e->flags & _W_EVENT_PLATFORM_INVERSE) == 0) {
		NSMutableArray *isPainting = mac_toolkit->isPainting;
		if (e->args[0]._int != 0
				&& NSArray_containsObject((NSArray*) isPainting,
						NSOBJECT(e->handle))) {
			NSMutableArray *needsDisplay = mac_toolkit->needsDisplay;
			if (needsDisplay == 0) {
				needsDisplay = NSMutableArray_initWithCapacity(12);
				mac_toolkit->needsDisplay = needsDisplay;
			}
			NSMutableArray_addObject(needsDisplay, NSOBJECT(e->handle));
			return W_TRUE;
		}
		e->flags |= _W_EVENT_PLATFORM_SUPER;
	} else {
	}
	return W_TRUE;
}
void _w_widget_class_init(struct _w_widget_class *clazz) {
	clazz->is_ok = _w_widget_is_ok;
	clazz->dispose = _w_widget_dispose;
	clazz->toolkit = (w_toolkit*) mac_toolkit;
	clazz->post_event = _w_widget_post_event;
	clazz->get_theme = _w_widget_get_theme;
	clazz->set_theme = _w_widget_set_theme;
	/*
	 * private
	 */
	_w_widget_priv *priv = W_WIDGET_CLASS(clazz)->reserved[0];
	priv->compute_size = (__compute_size) _w_dispatch_message_null;
	priv->compute_trim = (__compute_trim) _w_dispatch_message_null;
	priv->get_client_area = (__get_client_area) _w_dispatch_message_null;
	priv->check_style = _w_widget_check_style;
	priv->create_widget = _w_widget_create_widget;
	priv->create_handle = _w_widget_create_handle;
	priv->set_orientation = _w_widget_set_orientation;
	/*
	 * messages
	 */
	dispatch_message *msgs = priv->msgs;
	for (int i = 0; i < _NS_LAST_MSG; i++) {
		msgs[i] = _w_dispatch_message_null;
	}
	msgs[_NS_drawRect] = _ns_widget_drawRect;
	msgs[_NS_setNeedsDisplay] = _ns_widget_setNeedsDisplay;
}
/*
 * widgetdata
 */
wresult _w_widgetdata_is_ok(w_widgetdata *obj) {
	return W_TRUE;
}
wresult _w_widgetdata_close(w_widgetdata *obj) {
	return W_TRUE;
}
wresult _w_widgetdata_copy(w_widgetdata *from, w_widgetdata *to) {
	return W_FALSE;
}
wresult _w_widgetdata_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
	return obj1 == obj2;
}
void _w_widgetdata_class_init(struct _w_widgetdata_class *clazz) {
	clazz->is_ok = _w_widgetdata_is_ok;
	clazz->close = _w_widgetdata_close;
	clazz->copy = _w_widgetdata_copy;
	clazz->equals = _w_widgetdata_equals;
	clazz->toolkit = W_TOOLKIT(mac_toolkit);
}
/*
 * item
 */
wresult _w_item_copy(w_widgetdata *from, w_widgetdata *to) {
	_W_WIDGETDATA(to)->clazz = _W_WIDGETDATA(from)->clazz;
	_W_ITEM(to)->parent = _W_ITEM(from)->parent;
	_W_ITEM(to)->index = _W_ITEM(from)->index;
	return W_TRUE;
}
wresult _w_item_get_parent_widget(w_item *item, w_widget **parent) {
	*parent = _W_ITEM(item)->parent;
	return W_TRUE;
}
wresult _w_item_get_data(w_item *item, void **data) {
	*data = 0;
	return W_TRUE;
}
wresult _w_item_get_index(w_item *item) {
	return _W_ITEM(item)->index;
}
wresult _w_item_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_item_set_data(w_item *item, void *data) {
	return W_FALSE;
}
wresult _w_item_set_text(w_item *item, const char *text, int length, int enc) {
	return W_FALSE;
}
void _w_item_class_init(struct _w_item_class *clazz) {
	_w_widgetdata_class_init(W_WIDGETDATA_CLASS(clazz));
	W_WIDGETDATA_CLASS(clazz)->copy = _w_item_copy;
	clazz->get_parent_widget = _w_item_get_parent_widget;
	clazz->get_data = _w_item_get_data;
	clazz->get_index = _w_item_get_index;
	clazz->get_text = _w_item_get_text;
	clazz->set_data = _w_item_set_data;
	clazz->set_text = _w_item_set_text;
}
