/*
 * Name:        rect.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_RECT_H_
#define SWT_GRAPHICS_RECT_H_
#include "../core/core.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_point {
	int x;
	int y;
} w_point;
typedef struct w_pointf {
	float x;
	float y;
} w_pointf;
typedef struct w_size {
	int width;
	int height;
} w_size;
typedef struct w_sizef {
	float width;
	float height;
} w_sizef;
typedef struct w_rect {
	union {
		struct {
			int x;
			int y;
			int width;
			int height;
		};
		struct {
			w_point pt;
			w_size sz;
		};
	};
} w_rect;
SWT_PUBLIC void w_rect_add(w_rect *r, w_rect *rect);
SWT_PUBLIC wresult w_rect_contains(w_rect *r, int x, int y);
SWT_PUBLIC wresult w_rect_equals(w_rect *r, w_rect *rect);
SWT_PUBLIC void w_rect_intersect(w_rect *r, w_rect *rect);
SWT_PUBLIC void w_rect_intersection(w_rect *result, w_rect *r, w_rect *rect);
SWT_PUBLIC wresult w_rect_intersects(w_rect *r, w_rect *rect);
SWT_PUBLIC wresult w_rect_is_empty(w_rect *r);
SWT_PUBLIC void w_rect_union(w_rect *result, w_rect *r, w_rect *rect);
typedef struct w_rectf {
	float x;
	float y;
	float width;
	float height;
} w_rectf;
typedef struct w_range {
	int start;
	int end;
} w_range;
#ifdef __cplusplus
}
#endif

#endif /* SWT_GRAPHICS_RECT_H_ */
