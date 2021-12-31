/*
 * textedit.c
 *
 *  Created on: 21 févr. 2021
 *      Author: azeddine
 */
#include "textedit.h"
#include "../widgets/toolkit.h"
#define SPACE_FOR_CURSOR 1
#define LIMIT 0x7FFFFFFF
int _w_textedit_apply_theme_background(w_scrollable *scrollable,
		_w_control_priv *priv) {
	return (_W_CONTROL(scrollable)->backgroundAlpha == 0
			|| (_W_WIDGET(scrollable)->style
					& (W_BORDER | W_HSCROLL | W_VSCROLL)) == 0) ? 1 : 0;
}
void _w_textedit_apply_segments(w_textedit *text) {

}
void _w_textedit_clear_segments(w_textedit *text, int applyText) {

}
wresult _w_textedit_append(w_textedit *text, const char *string, size_t length,
		int enc) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	int newlength, mnemonic;
	char *s = _gtk_text_fix(string, length, enc, &newlength, &mnemonic);
	if (s == 0)
		return W_ERROR_NO_MEMORY;
	_w_textedit_clear_segments(text, W_TRUE);
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gint pos = -1;
		gtk_editable_insert_text(GTK_EDITABLE(handle), s, newlength, &pos);
		_gtk_text_free(string, s, newlength);
		gtk_editable_set_position(GTK_EDITABLE(handle), -1);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(handle));
		GtkTextIter position;
		gtk_text_buffer_get_end_iter(bufferHandle, &position);
		gtk_text_buffer_insert(bufferHandle, &position, s, newlength);
		_gtk_text_free(string, s, newlength);
		gtk_text_buffer_place_cursor(bufferHandle, &position);
		GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
		gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(handle), mark, 0, TRUE, 0,
				0);
	}
	_w_textedit_apply_segments(text);
	return W_TRUE;
}
wresult _w_textedit_clear_selection(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkEditable *editable = GTK_EDITABLE(_W_WIDGET(text)->handle);
		int position = gtk_editable_get_position(editable);
		gtk_editable_select_region(editable, position, position);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(_W_WIDGET(text)->handle));
		GtkTextIter position;
		GtkTextMark *insertMark = gtk_text_buffer_get_insert(bufferHandle);
		gtk_text_buffer_get_iter_at_mark(bufferHandle, &position, insertMark);
		gtk_text_buffer_select_range(bufferHandle, &position, &position);
	}
	return W_TRUE;
}
wresult _w_textedit_copy(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkEditable *editable = GTK_EDITABLE(_W_WIDGET(text)->handle);
		gtk_editable_copy_clipboard(editable);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(_W_WIDGET(text)->handle));
#if GTK3
		GtkClipboard *clipboard = gtk_clipboard_get(GDK_NONE);
#endif
#if GTK4
#endif
		_w_textedit_clear_segments(text, TRUE);
		gtk_text_buffer_copy_clipboard(bufferHandle, clipboard);
		_w_textedit_apply_segments(text);
	}
	return W_TRUE;
}
wuint64 _w_textedit_check_style(w_widget *control, wuint64 style) {
	if ((style & W_SEARCH) != 0) {
		style |= W_SINGLE | W_BORDER;
		style &= ~W_PASSWORD;
		/*
		 * NOTE: ICON_CANCEL has the same value as H_SCROLL and
		 * ICON_SEARCH has the same value as V_SCROLL so they are
		 * cleared because W_SINGLE is set.
		 */
	}
	if ((style & W_SINGLE) != 0 && (style & W_MULTI) != 0) {
		style &= ~W_MULTI;
	}
	style = _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
	if ((style & W_SINGLE) != 0)
		style &= ~(W_HSCROLL | W_VSCROLL | W_WRAP);
	if ((style & W_WRAP) != 0) {
		style |= W_MULTI;
		style &= ~W_HSCROLL;
	}
	if ((style & W_MULTI) != 0)
		style &= ~W_PASSWORD;
	if ((style & (W_SINGLE | W_MULTI)) != 0)
		return style;
	if ((style & (W_HSCROLL | W_VSCROLL)) != 0)
		return style | W_MULTI;
	return style | W_SINGLE;
}
wresult _w_textedit_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	w_event_compute_size ee;
	w_size widthNative;
	_w_scrollable_compute_trim(widget, e, priv);
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	int xborder = 0, yborder = 0;
	if ((_W_WIDGET(widget)->style & W_SINGLE) != 0) {
#if GTK3
		GtkBorder tmp;
		GtkStyleContext *context = gtk_widget_get_style_context(handle);
		if (GTK_VERSION < VERSION(3, 18, 0)) {
			gtk_style_context_get_padding(context, GTK_STATE_FLAG_NORMAL, &tmp);
		} else {
			gtk_style_context_get_padding(context,
					gtk_widget_get_state_flags(handle), &tmp);
		}
		e->result->x -= tmp.left;
		e->result->y -= tmp.top;
		e->result->width += tmp.left + tmp.right;
		if (tmp.bottom == 0 && tmp.top == 0) {
			memset(&ee, 0, sizeof(ee));
			ee.wHint = e->result->width;
			ee.hHint = W_DEFAULT;
			ee.size = &widthNative;
			_w_control_compute_native_size(widget, handle, &ee, priv);
			e->result->height = widthNative.height;
		} else {
			e->result->height += tmp.top + tmp.bottom;
		}
		if ((_W_WIDGET(widget)->style & W_BORDER) != 0) {
			if (GTK_VERSION < VERSION(3, 18, 0)) {
				gtk_style_context_get_border(context, GTK_STATE_FLAG_NORMAL,
						&tmp);
			} else {
				gtk_style_context_get_border(context,
						gtk_widget_get_state_flags(handle), &tmp);
			}
			e->result->x -= tmp.left;
			e->result->y -= tmp.top;
			e->result->width += tmp.left + tmp.right;
			e->result->height += tmp.top + tmp.bottom;
		}
		GdkRectangle icon_area;
		gtk_entry_get_icon_area(GTK_ENTRY(_W_WIDGET(widget)->handle),
				GTK_ENTRY_ICON_PRIMARY, &icon_area);
		e->result->x -= icon_area.width;
		e->result->width += icon_area.width;
		gtk_entry_get_icon_area(GTK_ENTRY(_W_WIDGET(widget)->handle),
				GTK_ENTRY_ICON_SECONDARY, &icon_area);
		e->result->width += icon_area.width;
#endif
#if GTK2
#endif
	} else {
		int borderWidth = gtk_container_get_border_width(
				GTK_CONTAINER(_W_WIDGET(widget)->handle));
		xborder += borderWidth;
		yborder += borderWidth;
	}
	int property;
	gtk_widget_style_get(handle, "interior-focus", &property,
	NULL);
	if (property == 0) {
		gtk_widget_style_get(handle, "focus-line-width", &property, NULL);
		xborder += property;
		yborder += property;
	}
	e->result->x -= xborder;
	e->result->y -= yborder;
	e->result->width += 2 * xborder;
	e->result->height += 2 * yborder;
	e->result->width += SPACE_FOR_CURSOR;
	return W_TRUE;
}

wresult _w_textedit_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int w, h;
	w_event_compute_trim ee;
	w_rect rect, result;
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	if ((_W_WIDGET(widget)->style & W_SINGLE) != 0) {
		gtk_widget_realize(handle);
		PangoLayout *layout = gtk_entry_get_layout(
				GTK_ENTRY(_W_WIDGET(widget)->handle));
		pango_layout_get_pixel_size(layout, &w, &h);
	} else {
		GtkTextIter start, end;
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(_W_WIDGET(widget)->handle));
		gtk_text_buffer_get_bounds(bufferHandle, &start, &end);
		gchar *text = gtk_text_buffer_get_text(bufferHandle, &start, &end,
		TRUE);
		PangoLayout *layout = gtk_widget_create_pango_layout(handle, text);
		g_free(text);
		pango_layout_set_width(layout, e->wHint * PANGO_SCALE);
		pango_layout_get_pixel_size(layout, &w, &h);
		g_object_unref(layout);
	}
	int width = w;
	int height = h;
	char *message = _W_TEXTEDIT(widget)->message;
	if ((_W_WIDGET(widget)->style & W_SINGLE) != 0 && message != 0) {
		PangoLayout *layout = gtk_widget_create_pango_layout(handle, message);
		pango_layout_get_pixel_size(layout, &w, &h);
		g_object_unref(layout);
		width = WMAX(width, w);
	}
	if (width == 0)
		width = DEFAULT_WIDTH;
	if (height == 0)
		height = DEFAULT_HEIGHT;
	width = e->wHint == W_DEFAULT ? width : e->wHint;
	height = e->hHint == W_DEFAULT ? height : e->hHint;
	memset(&ee, 0, sizeof(ee));
	ee.rect = &rect;
	ee.result = &result;
	rect.x = 0;
	rect.y = 0;
	rect.width = width;
	rect.height = height;
	memset(&result, 0, sizeof(result));
	_w_textedit_compute_trim(widget, &ee, priv);
	e->size->width = result.width;
	e->size->height = result.height;
	return W_TRUE;
}
wresult _w_textedit_create_handle(w_widget *widget, _w_control_priv *priv) {
	GtkWidget *fixedHandle, *handle = 0, *scrolledHandle;
	GtkTextBuffer *bufferHandle;
	GtkStyleContext *context;
	wuint64 style = _W_WIDGET(widget)->style;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_MENU;
	if ((style & W_READ_ONLY) != 0) {
		if (_w_scrollable_apply_theme_background(W_SCROLLABLE(widget), priv)
				== 1) {
			_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
		}
	}
	fixedHandle = _w_fixed_new(0);
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	if ((style & W_SINGLE) != 0) {
		handle = gtk_entry_new();
		if (handle == 0)
			goto _err;
		_w_fixed_set_child(fixedHandle, handle);
		gtk_editable_set_editable(GTK_EDITABLE(handle),
				(style & W_READ_ONLY) == 0);
		/*
		 * We need to handle borders differently in GTK3.20+. GtkEntry without frame will have a blank background color.
		 * So let's set border via css and override the background in this case to be COLOR_LIST_BACKGROUND.
		 */
		if (GTK_VERSION >= VERSION(3, 20, 0)) {
			if ((style & W_BORDER) == 0) {
				gtk_entry_set_has_frame(GTK_ENTRY(handle), FALSE);
				context = gtk_widget_get_style_context(handle);
				//String background = display.gtk_rgba_to_css_string(display.COLOR_LIST_BACKGROUND_RGBA);
				//gtk_css_provider_load_from_css(context, "entry {border: solid; background: " + background + ";}");
				gtk_style_context_invalidate(context);
			}
		} else {
			gtk_entry_set_has_frame(GTK_ENTRY(handle), (style & W_BORDER) != 0);
		}
		gtk_entry_set_visibility(GTK_ENTRY(handle), (style & W_PASSWORD) == 0);
		float alignment = 0.0f;
		if ((style & W_CENTER) != 0)
			alignment = 0.5f;
		if ((style & W_RIGHT) != 0)
			alignment = 1.0f;
		if (alignment > 0.0f) {
			gtk_entry_set_alignment(GTK_ENTRY(handle), alignment);
		}
	} else {
		scrolledHandle = gtk_scrolled_window_new(0, 0);
		if (scrolledHandle == 0)
			goto _err;
		handle = gtk_text_view_new();
		if (handle == 0)
			goto _err;
		bufferHandle = gtk_text_view_get_buffer(GTK_TEXT_VIEW(handle));
		if (bufferHandle == 0)
			goto _err;
		g_object_set_qdata(G_OBJECT(scrolledHandle), gtk_toolkit->quark[0],
				widget);
		_w_fixed_set_child(fixedHandle, scrolledHandle);
		gtk_container_add(GTK_CONTAINER(scrolledHandle), handle);
		gtk_text_view_set_editable(GTK_TEXT_VIEW(handle),
				(style & W_READ_ONLY) == 0);
		if ((style & W_WRAP) != 0)
			gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(handle),
					GTK_WRAP_WORD_CHAR);
		GtkPolicyType hsp = (style & W_HSCROLL) != 0 ? /*GTK_POLICY_ALWAYS*/
		GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER;
		GtkPolicyType vsp = (style & W_VSCROLL) != 0 ? /*GTK_POLICY_ALWAYS*/
		GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER;
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle), hsp,
				vsp);
		if ((style & W_BORDER) != 0) {
			gtk_scrolled_window_set_shadow_type(
					GTK_SCROLLED_WINDOW(scrolledHandle), GTK_SHADOW_ETCHED_IN);
		}
		GtkJustification just = GTK_JUSTIFY_LEFT;
		if ((style & W_CENTER) != 0)
			just = GTK_JUSTIFY_CENTER;
		if ((style & W_RIGHT) != 0)
			just = GTK_JUSTIFY_RIGHT;
		gtk_text_view_set_justification(GTK_TEXT_VIEW(handle), just);
	}
#if GTK3
	//imContext = imContextLast();
	if ((style & W_SINGLE) != 0) {
		gtk_entry_set_width_chars(GTK_ENTRY(handle), 6);
	}
	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
	/*w_font *font = _w_control_default_font(W_CONTROL(widget));
	 _w_control_set_font_description(W_CONTROL(widget), _W_FONT(font)->handle,
	 priv);*/
#endif
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);

	_W_WIDGET(widget)->handle = handle;
	gtk_widget_show_all(fixedHandle);
	_W_WIDGET(widget)->state |= STATE_TEXTEDIT_DOUBLE_CLICK;
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_textedit_cut(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkEditable *editable = GTK_EDITABLE(_W_WIDGET(text)->handle);
		gtk_editable_cut_clipboard(editable);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(_W_WIDGET(text)->handle));
#if GTK3
		GtkClipboard *clipboard = gtk_clipboard_get(GDK_NONE);
#endif
#if GTK4
#endif
		_w_textedit_clear_segments(text, TRUE);
		gtk_text_buffer_cut_clipboard(bufferHandle, clipboard,
				gtk_text_view_get_editable(
						GTK_TEXT_VIEW(_W_WIDGET(text)->handle)));
		_w_textedit_apply_segments(text);
	}
	return W_TRUE;
}
GtkWidget* _w_textedit_handle_scrolled(w_widget *control,
		_w_control_priv *priv) {
	if ((_W_WIDGET(control)->style & W_SINGLE) != 0) {
		return 0;
	} else {
		GtkWidget *handle = _W_WIDGET(control)->handle;
		return gtk_widget_get_parent(handle);
	}
}
GtkWidget* _w_textedit_handle_fixed(w_widget *control, _w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(control)->handle;
	if ((_W_WIDGET(control)->style & W_SINGLE) != 0) {
		return gtk_widget_get_parent(handle);
	} else {
		return gtk_widget_get_parent(gtk_widget_get_parent(handle));
	}
}
wresult _w_textedit_get_caret_line_number(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0)
		return 0;
	GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
			GTK_TEXT_VIEW(_W_WIDGET(text)->handle));
	GtkTextIter position;
	GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
	gtk_text_buffer_get_iter_at_mark(bufferHandle, &position, mark);
	return gtk_text_iter_get_line(&position);
}
wresult _w_textedit_get_caret_location(w_textedit *text, w_point *location) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkEntry *entry = GTK_ENTRY(_W_WIDGET(text)->handle);
		int index = gtk_editable_get_position(GTK_EDITABLE(entry));
		index = gtk_entry_text_index_to_layout_index(entry, index);
		gint offset_x = 0, offset_y = 0;
		gtk_entry_get_layout_offsets(entry, &offset_x, &offset_y);
		PangoLayout *layout = gtk_entry_get_layout(entry);
		PangoRectangle pos;
		pango_layout_index_to_pos(layout, index, &pos);
		int border = W_CONTROL_GET_CLASS(text)->get_border_width(
				W_CONTROL(text));
		location->x = offset_x + PANGO_PIXELS(pos.x) - border;
		location->y = offset_y + PANGO_PIXELS(pos.y);
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(_W_WIDGET(text)->handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter position;
		GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
		gtk_text_buffer_get_iter_at_mark(bufferHandle, &position, mark);
		GdkRectangle rect;
		gtk_text_view_get_iter_location(textview, &position, &rect);
		gint x = 0;
		gint y = 0;
		gtk_text_view_buffer_to_window_coords(textview, GTK_TEXT_WINDOW_TEXT,
				rect.x, rect.y, &x, &y);
		location->x = x;
		location->y = y;
	}
	return W_TRUE;
}
wresult _w_textedit_get_caret_position(w_textedit *text, int enc) {
	int result;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		return gtk_editable_get_position(GTK_EDITABLE(_W_WIDGET(text)->handle));
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(_W_WIDGET(text)->handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter position, zero;
		GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
		gtk_text_buffer_get_iter_at_mark(bufferHandle, &position, mark);
		return gtk_text_iter_get_offset(&position);
	}
}
wresult _w_textedit_get_char_count(w_textedit *text, int enc) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		const gchar *str = gtk_entry_get_text(
				GTK_ENTRY(_W_WIDGET(text)->handle));
		return strlen(str);
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(_W_WIDGET(text)->handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter startIter, endIter;
		gtk_text_buffer_get_bounds(bufferHandle, &startIter, &endIter);
		return gtk_text_iter_get_offset(&endIter);
	}
}
wresult _w_textedit_get_doubleclick_enabled(w_textedit *text) {
	return (_W_WIDGET(text)->state & STATE_TEXTEDIT_DOUBLE_CLICK) != 0;
}
wresult _w_textedit_get_echo_char(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkEntry *entry = GTK_ENTRY(_W_WIDGET(text)->handle);
		if (!gtk_entry_get_visibility(entry)) {
			return gtk_entry_get_invisible_char(entry);
		}
	}
	return '\0';
}
wresult _w_textedit_get_editable(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		return gtk_editable_get_editable(GTK_EDITABLE(_W_WIDGET(text)->handle));
	}
	return gtk_text_view_get_editable(GTK_TEXT_VIEW(_W_WIDGET(text)->handle));
}
wresult _w_textedit_get_line_count(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0)
		return 1;
	GtkTextView *textview = GTK_TEXT_VIEW(_W_WIDGET(text)->handle);
	GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
	return gtk_text_buffer_get_line_count(bufferHandle);
}
wresult _w_textedit_get_line_delimiter(w_textedit *text) {
	return '\n';
}
wresult _w_textedit_get_line_height(w_textedit *text) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(text);
	/*const PangoFontDescription *desc = _w_control_get_get_font_description(
	 W_CONTROL(text), priv);
	 _w_widget_font_height(desc, _W_WIDGET(text)->handle);*/
	return W_TRUE;
}
wresult _w_textedit_get_message(w_textedit *text, w_alloc alloc,
		void *user_data, int enc) {
	char *message = _W_TEXTEDIT(text)->message;
	return _gtk_alloc_set_text(alloc, user_data, message, -1, enc);
}
wresult _w_textedit_get_position(w_textedit *text, w_point *point, int enc) {
	int position = -1;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gint index;
		gint trailing;
		PangoLayout *layout = gtk_entry_get_layout(GTK_ENTRY(handle));
		pango_layout_xy_to_index(layout, point->x * PANGO_SCALE,
				point->y * PANGO_SCALE, &index, &trailing);
		const gchar *ptr = pango_layout_get_text(layout);
		if (enc == W_ENCODING_UNICODE) {
			//position = (int)g_utf16_pointer_to_offset (ptr, &ptr[index]);
		} else {
			position = index + trailing;
		}
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter p, zero;
		gtk_text_view_get_iter_at_location(textview, &p, point->x, point->y);
		gtk_text_buffer_get_iter_at_offset(bufferHandle, &zero, 0);
		gchar *ptr = gtk_text_buffer_get_text(bufferHandle, &zero, &p, TRUE);
		position = gtk_text_iter_get_offset(&p);
		if (enc == W_ENCODING_UNICODE) {
			//position = g_utf8_offset_to_utf16_offset(ptr, position);
		}
		g_free(ptr);
	}
	return position;
}
wresult _w_textedit_get_selection(w_textedit *text, w_range *range, int enc) {
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gint start;
		gint end;
		gtk_editable_get_selection_bounds(GTK_EDITABLE(handle), &start, &end);
		const gchar *ptr = gtk_entry_get_text(GTK_ENTRY(handle));
		if (enc == W_ENCODING_UNICODE) {
			//start = g_utf8_offset_to_utf16_offset(ptr, start);
			//end = g_utf8_offset_to_utf16_offset(ptr, end);
		}
		range->start = start;
		range->end = end;
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter startIter, endIter, zero;
		gtk_text_buffer_get_selection_bounds(bufferHandle, &startIter,
				&endIter);
		gtk_text_buffer_get_iter_at_offset(bufferHandle, &zero, 0);
		gchar *ptr = gtk_text_buffer_get_text(bufferHandle, &zero, &endIter,
		TRUE);
		range->start = gtk_text_iter_get_offset(&startIter);
		range->end = gtk_text_iter_get_offset(&endIter);
		if (enc == W_ENCODING_UNICODE) {
			//range->start = g_utf8_offset_to_utf16_offset(ptr, range->start);
			//range->end = g_utf8_offset_to_utf16_offset(ptr, range->end);
		}
		g_free(ptr);
	}
	return W_TRUE;
}
wresult _w_textedit_get_selection_count(w_textedit *text, int enc) {
	w_range selection;
	_w_textedit_get_selection(text, &selection, enc);
	return abs(selection.end - selection.start);
}
wresult _w_textedit_get_selection_text(w_textedit *text, w_alloc alloc,
		void *user_data, int enc) {
	GtkWidget *handle = _W_WIDGET(text)->handle;
	gint start;
	gint end;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gtk_editable_get_selection_bounds(GTK_EDITABLE(handle), &start, &end);
		const gchar *ptr = gtk_entry_get_text(GTK_ENTRY(handle));
		_gtk_alloc_set_text(alloc, user_data, &ptr[start], abs(end - start),
				enc);
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter startIter, endIter, zero;
		gtk_text_buffer_get_selection_bounds(bufferHandle, &startIter,
				&endIter);
		gtk_text_buffer_get_iter_at_offset(bufferHandle, &zero, 0);
		gchar *ptr = gtk_text_buffer_get_text(bufferHandle, &zero, &endIter,
		TRUE);
		start = gtk_text_iter_get_offset(&startIter);
		end = gtk_text_iter_get_offset(&endIter);
		_gtk_alloc_set_text(alloc, user_data, &ptr[start], abs(end - start),
				enc);
		g_free(ptr);
	}
	return W_TRUE;
}
int _w_textedit_get_tab_width(w_textedit *text) {
	GtkWidget *handle = _W_WIDGET(text)->handle;
	PangoLayout *layout = gtk_widget_create_pango_layout(handle, " ");
	gint width;
	gint height;
	pango_layout_get_size(layout, &width, &height);
	g_object_unref(layout);
	return width;
}
wresult _w_textedit_get_tabs(w_textedit *text) {
	GtkWidget *handle = _W_WIDGET(text)->handle;
	gint start;
	gint end;
	PangoTabArray *tabs;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		tabs = gtk_entry_get_tabs(GTK_ENTRY(handle));
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(handle);
		tabs = gtk_text_view_get_tabs(textview);
	}
	gint tabWidth;
	PangoTabAlign align;
	pango_tab_array_get_tab(tabs, 0, &align, &tabWidth);
	gint width = _w_textedit_get_tab_width(text);
	return tabWidth / width;
}
wresult _w_textedit_get_text(w_textedit *text, w_alloc alloc, void *user_data,
		int enc) {
	gchar *address;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		address = (gchar*) gtk_entry_get_text(GTK_ENTRY(handle));
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(handle));
		GtkTextIter start;
		GtkTextIter end;
		gtk_text_buffer_get_bounds(bufferHandle, &start, &end);
		address = gtk_text_buffer_get_text(bufferHandle, &start, &end, TRUE);
	}
	if (address == 0) {
		return _gtk_alloc_set_text(alloc, user_data, address, -1, enc);
	} else {
		wresult ret = _gtk_alloc_set_text(alloc, user_data, address, -1, enc);
		if ((_W_WIDGET(text)->style & W_MULTI) != 0)
			g_free(address);
		return ret;
	}
}
wresult _w_textedit_get_text_range(w_textedit *text, w_range *range,
		w_alloc alloc, void *user_data, int enc) {
	return W_FALSE;
}
wresult _w_textedit_get_text_limit(w_textedit *text, int enc) {
	if ((_W_WIDGET(text)->style & W_MULTI) != 0)
		return LIMIT;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	int limit = gtk_entry_get_max_length(GTK_ENTRY(handle));
	return limit == 0 ? 0xFFFF : limit;
}
wresult _w_textedit_get_top_index(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0)
		return 0;
	GtkTextIter position;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	/*
	 * Feature in GTK: GtkTextView widgets are subject to line validation
	 * which happens during idle. This causes GtkTextIter to not update quickly
	 * enough when changes are added to the text buffer. The fix is to use a
	 * GtkTextMark to track the precise index, then convert it back to a
	 * GtkTextIter when getTopIndex() is called. See bug 487467.
	 *
	 * NOTE: to cover cases where getTopIndex() is called without setTopIndex()
	 * being called, we fetch the current GtkAdjustment value and cache it for
	 * comparison. In getTopIndex() we compare the current value with the cached
	 * one to see if the user has scrolled/moved the viewport using the GUI.
	 * If so, we use the old method of fetching the top index.
	 */
#if GTK3
	GtkAdjustment *vAdjustment = gtk_scrollable_get_vadjustment(
			GTK_SCROLLABLE(handle));
	_W_TEXTEDIT(text)->currentAdjustment = gtk_adjustment_get_value(
			vAdjustment);
	if (_W_TEXTEDIT(text)->cachedAdjustment
			== _W_TEXTEDIT(text)->currentAdjustment) {
		// If indexMark is 0, fetch topIndex using the old method
		if (_W_TEXTEDIT(text)->indexMark != 0) {
			GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
					GTK_TEXT_VIEW(handle));
			gtk_text_buffer_get_iter_at_mark(bufferHandle, &position,
			_W_TEXTEDIT(text)->indexMark);
			return gtk_text_iter_get_line(&position);
		}
	}
#endif
	GdkRectangle rect;
	gtk_text_view_get_visible_rect(GTK_TEXT_VIEW(handle), &rect);
	gtk_text_view_get_line_at_y(GTK_TEXT_VIEW(handle), &position, rect.y, NULL);
	return gtk_text_iter_get_line(&position);
}
wresult _w_textedit_get_top_pixel(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0)
		return 0;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	GtkTextIter position;
	GdkRectangle rect;
	gtk_text_view_get_visible_rect(GTK_TEXT_VIEW(handle), &rect);
	int lineTop;
	gtk_text_view_get_line_at_y(GTK_TEXT_VIEW(handle), &position, rect.y,
			&lineTop);
	return lineTop;
}
wresult _w_textedit_insert(w_textedit *text, const char *string, size_t length,
		int enc) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	_w_textedit_clear_segments(text, TRUE);
	GtkWidget *handle = _W_WIDGET(text)->handle;
	int newlength, mnemonic;
	char *s = _gtk_text_fix(string, length, enc | _GTK_TEXT_FIX_REQUIRED_NULL,
			&newlength, &mnemonic);
	if (s == 0)
		return W_ERROR_NO_MEMORY;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		int start, end;
		gtk_editable_get_selection_bounds(GTK_EDITABLE(handle), &start, &end);
		gtk_editable_delete_selection(GTK_EDITABLE(handle));
		gtk_editable_insert_text(GTK_EDITABLE(handle), s, newlength, &start);
		gtk_editable_set_position(GTK_EDITABLE(handle), start);
	} else {
		GtkTextIter start;
		GtkTextIter end;
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(handle));
		if (gtk_text_buffer_get_selection_bounds(bufferHandle, &start, &end)) {
			gtk_text_buffer_delete(bufferHandle, &start, &end);
		}
		gtk_text_buffer_insert(bufferHandle, &start, s, newlength);
		gtk_text_buffer_place_cursor(bufferHandle, &start);
		GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
		gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(handle), mark, 0, TRUE, 0,
				0);
	}
	_w_textedit_apply_segments(text);
	return W_TRUE;
}
wresult _w_textedit_paste(w_textedit *text) {
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gtk_editable_paste_clipboard(GTK_EDITABLE(handle));
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(handle));
		GtkClipboard *clipboard = gtk_clipboard_get(GDK_NONE);
		_w_textedit_clear_segments(text, TRUE);
		gtk_text_buffer_paste_clipboard(bufferHandle, clipboard, 0,
				gtk_text_view_get_editable(GTK_TEXT_VIEW(handle)));
		_w_textedit_apply_segments(text);
	}
	return W_TRUE;
}
wresult _w_textedit_select_all(w_textedit *text) {
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gtk_editable_select_region(GTK_EDITABLE(handle), 0, -1);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(handle));
		GtkTextIter start;
		GtkTextIter end;
		gtk_text_buffer_get_iter_at_offset(bufferHandle, &start, 0);
		gtk_text_buffer_get_end_iter(bufferHandle, &end);
		gtk_text_buffer_select_range(bufferHandle, &start, &end);
	}
	return W_TRUE;
}
wresult _w_textedit_set_doubleclick_enabled(w_textedit *text, int doubleClick) {
	if (doubleClick) {
		_W_WIDGET(text)->state |= STATE_TEXTEDIT_DOUBLE_CLICK;
	} else {
		_W_WIDGET(text)->state &= ~STATE_TEXTEDIT_DOUBLE_CLICK;
	}
	return W_TRUE;
}
wresult _w_textedit_set_echochar(w_textedit *text, int echo) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkWidget *handle = _W_WIDGET(text)->handle;
		gtk_entry_set_visibility(GTK_ENTRY(handle), echo == '\0');
		gtk_entry_set_invisible_char(GTK_ENTRY(handle), echo);
	}
	return W_TRUE;
}
wresult _w_textedit_set_editable(w_textedit *text, int editable) {
	_W_WIDGET(text)->style &= ~W_READ_ONLY;
	if (!editable)
		_W_WIDGET(text)->style |= W_READ_ONLY;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gtk_editable_set_editable(GTK_EDITABLE(handle), editable);
	} else {
		gtk_text_view_set_editable(GTK_TEXT_VIEW(handle), editable);
	}
	return W_TRUE;
}
wresult _w_textedit_set_message(w_textedit *text, const char *message,
		size_t length, int enc) {
	int newlength, mnemonic;
	_W_TEXTEDIT(text)->message = _gtk_text_fix(message, length,
			enc | _GTK_TEXT_FIX_REQUIRED_NULL | _GTK_TEXT_FIX_COPY, &newlength,
			&mnemonic);
	return _W_TEXTEDIT(text)->message != 0 ? W_TRUE : W_FALSE;
}
wresult _w_textedit_set_selection(w_textedit *text, int start, int enc) {
	return W_FALSE;
}
wresult _w_textedit_set_selection_range(w_textedit *text, w_range *selection,
		int enc) {
	return W_FALSE;
}
void _w_textedit_set_tab_stops(w_textedit *text, int tabs) {
	int tabWidth = _w_textedit_get_tab_width(text);
	tabWidth = tabWidth * tabs;
	PangoTabArray *tabArray = pango_tab_array_new(1, FALSE);
	pango_tab_array_set_tab(tabArray, 0, PANGO_TAB_LEFT, tabWidth);
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gtk_entry_set_tabs(GTK_ENTRY(_W_WIDGET(text)->handle), tabArray);
	} else {
		gtk_text_view_set_tabs(GTK_TEXT_VIEW(_W_WIDGET(text)->handle),
				tabArray);
	}
	pango_tab_array_free(tabArray);
}
wresult _w_textedit_set_tabs(w_textedit *text, int tabs) {
	if (tabs < 0)
		return W_FALSE;
	_w_textedit_set_tab_stops(text, tabs);
	return W_TRUE;
}
wresult _w_textedit_set_text(w_textedit *text, const char *string,
		size_t length, int enc) {
	wresult result = W_ERROR_NO_MEMORY;
	int newlength, mnemonic;
	char *s = _gtk_text_fix(string, length, enc | _GTK_TEXT_FIX_REQUIRED_NULL,
			&newlength, &mnemonic);
	_w_textedit_clear_segments(text, FALSE);
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
				(void*) SIGNAL_CHANGED);
		g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
				(void*) SIGNAL_DELETE_TEXT);
		g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
				(void*) SIGNAL_INSERT_TEXT);
		gtk_entry_set_text(GTK_ENTRY(handle), s);
		g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
				0, (void*) SIGNAL_CHANGED);
		g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
				0, (void*) SIGNAL_DELETE_TEXT);
		g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0, 0,
				0, (void*) SIGNAL_INSERT_TEXT);
		result = W_TRUE;
		_gtk_text_free(string, s, newlength);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(handle));
		g_signal_handlers_block_matched(bufferHandle, G_SIGNAL_MATCH_DATA, 0, 0,
				0, 0, (void*) SIGNAL_CHANGED);
		g_signal_handlers_block_matched(bufferHandle, G_SIGNAL_MATCH_DATA, 0, 0,
				0, 0, (void*) SIGNAL_DELETE_RANGE);
		g_signal_handlers_block_matched(bufferHandle, G_SIGNAL_MATCH_DATA, 0, 0,
				0, 0, (void*) SIGNAL_TEXT_BUFFER_INSERT_TEXT);
		gtk_text_buffer_set_text(bufferHandle, s, newlength);
		g_signal_handlers_unblock_matched(bufferHandle, G_SIGNAL_MATCH_DATA, 0,
				0, 0, 0, (void*) SIGNAL_CHANGED);
		g_signal_handlers_unblock_matched(bufferHandle, G_SIGNAL_MATCH_DATA, 0,
				0, 0, 0, (void*) SIGNAL_DELETE_RANGE);
		g_signal_handlers_unblock_matched(bufferHandle, G_SIGNAL_MATCH_DATA, 0,
				0, 0, 0, (void*) SIGNAL_TEXT_BUFFER_INSERT_TEXT);
		result = W_TRUE;
		_gtk_text_free(string, s, newlength);
		if (result > 0) {
			GtkTextIter position;
			gtk_text_buffer_get_iter_at_offset(bufferHandle, &position, 0);
			gtk_text_buffer_place_cursor(bufferHandle, &position);
			GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
			gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(handle), mark, 0, TRUE,
					0, 0);
		}
	}
	if (result > 0) {
		w_event e;
		e.type = W_EVENT_MODIFY;
		e.widget = W_WIDGET(text);
		e.platform_event = 0;
		e.time = 0;
		e.data = 0;
		_w_widget_send_event(W_WIDGET(text), &e,W_EVENT_SEND);
		if ((_W_WIDGET(text)->style & W_SEARCH) != 0) {
			if ((_W_WIDGET(text)->style & W_ICON_CANCEL) != 0) {
				gtk_entry_set_icon_sensitive(GTK_ENTRY(handle),
						GTK_ENTRY_ICON_SECONDARY, TRUE);
			}
		}
	}
	_w_textedit_apply_segments(text);
	return result;
}
wresult _w_textedit_set_text_limit(w_textedit *text, int limit) {
	if (limit == 0)
		return W_ERROR_CANNOT_BE_ZERO;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkWidget *handle = _W_WIDGET(text)->handle;
		int max = limit;
		gtk_entry_set_max_length(GTK_ENTRY(handle), max);
	}
	return W_TRUE;
}
wresult _w_textedit_set_top_index(w_textedit *text, int index) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0)
		return W_TRUE;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
			GTK_TEXT_VIEW(handle));
	GtkTextIter position;
	gtk_text_buffer_get_iter_at_line(bufferHandle, &position, index);
#if GTK3
	/*
	 * Feature in GTK: create a new GtkTextMark for the purposes of
	 * keeping track of the top index. In getTopIndex() we can use this
	 * without worrying about line validation. See bug 487467.
	 *
	 * We also cache the current GtkAdjustment value for future comparison
	 * in getTopIndex().
	 */
	_W_TEXTEDIT(text)->indexMark = gtk_text_buffer_create_mark(bufferHandle,
			"index_mark", &position, TRUE);
	gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(handle),
	_W_TEXTEDIT(text)->indexMark, 0, TRUE, 0, 0);
	GtkAdjustment *vAdjustment = gtk_scrollable_get_vadjustment(
			GTK_SCROLLABLE(handle));
	_W_TEXTEDIT(text)->cachedAdjustment = gtk_adjustment_get_value(vAdjustment);
#endif
#if GTK2
		gtk_text_view_scroll_to_iter (GTK_TEXT_VIEW(handle), &position, 0, TRUE, 0, 0);
#endif
	return W_TRUE;
}
wresult _w_textedit_show_selection(w_textedit *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0)
		return W_TRUE;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
			GTK_TEXT_VIEW(handle));
	GtkTextMark *mark = gtk_text_buffer_get_selection_bound(bufferHandle);
	gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(handle), mark, 0, TRUE, 0, 0);
	mark = gtk_text_buffer_get_insert(bufferHandle);
	gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(handle), mark, 0, TRUE, 0, 0);
	return W_TRUE;
}
wresult _w_textedit_dispose_class(struct _w_widget_class *clazz){
	return W_TRUE;
}
void _w_textedit_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_textedit_class *clazz) {
	if (classId == _W_CLASS_TEXTEDIT) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&gtk_toolkit->class_textedit_priv;
	}
	_w_scrollable_class_init(toolkit, classId,W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TEXTEDIT;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_textedit_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_textedit);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_textedit);
	/*
	 * public function
	 */
	W_WIDGET_CLASS(clazz)->dispose_class = _w_textedit_dispose_class;
	clazz->append = _w_textedit_append;
	clazz->clear_selection = _w_textedit_clear_selection;
	clazz->copy = _w_textedit_copy;
	clazz->cut = _w_textedit_cut;
	clazz->get_caret_line_number = _w_textedit_get_caret_line_number;
	clazz->get_caret_location = _w_textedit_get_caret_location;
	clazz->get_caret_position = _w_textedit_get_caret_position;
	clazz->get_char_count = _w_textedit_get_char_count;
	clazz->get_doubleclick_enabled = _w_textedit_get_doubleclick_enabled;
	clazz->get_echo_char = _w_textedit_get_echo_char;
	clazz->get_editable = _w_textedit_get_editable;
	clazz->get_line_count = _w_textedit_get_line_count;
	clazz->get_line_delimiter = _w_textedit_get_line_delimiter;
	clazz->get_line_height = _w_textedit_get_line_height;
	clazz->get_message = _w_textedit_get_message;
	clazz->get_position = _w_textedit_get_position;
	clazz->get_selection = _w_textedit_get_selection;
	clazz->get_selection_count = _w_textedit_get_selection_count;
	clazz->get_selection_text = _w_textedit_get_selection_text;
	clazz->get_tabs = _w_textedit_get_tabs;
	clazz->get_text = _w_textedit_get_text;
	clazz->get_text_range = _w_textedit_get_text_range;
	clazz->get_text_limit = _w_textedit_get_text_limit;
	clazz->get_top_index = _w_textedit_get_top_index;
	clazz->get_top_pixel = _w_textedit_get_top_pixel;
	clazz->insert = _w_textedit_insert;
	clazz->paste = _w_textedit_paste;
	clazz->select_all = _w_textedit_select_all;
	clazz->set_doubleclick_enabled = _w_textedit_set_doubleclick_enabled;
	clazz->set_echochar = _w_textedit_set_echochar;
	clazz->set_editable = _w_textedit_set_editable;
	clazz->set_message = _w_textedit_set_message;
	clazz->set_selection = _w_textedit_set_selection;
	clazz->set_selection_range = _w_textedit_set_selection_range;
	clazz->set_tabs = _w_textedit_set_tabs;
	clazz->set_text = _w_textedit_set_text;
	clazz->set_text_limit = _w_textedit_set_text_limit;
	clazz->set_top_index = _w_textedit_set_top_index;
	clazz->show_selection = _w_textedit_show_selection;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_TEXTEDIT) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->handle_fixed = _w_textedit_handle_fixed;
		priv->widget.create_handle = _w_textedit_create_handle;
		priv->widget.check_style = _w_textedit_check_style;
		priv->widget.handle_top = _w_textedit_handle_fixed;
		_W_SCROLLABLE_PRIV(priv)->handle_scrolled = _w_textedit_handle_scrolled;
		_W_SCROLLABLE_PRIV(priv)->apply_theme_background =
				_w_textedit_apply_theme_background;
		priv->widget.compute_size = _w_textedit_compute_size;
		priv->widget.compute_trim = _w_textedit_compute_trim;
	}
}
