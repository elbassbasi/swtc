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
	if ((_W_WIDGET(label)->style & (W_SEPARATOR | W_HYPERLINK)) != 0)
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
		return W_FALSE;
	if ((_W_WIDGET(label)->style & W_HYPERLINK) != 0) {
		_w_hyperlink *link = _W_LABEL(label)->hyperlink;
		if (link != 0) {
			_gtk_alloc_set_text(string, user_data, link->text,
					link->text_length, enc);
		}
	} else {
		_w_widget_handles handles;
		_w_widget_get_handles( _W_WIDGET(label)->handle, &handles);
		if (handles.label != 0) {
			const char *text = gtk_label_get_text(GTK_LABEL(handles.label));
			_gtk_alloc_set_text(string, user_data, text, -1, enc);
		}
	}
	return W_TRUE;
}
wresult _w_label_set_image(w_label *label, w_image *image) {
	if ((_W_WIDGET(label)->style & (W_SEPARATOR | W_HYPERLINK)) != 0)
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
int _w_label_parse_mnemonics(char *buffer, int start, int end,
		_w_hyperlink *link, int *result_length) {
	int mnemonic = -1, index = start;
	char *result = link->text;
	int _result = result_length[0];
	while (index < end) {
		if (buffer[index] == '&') {
			if (index + 1 < end && buffer[index + 1] == '&') {
				result[_result] = buffer[index];
				_result++;
				index++;
			} else {
				mnemonic = _result;
			}
		} else {
			result[_result] = buffer[index];
			_result++;
		}
		index++;
	}
	result_length[0] = _result;
	return mnemonic;
}
int _w_label_parse(const char *string, int length, _w_hyperlink *link) {
	int index = 0, state = 0, linkIndex = 0;
	int start = 0, tagStart = 0, linkStart = 0, endtagStart = 0, refStart = 0;
	int result_length = 0;
	while (index < length) {
		char c = tolower(string[index]);
		switch (state) {
		case 0:
			if (c == '<') {
				tagStart = index;
				state++;
			}
			break;
		case 1:
			if (c == 'a')
				state++;
			break;
		case 2:
			switch (c) {
			case 'h':
				state = 7;
				break;
			case '>':
				linkStart = index + 1;
				state++;
				break;
			default:
				if (isspace(c))
					break;
				else
					state = 13;
			}
			break;
		case 3:
			if (c == '<') {
				endtagStart = index;
				state++;
			}
			break;
		case 4:
			state = c == '/' ? state + 1 : 3;
			break;
		case 5:
			state = c == 'a' ? state + 1 : 3;
			break;
		case 6:
			if (c == '>') {
				if (link != 0) {
					_w_hyperlink_id *id = &link->ids[linkIndex];
					id->mnemonic = _w_label_parse_mnemonics(string, start,
							tagStart, link, &result_length);
					id->offset.start = result_length;
					_w_label_parse_mnemonics(string, linkStart, endtagStart,
							link, &result_length);
					id->offset.end = result_length;
					if (id->id.end != 0) {
						id->id.start = linkStart;
						id->id.end = endtagStart;
					}
				}
				linkIndex++;
				start = tagStart = linkStart = endtagStart = refStart = index
						+ 1;
				state = 0;
			} else {
				state = 3;
			}
			break;
		case 7:
			state = c == 'r' ? state + 1 : 0;
			break;
		case 8:
			state = c == 'e' ? state + 1 : 0;
			break;
		case 9:
			state = c == 'f' ? state + 1 : 0;
			break;
		case 10:
			state = c == '=' ? state + 1 : 0;
			break;
		case 11:
			if (c == '"') {
				state++;
				refStart = index + 1;
			} else {
				state = 0;
			}
			break;
		case 12:
			if (c == '"') {
				if (link != 0) {
					_w_hyperlink_id *id = &link->ids[linkIndex];
					id->id.start = refStart;
					id->id.end = index;
				}
				state = 2;
			}
			break;
		case 13:
			if (isspace(c)) {
				state = 0;
			} else if (c == '=') {
				state++;
			}
			break;
		case 14:
			state = c == '"' ? state + 1 : 0;
			break;
		case 15:
			if (c == '"')
				state = 2;
			break;
		default:
			state = 0;
			break;
		}
		index++;
	}
	if (link != 0) {
		_w_hyperlink_id *id = &link->ids[linkIndex];
		if (start < length) {
			int tmp = _w_label_parse_mnemonics(string, start, tagStart, link,
					&result_length);
			int mnemonic = _w_label_parse_mnemonics(string,
					WMAX(tagStart, linkStart), length, link, &result_length);
			if (mnemonic == -1)
				mnemonic = tmp;
			id->mnemonic = mnemonic;
		} else {
			id->mnemonic = -1;
		}
		link->text_length = result_length;
	}
	return linkIndex;
}

void _w_label_set_text_hyperlink(w_label *label, const char *text, int length,
		int enc) {
	int newlength, mnemonic;
	char *s = _gtk_text_fix(text, length, enc | _GTK_TEXT_FIX_CALCUL_LENGTH,
			&newlength, &mnemonic);
	if (s != 0) {
		int ids_count = _w_label_parse(s, newlength, 0);
		_w_hyperlink *link;
		const int new_size = sizeof(_w_hyperlink) + newlength + 2
				+ ids_count * sizeof(_w_hyperlink_id);
		if (_W_LABEL(label)->hyperlink == 0) {
			link = calloc(new_size, 1);
		} else {
			link = realloc(_W_LABEL(label)->hyperlink, new_size);
		}
		_W_LABEL(label)->hyperlink = link;
		if (link != 0) {
			link->text = &((char*) link)[new_size - newlength - 2];
			link->ids_length = ids_count;
			if (link->layout == 0) {
				PangoContext *context = gdk_pango_context_get();
				if (context != 0) {
					pango_context_set_language(context,
							gtk_get_default_language());
					pango_context_set_base_dir(context, PANGO_DIRECTION_LTR);
					link->layout = pango_layout_new(context);
				}
			}
			if (link->layout != 0) {
				_w_label_parse(s, newlength, link);
				w_font *sysfont = w_toolkit_get_system_font(0);
				PangoFontDescription *hfont = _W_FONT(sysfont)->handle;
				pango_layout_set_font_description(link->layout, hfont);
				pango_layout_set_text(link->layout, link->text,
						link->text_length);
				if (ids_count > 0) {
					PangoAttrList *list = pango_attr_list_new();
					for (int i = 0; i < ids_count; i++) {
						_w_hyperlink_id *ids = &link->ids[i];
						PangoAttribute *attr = pango_attr_foreground_new(
								172 * 0x100, 168 * 0x100, 153 * 0x100);
						attr->start_index = ids->offset.start;
						attr->end_index = ids->offset.end;
						pango_attr_list_insert(list, attr);
						attr = pango_attr_underline_new(PANGO_UNDERLINE_SINGLE);
						attr->start_index = ids->offset.start;
						attr->end_index = ids->offset.end;
						pango_attr_list_insert(list, attr);
					}
					pango_layout_set_attributes(link->layout, list);
				}
			}
			memcpy(link->text, s, newlength);
			link->text[newlength] = 0;
		}
	}
	_gtk_text_free(text, s, newlength);
}

wresult _w_label_set_text(w_label *label, const char *text, int length,
		int enc) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_TRUE;
	int newlength, mnemonic;
	if ((_W_WIDGET(label)->style & W_HYPERLINK) != 0) {
		_w_label_set_text_hyperlink(label, text, length, enc);
	} else {
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
	}
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
	if (_W_WIDGET(widget)->style & W_HYPERLINK)
		return _w_widget_h(widget, priv);
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
	wuint64 style = _W_WIDGET(widget)->style;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	if ((style & W_HYPERLINK) != 0) {
		gtk_widget_set_can_focus(fixedHandle, TRUE);
		handle = fixedHandle;
	} else if ((style & W_SEPARATOR) != 0) {
		if ((style & W_HORIZONTAL) != 0) {
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
	if ((style & W_HYPERLINK) == 0) {
		if ((style & W_BORDER) != 0) {
			frameHandle = gtk_frame_new("");
			if (frameHandle == 0)
				goto _err;
			gtk_container_add(GTK_CONTAINER(fixedHandle), frameHandle);
			gtk_container_add(GTK_CONTAINER(frameHandle), handle);
			gtk_frame_set_shadow_type(GTK_FRAME(frameHandle),
					GTK_SHADOW_ETCHED_IN);
		} else {
			gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
		}
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
	if ((style & (W_SEPARATOR | W_HYPERLINK)) == 0) {
		if ((style & W_WRAP) != 0) {
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
	if (_W_WIDGET(widget)->style & W_HYPERLINK) {
		_w_hyperlink *link = _W_LABEL(widget)->hyperlink;
		if (link != 0) {
			if (wHint != W_DEFAULT && wHint < 0)
				wHint = 0;
			if (hHint != W_DEFAULT && hHint < 0)
				hHint = 0;
			int width, height;
			int layoutWidth = pango_layout_get_width(link->layout);
			//TEMPORARY CODE
			if (wHint == 0) {
				pango_layout_set_width(link->layout, 1);
				pango_layout_get_pixel_size(link->layout, &width, &height);
				width = 0;
			} else {
				pango_layout_set_width(link->layout, wHint);
				pango_layout_get_pixel_size(link->layout, &width, &height);
			}
			pango_layout_set_width(link->layout, layoutWidth);
			if (wHint != W_DEFAULT)
				width = wHint;
			if (hHint != W_DEFAULT)
				height = hHint;
			int border = w_control_get_border_width(W_CONTROL(widget));
			width += border * 2;
			height += border * 2;
			e->size->width = width;
			e->size->height = height;
		}
		return W_TRUE;
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
void _w_label_draw_widget(w_control *control, w_graphics *gc,
		_w_control_priv *priv) {
	if (_W_WIDGET(control)->style & W_HYPERLINK) {
		_w_hyperlink *link = _W_LABEL(control)->hyperlink;
		if (link != 0) {
			int selStart = link->selection.start;
			int selEnd = link->selection.end;
			// temporary code to disable text selection
			selStart = selEnd = -1;
			if ((_W_WIDGET(control)->state & STATE_DISABLED) != 0) {
				//w_graphics_set_foreground(gc, disabledColor);
			}
			if (link->layout != 0) {
				cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
				cairo_move_to(cairo, 0, 0);
				pango_cairo_show_layout(cairo, link->layout);
				cairo_new_path(cairo);
			}
		}
	}
}
wresult _w_label_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_priv *priv) {
	int result = _w_control_set_bounds_0(control, location, size, priv);
	if ((result & 2) != 0 && (_W_WIDGET(control)->style & W_HYPERLINK)) {
		_w_hyperlink *link = _W_LABEL(control)->hyperlink;
		if (link != 0) {
			if (link->layout != 0) {
				pango_layout_set_width(link->layout, size->width * PANGO_SCALE);
			}
		}
		W_CONTROL_GET_CLASS(control)->redraw(control, 0, 0);
	}
	return result;
}
/*
 * signals
 */
gboolean _gtk_label_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_button_press_event(widget, e, priv);
	if (result == 0 && (_W_WIDGET(widget)->style & W_HYPERLINK)) {
		_w_hyperlink *link = _W_LABEL(widget)->hyperlink;
		if (link != 0) {

		}
	}
	return result;
}
gboolean _gtk_label_button_release_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_button_release_event(widget, e, priv);
	if (result == 0 && (_W_WIDGET(widget)->style & W_HYPERLINK)) {
		_w_hyperlink *link = _W_LABEL(widget)->hyperlink;
		if (link != 0) {

		}
	}
	return result;
}
gboolean _gtk_label_event_after(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_event_after(widget, e, priv);
	if (result == 0 && (_W_WIDGET(widget)->style & W_HYPERLINK)) {
		_w_hyperlink *link = _W_LABEL(widget)->hyperlink;
		if (link != 0) {

		}
	}
	return result;
}
gboolean _gtk_label_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_draw(widget, e, priv);
	if (result == 0 && (_W_WIDGET(widget)->style & W_HYPERLINK)) {
		_w_hyperlink *link = _W_LABEL(widget)->hyperlink;
		if (link != 0) {

		}
	}
	return result;
}
gboolean _gtk_label_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_key_press_event(widget, e, priv);
	if (result == 0 && (_W_WIDGET(widget)->style & W_HYPERLINK)) {
		_w_hyperlink *link = _W_LABEL(widget)->hyperlink;
		if (link != 0) {

		}
	}
	return result;
}
gboolean _gtk_label_motion_notify_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	gboolean result = _gtk_control_motion_notify_event(widget, e, priv);
	if (result == 0 && (_W_WIDGET(widget)->style & W_HYPERLINK)) {
		_w_hyperlink *link = _W_LABEL(widget)->hyperlink;
		if (link != 0) {
			GdkEventMotion *gdkEvent = (GdkEventMotion*) e->args[0];
			int x = gdkEvent->x;
			int y = gdkEvent->y;
			if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
				x = priv->get_client_width(W_CONTROL(widget), priv) - x;
			}
			if ((gdkEvent->state & GDK_BUTTON1_MASK) != 0) {
				int oldSelection = link->selection.end;
				int index, trailing;
				pango_layout_xy_to_index(link->layout, x * PANGO_SCALE,
						y * PANGO_SCALE, &index, &trailing);
				link->selection.end = index;
				if (link->selection.end != oldSelection) {
					int newSelection = link->selection.end;
					if (oldSelection > newSelection) {
						int temp = oldSelection;
						oldSelection = newSelection;
						newSelection = temp;
					}
					w_rect rect;
					memset(&rect, 0, sizeof(rect));
					W_CONTROL_GET_CLASS(widget)->redraw(W_CONTROL(widget),
							&rect, W_FALSE);
				}
			} else {
				_w_hyperlink_id *ids = link->ids;
				int ids_count = link->ids_length;
				int index = 0, trailing = 0;
				pango_layout_xy_to_index(link->layout, x * PANGO_SCALE,
						y * PANGO_SCALE, &index, &trailing);
				for (int j = 0; j < ids_count; j++) {
					_w_hyperlink_id *id = &ids[j];
					if (index >= id->offset.start && index <= id->offset.end) {
						W_CONTROL_GET_CLASS(widget)->set_cursor(
								W_CONTROL(widget),
								w_toolkit_get_system_cursor(0, W_CURSOR_HAND));
						return result;
					}
				}
				W_CONTROL_GET_CLASS(widget)->set_cursor(W_CONTROL(widget), 0);
			}
		}
	}
	return result;
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
	priv->draw_widget = _w_label_draw_widget;
	priv->set_bounds_0 = _w_label_set_bounds_0;
	/*
	 * signals
	 */
	_gtk_signal *signals = _W_WIDGET_PRIV(priv)->signals;
	signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_label_button_press_event;
	signals[SIGNAL_BUTTON_RELEASE_EVENT] = _gtk_label_button_release_event;
	signals[SIGNAL_EVENT_AFTER] = _gtk_label_event_after;
	signals[SIGNAL_DRAW] = _gtk_label_draw;
	signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_label_key_press_event;
	signals[SIGNAL_MOTION_NOTIFY_EVENT] = _gtk_label_motion_notify_event;
}

