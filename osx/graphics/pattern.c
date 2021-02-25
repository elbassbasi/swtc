/*
 * Name:        pattern.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
wresult w_pattern_init(w_pattern *pattern) {
	_W_PATTERN(pattern)->color = 0;
	return W_TRUE;
}
wresult w_pattern_dispose(w_pattern *pattern) {
	if (_W_PATTERN(pattern)->color != 0) {
		if (_W_PATTERN(pattern)->is_color) {
			NSObject_release(NSOBJECT(_W_PATTERN(pattern)->color));
		} else {
			_w_pattern_gradient *gradient = _W_PATTERN(pattern)->gradient;
			NSObject_release(NSOBJECT(gradient->gradient));
			free(gradient);
		}
		_W_PATTERN(pattern)->color = 0;
	}
	return W_TRUE;
}
wresult w_pattern_is_ok(w_pattern *pattern) {
	return _W_PATTERN(pattern)->color != 0;
}
wresult w_pattern_create_from_image(w_pattern *pattern, w_image *image) {
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_IMAGE(image)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	_W_PATTERN(pattern)->color = NSColor_colorWithPatternImage(
	_W_IMAGE(image)->handle);
	if (_W_PATTERN(pattern)->color == 0)
		return W_ERROR_NO_HANDLES;
	NSSize size;
	NSImage_size(_W_IMAGE(image)->handle, &size);
	_W_PATTERN(pattern)->is_color = W_TRUE;
	_W_PATTERN(pattern)->height = size.height;
	NSObject_retain(NSOBJECT(_W_PATTERN(pattern)->color));
	return W_TRUE;
}
wresult w_pattern_create(w_pattern *pattern, float x1, float y1, float x2,
		float y2, w_color color1, w_color color2) {
	_w_pattern_gradient *gradient = malloc(sizeof(_w_pattern_gradient));
	if (gradient == 0)
		return W_ERROR_NO_MEMORY;
	NSPoint pt1, pt2;
	pt1.x = x1;
	pt1.y = y1;
	pt2.x = x2;
	pt2.y = y2;
	gradient->color1 = color1;
	gradient->color2 = color2;
	NSColor *start = NSColor_colorWithDeviceRed(W_RED(color1), W_GREEN(color1),
			W_BLUE(color1), W_ALPHA(color1) / 255.0);
	NSColor *end = NSColor_colorWithDeviceRed(W_RED(color2), W_GREEN(color2),
			W_BLUE(color2), W_ALPHA(color2) / 255.0);
	gradient->gradient = NSGradient_initWithStartingColor(start, end);
	if (gradient->gradient == 0) {
		free(gradient);
		return W_ERROR_NO_HANDLES;
	}
	_W_PATTERN(pattern)->gradient = gradient;
	_W_PATTERN(pattern)->is_color = W_FALSE;
	return W_TRUE;
}
