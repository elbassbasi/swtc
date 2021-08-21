/*
 * Name:        textlayout.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_TEXTLAYOUT_H_
#define SWT_GRAPHICS_TEXTLAYOUT_H_
#include "graphics.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_glyphmetrics {
	wushort ascent;
	wushort descent;
	wushort width;
} w_glyphmetrics;
typedef struct w_textstyle {
	union {
		struct {
			unsigned mask_metrics :1;
			unsigned mask_rise :1;
			unsigned mask_font :1;
			unsigned mask_foreground :1;
			unsigned mask_background :1;
			unsigned mask_underline :1;
			unsigned mask_strikeout :1;
			/**
			 * the underline flag of the style. The default underline
			 * style is <code>W_UNDERLINE_SINGLE</code>.
			 */
			unsigned underline :1;
			/**
			 * the strikeout flag of the style
			 */
			unsigned strikeout :1;
			/**
			 * the underline style. This style is ignored when
			 * <code>underline</code> is false.
			 * <p>
			 * This value should be one of <code>W_UNDERLINE_SINGLE</code>,
			 * <code>W_UNDERLINE_DOUBLE</code>, <code>W_UNDERLINE_ERROR</code>,
			 * <code>W_UNDERLINE_SQUIGGLE</code>, or <code>W_UNDERLINE_LINK</code>.
			 * </p>
			 *
			 * @see SWT#UNDERLINE_SINGLE
			 * @see SWT#UNDERLINE_DOUBLE
			 * @see SWT#UNDERLINE_ERROR
			 * @see SWT#UNDERLINE_SQUIGGLE
			 * @see SWT#UNDERLINE_LINK
			 */
			unsigned underlineStyle :3;
		};
		wushort flags;
	};
	/**
	 * the baseline rise of the style.
	 */
	wushort rise;
	/**
	 * the foreground of the style
	 */
	w_color foreground;
	/**
	 * the background of the style
	 */
	w_color background;
	/**
	 * the underline color of the style
	 */
	w_color underlineColor;
	/**
	 * the strikeout color of the style
	 */
	w_color strikeoutColor;
	/**
	 * the font of the style
	 */
	w_font *font;
	/**
	 * the GlyphMetrics of the style
	 */
	w_glyphmetrics metrics;
} w_textstyle;
enum {
	_W_TEXTSTYLE_METRICS = 0,
	_W_TEXTSTYLE_RISE = 1,
	_W_TEXTSTYLE_FONT = 2,
	_W_TEXTSTYLE_FOREGROUND = 3,
	_W_TEXTSTYLE_BACKGROUND = 4,
	_W_TEXTSTYLE_UNDERLINE = 5,
	_W_TEXTSTYLE_STRIKEOUT = 6,
	W_TEXTSTYLE_MASK_METRICS = 1 << _W_TEXTSTYLE_METRICS,
	W_TEXTSTYLE_MASK_RISE = 1 << _W_TEXTSTYLE_RISE,
	W_TEXTSTYLE_MASK_FONT = 1 << _W_TEXTSTYLE_FONT,
	W_TEXTSTYLE_MASK_FOREGROUND = 1 << _W_TEXTSTYLE_FOREGROUND,
	W_TEXTSTYLE_MASK_BACKGROUND = 1 << _W_TEXTSTYLE_BACKGROUND,
	W_TEXTSTYLE_MASK_UNDERLINE = 1 << _W_TEXTSTYLE_UNDERLINE,
	W_TEXTSTYLE_MASK_STRIKEOUT = 1 << _W_TEXTSTYLE_STRIKEOUT,
	W_TEXTSTYLE_MASK_ALL = W_TEXTSTYLE_MASK_METRICS | W_TEXTSTYLE_MASK_RISE
			| W_TEXTSTYLE_MASK_FONT | W_TEXTSTYLE_MASK_FOREGROUND
			| W_TEXTSTYLE_MASK_BACKGROUND | W_TEXTSTYLE_MASK_UNDERLINE
			| W_TEXTSTYLE_MASK_STRIKEOUT,
	W_TEXTSTYLE_UNDERLINE = 1 << 7,
	W_TEXTSTYLE_STRIKEOUT = 1 << 8,
	W_TEXTSTYLE_UNDERLINE_STYLE = 0x7 << 9,
}
;
typedef struct w_textstyle_range {
	w_textstyle style;
	w_range range;
} w_textstyle_range;

SWT_PUBLIC void w_textstyle_init(w_textstyle *style);
typedef struct w_textlayout_line {
	void *handle[0x10];
} w_textlayout_line;
#define W_TEXTLAYOUT_LINE(x) ((w_textlayout_line*)x)
typedef struct w_textlayout_iter {
	void *handle[0x20];
} w_textlayout_iter;
#define W_TEXTLAYOUT_ITER(x) ((w_textlayout_iter*)x)
SWT_PUBLIC wresult w_textlayout_iter_init(w_textlayout_iter *iter);
SWT_PUBLIC wresult w_textlayout_iter_dispose(w_textlayout_iter *iter);
SWT_PUBLIC wresult w_textlayout_iter_next_run(w_textlayout_iter *iter);
SWT_PUBLIC wresult w_textlayout_iter_next_char(w_textlayout_iter *iter);
SWT_PUBLIC wresult w_textlayout_iter_next_cluster(w_textlayout_iter *iter);
SWT_PUBLIC wresult w_textlayout_iter_next_line(w_textlayout_iter *iter);
SWT_PUBLIC wresult w_textlayout_iter_get_line(w_textlayout_iter *iter,
		w_textlayout_line **line);
typedef struct w_textlayout {
	void *handle[0x20];
} w_textlayout;
#define W_TEXTLAYOUT(x) ((w_textlayout*)x)

SWT_PUBLIC void w_textlayout_init(w_textlayout *textlayout);
SWT_PUBLIC void w_textlayout_dispose(w_textlayout *textlayout);
SWT_PUBLIC wresult w_textlayout_isok(w_textlayout *textlayout);
SWT_PUBLIC wresult w_textlayout_create(w_textlayout *textlayout);
SWT_PUBLIC wresult w_textlayout_draw(w_textlayout *textlayout, w_graphics *gc,
		w_rect *rect);
void w_textlayout_draw_border(w_textlayout *textlayout, w_graphics *gc, int x,
		int y, int start, int end, int style, w_color color);
SWT_PUBLIC int w_textlayout_get_alignment(w_textlayout *textlayout);
SWT_PUBLIC int w_textlayout_get_ascent(w_textlayout *textlayout);
SWT_PUBLIC void w_textlayout_get_bounds(w_textlayout *textlayout, int start,
		int end, w_rect *rect, int enc);
SWT_PUBLIC int w_textlayout_get_descent(w_textlayout *textlayout);
SWT_PUBLIC w_font* w_textlayout_get_font(w_textlayout *textlayout);
SWT_PUBLIC int w_textlayout_get_indent(w_textlayout *textlayout);
SWT_PUBLIC wresult w_textlayout_get_justify(w_textlayout *textlayout);
SWT_PUBLIC int w_textlayout_get_level(w_textlayout *textlayout, int offset,
		int enc);
SWT_PUBLIC void w_textlayout_get_line_bounds(w_textlayout *textlayout,
		int lineIndex, w_rect *rect);
SWT_PUBLIC int w_textlayout_get_line_count(w_textlayout *textlayout);
SWT_PUBLIC int w_textlayout_get_line_index(w_textlayout *textlayout, int offset,
		int enc);
SWT_PUBLIC wresult w_textlayout_get_line_metrics(w_textlayout *textlayout,
		int lineIndex, w_fontmetrics *fontmetrics);
SWT_PUBLIC int w_textlayout_get_line_offset(w_textlayout *textlayout,
		int lineIndex, int enc);
SWT_PUBLIC wresult w_textlayout_get_location(w_textlayout *textlayout,
		int offset, int trailing, w_point *pt, int enc);
SWT_PUBLIC int w_textlayout_get_next_offset(w_textlayout *textlayout,
		int offset, int movement, int enc);
SWT_PUBLIC int w_textlayout_get_offset(w_textlayout *textlayout, w_point *point,
		int *trailing, int trailing_count, int enc);
SWT_PUBLIC int w_textlayout_get_orientation(w_textlayout *textlayout);
SWT_PUBLIC int w_textlayout_get_previous_offset(w_textlayout *textlayout,
		int offset, int movement, int enc);
SWT_PUBLIC wresult w_textlayout_get_ranges(w_textlayout *textlayout,
		w_iterator *ranges);
SWT_PUBLIC int w_textlayout_get_spacing(w_textlayout *textlayout);
SWT_PUBLIC wresult w_textlayout_get_style(w_textlayout *textlayout, int offset,
		w_textstyle *style, int enc);
SWT_PUBLIC int w_textlayout_get_tabs(w_textlayout *textlayout, int *tabs,
		int count);
SWT_PUBLIC wresult w_textlayout_get_text(w_textlayout *textlayout, w_alloc text,
		void *user_data, int enc);
SWT_PUBLIC int w_textlayout_get_text_direction(w_textlayout *textlayout);
SWT_PUBLIC int w_textlayout_get_width(w_textlayout *textlayout);
SWT_PUBLIC int w_textlayout_get_wrap_indent(w_textlayout *textlayout);
SWT_PUBLIC wresult w_textlayout_set_alignment(w_textlayout *textlayout,
		int alignment);
SWT_PUBLIC wresult w_textlayout_set_ascent(w_textlayout *textlayout,
		int ascent);
SWT_PUBLIC wresult w_textlayout_set_descent(w_textlayout *textlayout,
		int descent);
SWT_PUBLIC wresult w_textlayout_set_font(w_textlayout *textlayout,
		w_font *font);
SWT_PUBLIC wresult w_textlayout_set_indent(w_textlayout *textlayout,
		int indent);
SWT_PUBLIC wresult w_textlayout_set_justify(w_textlayout *textlayout,
		int justify);
SWT_PUBLIC wresult w_textlayout_set_orientation(w_textlayout *textlayout,
		int orientation);
SWT_PUBLIC wresult w_textlayout_set_spacing(w_textlayout *textlayout,
		int spacing);
SWT_PUBLIC wresult w_textlayout_set_style(w_textlayout *textlayout,
		w_textstyle *style, int start, int end, int enc);
SWT_PUBLIC wresult w_textlayout_set_tabs(w_textlayout *textlayout, int *tabs,
		int count);
SWT_PUBLIC wresult w_textlayout_set_text(w_textlayout *textlayout,
		const char *text, int count);
SWT_PUBLIC wresult w_textlayout_set_text_segments(w_textlayout *textlayout,
		const char *text, int count, int *segments, wuint *segmentschars,
		int nsegments);
SWT_PUBLIC wresult w_textlayout_set_text_direction(w_textlayout *textlayout,
		int textDirection);
SWT_PUBLIC wresult w_textlayout_set_width(w_textlayout *textlayout, int width);
SWT_PUBLIC wresult w_textlayout_set_wrap_indent(w_textlayout *textlayout,
		int wrapIndent);

#ifdef __cplusplus
}
#endif
#endif /* SWT_GRAPHICS_TEXTLAYOUT_H_ */
