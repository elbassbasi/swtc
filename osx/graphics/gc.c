/*
 * Name:        gc.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"

void w_graphics_init(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	_gc->handle = 0;
}
void _w_graphics_init(w_graphics *gc, NSGraphicsContext *context, int flags) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	memset(gc, 0, sizeof(_w_graphics));
	_gc->state = -1;
	_gc->handle = context;
	_gc->path = NSBezierPath_bezierPath();
	_gc->alpha = 0xFF;
}

void w_graphics_dispose(w_graphics *gc) {
}
wuchar LINE_DOT[] = { 1, 1 };
wuchar LINE_DASH[] = { 3, 1 };
wuchar LINE_DASHDOT[] = { 3, 1, 1, 1 };
wuchar LINE_DASHDOTDOT[] = { 3, 1, 1, 1, 1, 1 };
wuchar LINE_DOT_ZERO[] = { 3, 3 };
wuchar LINE_DASH_ZERO[] = { 18, 6 };
wuchar LINE_DASHDOT_ZERO[] = { 9, 6, 3, 6 };
wuchar LINE_DASHDOTDOT_ZERO[] = { 9, 3, 3, 3, 3, 3 };
wresult _w_graphics_check(w_graphics *gc, int mask) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_gc->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_gc->flippedContext != 0
			&& _gc->handle != NSGraphicsContext_currentContext()) {
		_gc->restoreContext = W_TRUE;
		NSGraphicsContext_static_saveGraphicsState();
		NSGraphicsContext_setCurrentContext(_gc->handle);
	}
	if ((mask & (GRAPHICS_STATE_CLIPPING | GRAPHICS_STATE_TRANSFORM)) != 0) {
		NSView *view = _gc->view;
		if ((_gc->state & GRAPHICS_STATE_CLIPPING) == 0
				|| (_gc->state & GRAPHICS_STATE_TRANSFORM) == 0
				|| (_gc->state & GRAPHICS_STATE_VISIBLE_REGION) == 0) {
			int antialias = NSGraphicsContext_shouldAntialias(_gc->handle);
			NSGraphicsContext_restoreGraphicsState(_gc->handle);
			NSGraphicsContext_saveGraphicsState(_gc->handle);
			NSGraphicsContext_setShouldAntialias(_gc->handle, antialias);
			if (view != 0 && (!_gc->ispaintRect || !NSView_isFlipped(view))) {
				NSAffineTransform *transform = NSAffineTransform_transform();
				NSRect rect, bounds;
				NSView_bounds(view, &bounds);
				NSView_convertRect_toView_(view, &rect, &bounds, 0);
				if (_gc->ispaintRect == 0) {
					NSAffineTransform_translateXBy(transform, rect.x,
							rect.y + rect.height);
				} else {
					NSAffineTransform_translateXBy(transform, 0, rect.height);
				}
				NSAffineTransform_scaleXBy(transform, 1, -1);
				NSAffineTransform_concat(transform);
				if (_gc->visibleRgn != 0) {
					if (_gc->visiblePath == 0
							|| (_gc->state & GRAPHICS_STATE_VISIBLE_REGION)
									== 0) {
						if (_gc->visiblePath != 0) {
							NSObject_release(NSOBJECT(_gc->visiblePath));
						}
						//_gc->visiblePath = 0;
					}
					NSBezierPath_addClip(_gc->visiblePath);
					_gc->state |= GRAPHICS_STATE_VISIBLE_REGION;
				}
			}
			if (_gc->clipPath != 0) {
				NSBezierPath_addClip(_gc->clipPath);
			}
			if (_gc->transform != 0) {
				NSAffineTransform_concat(_gc->transform);
			}
			mask &= ~(GRAPHICS_STATE_TRANSFORM | GRAPHICS_STATE_CLIPPING);
			_gc->state |= GRAPHICS_STATE_TRANSFORM | GRAPHICS_STATE_CLIPPING;
			_gc->state &= ~(GRAPHICS_STATE_BACKGROUND
					| GRAPHICS_STATE_FOREGROUND);
		}
	}

	//CGContextSetBlendMode(NSGraphicsContext_graphicsPort(_gc->handle), _gc->xorMode ? kCGBlendModeDifference : kCGBlendModeNormal);
	int state = _gc->state;
	if ((state & mask) == mask)
		return W_TRUE;
	state = (state ^ mask) & mask;
	_gc->state |= mask;

	if ((state & GRAPHICS_STATE_FOREGROUND) != 0) {
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0) {
			NSColor *color = _W_PATTERN(pattern)->color;
			if (color != 0 && _W_PATTERN(pattern)->is_color) {
				NSColor_setStroke(color);
			}
		} else {
			w_color color = _gc->foreground;
			if (_gc->fg != 0) {
				NSObject_release(NSOBJECT(_gc->fg));
			}
			NSColor *fg = _gc->fg = NSColor_colorWithDeviceRed(W_RED(color),
					W_GREEN(color), W_BLUE(color), _gc->alpha / 255.0);
			NSObject_retain(NSOBJECT(fg));
			NSColor_setStroke(fg);
		}
	}
	if ((state & GRAPHICS_STATE_FOREGROUND_FILL) != 0) {
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0) {
			NSColor *color = _W_PATTERN(pattern)->color;
			if (color != 0 && _W_PATTERN(pattern)->is_color) {
				NSColor_setFill(color);
			}
		} else {
			w_color color = _gc->foreground;
			if (_gc->fg != 0) {
				NSObject_release(NSOBJECT(_gc->fg));
			}
			NSColor *fg = _gc->fg = NSColor_colorWithDeviceRed(W_RED(color),
					W_GREEN(color), W_BLUE(color), _gc->alpha / 255.0);
			NSObject_retain(NSOBJECT(fg));
			NSColor_setFill(fg);
		}
		_gc->state &= ~GRAPHICS_STATE_BACKGROUND;
	}
	if ((state & GRAPHICS_STATE_BACKGROUND) != 0) {
		w_pattern *pattern = _gc->backgroundPattern;
		if (pattern != 0) {
			NSColor *color = _W_PATTERN(pattern)->color;
			if (color != 0 && _W_PATTERN(pattern)->is_color) {
				NSColor_setFill(color);
			}
		} else {
			w_color color = _gc->background;
			if (_gc->bg != 0) {
				NSObject_release(NSOBJECT(_gc->bg));
			}
			NSColor *bg = _gc->bg = NSColor_colorWithDeviceRed(W_RED(color),
					W_GREEN(color), W_BLUE(color), _gc->alpha / 255.0);
			NSObject_retain(NSOBJECT(bg));
			NSColor_setFill(bg);
		}
		_gc->state &= ~GRAPHICS_STATE_FOREGROUND_FILL;
	}
	NSBezierPath *path = _gc->path;
	w_line_attributes *line = &_gc->line;
	if ((state & GRAPHICS_STATE_LINE_WIDTH) != 0) {
		NSBezierPath_setLineWidth(path, line->width == 0 ? 1 : line->width);
		switch (line->style) {
		case W_LINE_DOT:
		case W_LINE_DASH:
		case W_LINE_DASHDOT:
		case W_LINE_DASHDOTDOT:
			state |= GRAPHICS_STATE_LINE_STYLE;
		}
	}
	if ((state & GRAPHICS_STATE_LINE_STYLE) != 0) {
		wuchar *_dashes = 0;
		int _dashes_size = 0;
		float width = line->width;
		switch (line->style) {
		case W_LINE_SOLID:
			break;
		case W_LINE_DASH:
			_dashes = width != 0 ? LINE_DASH : LINE_DASH_ZERO;
			_dashes_size =
					width != 0 ? sizeof(LINE_DASH) : sizeof(LINE_DASH_ZERO);
			break;
		case W_LINE_DOT:
			_dashes = width != 0 ? LINE_DOT : LINE_DOT_ZERO;
			_dashes_size =
					width != 0 ? sizeof(LINE_DOT) : sizeof(LINE_DOT_ZERO);
			break;
		case W_LINE_DASHDOT:
			_dashes = width != 0 ? LINE_DASHDOT : LINE_DASHDOT_ZERO;
			_dashes_size =
					width != 0 ?
							sizeof(LINE_DASHDOT) : sizeof(LINE_DASHDOT_ZERO);
			break;
		case W_LINE_DASHDOTDOT:
			_dashes = width != 0 ? LINE_DASHDOTDOT : LINE_DASHDOTDOT_ZERO;
			_dashes_size =
					width != 0 ?
							sizeof(LINE_DASHDOTDOT) :
							sizeof(LINE_DASHDOTDOT_ZERO);
			break;
		}
		if (line->style == W_LINE_CUSTOM && line->dashes != 0) {
			CGFloat *lengths = malloc(line->length * sizeof(CGFloat));
			if (lengths != 0) {
				int *dashes = line->dashes;
				for (int i = 0; i < _dashes_size; i++) {
					lengths[i] = width == 0 || dashes[i];
				}
				NSBezierPath_setLineDash(path, lengths, _dashes_size,
						line->dashOffset);
				free(lengths);
			}
		} else if (_dashes != 0) {
			CGFloat *lengths = malloc(_dashes_size * sizeof(CGFloat));
			if (lengths != 0) {
				for (int i = 0; i < _dashes_size; i++) {
					lengths[i] = width == 0 || _dashes[i] * width;
				}
				NSBezierPath_setLineDash(path, lengths, _dashes_size,
						line->dashOffset);
				free(lengths);
			}
		} else {
			NSBezierPath_setLineDash(path, 0, 0, 0);
		}
	}
	if ((state & GRAPHICS_STATE_LINE_MITERLIMIT) != 0) {
		NSBezierPath_setMiterLimit(path, line->miterLimit);
	}
	if ((state & GRAPHICS_STATE_LINE_JOIN) != 0) {
		int joinStyle = 0;
		switch (line->join) {
		case W_JOIN_MITER:
			joinStyle = NSMiterLineJoinStyle;
			break;
		case W_JOIN_ROUND:
			joinStyle = NSRoundLineJoinStyle;
			break;
		case W_JOIN_BEVEL:
			joinStyle = NSBevelLineJoinStyle;
			break;
		}
		NSBezierPath_setLineJoinStyle(path, joinStyle);
	}
	if ((state & GRAPHICS_STATE_LINE_CAP) != 0) {
		int capStyle = 0;
		switch (line->cap) {
		case W_CAP_ROUND:
			capStyle = NSRoundLineCapStyle;
			break;
		case W_CAP_FLAT:
			capStyle = NSButtLineCapStyle;
			break;
		case W_CAP_SQUARE:
			capStyle = NSSquareLineCapStyle;
			break;
		}
		NSBezierPath_setLineCapStyle(path, capStyle);
	}
	return W_TRUE;
}
wresult _w_graphics_uncheck(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	NSGraphicsContext_flushGraphics(_gc->handle);
	return W_TRUE;
}
wresult w_graphics_create(w_graphics *gc) {
	return W_FALSE;
}
wresult w_graphics_is_ok(w_graphics *gc) {
	return W_FALSE;
}
wresult w_graphics_copy_area_to_image(w_graphics *gc, w_surface *surface, int x,
		int y) {
	return W_FALSE;
}
wresult w_graphics_copy_area(w_graphics *gc, int srcX, int srcY, int width,
		int height, int destX, int destY, wresult paint) {
	return W_FALSE;
}
void _w_graphics_create_layout(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	NSSize size;
	size.width = MAX_TEXT_CONTAINER_SIZE;
	size.height = MAX_TEXT_CONTAINER_SIZE;
	NSTextStorage *textStorage = NSTextStorage_init();
	NSLayoutManager *layoutManager = NSLayoutManager_init();
	NSLayoutManager_setBackgroundLayoutEnabled(layoutManager,
			NSThread_isMainThread());
	NSTextContainer *textContainer = NSTextContainer_initWithContainerSize(
			&size);
	NSTextContainer_setLineFragmentPadding(textContainer, 0);
	NSTextStorage_addLayoutManager(textStorage, layoutManager);
	NSLayoutManager_addTextContainer(layoutManager, textContainer);
	//NSObject_release(NSOBJECT(layoutManager));
	//NSObject_release(NSOBJECT(textContainer));
	_gc->textContainer = textContainer;
	_gc->layoutManager = layoutManager;
	_gc->textStorage = textStorage;
}

NSAttributedString* _w_graphics_create_string(w_graphics *gc,
		const char *string, int length, int flags, int draw, int enc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	NSMutableDictionary *dict = NSMutableDictionary_initWithCapacity(5);
	w_font *font = _gc->font;
	NSMutableDictionary_setObject(dict, (NSObject*) _W_FONT(font)->handle,
			(NSObject*) _NSFontAttributeName());
	_w_font_add_traits_1(font, dict);
	if (draw) {
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0) {
			if (_W_PATTERN(pattern)->color != 0
					&& _W_PATTERN(pattern)->is_color) {
				NSMutableDictionary_setObject(dict,
						(NSObject*) _W_PATTERN(pattern)->color,
						(NSObject*) _NSForegroundColorAttributeName);
			}
		} else {
			/*NSColor *fg = _gc->fg;
			 if (fg == 0) {
			 w_color color = _gc->foreground;
			 fg = _gc->fg = NSColor_colorWithDeviceRed(
			 W_RED(color), W_GREEN(color), W_BLUE(color),
			 _gc->alpha / 255.0);
			 NSObject_retain(NSOBJECT(fg));
			 }
			 NSMutableDictionary_setObject(dict, (NSObject*)fg,
			 (NSObject*)_NSForegroundColorAttributeName);*/
		}
	}
	if ((flags & W_DRAW_TAB) == 0) {
		//NSMutableDictionary_setObject(dict, mac_toolkit->paragraphStyle, _NSParagraphStyleAttributeName);
	}
	NSString *str = NSString_new(string, length, enc);
	NSAttributedString *attribStr = NSAttributedString_initWithString_0(str,
			(NSDictionary*) dict);
	//NSObject_release(NSOBJECT(dict));
	//NSObject_release(NSOBJECT(str));
	return attribStr;
}

wresult w_graphics_draw_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int x = rect->x;
		int y = rect->y;
		int width = rect->width;
		int height = rect->height;
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
		if (width == 0 || height == 0 || arcAngle == 0)
			return result;
		NSGraphicsContext_saveGraphicsState(_gc->handle);
		NSAffineTransform *transform = NSAffineTransform_transform();
		CGFloat xOffset = _gc->drawXOffset;
		CGFloat yOffset = _gc->drawYOffset;
		NSAffineTransform_translateXBy(transform, x + xOffset + width / 2.0,
				y + yOffset + height / 2.0);
		NSAffineTransform_scaleXBy(transform, width / 2.0, height / 2.0);
		NSBezierPath *path = _gc->path;
		NSPoint center;
		center.x = 0;
		center.y = 0;
		CGFloat sAngle = -startAngle;
		CGFloat eAngle = -(startAngle + arcAngle);
		NSBezierPath_appendBezierPathWithArcWithCenter_0(path, &center, 1,
				sAngle, eAngle, arcAngle > 0);
		NSBezierPath_transformUsingAffineTransform(path, transform);
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_stroke_pattern(gc, path, pattern);
		} else {
			NSBezierPath_stroke(path);
		}
		NSBezierPath_removeAllPoints(path);
		NSGraphicsContext_restoreGraphicsState(_gc->handle);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_image_0(w_graphics *gc, w_image *image,
		w_surface *surface, w_rect *src, w_rect *dest, int state) {
	NSImage *imageHandle = (NSImage*) _W_IMAGE(image)->handle;
	NSSize size;
	NSImage_size(imageHandle, &size);
	int imgWidth = (int) size.width;
	int imgHeight = (int) size.height;
	int srcX = src->x, srcY = src->y, srcWidth = src->width, srcHeight =
			src->height;
	int destX = dest->x, destY = dest->y, destWidth = dest->width, destHeight =
			dest->height;
	if (srcWidth < 0)
		srcWidth = imgWidth;
	if (srcHeight < 0)
		srcHeight = imgHeight;
	if (destWidth < 0)
		destWidth = imgWidth;
	if (destHeight < 0)
		destHeight = imgHeight;
	wresult result = _w_graphics_check(gc,
			GRAPHICS_STATE_CLIPPING | GRAPHICS_STATE_TRANSFORM);
	if (result > 0) {
		if (surface != 0) {
			//srcImage.createAlpha();
		}
		NSGraphicsContext_saveGraphicsState(_W_GRAPHICS(gc)->handle);
		NSAffineTransform *transform = NSAffineTransform_transform();
		NSAffineTransform_scaleXBy(transform, 1, -1);
		NSAffineTransform_translateXBy(transform, 0, -(destHeight + 2 * destY));
		NSAffineTransform_concat(transform);
		NSRect srcRect;
		srcRect.x = srcX;
		srcRect.y = imgHeight - (srcY + srcHeight);
		srcRect.width = srcWidth;
		srcRect.height = srcHeight;
		NSRect destRect;
		destRect.x = destX;
		destRect.y = destY;
		destRect.width = destWidth;
		destRect.height = destHeight;
		NSImage_drawInRect(imageHandle, &destRect, &srcRect,
				NSCompositeSourceOver, _W_GRAPHICS(gc)->alpha / 255.0);
		NSGraphicsContext_restoreGraphicsState(_W_GRAPHICS(gc)->handle);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_image(w_graphics *gc, w_image *image, w_rect *src,
		w_rect *dest, int state) {
	return w_graphics_draw_image_0(gc, image, 0, src, dest, state);
}
wresult w_graphics_draw_surface(w_graphics *gc, w_surface *surface, w_rect *src,
		w_rect *dest) {
	return W_FALSE;
}
wresult w_graphics_draw_line(w_graphics *gc, w_point *pt1, w_point *pt2) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		NSBezierPath *path = _gc->path;
		NSPoint pt;
		pt.x = pt1->x + _gc->drawXOffset;
		pt.y = pt1->y + _gc->drawYOffset;
		NSBezierPath_moveToPoint(path, &pt);
		pt.x = pt2->x + _gc->drawXOffset;
		pt.y = pt2->y + _gc->drawYOffset;
		NSBezierPath_lineToPoint(path, &pt);
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_stroke_pattern(gc, path, pattern);
		} else {
			NSBezierPath_stroke(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_oval(w_graphics *gc, w_rect *rect) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		NSRect r;
		if (rect->width >= 0) {
			r.origin.x = rect->x + _gc->drawXOffset;
			r.size.width = rect->width;
		} else {
			r.origin.x = rect->x + rect->width + _gc->drawXOffset;
			r.size.width = -rect->width;
		}
		if (rect->height >= 0) {
			r.origin.y = rect->y + _gc->drawYOffset;
			r.size.height = rect->height;
		} else {
			r.origin.y = rect->y + rect->height + _gc->drawYOffset;
			r.size.height = -rect->height;
		}
		NSBezierPath *path = _gc->path;
		NSBezierPath_appendBezierPathWithOvalInRect(path, &r);
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_stroke_pattern(gc, path, pattern);
		} else {
			NSBezierPath_stroke(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_path(w_graphics *gc, w_path *path) {
	return W_FALSE;
}
wresult w_graphics_draw_point(w_graphics *gc, w_point *pt) {
	wresult result = _w_graphics_check(gc,
			GRAPHICS_STATE_FOREGROUND_FILL | GRAPHICS_STATE_CLIPPING
					| GRAPHICS_STATE_TRANSFORM);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		NSRect rect;
		rect.origin.x = pt->x;
		rect.origin.y = pt->y;
		rect.size.width = 1;
		rect.size.height = 1;
		NSBezierPath *path = _gc->path;
		NSBezierPath_appendBezierPathWithRect(path, &rect);
		NSBezierPath_fill(path);
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		if (pointArray == 0)
			return W_ERROR_NULL_ARGUMENT;
		if (count < 2)
			return result;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		double xOffset = _gc->drawXOffset;
		double yOffset = _gc->drawYOffset;
		NSBezierPath *path = _gc->path;
		NSPoint pt;
		pt.x = pointArray[0].x + xOffset;
		pt.y = pointArray[0].y + yOffset;
		NSBezierPath_moveToPoint(path, &pt);
		for (int i = 1; i < count; i++) {
			pt.x = pointArray[i].x + xOffset;
			pt.y = pointArray[i].y + yOffset;
			NSBezierPath_lineToPoint(path, &pt);
		}
		NSBezierPath_closePath(path);
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_stroke_pattern(gc, path, pattern);
		} else {
			NSBezierPath_stroke(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_polygonv(w_graphics *gc, int count, va_list args) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		if (count < 2)
			return result;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		double xOffset = _gc->drawXOffset;
		double yOffset = _gc->drawYOffset;
		NSBezierPath *path = _gc->path;
		NSPoint pt;
		pt.x = va_arg(args, int) + xOffset;
		pt.y = va_arg(args, int) + yOffset;
		NSBezierPath_moveToPoint(path, &pt);
		for (int i = 1; i < count; i++) {
			pt.x = va_arg(args, int) + xOffset;
			pt.y = va_arg(args, int) + yOffset;
			NSBezierPath_lineToPoint(path, &pt);
		}
		NSBezierPath_closePath(path);
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_stroke_pattern(gc, path, pattern);
		} else {
			NSBezierPath_stroke(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_polyline(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		if (pointArray == 0)
			return W_ERROR_NULL_ARGUMENT;
		if (count < 2)
			return result;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		double xOffset = _gc->drawXOffset;
		double yOffset = _gc->drawYOffset;
		NSBezierPath *path = _gc->path;
		NSPoint pt;
		pt.x = pointArray[0].x + xOffset;
		pt.y = pointArray[0].y + yOffset;
		NSBezierPath_moveToPoint(path, &pt);
		for (int i = 1; i < count; i++) {
			pt.x = pointArray[i].x + xOffset;
			pt.y = pointArray[i].y + yOffset;
			NSBezierPath_lineToPoint(path, &pt);
		}
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_stroke_pattern(gc, path, pattern);
		} else {
			NSBezierPath_stroke(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_polylinev(w_graphics *gc, int count, va_list args) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		if (count < 2)
			return result;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		double xOffset = _gc->drawXOffset;
		double yOffset = _gc->drawYOffset;
		NSBezierPath *path = _gc->path;
		NSPoint pt;
		pt.x = va_arg(args, int) + xOffset;
		pt.y = va_arg(args, int) + yOffset;
		NSBezierPath_moveToPoint(path, &pt);
		for (int i = 1; i < count; i++) {
			pt.x = va_arg(args, int) + xOffset;
			pt.y = va_arg(args, int) + yOffset;
			NSBezierPath_lineToPoint(path, &pt);
		}
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_stroke_pattern(gc, path, pattern);
		} else {
			NSBezierPath_stroke(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		NSRect r;
		if (rect->width >= 0) {
			r.origin.x = rect->x + _gc->drawXOffset;
			r.size.width = rect->width;
		} else {
			r.origin.x = rect->x + rect->width + _gc->drawXOffset;
			r.size.width = -rect->width;
		}
		if (rect->height >= 0) {
			r.origin.y = rect->y + _gc->drawYOffset;
			r.size.height = rect->height;
		} else {
			r.origin.y = rect->y + rect->height + _gc->drawYOffset;
			r.size.height = -rect->height;
		}
		NSBezierPath *path = _gc->path;
		NSBezierPath_appendBezierPathWithRect(path, &r);
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_stroke_pattern(gc, path, pattern);
		} else {
			NSBezierPath_stroke(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	if (arcWidth == 0 || arcHeight == 0) {
		return w_graphics_draw_rectangle(gc, rect);
	}
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		NSRect r;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		r.origin.x = rect->x + _gc->drawXOffset;
		r.origin.y = rect->y + _gc->drawYOffset;
		r.size.height = rect->height;
		r.size.width = rect->width;
		NSBezierPath *path = _gc->path;
		NSBezierPath_appendBezierPathWithRoundedRect(path, &r, arcWidth / 2.0,
				arcHeight / 2.0);
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_stroke_pattern(gc, path, pattern);
		} else {
			NSBezierPath_stroke(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_draw_text(w_graphics *gc, const char *text, size_t length,
		w_rect *rect, int flags, int enc) {
	wresult result = _w_graphics_check(gc,
	GRAPHICS_STATE_CLIPPING | GRAPHICS_STATE_TRANSFORM |
	GRAPHICS_STATE_FONT | GRAPHICS_STATE_FOREGROUND_FILL);
	if (result > 0) {
		if (text == 0)
			return result;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		NSGraphicsContext *handle = _gc->handle;
		int mode = W_TRUE;
		int textAntialias = _gc->textAntialias - 1;
		switch (textAntialias) {
		case W_DEFAULT:
			/* Printer is off by default */
			if (!NSGraphicsContext_isDrawingToScreen(handle))
				mode = W_FALSE;
			break;
		case W_OFF:
			mode = W_FALSE;
			break;
		case W_ON:
			mode = W_TRUE;
			break;
		}
		NSGraphicsContext_saveGraphicsState(handle);
		NSGraphicsContext_setShouldAntialias(handle, mode);
		if (_gc->textStorage == 0)
			_w_graphics_create_layout(gc);
		NSAttributedString *attribStr = _w_graphics_create_string(gc, text,
				length, flags, W_TRUE, enc);
		NSMutableAttributedString_setAttributedString(
				(NSMutableAttributedString*) _gc->textStorage, attribStr);
		NSObject_release(NSOBJECT(attribStr));
		NSPoint pt;
		pt.x = rect->x;
		pt.y = rect->y;
		NSRange range;
		NSLayoutManager_glyphRangeForTextContainer(_gc->layoutManager,
				_gc->textContainer, &range);
		if ((flags & W_DRAW_TRANSPARENT) == 0) {
			NSRect r;
			NSLayoutManager_usedRectForTextContainer(_gc->layoutManager,
					_gc->textContainer, &r);
			r.x = rect->x;
			r.y = rect->y;
			w_pattern *pattern = _gc->backgroundPattern;
			if (pattern != 0)
				_w_graphics_set_pattern_phase(gc, pattern);
			if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
					&& _W_PATTERN(pattern)->is_color == W_FALSE) {
				NSBezierPath *path = NSBezierPath_bezierPathWithRect(&r);
				_w_graphics_fill_pattern(gc, path, pattern);
			} else {
				NSColor *bg = _gc->bg;
				if (bg == 0) {
					w_color color = _gc->background;
					bg = _gc->bg = NSColor_colorWithDeviceRed(
					W_RED(color) / 255.0, W_GREEN(color) / 255.0,
					W_BLUE(color) / 255.0, _gc->alpha / 255.0);
					NSObject_retain(NSOBJECT(bg));
				}
				NSColor_setFill(bg);
				NSBezierPath_fillRect(&r);
			}
		}
		NSLayoutManager_drawGlyphsForGlyphRange(_gc->layoutManager, &range,
				&pt);
		NSGraphicsContext_restoreGraphicsState(handle);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_fill_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		int x = rect->x;
		int y = rect->y;
		int width = rect->width;
		int height = rect->height;
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
		if (width == 0 || height == 0 || arcAngle == 0)
			return result;
		NSGraphicsContext_saveGraphicsState(_gc->handle);
		NSAffineTransform *transform = NSAffineTransform_transform();
		CGFloat xOffset = _gc->drawXOffset;
		CGFloat yOffset = _gc->drawYOffset;
		NSAffineTransform_translateXBy(transform, x + xOffset + width / 2.0,
				y + yOffset + height / 2.0);
		NSAffineTransform_scaleXBy(transform, width / 2.0, height / 2.0);
		NSBezierPath *path = _gc->path;
		NSPoint center;
		center.x = 0;
		center.y = 0;
		CGFloat sAngle = -startAngle;
		CGFloat eAngle = -(startAngle + arcAngle);
		NSBezierPath_appendBezierPathWithArcWithCenter_0(path, &center, 1,
				sAngle, eAngle, arcAngle > 0);
		NSBezierPath_closePath(path);
		NSBezierPath_transformUsingAffineTransform(path, transform);
		w_pattern *pattern = _gc->backgroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_fill_pattern(gc, path, pattern);
		} else {
			NSBezierPath_fill(path);
		}
		NSBezierPath_removeAllPoints(path);
		NSGraphicsContext_restoreGraphicsState(_gc->handle);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_fill_gradientrectangle(w_graphics *gc, w_rect *rect,
		int vertical) {
	wresult result = _w_graphics_check(gc,
	GRAPHICS_STATE_CLIPPING | GRAPHICS_STATE_TRANSFORM);
	if (result > 0) {
		if ((rect->width == 0) || (rect->height == 0))
			return result;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		w_color backgroundRGB, foregroundRGB;
		backgroundRGB = w_graphics_get_background(gc);
		foregroundRGB = w_graphics_get_foreground(gc);

		w_color fromRGB, toRGB;
		fromRGB = foregroundRGB;
		toRGB = backgroundRGB;
		int swapColors = W_FALSE;
		w_rect r;
		memcpy(&r, rect, sizeof(r));
		if (r.width < 0) {
			r.x += r.width;
			r.width = -r.width;
			if (!vertical)
				swapColors = W_TRUE;
		}
		if (r.height < 0) {
			r.y += r.height;
			r.height = -r.height;
			if (vertical)
				swapColors = W_TRUE;
		}
		if (swapColors) {
			fromRGB = backgroundRGB;
			toRGB = foregroundRGB;
		}
		if (fromRGB == toRGB) {
			w_graphics_fill_rectangle(gc, &r);
		} else {
			NSColor *startingColor = NSColor_colorWithDeviceRed(
			W_RED(fromRGB) / 255.0, W_GREEN(fromRGB) / 255.0,
			W_BLUE(fromRGB) / 255.0, _gc->alpha / 255.0);
			NSColor *endingColor = NSColor_colorWithDeviceRed(
			W_RED(toRGB) / 255.0,
			W_GREEN(toRGB) / 255.0, W_BLUE(toRGB) / 255.0, _gc->alpha / 255.0);
			NSGradient *gradient = NSGradient_initWithStartingColor(
					startingColor, endingColor);
			NSRect _rect;
			_rect.origin.x = r.x;
			_rect.origin.y = r.y;
			_rect.size.width = r.width;
			_rect.size.height = r.height;
			NSGradient_drawInRect(gradient, &_rect, vertical ? 90 : 0);
			NSObject_release(NSOBJECT(gradient));
		}
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_fill_oval(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		NSRect r;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		if (rect->width >= 0) {
			r.origin.x = rect->x;
			r.size.width = rect->width;
		} else {
			r.origin.x = rect->x + rect->width;
			r.size.width = -rect->width;
		}
		if (rect->height >= 0) {
			r.origin.y = rect->y;
			r.size.height = rect->height;
		} else {
			r.origin.y = rect->y + rect->height;
			r.size.height = -rect->height;
		}
		NSBezierPath *path = _gc->path;
		NSBezierPath_appendBezierPathWithOvalInRect(path, &r);
		w_pattern *pattern = _gc->backgroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_fill_pattern(gc, path, pattern);
		} else {
			NSBezierPath_fill(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
void _w_graphics_fill_pattern(w_graphics *gc, NSBezierPath *path,
		w_pattern *pattern) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	NSGraphicsContext_saveGraphicsState(_gc->handle);
	NSBezierPath_addClip(path);
	_w_pattern_gradient *gradient = _W_PATTERN(pattern)->gradient;
	NSRect bounds;
	NSBezierPath_bounds(path, &bounds);
	NSPoint start;
	start.x = gradient->pt1.x;
	start.y = gradient->pt1.y;
	NSPoint end;
	end.x = gradient->pt2.x;
	end.y = gradient->pt2.y;
	CGFloat difx = end.x - start.x;
	CGFloat dify = end.y - start.y;
	if (difx == 0 && dify == 0) {
		w_color color = gradient->color1;
		NSColor *c = NSColor_colorWithDeviceRed(W_RED(color), W_GREEN(color),
				W_BLUE(color), W_ALPHA(color) / 255.);
		NSColor_setFill(c);
		NSBezierPath_fill(path);
		NSGraphicsContext_restoreGraphicsState(_gc->handle);
		return;
	}
	CGFloat startx, starty, endx, endy;
	if (difx == 0 || dify == 0) {
		startx = bounds.x;
		starty = bounds.y;
		endx = bounds.x + bounds.width;
		endy = bounds.y + bounds.height;
		if (difx < 0 || dify < 0) {
			startx = endx;
			starty = endy;
			endx = bounds.x;
			endy = bounds.y;
		}
	} else {
		CGFloat m = (end.y - start.y) / (end.x - start.x);
		CGFloat b = end.y - (m * end.x);
		CGFloat m2 = -1 / m; //perpendicular slope
		CGFloat b2 = bounds.y - (m2 * bounds.x);
		startx = endx = (b - b2) / (m2 - m);
		b2 = (bounds.y + bounds.height) - (m2 * bounds.x);
		CGFloat x2 = (b - b2) / (m2 - m);
		if (difx > 0) {
			startx = WMIN(startx, x2);
		} else {
			startx = WMAX(startx, x2);
		}
		if (difx < 0) {
			endx = WMIN(endx, x2);
		} else {
			endx = WMAX(endx, x2);
		}
		b2 = bounds.y - (m2 * (bounds.x + bounds.width));
		x2 = (b - b2) / (m2 - m);
		if (difx > 0) {
			startx = WMIN(startx, x2);
		} else {
			startx = WMAX(startx, x2);
		}
		if (difx < 0) {
			endx = WMIN(endx, x2);
		} else {
			endx = WMAX(endx, x2);
		}
		b2 = (bounds.y + bounds.height) - (m2 * (bounds.x + bounds.width));
		x2 = (b - b2) / (m2 - m);
		if (difx > 0) {
			startx = WMIN(startx, x2);
		} else {
			startx = WMAX(startx, x2);
		}
		if (difx < 0) {
			endx = WMIN(endx, x2);
		} else {
			endx = WMAX(endx, x2);
		}
		starty = (m * startx) + b;
		endy = (m * endx) + b;
	}
	if (difx != 0) {
		while ((difx > 0 && start.x >= startx)
				|| (difx < 0 && start.x <= startx)) {
			start.x -= difx;
			start.y -= dify;
		}
	} else {
		while ((dify > 0 && start.y >= starty)
				|| (dify < 0 && start.y <= starty)) {
			start.x -= difx;
			start.y -= dify;
		}
	}
	end.x = start.x;
	end.y = start.y;
	do {
		end.x += difx;
		end.y += dify;
		NSGradient_drawFromPoint(gradient->gradient, &start, &end, 0);
		start.x = end.x;
		start.y = end.y;
	} while ((difx > 0 && end.x <= endx) || (difx < 0 && end.x >= endx)
			|| (difx == 0
					&& ((dify > 0 && end.y <= endy)
							|| (dify < 0 && end.y >= endy))));
	NSGraphicsContext_restoreGraphicsState(_gc->handle);

}
wresult w_graphics_fill_path(w_graphics *gc, w_path *path) {
	return W_FALSE;
}
wresult w_graphics_fill_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		if (pointArray == 0)
			return W_ERROR_NULL_ARGUMENT;
		if (count < 2)
			return result;
		NSBezierPath *path = _gc->path;
		NSPoint pt;
		pt.x = pointArray[0].x;
		pt.y = pointArray[0].y;
		NSBezierPath_moveToPoint(path, &pt);
		for (int i = 1; i < count; i++) {
			pt.x = pointArray[i].x;
			pt.y = pointArray[i].y;
			NSBezierPath_lineToPoint(path, &pt);
		}
		NSBezierPath_closePath(path);
		w_pattern *pattern = _gc->backgroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_fill_pattern(gc, path, pattern);
		} else {
			NSBezierPath_fill(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_fill_polygonv(w_graphics *gc, int count, va_list args) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		if (count < 2)
			return result;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		NSBezierPath *path = _gc->path;
		NSPoint pt;
		pt.x = va_arg(args, int);
		pt.y = va_arg(args, int);
		NSBezierPath_moveToPoint(path, &pt);
		for (int i = 1; i < count; i++) {
			pt.x = va_arg(args, int);
			pt.y = va_arg(args, int);
			NSBezierPath_lineToPoint(path, &pt);
		}
		NSBezierPath_closePath(path);
		w_pattern *pattern = _gc->foregroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_fill_pattern(gc, path, pattern);
		} else {
			NSBezierPath_fill(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_fill_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		NSRect r;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		if (rect->width >= 0) {
			r.origin.x = rect->x;
			r.size.width = rect->width;
		} else {
			r.origin.x = rect->x + rect->width;
			r.size.width = -rect->width;
		}
		if (rect->height >= 0) {
			r.origin.y = rect->y;
			r.size.height = rect->height;
		} else {
			r.origin.y = rect->y + rect->height;
			r.size.height = -rect->height;
		}
		NSBezierPath *path = _gc->path;
		NSBezierPath_appendBezierPathWithRect(path, &r);
		w_pattern *pattern = _gc->backgroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_fill_pattern(gc, path, pattern);
		} else {
			NSBezierPath_fill(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
	}
	return result;
}
wresult w_graphics_fill_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	if (arcWidth == 0 || arcHeight == 0) {
		return w_graphics_fill_rectangle(gc, rect);
	}
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		NSRect r;
		_w_graphics *_gc = _W_GRAPHICS(gc);
		r.origin.x = rect->x;
		r.origin.y = rect->y;
		r.size.height = rect->height;
		r.size.width = rect->width;
		NSBezierPath *path = _gc->path;
		NSBezierPath_appendBezierPathWithRoundedRect(path, &r, arcWidth / 2.0,
				arcHeight / 2.0);
		w_pattern *pattern = _gc->backgroundPattern;
		if (pattern != 0)
			_w_graphics_set_pattern_phase(gc, pattern);
		if (pattern != 0 && _W_PATTERN(pattern)->gradient != 0
				&& _W_PATTERN(pattern)->is_color == W_FALSE) {
			_w_graphics_fill_pattern(gc, path, pattern);
		} else {
			NSBezierPath_fill(path);
		}
		NSBezierPath_removeAllPoints(path);
		_w_graphics_uncheck(gc);
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
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _gc->alpha;
	}
	return 0xFF;
}
int w_graphics_get_antialias(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _gc->antialias - 1;
	}
	return W_DEFAULT;
}
w_color w_graphics_get_background(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return 0;
	return _gc->background;
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
wresult w_graphics_get_clipping_rect(w_graphics *gc, w_rect *rect) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
	}
	return result;
}
wresult w_graphics_get_clipping_region(w_graphics *gc, w_region *region) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
	}
	return result;
}
int w_graphics_get_fill_rule(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _gc->fillRule;
	}
	return result;
}
w_font* w_graphics_get_font(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _gc->font;
	}
	return 0;
}
wresult w_graphics_get_font_metrics(w_graphics *gc,
		w_fontmetrics *fontMetrics) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FONT);
	if (result > 0) {
		if (_gc->textStorage == 0)
			_w_graphics_create_layout(gc);
		w_font *font = _gc->font;
		if (font == 0)
			return W_ERROR_NO_HANDLES;
		_w_font_metrics *m = (_w_font_metrics*) fontMetrics;
		_w_font *desc = _W_FONT(font);
		if (desc->isMetrics == 0) {
			NSRange range;
			NSRect rect;
			const char *s =
					"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
			NSMutableDictionary *dict = NSMutableDictionary_initWithCapacity(3);
			NSMutableDictionary_setObject(dict,
					(NSObject*) _W_FONT(font)->handle,
					(NSObject*) _NSFontAttributeName);
			_w_font_add_traits_1(font, dict);
			NSString *str = NSString_stringWithUTF8String(s);
			NSAttributedString *attribStr = NSAttributedString_initWithString_0(
					str, (NSDictionary*) dict);
			NSMutableAttributedString_setAttributedString(
					(NSMutableAttributedString*) _gc->textStorage, attribStr);
			NSObject_release(NSOBJECT(attribStr));
			NSObject_release(NSOBJECT(dict));
			NSLayoutManager *layoutManager = _gc->layoutManager;
			NSLayoutManager_glyphRangeForTextContainer(layoutManager,
					_gc->textContainer, &range);
			NSLayoutManager_usedRectForTextContainer(layoutManager,
					_gc->textContainer, &rect);
			int avgWidth = ceil(rect.width) / strlen(s);
			int ascent = NSLayoutManager_defaultBaselineOffsetForFont(
					layoutManager, _W_FONT(font)->handle);
			int height = NSLayoutManager_defaultLineHeightForFont(layoutManager,
			_W_FONT(font)->handle);
			m->ascent = ascent;
			m->descent = height - ascent;
			m->averageCharWidth = avgWidth;
			m->leading = 0;
			m->height = height;
		} else {
			m->ascent = desc->ascent;
			m->descent = desc->height - desc->ascent;
			m->averageCharWidth = desc->avgWidth;
			m->leading = 0;
			m->height = desc->height;
		}
		_w_graphics_uncheck(gc);
	}
	return result;
}
w_color w_graphics_get_foreground(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return 0;
	return _gc->foreground;
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
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		int interpolation = NSGraphicsContext_imageInterpolation(_gc->handle);
		switch (interpolation) {
		case NSImageInterpolationDefault:
			return W_DEFAULT;
		case NSImageInterpolationNone:
			return W_NONE;
		case NSImageInterpolationLow:
			return W_LOW;
		case NSImageInterpolationHigh:
			return W_HIGH;
		}
	}
	return W_DEFAULT;
}
w_line_attributes* w_graphics_get_line_attributes(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return 0;
	return &_gc->line;
}
int w_graphics_get_line_cap(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return 0;
	return _gc->line.cap;
}
wresult w_graphics_get_line_dash(w_graphics *gc, int **dashes, int *length) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return W_ERROR_NO_HANDLES;
	*dashes = _gc->line.dashes;
	*length = _gc->line.length;
	return W_TRUE;
}
int w_graphics_get_line_join(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return 0;
	return _gc->line.join;
}
int w_graphics_get_line_style(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return 0;
	return _gc->line.style;
}
float w_graphics_get_line_width(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return 0;
	return _gc->line.width;
}
int w_graphics_get_style(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return 0;
	}
	return result;
}
int w_graphics_get_text_antialias(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _gc->textAntialias - 1;
	}
	return W_DEFAULT;
}
wresult w_graphics_get_transform(w_graphics *gc, w_transform *transform) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (transform == 0)
			return W_ERROR_NULL_ARGUMENT;
		if (w_transform_is_ok(transform) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		NSAffineTransform *cmt = _gc->transform;
		if (cmt != 0) {
			NSAffineTransformStruct st;
			NSAffineTransform_transformStruct(cmt, &st);
			NSAffineTransform_setTransformStruct(
			_W_TRANSFORM(transform)->handle, &st);
		} else {
			w_transformmatrix matrix = { 1, 0, 0, 1, 0, 0 };
			w_transform_set_elements(transform, &matrix);
		}
	}
	return result;
}
wresult w_graphics_get_xor_mode(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _gc->xorMode;
	}
	return result;
}
wresult w_graphics_is_clipped(w_graphics *gc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		return _gc->clipPath != 0;
	}
	return result;
}
wresult w_graphics_set_advanced(w_graphics *gc, int advanced) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
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
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		int mode = W_TRUE;
		switch (antialias) {
		case W_DEFAULT:
			/* Printer is off by default */

			if (!NSGraphicsContext_isDrawingToScreen(_gc->handle))
				mode = W_FALSE;
			break;
		case W_OFF:
			mode = W_FALSE;
			break;
		case W_ON:
			mode = W_TRUE;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		_gc->antialias = antialias + 1;
		NSGraphicsContext_setShouldAntialias(_gc->handle, mode);
	}
	return result;
}
wresult w_graphics_set_alpha(w_graphics *gc, int alpha) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		_gc->alpha = alpha & 0xFF;
		_gc->state &= ~(GRAPHICS_STATE_BACKGROUND | GRAPHICS_STATE_FOREGROUND
				| GRAPHICS_STATE_FOREGROUND_FILL);
	}
	return result;
}
wresult w_graphics_set_background(w_graphics *gc, w_color color) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		_gc->background = color;
		_gc->backgroundPattern = 0;
		if (_gc->bg != 0) {
			NSObject_release(NSOBJECT(_gc->bg));
		}
		_gc->bg = 0;
		_gc->state &= ~GRAPHICS_STATE_BACKGROUND;
	}
	return result;
}
wresult w_graphics_set_background_pattern(w_graphics *gc, w_pattern *pattern) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (pattern != 0 && w_pattern_is_ok(pattern) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (_gc->backgroundPattern == pattern)
			return result;
		_gc->backgroundPattern = pattern;
		_gc->state &= ~GRAPHICS_STATE_BACKGROUND;
	}
	return result;
}
void w_graphics_set_clipping_path_0(w_graphics *gc, NSBezierPath *path) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->clipPath != 0) {
		NSObject_release(NSOBJECT(_gc->clipPath));
		_gc->clipPath = 0;
	}
	if (path != 0) {
		_gc->clipPath = path;
		if (_gc->transform != 0) {
			NSBezierPath_transformUsingAffineTransform(path, _gc->transform);
		}
	}
	_gc->state &= ~GRAPHICS_STATE_CLIPPING;
}
wresult w_graphics_set_clipping_path(w_graphics *gc, w_path *path) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (path != 0 && _W_PATH(path)->handle == 0)
			return W_ERROR_INVALID_ARGUMENT;
		NSBezierPath *copy = 0;
		if (path != 0) {
			NSObject_copy((NSObject*) _W_PATH(path)->handle);
		}
		w_graphics_set_clipping_path_0(gc, copy);
	}
	return result;
}
wresult w_graphics_set_clipping_rect(w_graphics *gc, w_rect *_rect) {

	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		NSBezierPath *path = 0;
		if (_rect != 0) {
			NSRect rect;
			rect.x = _rect->x;
			rect.y = _rect->y;
			rect.width = _rect->width;
			rect.height = _rect->height;
			if (rect.width < 0) {
				rect.x = rect.x + rect.width;
				rect.width = -rect.width;
			}
			if (rect.height < 0) {
				rect.y = rect.y + rect.height;
				rect.height = -rect.height;
			}
			NSBezierPath *path = NSBezierPath_bezierPathWithRect(&rect);
			NSObject_retain(NSOBJECT(path));
		}
		w_graphics_set_clipping_path_0(gc, path);
	}
	return result;
}
wresult w_graphics_set_clipping_region(w_graphics *gc, w_region *region) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (region != 0 && _W_REGION(region)->handle == 0)
			return W_ERROR_INVALID_ARGUMENT;
		NSBezierPath *copy = 0;
		if (region != 0) {
			NSObject_copy((NSObject*) _W_REGION(region)->handle);
		}
		w_graphics_set_clipping_path_0(gc, copy);
	}
	return result;
}
wresult w_graphics_set_fill_rule(w_graphics *gc, int rule) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		switch (rule) {
		case W_FILL_WINDING:
		case W_FILL_EVEN_ODD:
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		_gc->fillRule = rule;
		NSBezierPath_setWindingRule(_gc->path,
				rule == W_FILL_WINDING ?
						NSNonZeroWindingRule : NSEvenOddWindingRule);
	}
	return result;
}
wresult w_graphics_set_font(w_graphics *gc, w_font *font) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (font != 0 && w_font_is_ok(font) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (font != 0) {
			_gc->font = font;
		} else {
			_gc->font = w_toolkit_get_system_font(0);
		}
		_gc->state &= ~GRAPHICS_STATE_FONT;
	}
	return result;
}
wresult w_graphics_set_foreground(w_graphics *gc, w_color color) {
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		_gc->foreground = color;
		_gc->foregroundPattern = 0;
		if (_gc->fg != 0) {
			NSObject_release(NSOBJECT(_gc->fg));
		}
		_gc->fg = 0;
		_gc->state &= ~(GRAPHICS_STATE_FOREGROUND
				| GRAPHICS_STATE_FOREGROUND_FILL);
	}
	return result;
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
		_gc->state &= ~(GRAPHICS_STATE_FOREGROUND
				| GRAPHICS_STATE_FOREGROUND_FILL);
	}
	return result;
}
wresult w_graphics_set_interpolation(w_graphics *gc, int interpolation) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		int quality = 0;
		switch (interpolation) {
		case W_DEFAULT:
			quality = NSImageInterpolationDefault;
			break;
		case W_NONE:
			quality = NSImageInterpolationNone;
			break;
		case W_LOW:
			quality = NSImageInterpolationLow;
			break;
		case W_HIGH:
			quality = NSImageInterpolationHigh;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		NSGraphicsContext_setImageInterpolation(_gc->handle, quality);
	}
	return result;
}
wresult w_graphics_set_line_attributes(w_graphics *gc,
		w_line_attributes *attributes) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	if (attributes == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_line_attributes *line = &_gc->line;
	int mask = 0;
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
			if (dash <= 0) {
//W_ERROR_INVALID_ARGUMENT;
				dash = 0;
			}
			if (!changed && lineDashes[i] != dash)
				changed = W_TRUE;
		}
		if (changed) {
			int *newDashes = realloc(line->dashes,
					attributes->length * sizeof(int));
			if (newDashes == 0)
				return W_ERROR_NO_MEMORY;
			memcpy(newDashes, dashes, attributes->length * sizeof(int));
			line->dashes = newDashes;
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
	if (_gc->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	w_line_attributes *line = &_gc->line;
	if (line->cap != cap) {
		switch (cap) {
		case W_CAP_ROUND:
		case W_CAP_FLAT:
		case W_CAP_SQUARE:
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		line->cap = cap;
		_gc->state &= ~GRAPHICS_STATE_LINE_CAP;
	}
	return W_TRUE;
}
wresult w_graphics_set_line_dash(w_graphics *gc, int *dashes, int length) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	w_line_attributes *line = &_gc->line;
	int *lineDashes = line->dashes;
	if (dashes != 0 && line->length > 0) {
		int changed = line->style != W_LINE_CUSTOM || lineDashes == 0
				|| line->length != length;
		for (int i = 0; i < length; i++) {
			int dash = dashes[i];
			if (dash <= 0) {
//W_ERROR_INVALID_ARGUMENT;
				dash = 0;
			}
			if (!changed && lineDashes[i] != dash)
				changed = W_TRUE;
		}
		if (!changed)
			return W_TRUE;
		line->dashes = realloc(line->dashes, length * sizeof(int));
		if (line->dashes == 0)
			return W_ERROR_NO_MEMORY;
		lineDashes = line->dashes;
		for (int i = 0; i < length; i++) {
			lineDashes[i] = dashes[i];
		}
		line->style = W_LINE_CUSTOM;
	} else {
		if (line->style == W_LINE_SOLID
				&& (lineDashes == 0 || line->length == 0))
			return W_TRUE;
		line->dashes = 0;
		line->style = W_LINE_SOLID;
	}
	_gc->state &= ~GRAPHICS_STATE_LINE_STYLE;
	return W_TRUE;
}
wresult w_graphics_set_line_join(w_graphics *gc, int join) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	w_line_attributes *line = &_gc->line;
	if (line->join != join) {
		switch (join) {
		case W_JOIN_MITER:
		case W_JOIN_ROUND:
		case W_JOIN_BEVEL:
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		line->join = join;
		_gc->state &= ~GRAPHICS_STATE_LINE_JOIN;
	}
	return W_TRUE;
}
wresult w_graphics_set_line_style(w_graphics *gc, int lineStyle) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	w_line_attributes *line = &_gc->line;
	if (line->style != lineStyle) {
		switch (lineStyle) {
		case W_LINE_SOLID:
		case W_LINE_DASH:
		case W_LINE_DOT:
		case W_LINE_DASHDOT:
		case W_LINE_DASHDOTDOT:
			break;
		case W_LINE_CUSTOM:
			if (line->dashes == 0)
				lineStyle = W_LINE_SOLID;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		line->style = lineStyle;
		_gc->state &= ~GRAPHICS_STATE_LINE_STYLE;
	}
	return W_TRUE;
}
wresult w_graphics_set_line_width(w_graphics *gc, float lineWidth) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_gc->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	w_line_attributes *line = &_gc->line;
	if (line->width != lineWidth) {
		line->width = lineWidth;
		_gc->state &= ~(GRAPHICS_STATE_LINE_WIDTH | GRAPHICS_STATE_DRAW_OFFSET);
	}
	return W_TRUE;
}
void _w_graphics_set_pattern_phase(w_graphics *gc, w_pattern *pattern) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	if (_W_PATTERN(pattern)->is_color == W_FALSE)
		return;
	NSPoint phase;
	NSRect bounds;
	phase.x = 0;
	phase.y = 0;
	if (_gc->image != 0) {
		NSImageRep *imageRep = (NSImageRep*) _gc->image;
		NSInteger h = NSImageRep_pixelsHigh((NSImageRep*) imageRep);
		phase.y += h - _W_PATTERN(pattern)->height;
	} else if (_gc->view != 0) {
		NSView *view = _gc->view;
		if (!NSView_isFlipped(view)) {
			NSView_bounds(view, &bounds);
			phase.y = bounds.height;
		}
		NSWindow *window = NSView_window(view);
		NSView *contentView = NSWindow_contentView(window);
		NSView_convertPoint_toView_(view, &phase, &phase, contentView);
		NSView_bounds(contentView, &bounds);
		phase.y = bounds.height - phase.y;
	} else if (_gc->issize != 0) {
		phase.y += _gc->size.height - _W_PATTERN(pattern)->height;
	}
	NSGraphicsContext_setPatternPhase(_gc->handle, &phase);
}
wresult w_graphics_set_xor_mode(w_graphics *gc, int _xor) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		_gc->xorMode = _xor;
	}
	return result;
}
wresult w_graphics_set_text_antialias(w_graphics *gc, int antialias) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		switch (antialias) {
		case W_DEFAULT:
		case W_OFF:
		case W_ON:
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		_gc->textAntialias = antialias;
	}
	return result;
}
wresult w_graphics_set_transform(w_graphics *gc, w_transform *transform) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, 0);
	if (result > 0) {
		if (transform != 0 && w_transform_is_ok(transform) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (transform != 0) {
			if (_gc->transform != 0)
				NSObject_release((NSObject*) _gc->transform);
			if (_gc->inverseTransform != 0)
				NSObject_release((NSObject*) _gc->inverseTransform);
			_gc->transform = NSAffineTransform_initWithTransform(
			_W_TRANSFORM(transform)->handle);
			_gc->inverseTransform = NSAffineTransform_initWithTransform(
			_W_TRANSFORM(transform)->handle);
			NSAffineTransformStruct st;
			NSAffineTransform_transformStruct(_gc->inverseTransform, &st);
			if ((st.m11 * st.m22 - st.m12 * st.m21) != 0) {
				NSAffineTransform_invert(_gc->inverseTransform);
			}
		} else {
			_gc->transform = _gc->inverseTransform = 0;
		}
		_gc->state &= ~(GRAPHICS_STATE_TRANSFORM | GRAPHICS_STATE_DRAW_OFFSET);
	}
	return result;
}
void _w_graphics_stroke_pattern(w_graphics *gc, NSBezierPath *path,
		w_pattern *pattern) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
}
wresult w_graphics_text_extent(w_graphics *gc, const char *string, int length,
		w_size *size, int flags, int enc) {
	_w_graphics *_gc = _W_GRAPHICS(gc);
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FONT);
	if (result > 0) {
		NSRange range;
		NSRect rect;
		if (_gc->textStorage == 0)
			_w_graphics_create_layout(gc);
		NSAttributedString *attribStr = _w_graphics_create_string(gc, string,
				length, flags, W_FALSE, enc);
		NSMutableAttributedString_setAttributedString(
				(NSMutableAttributedString*) _gc->textStorage, attribStr);
		NSLayoutManager_glyphRangeForTextContainer(_gc->layoutManager,
				_gc->textContainer, &range);
		NSLayoutManager_usedRectForTextContainer(_gc->layoutManager,
				_gc->textContainer, &rect);
		size->width = ceil(rect.width);
		size->height = ceil(rect.height);
		NSObject_release(NSOBJECT(attribStr));
		_w_graphics_uncheck(gc);
	}
	return result;
}
