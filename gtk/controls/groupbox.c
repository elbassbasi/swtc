/*
 * groupbox.c
 *
 *  Created on: 17 mars 2021
 *      Author: azeddine
 */
#include "groupbox.h"
#include "../widgets/toolkit.h"
wuint64 _w_groupbox_check_style(w_widget *widget, wuint64 style) {
	style |= W_NO_FOCUS;
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_groupbox_create_handle(w_widget *widget, _w_control_priv *priv) {
	GtkWidget *fixedHandle = 0, *handle = 0, *labelHandle = 0, *clientHandle;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_THEME_BACKGROUND;
	wuint64 style = _W_WIDGET(widget)->style;
	fixedHandle = _w_fixed_new(0);
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);

	handle = gtk_frame_new(NULL);
	if (handle == 0)
		goto _err;

	labelHandle = gtk_label_new(NULL);
	if (labelHandle == 0)
		goto _err;
	g_object_ref(labelHandle);
	g_object_ref_sink(labelHandle);

	clientHandle = _w_fixed_new(widget);
	if (clientHandle == 0)
		goto _err;
	/*
	 * Bug 453827 - clientHandle now has it's own window so that
	 * it can listen to events (clicking/tooltip etc.) and so that
	 * background can be drawn on it.
	 */
	gtk_widget_set_has_window(clientHandle, TRUE);
	_w_fixed_set_child(fixedHandle, handle);
	gtk_container_add(GTK_CONTAINER(handle), clientHandle);
	gtk_frame_set_label_widget(GTK_FRAME(handle), labelHandle);
	if ((style & W_SHADOW_IN) != 0) {
		gtk_frame_set_shadow_type(GTK_FRAME(handle), GTK_SHADOW_IN);
	}
	if ((style & W_SHADOW_OUT) != 0) {
		gtk_frame_set_shadow_type(GTK_FRAME(handle), GTK_SHADOW_OUT);
	}
	if ((style & W_SHADOW_ETCHED_IN) != 0) {
		gtk_frame_set_shadow_type(GTK_FRAME(handle), GTK_SHADOW_ETCHED_IN);
	}
	if ((style & W_SHADOW_ETCHED_OUT) != 0) {
		gtk_frame_set_shadow_type(GTK_FRAME(handle), GTK_SHADOW_ETCHED_OUT);
	}
	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
#if GTK3
	//setFontDescription (defaultFont ().handle);
#endif
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(clientHandle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(labelHandle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = clientHandle;
	gtk_widget_show_all(fixedHandle);
	gtk_widget_hide(labelHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_groupbox_get_text(w_groupbox *group, w_alloc alloc, void *user_data,
		int enc) {
	GtkWidget *_handle = _W_WIDGET(group)->handle;
	GtkWidget *handle = gtk_widget_get_parent(_handle);
	GtkWidget *label = gtk_frame_get_label_widget(GTK_FRAME(handle));
	if (label != 0) {
		gchar *s = (gchar*) gtk_label_get_text(GTK_LABEL(label));
		_gtk_alloc_set_text(alloc, user_data, s, -1, enc);
	}
	return W_TRUE;
}
wresult _w_groupbox_set_text(w_groupbox *group, const char *string, int length,
		int enc) {
	int newlength;
	int mnemonic;
	char *s = _gtk_text_fix(string, length, enc, &newlength, &mnemonic);
	if (s != 0) {
		GtkWidget *_handle = _W_WIDGET(group)->handle;
		GtkWidget *handle = gtk_widget_get_parent(_handle);
		GtkWidget *label = gtk_frame_get_label_widget(GTK_FRAME(handle));
		if (newlength != 0) {
			gtk_label_set_text_with_mnemonic(GTK_LABEL(label), s);
			gtk_widget_show(label);
		} else {
			gtk_widget_hide(label);
		}
		// Set the foreground now that the text has been set
		if (GTK_VERSION >= VERSION(3, 16, 0)
				&& _W_CONTROL(group)->foreground != 0) {
			//setForegroundGdkRGBA(label, _W_CONTROL(group)->foreground);
		}
	}
	_gtk_text_free(string, s, newlength);
	return W_TRUE;
}
wresult _w_groupbox_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	_w_composite_get_client_area(widget, e, priv);
	/*
	 * Bug 453827 Child position fix.
	 * SWT's calls to gtk_widget_size_allocate and gtk_widget_set_allocation
	 * causes GTK+ to move the clientHandle's SwtFixed down by the size of the label.
	 * These calls can come up from 'shell' and group has no control over these calls.
	 *
	 * This is an undesired side-effect. Client handle's x & y positions should never
	 * be incremented as this is an internal sub-container.
	 *
	 * Note: 0 by 0 was chosen as 1 by 1 shifts controls beyond their original pos.
	 * The long term fix would be to not use widget_*_allocation from higher containers
	 * like shell and to not use	gtkframe in non-group widgets (e.g used in label atm).
	 */
	e->rect->x = 0;
	e->rect->y = 0;
	return W_TRUE;
}
wresult _w_groupbox_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	w_size sz;
	_w_composite_compute_size(widget, e, priv);
	e->wHint = W_DEFAULT;
	e->hHint = W_DEFAULT;
	sz.width = e->size->width;
	sz.height = e->size->height;
	GtkWidget *_handle = _W_WIDGET(widget)->handle;
	GtkWidget *handle = gtk_widget_get_parent(_handle);
	_w_control_compute_native_size(widget, handle, e, priv);
	e->wHint = wHint;
	e->hHint = hHint;
	e->size->width = WMAX(e->size->width, sz.width);
	e->size->height = sz.height;
	return TRUE;
}
wresult _w_groupbox_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	priv->force_resize(W_CONTROL(widget), priv);
	GtkAllocation allocation;
	GtkWidget *clientHandle = priv->handle_client(widget, priv);
	gtk_widget_get_allocation(clientHandle, &allocation);
	int clientX = allocation.x;
	int clientY = allocation.y;
	e->result->x = e->rect->x - clientX;
	e->result->y = e->rect->y - clientX;
	e->result->width = e->rect->width + clientX + clientX;
	e->result->height = e->rect->height + clientX + clientY;
	return TRUE;
}
wresult _w_groupbox_dispose_class(struct _w_widget_class *clazz){
	return W_TRUE;
}
void _w_groupbox_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_groupbox_class *clazz) {
	if (classId == _W_CLASS_GROUPBOX) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&gtk_toolkit->class_groupbox_priv;
	}
	_w_composite_class_init(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_GROUPBOX;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_groupbox_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_groupbox);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_groupbox);
	/*
	 * public function
	 */
	W_WIDGET_CLASS(clazz)->dispose_class = _w_groupbox_dispose_class;
	clazz->get_text = _w_groupbox_get_text;
	clazz->set_text = _w_groupbox_set_text;
	/*
	 * priv
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_GROUPBOX) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		_W_WIDGET_PRIV(priv)->handle_top = _w_widget_hpp;
		_W_WIDGET_PRIV(priv)->check_style = _w_groupbox_check_style;
		priv->handle_fixed = _w_widget_hpp;
		priv->handle_client = _w_widget_h;
		_W_COMPOSITE_PRIV(priv)->handle_parenting = _w_widget_h;
		_W_WIDGET_PRIV(priv)->create_handle = _w_groupbox_create_handle;
		_W_WIDGET_PRIV(priv)->get_client_area = _w_groupbox_get_client_area;
		_W_WIDGET_PRIV(priv)->compute_trim = _w_groupbox_compute_trim;
		_W_WIDGET_PRIV(priv)->compute_size = _w_groupbox_compute_size;
	}
}

