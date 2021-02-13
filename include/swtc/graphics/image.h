/*
 * Name:        image.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_IMAGE_H_
#define SWT_GRAPHICS_IMAGE_H_
#include "rect.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef unsigned int w_color;
#define W_RGBA(r,g,b,a) (((((int)(a)) & 0xFF) << 24) | ((((int)(b)) & 0xFF) << 16) | \
                      ((((int)(g)) & 0xFF) << 8)  | ((((int)(r)) & 0xFF) << 0))
#define W_RGB(r,g,b) W_RGBA(r,g,b,0xff)
#define W_RED(color) (color & 0xFF)
#define W_GREEN(color) ((color & 0xFF00) >> 8)
#define W_BLUE(color) ((color & 0xFF0000) >> 16)
#define W_ALPHA(color) ((color >> 24) & 0xff)

enum {
	W_IMAGE_ARGB32 = 0, W_IMAGE_RGB24 = 1, W_IMAGE_A8 = 2, W_IMAGE_A1 = 3,
};

typedef struct w_graphics {
	void *handle[0x40];
} w_graphics;
#define W_GRAPHICS(x) ((w_graphics*)x)

typedef struct w_image {
	void *handle[2];
} w_image;
#define W_IMAGE(x) ((w_image*)x)
SWT_PUBLIC void w_image_init(w_image *image);
SWT_PUBLIC void w_image_dispose(w_image *image);
SWT_PUBLIC wresult w_image_is_ok(w_image *image);
SWT_PUBLIC wresult w_image_create_from_file(w_image *image, const char *file,
		int length, int enc);
SWT_PUBLIC wresult w_image_create_from_data(w_image *image, void *data,
		wuint format, w_size *size, int rowstride);
SWT_PUBLIC wresult w_image_create_from_stream(w_image *image, w_stream *stream);
SWT_PUBLIC int w_image_get_rowstride(wuint format, wuint width);
SWT_PUBLIC wresult w_image_create_from_buffer(w_image *image,
		const void *buffer, size_t size);
SWT_PUBLIC wresult w_image_get_size(w_image *image, w_size *size);
SWT_PUBLIC wresult w_image_get_pixels(w_image *image, w_color *colors,
		size_t length);
SWT_PUBLIC wresult w_image_save_to_file(w_image *image, const char *file,
		int length, int enc, const char *type);
SWT_PUBLIC wresult w_image_save_to_buffer(w_image *image, w_alloc alloc,
		void *user_data, const char *type);
SWT_PUBLIC wresult w_image_save_to_stream(w_image *image, w_stream *stream,
		const char *type);
SWT_PUBLIC wresult w_image_copy(w_image *image, w_rect *rect,
		w_image *destimage);
SWT_PUBLIC wresult w_image_resize(w_image *image, w_size *size,
		w_image *destimage);
SWT_PUBLIC wresult w_image_to_greyscale(w_image *image, w_image *destimage);
SWT_PUBLIC wresult w_image_to_disabled(w_image *image, w_color bg, w_color fg,
		w_image *destimage);
/*
 * surface
 */
typedef struct w_surface {
	void *handle[2];
} w_surface;
#define W_SURFACE(x) ((w_surface*)x)
SWT_PUBLIC void w_surface_init(w_surface *surface);
SWT_PUBLIC void w_surface_dispose(w_surface *surface);
SWT_PUBLIC wresult w_surface_is_ok(w_surface *surface);
SWT_PUBLIC wresult w_surface_create(w_surface *surface, w_size *size);
SWT_PUBLIC wresult w_surface_get_image(w_surface *surface, w_image *destimage);
SWT_PUBLIC wresult w_surface_get_graphics(w_surface *surface,
		w_graphics *graphics);

#ifdef __cplusplus
}
#endif

#endif /* SWT_GRAPHICS_IMAGE_H_ */
