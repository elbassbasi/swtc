/*
 * Name:        gc.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
#include "../widgets/toolkit.h"

void w_graphics_init(w_graphics *gc) {
#if USE_CAIRO
	_W_GRAPHICS(gc)->cairo = 0;
#endif
}
#if USE_CAIRO
void _w_graphics_init(w_graphics *gc, cairo_t *drawable) {
	memset(gc, 0, sizeof(_w_graphics));
	_W_GRAPHICS(gc)->cairo = drawable;
	_W_GRAPHICS(gc)->alpha = 0xFF;
	_W_GRAPHICS(gc)->stringWidth = _W_GRAPHICS(gc)->stringHeight = -1;
	_W_GRAPHICS(gc)->width = _W_GRAPHICS(gc)->height = -1;
}
#endif
void w_graphics_dispose(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->state & GRAPHICS_STATE_DISPOSE_CAIRO) {
		cairo_destroy(_W_GRAPHICS(gc)->cairo);
	}
	if (_W_GRAPHICS(gc)->context != 0)
		g_object_unref(_W_GRAPHICS(gc)->context);
	if (_W_GRAPHICS(gc)->layout != 0)
		g_object_unref(_W_GRAPHICS(gc)->layout);
}
wuchar LINE_DOT[] = { 1, 1, 0 };
wuchar LINE_DASH[] = { 3, 1, 0 };
wuchar LINE_DASHDOT[] = { 3, 1, 1, 1, 0 };
wuchar LINE_DASHDOTDOT[] = { 3, 1, 1, 1, 1, 1, 0 };
wuchar LINE_DOT_ZERO[] = { 3, 3, 0 };
wuchar LINE_DASH_ZERO[] = { 18, 6, 0 };
wuchar LINE_DASHDOT_ZERO[] = { 9, 6, 3, 6, 0 };
wuchar LINE_DASHDOTDOT_ZERO[] = { 9, 3, 3, 3, 3, 3, 0 };
wresult _w_graphics_check(w_graphics *gc, int mask) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
#if USE_CAIRO
	if (_gc->cairo == 0) {
		return W_FALSE;
	}
	int state = _gc->state;
	if ((state & mask) == mask)
		return W_TRUE;
	state = (state ^ mask) & mask;
	_gc->state |= mask;
	if ((state & (GRAPHICS_STATE_BACKGROUND | GRAPHICS_STATE_FOREGROUND))
			!= 0) {
		w_color color;
		w_pattern *pattern;
		if ((state & GRAPHICS_STATE_FOREGROUND) != 0) {
			color = _gc->foreground;
			pattern = _gc->foregroundPattern;
			_gc->state &= ~GRAPHICS_STATE_BACKGROUND;
		} else {
			color = _gc->background;
			pattern = _gc->backgroundPattern;
			_gc->state &= ~GRAPHICS_STATE_FOREGROUND;
		}
		if (pattern != 0) {
			if ((_gc->state & GRAPHICS_STATE_MIRRORED) != 0
					&& _W_PATTERN(pattern)->handle != 0) {
				cairo_pattern_t *newPattern = _W_PATTERN(pattern)->handle;
				if (newPattern == 0)
					return W_ERROR_NO_HANDLES;
				cairo_pattern_set_extend(newPattern, CAIRO_EXTEND_REPEAT);
				cairo_matrix_t matrix = { -1, 0, 0, 1, 0, 0 };
				cairo_pattern_set_matrix(newPattern, &matrix);
				cairo_set_source(_gc->cairo, newPattern);
				cairo_pattern_destroy(newPattern);
			} else {
				cairo_set_source(_gc->cairo,
				_W_PATTERN(pattern)->handle);
			}
		} else {
			cairo_set_source_rgba(_gc->cairo, W_RED(color) / 255.0,
			W_GREEN(color) / 255.0, W_BLUE(color) / 255.0,
					_gc->alpha / (float) 0xFF);
		}
	}
	if ((state & GRAPHICS_STATE_FONT) != 0) {
		if (_gc->layout != 0) {
			if (_gc->font != 0)
				pango_layout_set_font_description(_gc->layout,
				_W_FONT(_gc->font)->handle);
		}
	}
	if ((state & GRAPHICS_STATE_LINE_CAP) != 0) {
		cairo_line_cap_t cap_style = CAIRO_LINE_CAP_BUTT;
		switch (_gc->line.cap) {
		case W_CAP_ROUND:
			cap_style = CAIRO_LINE_CAP_ROUND;
			break;
		case W_CAP_FLAT:
			cap_style = CAIRO_LINE_CAP_BUTT;
			break;
		case W_CAP_SQUARE:
			cap_style = CAIRO_LINE_CAP_SQUARE;
			break;
		}
		cairo_set_line_cap(_gc->cairo, cap_style);
	}
	if ((state & GRAPHICS_STATE_LINE_JOIN) != 0) {
		cairo_line_join_t join_style = CAIRO_LINE_JOIN_MITER;
		switch (_gc->line.join) {
		case W_JOIN_MITER:
			join_style = CAIRO_LINE_JOIN_MITER;
			break;
		case W_JOIN_ROUND:
			join_style = CAIRO_LINE_JOIN_ROUND;
			break;
		case W_JOIN_BEVEL:
			join_style = CAIRO_LINE_JOIN_BEVEL;
			break;
		}
		cairo_set_line_join(_gc->cairo, join_style);
	}
	if ((state & GRAPHICS_STATE_LINE_WIDTH) != 0) {
		cairo_set_line_width(_gc->cairo,
				_gc->line.width == 0 ? 1 : _gc->line.width);
		switch (_gc->line.style) {
		case W_LINE_DOT:
		case W_LINE_DASH:
		case W_LINE_DASHDOT:
		case W_LINE_DASHDOTDOT:
			state |= GRAPHICS_STATE_LINE_STYLE;
		}
	}
	if ((state & GRAPHICS_STATE_LINE_STYLE) != 0) {
		float dashesOffset = 0;
		wuchar *dashes = 0;
		float width = _gc->line.width;
		int is_custom_dash = W_FALSE;
		switch (_gc->line.style) {
		case W_LINE_SOLID:
			break;
		case W_LINE_DASH:
			dashes = width != 0 ? LINE_DASH : LINE_DASH_ZERO;
			break;
		case W_LINE_DOT:
			dashes = width != 0 ? LINE_DOT : LINE_DOT_ZERO;
			break;
		case W_LINE_DASHDOT:
			dashes = width != 0 ? LINE_DASHDOT : LINE_DASHDOT_ZERO;
			break;
		case W_LINE_DASHDOTDOT:
			dashes = width != 0 ? LINE_DASHDOTDOT : LINE_DASHDOTDOT_ZERO;
			break;
		case W_LINE_CUSTOM:
			is_custom_dash = W_TRUE;
			//dashes = _gc->lineDashes;
			break;
		}
		if (dashes != 0) {
			dashesOffset = _gc->line.dashOffset;
			double cairoDashes[6];
			int i = 0;
			while (dashes[i] != 0) {
				cairoDashes[i] =
						width == 0 || _gc->line.style == W_LINE_CUSTOM ?
								dashes[i] : dashes[i] * width;
				i++;
			}
			cairo_set_dash(_gc->cairo, cairoDashes, i, dashesOffset);
		} else {
			w_line_attributes *line = &_gc->line;
			if (is_custom_dash && line->dashes != 0) {
				dashesOffset = _gc->line.dashOffset;
				double *cairoDashes = malloc(line->length * sizeof(double));
				if (cairoDashes != 0) {
					for (int i = 0; i < line->length; i++) {
						cairoDashes[i] =
								width == 0 || line->style == W_LINE_CUSTOM ?
										dashes[i] : dashes[i] * width;
					}
					cairo_set_dash(_gc->cairo, cairoDashes, line->length,
							dashesOffset);
					free(cairoDashes);
				} else {

				}
			} else
				cairo_set_dash(_gc->cairo, 0, 0, 0);
		}
	}
	if ((state & GRAPHICS_STATE_LINE_MITERLIMIT) != 0) {
		cairo_set_miter_limit(_gc->cairo, _gc->line.miterLimit);
	}
	if ((state & GRAPHICS_STATE_DRAW_OFFSET) != 0) {
		_gc->cairoXoffset = _gc->cairoYoffset = 0;
		cairo_matrix_t matrix;
		cairo_get_matrix(_gc->cairo, &matrix);
		double dx = 1;
		double dy = 1;
		cairo_user_to_device_distance(_gc->cairo, &dx, &dy);
		double scaling = dx;
		if (scaling < 0)
			scaling = -scaling;
		double strokeWidth = _gc->line.width * scaling;
		if (strokeWidth == 0 || ((int) strokeWidth % 2) == 1) {
			_gc->cairoXoffset = 0.5 / scaling;
		}
		scaling = dy;
		if (scaling < 0)
			scaling = -scaling;
		strokeWidth = _gc->line.width * scaling;
		if (strokeWidth == 0 || ((int) strokeWidth % 2) == 1) {
			_gc->cairoYoffset = 0.5 / scaling;
		}
	}
	return W_TRUE;
#endif
}
wresult w_graphics_create(w_graphics *gc) {
	return W_FALSE;
}
wresult w_graphics_is_ok(w_graphics *gc) {
	return _w_graphics_check(gc, 0) > 0;
}
wresult w_graphics_copy_area_to_image(w_graphics *gc, w_surface *surface, int x,
		int y) {
	return W_FALSE;
}
wresult w_graphics_copy_area(w_graphics *gc, int srcX, int srcY, int width,
		int height, int destX, int destY, wresult paint) {
	return W_FALSE;
}
wresult w_graphics_draw_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int x = rect->x, y = rect->y, width = rect->width,
				height = rect->height;
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
		if (width == 0 || height == 0 || arcAngle == 0)
			return W_TRUE;
#if USE_CAIRO
		double xOffset = _gc->cairoXoffset, yOffset = _gc->cairoYoffset;
		if (width == height) {
			if (arcAngle >= 0) {
				cairo_arc_negative(_gc->cairo, x + xOffset + width / 2.,
						y + yOffset + height / 2.0, width / 2.0,
						-startAngle * (float) M_PI / 180,
						-(startAngle + arcAngle) * (float) M_PI / 180);
			} else {
				cairo_arc(_gc->cairo, x + xOffset + width / 2.0,
						y + yOffset + height / 2.0, width / 2.0,
						-startAngle * (float) M_PI / 180,
						-(startAngle + arcAngle) * (float) M_PI / 180);
			}
		} else {
			cairo_save(_gc->cairo);
			cairo_translate(_gc->cairo, x + xOffset + width / 2.0,
					y + yOffset + height / 2.0);
			cairo_scale(_gc->cairo, width / 2.0, height / 2.0);
			if (arcAngle >= 0) {
				cairo_arc_negative(_gc->cairo, 0, 0, 1,
						-startAngle * (float) M_PI / 180,
						-(startAngle + arcAngle) * (float) M_PI / 180);
			} else {
				cairo_arc(_gc->cairo, 0, 0, 1, -startAngle * (float) M_PI / 180,
						-(startAngle + arcAngle) * (float) M_PI / 180);
			}
			cairo_restore(_gc->cairo);
		}
		cairo_stroke(_gc->cairo);
#endif
	}
	return result;
}
GdkPixbuf* _w_graphics_get_pixbuf(w_graphics *gc, w_image *image, int state) {
	GdkPixbuf *pixbuf = _W_IMAGE(image)->pixbuf;
	if (state & W_DISABLED) {
		w_image dst;
		_W_IMAGE(&dst)->pixbuf = 0;
		w_image_to_disabled(image, w_graphics_get_background(gc),
				w_graphics_get_foreground(gc), &dst);
		if (_W_IMAGE(&dst)->pixbuf != 0)
			return _W_IMAGE(&dst)->pixbuf;
	}
	return pixbuf;
}
wresult w_graphics_draw_image_0(w_graphics *gc, w_image *image,
		w_surface *_surface, w_rect *src, w_rect *dest, int state) {
#if USE_CAIRO
	if (_W_GRAPHICS(gc)->cairo == 0) {
		return W_ERROR_NO_HANDLES;
	}
	GdkPixbuf *pixbuf = 0;
	cairo_surface_t *surface = 0;
	if (image != 0) {
		if (_W_IMAGE(image)->pixbuf == 0)
			return W_ERROR_INVALID_ARGUMENT;
		pixbuf = _w_graphics_get_pixbuf(gc, image, state);
	}
	if (_surface != 0) {
		if (_W_SURFACE(_surface)->handle == 0)
			return W_ERROR_INVALID_ARGUMENT;
		surface = _W_SURFACE(_surface)->handle;
	}
	cairo_t *cr = _W_GRAPHICS(gc)->cairo;
	if (src->x == 0 && src->y == 0 && src->width < 0 && src->height < 0
			&& dest->width < 0 && dest->height < 0) {
		if (pixbuf)
			gdk_cairo_set_source_pixbuf(cr, pixbuf, dest->x, dest->y);
		if (surface)
			cairo_set_source_surface(cr, surface, dest->x, dest->y);
		cairo_paint(cr);
		return W_TRUE;
	}
	int srcX = src->x, srcY = src->y, srcWidth = src->width, srcHeight =
			src->height;
	int destX = dest->x, destY = dest->y, destWidth = dest->width, destHeight =
			dest->height;
	int imgWidth, imgHeight;
	if (pixbuf) {
		imgWidth = gdk_pixbuf_get_width(pixbuf);
		imgHeight = gdk_pixbuf_get_height(pixbuf);
	}
	if (surface) {
		imgWidth = cairo_image_surface_get_width(surface);
		imgHeight = cairo_image_surface_get_height(surface);
	}
	if (srcWidth < 0)
		srcWidth = imgWidth;
	if (srcHeight < 0)
		srcHeight = imgHeight;
	if (destWidth < 0)
		destWidth = imgWidth;
	if (destHeight < 0)
		destHeight = imgHeight;
	if (_W_GRAPHICS(gc)->alpha != 0) {
		//srcImage.createSurface();
		cairo_save(cr);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			cairo_scale(cr, -1., 1);
			cairo_translate(cr, -2 * destX - destWidth, 0);
		}
		cairo_rectangle(cr, destX, destY, destWidth, destHeight);
		cairo_clip(cr);
		if (srcWidth != destWidth || srcHeight != destHeight) {
			float scaleX = destWidth / (float) srcWidth;
			float scaleY = destHeight / (float) srcHeight;
			cairo_translate(cr, destX - (int) (srcX * scaleX),
					destY - (int) (srcY * scaleY));
			cairo_scale(cr, scaleX, scaleY);
		} else {
			cairo_translate(cr, destX - srcX, destY - srcY);
		}
		cairo_filter_t filter = CAIRO_FILTER_GOOD;
		switch (_W_GRAPHICS(gc)->interpolation) {
		case 0xFF: //W_DEFAULT
			filter = CAIRO_FILTER_GOOD;
			break;
		case W_NONE:
			filter = CAIRO_FILTER_NEAREST;
			break;
		case W_LOW:
			filter = CAIRO_FILTER_FAST;
			break;
		case W_HIGH:
			filter = CAIRO_FILTER_BEST;
			break;
		}
		if (surface == 0)
			surface = gdk_cairo_surface_create_from_pixbuf(pixbuf, 0, NULL);
		if (surface == 0) {
			return W_ERROR_NO_HANDLES;
		}
		cairo_pattern_t *pattern = cairo_pattern_create_for_surface(surface);
		if (pattern == 0) {
			if (_surface == 0)
				cairo_surface_destroy(surface);
			return W_ERROR_NO_HANDLES;
		}
		if (srcWidth != destWidth || srcHeight != destHeight) {
			/*
			 * Bug in Cairo.  When drawing the image stretched with an interpolation
			 * algorithm, the edges of the image are faded.  This is not a bug, but
			 * it is not desired.  To avoid the faded edges, it should be possible to
			 * use cairo_pattern_set_extend() to set the pattern extend to either
			 * CAIRO_EXTEND_REFLECT or CAIRO_EXTEND_PAD, but these are not implemented
			 * in some versions of cairo (1.2.x) and have bugs in others (in 1.4.2 it
			 * draws with black edges).  The fix is to implement CAIRO_EXTEND_REFLECT
			 * by creating an image that is 3 times bigger than the original, drawing
			 * the original image in every quadrant (with an appropriate transform) and
			 * use this image as the pattern.
			 *
			 * NOTE: For some reason, it is necessary to use CAIRO_EXTEND_PAD with
			 * the image that was created or the edges are still faded.
			 *
			 * NOTE: CAIRO_EXTEND_PAD works on Cairo 1.8.x and greater.
			 */
			int version = cairo_version();
			if (version >= CAIRO_VERSION_ENCODE(1, 4, 0)
					&& version < CAIRO_VERSION_ENCODE(1, 8, 0)) {
				cairo_surface_t *surf = cairo_image_surface_create(
						CAIRO_FORMAT_ARGB32, imgWidth * 3, imgHeight * 3);
				cairo_t *cr = cairo_create(surf);
				cairo_set_source_surface(cr, surf, imgWidth, imgHeight);
				cairo_paint(cr);
				cairo_scale(cr, -1, -1);
				cairo_set_source_surface(cr, surf, -imgWidth, -imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surf, -imgWidth * 3, -imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surf, -imgWidth, -imgHeight * 3);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surf, -imgWidth * 3,
						-imgHeight * 3);
				cairo_paint(cr);
				cairo_scale(cr, 1, -1);
				cairo_set_source_surface(cr, surf, -imgWidth, imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surf, -imgWidth * 3, imgHeight);
				cairo_paint(cr);
				cairo_scale(cr, -1, -1);
				cairo_set_source_surface(cr, surf, imgWidth, -imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surf, imgWidth, -imgHeight * 3);
				cairo_paint(cr);
				cairo_destroy(cr);
				cairo_pattern_t *newPattern = cairo_pattern_create_for_surface(
						surf);
				cairo_surface_destroy(surf);
				if (newPattern == 0) {
					if (pixbuf != _W_IMAGE(image)->pixbuf) {
						g_object_unref(pixbuf);
					}
					return W_ERROR_NO_HANDLES;
				}
				cairo_pattern_destroy(pattern);
				pattern = newPattern;
				cairo_pattern_set_extend(pattern, CAIRO_EXTEND_PAD);
				cairo_matrix_t matrix;
				cairo_matrix_init_translate(&matrix, imgWidth, imgHeight);
				cairo_pattern_set_matrix(pattern, &matrix);
			} else if (version >= CAIRO_VERSION_ENCODE(1, 8, 0)) {
				cairo_pattern_set_extend(pattern, CAIRO_EXTEND_PAD);
			}
		} else {
			gdk_cairo_set_source_pixbuf(cr, pixbuf, srcX, srcY);
		}
		cairo_pattern_set_filter(pattern, filter);
		cairo_set_source(cr, pattern);
		if (_W_GRAPHICS(gc)->alpha != 0xFF) {
			cairo_paint_with_alpha(cr,
			_W_GRAPHICS(gc)->alpha / (float) 0xFF);
		} else {
			cairo_paint(cr);
		}
		cairo_restore(cr);
		cairo_pattern_destroy(pattern);
	}
	return W_TRUE;
#else
#endif
}
wresult w_graphics_draw_image(w_graphics *gc, w_image *image, w_rect *src,
		w_rect *dest, int state) {
	return w_graphics_draw_image_0(gc, image, 0, src, dest, state);
}
wresult w_graphics_draw_surface(w_graphics *gc, w_surface *surface, w_rect *src,
		w_rect *dest) {
	return w_graphics_draw_image_0(gc, 0, surface, src, dest, 0);
}
wresult w_graphics_draw_line(w_graphics *gc, w_point *pt1, w_point *pt2) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
#if USE_CAIRO
		double xOffset = _gc->cairoXoffset;
		double yOffset = _gc->cairoYoffset;
		cairo_move_to(_gc->cairo, pt1->x + xOffset, pt1->y + yOffset);
		cairo_line_to(_gc->cairo, pt2->x + xOffset, pt2->y + yOffset);
		cairo_stroke(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_draw_oval(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int x = rect->x, y = rect->y, width = rect->width,
				height = rect->height;
#if USE_CAIRO
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
		double xOffset = _gc->cairoXoffset, yOffset = _gc->cairoYoffset;
		if (width == height) {
			cairo_arc_negative(_gc->cairo, x + xOffset + width / 2.0,
					y + yOffset + height / 2.0, width / 2.0, 0, -2 * M_PI);
		} else {
			cairo_save(_gc->cairo);
			cairo_translate(_gc->cairo, x + xOffset + width / 2.0,
					y + yOffset + height / 2.0);
			cairo_scale(_gc->cairo, width / 2.0, height / 2.0);
			cairo_arc_negative(_gc->cairo, 0, 0, 1, 0, -2 * M_PI);
			cairo_restore(_gc->cairo);
		}
		cairo_stroke(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_draw_path(w_graphics *gc, w_path *path) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		if (path == 0)
			return W_ERROR_NULL_ARGUMENT;
		if (_W_PATH(path)->handle == 0)
			return W_ERROR_INVALID_ARGUMENT;
		cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
		cairo_save(cairo);
		double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
		_W_GRAPHICS(gc)->cairoYoffset;
		cairo_translate(cairo, xOffset, yOffset);
		cairo_path_t *copy = cairo_copy_path(_W_PATH(path)->handle);
		if (copy == 0)
			return W_ERROR_NO_HANDLES;
		cairo_append_path(cairo, copy);
		cairo_path_destroy(copy);
		cairo_stroke(cairo);
		cairo_restore(cairo);
	}
	return result;
}
wresult w_graphics_draw_point(w_graphics *gc, w_point *pt) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
#if USE_CAIRO
		cairo_rectangle(_gc->cairo, pt->x, pt->y, 1, 1);
		cairo_fill(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_draw_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
#if USE_CAIRO
		if (count == 0)
			return W_TRUE;
		double xOffset = _gc->cairoXoffset, yOffset = _gc->cairoYoffset;
		cairo_move_to(_gc->cairo, pointArray[0].x + xOffset,
				pointArray[0].y + yOffset);
		for (int i = 1; i < count; i++) {
			cairo_line_to(_gc->cairo, pointArray[i].x + xOffset,
					pointArray[i].y + yOffset);
		}
		cairo_close_path(_gc->cairo);
		cairo_stroke(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_draw_polygonv(w_graphics *gc, int count, va_list args) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
#if USE_CAIRO
		if (count == 0)
			return W_TRUE;
		double xOffset = _gc->cairoXoffset, yOffset = _gc->cairoYoffset;
		int x = va_arg(args, int);
		int y = va_arg(args, int);
		cairo_move_to(_gc->cairo, x + xOffset, y + yOffset);
		for (int i = 1; i < count; i++) {
			x = va_arg(args, int);
			y = va_arg(args, int);
			cairo_line_to(_gc->cairo, x + xOffset, y + yOffset);
		}
		cairo_close_path(_gc->cairo);
		cairo_stroke(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_draw_polyline(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
#if USE_CAIRO
		if (count == 0)
			return W_TRUE;
		double xOffset = _gc->cairoXoffset, yOffset = _gc->cairoYoffset;
		cairo_move_to(_gc->cairo, pointArray[0].x + xOffset,
				pointArray[0].y + yOffset);
		for (int i = 1; i < count; i++) {
			cairo_line_to(_gc->cairo, pointArray[i].x + xOffset,
					pointArray[i].y + yOffset);
		}
		cairo_stroke(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_draw_polylinev(w_graphics *gc, int count, va_list args) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
#if USE_CAIRO
		if (count == 0)
			return W_TRUE;
		double xOffset = _gc->cairoXoffset, yOffset = _gc->cairoYoffset;
		int x = va_arg(args, int);
		int y = va_arg(args, int);
		cairo_move_to(_gc->cairo, x + xOffset, y + yOffset);
		for (int i = 1; i < count; i++) {
			x = va_arg(args, int);
			y = va_arg(args, int);
			cairo_line_to(_gc->cairo, x + xOffset, y + yOffset);
		}
		cairo_stroke(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_draw_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int x = rect->x, y = rect->y, width = rect->width,
				height = rect->height;
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
#if USE_CAIRO
		cairo_rectangle(_gc->cairo, x + _gc->cairoXoffset,
				y + _gc->cairoYoffset, width, height);
		cairo_stroke(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_draw_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int nx = rect->x;
		int ny = rect->y;
		int nw = rect->width;
		int nh = rect->height;
		int naw = arcWidth;
		int nah = arcHeight;
		if (nw < 0) {
			nw = 0 - nw;
			nx = nx - nw;
		}
		if (nh < 0) {
			nh = 0 - nh;
			ny = ny - nh;
		}
		if (naw < 0)
			naw = 0 - naw;
		if (nah < 0)
			nah = 0 - nah;
#if USE_CAIRO
		double xOffset = _gc->cairoXoffset, yOffset = _gc->cairoYoffset;
		if (naw == 0 || nah == 0) {
			cairo_rectangle(_gc->cairo, rect->x + xOffset, rect->y + yOffset,
					rect->width, rect->height);
		} else {
			float naw2 = naw / 2.0;
			float nah2 = nah / 2 / 2;
			float fw = nw / naw2;
			float fh = nh / nah2;
			cairo_save(_gc->cairo);
			cairo_translate(_gc->cairo, nx + xOffset, ny + yOffset);
			cairo_scale(_gc->cairo, naw2, nah2);
			cairo_move_to(_gc->cairo, fw - 1, 0);
			cairo_arc(_gc->cairo, fw - 1, 1, 1, M_PI + M_PI / 2.0,
			M_PI * 2.0);
			cairo_arc(_gc->cairo, fw - 1, fh - 1, 1, 0, M_PI / 2.0);
			cairo_arc(_gc->cairo, 1, fh - 1, 1, M_PI / 2, M_PI);
			cairo_arc(_gc->cairo, 1, 1, 1, M_PI, 270.0 * M_PI / 180.0);
			cairo_close_path(_gc->cairo);
			cairo_restore(_gc->cairo);
		}
		cairo_stroke(_gc->cairo);
#endif
	}
	return result;
}
wresult _w_graphics_create_layout(w_graphics *gc) {
	PangoContext *context = gdk_pango_context_get();
	if (context == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = pango_layout_new(context);
	if (layout == 0)
		return W_ERROR_NO_HANDLES;
	_w_graphics *_gc = _W_GRAPHICS(gc);
	_gc->layout = layout;
	_gc->context = context;
	pango_context_set_language(context, gtk_get_default_language());
	pango_context_set_base_dir(context,
			(_gc->state & GRAPHICS_STATE_MIRRORED) != 0 ?
					PANGO_DIRECTION_RTL : PANGO_DIRECTION_LTR);
	pango_layout_set_auto_dir(layout, FALSE);
	return W_TRUE;
}
wresult _w_graphics_set_text(w_graphics *gc, const char *text, size_t length,
		int flags, int enc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->layout == 0) {
		wresult result = _w_graphics_create_layout(gc);
		if (result < 0)
			return result;
	}
	int newlength, mnemonic;
	char *s = _gtk_text_fix(text, length, enc, &newlength, &mnemonic);
	if ((flags & W_DRAW_MNEMONIC) && mnemonic != -1) {
		PangoAttrList *attr_list = pango_attr_list_new();
		PangoAttribute *attribute = pango_attr_underline_new(
				PANGO_UNDERLINE_LOW);
		attribute->start_index = mnemonic - 1;
		attribute->end_index = mnemonic;
		pango_attr_list_insert(attr_list, attribute);
		pango_layout_set_attributes(_gc->layout, attr_list);
		pango_attr_list_unref(attr_list);
	} else {
		pango_layout_set_attributes(_gc->layout, 0);
	}
	pango_layout_set_text(_gc->layout, s, newlength);
	pango_layout_set_single_paragraph_mode(_gc->layout,
			(flags & W_DRAW_DELIMITER) == 0);
	pango_layout_set_tabs(_gc->layout,
			(flags & W_DRAW_TAB) != 0 ? 0 : gtk_toolkit->empty_tab);
	_gtk_text_free(text, s, newlength);
	return W_TRUE;
}
wresult w_graphics_draw_text(w_graphics *gc, const char *text, size_t length,
		w_rect *rect, int flags, int enc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		if (text == 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (*text == 0)
			return W_OK;
		result = _w_graphics_set_text(gc, text, length, flags, enc);
		if (result < 0)
			return result;
		int x = rect->x, y = rect->y, stringWidth, stringHeight;
		int width = rect->width;
		int height = rect->height;
		if (width >= 0) {
			pango_layout_set_width(_gc->layout, width);
		}
		if (height >= 0) {
			pango_layout_set_height(_gc->layout, height);
		}
		pango_layout_get_pixel_size(_gc->layout, &stringWidth, &stringHeight);
		if (width >= 0) {
			PangoAlignment alignment = PANGO_ALIGN_LEFT;
			int diff = width - stringWidth;
			if (diff < 0)
				diff = 0;
			if (flags & W_DRAW_HCENTER) {
				//alignment = PANGO_ALIGN_CENTER;
				x = rect->x + diff / 2;
			}
			if (flags & W_DRAW_RIGHT) {
				alignment = PANGO_ALIGN_RIGHT;
				x = rect->x + diff;
			}
			pango_layout_set_alignment(_gc->layout, alignment);
		} else {
			width = stringWidth;
		}
		if (height >= 0) {
			if (height >= stringHeight) {
				int diff = height - stringWidth;
				if (flags & W_DRAW_VCENTER) {
					y = rect->y + diff / 2;
				}
				if (flags & W_DRAW_BOTTOM) {
					y = rect->y + diff;
				}
			}
		} else {
			height = stringHeight;
		}
#if USE_CAIRO
		if ((flags & W_DRAW_TRANSPARENT) == 0) {
			_w_graphics_check(gc, GRAPHICS_STATE_BACKGROUND);
			cairo_rectangle(_gc->cairo, x, y, stringWidth, stringHeight);
			cairo_fill(_gc->cairo);
		}
		_w_graphics_check(gc, GRAPHICS_STATE_FOREGROUND | GRAPHICS_STATE_FONT);
		if ((_gc->state & GRAPHICS_STATE_MIRRORED) != 0) {
			cairo_save(_gc->cairo);
			cairo_scale(_gc->cairo, -1.0, 1);
			cairo_translate(_gc->cairo, -2 * x - stringWidth, 0);
		}
		cairo_move_to(_gc->cairo, x, y);
		pango_cairo_show_layout(_gc->cairo, _gc->layout);
		if ((_gc->state & GRAPHICS_STATE_MIRRORED) != 0) {
			cairo_restore(_gc->cairo);
		}
		cairo_new_path(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_fill_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int x = rect->x, y = rect->y, width = rect->width,
				height = rect->height;
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
		if (width == 0 || height == 0 || arcAngle == 0)
			return W_TRUE;
#if USE_CAIRO
		if (width == height) {
			if (arcAngle >= 0) {
				cairo_arc_negative(_gc->cairo, x + width / 2.0,
						y + height / 2.0, width / 2.0, -startAngle * M_PI / 180,
						-(startAngle + arcAngle) * M_PI / 180);
			} else {
				cairo_arc(_gc->cairo, x + width / 2.0, y + height / 2.0,
						width / 2.0, -startAngle * M_PI / 180,
						-(startAngle + arcAngle) * M_PI / 180);
			}
			cairo_line_to(_gc->cairo, x + width / 2.0, y + height / 2.0);
		} else {
			cairo_save(_gc->cairo);
			cairo_translate(_gc->cairo, x + width / 2.0, y + height / 2.0);
			cairo_scale(_gc->cairo, width / 2.0, height / 2.0);
			if (arcAngle >= 0) {
				cairo_arc_negative(_gc->cairo, 0, 0, 1,
						-startAngle * M_PI / 180,
						-(startAngle + arcAngle) * M_PI / 180);
			} else {
				cairo_arc(_gc->cairo, 0, 0, 1, -startAngle * M_PI / 180,
						-(startAngle + arcAngle) * M_PI / 180);
			}
			cairo_line_to(_gc->cairo, 0, 0);
			cairo_restore(_gc->cairo);
		}
		cairo_fill(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_fill_gradientrectangle(w_graphics *gc, w_rect *rect,
		int vertical) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int x = rect->x, y = rect->y, width = rect->width,
				height = rect->height;
		if ((width == 0) || (height == 0))
			return W_TRUE;
		w_color backgroundRGB, foregroundRGB;
		backgroundRGB = _gc->background;
		foregroundRGB = _gc->foreground;

		w_color fromRGB, toRGB;
		fromRGB = foregroundRGB;
		toRGB = backgroundRGB;
		wresult swapColors = W_FALSE;
		if (width < 0) {
			x += width;
			width = -width;
			if (!vertical)
				swapColors = W_TRUE;
		}
		if (height < 0) {
			y += height;
			height = -height;
			if (vertical)
				swapColors = W_TRUE;
		}
		if (swapColors) {
			fromRGB = backgroundRGB;
			toRGB = foregroundRGB;
		}
		if (fromRGB == toRGB) {
			return w_graphics_fill_rectangle(gc, rect);
		}
#if USE_CAIRO
		cairo_pattern_t *pattern;
		if (vertical) {
			pattern = cairo_pattern_create_linear(0.0, 0.0, 0.0, 1.0);
		} else {
			pattern = cairo_pattern_create_linear(0.0, 0.0, 1.0, 0.0);
		}
		cairo_pattern_add_color_stop_rgba(pattern, 0, W_RED(fromRGB) / 255.0,
		W_GREEN(fromRGB) / 255.0, W_BLUE(fromRGB) / 255.0, _gc->alpha / 255.0);
		cairo_pattern_add_color_stop_rgba(pattern, 1, W_RED(toRGB) / 255.0,
		W_GREEN(toRGB) / 255.0, W_BLUE(toRGB) / 255.0, _gc->alpha / 255.0);
		cairo_save(_gc->cairo);
		cairo_translate(_gc->cairo, x, y);
		cairo_scale(_gc->cairo, width, height);
		cairo_rectangle(_gc->cairo, 0, 0, 1, 1);
		cairo_set_source(_gc->cairo, pattern);
		cairo_fill(_gc->cairo);
		cairo_restore(_gc->cairo);
		cairo_pattern_destroy(pattern);
#endif
	}
	return result;
}
wresult w_graphics_fill_oval(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int x = rect->x, y = rect->y, width = rect->width,
				height = rect->height;
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
#if USE_CAIRO
		if (width == height) {
			cairo_arc_negative(_gc->cairo, x + width / 2.0, y + height / 2.0,
					width / 2.0, 0, -2 * M_PI);
		} else {
			cairo_save(_gc->cairo);
			cairo_translate(_gc->cairo, x + width / 2.0, y + height / 2.0);
			cairo_scale(_gc->cairo, width / 2.0, height / 2.0);
			cairo_arc_negative(_gc->cairo, 0, 0, 1, 0, -2 * M_PI);
			cairo_restore(_gc->cairo);
		}
		cairo_fill(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_fill_path(w_graphics *gc, w_path *path) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		if (path == 0)
			return W_ERROR_NULL_ARGUMENT;
		if (_W_PATH(path)->handle == 0)
			return W_ERROR_INVALID_ARGUMENT;
		cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
		cairo_path_t *copy = cairo_copy_path(_W_PATH(path)->handle);
		if (copy == 0)
			return W_ERROR_NO_HANDLES;
		cairo_append_path(cairo, copy);
		cairo_path_destroy(copy);
		cairo_fill(cairo);
	}
	return result;
}
wresult w_graphics_fill_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
#if USE_CAIRO
		if (count == 0)
			return W_TRUE;
		double xOffset = _gc->cairoXoffset, yOffset = _gc->cairoYoffset;
		cairo_move_to(_gc->cairo, pointArray[0].x + xOffset,
				pointArray[0].y + yOffset);
		for (int i = 1; i < count; i++) {
			cairo_line_to(_gc->cairo, pointArray[i].x + xOffset,
					pointArray[i].y + yOffset);
		}
		cairo_close_path(_gc->cairo);
		cairo_fill(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_fill_polygonv(w_graphics *gc, int count, va_list args) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
#if USE_CAIRO
		if (count == 0)
			return W_TRUE;
		double xOffset = _gc->cairoXoffset, yOffset = _gc->cairoYoffset;
		int x = va_arg(args, int);
		int y = va_arg(args, int);
		cairo_move_to(_gc->cairo, x + xOffset, y + yOffset);
		for (int i = 1; i < count; i++) {
			int x = va_arg(args, int);
			int y = va_arg(args, int);
			cairo_line_to(_gc->cairo, x + xOffset, y + yOffset);
		}
		cairo_close_path(_gc->cairo);
		cairo_fill(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_fill_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int x = rect->x, y = rect->y, width = rect->width,
				height = rect->height;
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
#if USE_CAIRO
		cairo_rectangle(_gc->cairo, x, y, width, height);
		cairo_fill(_gc->cairo);
#endif
	}
	return result;
}
wresult w_graphics_fill_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int nx = rect->x;
		int ny = rect->y;
		int nw = rect->width;
		int nh = rect->height;
		int naw = arcWidth;
		int nah = arcHeight;
		if (nw < 0) {
			nw = 0 - nw;
			nx = nx - nw;
		}
		if (nh < 0) {
			nh = 0 - nh;
			ny = ny - nh;
		}
		if (naw < 0)
			naw = 0 - naw;
		if (nah < 0)
			nah = 0 - nah;
#if USE_CAIRO
		if (naw == 0 || nah == 0) {
			cairo_rectangle(_gc->cairo, rect->x, rect->y, rect->width,
					rect->height);
		} else {
			float naw2 = naw / 2.0;
			float nah2 = nah / 2.0;
			float fw = nw / naw2;
			float fh = nh / nah2;
			cairo_save(_gc->cairo);
			cairo_translate(_gc->cairo, nx, ny);
			cairo_scale(_gc->cairo, naw2, nah2);
			cairo_move_to(_gc->cairo, fw - 1, 0);
			cairo_arc(_gc->cairo, fw - 1, 1, 1, M_PI + M_PI / 2.0,
			M_PI * 2.0);
			cairo_arc(_gc->cairo, fw - 1, fh - 1, 1, 0, M_PI / 2.0);
			cairo_arc(_gc->cairo, 1, fh - 1, 1, M_PI / 2, M_PI);
			cairo_arc(_gc->cairo, 1, 1, 1, M_PI, 270.0 * M_PI / 180.0);
			cairo_close_path(_gc->cairo);
			cairo_restore(_gc->cairo);
		}
		cairo_fill(_gc->cairo);
#endif
	}
	return result;
}
int w_graphics_get_advance_width(w_graphics *gc, int ch) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		w_size size;
		char s[10];
		int len = w_utf8_from_unichar(ch, s);
		w_graphics_text_extent(gc, s, len, &size, 0, W_ENCODING_UTF8);
		return size.width;
	}
	return 0;
}
wresult w_graphics_get_advanced(w_graphics *gc) {
	return W_TRUE;
}
int w_graphics_get_alpha(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _W_GRAPHICS(gc)->alpha;
	}
	return 0xFF;
}
int w_graphics_get_antialias(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		int antialias = cairo_get_antialias(_W_GRAPHICS(gc)->cairo);
		switch (antialias) {
		case CAIRO_ANTIALIAS_DEFAULT:
			return W_DEFAULT;
		case CAIRO_ANTIALIAS_NONE:
			return W_OFF;
		case CAIRO_ANTIALIAS_GRAY:
		case CAIRO_ANTIALIAS_SUBPIXEL:
			return W_ON;
		}
	}
	return W_DEFAULT;
}
w_color w_graphics_get_background(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _W_GRAPHICS(gc)->background;
	}
	return 0;
}
w_pattern* w_graphics_get_background_pattern(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _gc->backgroundPattern;
	}
	return 0;
}
int w_graphics_get_char_width(w_graphics *gc, int ch) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		w_size size;
		char s[10];
		int len = w_utf8_from_unichar(ch, s);
		w_graphics_text_extent(gc, s, len, &size, 0, W_ENCODING_UTF8);
		return size.width;
	}
	return 0;
}
void w_graphics_get_size(w_graphics *gc, int *width, int *height);
cairo_region_t* w_graphics_convert_rgn(w_graphics *gc, cairo_region_t *rgn,
		cairo_matrix_t *matrix) {
	cairo_region_t *newRgn = cairo_region_create();
	int isIdentity = 1;
	if (matrix != 0) {
		isIdentity = matrix->xx == 1 && matrix->yx == 0 && matrix->xy == 0
				&& matrix->yy == 1 && matrix->x0 == 0 && matrix->y0 == 0;
	}
	if (isIdentity) {
		cairo_region_union(newRgn, rgn);
		return newRgn;
	}
	int nRects;
	GdkRectangle *rects, *rect;
	_gdk_region_get_rectangles(rgn, &rects, &nRects);
	w_point points[4];
	double x, y;
	for (int i = 0; i < nRects; i++) {
		rect = &rects[i];
		x = rect->x;
		y = rect->y;
		cairo_matrix_transform_point(matrix, &x, &y);
		points[0].x = x;
		points[0].y = y;
		x = rect->x + rect->width;
		y = rect->y;
		cairo_matrix_transform_point(matrix, &x, &y);
		points[1].x = round(x);
		points[1].y = y;
		x = rect->x + rect->width;
		y = rect->y + rect->height;
		cairo_matrix_transform_point(matrix, &x, &y);
		points[2].x = round(x);
		points[2].y = round(y);
		x = rect->x;
		y = rect->y + rect->height;
		cairo_matrix_transform_point(matrix, &x, &y);
		points[3].x = x;
		points[3].y = round(y);
		cairo_region_t *polyRgn = _gdk_region_polygon(points, 4,
		GDK_EVEN_ODD_RULE);
		cairo_region_union(newRgn, polyRgn);
		cairo_region_destroy(polyRgn);
	}
	if (rects != 0)
		g_free(rects);
	return newRgn;
}
wresult w_graphics_get_clipping_rect(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		GdkRectangle r;
		//Calculate visible bounds in device space
		rect->x = 0, rect->y = 0, rect->width = 0, rect->height = 0;
		w_graphics_get_size(gc, &rect->width, &rect->height);
		gdk_cairo_get_clip_rectangle(_W_GRAPHICS(gc)->cairo, &r);
		// Intersect visible bounds with clipping in device space and then convert then to user space
		cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
		cairo_region_t *clipRgn = _W_GRAPHICS(gc)->clipRgn;
		cairo_region_t *damageRgn = _W_GRAPHICS(gc)->damageRgn;
		if (clipRgn != 0 || damageRgn != 0 || cairo != 0) {
			cairo_region_t *rgn = cairo_region_create();
			r.x = r.y = 0;
			r.width = rect->width;
			r.height = rect->height;
			cairo_region_union_rectangle(rgn, &r);
			if (damageRgn != 0) {
				cairo_region_intersect(rgn, damageRgn);
			}
			// Intersect visible bounds with clipping
			if (clipRgn != 0) {
				// Convert clipping to device space if needed
				if (_W_GRAPHICS(gc)->isclippingTransform
						!= 0&& GTK_VERSION < VERSION (3, 14, 0)) {
					clipRgn = w_graphics_convert_rgn(gc, clipRgn,
							&_W_GRAPHICS(gc)->clippingTransform);
					cairo_region_intersect(rgn, clipRgn);
					cairo_region_destroy(clipRgn);
				} else {
					cairo_region_intersect(rgn, clipRgn);
				}
			}
			// Convert to user space
			if (cairo != 0 && GTK_VERSION < VERSION(3, 14, 0)) {
				cairo_matrix_t matrix;
				cairo_get_matrix(cairo, &matrix);
				cairo_matrix_invert(&matrix);
				clipRgn = w_graphics_convert_rgn(gc, rgn, &matrix);
				cairo_region_destroy(rgn);
				rgn = clipRgn;
			}
			cairo_region_get_extents(rgn, &r);
			cairo_region_destroy(rgn);
			rect->x = r.x;
			rect->y = r.y;
			rect->width = r.width;
			rect->height = r.height;
		}
	}
	return result;
}
wresult w_graphics_get_clipping_region(w_graphics *gc, w_region *region) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0) {
		if (region == 0)
			return W_ERROR_NULL_ARGUMENT;
		cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
		if (_W_REGION(region)->handle == 0) {
			_W_REGION(region)->handle = cairo_region_create();
		} else {
			cairo_region_subtract(_W_REGION(region)->handle,
			_W_REGION(region)->handle);
		}
		cairo_region_t *clipping = _W_REGION(region)->handle;
		if (_W_GRAPHICS(gc)->clipRgn == 0) {
			GdkRectangle rect;
			gdk_cairo_get_clip_rectangle(_W_GRAPHICS(gc)->cairo, &rect);
			w_graphics_get_size(gc, &rect.width, &rect.height);
			cairo_region_union_rectangle(clipping, &rect);
		} else {
			// Convert clipping to device space if needed
			if (_W_GRAPHICS(gc)->isclippingTransform
					!= 0&& GTK_VERSION < VERSION (3, 14, 0)) {
				cairo_region_t *rgn = w_graphics_convert_rgn(gc,
				_W_GRAPHICS(gc)->clipRgn, &_W_GRAPHICS(gc)->clippingTransform);
				cairo_region_union(clipping, rgn);
				cairo_region_destroy(rgn);
			} else {
				cairo_region_union(clipping, _W_GRAPHICS(gc)->clipRgn);
			}
		}
		if (_W_GRAPHICS(gc)->damageRgn != 0) {
			cairo_region_intersect(clipping, _W_GRAPHICS(gc)->damageRgn);
		}
		// Convert to user space
		if (GTK_VERSION < VERSION(3, 14, 0)) {
			cairo_matrix_t matrix;
			cairo_get_matrix(cairo, &matrix);
			cairo_matrix_invert(&matrix);
			cairo_region_t *rgn = w_graphics_convert_rgn(gc, clipping, &matrix);
			cairo_region_subtract(clipping, clipping);
			cairo_region_union(clipping, rgn);
			cairo_region_destroy(rgn);
		}
	}
	return result;
}
int w_graphics_get_fill_rule(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return cairo_get_fill_rule(_W_GRAPHICS(gc)->cairo)
				== CAIRO_FILL_RULE_WINDING ? W_FILL_WINDING : W_FILL_EVEN_ODD;
	}
	return W_FILL_EVEN_ODD;
}
w_font* w_graphics_get_font(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _W_GRAPHICS(gc)->font;
	}
	return 0;
}
wresult w_graphics_get_font_metrics(w_graphics *gc, w_fontmetrics *fm) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
	if (_W_GRAPHICS(gc)->context == 0) {
		wresult result = _w_graphics_create_layout(gc);
		if (result < 0)
			return result;
	}
	result = _w_graphics_check(gc, GRAPHICS_STATE_FONT);
	if (result < 0)
		return 0;
	w_font *font = _W_GRAPHICS(gc)->font;
	PangoContext *context = _W_GRAPHICS(gc)->context;
	PangoLanguage *lang = pango_context_get_language(context);
	PangoFontMetrics *metrics = pango_context_get_metrics(context,
	_W_FONT(font)->handle, lang);
	int ascent = pango_font_metrics_get_ascent(metrics);
	int descent = pango_font_metrics_get_descent(metrics);
	int ascentInPoints = ascent;
	_W_FONTMETRICS(fm)->ascentInPoints = ascentInPoints;
	int heightInPoints = ascent + descent;
	_W_FONTMETRICS(fm)->descentInPoints = heightInPoints - ascentInPoints;
	_W_FONTMETRICS(fm)->averageCharWidthInPoints =
			pango_font_metrics_get_approximate_char_width(metrics);
	pango_font_metrics_unref(metrics);
	return W_TRUE;
}
w_color w_graphics_get_foreground(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _W_GRAPHICS(gc)->foreground;
	}
	return 0;
}
w_pattern* w_graphics_get_foreground_pattern(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _gc->foregroundPattern;
	}
	return 0;
}
int w_graphics_get_interpolation(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return W_DEFAULT;
	if (_W_GRAPHICS(gc)->interpolation == 0xFF)
		return W_DEFAULT;
	else
		return _W_GRAPHICS(gc)->interpolation - 1;
}
w_line_attributes* w_graphics_get_line_attributes(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return 0;
	return &_W_GRAPHICS(gc)->line;
}
int w_graphics_get_line_cap(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return 0;
	return _W_GRAPHICS(gc)->line.cap;
}
wresult w_graphics_get_line_dash(w_graphics *gc, int **dashes, int *length) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	*dashes = _W_GRAPHICS(gc)->line.dashes;
	*length = _W_GRAPHICS(gc)->line.length;
	return W_TRUE;
}
int w_graphics_get_line_join(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return 0;
	return _W_GRAPHICS(gc)->line.join;
}
int w_graphics_get_line_style(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return 0;
	return _W_GRAPHICS(gc)->line.style;
}
float w_graphics_get_line_width(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return 0;
	return _W_GRAPHICS(gc)->line.width;
}
void w_graphics_get_size(w_graphics *gc, int *width, int *height) {
	if (_W_GRAPHICS(gc)->width != -1 && _W_GRAPHICS(gc)->height != -1) {
		width[0] = _W_GRAPHICS(gc)->width;
		height[0] = _W_GRAPHICS(gc)->height;
		return;
	}
	cairo_surface_t *surface = cairo_get_target(_W_GRAPHICS(gc)->cairo);
	switch (cairo_surface_get_type(surface)) {
	case CAIRO_SURFACE_TYPE_IMAGE:
		width[0] = cairo_image_surface_get_width(surface);
		height[0] = cairo_image_surface_get_height(surface);
		break;
		/*case CAIRO_SURFACE_TYPE_XLIB:
		 width[0] = cairo_xlib_surface_get_width(surface);
		 height[0] = cairo_xlib_surface_get_height(surface);
		 break;*/
	}
}
int w_graphics_get_style(w_graphics *gc) {
	int style = 0;
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED)
			style |= W_MIRRORED;
	}
	return style;
}
int w_graphics_get_text_antialias(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	int antialias = CAIRO_ANTIALIAS_DEFAULT;
	if (_W_GRAPHICS(gc)->context != 0) {
		const cairo_font_options_t *options =
				pango_cairo_context_get_font_options(
				_W_GRAPHICS(gc)->context);
		if (options != 0)
			antialias = cairo_font_options_get_antialias(options);
	}
	switch (antialias) {
	case CAIRO_ANTIALIAS_DEFAULT:
		return W_DEFAULT;
	case CAIRO_ANTIALIAS_NONE:
		return W_OFF;
	case CAIRO_ANTIALIAS_GRAY:
	case CAIRO_ANTIALIAS_SUBPIXEL:
		return W_ON;
	}
	return W_DEFAULT;
}
void _w_graphics_identity(w_graphics *gc, cairo_matrix_t *identity) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if ((_gc->state & W_MIRRORED) != 0) {
		int w = 0, h = 0;
		w_graphics_get_size(gc, &w, &h);
		cairo_matrix_init(identity, -1, 0, 0, 1, w, 0);
	} else {
		cairo_matrix_init_identity(identity);
	}
	if (_gc->state & GRAPHICS_STATE_HAS_IDENTITY) {
		cairo_matrix_multiply(identity, &_gc->identity, identity);
	}
}
wresult w_graphics_get_transform(w_graphics *gc, w_transform *transform) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (transform == 0)
			return W_ERROR_NULL_ARGUMENT;
		cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
		cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
		if (cairo != 0) {
			cairo_get_matrix(cairo, handle);
			cairo_matrix_t identity;
			_w_graphics_identity(gc, &identity);
			cairo_matrix_invert(&identity);
			cairo_matrix_multiply(handle, handle, &identity);
		} else {
			cairo_matrix_init(handle, 1, 0, 0, 1, 0, 0);
		}
	}
	return result;
}
wresult w_graphics_get_xor_mode(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	return _W_GRAPHICS(gc)->xorMode;
}
wresult w_graphics_is_clipped(w_graphics *gc) {
	return W_FALSE;
}
wresult w_graphics_set_advanced(w_graphics *gc, int advanced) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (!advanced) {
			w_graphics_set_alpha(gc, 0XFF);
			w_graphics_set_antialias(gc, W_DEFAULT);
			w_graphics_set_background_pattern(gc, 0);
			w_graphics_set_clipping_rect(gc, 0);
			w_graphics_set_foreground_pattern(gc, 0);
			w_graphics_set_interpolation(gc, W_DEFAULT);
			w_graphics_set_text_antialias(gc, W_DEFAULT);
			w_graphics_set_transform(gc, 0);
		}
	}
	return result;
}
wresult w_graphics_set_antialias(w_graphics *gc, int antialias) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		cairo_antialias_t mode = 0;
		switch (antialias) {
		case W_DEFAULT:
			mode = CAIRO_ANTIALIAS_DEFAULT;
			break;
		case W_OFF:
			mode = CAIRO_ANTIALIAS_NONE;
			break;
		case W_ON:
			mode = CAIRO_ANTIALIAS_GRAY;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		cairo_set_antialias(_W_GRAPHICS(gc)->cairo, mode);
	}
	return result;
}
wresult w_graphics_set_alpha(w_graphics *gc, int alpha) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	if ((_W_GRAPHICS(gc)->alpha & 0xff) == (alpha & 0xff))
		return W_TRUE;
	_W_GRAPHICS(gc)->alpha = alpha & 0xff;
	_W_GRAPHICS(gc)->state &= ~(GRAPHICS_STATE_BACKGROUND
			| GRAPHICS_STATE_FOREGROUND | GRAPHICS_STATE_BACKGROUND_BG);
	return W_TRUE;
}
wresult w_graphics_set_background(w_graphics *gc, w_color color) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->cairo == 0)
		return W_ERROR_NO_HANDLES;
	_gc->background = color & 0x00FFFFFF;
	_gc->backgroundPattern = 0;
	_gc->state &= ~(GRAPHICS_STATE_BACKGROUND | GRAPHICS_STATE_BACKGROUND_BG);
	return W_TRUE;
}
wresult w_graphics_set_background_pattern(w_graphics *gc, w_pattern *pattern) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		if (pattern == 0 || _W_PATTERN(pattern)->handle != 0) {
			if (_gc->backgroundPattern != pattern) {
				_gc->backgroundPattern = pattern;
				_gc->state &= ~GRAPHICS_STATE_BACKGROUND;
			}
		} else
			result = W_ERROR_INVALID_ARGUMENT;
	}
	return result;
}
/**
 * Intersects given clipping with original clipping of this context, so
 * that resulting clip does not allow to paint outside of the GC bounds.
 */
wresult w_graphics_limitClipping(w_graphics *gc, cairo_region_t *gcClipping) {
	cairo_region_t *clippingRegion = cairo_region_create();
	cairo_region_union_rectangle(clippingRegion, &_W_GRAPHICS(gc)->clipping);
	cairo_region_intersect(gcClipping, clippingRegion);
	cairo_region_destroy(clippingRegion);
	return W_TRUE;
}
wresult w_graphics_set_cairo_clip(w_graphics *gc, cairo_region_t *damageRgn,
		cairo_region_t *clipRgn) {
#if GTK3
	cairo_reset_clip(_W_GRAPHICS(gc)->cairo);
#endif
	if (damageRgn != 0) {
		cairo_matrix_t matrix;
		cairo_get_matrix(_W_GRAPHICS(gc)->cairo, &matrix);
		cairo_matrix_t identity;
		cairo_matrix_init_identity(&identity);
		cairo_set_matrix(_W_GRAPHICS(gc)->cairo, &identity);
		gdk_cairo_region(_W_GRAPHICS(gc)->cairo, damageRgn);
		cairo_clip(_W_GRAPHICS(gc)->cairo);
		cairo_set_matrix(_W_GRAPHICS(gc)->cairo, &matrix);
	}
	if (clipRgn != 0) {
		cairo_region_t *clipRgnCopy = cairo_region_create();
		cairo_region_union(clipRgnCopy, clipRgn);

		/*
		 * Bug 531667: widgets paint over other widgets
		 *
		 * The Cairo handle is shared by all widgets, but GC.setClipping allows global clipping changes.
		 * So we intersect whatever the client sets with the initial GC clipping.
		 */
		if (GTK_VERSION >= VERSION(3, 14, 0)) {
			w_graphics_limitClipping(gc, clipRgnCopy);
		}

		gdk_cairo_region(_W_GRAPHICS(gc)->cairo, clipRgnCopy);
		cairo_clip(_W_GRAPHICS(gc)->cairo);
		cairo_region_destroy(clipRgnCopy);
	}
	return W_TRUE;
}
wresult w_graphics_set_clipping(w_graphics *gc, cairo_region_t *clipRgn) {
	if (clipRgn == 0) {
		if (_W_GRAPHICS(gc)->clipRgn != 0) {
			cairo_region_destroy(_W_GRAPHICS(gc)->clipRgn);
			_W_GRAPHICS(gc)->clipRgn = 0;
		}
		_W_GRAPHICS(gc)->isclippingTransform = 0;
		w_graphics_set_cairo_clip(gc, _W_GRAPHICS(gc)->damageRgn, 0);
	} else {
		if (_W_GRAPHICS(gc)->clipRgn == 0)
			_W_GRAPHICS(gc)->clipRgn = cairo_region_create();
		cairo_region_subtract(_W_GRAPHICS(gc)->clipRgn,
		_W_GRAPHICS(gc)->clipRgn);
		cairo_region_union(_W_GRAPHICS(gc)->clipRgn, clipRgn);
		if (GTK_VERSION < VERSION(3, 14, 0)) {
			_W_GRAPHICS(gc)->isclippingTransform = 1;
			cairo_get_matrix(_W_GRAPHICS(gc)->cairo,
					&_W_GRAPHICS(gc)->clippingTransform);
		}
		w_graphics_set_cairo_clip(gc, _W_GRAPHICS(gc)->damageRgn, clipRgn);
	}
	return W_TRUE;
}
void w_graphics_reset_clipping(w_graphics *gc) {
	if (GTK_VERSION >= VERSION(3, 14, 0)) {
		/*
		 * Bug 531667: widgets paint over other widgets
		 *
		 * The Cairo handle is shared by all widgets, and GC.setClipping(0) allows painting outside the current GC area.
		 * So if we reset any custom clipping we still want to restrict GC operations with the initial GC clipping.
		 */
		cairo_region_t *clipRgn = cairo_region_create();
		cairo_region_union_rectangle(clipRgn, &_W_GRAPHICS(gc)->clipping);
		w_graphics_set_clipping(gc, clipRgn);
		cairo_region_destroy(clipRgn);
	} else {
		w_graphics_set_clipping(gc, 0);
	}
}
wresult w_graphics_set_clipping_path(w_graphics *gc, w_path *path) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (path != 0 && w_path_is_ok(path) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		w_graphics_reset_clipping(gc);
		if (path != 0) {
			cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
			cairo_path_t *copy = cairo_copy_path(_W_PATH(path)->handle);
			if (copy == 0)
				return W_ERROR_NO_HANDLES;
			cairo_append_path(cairo, copy);
			cairo_path_destroy(copy);
			cairo_clip(cairo);
		}
	}
	return result;
}
wresult w_graphics_set_clipping_rect(w_graphics *gc, w_rect *r) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (r == 0)
		return W_ERROR_NULL_ARGUMENT;
	GdkRectangle rect;
	rect.x = r->x;
	rect.y = r->y;
	rect.width = r->width;
	rect.height = r->height;
	if (rect.width < 0) {
		rect.x = rect.x + rect.width;
		rect.width = -rect.width;
	}
	if (rect.height < 0) {
		rect.y = rect.y + rect.height;
		rect.height = -rect.height;
	}

	cairo_region_t *clipRgn = cairo_region_create();
	cairo_region_union_rectangle(clipRgn, &rect);
	w_graphics_set_clipping(gc, clipRgn);
	cairo_region_destroy(clipRgn);
	return W_TRUE;
}
wresult w_graphics_set_clipping_region(w_graphics *gc, w_region *region) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (region == 0) {
		w_graphics_reset_clipping(gc);
	} else {
		if (_W_REGION(region)->handle == 0)
			return W_ERROR_INVALID_ARGUMENT;

		w_graphics_set_clipping(gc, _W_REGION(region)->handle);
	}
	return W_TRUE;
}
wresult w_graphics_set_fill_rule(w_graphics *gc, int rule) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		cairo_fill_rule_t cairo_mode = CAIRO_FILL_RULE_EVEN_ODD;
		switch (rule) {
		case W_FILL_WINDING:
			cairo_mode = CAIRO_FILL_RULE_WINDING;
			break;
		case W_FILL_EVEN_ODD:
			cairo_mode = CAIRO_FILL_RULE_EVEN_ODD;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		cairo_set_fill_rule( _W_GRAPHICS(gc)->cairo, cairo_mode);
	}
	return result;
}
wresult w_graphics_set_font(w_graphics *gc, w_font *font) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (font != 0 && _W_FONT(font)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	_gc->font = font != 0 ? font : w_toolkit_get_system_font(0);
	_gc->state &= ~GRAPHICS_STATE_FONT;
	_gc->stringWidth = _gc->stringHeight = -1;
	return W_TRUE;
}
wresult w_graphics_set_foreground(w_graphics *gc, w_color color) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->cairo == 0)
		return W_ERROR_NO_HANDLES;
	_gc->foreground = color & 0x00FFFFFF;
	_gc->foregroundPattern = 0;
	_gc->state &= ~GRAPHICS_STATE_FOREGROUND;
	return W_TRUE;
}
wresult w_graphics_set_foreground_pattern(w_graphics *gc, w_pattern *pattern) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (pattern != 0 && w_pattern_is_ok(pattern) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (_gc->foregroundPattern == pattern)
			return result;
		_gc->foregroundPattern = pattern;
		_gc->state &= ~GRAPHICS_STATE_FOREGROUND;
	}
	return result;
}
wresult w_graphics_set_interpolation(w_graphics *gc, int interpolation) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		switch (interpolation) {
		case W_DEFAULT:
		case W_NONE:
		case W_LOW:
		case W_HIGH:
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		_W_GRAPHICS(gc)->interpolation = interpolation + 1;
	}
	return result;
}
wresult w_graphics_set_line_attributes(w_graphics *gc,
		w_line_attributes *attributes) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->cairo == 0)
		return W_ERROR_NO_HANDLES;
	int mask = 0;
	w_line_attributes *line = &_gc->line;
	float lineWidth = attributes->width;
	if (lineWidth != line->width) {
		mask |= GRAPHICS_STATE_LINE_WIDTH | GRAPHICS_STATE_DRAW_OFFSET;
	}
	int lineStyle = attributes->style;
	if (lineStyle != line->style) {
		mask |= GRAPHICS_STATE_LINE_STYLE;
		switch (lineStyle) {
		case W_LINE_SOLID:
		case W_LINE_DASH:
		case W_LINE_DOT:
		case W_LINE_DASHDOT:
		case W_LINE_DASHDOTDOT:
			break;
		case W_LINE_CUSTOM:
			if (attributes->dashes == 0)
				lineStyle = W_LINE_SOLID;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
	}
	int join = attributes->join;
	if (join != line->join) {
		mask |= GRAPHICS_STATE_LINE_JOIN;
		switch (join) {
		case W_JOIN_MITER:
		case W_JOIN_ROUND:
		case W_JOIN_BEVEL:
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
	}
	int cap = attributes->cap;
	if (cap != line->cap) {
		mask |= GRAPHICS_STATE_LINE_CAP;
		switch (cap) {
		case W_CAP_FLAT:
		case W_CAP_ROUND:
		case W_CAP_SQUARE:
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
	}
	int *dashes = attributes->dashes;
	int *lineDashes = line->dashes;
	if (dashes != 0 && attributes->length > 0) {
		int changed = lineDashes == 0 || line->length != attributes->length;
		int length = attributes->length;
		for (int i = 0; i < length; i++) {
			float dash = dashes[i];
			if (dash <= 0)
				return W_ERROR_INVALID_ARGUMENT;
			if (!changed && lineDashes[i] != dash)
				changed = W_TRUE;
		}
		if (changed) {
			int *newDashes = (int*) realloc(line->dashes, length * sizeof(int));
			if (newDashes == 0)
				return W_ERROR_NO_MEMORY;
			memcpy(newDashes, dashes, length * sizeof(int));
			line->length = length;
			dashes = newDashes;
			mask |= GRAPHICS_STATE_LINE_STYLE;
		} else {
			dashes = lineDashes;
		}
	} else {
		if (lineDashes != 0 && line->length > 0) {
			mask |= GRAPHICS_STATE_LINE_STYLE;
		} else {
			dashes = lineDashes;
		}
	}
	float dashOffset = attributes->dashOffset;
	if (dashOffset != line->dashOffset) {
		mask |= GRAPHICS_STATE_LINE_STYLE;
	}
	float miterLimit = attributes->miterLimit;
	if (miterLimit != line->miterLimit) {
		mask |= GRAPHICS_STATE_LINE_MITERLIMIT;
	}
	if (mask != 0) {
		line->width = lineWidth;
		line->style = lineStyle;
		line->cap = cap;
		line->join = join;
		line->dashes = dashes;
		line->dashOffset = dashOffset;
		line->miterLimit = miterLimit;
		_gc->state &= ~mask;
	}
	return W_TRUE;
}
wresult w_graphics_set_line_cap(w_graphics *gc, int cap) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (_gc->line.cap == cap)
		return W_TRUE;
	switch (cap) {
	case W_CAP_ROUND:
	case W_CAP_FLAT:
	case W_CAP_SQUARE:
		break;
	default:
		return W_TRUE;
	}
	_gc->line.cap = cap;
	_gc->state &= ~GRAPHICS_STATE_LINE_CAP;
	return W_TRUE;
}
wresult w_graphics_set_line_dash(w_graphics *gc, int *dashes, int length) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->cairo == 0)
		return W_ERROR_NO_HANDLES;
	w_line_attributes *line = &_gc->line;
	int *lineDashes = line->dashes;
	if (dashes != 0 && length > 0) {
		wresult changed = line->style != W_LINE_CUSTOM || lineDashes == 0
				|| line->length != length;
		if (lineDashes != 0) {
			for (int i = 0; i < length; i++) {
				int dash = dashes[i];
				if (dash <= 0)
					dash = 0;	//W_ERROR_INVALID_ARGUMENT;
				if (!changed && lineDashes[i] != dash)
					changed = W_TRUE;
			}
		}
		if (!changed)
			return W_TRUE;

		lineDashes = (int*) realloc(lineDashes, length * sizeof(int));
		line->length = length;
		for (int i = 0; i < length; i++) {
			lineDashes[i] = dashes[i];
		}
		line->dashes = lineDashes;
		line->style = W_LINE_CUSTOM;
	} else {
		if (line->style == W_LINE_SOLID
				&& (lineDashes == 0 || line->length == 0))
			return W_TRUE;
		if (line->dashes != 0) {
			free(line->dashes);
		}
		line->dashes = 0;
		line->style = W_LINE_SOLID;
	}
	_gc->state &= ~GRAPHICS_STATE_LINE_STYLE;
	return W_TRUE;
}
wresult w_graphics_set_line_join(w_graphics *gc, int join) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (_gc->line.join == join)
		return W_TRUE;
	switch (join) {
	case W_JOIN_MITER:
	case W_JOIN_ROUND:
	case W_JOIN_BEVEL:
		break;
	default:
		return W_ERROR_INVALID_ARGUMENT;
	}
	_gc->line.join = join;
	_gc->state &= ~GRAPHICS_STATE_LINE_JOIN;
	return W_TRUE;
}
wresult w_graphics_set_line_style(w_graphics *gc, int lineStyle) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (_gc->line.style == lineStyle)
		return W_TRUE;
	switch (lineStyle) {
	case W_LINE_SOLID:
	case W_LINE_DASH:
	case W_LINE_DOT:
	case W_LINE_DASHDOT:
	case W_LINE_DASHDOTDOT:
		break;
	case W_LINE_CUSTOM:
		if (_gc->line.dashes == 0)
			lineStyle = W_LINE_SOLID;
		break;
	default:
		return W_ERROR_INVALID_ARGUMENT;
	}
	_gc->line.style = lineStyle;
	_gc->state &= ~GRAPHICS_STATE_LINE_STYLE;
	return W_TRUE;
}
wresult w_graphics_set_line_width(w_graphics *gc, float lineWidth) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (_gc->line.width == lineWidth)
		return W_TRUE;
	_gc->line.width = lineWidth;
	_gc->state &= ~(GRAPHICS_STATE_LINE_WIDTH | GRAPHICS_STATE_DRAW_OFFSET);
	return W_TRUE;
}
wresult w_graphics_set_xor_mode(w_graphics *gc, int _xor) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (cairo_version() >= CAIRO_VERSION_ENCODE(1, 10, 0)) {
			cairo_set_operator(_W_GRAPHICS(gc)->cairo,
					_xor ? CAIRO_OPERATOR_DIFFERENCE : CAIRO_OPERATOR_OVER);
		}
		_W_GRAPHICS(gc)->xorMode = _xor;
	}
	return result;
}
wresult w_graphics_set_text_antialias(w_graphics *gc, int antialias) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		cairo_antialias_t mode = 0;
		switch (antialias) {
		case W_DEFAULT:
			mode = CAIRO_ANTIALIAS_DEFAULT;
			break;
		case W_OFF:
			mode = CAIRO_ANTIALIAS_NONE;
			break;
		case W_ON:
			mode = CAIRO_ANTIALIAS_GRAY;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		cairo_font_options_t *options = cairo_font_options_create();
		cairo_font_options_set_antialias(options, mode);
		if (_W_GRAPHICS(gc)->context == 0)
			_w_graphics_create_layout(gc);
		pango_cairo_context_set_font_options(_W_GRAPHICS(gc)->context, options);
		cairo_font_options_destroy(options);
	}
	return result;
}
wresult w_graphics_set_transform(w_graphics *gc, w_transform *transform) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (!w_transform_is_ok(transform))
			return W_ERROR_INVALID_ARGUMENT;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		cairo_t *cairo = _gc->cairo;
		cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
		cairo_matrix_t identity;
		_w_graphics_identity(gc, &identity);
		if (transform != 0) {
			cairo_matrix_multiply(&identity, handle, &identity);
		}
		if (GTK_VERSION >= VERSION(3, 14, 0)) {
			cairo_transform(cairo, &identity);
		} else {
			cairo_set_matrix(cairo, &identity);
		}
		_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_DRAW_OFFSET;
	}
	return result;
}
wresult w_graphics_text_extent(w_graphics *gc, const char *string, int length,
		w_size *size, int flags, int enc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		if (string == 0)
			return W_ERROR_INVALID_ARGUMENT;
		result = _w_graphics_set_text(gc, string, length, flags, enc);
		if (result > 0) {
			result = _w_graphics_check(gc, GRAPHICS_STATE_FONT);
			if (result > 0) {
				pango_layout_get_pixel_size(_gc->layout, &size->width,
						&size->height);
			}
		}
	}
	return result;
}
