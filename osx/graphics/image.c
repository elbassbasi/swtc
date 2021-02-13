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
}
void w_image_dispose(w_image *image) {
}
wresult w_image_is_ok(w_image *image) {
	return W_FALSE;
}
wresult w_image_create_from_file(w_image *image, const char *file, int length,
		int enc) {
	w_image_dispose(image);
	return W_FALSE;
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
	return W_FALSE;
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
}
void w_surface_dispose(w_surface *surface) {
}
wresult w_surface_is_ok(w_surface *surface) {
	return W_FALSE;
}
wresult w_surface_create(w_surface *surface, w_size *size) {
	return W_FALSE;
}
wresult w_surface_get_image(w_surface *surface, w_image *destimage) {
	return W_FALSE;
}
wresult w_surface_get_graphics(w_surface *surface, w_graphics *graphics) {
	return W_FALSE;
}
