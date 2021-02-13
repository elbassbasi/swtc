/*
 * Name:        transform.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
void w_transform_init(w_transform *transform) {
	_W_TRANSFORM(transform)->is_created = 0;
}
void w_transform_dispose(w_transform *transform) {
	if (transform != 0 && _W_TRANSFORM(transform)->is_created != 0) {
		_W_TRANSFORM(transform)->is_created = 0;
	}
}
wresult w_transform_is_ok(w_transform *transform) {
	return transform != 0 && _W_TRANSFORM(transform)->is_created != 0;
}
wresult w_transform_create(w_transform *transform, w_transformmatrix *matrix) {
	cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
	if (matrix != 0) {
		cairo_matrix_init(handle, matrix->m11, matrix->m12, matrix->m21,
				matrix->m22, matrix->dx, matrix->dy);
	} else {
		cairo_matrix_init(handle, 1, 0, 0, 1, 0, 0);
	}
	_W_TRANSFORM(transform)->is_created = 1;
	return W_TRUE;
}
wresult w_transform_get_elements(w_transform *transform,
		w_transformmatrix *elements) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	if (elements == 0)
		return W_ERROR_NULL_ARGUMENT;
	cairo_matrix_t *mat = &_W_TRANSFORM(transform)->handle;
	elements->m11 = mat->xx;
	elements->m12 = mat->yx;
	elements->m21 = mat->xy;
	elements->m22 = mat->yy;
	elements->dx = mat->x0;
	elements->dy = mat->y0;
	return W_TRUE;
}
wresult w_transform_identity(w_transform *transform) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
	cairo_matrix_init(handle, 1, 0, 0, 1, 0, 0);
	return W_TRUE;
}
wresult w_transform_invert(w_transform *transform) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
	if (cairo_matrix_invert(handle) != 0) {
		return W_ERROR_CANNOT_INVERT_MATRIX;
	}
	return W_TRUE;
}
wresult w_transform_is_identity(w_transform *transform) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	cairo_matrix_t *mat = &_W_TRANSFORM(transform)->handle;
	return mat->xx == 1 && mat->yx == 0 && mat->xy == 0 && mat->yy == 1
			&& mat->x0 == 0 && mat->y0 == 0;
}
wresult w_transform_multiply(w_transform *transform,
		w_transformmatrix *matrix) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	if (matrix == 0)
		return W_ERROR_NULL_ARGUMENT;
	cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
	cairo_matrix_t mat;
	cairo_matrix_init(&mat, matrix->m11, matrix->m12, matrix->m21, matrix->m22,
			matrix->dx, matrix->dy);
	cairo_matrix_multiply(handle, &mat, handle);
	return W_TRUE;
}
wresult w_transform_rotate(w_transform *transform, float angle) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
	cairo_matrix_rotate(handle, angle * M_PI / 180);
	return W_TRUE;
}
wresult w_transform_scale(w_transform *transform, float scaleX, float scaleY) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
	cairo_matrix_scale(handle, scaleX, scaleY);
	return W_TRUE;
}
wresult w_transform_set_elements(w_transform *transform,
		w_transformmatrix *matrix) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
	cairo_matrix_init(handle, matrix->m11, matrix->m12, matrix->m21,
			matrix->m22, matrix->dx, matrix->dy);
	return W_TRUE;
}
wresult w_transform_shear(w_transform *transform, float shearX, float shearY) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
	cairo_matrix_t matrix = { 1, shearX, shearY, 1, 0, 0 };
	cairo_matrix_multiply(handle, &matrix, handle);
	return W_TRUE;
}
wresult w_transform_transform(w_transform *transform, w_pointf *points,
		size_t count) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
	if (points == 0)
		return W_ERROR_NULL_ARGUMENT;
	double dx, dy;
	for (int i = 0, j = 0; i < count; i++) {
		dx = points[i].x;
		dy = points[i].y;
		cairo_matrix_transform_point(handle, &dx, &dy);
		points[i].x = dx;
		points[i].y = dy;
	}
	return W_TRUE;
}
wresult w_transform_translate(w_transform *transform, float offsetX,
		float offsetY) {
	if (transform == 0 || _W_TRANSFORM(transform)->is_created == 0)
		return W_ERROR_NO_HANDLES;
	cairo_matrix_t *handle = &_W_TRANSFORM(transform)->handle;
	cairo_matrix_translate(handle, offsetX, offsetY);
	return W_TRUE;
}
