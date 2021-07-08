/*
 * slider.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "slider.h"
#include "../widgets/toolkit.h"
wuint64 _w_slider_check_style(w_widget *widget, wuint64 style) {
	return _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
}
wresult _w_slider_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	gtk_widget_realize(handle);
	wresult result = _w_control_compute_native_size(widget, handle, e, priv);
	if ((style & W_HORIZONTAL) != 0) {
		if (wHint == W_DEFAULT)
			e->size->width *= 2;
	} else {
		if (hHint == W_DEFAULT)
			e->size->height *= 2;
	}
	return result;
}
wresult _w_slider_create_handle(w_widget *widget, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	GtkWidget *fixedHandle, *handle = 0;
	GtkAdjustment *hAdjustment = 0;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_THEME_BACKGROUND;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	hAdjustment = gtk_adjustment_new(0, 0, 100, 1, 10, 0);
	if (hAdjustment == 0)
		goto _err;
	GtkOrientation orientation =
			(style & W_HORIZONTAL) != 0 ?
					GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL;
	if ((style & W_SCALE) != 0) {
		handle = gtk_scale_new(orientation, hAdjustment);
		gtk_scale_set_digits(GTK_SCALE(handle), 0);
		gtk_scale_set_draw_value(GTK_SCALE(handle), FALSE);
	} else {
		handle = gtk_scrollbar_new(orientation, hAdjustment);
	}
	if (handle == 0)
		goto _err;
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	_w_widget_set_control(handle, widget);
	_w_widget_set_control(fixedHandle, widget);
	_W_WIDGET(widget)->handle = handle;
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_slider_get_increment(w_slider *slider) {
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	GtkAdjustment *hAdjustment = gtk_range_get_adjustment(GTK_RANGE(handle));
	return gtk_adjustment_get_step_increment(hAdjustment);
}
wresult _w_slider_get_maximum(w_slider *slider) {
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	GtkAdjustment *hAdjustment = gtk_range_get_adjustment(GTK_RANGE(handle));
	return gtk_adjustment_get_upper(hAdjustment);
}
wresult _w_slider_get_minimum(w_slider *slider) {
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	GtkAdjustment *hAdjustment = gtk_range_get_adjustment(GTK_RANGE(handle));
	return gtk_adjustment_get_lower(hAdjustment);
}
wresult _w_slider_get_page_increment(w_slider *slider) {
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	GtkAdjustment *hAdjustment = gtk_range_get_adjustment(GTK_RANGE(handle));
	return gtk_adjustment_get_page_increment(hAdjustment);
}
wresult _w_slider_get_selection(w_slider *slider) {
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	GtkAdjustment *hAdjustment = gtk_range_get_adjustment(GTK_RANGE(handle));
	return gtk_adjustment_get_value(hAdjustment);
}
wresult _w_slider_get_thumb(w_slider *slider) {
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	GtkAdjustment *adjustmentHandle = gtk_range_get_adjustment(
			GTK_RANGE(handle));
	return gtk_adjustment_get_page_size(adjustmentHandle);
}
wresult _w_slider_get_values(w_slider *slider, w_slider_value *value) {
	return W_FALSE;
}
void _w_slider_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_control_hook_events(widget, priv);
	_w_slider_priv *cpriv = (_w_slider_priv*) priv;
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	if (_W_WIDGET(widget)->style & W_SCALE) {
		_w_widget_connect(handle, &cpriv->signals[2], FALSE);
	} else {
		_w_widget_connect(handle, &cpriv->signals[0], FALSE);
		_w_widget_connect(handle, &cpriv->signals[1], FALSE);
	}
}
wresult _w_slider_set_increment(w_slider *slider, int value) {
	if (value < 1)
		return W_FALSE;
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	gtk_range_set_increments(GTK_RANGE(handle), value,
			_w_slider_get_page_increment(slider));
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_slider_set_maximum(w_slider *slider, int value) {
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		int minimum = _w_slider_get_minimum(slider);
		if (value <= minimum)
			return W_FALSE;
		g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
				(void*) SIGNAL_VALUE_CHANGED);
		gtk_range_set_range(GTK_RANGE(handle), minimum, value);
		g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
				0, (void*) SIGNAL_VALUE_CHANGED);
	} else {
		GtkAdjustment *adjustmentHandle = gtk_range_get_adjustment(
				GTK_RANGE(handle));
		GtkAdjustment adjustment;
		int minimum = gtk_adjustment_get_lower(adjustmentHandle);
		if (value <= minimum)
			return W_FALSE;
		int page_size = gtk_adjustment_get_page_size(adjustmentHandle);
		int _value = gtk_adjustment_get_value(adjustmentHandle);
		int upper = value;
		page_size = WMIN(page_size, value - minimum);
		_value = WMIN((int ) _value, (int ) (value - page_size));
		int step_increment = gtk_adjustment_get_step_increment(
				adjustmentHandle);
		int page_increment = gtk_adjustment_get_page_increment(
				adjustmentHandle);
		g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA,
				0, 0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
		gtk_adjustment_configure(adjustmentHandle, _value, minimum, upper,
				step_increment, page_increment, page_size);
		g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA,
				0, 0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	}
	return W_TRUE;
}
wresult _w_slider_set_minimum(w_slider *slider, int value) {
	if (value < 0)
		return W_FALSE;
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		int maximum = _w_slider_get_maximum(slider);
		if (value >= maximum)
			return W_FALSE;
		GtkWidget *handle = _W_WIDGET(slider)->handle;
		g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
				(void*) SIGNAL_VALUE_CHANGED);
		gtk_range_set_range(GTK_RANGE(handle), value, maximum);
		g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
				0, (void*) SIGNAL_VALUE_CHANGED);
	} else {
		GtkAdjustment *adjustmentHandle = gtk_range_get_adjustment(
				GTK_RANGE(handle));
		int maximum = (int) gtk_adjustment_get_upper(adjustmentHandle);
		if (value >= maximum)
			return W_FALSE;
		double adjustment_lower = value;
		double adjustment_page_size = WMIN(
				(int ) gtk_adjustment_get_page_size(adjustmentHandle),
				maximum - value);
		double adjustment_value = WMAX(
				(int ) gtk_adjustment_get_value(adjustmentHandle), value);
		g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA,
				0, 0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
		gtk_adjustment_configure(adjustmentHandle, adjustment_value,
				adjustment_lower, maximum,
				gtk_adjustment_get_step_increment(adjustmentHandle),
				gtk_adjustment_get_page_increment(adjustmentHandle),
				adjustment_page_size);
		g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA,
				0, 0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	}
	return W_TRUE;
}
wresult _w_slider_set_page_increment(w_slider *slider, int value) {
	if (value < 1)
		return W_FALSE;
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	GtkAdjustment *hAdjustment = gtk_range_get_adjustment(GTK_RANGE(handle));
	int increment = gtk_adjustment_get_step_increment(hAdjustment);
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	gtk_range_set_increments(GTK_RANGE(handle), increment, value);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_slider_set_selection(w_slider *slider, int value) {
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	gtk_range_set_value(GTK_RANGE(handle), value);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_slider_set_thumb(w_slider *slider, int value) {
	if (_W_WIDGET(slider)->style & W_SCALE)
		return W_FALSE;
	if (value < 1)
		return W_FALSE;
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	GtkAdjustment *adjustmentHandle = gtk_range_get_adjustment(
			GTK_RANGE(handle));
	double adjustment_upper = gtk_adjustment_get_upper(adjustmentHandle);
	double adjustment_lower = gtk_adjustment_get_lower(adjustmentHandle);
	value = (int) WMIN(value, (int ) (adjustment_upper - adjustment_lower));
	double adjustment_page_size = (double) value;
	double adjustment_value = WMIN(
			(int ) gtk_adjustment_get_value(adjustmentHandle),
			(int ) (adjustment_upper - value));
	g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0, 0,
			0, 0, (void*) SIGNAL_VALUE_CHANGED);
	gtk_adjustment_configure(adjustmentHandle, adjustment_value,
			adjustment_lower, adjustment_upper,
			gtk_adjustment_get_step_increment(adjustmentHandle),
			gtk_adjustment_get_page_increment(adjustmentHandle),
			adjustment_page_size);
	g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0,
			0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_slider_set_values(w_slider *slider, w_slider_value *values) {
	if (values->minimum < 0)
		return W_FALSE;
	if (values->maximum < 0)
		return W_FALSE;
	if (values->thumb < 1)
		return W_FALSE;
	if (values->increment < 1)
		return W_FALSE;
	if (values->pageIncrement < 1)
		return W_FALSE;
	GtkWidget *handle = _W_WIDGET(slider)->handle;
	GtkAdjustment *adjustmentHandle = gtk_range_get_adjustment(
			GTK_RANGE(handle));
	int thumb = WMIN(values->thumb, values->maximum - values->minimum);
	g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0, 0,
			0, 0, (void*) SIGNAL_VALUE_CHANGED);
	gtk_adjustment_configure(adjustmentHandle,
			WMIN(WMAX(values->selection, values->minimum),
					values->maximum - thumb), values->minimum, values->maximum,
			values->increment, values->pageIncrement, thumb);
	gtk_adjustment_value_changed(adjustmentHandle);
	g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0,
			0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
/*
 * signals
 */
gboolean _gtk_slider_value_changed(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event event;
	memset(&event, 0, sizeof(event));
	event.type = W_EVENT_SELECTION;
	event.platform_event = _EVENT_PLATFORM(e);
	event.widget = widget;
	_w_widget_send_event(widget, &event,W_EVENT_SEND);
	return FALSE;
}
_gtk_signal_info _gtk_slider_signal_lookup[3] = { //
		{ SIGNAL_VALUE_CHANGED, 2, "value-changed" }, //
				{ SIGNAL_CHANGE_VALUE, 3, "change-value" }, //
				{ SIGNAL_VALUE_CHANGED, 2, "value-changed" }, //
		};
void _w_slider_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_slider_class *clazz) {
	if (classId == _W_CLASS_SLIDER) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&gtk_toolkit->class_slider_priv;
	}
	_w_control_class_init(toolkit, classId,W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SLIDER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_slider_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_slider);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_slider);
	/*
	 * public function
	 */
	clazz->get_increment = _w_slider_get_increment;
	clazz->get_maximum = _w_slider_get_maximum;
	clazz->get_minimum = _w_slider_get_minimum;
	clazz->get_page_increment = _w_slider_get_page_increment;
	clazz->get_selection = _w_slider_get_selection;
	clazz->set_increment = _w_slider_set_increment;
	clazz->set_maximum = _w_slider_set_maximum;
	clazz->set_minimum = _w_slider_set_minimum;
	clazz->set_page_increment = _w_slider_set_page_increment;
	clazz->set_selection = _w_slider_set_selection;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_SLIDER) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->widget.handle_top = _w_widget_hp;
		priv->handle_fixed = _w_widget_hp;
		priv->widget.compute_size = _w_slider_compute_size;
		priv->widget.check_style = _w_slider_check_style;
		priv->widget.create_handle = _w_slider_create_handle;
		priv->widget.hook_events = _w_slider_hook_events;
		_w_widget_init_signal(_W_SLIDER_PRIV(priv)->signals,
				_gtk_slider_signal_lookup, 3);
		/*
		 * signals
		 */
		_gtk_signal_fn *signals = priv->widget.signals;
		signals[SIGNAL_VALUE_CHANGED] = _gtk_slider_value_changed;
	}
}
