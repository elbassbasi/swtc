/*
 * progressbar.c
 *
 *  Created on: 14 mars 2021
 *      Author: azeddine
 */
#include "progressbar.h"
#include "../widgets/toolkit.h"
wresult _w_progressbar_update(w_progressbar *progressbar) {
	/*
	 * Bug in GTK.  When a progress bar has been unrealized after being
	 * realized at least once, gtk_progress_bar_set_fraction() GP's.  The
	 * fix is to update the progress bar state only when realized and restore
	 * the state when the progress bar becomes realized.
	 */
	if (!gtk_widget_get_realized(_W_WIDGET(progressbar)->handle))
		return W_FALSE;

	double fraction = (double) _W_PROGRESSBAR(progressbar)->selection
			/ _W_PROGRESSBAR(progressbar)->maximum;
	gtk_progress_bar_set_fraction(
			GTK_PROGRESS_BAR(_W_WIDGET(progressbar)->handle), fraction);
#if GTK3
#else
#endif
	return W_TRUE;
}
int _w_progressbar_get_maximum(w_progressbar *progressbar) {
	return _W_PROGRESSBAR(progressbar)->maximum;
}
int _w_progressbar_get_selection(w_progressbar *progressbar) {
	return _W_PROGRESSBAR(progressbar)->selection;
}
int _w_progressbar_get_state(w_progressbar *progressbar) {
	return W_NORMAL;
}
wresult _w_progressbar_set_maximum(w_progressbar *progressbar, int value) {
	if (value <= 0)
		value = 1;
	_W_PROGRESSBAR(progressbar)->maximum = value;
	_W_PROGRESSBAR(progressbar)->selection = MIN(
			_W_PROGRESSBAR(progressbar)->selection, value);
	return _w_progressbar_update(progressbar);
}
wresult _w_progressbar_set_selection(w_progressbar *progressbar, int value) {
	if (value < 0)
		value = 0;
	_W_PROGRESSBAR(progressbar)->selection = MIN(
			_W_PROGRESSBAR(progressbar)->maximum, value);
	return _w_progressbar_update(progressbar);
}
wresult _w_progressbar_set_state(w_progressbar *progressbar, int state) {
	return W_ERROR_NOT_IMPLEMENTED;
}
gboolean _w_progressbar_timer_fn(gpointer widget) {
	if (w_control_is_visible(W_CONTROL(widget)))
		gtk_progress_bar_pulse(GTK_PROGRESS_BAR(_W_WIDGET(widget)->handle));
	return 1;
}
wresult _w_progressbar_create_handle(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	GtkWidget *fixedHandle, *handle = 0;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	handle = gtk_progress_bar_new();
	if (handle == 0)
		goto _err;
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	wuint64 style = _W_WIDGET(widget)->style;
	GtkOrientation orientation =
			(style & W_VERTICAL) != 0 ?
					GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL;
	gtk_orientable_set_orientation(GTK_ORIENTABLE(handle), orientation);
	if ((style & W_INDETERMINATE) != 0) {
		_W_PROGRESSBAR(widget)->timerId = g_timeout_add(100,
				_w_progressbar_timer_fn, widget);
	}
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = handle;
	_W_PROGRESSBAR(widget)->maximum = 100;
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
gboolean _gtk_progressbar_realize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_realize(widget, e, priv);
	if (result != 0)
		return result;
	/*
	 * Bug in GTK.  When a progress bar has been unrealized after being
	 * realized at least once, gtk_progress_bar_set_fraction() GP's.  The
	 * fix is to update the progress bar state only when realized and restore
	 * the state when the progress bar becomes realized.
	 */
	_w_progressbar_update(W_PROGRESSBAR(widget));
	return 0;
}
wresult _w_progressbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	return _w_control_compute_size(widget, e, priv);
}
void _w_progressbar_class_init(struct _w_progressbar_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_PROGRESSBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_progressbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_progressbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_progressbar);
	/*
	 * public function
	 */
	clazz->get_maximum = _w_progressbar_get_maximum;
	clazz->get_selection = _w_progressbar_get_selection;
	clazz->get_state = _w_progressbar_get_state;
	clazz->set_maximum = _w_progressbar_set_maximum;
	clazz->set_selection = _w_progressbar_set_selection;
	clazz->set_state = _w_progressbar_set_state;
	/*
	 * priv
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->handle_fixed = _w_widget_hp;
	priv->widget.handle_top = _w_widget_hp;
	priv->widget.create_handle = _w_progressbar_create_handle;
	priv->widget.compute_size = _w_progressbar_compute_size;
	/*
	 * signal
	 */
	_gtk_signal *signals = priv->widget.signals;
	signals[SIGNAL_REALIZE] = _gtk_progressbar_realize;
}

