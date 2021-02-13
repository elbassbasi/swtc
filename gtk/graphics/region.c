/*
 * Name:        region.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"

void w_region_init(w_region *region) {
}
void w_region_dispose(w_region *region) {
}
wresult w_region_is_ok(w_region *region) {
	return W_FALSE;
}
wresult w_region_create(w_region *region) {
	return W_FALSE;
}
wresult w_region_create_rectangle(w_region *region, w_rect *rect) {
	return W_FALSE;
}
wresult w_region_create_ellipse(w_region *region, w_rect *rect) {
	return W_FALSE;
}
wresult w_region_add_points(w_region *region, w_point *points, size_t count) {
	return W_FALSE;
}
wresult w_region_add_rectangle(w_region *region, w_rect *rect) {
	return W_FALSE;
}
wresult w_region_add_region(w_region *region, w_region *_region) {
	return W_FALSE;
}
wresult w_region_contains(w_region *region, w_point *pt) {
	return W_FALSE;
}
wresult w_region_get_bounds(w_region *region, w_rect *bounds) {
	return W_FALSE;
}
wresult w_region_intersect_rect(w_region *region, w_rect *rect) {
	return W_FALSE;
}
wresult w_region_intersect_region(w_region *region, w_region *_region) {
	return W_FALSE;
}
wresult w_region_intersects(w_region *region, w_rect *rect) {
	return W_FALSE;
}
wresult w_region_is_empty(w_region *region) {
	return W_FALSE;
}
wresult w_region_subtract_points(w_region *region, w_point *points,
		size_t count) {
	return W_FALSE;
}
wresult w_region_subtract_rectangle(w_region *region, w_rect *rect) {
	return W_FALSE;
}
wresult w_region_subtract_region(w_region *region, w_region *_region) {
	return W_FALSE;
}
wresult w_region_translate(w_region *region, w_point *pt) {
	return W_FALSE;
}
