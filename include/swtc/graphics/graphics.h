/*
 * Name:        graphics.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_GRAPHICS_H_
#define SWT_GRAPHICS_GRAPHICS_H_
#include "region.h"
#include "cursor.h"
#include "font.h"
#include "path.h"
#include "pattern.h"
#include "transform.h"
#include "line.h"
#include "imagelist.h"
#ifdef __cplusplus
extern "C" {
#endif
SWT_PUBLIC void w_graphics_init(w_graphics *gc);
SWT_PUBLIC void w_graphics_dispose(w_graphics *gc);
SWT_PUBLIC wresult w_graphics_create(w_graphics *gc);
SWT_PUBLIC wresult w_graphics_is_ok(w_graphics *gc);
SWT_PUBLIC wresult w_graphics_copy_area_to_image(w_graphics *gc,
		w_surface *surface, int x, int y);
SWT_PUBLIC wresult w_graphics_copy_area(w_graphics *gc, int srcX, int srcY,
		int width, int height, int destX, int destY, wresult paint);
SWT_PUBLIC wresult w_graphics_draw_arc(w_graphics *gc, w_rect *rect,
		int startAngle, int arcAngle);
SWT_PUBLIC wresult w_graphics_draw_image(w_graphics *gc, w_image *image,
		w_rect *src, w_rect *dest, int state);
SWT_PUBLIC wresult w_graphics_draw_surface(w_graphics *gc, w_surface *surface,
		w_rect *src, w_rect *dest);
SWT_PUBLIC wresult w_graphics_draw_line(w_graphics *gc, w_point *pt1,
		w_point *pt2);
SWT_PUBLIC wresult w_graphics_draw_oval(w_graphics *gc, w_rect *rect);
SWT_PUBLIC wresult w_graphics_draw_path(w_graphics *gc, w_path *path);
SWT_PUBLIC wresult w_graphics_draw_point(w_graphics *gc, w_point *pt);
SWT_PUBLIC wresult w_graphics_draw_polygon(w_graphics *gc, w_point *pointArray,
		int count);
SWT_PUBLIC wresult w_graphics_draw_polygonv(w_graphics *gc, int count,
		va_list args);
SWT_PUBLIC wresult w_graphics_draw_polyline(w_graphics *gc, w_point *pointArray,
		int count);
SWT_PUBLIC wresult w_graphics_draw_polylinev(w_graphics *gc, int count,
		va_list args);
SWT_PUBLIC wresult w_graphics_draw_rectangle(w_graphics *gc, w_rect *rect);
SWT_PUBLIC wresult w_graphics_draw_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight);
SWT_PUBLIC wresult w_graphics_draw_text(w_graphics *gc, const char *text,
		size_t length, w_rect *rect, int flags, int enc);
SWT_PUBLIC wresult w_graphics_fill_arc(w_graphics *gc, w_rect *rect,
		int startAngle, int arcAngle);
SWT_PUBLIC wresult w_graphics_fill_gradientrectangle(w_graphics *gc,
		w_rect *rect, int vertical);
SWT_PUBLIC wresult w_graphics_fill_oval(w_graphics *gc, w_rect *rect);
SWT_PUBLIC wresult w_graphics_fill_path(w_graphics *gc, w_path *path);
SWT_PUBLIC wresult w_graphics_fill_polygon(w_graphics *gc, w_point *pointArray,
		int length);
SWT_PUBLIC wresult w_graphics_fill_polygonv(w_graphics *gc, int count,
		va_list args);
SWT_PUBLIC wresult w_graphics_fill_rectangle(w_graphics *gc, w_rect *rect);
SWT_PUBLIC wresult w_graphics_fill_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight);
SWT_PUBLIC int w_graphics_get_advance_width(w_graphics *gc, int ch);
SWT_PUBLIC wresult w_graphics_get_advanced(w_graphics *gc);
SWT_PUBLIC int w_graphics_get_alpha(w_graphics *gc);
SWT_PUBLIC int w_graphics_get_antialias(w_graphics *gc);
SWT_PUBLIC w_color w_graphics_get_background(w_graphics *gc);
SWT_PUBLIC w_pattern* w_graphics_get_background_pattern(w_graphics *gc);
SWT_PUBLIC int w_graphics_get_char_width(w_graphics *gc, int ch);
SWT_PUBLIC wresult w_graphics_get_clipping_rect(w_graphics *gc, w_rect *rect);
SWT_PUBLIC wresult w_graphics_get_clipping_region(w_graphics *gc,
		w_region *region);
SWT_PUBLIC int w_graphics_get_fill_rule(w_graphics *gc);
SWT_PUBLIC w_font* w_graphics_get_font(w_graphics *gc);
SWT_PUBLIC wresult w_graphics_get_font_metrics(w_graphics *gc,
		w_fontmetrics *fontMetrics);
SWT_PUBLIC w_color w_graphics_get_foreground(w_graphics *gc);
SWT_PUBLIC w_pattern* w_graphics_get_foreground_pattern(w_graphics *gc);
SWT_PUBLIC int w_graphics_get_interpolation(w_graphics *gc);
SWT_PUBLIC w_line_attributes* w_graphics_get_line_attributes(w_graphics *gc);
SWT_PUBLIC int w_graphics_get_line_cap(w_graphics *gc);
SWT_PUBLIC wresult w_graphics_get_line_dash(w_graphics *gc, int **dashes,
		int *length);
SWT_PUBLIC int w_graphics_get_line_join(w_graphics *gc);
SWT_PUBLIC int w_graphics_get_line_style(w_graphics *gc);
SWT_PUBLIC float w_graphics_get_line_width(w_graphics *gc);
SWT_PUBLIC int w_graphics_get_style(w_graphics *gc);
SWT_PUBLIC int w_graphics_get_text_antialias(w_graphics *gc);
SWT_PUBLIC wresult w_graphics_get_transform(w_graphics *gc,
		w_transform *Transform);
SWT_PUBLIC wresult w_graphics_get_xor_mode(w_graphics *gc);
SWT_PUBLIC wresult w_graphics_is_clipped(w_graphics *gc);
SWT_PUBLIC wresult w_graphics_set_advanced(w_graphics *gc, int advanced);
SWT_PUBLIC wresult w_graphics_set_antialias(w_graphics *gc, int antialias);
SWT_PUBLIC wresult w_graphics_set_alpha(w_graphics *gc, int alpha);
SWT_PUBLIC wresult w_graphics_set_background(w_graphics *gc, w_color color);
SWT_PUBLIC wresult w_graphics_set_background_pattern(w_graphics *gc,
		w_pattern *pattern);
SWT_PUBLIC wresult w_graphics_set_clipping_path(w_graphics *gc, w_path *path);
SWT_PUBLIC wresult w_graphics_set_clipping_rect(w_graphics *gc, w_rect *rect);
SWT_PUBLIC wresult w_graphics_set_clipping_region(w_graphics *gc,
		w_region *region);
SWT_PUBLIC wresult w_graphics_set_fill_rule(w_graphics *gc, int rule);
SWT_PUBLIC wresult w_graphics_set_font(w_graphics *gc, w_font *font);
SWT_PUBLIC wresult w_graphics_set_foreground(w_graphics *gc, w_color color);
SWT_PUBLIC wresult w_graphics_set_foreground_pattern(w_graphics *gc,
		w_pattern *pattern);
SWT_PUBLIC wresult w_graphics_set_interpolation(w_graphics *gc,
		int interpolation);
SWT_PUBLIC wresult w_graphics_set_line_attributes(w_graphics *gc,
		w_line_attributes *attributes);
SWT_PUBLIC wresult w_graphics_set_line_cap(w_graphics *gc, int cap);
SWT_PUBLIC wresult w_graphics_set_line_dash(w_graphics *gc, int *dashes,
		int peer_data_length);
SWT_PUBLIC wresult w_graphics_set_line_join(w_graphics *gc, int join);
SWT_PUBLIC wresult w_graphics_set_line_style(w_graphics *gc, int lineStyle);
SWT_PUBLIC wresult w_graphics_set_line_width(w_graphics *gc, float lineWidth);
SWT_PUBLIC wresult w_graphics_set_xor_mode(w_graphics *gc, int _xor);
SWT_PUBLIC wresult w_graphics_set_text_antialias(w_graphics *gc, int antialias);
SWT_PUBLIC wresult w_graphics_set_transform(w_graphics *gc,
		w_transform *Transform);
SWT_PUBLIC wresult w_graphics_text_extent(w_graphics *gc, const char *string,
		int length, w_size *size, int flags,int enc);
#ifdef __cplusplus
}
#endif
#endif /* SWT_GRAPHICS_GRAPHICS_H_ */
