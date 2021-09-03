/*
 * Name:        imagelist.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_IMAGELIST_H_
#define SWT_GRAPHICS_IMAGELIST_H_
#include "image.h"
#ifdef __cplusplus
extern "C" {
#endif
enum {
	W_IMAGELIST_DRAW_NORMAL = 0,
	W_IMAGELIST_DRAW_TRANSPARENT = W_TRANSPARENT,
	W_IMAGELIST_DRAW_SELECTED = W_SELECTED,
	W_IMAGELIST_DRAW_FOCUSED = W_FOCUSED,
	W_IMAGELIST_DRAW_DISABLED = W_DISABLED,
};
typedef struct w_imagelist {
	void *handle[2];
} w_imagelist;

#define W_IMAGELIST(x) ((w_imagelist*)x)
SWT_PUBLIC void w_imagelist_init(w_imagelist *imagelist);
SWT_PUBLIC void w_imagelist_dispose(w_imagelist *imagelist);
SWT_PUBLIC wresult w_imagelist_create(w_imagelist *imagelist, w_size *size,
		int initialCount);
SWT_PUBLIC int w_imagelist_add(w_imagelist *imagelist, w_image *image);
SWT_PUBLIC wresult w_imagelist_is_ok(w_imagelist *imagelist);
SWT_PUBLIC wresult w_imagelist_replace(w_imagelist *imagelist, int index,
		w_image *image);
SWT_PUBLIC wresult w_imagelist_remove(w_imagelist *imagelist, int index);
SWT_PUBLIC wresult w_imagelist_remove_all(w_imagelist *imagelist);
SWT_PUBLIC wresult w_imagelist_get_image(w_imagelist *imagelist, int index,
		int copy, w_image *image);
SWT_PUBLIC wresult w_imagelist_get_size(w_imagelist *imagelist, w_size *size);
SWT_PUBLIC int w_imagelist_get_count(w_imagelist *imagelist);
SWT_PUBLIC wresult w_imagelist_draw(w_imagelist *imagelist,
		w_graphics *graphics, int index, w_point *pt, int state);
SWT_PUBLIC wresult w_imagelist_draw_ex(w_imagelist *imagelist,
		w_graphics *graphics, int index, w_point *pt, int state, w_color bg,
		w_color fg);
/*
 *
 */
enum {
	W_IMAGELIST_MIME_DEFAULT = 0, //
	W_IMAGELIST_MIME_DIRECTORY, //
	W_IMAGELIST_MIME_EXTENSION,
	W_IMAGELIST_MIME_FILE,
};
SWT_PUBLIC int w_imagelist_add_mimetype(w_imagelist *imagelist,
		const char *filename, int flags);

#ifdef __cplusplus
}
#endif
#endif /* SWT_GRAPHICS_IMAGELIST_H_ */
