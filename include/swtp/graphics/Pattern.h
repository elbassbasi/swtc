/*
 * Pattern.h
 *
 *  Created on: 15 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_GRAPHICS_PATTERN_H_
#define SWTP_GRAPHICS_PATTERN_H_
#include "Image.h"
/**
 * Instances of this class represent patterns to use while drawing. Patterns
 * can be specified either as bitmaps or gradients.
 * <p>
 * Application code must explicitly invoke the <code>Pattern.dispose()</code>
 * method to release the operating system resources managed by each instance
 * when those instances are no longer required.
 * </p>
 * <p>
 * This class requires the operating system's advanced graphics subsystem
 * which may not be available on some platforms.
 * </p>
 */
class WPattern: public WResource {
public:
	WPattern() {
		w_pattern_init(W_PATTERN(this));
	}
	~WPattern() {
		w_pattern_dispose(W_PATTERN(this));
	}
	void Dispose() {
		w_pattern_dispose(W_PATTERN(this));
	}
	/**
	 * Constructs a new Pattern given an image. Drawing with the resulting
	 * pattern will cause the image to be tiled over the resulting area.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 * <p>
	 * You must dispose the pattern when it is no longer required.
	 * </p>
	 *
	 * @param image the image that the pattern will draw
	 *
	 * @see #dispose()
	 */
	bool Create(WImage &image) {
		return w_pattern_create_from_image(W_PATTERN(this), W_IMAGE(&image)) > 0;
	}
	/**
	 * Constructs a new Pattern that represents a linear, two color
	 * gradient. Drawing with the pattern will cause the resulting area to be
	 * tiled with the gradient specified by the arguments.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 * <p>
	 * You must dispose the pattern when it is no longer required.
	 * </p>
	 *
	 * @param device the device on which to allocate the pattern
	 * @param x1 the x coordinate of the starting corner of the gradient
	 * @param y1 the y coordinate of the starting corner of the gradient
	 * @param x2 the x coordinate of the ending corner of the gradient
	 * @param y2 the y coordinate of the ending corner of the gradient
	 * @param color1 the starting color of the gradient
	 * @param color2 the ending color of the gradient
	 */
	bool Create(float x1, float y1, float x2, float y2, WColor color1,
			WColor color2) {
		return w_pattern_create(W_PATTERN(this), x1, y1, x2, y2, color1.color,
				color2.color) > 0;
	}
	bool IsOk() {
		return w_pattern_is_ok(W_PATTERN(this));
	}
	/**
	 * Returns <code>true</code> if the Pattern has been disposed,
	 * and <code>false</code> otherwise.
	 * <p>
	 * This method gets the dispose state for the Pattern.
	 * When a Pattern has been disposed, it is an error to
	 * invoke any other method (except {@link #dispose()}) using the Pattern.
	 *
	 * @return <code>true</code> when the Pattern is disposed, and <code>false</code> otherwise
	 */
	bool IsDisposed() {
		return !IsOk();
	}
};

#endif /* SWTP_GRAPHICS_PATTERN_H_ */
