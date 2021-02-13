/*
 * Transform.h
 *
 *  Created on: 17 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_GRAPHICS_TRANSFORM_H_
#define SWTP_GRAPHICS_TRANSFORM_H_
#include "Rect.h"
struct WTransformMatrix {
	float m11;
	float m12;
	float m21;
	float m22;
	float dx;
	float dy;
};
/**
 * Instances of this class represent transformation matrices for
 * points expressed as (x; y) pairs of floating point numbers.
 * <p>
 * Application code must explicitly invoke the <code>Transform.dispose()</code>
 * method to release the operating system resources managed by each instance
 * when those instances are no longer required.
 * </p>
 * <p>
 * This class requires the operating system's advanced graphics subsystem
 * which may not be available on some platforms.
 * </p>
 */
class WTransform: public WResource {
public:
	WTransform() {
		w_transform_init(W_TRANSFORM(this));
	}
	~WTransform() {
		w_transform_dispose(W_TRANSFORM(this));
	}
	void Dispose() {
		w_transform_dispose(W_TRANSFORM(this));
	}
	/**
	 * Constructs a new identity Transform.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 * <p>
	 * You must dispose the transform when it is no longer required.
	 * </p>
	 *
	 * @param device the device on which to allocate the Transform
	 *
	 * @see #dispose()
	 */
	bool Create() {
		return w_transform_create(W_TRANSFORM(this), 0) > 0;
	}

	/**
	 * Constructs a new Transform given an array of elements that represent the
	 * matrix that describes the transformation.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 * <p>
	 * You must dispose the transform when it is no longer required.
	 * </p>
	 *
	 * @param device the device on which to allocate the Transform
	 * @param elements an array of floats that describe the transformation matrix
	 *
	 * @see #dispose()
	 */
	bool Create(WTransformMatrix &matrix) {
		return w_transform_create(W_TRANSFORM(this),
				(w_transformmatrix*) &matrix) > 0;
	}

	/**
	 * Constructs a new Transform given all of the elements that represent the
	 * matrix that describes the transformation.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 * <p>
	 * You must dispose the transform when it is no longer required.
	 * </p>
	 *
	 * @param device the device on which to allocate the Transform
	 * @param m11 the first element of the first row of the matrix
	 * @param m12 the second element of the first row of the matrix
	 * @param m21 the first element of the second row of the matrix
	 * @param m22 the second element of the second row of the matrix
	 * @param dx the third element of the first row of the matrix
	 * @param dy the third element of the second row of the matrix
	 *
	 * @see #dispose()
	 */
	bool Create(float m11, float m12, float m21, float m22, float dx,
			float dy) {
		WTransformMatrix matrix = { m11, m12, m21, m22, dx, dy };
		return Create(matrix);
	}

	/**
	 * Fills the parameter with the values of the transformation matrix
	 * that the receiver represents, in the order {m11, m12, m21, m22, dx, dy}.
	 *
	 * @param elements array to hold the matrix values
	 */
	bool GetElements(WTransformMatrix &elements) {
		return w_transform_get_elements(W_TRANSFORM(this),
				(w_transformmatrix*) &elements) > 0;
	}

	/**
	 * Modifies the receiver such that the matrix it represents becomes the
	 * identity matrix.
	 */
	bool Identity() {
		return w_transform_identity(W_TRANSFORM(this)) > 0;
	}

	/**
	 * Modifies the receiver such that the matrix it represents becomes
	 * the mathematical inverse of the matrix it previously represented.
	 */
	bool Invert() {
		return w_transform_invert(W_TRANSFORM(this)) > 0;
	}

	/**
	 * Returns <code>true</code> if the Transform has been disposed,
	 * and <code>false</code> otherwise.
	 * <p>
	 * This method gets the dispose state for the Transform.
	 * When a Transform has been disposed, it is an error to
	 * invoke any other method (except {@link #dispose()}) using the Transform.
	 *
	 * @return <code>true</code> when the Transform is disposed, and <code>false</code> otherwise
	 */
	bool IsDisposed() {
		return !IsOk();
	}
	bool IsOk() {
		return w_transform_is_ok(W_TRANSFORM(this));
	}

	/**
	 * Returns <code>true</code> if the Transform represents the identity matrix
	 * and false otherwise.
	 *
	 * @return <code>true</code> if the receiver is an identity Transform, and <code>false</code> otherwise
	 */
	bool IsIdentity() {
		return w_transform_is_identity(W_TRANSFORM(this));
	}

	/**
	 * Modifies the receiver such that the matrix it represents becomes the
	 * the result of multiplying the matrix it previously represented by the
	 * argument.
	 *
	 * @param matrix the matrix to multiply the receiver by
	 */
	bool Multiply(WTransformMatrix &matrix) {
		return w_transform_multiply(W_TRANSFORM(this),
				(w_transformmatrix*) &matrix) > 0;
	}

	/**
	 * Modifies the receiver so that it represents a transformation that is
	 * equivalent to its previous transformation rotated by the specified angle.
	 * The angle is specified in degrees and for the identity transform 0 degrees
	 * is at the 3 o'clock position. A positive value indicates a clockwise rotation
	 * while a negative value indicates a counter-clockwise rotation.
	 *
	 * @param angle the angle to rotate the transformation by
	 */
	bool Rotate(float angle) {
		return w_transform_rotate(W_TRANSFORM(this), angle) > 0;
	}

	/**
	 * Modifies the receiver so that it represents a transformation that is
	 * equivalent to its previous transformation scaled by (scaleX, scaleY).
	 *
	 * @param scaleX the amount to scale in the X direction
	 * @param scaleY the amount to scale in the Y direction
	 */
	bool Scale(float scaleX, float scaleY) {
		return w_transform_scale(W_TRANSFORM(this), scaleX, scaleY) > 0;
	}
	/**
	 * Modifies the receiver to represent a new transformation given all of
	 * the elements that represent the matrix that describes that transformation.
	 *
	 * @param m11 the first element of the first row of the matrix
	 * @param m12 the second element of the first row of the matrix
	 * @param m21 the first element of the second row of the matrix
	 * @param m22 the second element of the second row of the matrix
	 * @param dx the third element of the first row of the matrix
	 * @param dy the third element of the second row of the matrix
	 */
	bool SetElements(WTransformMatrix &matrix) {
		return w_transform_set_elements(W_TRANSFORM(this),
				(w_transformmatrix*) &matrix) > 0;
	}
	/**
	 * Modifies the receiver to represent a new transformation given all of
	 * the elements that represent the matrix that describes that transformation.
	 *
	 * @param m11 the first element of the first row of the matrix
	 * @param m12 the second element of the first row of the matrix
	 * @param m21 the first element of the second row of the matrix
	 * @param m22 the second element of the second row of the matrix
	 * @param dx the third element of the first row of the matrix
	 * @param dy the third element of the second row of the matrix
	 */
	bool SetElements(float m11, float m12, float m21, float m22, float dx,
			float dy) {
		WTransformMatrix matrix = { m11, m12, m21, m22, dx, dy };
		return SetElements(matrix);
	}

	/**
	 * Modifies the receiver so that it represents a transformation that is
	 * equivalent to its previous transformation sheared by (shearX, shearY).
	 *
	 * @param shearX the shear factor in the X direction
	 * @param shearY the shear factor in the Y direction
	 */
	bool Shear(float shearX, float shearY) {
		return w_transform_shear(W_TRANSFORM(this), shearX, shearY) > 0;
	}

	/**
	 * Given an array containing points described by alternating x and y values,
	 * modify that array such that each point has been replaced with the result of
	 * applying the transformation represented by the receiver to that point.
	 *
	 * @param pointArray an array of alternating x and y values to be transformed
	 */
	bool Transform(WPointF *points, size_t count) {
		return w_transform_transform(W_TRANSFORM(this), (w_pointf*) points,
				count) > 0;
	}

	/**
	 * Modifies the receiver so that it represents a transformation that is
	 * equivalent to its previous transformation translated by (offsetX, offsetY).
	 *
	 * @param offsetX the distance to translate in the X direction
	 * @param offsetY the distance to translate in the Y direction
	 */
	bool Translate(float offsetX, float offsetY) {
		return w_transform_translate(W_TRANSFORM(this), offsetX, offsetY) > 0;
	}
private:
	void *handle[sizeof(w_transform) / sizeof(void*)];
};

#endif /* SWTP_GRAPHICS_TRANSFORM_H_ */
