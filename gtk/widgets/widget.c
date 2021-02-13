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
	while (clazz->toolkit != W_TOOLKIT(gtk_toolkit)) {
		clazz = clazz->next_class;
	}
	return _W_WIDGET_PRIV(clazz->reserved[0]);
}
void _w_widget_set_control(void *handle, w_widget *widget) {
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[SWT_GQUARK_WIDGET],
			widget);
}
w_widget* _w_widget_find_control(void *handle) {
	return (w_widget*) g_object_get_qdata(G_OBJECT(handle),
			gtk_toolkit->quark[SWT_GQUARK_WIDGET]);
}
GtkWidget* _w_widget_h0(w_widget *widget, _w_control_priv *priv) {
	return 0;
}
GtkWidget* _w_widget_h(w_widget *widget, _w_control_priv *priv) {
	return _W_WIDGET(widget)->handle;
}
GtkWidget* _w_widget_hp(w_widget *widget, _w_control_priv *priv) {
	return gtk_widget_get_parent(_W_WIDGET(widget)->handle);
}
GtkWidget* _w_widget_hpp(w_widget *widget, _w_control_priv *priv) {
	return gtk_widget_get_parent(
			gtk_widget_get_parent(_W_WIDGET(widget)->handle));
}
GtkWidget* _w_widget_hppp(w_widget *widget, _w_control_priv *priv) {
	return gtk_widget_get_parent(
			gtk_widget_get_parent(
					gtk_widget_get_parent(_W_WIDGET(widget)->handle)));
}
void _w_widget_check_open(w_widget *widget, _w_control_priv *priv) {

}
void _w_widget_check_orientation(w_widget *widget, w_widget *parent,
		_w_control_priv *priv) {
	_W_WIDGET(widget)->style &= ~W_MIRRORED;
	if ((_W_WIDGET(widget)->style & (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT)) == 0) {
		if (_W_CONTROL(widget)->parent != 0) {
			if ((_W_WIDGET(_W_CONTROL(widget)->parent)->style & W_LEFT_TO_RIGHT)
					!= 0)
				_W_WIDGET(widget)->style |= W_LEFT_TO_RIGHT;
			if ((_W_WIDGET(_W_CONTROL(widget)->parent)->style & W_RIGHT_TO_LEFT)
					!= 0)
				_W_WIDGET(widget)->style |= W_RIGHT_TO_LEFT;
		}
	}
	_W_WIDGET(widget)->style = _w_widget_check_bits(_W_WIDGET(widget)->style,
			W_LEFT_TO_RIGHT, W_RIGHT_TO_LEFT, 0, 0, 0, 0);
}
wuint64 _w_widget_check_style(w_widget *control, wuint64 style) {
	return style;
}
wresult _w_widget_create_widget(w_widget *widget, _w_control_priv *priv) {
	wresult ret = _W_WIDGET_PRIV(priv)->create_handle(widget, priv);
	if (ret < 0)
		return ret;
	_W_WIDGET_PRIV(priv)->set_orientation(widget, W_TRUE, priv);
	_W_WIDGET_PRIV(priv)->hook_events(widget, priv);
	return ret;
}
wresult _w_widget_create_handle(w_widget *widget, _w_control_priv *priv) {
	return W_TRUE;
}
void _w_widget_set_orientation(w_widget *widget, int create,
		_w_control_priv *priv) {
}
void _w_widget_hook_events(w_widget *widget, _w_control_priv *priv) {
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
wresult _w_widget_send_event(w_widget *widget, w_event *event) {
	if (widget->post_event != 0) {
		return widget->post_event(widget, event);
	} else {
		return w_widget_default_post_event(widget, event);
	}
}
/*
 * signals
 */
wresult _w_widget_post_event(w_widget *widget, w_event *ee) {
	_w_widget_priv *priv;
	switch (ee->type) {
	case W_EVENT_PLATFORM: {
		priv = _W_WIDGET_GET_PRIV(widget);
		_w_event_platform *e = _W_EVENT_PLATFORM(ee);
		e->result = priv->signals[e->msg](widget, e, _W_CONTROL_PRIV(priv));
		return W_TRUE;
	}
		break;
	case W_EVENT_COMPUTE_SIZE:
		priv = _W_WIDGET_GET_PRIV(widget);
		return priv->compute_size(widget, (w_event_compute_size*) ee,
				_W_CONTROL_PRIV(priv));
		break;
	case W_EVENT_CLIENT_AREA:
		priv = _W_WIDGET_GET_PRIV(widget);
		return priv->get_client_area(widget, (w_event_client_area*) ee,
				_W_CONTROL_PRIV(priv));
		break;
	case W_EVENT_COMPUTE_TRIM:
		priv = _W_WIDGET_GET_PRIV(widget);
		return priv->compute_trim(widget, (w_event_compute_trim*) ee,
				_W_CONTROL_PRIV(priv));
		break;
	case W_EVENT_LAYOUTDETECT: {
		w_layout *layout;
		w_composite_get_layout(W_COMPOSITE(widget), &layout);
		w_layout_do_layout(layout, W_COMPOSITE(widget), W_FALSE);
		return W_TRUE;
	}
		break;
	}
	return W_FALSE;
}
gboolean _w_widget_proc(GtkWidget *widget, int msg, void *args0, void *args1,
		void *args2) {
	w_widget *cc = (w_widget*) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[0]);
	if (cc == 0)
		return FALSE;
	else {
		struct _w_event_platform e;
		e.event.type = W_EVENT_PLATFORM;
		e.event.widget = cc;
		e.event.platform_event = 0;
		e.event.data = 0;
		e.msg = msg;
		e.result = FALSE;
		e.widget = widget;
		e.args[0] = args0;
		e.args[1] = args1;
		e.args[2] = args2;
		_w_widget_send_event(cc, (w_event*) &e);
		return e.result;
	}
}
gboolean _w_widget_proc_2(GtkWidget *widget, void *msg) {
	return _w_widget_proc(widget, (intptr_t) msg, 0, 0, 0);
}
gboolean _w_widget_proc_3(GtkWidget *widget, void *args0, void *msg) {
	return _w_widget_proc(widget, (intptr_t) msg, args0, 0, 0);
}
gboolean _w_widget_proc_4(GtkWidget *widget, void *args0, void *args1,
		void *msg) {
	return _w_widget_proc(widget, (intptr_t) msg, args0, args1, 0);
}
gboolean _w_widget_proc_5(GtkWidget *widget, void *args0, void *args1,
		void *args2, void *msg) {
	return _w_widget_proc(widget, (intptr_t) msg, args0, args1, args2);
}
wuchar _gtk_signal_lookup[] = { //
		SIGNAL_BUTTON_PRESS_EVENT, //
				SIGNAL_DESTROY, //
				SIGNAL_GRAB_FOCUS, //
				SIGNAL_HIDE, //
				SIGNAL_MAP, //
				SIGNAL_POPUP_MENU, //
				SIGNAL_REALIZE, //
				SIGNAL_SHOW, //
				SIGNAL_UNMAP, //
				SIGNAL_UNREALIZE, //
				SIGNAL_BUTTON_RELEASE_EVENT, //
				SIGNAL_CONFIGURE_EVENT, //
				SIGNAL_DELETE_EVENT, //
				SIGNAL_ENTER_NOTIFY_EVENT, //
				SIGNAL_EVENT_AFTER, //
				SIGNAL_EXPOSE_EVENT, //
				SIGNAL_EXPOSE_EVENT_INVERSE, //
				SIGNAL_DRAW, //
				SIGNAL_FOCUS, //
				SIGNAL_FOCUS_IN_EVENT, //
				SIGNAL_FOCUS_OUT_EVENT, //
				SIGNAL_KEY_PRESS_EVENT, //
				SIGNAL_KEY_RELEASE_EVENT, //
				SIGNAL_LEAVE_NOTIFY_EVENT, //
				SIGNAL_MAP_EVENT, //
				SIGNAL_MNEMONIC_ACTIVATE, //
				SIGNAL_MOTION_NOTIFY_EVENT, //
				SIGNAL_SCROLL_EVENT, //
				SIGNAL_SHOW_HELP, //
				SIGNAL_SIZE_ALLOCATE, //
				SIGNAL_STYLE_SET, //
				SIGNAL_UNMAP_EVENT, //
				SIGNAL_WINDOW_STATE_EVENT, //
				SIGNAL_DRAG_DATA_GET, //
				SIGNAL_DRAG_MOTION, //
				SIGNAL_DRAG_DROP, //
				SIGNAL_DRAG_LEAVE, //
				SIGNAL_DRAG_DATA_RECEIVED, //
				SIGNAL_DRAG_END, //
				SIGNAL_DRAG_DATA_DELETE };
void _w_widget_connect(GtkWidget *widget, wushort signal, wushort signal_id,
		gboolean after) {
	if (gtk_toolkit->closures[signal] == 0) {
		GCallback callback;
		if (signal >= SIGNAL_5_ARGS) {
			callback = (GCallback) _w_widget_proc_5;
		} else if (signal >= SIGNAL_4_ARGS) {
			callback = (GCallback) _w_widget_proc_4;
		} else if (signal >= SIGNAL_3_ARGS) {
			callback = (GCallback) _w_widget_proc_3;
		} else if (signal >= SIGNAL_2_ARGS) {
			callback = (GCallback) _w_widget_proc_2;
		}
		gtk_toolkit->closures[signal] = g_cclosure_new(callback,
				(gpointer) (long) signal, 0);
		if (gtk_toolkit->closures[signal] == 0)
			return;
	}
	if (signal_id == 0) {
		if (gtk_toolkit->signal_id[_gtk_signal_lookup[0]] == 0) {
			const int _gtk_signal_lookup_length = sizeof(_gtk_signal_lookup)
					/ sizeof(_gtk_signal_lookup[0]);
			for (int i = 0; i < _gtk_signal_lookup_length; i++) {
				_gtk_signal_type _signal = _gtk_signal_lookup[i];
				const char *_name = _gtk_signal_names[_signal];
				gtk_toolkit->signal_id[_signal] = g_signal_lookup(_name,
				GTK_TYPE_WIDGET);
			}
		}
		signal_id = gtk_toolkit->signal_id[signal];
	}
	g_signal_connect_closure_by_id(widget, signal_id, 0,
			gtk_toolkit->closures[signal], after);
}
gboolean _gtk_signal_null(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return FALSE;
}
gboolean _gtk_widget_expose_inverse(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (GTK_IS_CONTAINER(e->widget)) {
		return _W_WIDGET_PRIV(priv)->signals[SIGNAL_DRAW](widget, e, priv);
	}
	return FALSE;
}
gboolean _gtk_widget_expose(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (!GTK_IS_CONTAINER(e->widget)) {
		return _W_WIDGET_PRIV(priv)->signals[SIGNAL_DRAW](widget, e, priv);
	}
	return FALSE;
}
wresult _w_widget_dispose(w_widget *widget) {
	_w_widget_priv *priv = _W_WIDGET_GET_PRIV(widget);
	GtkWidget *topHandle = priv->handle_top(W_WIDGET(widget),
			_W_CONTROL_PRIV(priv));
	gtk_widget_destroy(topHandle);
	widget->clazz = 0;
	return W_TRUE;
}
void _w_widget_class_init(struct _w_widget_class *clazz) {
	clazz->is_ok = _w_widget_is_ok;
	clazz->dispose = _w_widget_dispose;
	clazz->post_event = _w_widget_post_event;
	clazz->toolkit = W_TOOLKIT(gtk_toolkit);
	/*
	 * private
	 */
	_w_widget_priv *priv = _W_WIDGET_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	if (priv != 0) {
		priv->check_open = _w_widget_check_open;
		priv->check_orientation = _w_widget_check_orientation;
		priv->check_style = _w_widget_check_style;
		priv->create_handle = _w_widget_create_handle;
		priv->create_widget = _w_widget_create_widget;
		priv->set_orientation = _w_widget_set_orientation;
		priv->hook_events = _w_widget_hook_events;
		/*
		 * signals
		 */
		for (int i = 0; i < SIGNAL_LAST; i++) {
			priv->signals[i] = _gtk_signal_null;
		}
		priv->compute_size = (__compute_size ) _gtk_signal_null;
		priv->get_client_area = (__get_client_area) _gtk_signal_null;
		priv->compute_trim = (__compute_trim) _gtk_signal_null;
		priv->signals[SIGNAL_EXPOSE_EVENT] = _gtk_widget_expose;
		priv->signals[SIGNAL_EXPOSE_EVENT_INVERSE] = _gtk_widget_expose_inverse;
	}
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
	clazz->toolkit = W_TOOLKIT(gtk_toolkit);
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
