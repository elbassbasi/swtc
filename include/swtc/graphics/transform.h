/*
 * Name:        transform.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_TRANSFORM_H_
#define SWT_GRAPHICS_TRANSFORM_H_
#include "rect.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_transformmatrix {
	float m11;
	float m12;
	float m21;
	float m22;
	float dx;
	float dy;
} w_transformmatrix;
typedef struct w_transform {
	void *handle[8];
} w_transform;
#define W_TRANSFORM(x) ((w_transform*)x)
SWT_PUBLIC void w_transform_init(w_transform *transform);
SWT_PUBLIC void w_transform_dispose(w_transform *transform);
SWT_PUBLIC wresult w_transform_is_ok(w_transform *transform);
SWT_PUBLIC wresult w_transform_create(w_transform *transform,
		w_transformmatrix *matrix);
SWT_PUBLIC wresult w_transform_get_elements(w_transform *transform,
		w_transformmatrix *elements);
SWT_PUBLIC wresult w_transform_identity(w_transform *transform);
SWT_PUBLIC wresult w_transform_invert(w_transform *transform);
SWT_PUBLIC wresult w_transform_is_identity(w_transform *transform);
SWT_PUBLIC wresult w_transform_multiply(w_transform *transform,
		w_transform *matrix);
SWT_PUBLIC wresult w_transform_rotate(w_transform *transform, float angle);
SWT_PUBLIC wresult w_transform_scale(w_transform *transform, float scaleX,
		float scaleY);
SWT_PUBLIC wresult w_transform_set_elements(w_transform *transform,
		w_transformmatrix *matrix);
SWT_PUBLIC wresult w_transform_shear(w_transform *transform, float shearX,
		float shearY);
SWT_PUBLIC wresult w_transform_transform(w_transform *transform,
		w_pointf *points, size_t count);
SWT_PUBLIC wresult w_transform_translate(w_transform *transform, float offsetX,
		float offsetY);

#ifdef __cplusplus
}
#endif

#endif /* SWT_GRAPHICS_TRANSFORM_H_ */
