/*
 * Name:        textlayout.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"

void w_textstyle_init(w_textstyle *style) {
}
void w_textlayout_init(w_textlayout *textlayout) {
}
void w_textlayout_dispose(w_textlayout *textlayout) {
}
wresult w_textlayout_isok(w_textlayout *textlayout) {
	return W_FALSE;
}
wresult w_textlayout_create(w_textlayout *textlayout) {
	return W_FALSE;
}
wresult w_textlayout_draw(w_textlayout *textlayout, w_graphics *gc, int x,
		int y, w_text_selection *selection, int flags) {
	return W_FALSE;
}
void w_textlayout_draw_border(w_textlayout *textlayout, w_graphics *gc, int x,
		int y, int start, int end, int style, w_color color) {
}
int w_textlayout_get_alignment(w_textlayout *textlayout) {
	return W_FALSE;
}
int w_textlayout_get_ascent(w_textlayout *textlayout) {
	return W_FALSE;
}
void w_textlayout_get_bounds(w_textlayout *textlayout, int start, int end,
		w_rect *rect, int enc) {
}
int w_textlayout_get_descent(w_textlayout *textlayout) {
	return W_FALSE;
}
w_font* w_textlayout_get_font(w_textlayout *textlayout) {
	return W_FALSE;
}
int w_textlayout_get_indent(w_textlayout *textlayout) {
	return W_FALSE;
}
wresult w_textlayout_get_justify(w_textlayout *textlayout) {
	return W_FALSE;
}
int w_textlayout_get_level(w_textlayout *textlayout, int offset, int enc) {
	return W_FALSE;
}
void w_textlayout_get_line_bounds(w_textlayout *textlayout, int lineIndex,
		w_rect *rect) {
}
int w_textlayout_get_line_count(w_textlayout *textlayout) {
	return W_FALSE;
}
int w_textlayout_get_line_index(w_textlayout *textlayout, int offset, int enc) {
	return W_FALSE;
}
wresult w_textlayout_get_line_metrics(w_textlayout *textlayout, int lineIndex,
		w_fontmetrics *fontmetrics) {
	return W_FALSE;
}
int w_textlayout_get_line_offset(w_textlayout *textlayout, int lineIndex,
		int enc) {
	return W_FALSE;
}
wresult w_textlayout_get_location(w_textlayout *textlayout, int offset,
		int trailing, w_point *pt, int enc) {
	return W_FALSE;
}
int w_textlayout_get_next_offset(w_textlayout *textlayout, int offset,
		int movement, int enc) {
	return W_FALSE;
}
int w_textlayout_get_offset(w_textlayout *textlayout, w_point *point,
		int *trailing, int trailing_count, int enc) {
	return W_FALSE;
}
int w_textlayout_get_orientation(w_textlayout *textlayout) {
	return W_FALSE;
}
int w_textlayout_get_previous_offset(w_textlayout *textlayout, int offset,
		int movement, int enc) {
	return W_FALSE;
}
wresult w_textlayout_get_ranges(w_textlayout *textlayout, w_iterator *ranges) {
	return W_FALSE;
}
int w_textlayout_get_spacing(w_textlayout *textlayout) {
	return W_FALSE;
}
wresult w_textlayout_get_style(w_textlayout *textlayout, int offset,
		w_textstyle *style, int enc) {
	return W_FALSE;
}
int w_textlayout_get_tabs(w_textlayout *textlayout, int *tabs, int count) {
	return W_FALSE;
}
wresult w_textlayout_get_text(w_textlayout *textlayout, w_alloc text,
		void *user_data) {
	return W_FALSE;
}
int w_textlayout_get_text_direction(w_textlayout *textlayout) {
	return W_FALSE;
}
int w_textlayout_get_width(w_textlayout *textlayout) {
	return W_FALSE;
}
int w_textlayout_get_wrap_indent(w_textlayout *textlayout) {
	return W_FALSE;
}
wresult w_textlayout_set_alignment(w_textlayout *textlayout, int alignment) {
	return W_FALSE;
}
wresult w_textlayout_set_ascent(w_textlayout *textlayout, int ascent) {
	return W_FALSE;
}
wresult w_textlayout_set_descent(w_textlayout *textlayout, int descent) {
	return W_FALSE;
}
wresult w_textlayout_set_font(w_textlayout *textlayout, w_font *font) {
	return W_FALSE;
}
wresult w_textlayout_set_indent(w_textlayout *textlayout, int indent) {
	return W_FALSE;
}
wresult w_textlayout_set_justify(w_textlayout *textlayout, int justify) {
	return W_FALSE;
}
wresult w_textlayout_set_orientation(w_textlayout *textlayout,
		int orientation) {
	return W_FALSE;
}
wresult w_textlayout_set_spacing(w_textlayout *textlayout, int spacing) {
	return W_FALSE;
}
wresult w_textlayout_set_style(w_textlayout *textlayout, w_textstyle *style,
		int start, int end, int enc) {
	return W_FALSE;
}
wresult w_textlayout_set_tabs(w_textlayout *textlayout, int *tabs, int count) {
	return W_FALSE;
}
wresult w_textlayout_set_text(w_textlayout *textlayout, const char *text,
		int count) {
	return W_FALSE;
}
wresult w_textlayout_set_text_segments(w_textlayout *textlayout,
		const char *text, int count, int *segments, wuint *segmentschars,
		int nsegments) {
	return W_FALSE;
}
wresult w_textlayout_set_text_direction(w_textlayout *textlayout,
		int textDirection) {
	return W_FALSE;
}
wresult w_textlayout_set_width(w_textlayout *textlayout, int width) {
	return W_FALSE;
}
wresult w_textlayout_set_wrap_indent(w_textlayout *textlayout, int wrapIndent) {
	return W_FALSE;
}
