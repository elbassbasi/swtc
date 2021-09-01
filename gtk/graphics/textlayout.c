/*
 * Name:        textlayout.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
#include "../widgets/toolkit.h"

void w_textstyle_init(w_textstyle *style) {
	memset(style, 0, sizeof(w_textstyle));
}
void w_textlayout_init(w_textlayout *textlayout) {
	_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
	_layout->layout = 0;
}
void w_textlayout_dispose(w_textlayout *textlayout) {
	_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
	if (_layout->layout != 0) {
		PangoContext *context = pango_layout_get_context(_layout->layout);
		if (_layout->layout != 0)
			g_object_unref(_layout->layout);
		if (context != 0)
			g_object_unref(context);
		_layout->layout = 0;
	}
}
wresult w_textlayout_isok(w_textlayout *textlayout) {
	_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
	if (textlayout == 0)
		return W_FALSE;
	return _layout->layout != 0;
}
wresult w_textlayout_check(w_textlayout *textlayout) {
	_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
	if (textlayout == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_layout->layout == 0) {
		return W_ERROR_NO_HANDLES;
	}
	return W_TRUE;
}
int _w_textlayout_translate_offset(w_textlayout *textlayout, int offset) {
	_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
	if (_layout->is_ascii)
		return offset;
	PangoLayout *layout = _layout->layout;
	const char *ptr = pango_layout_get_text(layout);
	int length = _layout->text_length;
	if (offset < 0 || offset > length)
		offset = length;
	if (offset < length) {
		while (offset >= 0 && ptr[offset] >= 128) {
			offset--;
		}
	}
	return offset;
}
/*
 *  Translate an internal offset to a client offset
 */
int _w_textlayout_untranslate_offset(w_textlayout *textlayout, int offset) {
	_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
	if (_layout->is_ascii)
		return offset;
	int length = _layout->text_length;
	if (length == 0)
		return offset;
	int i = 0;
	/*while (i < invalidOffsets.length && offset > ptr[i]) {
	 i++;
	 }*/
	return offset /*- i*/;
}
wresult w_textlayout_create(w_textlayout *textlayout) {
	w_textlayout_dispose(textlayout);
	_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
	PangoContext *context = gdk_pango_context_get();
	if (context == 0) {
		return W_ERROR_NO_HANDLES;
	}
	pango_context_set_language(context, gtk_get_default_language());
	pango_context_set_base_dir(context, PANGO_DIRECTION_LTR);
	PangoLayout *layout = pango_layout_new(context);
	if (layout == 0) {
		return W_ERROR_NO_HANDLES;
	}
	memset(textlayout, 0, sizeof(_w_textlayout));
	w_font *sysfont = w_toolkit_get_system_font(0);
	PangoFontDescription *hfont = (PangoFontDescription*) sysfont;
	pango_layout_set_font_description(layout, hfont);
	pango_layout_set_wrap(layout, PANGO_WRAP_WORD_CHAR);
	pango_layout_set_tabs(layout, gtk_toolkit->empty_tab);
	pango_layout_set_auto_dir(layout, FALSE);
	_layout->layout = layout;
	_layout->wrapWidth = -1;
	_layout->ascentInPoints = -1;
	_layout->descentInPoints = -1;
	return W_TRUE;
}
int _w_textlayout_width(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		int wrapWidth = pango_layout_get_width(_layout->layout);
		if (wrapWidth != -1)
			return PANGO_PIXELS(wrapWidth);
		int w, h;
		pango_layout_get_pixel_size(_layout->layout, &w, &h);
		return w;
	}
	return 0;
}
wresult w_textlayout_draw(w_textlayout *textlayout, w_graphics *gc,
		w_rect *rect) {
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		int x = rect->x;
		int y = rect->y;
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		_w_graphics_check(gc, GRAPHICS_STATE_FOREGROUND);
		int length = pango_layout_get_character_count(layout);
		x += MIN(_layout->indent, _layout->wrapIndent);
		cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
		if (length == 0)
			return W_FALSE;
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			cairo_save(cairo);
			cairo_scale(cairo, -1, 1);
			cairo_translate(cairo, -2 * x - _w_textlayout_width(textlayout), 0);
		}
		cairo_move_to(cairo, x, y);
		pango_cairo_show_layout(cairo, layout);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			cairo_restore(cairo);
		}
		cairo_new_path(cairo);
	}
	return result;
}
void w_textlayout_draw_border(w_textlayout *textlayout, w_graphics *gc, int x,
		int y, int start, int end, int style, w_color color) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

	}
}
int w_textlayout_get_alignment(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		int align = pango_layout_get_alignment(_layout->layout);
		PangoContext *context = pango_layout_get_context(_layout->layout);
		int rtl = pango_context_get_base_dir(context) == PANGO_DIRECTION_RTL;
		switch (align) {
		case PANGO_ALIGN_LEFT:
			return rtl ? W_RIGHT : W_LEFT;
		case PANGO_ALIGN_RIGHT:
			return rtl ? W_LEFT : W_RIGHT;
		}
		return W_CENTER;
	}
	return W_LEFT;
}
int w_textlayout_get_ascent(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		return _layout->ascentInPoints;
	}
	return result;
}
void w_textlayout_get_bounds(w_textlayout *textlayout, int start, int end,
		w_rect *rect, int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		int length = pango_layout_get_character_count(layout);
		if (length == 0) {
			memset(rect, 0, sizeof(w_rect));
			return;
		}
		if (end < 0)
			end = length - 1;
		if (start > end) {
			memset(rect, 0, sizeof(w_rect));
			return;
		}
		start = WMIN(WMAX(0, start), length - 1);
		end = WMIN(WMAX(0, end), length - 1);
		const char *ptr = pango_layout_get_text(layout);
		int byteStart;
		int byteEnd;
		int strlen = _layout->text_length;
		if (_layout->is_ascii) {
			byteStart = start;
			byteEnd = end + 1;
		} else {
			byteStart = start;
			byteEnd = end;
			//byteStart = g_utf16_offset_to_pointer (ptr, start) - ptr;
			//byteEnd = g_utf16_offset_to_pointer (ptr, end + 1) - ptr;
		}
		byteStart = WMIN(byteStart, strlen);
		byteEnd = WMIN(byteEnd, strlen);
		gint ranges[2] = { byteStart, byteEnd };
		cairo_region_t *clipRegion = gdk_pango_layout_get_clip_region(layout, 0,
				0, ranges, 1);
		if (clipRegion == 0) {
			memset(rect, 0, sizeof(w_rect));
			return;
		}
		GdkRectangle _rect;

		/*
		 * Bug in Pango. The region returned by gdk_pango_layout_get_clip_region()
		 * includes areas from lines outside of the requested range.  The fix
		 * is to subtract these areas from the clip region.
		 */
		PangoRectangle pangoRect;
		PangoLayoutIter *iter = pango_layout_get_iter(layout);
		if (iter == 0) {
			memset(rect, 0, sizeof(w_rect));
			return /*W_ERROR_NO_HANDLES*/;
		}
		cairo_region_t *linesRegion = cairo_region_create();
		if (linesRegion == 0) {
			pango_layout_iter_free(iter);
			memset(rect, 0, sizeof(w_rect));
			return /*W_ERROR_NO_HANDLES*/;
		}
		int lineEnd = 0;
		do {
			pango_layout_iter_get_line_extents(iter, 0, &pangoRect);
			if (pango_layout_iter_next_line(iter)) {
				lineEnd = pango_layout_iter_get_index(iter) - 1;
			} else {
				lineEnd = strlen;
			}
			if (byteStart > lineEnd)
				continue;
			_rect.x = PANGO_PIXELS(pangoRect.x);
			_rect.y = PANGO_PIXELS(pangoRect.y);
			_rect.width = PANGO_PIXELS(pangoRect.width);
			_rect.height = PANGO_PIXELS(pangoRect.height);
			cairo_region_union_rectangle(linesRegion, &_rect);
		} while (lineEnd + 1 <= byteEnd);
		cairo_region_intersect(clipRegion, linesRegion);
		cairo_region_destroy(linesRegion);
		pango_layout_iter_free(iter);

		cairo_region_get_extents(clipRegion, &_rect);
		cairo_region_destroy(clipRegion);
		PangoContext *context = pango_layout_get_context(layout);
		if (pango_context_get_base_dir(context) == PANGO_DIRECTION_RTL) {
			int w, h;
			int wrapWidth = pango_layout_get_width(layout);
			if (wrapWidth != -1) {
				w = PANGO_PIXELS(wrapWidth);
			} else {
				w = 0, h = 0;
				pango_layout_get_pixel_size(layout, &w, &h);
			}
			_rect.x = w - _rect.x - _rect.width;
		}
		_rect.x += WMIN(_layout->indent, _layout->wrapIndent);
		rect->x = _rect.x;
		rect->y = _rect.y;
		rect->width = _rect.width;
		rect->height = _rect.height;
		return;
	}
}
int w_textlayout_get_descent(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		return _layout->descentInPoints;
	}
	return 0;
}
w_font* w_textlayout_get_font(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		return _layout->font;
	}
	return 0;
}
int w_textlayout_get_indent(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		return _layout->indent;
	}
	return 0;
}
wresult w_textlayout_get_justify(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		return pango_layout_get_justify(layout);
	}
	return result;
}
int w_textlayout_get_level(w_textlayout *textlayout, int offset, int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		int length = pango_layout_get_character_count(layout);
		if (!(0 <= offset && offset <= length)) {
			//return W_ERROR_INVALID_RANGE;
			return 0;
		}
		PangoLayoutIter *iter = pango_layout_get_iter(layout);
		if (iter == 0) {
			//return W_ERROR_NO_HANDLES;
			return 0;
		}
		int level = 0;
		PangoItem *item;
		const char *ptr = pango_layout_get_text(layout);
		int strlen = _layout->text_length;
		int byteOffset;
		if (_layout->is_ascii) {
			byteOffset = offset;
		} else {
			//offset = translateOffset(offset);
			//byteOffset = OS.g_utf16_offset_to_pointer(ptr, offset) - ptr;
			byteOffset = offset;
		}
		byteOffset = WMIN(byteOffset, strlen);
		do {
			PangoLayoutRun *run = pango_layout_iter_get_run(iter);
			if (run != 0) {
				item = run->item;
				if (item->offset <= byteOffset
						&& byteOffset < item->offset + item->length) {
					level = item->analysis.level;
					break;
				}
			}
		} while (pango_layout_iter_next_run(iter));
		pango_layout_iter_free(iter);
		return level;

	}
	return result;
}
void w_textlayout_get_line_bounds(w_textlayout *textlayout, int lineIndex,
		w_rect *_rect) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		int lineCount = pango_layout_get_line_count(layout);
		if (!(0 <= lineIndex && lineIndex < lineCount))
			return /*W_ERROR_INVALID_RANGE*/;
		PangoLayoutIter *iter = pango_layout_get_iter(layout);
		if (iter == 0)
			return /*W_ERROR_NO_HANDLES*/;
		for (int i = 0; i < lineIndex; i++)
			pango_layout_iter_next_line(iter);
		PangoRectangle rect;
		pango_layout_iter_get_line_extents(iter, 0, &rect);
		pango_layout_iter_free(iter);
		int x = PANGO_PIXELS(rect.x);
		int y = PANGO_PIXELS(rect.y);
		int width = PANGO_PIXELS(rect.width);
		int height = PANGO_PIXELS(rect.height);
		if (_layout->ascentInPoints != -1 && _layout->descentInPoints != -1) {
			height = WMAX(height,
					_layout->ascentInPoints + _layout->descentInPoints);
		}
		PangoContext *context = pango_layout_get_context(layout);
		if (pango_context_get_base_dir(context) == PANGO_DIRECTION_RTL) {
			int w, h;
			int wrapWidth = pango_layout_get_width(layout);
			if (wrapWidth != -1) {
				w = PANGO_PIXELS(wrapWidth);
			} else {
				w = 0, h = 0;
				pango_layout_get_pixel_size(layout, &w, &h);
			}
			x = w - x - width;
		}
		x += WMIN(_layout->indent, _layout->wrapIndent);
		_rect->x = x;
		_rect->y = y;
		_rect->width = width;
		_rect->height = height;
	}
}
int w_textlayout_get_line_count(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		return pango_layout_get_line_count(layout);
	}
	return result;
}
int w_textlayout_get_line_index(w_textlayout *textlayout, int offset, int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		int length = pango_layout_get_character_count(layout);
		if (!(0 <= offset && offset <= length)) {
			return W_ERROR_INVALID_ARGUMENT;
		}
		int line = 0;
		const char *ptr = pango_layout_get_text(layout);
		int byteOffset;
		if (_layout->is_ascii) {
			byteOffset = offset;
		} else {
			//offset = translateOffset(offset);
			//byteOffset = g_utf16_offset_to_pointer(ptr,offset) - ptr;
			byteOffset = offset;
		}
		int strlen = _layout->text_length;
		byteOffset = WMIN(byteOffset, strlen);
		PangoLayoutIter *iter = pango_layout_get_iter(layout);
		if (iter == 0) {
			return W_ERROR_NO_HANDLES;
		}
		while (pango_layout_iter_next_line(iter)) {
			if (pango_layout_iter_get_index(iter) > byteOffset)
				break;
			line++;
		}
		pango_layout_iter_free(iter);
		return line;
	}
	return result;
}
wresult w_textlayout_get_line_metrics(w_textlayout *textlayout, int lineIndex,
		w_fontmetrics *fontmetrics) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		PangoLayoutLine *line = pango_layout_get_line(layout, lineIndex);
		if (line == 0)
			return W_ERROR_INVALID_RANGE;
		int heightInPoints;
		int ascentInPoints;
		if (line->runs == 0) {
			PangoFontDescription *font;
			if (_layout->font != 0) {
				font = (PangoFontDescription*) _layout->font;
			} else {
				font = (PangoFontDescription*) w_toolkit_get_system_font(0);
			}
			PangoContext *context = pango_layout_get_context(layout);
			PangoLanguage *lang = pango_context_get_language(context);
			PangoFontMetrics *metrics = pango_context_get_metrics(context, font,
					lang);
			int ascent = pango_font_metrics_get_ascent(metrics);
			int descent = pango_font_metrics_get_descent(metrics);
			ascentInPoints = PANGO_PIXELS(ascent);
			heightInPoints = PANGO_PIXELS(ascent + descent);
			pango_font_metrics_unref(metrics);
		} else {
			PangoRectangle rect;
			pango_layout_line_get_extents(
					pango_layout_get_line(layout, lineIndex), 0, &rect);
			ascentInPoints = PANGO_PIXELS(-rect.y);
			heightInPoints = PANGO_PIXELS(rect.height);
		}
		heightInPoints = WMAX(
				_layout->ascentInPoints + _layout->descentInPoints,
				heightInPoints);
		ascentInPoints = WMAX(_layout->ascentInPoints, ascentInPoints);
		int descentInPoints = heightInPoints - ascentInPoints;
		_w_fontmetrics *_metrics = (_w_fontmetrics*) fontmetrics;
		_metrics->ascentInPoints = ascentInPoints;
		_metrics->descentInPoints = descentInPoints;
		_metrics->averageCharWidthInPoints = 0;
		return W_TRUE;
	}
	return result;
}
int w_textlayout_get_line_offset(w_textlayout *textlayout, int lineIndex,
		int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		int lineCount = pango_layout_get_line_count(layout);
		const char *ptr = pango_layout_get_text(layout);
		PangoLayoutLine *line = pango_layout_get_line(layout, lineIndex);
		if (line == 0)
			return W_ERROR_INVALID_RANGE;
		if (_layout->is_ascii) {
			return line->start_index;
		} else {
			//int pos = g_utf16_pointer_to_offset(ptr, ptr + line.start_index);
			//offsets = untranslateOffset(pos);
			return line->start_index;
		}
	}
	return result;
}
wresult w_textlayout_get_location(w_textlayout *textlayout, int offset,
		int trailing, w_point *pt, int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		int length = pango_layout_get_character_count(layout);
		if (!(0 <= offset && offset <= length)) {
			return W_ERROR_INVALID_RANGE;
		}

		const char *ptr = pango_layout_get_text(layout);
		int byteOffset;
		if (_layout->is_ascii) {
			byteOffset = offset;
		} else {
			//offset = translateOffset(offset);
			//byteOffset =g_utf16_offset_to_pointer(ptr, offset) - ptr;
			byteOffset = offset;
		}

		int strlen = _layout->text_length;
		byteOffset = WMIN(byteOffset, strlen);
		PangoRectangle pos;
		pango_layout_index_to_pos(layout, byteOffset, &pos);
		int x = trailing ? pos.x + pos.width : pos.x;
		int y = pos.y;
		x = PANGO_PIXELS(x);
		PangoContext *context = pango_layout_get_context(layout);
		if (pango_context_get_base_dir(context) == PANGO_DIRECTION_RTL) {
			int w, h;
			int wrapWidth = pango_layout_get_width(layout);
			if (wrapWidth != -1) {
				w = PANGO_PIXELS(wrapWidth);
			} else {
				w = 0, h = 0;
				pango_layout_get_pixel_size(layout, &w, &h);
			}
			x = w - x;
		}
		x += WMIN(_layout->indent, _layout->wrapIndent);
		pt->x = x;
		pt->y = PANGO_PIXELS(y);
		return W_TRUE;
	}
	return result;
}
int w_textlayout_get_next_offset(w_textlayout *textlayout, int offset,
		int movement, int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

	}
	return result;
}
int w_textlayout_get_offset(w_textlayout *textlayout, w_point *point,
		int *trailing, int trailing_count, int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

	}
	return result;
}
int w_textlayout_get_orientation(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		;
		PangoContext *context = pango_layout_get_context(layout);
		int baseDir = pango_context_get_base_dir(context);
		return baseDir == PANGO_DIRECTION_RTL ?
				W_RIGHT_TO_LEFT : W_LEFT_TO_RIGHT;
	}
	return result;
}
int w_textlayout_get_previous_offset(w_textlayout *textlayout, int offset,
		int movement, int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

	}
	return result;
}
typedef struct w_iterator_textlayout_ranges {
	w_basic_iterator base;
	PangoAttrList *attrList;
	PangoAttrIterator *iter;
	gboolean next;
	size_t count;
} w_iterator_textlayout_ranges;
wresult w_iterator_textlayout_ranges_close(w_iterator *_it) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) _it;
	pango_attr_iterator_destroy(it->iter);
	return W_TRUE;
}
wresult w_iterator_textlayout_ranges_next(w_iterator *_it, void *obj) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) _it;
	gboolean ret = it->next;
	PangoAttrColor *color;
	PangoAttrInt *attrInt;
	if (ret) {
		w_textstyle_range *range = (w_textstyle_range*) obj;
		pango_attr_iterator_range(it->iter, &range->range.start,
				&range->range.end);
		color = (PangoAttrColor*) pango_attr_iterator_get(it->iter,
				PANGO_ATTR_FOREGROUND);
		if (color != 0) {
			range->style.foreground = W_RGB(color->color.red / 0x100,
					color->color.green / 0x100, color->color.blue / 0x100);
		} else {
			range->style.foreground = 0;
		}
		color = (PangoAttrColor*) pango_attr_iterator_get(it->iter,
				PANGO_ATTR_BACKGROUND);
		if (color != 0) {
			range->style.background = W_RGB(color->color.red / 0x100,
					color->color.green / 0x100, color->color.blue / 0x100);
		} else {
			range->style.background = 0;
		}
		attrInt = (PangoAttrInt*) pango_attr_iterator_get(it->iter,
				PANGO_ATTR_UNDERLINE);
		if (attrInt != 0) {
			range->style.underline = attrInt->value != PANGO_UNDERLINE_NONE;
			range->style.underlineStyle = W_UNDERLINE_SINGLE;
			switch (attrInt->value) {
			case PANGO_UNDERLINE_DOUBLE:
				range->style.underlineStyle = W_UNDERLINE_DOUBLE;
				break;
			case PANGO_UNDERLINE_ERROR:
				range->style.underlineStyle = W_UNDERLINE_ERROR;
				break;
			}
			if (range->style.underline) {
				color = (PangoAttrColor*) pango_attr_iterator_get(it->iter,
						PANGO_ATTR_UNDERLINE_COLOR);
				if (color != 0) {
					range->style.underlineColor = W_RGB(
							color->color.red / 0x100,
							color->color.green / 0x100,
							color->color.blue / 0x100);
				}
			}
		}
		it->next = pango_attr_iterator_next(it->iter);
	}
	return ret;
}
wresult w_iterator_textlayout_ranges_reset(w_iterator *_it) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) _it;
	pango_attr_iterator_destroy(it->iter);
	PangoAttrList *attrList = it->attrList;
	it->iter = pango_attr_list_get_iterator(attrList);
	return W_TRUE;
}
wresult w_iterator_textlayout_ranges_remove(w_iterator *_it) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) _it;
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t w_iterator_textlayout_ranges_get_count(w_iterator *_it) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) _it;
	return 0;

}
struct _w_iterator_class w_iterator_textlayout_ranges_class = { //
		w_iterator_textlayout_ranges_close, //
				w_iterator_textlayout_ranges_next, //
				w_iterator_textlayout_ranges_reset, //
				w_iterator_textlayout_ranges_remove, //
				w_iterator_textlayout_ranges_get_count, //
		};
wresult _w_attr_list_get_ranges(PangoAttrList *attrList, w_iterator *ranges) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) ranges;
	it->base.clazz = &w_iterator_textlayout_ranges_class;
	it->attrList = attrList;
	it->iter = pango_attr_list_get_iterator(attrList);
	it->next = TRUE;
	it->count = -1;
	return W_TRUE;
}
wresult w_textlayout_get_ranges(w_textlayout *textlayout, w_iterator *ranges) {
	w_iterator_close(ranges);
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoAttrList *attrList = pango_layout_get_attributes(_layout->layout);
		return _w_attr_list_get_ranges(attrList, ranges);
	}
	return result;
}
int w_textlayout_get_spacing(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

		PangoLayout *layout = _layout->layout;
		return pango_layout_get_spacing(layout);
	}
	return result;
}
wresult w_textlayout_get_style(w_textlayout *textlayout, int offset,
		w_textstyle *style, int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

	}
	return result;
}
int w_textlayout_get_tabs(w_textlayout *textlayout, int *tabs, int count) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

	}
	return result;
}
wresult w_textlayout_get_text(w_textlayout *textlayout, w_alloc text,
		void *user_data, int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

		PangoLayout *layout = _layout->layout;
		const char *_text = pango_layout_get_text(layout);
		return w_alloc_set_text(text, user_data, enc, _text, -1,
				W_ENCODING_UTF8);
	}
	return result;
}
int w_textlayout_get_text_direction(w_textlayout *textlayout) {
	return w_textlayout_get_orientation(textlayout);
}
int w_textlayout_get_width(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

		return _layout->wrapWidth;
	}
	return result;
}
int w_textlayout_get_wrap_indent(w_textlayout *textlayout) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

		return _layout->wrapIndent;
	}
	return result;
}
wresult w_textlayout_set_alignment(w_textlayout *textlayout, int alignment) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

		PangoLayout *layout = _layout->layout;
		int mask = W_LEFT | W_CENTER | W_RIGHT;
		alignment &= mask;
		if (alignment == 0)
			return W_TRUE;
		if ((alignment & W_LEFT) != 0)
			alignment = W_LEFT;
		if ((alignment & W_RIGHT) != 0)
			alignment = W_RIGHT;
		PangoContext *context = pango_layout_get_context(_layout->layout);
		gboolean rtl = pango_context_get_base_dir(context)
				== PANGO_DIRECTION_RTL;
		PangoAlignment align = PANGO_ALIGN_CENTER;
		switch (alignment) {
		case W_LEFT:
			align = rtl ? PANGO_ALIGN_RIGHT : PANGO_ALIGN_LEFT;
			break;
		case W_RIGHT:
			align = rtl ? PANGO_ALIGN_LEFT : PANGO_ALIGN_RIGHT;
			break;
		}
		pango_layout_set_alignment(layout, align);
	}
	return result;
}
wresult w_textlayout_set_ascent(w_textlayout *textlayout, int ascent) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

		if (ascent < -1)
			return W_ERROR_INVALID_ARGUMENT;
		if (_layout->ascentInPoints == ascent)
			return W_TRUE;
		//freeRuns();
		_layout->ascentInPoints = ascent;
	}
	return result;
}
wresult w_textlayout_set_descent(w_textlayout *textlayout, int descent) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

		if (descent < -1)
			return W_ERROR_INVALID_ARGUMENT;
		if (_layout->descentInPoints == descent)
			return W_TRUE;
		//freeRuns();
		_layout->descentInPoints = descent;
	}
	return result;
}
wresult w_textlayout_set_font(w_textlayout *textlayout, w_font *font) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

		PangoLayout *layout = _layout->layout;
		w_font *oldFont = _layout->font;
		if (oldFont == font)
			return W_TRUE;
		//freeRuns();
		_layout->font = font;
		if (oldFont != 0 && oldFont == font)
			return W_TRUE;
		PangoFontDescription *_font;
		if (font == 0) {
			_font = (PangoFontDescription*) w_toolkit_get_system_font(0);
		} else {
			_font = (PangoFontDescription*) font;
		}
		pango_layout_set_font_description(layout,
				(PangoFontDescription*) _font);
	}
	return result;
}
wresult w_textlayout_set_indent(w_textlayout *textlayout, int indent) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

		PangoLayout *layout = _layout->layout;
		if (indent < 0)
			return W_TRUE;
		if (_layout->indent == indent)
			return W_TRUE;
		_layout->indent = indent;
		pango_layout_set_indent(layout,
				(indent - _layout->wrapIndent) * PANGO_SCALE);
		if (_layout->wrapWidth != -1) {
			int margin = MIN(indent, _layout->wrapIndent);
			pango_layout_set_width(layout,
					(_layout->wrapWidth - margin) * PANGO_SCALE);
		}
	}
	return result;
}
wresult w_textlayout_set_justify(w_textlayout *textlayout, int justify) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		pango_layout_set_justify(layout, justify);
	}
	return result;
}
wresult w_textlayout_set_orientation(w_textlayout *textlayout,
		int orientation) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);

		PangoLayout *layout = _layout->layout;
		int mask = W_RIGHT_TO_LEFT | W_LEFT_TO_RIGHT;
		orientation &= mask;
		if (orientation == 0)
			return W_TRUE;
		if ((orientation & W_LEFT_TO_RIGHT) != 0)
			orientation = W_LEFT_TO_RIGHT;
		PangoDirection baseDir =
				orientation == W_RIGHT_TO_LEFT ?
						PANGO_DIRECTION_RTL : PANGO_DIRECTION_LTR;
		PangoContext *context = pango_layout_get_context(_layout->layout);
		if (pango_context_get_base_dir(context) == baseDir)
			return W_TRUE;
		//freeRuns();
		pango_context_set_base_dir(context, baseDir);
		pango_layout_context_changed(layout);
		PangoAlignment align = pango_layout_get_alignment(layout);
		if (align != PANGO_ALIGN_CENTER) {
			align = align == PANGO_ALIGN_LEFT ?
					PANGO_ALIGN_RIGHT : PANGO_ALIGN_LEFT;
			pango_layout_set_alignment(layout, align);
		}
	}
	return result;
}
wresult w_textlayout_set_spacing(w_textlayout *textlayout, int spacing) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		if (spacing < 0)
			return W_ERROR_INVALID_ARGUMENT;
		PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
		pango_layout_set_spacing(layout, spacing * PANGO_SCALE);
	}
	return result;
}
wresult w_textlayout_set_style_0(w_textlayout *textlayout, w_textstyle *style,
		int byteStart, int byteEnd) {
	_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
	if (style == 0)
		return W_FALSE;
	if ((style->flags & W_TEXTSTYLE_MASK_ALL) == 0)
		return W_FALSE;
	PangoAttribute *attr;
	PangoAttrList *attrList = pango_layout_get_attributes(_layout->layout);
	if (attrList == 0) {
		attrList = pango_attr_list_new();
		pango_layout_set_attributes(_layout->layout, attrList);
	}
	w_font *font = style->font;
	if ((style->flags & W_TEXTSTYLE_MASK_FONT) != 0 && font != 0
			&& font != _layout->font) {
		PangoAttribute *attr = pango_attr_font_desc_new(
				(PangoFontDescription*) font);
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
	}
	if ((style->flags & W_TEXTSTYLE_MASK_UNDERLINE) != 0 && style->underline) {
		PangoUnderline underlineStyle = PANGO_UNDERLINE_NONE;
		switch (style->underlineStyle) {
		case W_UNDERLINE_SINGLE:
			underlineStyle = PANGO_UNDERLINE_SINGLE;
			break;
		case W_UNDERLINE_DOUBLE:
			underlineStyle = PANGO_UNDERLINE_DOUBLE;
			break;
		case W_UNDERLINE_SQUIGGLE:
		case W_UNDERLINE_ERROR:
			underlineStyle = PANGO_UNDERLINE_ERROR;
			break;
		case W_UNDERLINE_LINK: {
			if (style->foreground == 0) {
				// Bug 497071: use COLOR_LINK_FOREGROUND for StyledText links
				PangoAttribute *attr;
#if GTK3
				w_color linkRGBA = w_toolkit_get_system_color(0,
						W_COLOR_LINK_FOREGROUND);
				// Manual conversion since PangoAttrColor is a special case.
				// It uses GdkColor style colors but is supported on GTK3.
				attr = pango_attr_foreground_new(W_RED(linkRGBA),
						W_GREEN(linkRGBA), W_BLUE(linkRGBA));
#endif
#if GTK2
#endif
				attr->start_index = byteStart;
				attr->end_index = byteEnd;
				pango_attr_list_insert(attrList, attr);
			}
			underlineStyle = PANGO_UNDERLINE_SINGLE;
			break;
		}
		}
		PangoAttribute *attr = pango_attr_underline_new(underlineStyle);
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
		if (style->underlineColor != 0) {
#if GTK3
			attr = pango_attr_underline_color_new(
			W_RED(style->underlineColor) * 0x100,
			W_GREEN(style->underlineColor) * 0x100,
			W_BLUE(style->underlineColor) * 0x100);
#endif
#if GTK2
#endif
			if (attr != 0) {
				attr->start_index = byteStart;
				attr->end_index = byteEnd;
				pango_attr_list_insert(attrList, attr);
			}
		}
	}
	if ((style->flags & W_TEXTSTYLE_MASK_STRIKEOUT) != 0 && style->strikeout) {
		attr = pango_attr_strikethrough_new(TRUE);
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
		if (style->strikeoutColor != 0) {
#if GTK3
			attr = pango_attr_strikethrough_color_new(
			W_RED(style->strikeoutColor) * 0x100,
			W_GREEN(style->strikeoutColor) * 0x100,
			W_BLUE(style->strikeoutColor) * 0x100);
#endif
#if GTK2
#endif
			if (attr != 0) {
				attr->start_index = byteStart;
				attr->end_index = byteEnd;
				pango_attr_list_insert(attrList, attr);
			}
		}
	}
	w_color foreground = style->foreground;
	if ((style->flags & W_TEXTSTYLE_MASK_FOREGROUND) != 0 && foreground != 0) {
#if GTK3
		attr = pango_attr_foreground_new(W_RED(style->foreground) * 0x100,
		W_GREEN(style->foreground) * 0x100,
		W_BLUE(style->foreground) * 0x100);
#endif
#if GTK2
#endif
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
	}
	w_color background = style->background;
	if ((style->flags & W_TEXTSTYLE_MASK_BACKGROUND) != 0 && background != 0) {
#if GTK3
		attr = pango_attr_background_new(W_RED(style->background) * 0x100,
		W_GREEN(style->background) * 0x100,
		W_BLUE(style->background) * 0x100);
#endif
#if GTK2
#endif
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
	}
	w_glyphmetrics *metrics = &style->metrics;
	if ((style->flags & W_TEXTSTYLE_MASK_METRICS) != 0
			&& metrics->width != (wushort) -1 && metrics->ascent != (wushort) -1
			&& metrics->descent != (wushort) -1) {
		int width = metrics->width == -1 ? 0 : metrics->width;
		int ascent = metrics->ascent == -1 ? 0 : metrics->ascent;
		int descent = metrics->descent == -1 ? 0 : metrics->descent;
		PangoRectangle rect;
		rect.x = 0;
		rect.y = -ascent * PANGO_SCALE;
		rect.height = ascent + descent * PANGO_SCALE;
		rect.width = width * PANGO_SCALE;
		PangoAttribute *attr = pango_attr_shape_new(&rect, &rect);
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
	}
	int rise = style->rise;
	if ((style->flags & W_TEXTSTYLE_MASK_RISE) != 0 && rise != 0) {
		PangoAttribute *attr = pango_attr_rise_new(rise * PANGO_SCALE);
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
	}
	return W_TRUE;
}
wresult w_textlayout_set_style(w_textlayout *textlayout, w_textstyle *style,
		int start, int end, int enc) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		const char *ptr = pango_layout_get_text(_layout->layout);
		//start = _w_textlayout_translate_offset(textlayout,start);
		//end = _w_textlayout_translate_offset(textlayout,end);
		return w_textlayout_set_style_0(textlayout, style, start, end);
	}
	return result;
}
wresult w_textlayout_set_tabs(w_textlayout *textlayout, int *tabs, int count) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		if (tabs == 0) {
			pango_layout_set_tabs(layout, gtk_toolkit->empty_tab);
		} else {
			PangoTabArray *tabArray = pango_tab_array_new(count, TRUE);
			if (tabArray != 0) {
				for (int i = 0; i < count; i++) {
					pango_tab_array_set_tab(tabArray, i, PANGO_TAB_LEFT,
							tabs[i]);
				}
				pango_layout_set_tabs(layout, tabArray);
				pango_tab_array_free(tabArray);
			}
		}
		/*
		 * Bug in Pango. A change in the tab stop array is not automatically reflected in the
		 * pango layout object because the call pango_layout_set_tabs() does not free the
		 * lines cache. The fix to use pango_layout_context_changed() to free the lines cache.
		 */
		pango_layout_context_changed(layout);
	}
	return result;
}
wresult w_textlayout_set_text(w_textlayout *textlayout, const char *text,
		int count) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		_layout->is_ascii = W_TRUE;
		if (count < 0) {
			count = -1;
			const unsigned char *s = (const unsigned char*) text;
			while (*s != 0) {
				if (*s > 0x7f) {
					_layout->is_ascii = W_FALSE;
					break;
				}
				s++;
			}
			while (*s != 0)
				s++;
			_layout->text_length = (intptr_t) s - (intptr_t) text;
		} else {
			const unsigned char *s = (const unsigned char*) text;
			int i = 0;
			while (i < count) {
				if (s[i] > 0x7f) {
					_layout->is_ascii = W_FALSE;
					break;
				}
				i++;
			}
			_layout->text_length = count;
		}
		pango_layout_set_text(layout, text, count);
	}
	return result;
}
wresult w_textlayout_set_text_segments(w_textlayout *textlayout,
		const char *text, int count, int *segments, wuint *segmentschars,
		int nsegments) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {

	}
	return result;
}
wresult w_textlayout_set_text_direction(w_textlayout *textlayout,
		int textDirection) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {

	}
	return result;
}
wresult w_textlayout_set_width(w_textlayout *textlayout, int width) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		_layout->wrapWidth = width;
		if (_layout->wrapWidth == -1) {
			pango_layout_set_width(_layout->layout, -1);
			PangoContext *context = pango_layout_get_context(_layout->layout);
			int rtl = pango_context_get_base_dir(context)
					== PANGO_DIRECTION_RTL;
			pango_layout_set_alignment(_layout->layout,
					rtl ? PANGO_ALIGN_RIGHT : PANGO_ALIGN_LEFT);
		} else {
			int margin = WMIN(_layout->indent, _layout->wrapIndent);
			pango_layout_set_width(_layout->layout,
					(_layout->wrapWidth - margin) * PANGO_SCALE);
		}
	}
	return result;
}
wresult w_textlayout_set_wrap_indent(w_textlayout *textlayout, int wrapIndent) {
	wresult result = w_textlayout_check(textlayout);
	if (result > 0) {
		_w_textlayout *_layout = _W_TEXTLAYOUT(textlayout);
		PangoLayout *layout = _layout->layout;
		if (wrapIndent < 0)
			return W_ERROR_NO_INDEX;
		if (_layout->wrapIndent == wrapIndent)
			return W_TRUE;
		_layout->wrapIndent = wrapIndent;
		pango_layout_set_indent(layout,
				(_layout->indent - wrapIndent) * PANGO_SCALE);
		if (_layout->wrapWidth != -1) {
			int margin = MIN(_layout->indent, wrapIndent);
			pango_layout_set_width(layout,
					(_layout->wrapWidth - margin) * PANGO_SCALE);
		}
	}
	return result;
}
