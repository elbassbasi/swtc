/*
 * Name:        image.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
#include "../widgets/toolkit.h"
/*
 * image
 */
void w_image_init(w_image *image) {
	_W_IMAGE(image)->handle = 0;
}
void w_image_dispose(w_image *image) {
	if (_W_IMAGE(image)->handle != 0) {
		NSObject_release(NSOBJECT(_W_IMAGE(image)->handle));
		_W_IMAGE(image)->handle = 0;
	}
}
wresult w_image_is_ok(w_image *image) {
	return _W_IMAGE(image)->handle != 0;
}
wresult w_image_create_from_file(w_image *image, const char *file, int length,
		int enc) {
	w_image_dispose(image);
	NSImage *nativeImage = 0;
	NSString *str = NSString_new(file, length, enc);
	nativeImage = NSImage_initWithContentsOfFile(str);
	//NSObject_release(NSOBJECT(str));
	if (nativeImage == 0) {
		return W_ERROR_NO_HANDLES;
	}
	_W_IMAGE(image)->handle = nativeImage;
	return W_TRUE;
}
wresult w_image_create_from_data(w_image *image, void *data, wuint format,
		w_size *size, int rowstride) {
	w_image_dispose(image);
	return W_FALSE;
}
wresult w_image_create_from_stream(w_image *image, w_stream *stream) {
	return W_FALSE;
}
int w_image_get_rowstride(wuint format, wuint width) {
	switch (format) {
	case W_IMAGE_ARGB32:
		return width * 4;
		break;
	case W_IMAGE_RGB24:
		return width * 3;
		break;
	case W_IMAGE_A8:
		return width;
		break;
	case W_IMAGE_A1:
		return width / 8;
		break;
	default:
		return width * 4;
		break;
	}
}
wresult w_image_create_from_buffer(w_image *image, const void *buffer,
		size_t size) {
	return W_FALSE;
}
wresult w_image_get_size(w_image *image, w_size *size) {
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_IMAGE(image)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSSize sz;
	NSImage_size(_W_IMAGE(image)->handle, &sz);
	size->width = sz.width;
	size->height = sz.height;
	return W_TRUE;
}
wresult w_image_get_pixels(w_image *image, w_color *colors, size_t length) {
	return W_FALSE;
}
wresult w_image_save_to_file(w_image *image, const char *file, int length,
		int enc, const char *type) {
	return W_FALSE;
}
wresult w_image_save_to_buffer(w_image *image, w_alloc alloc, void *user_data,
		const char *type) {
	return W_FALSE;
}
wresult w_image_save_to_stream(w_image *image, w_stream *stream,
		const char *type) {
	return W_FALSE;
}
wresult w_image_copy(w_image *image, w_rect *rect, w_image *destimage) {
	return W_FALSE;
}
wresult w_image_resize(w_image *image, w_size *size, w_image *destimage) {
	return W_FALSE;
}
wresult w_image_to_greyscale(w_image *image, w_image *destimage) {
	return W_FALSE;
}
wresult w_image_to_disabled(w_image *image, w_color background, w_color shadow,
		w_image *destimage) {
	return W_FALSE;
}
/*
 * surface
 */
void w_surface_init(w_surface *surface) {
	_W_SURFACE(surface)->handle = 0;
}
void w_surface_dispose(w_surface *surface) {
	if (_W_SURFACE(surface)->handle != 0) {
		NSObject_release(NSOBJECT(_W_SURFACE(surface)->handle));
		_W_SURFACE(surface)->handle = 0;
	}
}
wresult w_surface_is_ok(w_surface *surface) {
	return _W_SURFACE(surface)->handle != 0;
}
wresult w_surface_create(w_surface *surface, w_size *size) {
	NSBitmapImageRep *rep = NSBitmapImageRep_initWithBitmapDataPlanes_0(0,
			size->width, size->height, 8, 3, W_FALSE, W_FALSE,
			_NSDeviceRGBColorSpace(),
			NSAlphaFirstBitmapFormat | NSAlphaNonpremultipliedBitmapFormat,
			size->width * 4, 32);
	if (rep == 0)
		return W_ERROR_NO_HANDLES;
	_W_SURFACE(surface)->handle = rep;
	unsigned char *data = NSBitmapImageRep_bitmapData(rep);
	memset(data, 0xFF, size->width * size->height * 4);
	return W_TRUE;
}
wresult w_surface_get_image(w_surface *surface, w_image *destimage) {
	NSBitmapImageRep *imageRep = _W_SURFACE(surface)->handle;
	NSSize size;
	size.width = NSImageRep_pixelsWide((NSImageRep*) imageRep);
	size.height = NSImageRep_pixelsHigh((NSImageRep*) imageRep);
	_W_IMAGE(destimage)->handle = NSImage_initWithSize(&size);
	NSImage_addRepresentation(_W_IMAGE(destimage)->handle, imageRep);
	return W_TRUE;
}
wresult w_surface_get_graphics(w_surface *surface, w_graphics *gc) {
	NSBitmapImageRep *imageRep = _W_SURFACE(surface)->handle;

	// Can't perform transforms on image reps with alpha.
	NSImageRep_setAlpha((NSImageRep*) imageRep, W_FALSE);

	NSGraphicsContext *context =
			NSGraphicsContext_graphicsContextWithBitmapImageRep(imageRep);
	NSGraphicsContext *flippedContext =
			NSGraphicsContext_graphicsContextWithGraphicsPort(
					NSGraphicsContext_graphicsPort(context), W_TRUE);
	context = flippedContext;
	NSObject_retain(NSOBJECT(context));
	_w_graphics_init(gc, context, 0);
	_W_GRAPHICS(gc)->flippedContext = flippedContext;
	_W_GRAPHICS(gc)->handle = context;
	NSGraphicsContext_static_saveGraphicsState();
	NSGraphicsContext_setCurrentContext(context);
	NSAffineTransform *transform = NSAffineTransform_transform();
	NSInteger w = NSImageRep_pixelsWide((NSImageRep*) imageRep);
	NSInteger h = NSImageRep_pixelsHigh((NSImageRep*) imageRep);
	NSAffineTransform_translateXBy(transform, 0, h);
	NSAffineTransform_scaleXBy(transform, 1, -1);
	NSAffineTransform_set(transform);
	NSGraphicsContext_static_restoreGraphicsState();
	int mask = W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT;
	if ((_W_GRAPHICS(gc)->style & mask) == 0) {
		_W_GRAPHICS(gc)->style |= W_LEFT_TO_RIGHT;
	}
	_W_GRAPHICS(gc)->background = W_COLOR_WHITE;
	_W_GRAPHICS(gc)->foreground = W_COLOR_BLACK;
	_W_GRAPHICS(gc)->font = w_toolkit_get_system_font(W_TOOLKIT(mac_toolkit));
	return W_TRUE;
}
