/*
 * button.c
 *
 *  Created on: 21 févr. 2021
 *      Author: azeddine
 */
#include "button.h"
#include "../widgets/toolkit.h"
#define  INNER_BORDER  1
#define  DEFAULT_BORDER  1
/*
 * private function
 */
wuint64 _w_button_check_style(w_widget *widget, wuint64 style) {
	style = _w_widget_check_bits(style, W_PUSH, W_ARROW, W_CHECK, W_RADIO,
			W_TOGGLE, 0);
	if ((style & (W_PUSH | W_TOGGLE)) != 0) {
		return _w_widget_check_bits(style, W_CENTER, W_LEFT, W_RIGHT, 0, 0, 0);
	}
	if ((style & (W_CHECK | W_RADIO)) != 0) {
		return _w_widget_check_bits(style, W_LEFT, W_RIGHT, W_CENTER, 0, 0, 0);
	}
	if ((style & W_ARROW) != 0) {
		style |= W_NO_FOCUS;
		return _w_widget_check_bits(style, W_UP, W_DOWN, W_LEFT, W_RIGHT, 0, 0);
	}
	return style;
}
wresult _w_button_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	/*
	 * Feature in GTK, GtkCheckButton and GtkRadioButton allocate
	 * only the minimum size necessary for its child. This causes the child
	 * alignment to fail. The fix is to set the child size to the size
	 * of the button.
	 */
	priv->force_resize(W_CONTROL(widget), priv);
	_w_widget_handles handles;
	_w_widget_get_handles(_W_WIDGET(widget)->handle, &handles);
	int reqWidth, reqHeight;
	if ((_W_WIDGET(widget)->style & (W_CHECK | W_RADIO)) != 0) {
		gtk_widget_get_size_request(handles.box, &reqWidth, &reqHeight);
		gtk_widget_set_size_request(handles.box, -1, -1);
	}
	int wrap = handles.label != 0 && (_W_WIDGET(widget)->style & W_WRAP) != 0
			&& gtk_widget_get_visible(handles.label);
	if (wrap) {
		int borderWidth = gtk_container_get_border_width(
				GTK_CONTAINER(handles.handle));
		int focusWidth = 0;
		gtk_widget_style_get(handles.handle, "focus-line-width", &focusWidth,
		NULL);
		int focusPadding = 0;
		gtk_widget_style_get(handles.handle, "focus-padding", &focusPadding,
		NULL);
		int trimWidth = 2 * (borderWidth + focusWidth + focusPadding);
		int trimHeight = trimWidth;
		int indicatorHeight = 0;
		if ((_W_WIDGET(widget)->style & (W_CHECK | W_RADIO)) != 0) {
			int indicatorSize = 0;
			gtk_widget_style_get(handles.handle, "indicator-size",
					&indicatorSize, NULL);
			int indicatorSpacing = 0;
			gtk_widget_style_get(handles.handle, "indicator-spacing",
					indicatorSpacing, NULL);
			indicatorHeight = indicatorSize + 2 * indicatorSpacing;
			trimWidth += indicatorHeight + indicatorSpacing;
		} else {
			w_point thickness;
			_w_control_get_thickness(handles.handle, &thickness);
			trimWidth += thickness.x * 2;
			trimHeight += thickness.y * 2;
			GtkBorder innerBorder;
			_w_button_get_border(&innerBorder, "inner-border", handles.handle,
			INNER_BORDER);
			trimWidth += innerBorder.left + innerBorder.right;
			trimHeight += innerBorder.top + innerBorder.bottom;
			if (gtk_widget_get_can_default(handles.handle)) {
				GtkBorder defaultBorder;
				_w_button_get_border(&defaultBorder, "default-border",
						handles.handle, DEFAULT_BORDER);
				trimWidth += defaultBorder.left + defaultBorder.right;
				trimHeight += defaultBorder.top + defaultBorder.bottom;
			}
		}
		int imageWidth = 0, imageHeight = 0;
		if (gtk_widget_get_visible(handles.image)) {
			GtkRequisition requisition;
			gtk_widget_get_preferred_size(handles.image, NULL, &requisition);
			imageWidth = requisition.width;
			imageHeight = requisition.height;
			int spacing = 0;
			g_object_get(G_OBJECT(handles.box), "spacing", &spacing, NULL);
			imageWidth += spacing;
		}
		PangoLayout *labelLayout = gtk_label_get_layout(
				GTK_LABEL(handles.label));
		int pangoWidth = pango_layout_get_width(labelLayout);
		if (wHint != W_DEFAULT) {
			pango_layout_set_width(labelLayout,
			WMAX(1, (wHint - imageWidth - trimWidth)) * PANGO_SCALE);
		} else {
			pango_layout_set_width(labelLayout, -1);
		}
		int w = 0, h = 0;
		pango_layout_get_pixel_size(labelLayout, &w, &h);
		pango_layout_set_width(labelLayout, pangoWidth);
		e->size->width = 0;
		e->size->height = 0;
		e->size->width +=
				wHint == W_DEFAULT ? w + imageWidth + trimWidth : wHint;
		int _height = WMAX(imageHeight, indicatorHeight);
		e->size->height += hHint == W_DEFAULT ?
		WMAX(_height, h) + trimHeight :
												hHint;
	} else {
		_w_control_compute_native_size(widget, handles.handle, e, priv);
	}
	if ((_W_WIDGET(widget)->style & (W_CHECK | W_RADIO)) != 0) {
		gtk_widget_set_size_request(handles.box, reqWidth, reqHeight);
	}
	if (wHint != W_DEFAULT || hHint != W_DEFAULT) {
		if (gtk_widget_get_can_default(handles.handle)) {
			GtkBorder border;
			_w_button_get_border(&border, "default-border", handles.handle,
			DEFAULT_BORDER);
			if (wHint != W_DEFAULT)
				e->size->width += border.left + border.right;
			if (hHint != W_DEFAULT)
				e->size->height += border.top + border.bottom;
		}
	}
	return W_TRUE;
}
wresult _w_button_create_handle(w_widget *widget, _w_control_priv *priv) {
	GtkWidget *fixedHandle, *arrowHandle = 0, *groupHandle = 0, *boxHandle = 0,
			*labelHandle = 0, *imageHandle = 0, *handle = 0;
	wuint64 style = _W_WIDGET(widget)->style;
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	if ((style & (W_PUSH | W_TOGGLE)) == 0)
		_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
	int bits = W_ARROW | W_TOGGLE | W_CHECK | W_RADIO | W_PUSH;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	//gtk_fixed_set_has_window (fixedHandle, TRUE);
	switch (style & bits) {
	case W_ARROW: {
		GtkArrowType arrow_type = GTK_ARROW_UP;
		if ((style & W_UP) != 0)
			arrow_type = GTK_ARROW_UP;
		if ((style & W_DOWN) != 0)
			arrow_type = GTK_ARROW_DOWN;
		if ((style & W_LEFT) != 0)
			arrow_type = GTK_ARROW_LEFT;
		if ((style & W_RIGHT) != 0)
			arrow_type = GTK_ARROW_RIGHT;
		handle = gtk_button_new();
		if (handle == 0)
			goto _err;
		arrowHandle = gtk_arrow_new(arrow_type, GTK_SHADOW_OUT);
		if (arrowHandle == 0)
			goto _err;
		g_object_set_qdata(G_OBJECT(arrowHandle), gtk_toolkit->quark[0],
				widget);
	}
		break;
	case W_TOGGLE:
		handle = gtk_toggle_button_new();
		if (handle == 0)
			goto _err;
		break;
	case W_CHECK:
		handle = gtk_check_button_new();
		if (handle == 0)
			goto _err;
		break;
	case W_RADIO:
		/*
		 * Feature in GTK.  In GTK, radio button must always be part of
		 * a radio button group.  In a GTK radio group, one button is always
		 * selected.  This means that it is not possible to have a single
		 * radio button that is unselected.  This is necessary to allow
		 * applications to implement their own radio behavior or use radio
		 * buttons outside of radio groups.  The fix is to create a hidden
		 * radio button for each radio button we create and add them
		 * to the same group.  This allows the visible button to be
		 * unselected.
		 */
		groupHandle = gtk_radio_button_new(0);
		if (groupHandle == 0)
			goto _err;
		g_object_set_qdata(G_OBJECT(groupHandle), gtk_toolkit->quark[0],
				widget);
		//g_object_ref(groupHandle);
		//gtk_object_sink(groupHandle);
		handle = gtk_radio_button_new(
				gtk_radio_button_get_group(GTK_RADIO_BUTTON(groupHandle)));
		if (handle == 0)
			goto _err;
		break;
	case W_PUSH:
	default:
		handle = gtk_button_new();
		if (handle == 0)
			goto _err;
		break;
	}

	if ((style & W_ARROW) != 0) {
		gtk_container_add(GTK_CONTAINER(handle), arrowHandle);
	} else {
		boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
		if (boxHandle == 0)
			goto _err;
		gtk_container_add(GTK_CONTAINER(handle), boxHandle);
		labelHandle = gtk_label_new_with_mnemonic("");
		if (labelHandle == 0)
			goto _err;
		imageHandle = gtk_image_new();
		if (imageHandle == 0)
			goto _err;
		gtk_container_add(GTK_CONTAINER(boxHandle), imageHandle);
		gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
		if ((style & W_WRAP) != 0) {
			gtk_label_set_line_wrap(GTK_LABEL(labelHandle), TRUE);
			if (GTK_VERSION >= VERSION(2, 10, 0)) {
				gtk_label_set_line_wrap_mode(GTK_LABEL(labelHandle),
						PANGO_WRAP_WORD_CHAR);
			}
		}
	}
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	_w_widget_set_control(handle, widget);
	_w_widget_set_control(imageHandle, widget);
	_w_widget_set_control(boxHandle, widget);
	_w_widget_set_control(labelHandle, widget);
	_w_widget_set_control(fixedHandle, widget);
	_W_WIDGET(widget)->handle = handle;
	if ((style & W_ARROW) == 0) {
		_w_button_set_alignment(W_BUTTON(widget),
				style & (W_LEFT | W_CENTER | W_RIGHT));
	}
	gtk_widget_show_all(fixedHandle);
	if (imageHandle != 0)
		gtk_widget_hide(imageHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_button_get_alignment(w_button *button) {
	if ((_W_WIDGET(button)->style & W_ARROW) != 0) {
		if ((_W_WIDGET(button)->style & W_UP) != 0)
			return W_UP;
		if ((_W_WIDGET(button)->style & W_DOWN) != 0)
			return W_DOWN;
		if ((_W_WIDGET(button)->style & W_LEFT) != 0)
			return W_LEFT;
		if ((_W_WIDGET(button)->style & W_RIGHT) != 0)
			return W_RIGHT;
		return W_UP;
	}
	if ((_W_WIDGET(button)->style & W_LEFT) != 0)
		return W_LEFT;
	if ((_W_WIDGET(button)->style & W_CENTER) != 0)
		return W_CENTER;
	if ((_W_WIDGET(button)->style & W_RIGHT) != 0)
		return W_RIGHT;
	return W_LEFT;
}
void _w_button_get_border(GtkBorder *gtkBorder, const char *border,
		GtkWidget *handle, int defaultBorder) {
	GtkBorder *borderPtr;
	gtk_widget_style_get(handle, border, &borderPtr, NULL);
	if (borderPtr != 0) {
		memmove(gtkBorder, borderPtr, sizeof(GtkBorder));
		gtk_border_free(borderPtr);
		return;
	}
	gtkBorder->left = defaultBorder;
	gtkBorder->top = defaultBorder;
	gtkBorder->right = defaultBorder;
	gtkBorder->bottom = defaultBorder;
}
wresult _w_button_get_grayed(w_button *button) {
	return _W_WIDGET(button)->state & STATE_BUTTON_GRAYED;
}
wresult _w_button_get_image(w_button *button, w_image *image) {
	_w_widget_handles handles;
	_w_widget_get_handles(_W_WIDGET(button)->handle, &handles);
	if (handles.image != 0) {
		GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(handles.image));
		_W_IMAGE(image)->pixbuf = gdk_pixbuf_copy(pixbuf);
	}
	return W_TRUE;
}
wresult _w_button_get_selection(w_button *button) {
	if ((_W_WIDGET(button)->style & (W_CHECK | W_RADIO | W_TOGGLE)) == 0)
		return W_FALSE;
	return gtk_toggle_button_get_active(
			GTK_TOGGLE_BUTTON(_W_WIDGET(button)->handle));
}
wresult _w_button_get_text(w_button *button, w_alloc alloc, void *user_data,
		int enc) {
	if ((_W_WIDGET(button)->style & W_ARROW) != 0)
		return W_TRUE;
	_w_widget_handles handles;
	_w_widget_get_handles(_W_WIDGET(button)->handle, &handles);
	if (handles.label != 0) {
		gchar *s = (gchar*) gtk_label_get_text(GTK_LABEL(handles.label));
		_gtk_alloc_set_text(alloc, user_data, s, -1, enc);
	}
	return W_TRUE;
}
void _w_button_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_control_hook_events(widget, priv);
	if (_W_BUTTON_PRIV(priv)->signal_clicked_id == 0) {
		_W_BUTTON_PRIV(priv)->signal_clicked_id = g_signal_lookup("clicked",
				gtk_button_get_type());
	}
	_w_widget_connect(_W_WIDGET(widget)->handle, SIGNAL_CLICKED,
	_W_BUTTON_PRIV(priv)->signal_clicked_id, FALSE);
	_w_widget_handles handles;
	_w_widget_get_handles(_W_WIDGET(widget)->handle, &handles);
	if (handles.label != 0) {
		_w_widget_connect(handles.label, SIGNAL_MNEMONIC_ACTIVATE, 0, FALSE);
	}
}
void _w_button_select_radio_0(_w_fixed *t, _w_event_platform *e, int next) {
	_w_fixed *first;
	w_widget *w;
	w_event event;
	if (next) {
		t = t->next;
	} else {
		first = (_w_fixed*) gtk_widget_get_parent((GtkWidget*) t);
		first = (_w_fixed*) first->first;
		if (t == first)
			return;
		t = t->prev;
	}
	while (t != 0) {
		w = g_object_get_qdata(G_OBJECT(t), gtk_toolkit->quark[0]);
		if (w == 0)
			return;
		if (w_widget_class_id(w) != _W_CLASS_BUTTON)
			return;
		if ((_W_WIDGET(w)->style & W_RADIO) == 0)
			return;
		if (gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(_W_WIDGET(w)->handle)) == TRUE) {
			_w_button_set_selection(W_BUTTON(w), W_FALSE);
			memset(&event, 0, sizeof(event));
			event.type = W_EVENT_SELECTION;
			event.platform_event = (struct w_event_platform*) e;
			event.widget = w;
			_w_widget_send_event(w, &event);
		}
		if (next)
			t = t->next;
		else {
			if (t == first)
				t = 0;
			else
				t = t->prev;
		}
	}
}
void _w_button_select_radio(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event event;
	/*
	 * This code is intentionally commented.  When two groups
	 * of radio buttons with the same parent are separated by
	 * another control, the correct behavior should be that
	 * the two groups act independently.  This is consistent
	 * with radio tool and menu items.  The commented code
	 * implements this behavior.
	 */
	_w_fixed *fixed = (_w_fixed*) priv->widget.handle_top(widget, priv);
	_w_button_select_radio_0(fixed, e, W_FALSE);
	_w_button_select_radio_0(fixed, e, W_TRUE);
	gboolean selected = _W_WIDGET(widget)->state & STATE_BUTTON_SELECTED;
	_w_button_set_selection(W_BUTTON(widget), !selected);
}
wresult _w_button_set_alignment(w_button *button, int alignment) {
	_w_widget_handles handles;
	_w_widget_get_handles(_W_WIDGET(button)->handle, &handles);
	if ((_W_WIDGET(button)->style & W_ARROW) != 0) {
		if ((_W_WIDGET(button)->style & (W_UP | W_DOWN | W_LEFT | W_RIGHT))
				== 0)
			return W_TRUE;
		_W_WIDGET(button)->style &= ~(W_UP | W_DOWN | W_LEFT | W_RIGHT);
		_W_WIDGET(button)->style |= alignment
				& (W_UP | W_DOWN | W_LEFT | W_RIGHT);
		GtkArrowType arrow_type = GTK_ARROW_UP;
		int isRTL = (_W_WIDGET(button)->style & W_RIGHT_TO_LEFT) != 0;
		switch (alignment) {
		case W_UP:
			arrow_type = GTK_ARROW_UP;
			break;
		case W_DOWN:
			arrow_type = GTK_ARROW_DOWN;
			break;
		case W_LEFT:
			arrow_type = isRTL ? GTK_ARROW_RIGHT : GTK_ARROW_LEFT;
			break;
		case W_RIGHT:
			arrow_type = isRTL ? GTK_ARROW_LEFT : GTK_ARROW_RIGHT;
			break;
		}
		gtk_arrow_set(GTK_ARROW(handles.arrow), arrow_type, GTK_SHADOW_OUT);
		return W_TRUE;
	}
	if ((alignment & (W_LEFT | W_RIGHT | W_CENTER)) == 0)
		return W_TRUE;

	_W_WIDGET(button)->style &= ~(W_LEFT | W_RIGHT | W_CENTER);
	_W_WIDGET(button)->style |= alignment & (W_LEFT | W_RIGHT | W_CENTER);
	/* Alignment not honoured when image and text are visible */
	int bothVisible = gtk_widget_get_visible(handles.label)
			&& gtk_widget_get_visible(handles.image);
	if (bothVisible) {
		if ((_W_WIDGET(button)->style & (W_RADIO | W_CHECK)) != 0)
			alignment = W_LEFT;
		if ((_W_WIDGET(button)->style & (W_PUSH | W_TOGGLE)) != 0)
			alignment = W_CENTER;
	}
	if ((alignment & W_LEFT) != 0) {
		if (bothVisible) {
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.label,
			FALSE, FALSE, 0, GTK_PACK_START);
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.image,
			FALSE, FALSE, 0, GTK_PACK_START);
		} else {
#if GTK3
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.label, TRUE,
			TRUE, 0, GTK_PACK_END);
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.image, TRUE,
			TRUE, 0, GTK_PACK_START);
#endif
		}

#if GTK3
		gtk_widget_set_align(handles.label, GTK_ALIGN_START, GTK_ALIGN_CENTER);
		gtk_widget_set_align(handles.image, GTK_ALIGN_START, GTK_ALIGN_CENTER);
#else
			gtk_misc_set_alignment (handles.label, 0.0, 0.5);
			gtk_misc_set_alignment (handles.image, 0.0, 0.5);
#endif

		gtk_label_set_justify(GTK_LABEL(handles.label), GTK_JUSTIFY_LEFT);
		return W_TRUE;
	}
	if ((alignment & W_CENTER) != 0) {
		if (bothVisible) {
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.label, TRUE,
			TRUE, 0, GTK_PACK_END);
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.image, TRUE,
			TRUE, 0, GTK_PACK_START);

#if GTK3
			gtk_widget_set_align(handles.label, GTK_ALIGN_START,
					GTK_ALIGN_CENTER);
			gtk_widget_set_align(handles.image, GTK_ALIGN_END,
					GTK_ALIGN_CENTER);
#else
				gtk_misc_set_alignment (GTK_MISC(handles.label), 0.0, 0.5);
				gtk_misc_set_alignment (GTK_MISC(handles.image), 1.0, 0.5);
#endif
		} else {
#if GTK3
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.label, TRUE,
			TRUE, 0, GTK_PACK_END);
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.image, TRUE,
			TRUE, 0, GTK_PACK_START);
#endif
#if GTK3
			gtk_widget_set_align(handles.label, GTK_ALIGN_CENTER,
					GTK_ALIGN_CENTER);
			gtk_widget_set_align(handles.image, GTK_ALIGN_CENTER,
					GTK_ALIGN_CENTER);
#else
				gtk_misc_set_alignment (GTK_MISC(handles.label), 0.5, 0.5);
				gtk_misc_set_alignment (GTK_MISC(handles.image), 0.5, 0.5);
#endif
			gtk_label_set_justify(GTK_LABEL(handles.label), GTK_JUSTIFY_CENTER);
		}
		return W_TRUE;
	}
	if ((alignment & W_RIGHT) != 0) {
		if (bothVisible) {
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.label,
			FALSE, FALSE, 0, GTK_PACK_END);
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.image,
			FALSE, FALSE, 0, GTK_PACK_END);
		} else {
#if GTK3
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.label, TRUE,
			TRUE, 0, GTK_PACK_END);
			gtk_box_set_child_packing(GTK_BOX(handles.box), handles.image, TRUE,
			TRUE, 0, GTK_PACK_START);
#endif
		}
#if GTK3
		gtk_widget_set_align(handles.label, GTK_ALIGN_END, GTK_ALIGN_CENTER);
		gtk_widget_set_align(handles.image, GTK_ALIGN_END, GTK_ALIGN_CENTER);
#else
			gtk_misc_set_alignment (GTK_MISC(handles.label), 1.0, 0.5);
			gtk_misc_set_alignment (GTK_MISC(handles.image), 1.0, 0.5);
#endif
		gtk_label_set_justify(GTK_LABEL(handles.label), GTK_JUSTIFY_RIGHT);
		return W_TRUE;
	}
	return W_TRUE;
}
wresult _w_button_set_grayed(w_button *button, int grayed) {
	if ((_W_WIDGET(button)->style & W_CHECK) == 0)
		return W_FALSE;
	if (grayed) {
		_W_WIDGET(button)->state |= STATE_BUTTON_GRAYED;
	} else {
		_W_WIDGET(button)->state &= ~STATE_BUTTON_GRAYED;
	}
	if (grayed
			&& gtk_toggle_button_get_active(
					GTK_TOGGLE_BUTTON(_W_WIDGET(button)->handle))) {
		gtk_toggle_button_set_inconsistent(
				GTK_TOGGLE_BUTTON(_W_WIDGET(button)->handle), TRUE);
	} else {
		gtk_toggle_button_set_inconsistent(
				GTK_TOGGLE_BUTTON(_W_WIDGET(button)->handle), FALSE);
	}
	return W_TRUE;
}
wresult _w_button_set_image(w_button *button, w_image *image) {
	if ((_W_WIDGET(button)->style & W_ARROW) != 0)
		return W_FALSE;
	_w_widget_handles handles;
	_w_widget_get_handles(_W_WIDGET(button)->handle, &handles);
	GdkPixbuf *lastpixbuf = gtk_image_get_pixbuf(GTK_IMAGE(handles.image));
	gtk_image_set_from_pixbuf(GTK_IMAGE(handles.image), 0);
	if (lastpixbuf != 0) {
		g_object_unref(lastpixbuf);
		gtk_widget_hide(handles.image);
	}
	if (image != 0) {
		if (!w_image_is_ok(image))
			return W_ERROR_INVALID_ARGUMENT;
		lastpixbuf = gdk_pixbuf_copy(_W_IMAGE(image)->pixbuf);
		gtk_image_set_from_pixbuf(GTK_IMAGE(handles.image), lastpixbuf);
		gtk_widget_show(handles.image);
	}
	_w_button_set_alignment(button, _W_WIDGET(button)->style);
	return W_TRUE;
}
wresult _w_button_set_selection(w_button *button, int selected) {
	if ((_W_WIDGET(button)->style & (W_CHECK | W_RADIO | W_TOGGLE)) == 0)
		return W_FALSE;
	g_signal_handlers_block_matched(_W_WIDGET(button)->handle,
			G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) ((intptr_t) SIGNAL_CLICKED));
	if (selected) {
		_W_WIDGET(button)->state |= STATE_BUTTON_SELECTED;
	} else {
		_W_WIDGET(button)->state &= ~STATE_BUTTON_SELECTED;
	}
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(_W_WIDGET(button)->handle),
			selected);
	if ((_W_WIDGET(button)->style & W_CHECK) != 0) {
		int grayed = _w_button_get_grayed(button);
		if (selected && grayed) {
			gtk_toggle_button_set_inconsistent(
					GTK_TOGGLE_BUTTON(_W_WIDGET(button)->handle), TRUE);
		} else {
			gtk_toggle_button_set_inconsistent(
					GTK_TOGGLE_BUTTON(_W_WIDGET(button)->handle),
					FALSE);
		}
	}
	if ((_W_WIDGET(button)->style & W_RADIO) != 0) {
		GSList *list = gtk_radio_button_get_group(
				GTK_RADIO_BUTTON(_W_WIDGET(button)->handle));
		while (list != 0) {
			GtkToggleButton *btn = GTK_TOGGLE_BUTTON(list->data);
			if (btn != (GtkToggleButton*) _W_WIDGET(button)->handle) {
				gtk_toggle_button_set_active(btn, !selected);
			}
			list = list->next;
		}
	}
	g_signal_handlers_unblock_matched(_W_WIDGET(button)->handle,
			G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
			(void*) ((intptr_t) SIGNAL_CLICKED));
	return W_TRUE;
}
wresult _w_button_set_text(w_button *button, const char *text, int length,
		int enc) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(button)->style & W_ARROW) != 0)
		return W_TRUE;
	_w_widget_handles handles;
	int newlength, mnemonic;
	char *s = _gtk_text_fix(text, length, enc, &newlength, &mnemonic);
	if (s != 0) {
		_w_widget_get_handles(_W_WIDGET(button)->handle, &handles);
		if (handles.label != 0) {
			gtk_label_set_text_with_mnemonic(GTK_LABEL(handles.label), s);
			gtk_widget_show(handles.label);
		}
		_w_button_set_alignment(button, _W_WIDGET(button)->style);
	}
	_gtk_text_free(text, s, length);
	return W_TRUE;
}
/*
 * signals
 */
gboolean _gtk_button_clicked(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_point *lastInput = &_W_CONTROL(widget)->lastInput;
	if (_w_control_contained_in_region(widget, lastInput, priv)) {
		return FALSE;
	}
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	if ((_W_WIDGET(widget)->style & W_RADIO) != 0) {
		w_composite *parent;
		w_control_get_parent(W_CONTROL(widget), &parent);
		if ((_W_WIDGET(parent)->style & W_NO_RADIO_GROUP) != 0) {
			gboolean selected = _W_WIDGET(widget)->state & STATE_BUTTON_SELECTED;
			_w_button_set_selection(W_BUTTON(widget), !selected);
		} else {
			_w_button_select_radio(widget, e, priv);
		}
	} else {
		if ((_W_WIDGET(widget)->style & W_CHECK) != 0) {
			if (_W_WIDGET(widget)->state & STATE_BUTTON_GRAYED) {
				if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(handle))) {
					gtk_toggle_button_set_inconsistent(
							GTK_TOGGLE_BUTTON(handle), TRUE);
				} else {
					gtk_toggle_button_set_inconsistent(
							GTK_TOGGLE_BUTTON(handle),
							FALSE);
				}
			}
		}
	}
	w_event event;
	memset(&event, 0, sizeof(event));
	event.type = W_EVENT_SELECTION;
	event.platform_event = (struct w_event_platform*) e;
	event.widget = widget;
	_w_widget_send_event(widget, &event);
	return FALSE;
}
void _w_button_class_init(struct _w_button_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_BUTTON;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_button_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_button);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_button);
	/*
	 * public function
	 */
	clazz->get_alignment = _w_button_get_alignment;
	clazz->get_grayed = _w_button_get_grayed;
	clazz->get_image = _w_button_get_image;
	clazz->get_selection = _w_button_get_selection;
	clazz->get_text = _w_button_get_text;
	clazz->set_alignment = _w_button_set_alignment;
	clazz->set_grayed = _w_button_set_grayed;
	clazz->set_image = _w_button_set_image;
	clazz->set_selection = _w_button_set_selection;
	clazz->set_text = _w_button_set_text;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.handle_top = _w_widget_hp;
	priv->handle_fixed = _w_widget_hp;
	priv->widget.compute_size = _w_button_compute_size;
	priv->widget.check_style = _w_button_check_style;
	priv->widget.create_handle = _w_button_create_handle;
	priv->widget.hook_events = _w_button_hook_events;
	/*
	 * signals
	 */
	_gtk_signal *signals = priv->widget.signals;
	signals[SIGNAL_CLICKED] = _gtk_button_clicked;

}

