/*
 * Name:        path.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_PATH_H_
#define SWT_GRAPHICS_PATH_H_
#include "rect.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_pathdata {
	int type;
	float pt[6];
} w_pathdata;
typedef struct w_path {
	void *handle;
	float temp[6];
} w_path;
#define W_PATH(x) ((w_path*)x)
SWT_PUBLIC void w_path_init(w_path *path);
SWT_PUBLIC void w_path_dispose(w_path *path);
SWT_PUBLIC wresult w_path_create(w_path *path);
SWT_PUBLIC wresult w_path_create_from_path(w_path *path, w_path *frompath,
		float flatness);
SWT_PUBLIC wresult w_path_create_from_data(w_path *path, size_t length,
		char *types, float *pt);
SWT_PUBLIC wresult w_path_add_arc(w_path *path, w_rectf *rect, float startAngle,
		float arcAngle);
SWT_PUBLIC wresult w_path_add_path(w_path *path, w_path *frompath);
SWT_PUBLIC wresult w_path_add_rectangle(w_path *path, w_rectf *rect);
SWT_PUBLIC wresult w_path_add_string(w_path *path, const char *string,
		int length, int enc, w_pointf *pt, w_font *font);
SWT_PUBLIC wresult w_path_close(w_path *path);
SWT_PUBLIC wresult w_path_contains(w_path *path, w_pointf *pt, w_graphics *gc,
		int outline);
SWT_PUBLIC wresult w_path_cubicto(w_path *path, float cx1, float cy1, float cx2,
		float cy2, float x, float y);
SWT_PUBLIC wresult w_path_get_bounds(w_path *path, w_rectf *bounds);
SWT_PUBLIC wresult w_path_get_current_point(w_path *path, w_pointf *point);
SWT_PUBLIC wresult w_path_get_path_data(w_path *path, w_iterator *data);
SWT_PUBLIC wresult w_path_lineto(w_path *path, float x, float y);
SWT_PUBLIC wresult w_path_is_ok(w_path *path);
SWT_PUBLIC wresult w_path_move_to(w_path *path, float x, float y);
SWT_PUBLIC wresult w_path_quadto(w_path *path, float cx, float cy, float x,
		float y);

#ifdef __cplusplus
}
#endif

#endif /* SWT_GRAPHICS_PATH_H_ */
