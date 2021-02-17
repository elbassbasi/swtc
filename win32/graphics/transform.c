/*
 * Name:        transform.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
void w_transform_init(w_transform *transform) {
	_W_TRANSFORM(transform)->handle = 0;
}
void w_transform_dispose(w_transform *transform) {
	if (_W_TRANSFORM(transform)->handle != 0) {
		GdipDeleteMatrix(_W_TRANSFORM(transform)->handle);
		_W_TRANSFORM(transform)->handle = 0;
	}
}
wresult w_transform_is_ok(w_transform *transform) {
	return transform != 0 && _W_TRANSFORM(transform)->handle != 0;
}
wresult w_transform_create(w_transform *transform, w_transformmatrix *matrix) {
	w_transform_dispose(transform);
	GpMatrix *m = 0;
	if (matrix == 0) {
		GdipCreateMatrix2(1, 0, 0, 1, 0, 0, &m);
	} else {
		GdipCreateMatrix2(matrix->m11, matrix->m12, matrix->m21, matrix->m22,
				matrix->dx, matrix->dy, &m);
	}
	if (m == 0)
		return W_ERROR_NO_HANDLES;
	_W_TRANSFORM(transform)->handle = m;
	return W_TRUE;
}
wresult w_transform_get_elements(w_transform *transform,
		w_transformmatrix *elements) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	REAL m[6];
	GdipGetMatrixElements(_W_TRANSFORM(transform)->handle, m);
	elements->m11 = m[0];
	elements->m12 = m[1];
	elements->m21 = m[2];
	elements->m22 = m[3];
	elements->dx = m[4];
	elements->dy = m[5];
	return W_TRUE;
}
wresult w_transform_identity(w_transform *transform) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GdipSetMatrixElements(_W_TRANSFORM(transform)->handle, 1, 0, 0, 1, 0, 0);
	return W_TRUE;
}
wresult w_transform_invert(w_transform *transform) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GdipInvertMatrix(_W_TRANSFORM(transform)->handle);
	return W_TRUE;
}
wresult w_transform_is_identity(w_transform *transform) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	BOOL isIdentity = W_TRUE;
	GdipIsMatrixIdentity(_W_TRANSFORM(transform)->handle, &isIdentity);
	return isIdentity;
}
wresult w_transform_multiply(w_transform *transform, w_transform *matrix) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GdipMultiplyMatrix(_W_TRANSFORM(transform)->handle,
	_W_TRANSFORM(matrix)->handle, MatrixOrderPrepend);
	return W_TRUE;
}
wresult w_transform_rotate(w_transform *transform, float angle) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GdipRotateMatrix(_W_TRANSFORM(transform)->handle, angle,
			MatrixOrderPrepend);
	return W_TRUE;
}
wresult w_transform_scale(w_transform *transform, float scaleX, float scaleY) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GdipScaleMatrix(_W_TRANSFORM(transform)->handle, scaleX, scaleY,
			MatrixOrderPrepend);
	return W_TRUE;
}
wresult w_transform_set_elements(w_transform *transform,
		w_transformmatrix *matrix) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GdipSetMatrixElements(_W_TRANSFORM(transform)->handle, matrix->m11,
			matrix->m12, matrix->m21, matrix->m22, matrix->dx, matrix->dy);
	return W_TRUE;
}
wresult w_transform_shear(w_transform *transform, float shearX, float shearY) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GdipShearMatrix(_W_TRANSFORM(transform)->handle, shearX, shearY,
			MatrixOrderPrepend);
	return W_TRUE;
}
wresult w_transform_transform(w_transform *transform, w_pointf *points,
		size_t count) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GdipTransformMatrixPoints(_W_TRANSFORM(transform)->handle,
			(GpPointF*) points, count);
	return W_TRUE;
}
wresult w_transform_translate(w_transform *transform, float offsetX,
		float offsetY) {
	if (transform == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GdipTranslateMatrix(_W_TRANSFORM(transform)->handle, offsetX, offsetY,
			MatrixOrderPrepend);
	return W_TRUE;
}
