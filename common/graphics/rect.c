/*
 * rect.c
 *
 *  Created on: 5 avr. 2020
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
void w_rect_add(w_rect *r, w_rect *rect) {
	int left = r->x < rect->x ? r->x : rect->x;
	int top = r->y < rect->y ? r->y : rect->y;
	int lhs = r->x + r->width;
	int rhs = rect->x + rect->width;
	int right = lhs > rhs ? lhs : rhs;
	lhs = r->y + r->height;
	rhs = rect->y + rect->height;
	int bottom = lhs > rhs ? lhs : rhs;
	r->x = left;
	r->y = top;
	r->width = right - left;
	r->height = bottom - top;
}

wresult w_rect_contains(w_rect *r, int x, int y) {
	return (x >= r->x) && (y >= r->y) && x < (r->x + r->width)
			&& y < (r->y + r->height);
}
wresult w_rect_equals(w_rect *r, w_rect *rect) {
	return (r->x == rect->x) && (r->y == rect->y) && (r->width == rect->width)
			&& (r->height == rect->height);
}
void w_rect_intersect(w_rect *r, w_rect *rect) {
	int left = r->x > rect->x ? r->x : rect->x;
	int top = r->y > rect->y ? r->y : rect->y;
	int lhs = r->x + r->width;
	int rhs = rect->x + rect->width;
	int right = lhs < rhs ? lhs : rhs;
	lhs = r->y + r->height;
	rhs = rect->y + rect->height;
	int bottom = lhs < rhs ? lhs : rhs;
	r->x = right < left ? 0 : left;
	r->y = bottom < top ? 0 : top;
	r->width = right < left ? 0 : right - left;
	r->height = bottom < top ? 0 : bottom - top;
}
void w_rect_intersection(w_rect *result, w_rect *r, w_rect *rect) {
	int left = r->x > rect->x ? r->x : rect->x;
	int top = r->y > rect->y ? r->y : rect->y;
	int lhs = r->x + r->width;
	int rhs = rect->x + rect->width;
	int right = lhs < rhs ? lhs : rhs;
	lhs = r->y + r->height;
	rhs = rect->y + rect->height;
	int bottom = lhs < rhs ? lhs : rhs;
	result->x = right < left ? 0 : left;
	result->y = bottom < top ? 0 : top;
	result->width = right < left ? 0 : right - left;
	result->height = bottom < top ? 0 : bottom - top;
}
wresult w_rect_intersects(w_rect *r, w_rect *rect) {
	return (rect->x < r->x + r->width) && (rect->y < r->y + r->height)
			&& (rect->x + rect->width > r->x) && (rect->y + rect->height > r->y);
}
wresult w_rect_is_empty(w_rect *r) {
	return (r->width <= 0) || (r->height <= 0);
}
void w_rect_union(w_rect *result, w_rect *r, w_rect *rect) {
	result->x = r->x < rect->x ? r->x : rect->x;
	result->y = r->y < rect->y ? r->y : rect->y;
	int lhs = r->x + r->width;
	int rhs = rect->x + rect->width;
	int right = lhs > rhs ? lhs : rhs;
	lhs = r->y + r->height;
	rhs = rect->y + rect->height;
	int bottom = lhs > rhs ? lhs : rhs;
	result->width = right - result->x;
	result->height = bottom - result->y;
}
