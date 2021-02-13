/*
 * Name:        path.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
void w_path_init(w_path *path) {
}
void w_path_dispose(w_path *path) {
}
wresult w_path_create(w_path *path) {
	return W_FALSE;
}
wresult w_path_create_from_path(w_path *path, w_path *frompath,
		float flatness) {
	return W_FALSE;
}
wresult w_path_create_from_data(w_path *path, size_t length, char *types,
		float *pt) {
	return W_FALSE;
}
wresult w_path_add_arc(w_path *path, w_rectf *rect, float startAngle,
		float arcAngle) {
	return W_FALSE;
}
wresult w_path_add_path(w_path *path, w_path *frompath) {
	return W_FALSE;
}
wresult w_path_add_rectangle(w_path *path, w_rectf *rect) {
	return W_FALSE;
}
wresult w_path_add_string(w_path *path, const char *string, int length, int enc,
		w_pointf *pt, w_font *font) {
	return W_FALSE;
}
wresult w_path_close(w_path *path) {
	return W_FALSE;
}
wresult w_path_contains(w_path *path, w_pointf *pt, w_graphics *gc,
		int outline) {
	return W_FALSE;
}
wresult w_path_cubicto(w_path *path, float cx1, float cy1, float cx2, float cy2,
		float x, float y) {
	return W_FALSE;
}
wresult w_path_get_bounds(w_path *path, w_rectf *bounds) {
	return W_FALSE;
}
wresult w_path_get_current_point(w_path *path, w_pointf *point) {
	return W_FALSE;
}
wresult w_path_get_path_data(w_path *path, w_iterator *data) {
	return W_FALSE;
}
wresult w_path_lineto(w_path *path, float x, float y) {
	return W_FALSE;
}
wresult w_path_is_ok(w_path *path) {
	return W_FALSE;
}
wresult w_path_move_to(w_path *path, float x, float y) {
	return W_FALSE;
}
wresult w_path_quadto(w_path *path, float cx, float cy, float x, float y) {
	return W_FALSE;
}
