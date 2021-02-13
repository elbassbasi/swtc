/*
 * Name:        pattern.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
wresult w_pattern_init(w_pattern *pattren) {
	_W_PATTERN(pattren)->handle = 0;
	return W_TRUE;
}
wresult w_pattern_dispose(w_pattern *pattren) {
	if (pattren != 0 && _W_PATTERN(pattren)->handle != 0) {
		cairo_pattern_destroy(_W_PATTERN(pattren)->handle);
		_W_PATTERN(pattren)->handle = 0;
	}
	return W_TRUE;
}
wresult w_pattern_is_ok(w_pattern *pattren) {
	return pattren != 0 && _W_PATTERN(pattren)->handle != 0;
}
wresult w_pattern_create_from_image(w_pattern *pattren, w_image *image) {
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_image_is_ok(image))
		return W_ERROR_INVALID_ARGUMENT;
	GdkPixbuf *pixbuf = _W_IMAGE(image)->pixbuf;
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			gdk_pixbuf_get_width(pixbuf), gdk_pixbuf_get_height(pixbuf));
	if (surface == 0)
		return W_ERROR_NO_HANDLES;
	cairo_t *cairo = cairo_create(surface);
	if (cairo == 0) {
		cairo_surface_destroy(surface);
		return W_ERROR_NO_HANDLES;
	}
	gdk_cairo_set_source_pixbuf(cairo, pixbuf, 0, 0);
	cairo_paint(cairo);
	_W_PATTERN(pattren)->handle = cairo_pattern_create_for_surface(surface);
	cairo_destroy(cairo);
	cairo_surface_destroy(surface);
	if (_W_PATTERN(pattren)->handle == 0)
		return W_ERROR_NO_HANDLES;
	cairo_pattern_set_extend(_W_PATTERN(pattren)->handle, CAIRO_EXTEND_REPEAT);
	return W_TRUE;
}
wresult w_pattern_create(w_pattern *pattren, float x1, float y1, float x2,
		float y2, w_color color1, w_color color2) {
	_W_PATTERN(pattren)->handle = cairo_pattern_create_linear(x1, y1, x2, y2);
	if (_W_PATTERN(pattren)->handle == 0)
		return W_ERROR_NO_HANDLES;
	cairo_pattern_add_color_stop_rgba(_W_PATTERN(pattren)->handle, 0,
			(double) W_RED(color1) / (double) 0xFFFF,
			(double) W_GREEN(color1) / (double) 0xFFFF,
			(double) W_BLUE(color1) / (double) 0xFFFF,
			(double) W_ALPHA(color1) / (double) 0xFFFF);
	cairo_pattern_add_color_stop_rgba(_W_PATTERN(pattren)->handle, 1,
			(double) W_RED(color2) / (double) 0xFFFF,
			(double) W_GREEN(color2) / (double) 0xFFFF,
			(double) W_BLUE(color2) / (double) 0xFFFF,
			(double) W_ALPHA(color2) / (double) 0xFFFF);
	cairo_pattern_set_extend(_W_PATTERN(pattren)->handle, CAIRO_EXTEND_REPEAT);
	return W_TRUE;
}
