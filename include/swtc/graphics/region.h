/*
 * Name:        region.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_REGION_H_
#define SWT_GRAPHICS_REGION_H_
#include "rect.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_region {
	void* handle[2];
} w_region;
#define W_REGION(x) ((w_region*)x)
SWT_PUBLIC void w_region_init(w_region* region);
SWT_PUBLIC void w_region_dispose(w_region* region);
SWT_PUBLIC wresult w_region_is_ok(w_region* region);
SWT_PUBLIC wresult w_region_create(w_region* region);
SWT_PUBLIC wresult w_region_create_rectangle(w_region* region,w_rect* rect);
SWT_PUBLIC wresult w_region_create_ellipse(w_region* region,w_rect* rect);
SWT_PUBLIC wresult w_region_add_points(w_region* region,w_point *points, size_t count);
SWT_PUBLIC wresult w_region_add_rectangle(w_region* region,w_rect *rect);
SWT_PUBLIC wresult w_region_add_region(w_region* region,w_region* _region);
SWT_PUBLIC wresult w_region_contains(w_region* region,w_point* pt);
SWT_PUBLIC wresult w_region_get_bounds(w_region* region,w_rect* bounds);
SWT_PUBLIC wresult w_region_intersect_rect(w_region* region,w_rect* rect);
SWT_PUBLIC wresult w_region_intersect_region(w_region* region,w_region* _region);
SWT_PUBLIC wresult w_region_intersects(w_region* region,w_rect* rect);
SWT_PUBLIC wresult w_region_is_empty(w_region* region);
SWT_PUBLIC wresult w_region_subtract_points(w_region* region,w_point* points, size_t count);
SWT_PUBLIC wresult w_region_subtract_rectangle(w_region* region,w_rect* rect);
SWT_PUBLIC wresult w_region_subtract_region(w_region* region,w_region* _region);
SWT_PUBLIC wresult w_region_translate(w_region* region,w_point* pt);

#ifdef __cplusplus
}
#endif
#endif /* SWT_GRAPHICS_REGION_H_ */
