/*
 * Name:        image.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
/*
 * image
 */
void w_image_init(w_image *image) {
	_W_IMAGE(image)->pixbuf = 0;
}
void w_image_dispose(w_image *image) {
	if (image != 0 && _W_IMAGE(image)->pixbuf != 0) {
		if (!(_W_IMAGE(image)->flags & IMAGE_REF)) {
			g_object_unref(_W_IMAGE(image)->pixbuf);
		}
		_W_IMAGE(image)->pixbuf = 0;
	}
}
wresult w_image_is_ok(w_image *image) {
	return _W_IMAGE(image)->pixbuf != 0;
}
wresult w_image_create_from_file(w_image *image, const char *file, int length,
		int enc) {
	GError *err = NULL;
	_W_IMAGE(image)->pixbuf = gdk_pixbuf_new_from_file(file, &err);
	if (_W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_NO_HANDLES;
	}
	_W_IMAGE(image)->flags = 0;
	return W_TRUE;
}
cairo_surface_t* _w_image_create_surface(GdkPixbuf *pixbuf) {
	int w = gdk_pixbuf_get_width(pixbuf);
	int h = gdk_pixbuf_get_height(pixbuf);
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			w, h);
	if (surface != 0) {
		cairo_t *cr = cairo_create(surface);
		if (cr == 0) {
			cairo_surface_destroy(surface);
			surface = 0;
		} else {
			gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
			cairo_paint(cr);
			cairo_destroy(cr);
		}
	}
	return surface;
}
cairo_pattern_t* _w_image_create_pattern(GdkPixbuf *pixbuf,
		cairo_surface_t **surface) {
	cairo_pattern_t *pattern = 0;
	*surface = _w_image_create_surface(pixbuf);
	if (surface != 0) {
		cairo_pattern_t *pattern = cairo_pattern_create_for_surface(*surface);
		if (pattern == 0) {
			cairo_surface_destroy(*surface);
			*surface = 0;
		}
	}
	return pattern;
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
wresult w_image_create_from_data(w_image *image, void *data, wuint format,
		w_size *size, int rowstride) {
	w_image_dispose(image);
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	int has_alpha;
	int bits_per_sample;
	int default_rowstride;
	switch (format) {
	case W_IMAGE_ARGB32:
		default_rowstride = size->width * 4;
		bits_per_sample = 32;
		has_alpha = TRUE;
		break;
	case W_IMAGE_RGB24:
		default_rowstride = size->width * 3;
		bits_per_sample = 24;
		has_alpha = FALSE;
		break;
	case W_IMAGE_A8:
		default_rowstride = size->width;
		bits_per_sample = 8;
		has_alpha = FALSE;
		break;
	case W_IMAGE_A1:
		default_rowstride = size->width / 8;
		bits_per_sample = 1;
		has_alpha = FALSE;
		break;
	default:
		default_rowstride = size->width * 4;
		bits_per_sample = 32;
		has_alpha = TRUE;
		break;
	}
	//rowstride = (width * channels + 3) & ~3;
	if (rowstride <= 0) {
		rowstride = default_rowstride;
	}
	_W_IMAGE(image)->pixbuf = gdk_pixbuf_new_from_data((guchar*) data,
			GDK_COLORSPACE_RGB, has_alpha, bits_per_sample, size->width,
			size->height, rowstride, 0, 0);
	if (_W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_NO_HANDLES;
	}
	_W_IMAGE(image)->flags = 0;
	return W_TRUE;
}
wresult w_image_create_from_stream(w_image *image, w_stream *stream) {
	return W_FALSE;
}
wresult w_image_create_from_buffer(w_image *image, const void *buffer,
		size_t size) {
	return W_FALSE;
}
wresult w_image_get_size(w_image *image, w_size *size) {
	if (_W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_NO_HANDLES;
	}
	size->width = gdk_pixbuf_get_width(_W_IMAGE(image)->pixbuf);
	size->height = gdk_pixbuf_get_height(_W_IMAGE(image)->pixbuf);
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
	w_image_dispose(destimage);
	if (image == 0 || _W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_NO_HANDLES;
	}
	if (destimage == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (rect == 0) {
		_W_IMAGE(destimage)->pixbuf = gdk_pixbuf_copy(
		_W_IMAGE(image)->pixbuf);
	} else {
		_W_IMAGE(destimage)->pixbuf = gdk_pixbuf_new_subpixbuf(
		_W_IMAGE(image)->pixbuf, rect->x, rect->y, rect->width, rect->height);
	}
	if (_W_IMAGE(destimage)->pixbuf == 0) {
		return W_ERROR_NO_HANDLES;
	}
	_W_IMAGE(destimage)->flags = 0;
	return W_TRUE;
}
wresult w_image_resize(w_image *image, w_size *size, w_image *destimage) {
	w_image_dispose(destimage);
	if (image == 0 || _W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_NO_HANDLES;
	}
	if (destimage == 0 || size == 0)
		return W_ERROR_NULL_ARGUMENT;
	GdkPixbuf *pixbuf = _W_IMAGE(image)->pixbuf;
	_W_IMAGE(destimage)->pixbuf = gdk_pixbuf_new(
			gdk_pixbuf_get_colorspace(pixbuf), gdk_pixbuf_get_has_alpha(pixbuf),
			gdk_pixbuf_get_bits_per_sample(pixbuf), size->width, size->height);
	if (_W_IMAGE(destimage)->pixbuf == 0) {
		return W_ERROR_NO_HANDLES;
	}
	gdk_pixbuf_scale(_W_IMAGE(image)->pixbuf, _W_IMAGE(destimage)->pixbuf, 0, 0,
			size->width, size->height, 0, 0, size->width, size->height,
			GDK_INTERP_HYPER);
	return W_FALSE;
}
wresult w_image_to_greyscale(w_image *image, w_image *destimage) {
	if (image == 0 || _W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_NO_HANDLES;
	}
	if (destimage == 0)
		return W_ERROR_NULL_ARGUMENT;
	GdkPixbuf *pixbuf = gdk_pixbuf_copy(_W_IMAGE(image)->pixbuf);
	const int w = gdk_pixbuf_get_width(_W_IMAGE(image)->pixbuf);
	const int h = gdk_pixbuf_get_height(_W_IMAGE(image)->pixbuf);
	const int stride = gdk_pixbuf_get_rowstride(pixbuf);
	const int channels = gdk_pixbuf_get_n_channels(pixbuf);
	guchar *dst = gdk_pixbuf_get_pixels(pixbuf);
	for (int j = 0; j < h; j++, dst += stride) {
		guchar *d = dst;
		for (int i = 0; i < w; i++, d += channels) {
			guchar intensity = (guchar) ((2 * d[0] + 5 * d[1] + d[2]) >> 3);
			d[0] = intensity;
			d[1] = intensity;
			d[2] = intensity;
		}
	}
	return W_TRUE;
}
wresult w_image_to_disabled(w_image *image, w_color bg, w_color fg,
		w_image *destimage) {
	if (image == 0 || _W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_NO_HANDLES;
	}
	if (destimage == 0)
		return W_ERROR_NULL_ARGUMENT;
	GdkPixbuf *pixbuf = gdk_pixbuf_copy(_W_IMAGE(image)->pixbuf);
	const int w = gdk_pixbuf_get_width(_W_IMAGE(image)->pixbuf);
	const int h = gdk_pixbuf_get_height(_W_IMAGE(image)->pixbuf);
	const int stride = gdk_pixbuf_get_rowstride(pixbuf);
	const int channels = gdk_pixbuf_get_n_channels(pixbuf);
	guchar *dst = gdk_pixbuf_get_pixels(pixbuf);
	guchar fg_r = 0, fg_g = 0, fg_b = 0;
	if (fg != 0) {
		fg_r = W_RED(fg);
		fg_g = W_GREEN(fg);
		fg_b = W_BLUE(fg);
	}
	guchar bg_r = 255, bg_g = 255, bg_b = 255;
	if (bg != 0) {
		bg_r = W_RED(bg);
		bg_g = W_GREEN(bg);
		bg_b = W_BLUE(bg);
	}
	for (int j = 0; j < h; j++, dst += stride) {
		guchar *d = dst;
		for (int i = 0; i < w; i++, d += channels) {
			int intensity = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
			if (intensity > 98304) { //if (d[0])
				d[0] = bg_r;
				d[1] = bg_g;
				d[2] = bg_b;
			} else {
				d[0] = fg_r;
				d[1] = fg_g;
				d[2] = fg_b;
			}
		}
	}
	return W_TRUE;
}
/*
 * surface
 */
void w_surface_init(w_surface *surface) {
	_W_SURFACE(surface)->handle = 0;
}
void w_surface_dispose(w_surface *surface) {
	if (surface != 0 && _W_SURFACE(surface)->handle != 0) {
		cairo_surface_destroy(_W_SURFACE(surface)->handle);
		_W_SURFACE(surface)->handle = 0;
	}
}
wresult w_surface_is_ok(w_surface *surface) {
	return _W_SURFACE(surface)->handle != 0;
}
wresult w_surface_create(w_surface *surface, w_size *size) {
	w_surface_dispose(surface);
#if USE_CAIRO
	_W_SURFACE(surface)->handle = cairo_image_surface_create(
			CAIRO_FORMAT_ARGB32, size->width, size->height);
	if ( _W_SURFACE(surface)->handle == 0) {
		return W_ERROR_NO_HANDLES;
	}
	return W_TRUE;
#else

#endif
}
wresult w_surface_get_image(w_surface *surface, w_image *destimage) {
	if (surface == 0 && _W_SURFACE(surface)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (destimage == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_image_dispose(destimage);
	cairo_surface_t *handle = _W_SURFACE(surface)->handle;
	_W_IMAGE(destimage)->pixbuf = gdk_pixbuf_get_from_surface(handle, 0, 0,
			cairo_image_surface_get_width(handle),
			cairo_image_surface_get_height(handle));
	if (_W_IMAGE(destimage)->pixbuf == 0) {
		return W_ERROR_NO_HANDLES;
	} else {
		return W_TRUE;
	}
}
wresult w_surface_get_graphics(w_surface *surface, w_graphics *graphics) {
	if (surface == 0 && _W_SURFACE(surface)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (graphics == 0)
		return W_ERROR_NULL_ARGUMENT;
	cairo_t *cairo = cairo_create(_W_SURFACE(surface)->handle);
	if (cairo == 0) {
		return W_ERROR_NO_HANDLES;
	}
	_w_graphics_init(graphics, cairo);
	return W_TRUE;
}
