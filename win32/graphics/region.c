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
	if (_W_REGION(region)->handle == 0) {
		DeleteObject(_W_REGION(region)->handle);
		_W_REGION(region)->handle = 0;
	}
}
wresult w_region_is_ok(w_region *region) {
	return region != 0 && _W_REGION(region)->handle != 0;
}
wresult w_region_create(w_region *region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_region_dispose(region);
	_W_REGION(region)->handle = CreateRectRgn(0, 0, 0, 0);
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	return W_TRUE;
}
wresult w_region_create_rectangle(w_region *region, w_rect *rect) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_region_dispose(region);
	_W_REGION(region)->handle = CreateRectRgn(rect->x, rect->y, rect->width,
			rect->height);
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	return W_TRUE;
}
wresult w_region_create_ellipse(w_region *region, w_rect *rect) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_region_dispose(region);
	_W_REGION(region)->handle = CreateEllipticRgn(rect->x, rect->y, rect->width,
			rect->height);
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	return W_TRUE;
}
wresult w_region_add_points(w_region *region, w_point *points, size_t count) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	HRGN polyRgn = CreatePolygonRgn((POINT*) points, count, ALTERNATE);
	CombineRgn(_W_REGION(region)->handle, _W_REGION(region)->handle, polyRgn,
	RGN_OR);
	DeleteObject(polyRgn);
	return W_TRUE;
}
wresult w_region_add_rectangle(w_region *region, w_rect *rect) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	HRGN rectRgn = CreateRectRgn(rect->x, rect->y, rect->x + rect->width,
			rect->y + rect->height);
	CombineRgn(_W_REGION(region)->handle, _W_REGION(region)->handle, rectRgn,
	RGN_OR);
	DeleteObject(rectRgn);
	return W_TRUE;
}
wresult w_region_add_region(w_region *region, w_region *_region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(_region)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	CombineRgn(_W_REGION(region)->handle, _W_REGION(region)->handle,
	_W_REGION(_region)->handle, RGN_OR);
	return W_TRUE;
}
wresult w_region_contains(w_region *region, w_point *pt) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	return PtInRegion(_W_REGION(region)->handle, pt->x, pt->y);
}
wresult w_region_get_bounds(w_region *region, w_rect *bounds) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	RECT rect;
	GetRgnBox(_W_REGION(region)->handle, &rect);
	bounds->x = rect.left;
	bounds->y = rect.top;
	bounds->width = rect.right - rect.left;
	bounds->height = rect.bottom - rect.top;
	return W_TRUE;
}
wresult w_region_intersect_rect(w_region *region, w_rect *rect) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	HRGN rectRgn = CreateRectRgn(rect->x, rect->y, rect->x + rect->width,
			rect->y + rect->height);
	CombineRgn(_W_REGION(region)->handle, _W_REGION(region)->handle, rectRgn,
	RGN_AND);
	DeleteObject(rectRgn);
	return W_TRUE;
}
wresult w_region_intersect_region(w_region *region, w_region *_region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	CombineRgn(_W_REGION(region)->handle, _W_REGION(region)->handle,
	_W_REGION(_region)->handle, RGN_AND);
	return W_TRUE;
}
wresult w_region_intersects(w_region *region, w_rect *rect) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	RECT r;
	SetRect(&r, rect->x, rect->y, rect->x + rect->width,
			rect->y + rect->height);
	return RectInRegion(_W_REGION(region)->handle, &r);
}
wresult w_region_is_empty(w_region *region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	RECT rect;
	int result = GetRgnBox(_W_REGION(region)->handle, &rect);
	if (result == NULLREGION)
		return W_TRUE;
	return ((rect.right - rect.left) <= 0) || ((rect.bottom - rect.top) <= 0);
}
wresult w_region_subtract_points(w_region *region, w_point *points,
		size_t count) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	HRGN polyRgn = CreatePolygonRgn((POINT*) points, count,
	ALTERNATE);
	CombineRgn(_W_REGION(region)->handle, _W_REGION(region)->handle, polyRgn,
	RGN_DIFF);
	DeleteObject(polyRgn);
	return W_TRUE;
}
wresult w_region_subtract_rectangle(w_region *region, w_rect *rect) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	HRGN rectRgn = CreateRectRgn(rect->x, rect->y, rect->x + rect->width,
			rect->y + rect->height);
	CombineRgn(_W_REGION(region)->handle, _W_REGION(region)->handle, rectRgn,
	RGN_DIFF);
	DeleteObject(rectRgn);
	return W_TRUE;
}
wresult w_region_subtract_region(w_region *region, w_region *_region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	CombineRgn(_W_REGION(region)->handle, _W_REGION(region)->handle,
	_W_REGION(_region)->handle, RGN_DIFF);
	return W_TRUE;
}
wresult w_region_translate(w_region *region, w_point *pt) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	OffsetRgn(_W_REGION(region)->handle, pt->x, pt->y);
	return W_TRUE;
}
