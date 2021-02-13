/*
 * Name:        cursor.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_CURSOR_H_
#define SWT_GRAPHICS_CURSOR_H_
#include "image.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_cursor {
	void *handle[2];
} w_cursor;
#define W_CURSOR(x) ((w_cursor*)x)
SWT_PUBLIC void w_cursor_init(w_cursor *cursor);
SWT_PUBLIC void w_cursor_dispose(w_cursor *cursor);
SWT_PUBLIC wresult w_cursor_is_ok(w_cursor *cursor);
SWT_PUBLIC wresult w_cursor_create(w_cursor *cursor, w_image *source,
		int hotspotX, int hotspotY);
#ifdef __cplusplus
}
#endif
#endif /* SWT_GRAPHICS_CURSOR_H_ */
