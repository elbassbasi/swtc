/*
 * spinner.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "spinner.h"
#include "../widgets/toolkit.h"
wuint64 _w_spinner_check_style(w_widget *control, wuint64 style) {
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_spinner_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	int w = 0, h = 0;
	gtk_widget_realize(handle);
	PangoLayout *layout = gtk_entry_get_layout(GTK_ENTRY(handle));
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(
			GTK_SPIN_BUTTON(handle));
	double upper = gtk_adjustment_get_upper(hAdjustment);
	int digits = gtk_spin_button_get_digits(GTK_SPIN_BUTTON(handle));
	w_rect rect;
	memset(&rect, 0, sizeof(w_rect));
	gtk_widget_realize(handle);
#if GTK3
	gtk_widget_set_size_request(handle, wHint, hHint);
	GtkRequisition requisition;
	gtk_widget_get_preferred_size(handle, &requisition, NULL);
	rect.width = wHint == W_DEFAULT ? requisition.width : wHint;
	rect.height = hHint == W_DEFAULT ? requisition.height : hHint;
#endif
	w_event_compute_trim _e;
	w_rect trim;
	memset(&trim, 0, sizeof(w_rect));
	_e.event.type = W_EVENT_COMPUTE_TRIM;
	_e.event.platform_event = 0;
	_e.event.widget = widget;
	_e.event.time = 0;
	_e.rect = &rect;
	_e.result = &trim;
	_e.event.data = 0;
	priv->widget.compute_trim(widget, &_e, priv);
	e->size->width = trim.width;
	e->size->height = trim.height;
	return W_TRUE;
}
wresult _w_spinner_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	int xborder = 0, yborder = 0;
	_w_scrollable_compute_trim(widget, e, priv);
	w_rect *trim = e->result;
#if GTK3
	GtkBorder tmp;
	GtkStyleContext *context = gtk_widget_get_style_context(handle);
	if (GTK_VERSION < VERSION(3, 18, 0)) {
		gtk_style_context_get_padding(context, GTK_STATE_FLAG_NORMAL, &tmp);
	} else {
		gtk_style_context_get_padding(context,
				gtk_widget_get_state_flags(handle), &tmp);
	}
	if ((_W_WIDGET(widget)->style & W_BORDER) != 0) {
		if (GTK_VERSION < VERSION(3, 18, 0)) {
			gtk_style_context_get_border(context, GTK_STATE_FLAG_NORMAL, &tmp);
		} else {
			gtk_style_context_get_border(context,
					gtk_widget_get_state_flags(handle), &tmp);
		}
		trim->x -= tmp.left;
		trim->y -= tmp.top;
		trim->width += tmp.left + tmp.right;
		trim->height += tmp.top + tmp.bottom;
	}
#endif
	int property = 0;
	gtk_widget_style_get(handle, "interior-focus", &property, NULL);
	if (property == 0) {
		gtk_widget_style_get(handle, "focus-line-width", &property, NULL);
		xborder += property;
		yborder += property;
	}
	trim->x -= xborder;
	trim->y -= yborder;
	trim->width += 2 * xborder;
	trim->height += 2 * yborder;
	GtkBorder innerBorder;
	_w_toolkit_get_entry_inner_border(handle, &innerBorder);
	trim->x -= innerBorder.left;
	trim->y -= innerBorder.top;
	trim->width += innerBorder.left + innerBorder.right;
	trim->height += innerBorder.top + innerBorder.bottom;
	return W_TRUE;
}
wresult _w_spinner_copy(w_spinner *spinner) {
	GtkWidget *handle = _W_WIDGET(spinner)->handle;
	gtk_editable_copy_clipboard(GTK_EDITABLE(handle));
	return W_TRUE;
}
wresult _w_spinner_cut(w_spinner *spinner) {
	GtkWidget *handle = _W_WIDGET(spinner)->handle;
	gtk_editable_cut_clipboard(GTK_EDITABLE(handle));
	return W_TRUE;
}
wresult _w_spinner_create_handle(w_widget *widget, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	GtkWidget *fixedHandle, *handle = 0;
	GtkAdjustment *adjustment;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_MENU;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	adjustment = gtk_adjustment_new(0, 0, 100, 1, 10, 0);
	if (adjustment == 0)
		goto _err;
	_W_SPINNER(widget)->climbRate = 1;
	handle = gtk_spin_button_new(adjustment, _W_SPINNER(widget)->climbRate, 0);
	if (handle == 0)
		goto _err;
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	gtk_editable_set_editable(GTK_EDITABLE(handle), (style & W_READ_ONLY) == 0);
	if (GTK_VERSION <= VERSION(3, 20, 0)) {
		gtk_entry_set_has_frame(GTK_ENTRY(handle), (style & W_BORDER) != 0);
	}
	gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(handle), (style & W_WRAP) != 0);
#if GTK3
	//imContext = imContextLast();
#endif
	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
#if GTK3
	//setFontDescription(defaultFont().handle);
#endif
	_w_widget_set_control(handle, widget);
	_w_widget_set_control(fixedHandle, widget);
	_W_WIDGET(widget)->handle = handle;
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
void _w_spinner_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_composite_hook_events(widget, priv);
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	_w_widget_connect(handle, SIGNAL_CHANGED, 0, TRUE);
	_w_widget_connect(handle, SIGNAL_INSERT_TEXT, 0, FALSE);
	_w_widget_connect(handle, SIGNAL_DELETE_TEXT, 0, FALSE);
	_w_widget_connect(handle, SIGNAL_VALUE_CHANGED, 0, FALSE);
	_w_widget_connect(handle, SIGNAL_ACTIVATE, 0, FALSE);
	_w_widget_connect(handle, SIGNAL_POPULATE_POPUP, 0, FALSE);
	GtkIMContext *imContext = 0;	//imContext();
	if (imContext != 0) {
		_w_widget_connect((GtkWidget*) imContext, SIGNAL_COMMIT, 0, FALSE);
		int id = g_signal_lookup("commit", gtk_im_context_get_type());
		GSignalMatchType mask = G_SIGNAL_MATCH_DATA | G_SIGNAL_MATCH_ID;
		g_signal_handlers_block_matched(imContext, mask, id, 0, 0, 0, handle);
	}
}
wresult _w_spinner_get_digits(w_spinner *spinner) {
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	return gtk_spin_button_get_digits(GTK_SPIN_BUTTON(handle));
}
wresult _w_spinner_get_increment(w_spinner *spinner) {
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(
			GTK_SPIN_BUTTON(handle));
	int digits = gtk_spin_button_get_digits(GTK_SPIN_BUTTON(handle));
	double value = gtk_adjustment_get_step_increment(hAdjustment);
	for (int i = 0; i < digits; i++)
		value *= 10;
	return (int) (value > 0 ? value + 0.5 : value - 0.5);
}
wresult _w_spinner_get_maximum(w_spinner *spinner) {
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(
			GTK_SPIN_BUTTON(handle));
	int digits = gtk_spin_button_get_digits(GTK_SPIN_BUTTON(handle));
	double value = gtk_adjustment_get_upper(hAdjustment);
	for (int i = 0; i < digits; i++)
		value *= 10;
	return (int) (value > 0 ? value + 0.5 : value - 0.5);
}
wresult _w_spinner_get_minimum(w_spinner *spinner) {
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(
			GTK_SPIN_BUTTON(handle));
	int digits = gtk_spin_button_get_digits(GTK_SPIN_BUTTON(handle));
	double value = gtk_adjustment_get_lower(hAdjustment);
	for (int i = 0; i < digits; i++)
		value *= 10;
	return (int) (value > 0 ? value + 0.5 : value - 0.5);
}
wresult _w_spinner_get_page_increment(w_spinner *spinner) {
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(handle);
	int digits = gtk_spin_button_get_digits(handle);
	double value = gtk_adjustment_get_page_increment(hAdjustment);
	for (int i = 0; i < digits; i++)
		value *= 10;
	return (int) (value > 0 ? value + 0.5 : value - 0.5);
}
wresult _w_spinner_get_selection(w_spinner *spinner) {
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(handle);
	int digits = gtk_spin_button_get_digits(handle);
	double value = gtk_adjustment_get_value(hAdjustment);
	for (int i = 0; i < digits; i++)
		value *= 10;
	return (int) (value > 0 ? value + 0.5 : value - 0.5);
}
wresult _w_spinner_get_text(w_spinner *spinner, w_alloc alloc, void *user_data,
		int enc) {
	GtkWidget *handle = _W_WIDGET(spinner)->handle;
	const gchar *str = gtk_entry_get_text(GTK_ENTRY(handle));
	return _gtk_alloc_set_text(alloc, user_data, str, -1, enc);
}
wresult _w_spinner_get_text_limit(w_spinner *spinner) {
	GtkWidget *handle = _W_WIDGET(spinner)->handle;
	int limit = gtk_entry_get_max_length(GTK_ENTRY(handle));
	return limit == 0 ? 0xFFFF : limit;
}
wresult _w_spinner_get_values(w_spinner *spinner, w_spinner_value *value) {
	return W_FALSE;
}
wresult _w_spinner_paste(w_spinner *spinner) {
	GtkWidget *handle = _W_WIDGET(spinner)->handle;
	gtk_editable_paste_clipboard(GTK_EDITABLE(handle));
	return W_TRUE;
}
wresult _w_spinner_set_digits(w_spinner *spinner, int value) {
	if (value < 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	int digits = gtk_spin_button_get_digits(handle);
	if (value == digits)
		return W_TRUE;
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(handle);
	int adjustment_value = gtk_adjustment_get_value(hAdjustment);
	int adjustment_upper = gtk_adjustment_get_upper(hAdjustment);
	int adjustment_lower = gtk_adjustment_get_lower(hAdjustment);
	int adjustment_step_increment = gtk_adjustment_get_step_increment(
			hAdjustment);
	int adjustment_page_increment = gtk_adjustment_get_page_increment(
			hAdjustment);
	int adjustment_page_size = gtk_adjustment_get_page_size(hAdjustment);
	int diff = abs(value - digits);
	int factor = 1;
	for (int i = 0; i < diff; i++)
		factor *= 10;
	if (digits > value) {
		adjustment_value *= factor;
		adjustment_upper *= factor;
		adjustment_lower *= factor;
		adjustment_step_increment *= factor;
		adjustment_page_increment *= factor;
		_W_SPINNER(spinner)->climbRate *= factor;
	} else {
		adjustment_value /= factor;
		adjustment_upper /= factor;
		adjustment_lower /= factor;
		adjustment_step_increment /= factor;
		adjustment_page_increment /= factor;
		_W_SPINNER(spinner)->climbRate /= factor;
	}
	gtk_adjustment_configure(hAdjustment, adjustment_value, adjustment_lower,
			adjustment_upper, adjustment_step_increment,
			adjustment_page_increment, adjustment_page_size);
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	gtk_spin_button_configure(handle, hAdjustment,
	_W_SPINNER(spinner)->climbRate, value);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_spinner_set_increment(w_spinner *spinner, int value) {
	if (value < 1)
		return W_FALSE;
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(handle);
	double page_increment = gtk_adjustment_get_page_increment(hAdjustment);
	double newValue = value;
	int digits = gtk_spin_button_get_digits(handle);
	for (int i = 0; i < digits; i++)
		newValue /= 10;
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	gtk_spin_button_set_increments(handle, newValue, page_increment);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_spinner_set_maximum(w_spinner *spinner, int value) {
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(handle);
	double lower = gtk_adjustment_get_lower(hAdjustment);
	double newValue = value;
	int digits = gtk_spin_button_get_digits(handle);
	for (int i = 0; i < digits; i++)
		newValue /= 10;
	if (newValue < lower)
		return W_TRUE;
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	gtk_spin_button_set_range(handle, lower, newValue);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_spinner_set_minimum(w_spinner *spinner, int value) {
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(handle);
	double upper = gtk_adjustment_get_upper(hAdjustment);
	double newValue = value;
	int digits = gtk_spin_button_get_digits(handle);
	for (int i = 0; i < digits; i++)
		newValue /= 10;
	if (newValue > upper)
		return W_TRUE;
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	gtk_spin_button_set_range(handle, newValue, upper);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_spinner_set_page_increment(w_spinner *spinner, int value) {
	if (value < 1)
		return W_FALSE;
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	GtkAdjustment *hAdjustment = gtk_spin_button_get_adjustment(handle);
	double step_increment = gtk_adjustment_get_step_increment(hAdjustment);
	double newValue = value;
	int digits = gtk_spin_button_get_digits(handle);
	for (int i = 0; i < digits; i++)
		newValue /= 10;
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	gtk_spin_button_set_increments(handle, step_increment, newValue);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_spinner_set_selection(w_spinner *spinner, int value) {
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	double newValue = value;
	int digits = gtk_spin_button_get_digits(handle);
	for (int i = 0; i < digits; i++)
		newValue /= 10;
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	gtk_spin_button_set_value(handle, newValue);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_spinner_set_text_limit(w_spinner *spinner, int limit) {
	if (limit == 0)
		return W_ERROR_CANNOT_BE_ZERO;
	GtkWidget *handle = _W_WIDGET(spinner)->handle;
	gtk_entry_set_max_length(GTK_ENTRY(handle), limit);
	return W_TRUE;
}
wresult _w_spinner_set_values(w_spinner *spinner, w_spinner_value *value) {
	if (value->maximum < value->minimum)
		return W_FALSE;
	if (value->digits < 0)
		return W_FALSE;
	if (value->increment < 1)
		return W_FALSE;
	if (value->pageIncrement < 1)
		return W_FALSE;
	GtkSpinButton *handle = GTK_SPIN_BUTTON(_W_WIDGET(spinner)->handle);
	int selection = WMIN(WMAX (value->minimum, value->selection),
			value->maximum);
	double factor = 1;
	int digits = gtk_spin_button_get_digits(handle);
	for (int i = 0; i < digits; i++)
		factor *= 10;
	g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	gtk_spin_button_set_range(handle, value->minimum / factor,
			value->maximum / factor);
	gtk_spin_button_set_increments(handle, value->increment / factor,
			value->pageIncrement / factor);
	gtk_spin_button_set_value(handle, selection / factor);
	/*
	 * The value of climb-rate indicates the acceleration rate
	 * to spin the value when the button is pressed and hold
	 * on the arrow button. This value should be varied
	 * depending upon the value of digits.
	 */
	_W_SPINNER(spinner)->climbRate = 1.0 / factor;
	GtkAdjustment *adjustment = gtk_spin_button_get_adjustment(handle);
	gtk_spin_button_configure(handle, adjustment,
	_W_SPINNER(spinner)->climbRate, digits);
	g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
void _w_spinner_class_init(struct _w_spinner_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SPINNER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_spinner_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_spinner);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_spinner);
	/*
	 * public
	 */
	clazz->copy = _w_spinner_copy;
	clazz->cut = _w_spinner_cut;
	clazz->get_digits = _w_spinner_get_digits;
	clazz->get_increment = _w_spinner_get_increment;
	clazz->get_maximum = _w_spinner_get_maximum;
	clazz->get_minimum = _w_spinner_get_minimum;
	clazz->get_page_increment = _w_spinner_get_page_increment;
	clazz->get_selection = _w_spinner_get_selection;
	clazz->get_text = _w_spinner_get_text;
	clazz->get_text_limit = _w_spinner_get_text_limit;
	clazz->get_values = _w_spinner_get_values;
	clazz->paste = _w_spinner_paste;
	clazz->set_digits = _w_spinner_set_digits;
	clazz->set_increment = _w_spinner_set_increment;
	clazz->set_maximum = _w_spinner_set_maximum;
	clazz->set_minimum = _w_spinner_set_minimum;
	clazz->set_page_increment = _w_spinner_set_page_increment;
	clazz->set_selection = _w_spinner_set_selection;
	clazz->set_text_limit = _w_spinner_set_text_limit;
	clazz->set_values = _w_spinner_set_values;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.handle_top = _w_widget_hp;
	priv->handle_fixed = _w_widget_hp;
	priv->widget.compute_size = _w_spinner_compute_size;
	priv->widget.compute_trim = _w_spinner_compute_trim;
	priv->widget.check_style = _w_spinner_check_style;
	priv->widget.create_handle = _w_spinner_create_handle;
	priv->widget.hook_events = _w_spinner_hook_events;
	/*
	 * signals
	 */
	_gtk_signal *signals = priv->widget.signals;
	signals[SIGNAL_CLICKED] = _gtk_button_clicked;
}
