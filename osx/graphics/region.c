/*
 * Name:        region.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"

void w_region_init(w_region *region) {
	_W_REGION(region)->handle = 0;
}
void w_region_dispose(w_region *region) {
	if (region != 0 && _W_REGION(region)->handle != 0) {
		NSObject_release(NSOBJECT(_W_REGION(region)->handle));
		_W_REGION(region)->handle = 0;
	}
}
wresult w_region_is_ok(w_region *region) {
	return region != 0 && _W_REGION(region)->handle != 0;
}
wresult w_region_create(w_region *region) {
	w_region_dispose(region);
	_W_REGION(region)->handle = NSBezierPath_new();
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	return W_TRUE;
}
wresult w_region_create_rectangle(w_region *region, w_rect *rect) {
	w_region_dispose(region);
	NSRect r;
	r.x = rect->x;
	r.y = rect->y;
	r.width = rect->width;
	r.height = rect->height;
	NSBezierPath *handle = NSBezierPath_bezierPathWithRect(&r);
	if (handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_REGION(region)->handle = handle;
	return W_TRUE;
}
wresult w_region_create_ellipse(w_region *region, w_rect *rect) {
	return W_FALSE;
}
wresult w_region_add_points(w_region *region, w_point *points, size_t count) {
	return W_FALSE;
}
wresult w_region_add_rectangle(w_region *region, w_rect *rect) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	if (handle == 0)
		return W_ERROR_NO_HANDLES;
	NSRect r;
	r.x = rect->x;
	r.y = rect->y;
	r.width = rect->width;
	r.height = rect->height;
	NSBezierPath_appendBezierPathWithRect(handle, &r);
	_W_REGION(region)->handle = handle;
	return W_TRUE;
}
wresult w_region_add_region(w_region *region, w_region *_region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	if (handle == 0)
		return W_ERROR_NO_HANDLES;
	NSBezierPath_appendBezierPath(handle, _W_REGION(_region)->handle);
	_W_REGION(region)->handle = handle;
	return W_TRUE;
}
wresult w_region_contains(w_region *region, w_point *pt) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	NSPoint p;
	p.x = pt->x;
	p.y = pt->y;
	return NSBezierPath_containsPoint(handle, &p);
}
wresult w_region_get_bounds(w_region *region, w_rect *bounds) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	NSRect r;
	NSBezierPath_bounds(handle, &r);
	bounds->x = r.x;
	bounds->y = r.y;
	bounds->width = r.width;
	bounds->height = r.height;
	return W_TRUE;
}
wresult w_region_intersect_rect(w_region *region, w_rect *rect) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	return W_FALSE;
}
wresult w_region_intersect_region(w_region *region, w_region *_region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	return W_FALSE;
}
wresult w_region_intersects(w_region *region, w_rect *rect) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	return W_FALSE;
}
wresult w_region_is_empty(w_region *region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	return NSBezierPath_isEmpty(handle);
}
wresult w_region_subtract_points(w_region *region, w_point *points,
		size_t count) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	return W_FALSE;
}
wresult w_region_subtract_rectangle(w_region *region, w_rect *rect) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	return W_FALSE;
}
wresult w_region_subtract_region(w_region *region, w_region *_region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	return W_FALSE;
}
wresult w_region_translate(w_region *region, w_point *pt) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSBezierPath *handle = _W_REGION(region)->handle;
	return W_FALSE;
}
