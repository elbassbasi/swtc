/*
 * label.c
 *
 *  Created on: 21 févr. 2021
 *      Author: azeddine
 */
#include "label.h"
#include "../widgets/toolkit.h"
wresult _w_label_get_image(w_label *label, w_image *image) {
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_FALSE;
	_w_widget_handles handles;
	_w_widget_get_handles( _W_WIDGET(label)->handle, &handles);
	GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(handles.image));
	_W_IMAGE(image)->pixbuf = pixbuf;
	return W_TRUE;
}
wresult _w_label_get_text(w_label *label, w_alloc string, void *user_data,
		int enc) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_TRUE;
	_w_widget_handles handles;
	_w_widget_get_handles( _W_WIDGET(label)->handle, &handles);
	if (handles.label != 0) {
		const char *text = gtk_label_get_text(GTK_LABEL(handles.label));
		_gtk_alloc_set_text(string, user_data, text, -1, enc);
	}
	return W_TRUE;
}
wresult _w_label_set_image(w_label *label, w_image *image) {
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_FALSE;
	_w_widget_handles handles;
	_w_widget_get_handles( _W_WIDGET(label)->handle, &handles);
	if (image != 0) {
		GdkPixbuf *pixbuf = _W_IMAGE(image)->pixbuf;
		gtk_image_set_from_pixbuf(GTK_IMAGE(handles.image), pixbuf);
		gtk_widget_hide(handles.label);
		gtk_widget_show(handles.image);
	} else {
		gtk_image_set_from_pixbuf(GTK_IMAGE(handles.image), 0);
		gtk_widget_show(handles.label);
		gtk_widget_hide(handles.image);
	}
	return W_TRUE;
}
wresult _w_label_set_text(w_label *label, const char *text, int length,
		int enc) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_TRUE;
	int newlength, mnemonic;
	_w_widget_handles handles;
	_w_widget_get_handles( _W_WIDGET(label)->handle, &handles);
	if (handles.label != 0) {
		char *s = _gtk_text_fix(text, length, enc, &newlength, &mnemonic);
		if (s != 0) {
			gtk_label_set_text_with_mnemonic(GTK_LABEL(handles.label), s);
			gtk_widget_show(handles.label);
		}
		_gtk_text_free(text, s, newlength);
	}
	//_setAlignment (style);
	return W_TRUE;
}
wuint64 _w_label_check_style(w_widget *widget, wuint64 style) {
	style |= W_NO_FOCUS;
	if ((style & W_SEPARATOR) != 0) {
		style = _w_widget_check_bits(style, W_VERTICAL, W_HORIZONTAL, 0, 0, 0,
				0);
		return _w_widget_check_bits(style, W_SHADOW_OUT, W_SHADOW_IN,
				W_SHADOW_NONE, 0, 0, 0);
	}
	return _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
}
GtkWidget* _w_label_fixed_handle(w_widget *widget, _w_control_priv *priv) {
	if (_W_WIDGET(widget)->style & W_BORDER) {
		return _w_widget_hpp(widget, priv);
	} else {
		return _w_widget_hp(widget, priv);
	}
}
void gtk_label_set_align(GtkWidget *labelHandle, float xalign, float yalign) {
	gtk_label_set_xalign(GTK_LABEL(labelHandle), xalign);
	gtk_label_set_yalign(GTK_LABEL(labelHandle), yalign);
}

void _w_label_set_alignment_0(w_label *label) {
	_w_widget_handles handles;
	_w_widget_get_handles(_W_WIDGET(label)->handle, &handles);
	if ((_W_WIDGET(label)->style & W_LEFT) != 0) {
		if (GTK_VERSION >= VERSION(3, 16, 0)) {
			gtk_widget_set_align(handles.label, GTK_ALIGN_START,
					GTK_ALIGN_START); //Aligns widget
			gtk_label_set_align(handles.label, 0.0, 0.0); //Aligns text inside the widget.
			gtk_widget_set_align(handles.image, GTK_ALIGN_START,
					GTK_ALIGN_CENTER);
		} else {
			gtk_misc_set_alignment(GTK_MISC(handles.label), 0.0, 0.0);
			gtk_misc_set_alignment(GTK_MISC(handles.image), 0.0, 0.5);
		}
		gtk_label_set_justify(GTK_LABEL(handles.label), GTK_JUSTIFY_LEFT);
		return;
	}
	if ((_W_WIDGET(label)->style & W_CENTER) != 0) {
		if (GTK_VERSION >= VERSION(3, 16, 0)) {
			gtk_widget_set_align(handles.label, GTK_ALIGN_CENTER,
					GTK_ALIGN_START); //Aligns widget
			gtk_label_set_align(handles.label, 0.5, 0.0); //Aligns text inside the widget.
			gtk_widget_set_align(handles.image, GTK_ALIGN_CENTER,
					GTK_ALIGN_CENTER);
		} else {
			gtk_misc_set_alignment(GTK_MISC(handles.label), 0.5, 0.0);
			gtk_misc_set_alignment(GTK_MISC(handles.image), 0.5, 0.5);
		}

		gtk_label_set_justify(GTK_LABEL(handles.label), GTK_JUSTIFY_CENTER);
		return;
	}
	if ((_W_WIDGET(label)->style & W_RIGHT) != 0) {
		if (GTK_VERSION >= VERSION(3, 16, 0)) {
			gtk_widget_set_align(handles.label, GTK_ALIGN_END, GTK_ALIGN_START); //Aligns widget.
			gtk_label_set_align(handles.label, 1.0f, 0.0f); //Aligns text inside the widget.
			gtk_widget_set_align(handles.image, GTK_ALIGN_END,
					GTK_ALIGN_CENTER);
		} else {
			gtk_misc_set_alignment(GTK_MISC(handles.label), 1.0, 0.0);
			gtk_misc_set_alignment(GTK_MISC(handles.image), 1.0, 0.5);
		}
		gtk_label_set_justify(GTK_LABEL(handles.label), GTK_JUSTIFY_RIGHT);
		return;
	}
}
void _w_label_set_alignment(w_label *label, int alignment) {
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return;
	if ((alignment & (W_LEFT | W_RIGHT | W_CENTER)) == 0)
		return;
	_W_WIDGET(label)->style &= ~(W_LEFT | W_RIGHT | W_CENTER);
	_W_WIDGET(label)->style |= alignment & (W_LEFT | W_RIGHT | W_CENTER);
	_w_label_set_alignment_0(label);
}

wresult _w_label_create_handle(w_widget *widget, _w_control_priv *priv) {
	GtkWidget *fixedHandle, *boxHandle = 0, *frameHandle = 0, *labelHandle = 0,
			*imageHandle = 0, *handle = 0;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_THEME_BACKGROUND;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	if ((_W_WIDGET(widget)->style & W_SEPARATOR) != 0) {
		if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
			handle = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
			if (handle != 0 && GTK_VERSION >= VERSION(3, 20, 0)) {
				gtk_widget_set_valign(handle, GTK_ALIGN_CENTER);
			}
		} else {
			handle = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
			if (handle != 0 && GTK_VERSION >= VERSION(3, 20, 0)) {
				gtk_widget_set_halign(handle, GTK_ALIGN_CENTER);
			}
		}
		if (handle == 0)
			goto _err;
	} else {
		handle = gtk_event_box_new();
		if (handle == 0)
			goto _err;
		boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
		if (boxHandle == 0)
			goto _err;
		labelHandle = gtk_label_new_with_mnemonic("");
		if (labelHandle == 0)
			goto _err;
		imageHandle = gtk_image_new();
		if (imageHandle == 0)
			goto _err;
		gtk_container_add(GTK_CONTAINER(handle), boxHandle);
		gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
		gtk_container_add(GTK_CONTAINER(boxHandle), imageHandle);
		gtk_box_set_child_packing(GTK_BOX(boxHandle), labelHandle, TRUE, TRUE,
				0, GTK_PACK_START);
		gtk_box_set_child_packing(GTK_BOX(boxHandle), imageHandle, TRUE, TRUE,
				0, GTK_PACK_START);
	}
	if ((_W_WIDGET(widget)->style & W_BORDER) != 0) {
		frameHandle = gtk_frame_new("");
		if (frameHandle == 0)
			goto _err;
		gtk_container_add(GTK_CONTAINER(fixedHandle), frameHandle);
		gtk_container_add(GTK_CONTAINER(frameHandle), handle);
		gtk_frame_set_shadow_type(GTK_FRAME(frameHandle), GTK_SHADOW_ETCHED_IN);
	} else {
		gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	}
	_w_widget_set_control(handle, widget);
	_w_widget_set_control(fixedHandle, widget);
	if (labelHandle != 0)
		_w_widget_set_control(labelHandle, widget);
	if (boxHandle != 0)
		_w_widget_set_control(boxHandle, widget);
	if (frameHandle != 0)
		_w_widget_set_control(frameHandle, widget);
	if (imageHandle != 0)
		_w_widget_set_control(imageHandle, widget);
	_W_WIDGET(widget)->handle = handle;
	if ((_W_WIDGET(widget)->style & W_SEPARATOR) == 0) {
		if ((_W_WIDGET(widget)->style & W_WRAP) != 0) {
			gtk_label_set_line_wrap(GTK_LABEL(labelHandle), TRUE);
			gtk_label_set_line_wrap_mode(GTK_LABEL(labelHandle),
					PANGO_WRAP_WORD_CHAR);
		}
		// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
		// reset to default font to get the usual behavior
#if GTK3
		//setFontDescription(defaultFont().handle);
#endif
		_w_label_set_alignment_0(W_LABEL(widget));
	}
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_label_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint, hHint = e->hHint;
	int tmp[2];
	if ((_W_WIDGET(widget)->style & W_SEPARATOR) != 0) {
		if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
			if (wHint == W_DEFAULT)
				wHint = DEFAULT_WIDTH;
		} else {
			if (hHint == W_DEFAULT)
				hHint = DEFAULT_HEIGHT;
		}
	}
	_w_widget_handles handles;
	GtkWidget *frameHandle;
	_w_widget_get_handles(_W_WIDGET(widget)->handle, &handles);
	frameHandle = gtk_widget_get_parent(_W_WIDGET(widget)->handle);
	if (!GTK_IS_FRAME(frameHandle)) {
		frameHandle = 0;
	}
	/*
	 * Feature in GTK. GTK has a predetermined maximum width for wrapping text.
	 * The fix is to use pango layout directly instead of the label size request
	 * to calculate its preferred size.
	 */
	int fixWrap = handles.label != 0 && (_W_WIDGET(widget)->style & W_WRAP) != 0
			&& gtk_widget_get_visible(handles.label);
	if (fixWrap || frameHandle != 0) {
		priv->force_resize(W_CONTROL(widget), priv);
	}
	if (fixWrap) {
		PangoLayout *labelLayout = gtk_label_get_layout(
				GTK_LABEL(handles.label));
		int pangoWidth = pango_layout_get_width(labelLayout);
		if (e->wHint != W_DEFAULT) {
			pango_layout_set_width(labelLayout, wHint * PANGO_SCALE);
		} else {
			pango_layout_set_width(labelLayout, -1);
		}
		int w, h;
		pango_layout_get_pixel_size(labelLayout, &w, &h);
		pango_layout_set_width(labelLayout, pangoWidth);
		if (frameHandle != 0) {
			int labelWidth, labelHeight;
			gtk_widget_get_size_request(handles.label, &labelWidth,
					&labelHeight);
			gtk_widget_set_size_request(handles.label, 1, 1);
			tmp[0] = e->wHint;
			tmp[1] = e->hHint;
			e->wHint = -1;
			e->hHint = -1;
			_w_control_compute_native_size(widget, frameHandle, e, priv);
			e->wHint = tmp[0];
			e->hHint = tmp[1];
			gtk_widget_set_size_request(handles.label, labelWidth, labelHeight);
			e->size->width = e->size->width - 1;
			e->size->height = e->size->height - 1;
		} else {
			e->size->width = 0;
			e->size->height = 0;
		}
		e->size->width += wHint == W_DEFAULT ? w : wHint;
		e->size->height += hHint == W_DEFAULT ? h : hHint;
	} else {
		if (frameHandle != 0) {
			int reqWidth, reqHeight;
			gtk_widget_get_size_request(_W_WIDGET(widget)->handle, &reqWidth,
					&reqHeight);
			gtk_widget_set_size_request(_W_WIDGET(widget)->handle, wHint,
					hHint);
			tmp[0] = e->wHint;
			tmp[1] = e->hHint;
			e->wHint = -1;
			e->hHint = -1;
			_w_control_compute_native_size(widget, frameHandle, e, priv);
			e->wHint = tmp[0];
			e->hHint = tmp[1];
			gtk_widget_set_size_request(_W_WIDGET(widget)->handle, reqWidth,
					reqHeight);
		} else {
			tmp[0] = e->wHint;
			tmp[1] = e->hHint;
			e->wHint = wHint;
			e->hHint = hHint;
			_w_control_compute_native_size(widget, _W_WIDGET(widget)->handle, e,
					priv);
			e->wHint = tmp[0];
			e->hHint = tmp[1];
		}
	}
	/*
	 * Feature in GTK.  Instead of using the font height to determine
	 * the preferred height of the widget, GTK uses the text metrics.
	 * The fix is to ensure that the preferred height is at least as
	 * tall as the font height.
	 *
	 * NOTE: This work around does not fix the case when there are
	 * muliple lines of text.
	 */
	if (hHint == W_DEFAULT && handles.label != 0) {
		PangoLayout *layout = gtk_label_get_layout(GTK_LABEL(handles.label));
		const PangoFontDescription *font = pango_layout_get_font_description(
				layout);
		PangoContext *context = pango_layout_get_context(layout);
		PangoLanguage *lang = pango_context_get_language(context);
		/* PangoFontDescription *font = _w_control_getFontDescription(
		 W_CONTROL(widget), reserved);*/
		PangoFontMetrics *metrics = pango_context_get_metrics(context, font,
				lang);
		int ascent = pango_font_metrics_get_ascent(metrics);
		int descent = pango_font_metrics_get_descent(metrics);
		pango_font_metrics_unref(metrics);
		//int fontHeight = ascent + descent;
		int fontHeight = 0;
#if GTK3
		int bufferBottom;
		int bufferTop;
		g_object_get(handles.label, "margin-bottom", &bufferBottom, NULL);
		g_object_get(handles.label, "margin-top", &bufferTop, NULL);
		fontHeight += bufferBottom + bufferTop;
#endif
		if (frameHandle != 0) {
			w_point thickness;
			_w_control_get_thickness(frameHandle, &thickness);
			fontHeight += 2 * thickness.y;
			fontHeight += 2
					* gtk_container_get_border_width(
							GTK_CONTAINER(frameHandle));
		}
		e->size->height = WMAX(e->size->height, fontHeight);
	}
	return W_TRUE;
}
void _w_label_class_init(struct _w_label_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LABEL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_label_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_label);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_label);
	/*
	 * public function
	 */
	clazz->get_image = _w_label_get_image;
	clazz->get_text = _w_label_get_text;
	clazz->set_image = _w_label_set_image;
	clazz->set_text = _w_label_set_text;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.handle_top = _w_label_fixed_handle;
	priv->handle_fixed = _w_label_fixed_handle;
	priv->handle_event = _w_label_fixed_handle;
	priv->widget.check_style = _w_label_check_style;
	priv->widget.create_handle = _w_label_create_handle;
	priv->widget.compute_size = _w_label_compute_size;
}

