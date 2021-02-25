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
		NSObject_release(NSOBJECT(_W_TRANSFORM(transform)->handle));
		_W_TRANSFORM(transform)->handle = 0;
	}
}
wresult w_transform_is_ok(w_transform *transform) {
	return _W_TRANSFORM(transform)->handle != 0;
}
wresult w_transform_create(w_transform *transform, w_transformmatrix *matrix) {
	_W_TRANSFORM(transform)->handle = NSAffineTransform_transform();
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSObject_retain(NSOBJECT(_W_TRANSFORM(transform)->handle));
	if (matrix != 0) {
		w_transform_set_elements(transform, matrix);
	} else {
		w_transform_identity(transform);
	}
	return W_TRUE;
}
wresult w_transform_get_elements(w_transform *transform,
		w_transformmatrix *elements) {
	NSAffineTransformStruct st;
	NSAffineTransform_transformStruct(_W_TRANSFORM(transform)->handle, &st);
	elements->m11 = st.m11;
	elements->m12 = st.m12;
	elements->m21 = st.m21;
	elements->m22 = st.m22;
	elements->dx = st.tX;
	elements->dy = st.tY;
	return W_TRUE;
}
wresult w_transform_identity(w_transform *transform) {
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSAffineTransformStruct st;
	NSAffineTransform_transformStruct(_W_TRANSFORM(transform)->handle, &st);
	st.m11 = 1;
	st.m22 = 1;
	NSAffineTransform_setTransformStruct(_W_TRANSFORM(transform)->handle, &st);
	return W_TRUE;
}
wresult w_transform_invert(w_transform *transform) {
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSAffineTransformStruct st;
	NSAffineTransform_transformStruct(_W_TRANSFORM(transform)->handle, &st);
	if ((st.m11 * st.m22 - st.m12 * st.m21) == 0) {
		return W_ERROR_CANNOT_INVERT_MATRIX;
	}
	NSAffineTransform_invert(_W_TRANSFORM(transform)->handle);
	return W_TRUE;
}
wresult w_transform_is_identity(w_transform *transform) {
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSAffineTransformStruct st;
	NSAffineTransform_transformStruct(_W_TRANSFORM(transform)->handle, &st);
	return st.m11 == 1 && st.m12 == 0 && st.m21 == 0 && st.m22 == 1
			&& st.tX == 0 && st.tY == 0;
}
wresult w_transform_multiply(w_transform *transform, w_transform *matrix) {
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (matrix == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TRANSFORM(matrix)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	NSAffineTransform_prependTransform(_W_TRANSFORM(transform)->handle,
	_W_TRANSFORM(matrix)->handle);
	return W_TRUE;
}
wresult w_transform_rotate(w_transform *transform, float angle) {
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSAffineTransform_rotateByDegrees(_W_TRANSFORM(transform)->handle, angle);
	return W_TRUE;
}
wresult w_transform_scale(w_transform *transform, float scaleX, float scaleY) {
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSAffineTransform_scaleXBy(_W_TRANSFORM(transform)->handle, scaleX, scaleY);
	return W_TRUE;
}
wresult w_transform_set_elements(w_transform *transform,
		w_transformmatrix *matrix) {
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSAffineTransformStruct st;
	st.m11 = matrix->m11;
	st.m12 = matrix->m12;
	st.m21 = matrix->m21;
	st.m22 = matrix->m22;
	st.tX = matrix->dx;
	st.tY = matrix->dy;
	NSAffineTransform_setTransformStruct(_W_TRANSFORM(transform)->handle, &st);
	return W_TRUE;
}
wresult w_transform_shear(w_transform *transform, float shearX, float shearY) {
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSAffineTransformStruct st;
	st.m11 = 1;
	st.m12 = shearX;
	st.m21 = shearY;
	st.m22 = 1;
	st.tX = 0;
	st.tY = 0;
	NSAffineTransform *matrix = NSAffineTransform_transform();
	NSAffineTransform_setTransformStruct(matrix, &st);
	NSAffineTransform_prependTransform(_W_TRANSFORM(transform)->handle, matrix);
	return W_TRUE;
}
wresult w_transform_transform(w_transform *transform, w_pointf *points,
		size_t length) {
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (points == 0)
		return W_ERROR_NULL_ARGUMENT;
	NSPoint point, result;
	for (size_t i = 0; i < length; i++) {
		point.x = points[i].x;
		point.y = points[i].y;
		NSAffineTransform_transformPoint(_W_TRANSFORM(transform)->handle,
				&point, &result);
		points[i].x = result.x;
		points[i].y = result.y;
	}
	return W_TRUE;
}
wresult w_transform_translate(w_transform *transform, float offsetX,
		float offsetY) {
	if (_W_TRANSFORM(transform)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSAffineTransform_translateXBy(_W_TRANSFORM(transform)->handle, offsetX,
			offsetY);
	return W_TRUE;
}
