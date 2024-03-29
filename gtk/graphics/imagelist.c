/*
 * Name:        imagelist.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
void w_imagelist_init(w_imagelist *imagelist) {
	_W_IMAGELIST(imagelist)->handles = 0;
}
GdkPixbuf* _w_imagelist_get_pixbuf(w_imagelist *imagelist, int index) {
	if (imagelist == 0)
		return 0;
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	if (handles == 0)
		return 0;
	return handles->pixbufs[index];
}
void w_imagelist_mime_types_free(w_array *array, void *element, int index) {
	_w_imagelist_mime *mime = element;
	if (mime->mime != 0) {
		free(mime->mime);
		mime->mime = 0;
	}
}
void w_imagelist_dispose(w_imagelist *imagelist) {
	if (imagelist == 0)
		return;
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	if (handles == 0)
		return;
	w_array *mime_types = handles->mime_types;
	if (mime_types != 0) {
		w_array_free(mime_types, sizeof(_w_imagelist_mime),
				w_imagelist_mime_types_free);
	}
	GdkPixbuf **pixbuf = handles->pixbufs;
	int count = handles->count;
	for (int i = 0; i < count; i++) {
		GdkPixbuf *image = pixbuf[i];
		if (image != 0)
			g_object_unref(image);
	}
	free(handles);
	_W_IMAGELIST(imagelist)->handles = 0;
}
wresult w_imagelist_create(w_imagelist *imagelist, w_size *size,
		int initialCount) {
	w_imagelist_dispose(imagelist);
	if (initialCount == 0)
		initialCount = 4;
	int _sz = sizeof(_w_imagelist_info) + initialCount * sizeof(GdkPixbuf*);
	_w_imagelist_info *handles = malloc(_sz);
	if (handles == 0)
		return W_ERROR_NO_MEMORY;
	handles->alloc = initialCount;
	handles->count = 0;
	handles->width = size->width;
	handles->height = size->height;
	handles->style = 0;
	handles->mime_types = 0;
	_W_IMAGELIST(imagelist)->handles = handles;
	return W_TRUE;
}
int w_imagelist_add(w_imagelist *imagelist, w_image *image) {
	if (imagelist == 0)
		return W_ERROR_NULL_ARGUMENT;
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	if (handles == 0)
		return W_ERROR_NO_HANDLES;
	if (image == 0 || _W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_INVALID_ARGUMENT;
	}
	if (handles->count >= handles->alloc) {
		int newalloc = handles->alloc + 8;
		int newsize = sizeof(_w_imagelist_info) + newalloc * sizeof(GdkPixbuf*);
		handles = realloc(handles, newsize);
		if (handles == 0)
			return W_ERROR_NO_MEMORY;
		_W_IMAGELIST(imagelist)->handles = handles;
		handles->alloc = newalloc;
	}
	GdkPixbuf *newpixbuf = 0;
	int width = gdk_pixbuf_get_width(_W_IMAGE(image)->pixbuf);
	int heigth = gdk_pixbuf_get_height(_W_IMAGE(image)->pixbuf);
	if (width == handles->width && heigth == handles->height) {
		newpixbuf = gdk_pixbuf_copy(_W_IMAGE(image)->pixbuf);
	} else {
		newpixbuf = gdk_pixbuf_scale_simple(_W_IMAGE(image)->pixbuf,
				handles->width, handles->height, GDK_INTERP_BILINEAR);
	}
	if (newpixbuf == 0)
		return W_ERROR_NO_HANDLES;
	int newindex = handles->count;
	handles->pixbufs[handles->count] = newpixbuf;
	handles->count++;
	return newindex;
}
int w_imagelist_add_mimetype(w_imagelist *imagelist, const char *filename,
		int flags) {
	if (imagelist == 0)
		return W_ERROR_NULL_ARGUMENT;
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	if (handles == 0)
		return W_ERROR_NO_HANDLES;
	char *mime_type = 0;
	char *mime_type_free = 0;
	GFileInfo *info = 0;
	switch (flags) {
	case W_IMAGELIST_MIME_DEFAULT:
		mime_type = filename;
		break;
	case W_IMAGELIST_MIME_DIRECTORY:
		mime_type = "inode/directory";
		break;
	case W_IMAGELIST_MIME_EXTENSION: {
		gboolean is_certain = FALSE;

		char *content_type = g_content_type_guess(filename, NULL, 0,
				&is_certain);

		if (content_type != NULL) {
			mime_type = g_content_type_get_mime_type(content_type);
			mime_type_free = mime_type;
			g_free(content_type);
		}
	}
		break;
	case W_IMAGELIST_MIME_FILE: {
		GFile *file = g_file_new_for_path(filename);
		if (file) {
			GFileInfo *info = g_file_query_info(file,
			G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE, G_FILE_QUERY_INFO_NONE,
			NULL, NULL);
			if (info) {
				mime_type = g_file_info_get_content_type(info);
			}
			g_object_unref(file);
		}
	}
		break;
	}
	if (mime_type == 0)
		mime_type = "text/plein";
	int index = -1;
	_w_imagelist_mime *mimes;
	int count = w_array_get_count(handles->mime_types, (void**) &mimes);
	for (int i = 0; i < count; i++) {
		if (!strcmp(mimes[i].mime, mime_type)) {
			index = mimes[i].index;
			break;
		}
	}
	if (index == -1) {
		_w_image img;
		w_image_init(W_IMAGE(&img));
		w_size size;
		w_imagelist_get_size(imagelist, &size);
		GtkIconTheme *icon_theme = gtk_icon_theme_get_default();
		if (icon_theme) {
			GIcon *mime_icon = g_content_type_get_icon(mime_type);
			if (mime_icon) {
				GtkIconInfo *icon_info = gtk_icon_theme_lookup_by_gicon(
						icon_theme, mime_icon, size.width,
						GTK_ICON_LOOKUP_USE_BUILTIN);
				if (icon_info) {
					img.pixbuf = gtk_icon_info_load_icon(icon_info, NULL);
					img.flags = 0;
					g_object_unref(icon_info);
				}
				g_object_unref(mime_icon);
			}
		}
		index = w_imagelist_add(imagelist, W_IMAGE(&img));
		w_image_dispose(W_IMAGE(&img));
		int newIndex;
		_w_imagelist_mime *miimes = w_array_add(&handles->mime_types, -1,
				sizeof(_w_imagelist_mime), &newIndex);
		if (miimes != 0) {
			miimes->index = index;
			miimes->mime = strdup(mime_type);
		}
	}
	if (info != 0) {
		g_object_unref(info);
	}
	if (mime_type_free != 0) {
		g_free(mime_type_free);
	}
	return index;
}
wresult w_imagelist_is_ok(w_imagelist *imagelist) {
	return imagelist != 0 && _W_IMAGELIST(imagelist)->handles != 0;
}
wresult w_imagelist_replace(w_imagelist *imagelist, int index, w_image *image) {
	if (imagelist == 0)
		return W_ERROR_NULL_ARGUMENT;
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	if (handles == 0)
		return W_ERROR_NO_HANDLES;
	if (image == 0 || _W_IMAGE(image)->pixbuf == 0) {
		return W_ERROR_INVALID_ARGUMENT;
	}
	_w_imagelist_info *info;
	GdkPixbuf **pixbuf = handles->pixbufs;
	int count = handles->count;
	if (index < count) {
		GdkPixbuf *imageidx, *imagelast;
		imagelast = pixbuf[index];
		int width = gdk_pixbuf_get_width(_W_IMAGE(image)->pixbuf);
		int heigth = gdk_pixbuf_get_height(_W_IMAGE(image)->pixbuf);
		if (width == info->width && heigth == info->height) {
			imageidx = gdk_pixbuf_copy(_W_IMAGE(image)->pixbuf);
		} else {
			imageidx = gdk_pixbuf_scale_simple(_W_IMAGE(image)->pixbuf,
					info->width, info->height, GDK_INTERP_BILINEAR);
		}
		if (imageidx == 0)
			return W_ERROR_NO_HANDLES;
		g_object_ref(imageidx);
		pixbuf[index] = imageidx;
		g_object_unref(imagelast);
		return index;
	} else {
		return w_imagelist_add(imagelist, image);
	}
}
wresult w_imagelist_remove(w_imagelist *imagelist, int index) {
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	return W_FALSE;
}
wresult w_imagelist_remove_all(w_imagelist *imagelist) {
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	return W_FALSE;
}
wresult w_imagelist_get_image(w_imagelist *imagelist, int index, int copy,
		w_image *image) {
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	return W_FALSE;
}
wresult w_imagelist_get_size(w_imagelist *imagelist, w_size *size) {
	if (imagelist == 0)
		return W_ERROR_NULL_ARGUMENT;
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	if (handles == 0)
		return W_ERROR_NO_HANDLES;
	size->width = handles->width;
	size->height = handles->height;
	return W_TRUE;
}
int w_imagelist_get_count(w_imagelist *imagelist) {
	if (imagelist == 0)
		return W_ERROR_NULL_ARGUMENT;
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	if (handles == 0)
		return W_ERROR_NO_HANDLES;
	return handles->count;
}
wresult w_imagelist_draw(w_imagelist *imagelist, w_graphics *graphics,
		int index, w_point *pt, int state) {
	return w_imagelist_draw_ex(imagelist, graphics, index, pt, state, 0, 0);
}
wresult w_imagelist_draw_ex(w_imagelist *imagelist, w_graphics *graphics,
		int index, w_point *pt, int state, w_color bg, w_color fg) {
	if (imagelist == 0)
		return W_ERROR_NULL_ARGUMENT;
	_w_imagelist_info *handles = _W_IMAGELIST(imagelist)->handles;
	if (handles == 0)
		return W_ERROR_NO_HANDLES;
	if (graphics == 0 || _W_GRAPHICS(graphics)->cairo == 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (index < 0 || index >= handles->count)
		return W_ERROR_INVALID_RANGE;
	w_image img;
	_W_IMAGE(&img)->pixbuf = handles->pixbufs[index];
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
