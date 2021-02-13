/*
 * Name:        pattern.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
#include "gdip.h"
wresult w_pattern_init(w_pattern *pattern) {
	_W_PATTERN(pattern)->handle = 0;
	return W_TRUE;
}
wresult w_pattern_dispose(w_pattern *pattern) {
	if (_W_PATTERN(pattern)->handle != 0) {
		GdipDeleteBrush(_W_PATTERN(pattern)->handle);
		_W_PATTERN(pattern)->handle = 0;
	}
	return W_TRUE;
}
wresult w_pattern_is_ok(w_pattern *pattern) {
	return _W_PATTERN(pattern)->handle != 0;
}
wresult w_pattern_create_from_image(w_pattern *pattern, w_image *image) {
	w_pattern_dispose(pattern);
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (w_image_is_ok(image) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	GpImage *gdipImage;
	GpBrush *handle = 0;
	_w_image_get_gpimage(image, &gdipImage);
	if (gdipImage != 0) {
		UINT width;
		UINT height;
		GdipGetImageWidth(gdipImage, &width);
		GdipGetImageHeight(gdipImage, &height);
		GdipCreateTexture2(gdipImage, WrapModeTile, 0, 0, width, height,
				&handle);
		_w_image_dispose_gpimage(image, gdipImage);
	}
	_W_PATTERN(pattern)->handle = handle;
	if (handle == 0)
		return W_ERROR_NO_HANDLES;
	return W_TRUE;
}
wresult w_pattern_create(w_pattern *pattern, float x1, float y1, float x2,
		float y2, w_color color1, w_color color2) {
	GpBrush *handle = 0;
	ARGB foreColor = color1;
	if (x1 == x2 && y1 == y2) {
		GdipCreateSolidFill(foreColor, &handle);
	} else {
		ARGB backColor = color2;
		GpPointF p1;
		p1.X = x1;
		p1.Y = y1;
		GpPointF p2;
		p2.X = x2;
		p2.Y = y2;
		GdipCreateLineBrush(&p1, &p2, foreColor, backColor, WrapModeTile,
				&handle);
		if (handle == 0)
			return W_ERROR_NO_HANDLES;
	}
	_W_PATTERN(pattern)->handle = handle;
	if (handle == 0)
		return W_ERROR_NO_HANDLES;
	return W_TRUE;
}
