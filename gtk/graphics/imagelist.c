/*
 * Name:        imagelist.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
void w_imagelist_init(w_imagelist *imagelist) {
	_W_IMAGELIST(imagelist)->images = 0;
}
void w_imagelist_dispose(w_imagelist *imagelist) {
	if (_W_IMAGELIST(imagelist)->images == 0)
		return;
	struct _w_imagelist_images *images = _W_IMAGELIST(imagelist)->images;
	int i, count = images->count;
	for (i = 0; i < count; i++) {
		GdkPixbuf *image = images->images[i];
		if (image != 0)
			g_object_unref(image);
	}
	free(images);
	_W_IMAGELIST(imagelist)->images = 0;
}
wresult w_imagelist_create(w_imagelist *imagelist, w_size *size,
		int initialCount) {
	if (initialCount == 0)
		initialCount = 4;
	_W_IMAGELIST(imagelist)->images = (struct _w_imagelist_images*) malloc(
			sizeof(struct _w_imagelist_images)
					+ initialCount * sizeof(struct _w_imagelist_image*));
	if (_W_IMAGELIST(imagelist)->images == 0) {
		return W_ERROR_NO_HANDLES;
	}
	_W_IMAGELIST(imagelist)->images->alloc = initialCount;
	_W_IMAGELIST(imagelist)->images->count = 0;
	_W_IMAGELIST(imagelist)->images->width = size->width;
	_W_IMAGELIST(imagelist)->images->height = size->height;
	_W_IMAGELIST(imagelist)->images->reserved = 0;
	return W_TRUE;
}
int w_imagelist_add(w_imagelist *imagelist, w_image *image) {
	if (_W_IMAGELIST(imagelist)->images == 0)
		return W_ERROR_NO_HANDLES;
	if (image == 0 || _W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_INVALID_ARGUMENT;
	}
	struct _w_imagelist_images *images = _W_IMAGELIST(imagelist)->images;
	if (images->count >= images->alloc) {
		size_t new_alloc = images->alloc + 4;
		struct _w_imagelist_images *newimages = realloc(images,
				sizeof(struct _w_imagelist_images)
						+ new_alloc * sizeof(struct _w_imagelist_image*));
		if (newimages == 0) {
			return W_ERROR_NO_MEMORY;
		}
		newimages->alloc = new_alloc;
		_W_IMAGELIST(imagelist)->images = newimages;
	}
	GdkPixbuf *imageidx;
	int width = gdk_pixbuf_get_width(_W_IMAGE(image)->pixbuf);
	int heigth = gdk_pixbuf_get_height(_W_IMAGE(image)->pixbuf);
	if (width == images->width && heigth == images->height) {
		imageidx = gdk_pixbuf_copy(_W_IMAGE(image)->pixbuf);
	} else {
		imageidx = gdk_pixbuf_scale_simple(_W_IMAGE(image)->pixbuf,
				images->width, images->height, GDK_INTERP_BILINEAR);
	}
	if (imageidx == 0) {
		return W_ERROR_NO_MEMORY;
	} else {
		images->images[images->count] = imageidx;
	}
	images->count++;
	return W_TRUE;
}
wresult w_imagelist_is_ok(w_imagelist *imagelist) {
	return imagelist != 0 && _W_IMAGELIST(imagelist)->images != 0;
}
wresult w_imagelist_replace(w_imagelist *imagelist, int index, w_image *image) {
	if (_W_IMAGELIST(imagelist)->images == 0)
		return W_ERROR_NO_HANDLES;
	if (image == 0 || _W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_INVALID_ARGUMENT;
	}
	_w_imagelist_images *images = _W_IMAGELIST(imagelist)->images;
	if (images->count > index) {
		GdkPixbuf *imageidx, *imagelast;
		imagelast = images->images[index];
		int width = gdk_pixbuf_get_width(_W_IMAGE(image)->pixbuf);
		int heigth = gdk_pixbuf_get_height(_W_IMAGE(image)->pixbuf);
		if (width == images->width && heigth == images->height) {
			imageidx = gdk_pixbuf_copy(_W_IMAGE(image)->pixbuf);
		} else {
			imageidx = gdk_pixbuf_scale_simple(_W_IMAGE(image)->pixbuf,
					images->width, images->height, GDK_INTERP_BILINEAR);
		}
		if (imageidx == 0)
			return W_ERROR_NO_HANDLES;
		images->images[images->count] = imageidx;
		g_object_unref(imagelast);
		return W_TRUE;
	} else {
		return w_imagelist_add(imagelist, image);
	}
}
wresult w_imagelist_remove(w_imagelist *imagelist, int index) {
	return W_FALSE;
}
wresult w_imagelist_remove_all(w_imagelist *imagelist) {
	return W_FALSE;
}
GdkPixbuf* w_imagelist_get_pixbuf(w_imagelist *imagelist, int index) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->images == 0)
		return 0;
	_w_imagelist_images *images = _W_IMAGELIST(imagelist)->images;
	if (images->count > index) {
		return images->images[index];
	}
	return 0;
}
wresult w_imagelist_get_image(w_imagelist *imagelist, int index, int copy,
		w_image *image) {
	return W_FALSE;
}
wresult w_imagelist_get_size(w_imagelist *imagelist, w_size *size) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->images == 0)
		return W_ERROR_NO_HANDLES;
	size->width = _W_IMAGELIST(imagelist)->images->width;
	size->height = _W_IMAGELIST(imagelist)->images->height;
	return W_TRUE;
}
int w_imagelist_get_count(w_imagelist *imagelist) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->images == 0)
		return 0;
	return _W_IMAGELIST(imagelist)->images->count;
}
wresult w_imagelist_draw(w_imagelist *imagelist, w_graphics *graphics,
		int index, w_point *pt, int state) {
	return w_imagelist_draw_ex(imagelist, graphics, index, pt, state, 0, 0);
}
wresult w_imagelist_draw_ex(w_imagelist *imagelist, w_graphics *graphics,
		int index, w_point *pt, int state, w_color bg, w_color fg) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->images == 0)
		return W_ERROR_NO_HANDLES;
	if (graphics == 0 || _W_GRAPHICS(graphics)->cairo == 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (index < 0 || index >= _W_IMAGELIST(imagelist)->images->count)
		return W_ERROR_INVALID_RANGE;
	w_image img;
	_W_IMAGE(&img)->pixbuf = _W_IMAGELIST(imagelist)->images->images[index];
	w_rect src, dest;
	src.x = 0;
	src.y = 0;
	src.width = -1;
	src.height = -1;
	dest.x = pt->x;
	dest.y = pt->y;
	dest.width = -1;
	dest.height = -1;
	w_color last_bg, last_fg;
	if (state & W_DISABLED) {
		if (bg != 0) {
			last_bg = w_graphics_get_background(graphics);
			w_graphics_set_background(graphics, bg);
		}
		if (fg != 0) {
			last_fg = w_graphics_get_foreground(graphics);
			w_graphics_set_foreground(graphics, fg);
		}
	}
	w_graphics_draw_image(graphics, &img, &src, &dest, state);
	if (state & W_DISABLED) {
		if (bg != 0)
			w_graphics_set_background(graphics, last_bg);
		if (fg != 0)
			w_graphics_set_foreground(graphics, last_fg);
	}
	return W_TRUE;
}
