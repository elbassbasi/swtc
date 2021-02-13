/*
 * Color.h
 *
 *  Created on: 17 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_GRAPHICS_COLOR_H_
#define SWTP_GRAPHICS_COLOR_H_
#include "Rect.h"
/**
 * Instances of this class manage the operating system resources that
 * implement SWT's RGB color model. To create a color you can either
 * specify the individual color components as integers in the range
 * 0 to 255 or provide an instance of an <code>RGB</code> or <code>RGBA</code>.
 * <p>
 * Application code must explicitly invoke the <code>Color.dispose()</code>
 * method to release the operating system resources managed by each instance
 * when those instances are no longer required.
 * </p>
 *
 * @see RGB
 * @see RGBA
 * @see Device#getSystemColor
 */
class SWTP_PUBLIC WColor {
public:
	w_color color;
	WColor() {
		this->color = W_RGB(0, 0, 0);
	}
	WColor(w_color color) {
		this->color = color;
	}
	/**
	 * Constructs a new instance of this class given a device and the
	 * desired red, green and blue values expressed as ints in the range
	 * 0 to 255 (where 0 is black and 255 is full brightness). On limited
	 * color devices, the color instance created by this call may not have
	 * the same RGB values as the ones specified by the arguments. The
	 * RGB values on the returned instance will be the color values of
	 * the operating system color.
	 * <p>
	 * You must dispose the color when it is no longer required.
	 * </p>
	 *
	 * @param device the device on which to allocate the color
	 * @param red the amount of red in the color
	 * @param green the amount of green in the color
	 * @param blue the amount of blue in the color
	 */
	WColor(int r, int g, int b) {
		Init(r, g, b);
	}
	/**
	 * Constructs a new instance of this class given a device and the
	 * desired red, green, blue & alpha values expressed as ints in the range
	 * 0 to 255 (where 0 is black and 255 is full brightness). On limited
	 * color devices, the color instance created by this call may not have
	 * the same RGB values as the ones specified by the arguments. The
	 * RGB values on the returned instance will be the color values of
	 * the operating system color.
	 * <p>
	 * You must dispose the color when it is no longer required.
	 * </p>
	 *
	 * @param device the device on which to allocate the color
	 * @param red the amount of red in the color
	 * @param green the amount of green in the color
	 * @param blue the amount of blue in the color
	 * @param alpha the amount of alpha in the color. Currently, SWT only honors extreme values for alpha i.e. 0 (transparent) or 255 (opaque).
	 */
	WColor(int r, int g, int b, int a) {
		Init(r, g, b, a);
	}
	/**
	 * Allocates the operating system resources associated
	 * with the receiver.
	 *
	 * @param red the amount of red in the color
	 * @param green the amount of green in the color
	 * @param blue the amount of blue in the color
	 * @param alpha the amount of alpha in the color. Currently, SWT only honors extreme values for alpha i.e. 0 (transparent) or 255 (opaque).
	 */
	void Init(int red, int green, int blue, int alpha) {
		this->color = W_RGBA(red, green, blue, alpha);
	}
	/**
	 * Allocates the operating system resources associated
	 * with the receiver.
	 *
	 * @param red the amount of red in the color
	 * @param green the amount of green in the color
	 * @param blue the amount of blue in the color
	 */
	void Init(int red, int green, int blue) {
		this->color = W_RGB(red, green, blue);
	}
	operator w_color() {
		return this->color;
	}
	/**
	 * Returns the amount of red in the color, from 0 to 255.
	 *
	 * @return the red component of the color
	 */
	int GetRed() {
		return W_RED(this->color);
	}
	/**
	 * Returns the amount of green in the color, from 0 to 255.
	 *
	 * @return the green component of the color
	 */
	int GetGreen() {
		return W_GREEN(this->color);
	}
	/**
	 * Returns the amount of blue in the color, from 0 to 255.
	 *
	 * @return the blue component of the color
	 */
	int GetBlue() {
		return W_BLUE(this->color);
	}
	/**
	 * Returns the amount of alpha in the color, from 0 (transparent) to 255 (opaque).
	 *
	 * @return the alpha component of the color
	 */
	int GetAlpha() {
		return W_ALPHA(this->color);
	}
	/**
	 * Creates a new <code>Color</code> that is a brighter version of this
	 * <code>Color</code>.
	 * <p>
	 * This method applies an arbitrary scale factor to each of the three RGB
	 * components of this <code>Color</code> to create a brighter version
	 * of this <code>Color</code>.
	 * The {@code alpha} value is preserved.
	 * Although <code>brighter</code> and
	 * <code>darker</code> are inverse operations, the results of a
	 * series of invocations of these two methods might be inconsistent
	 * because of rounding errors.
	 * @return     a new <code>Color</code> object that is
	 *                 a brighter version of this <code>Color</code>
	 *                 with the same {@code alpha} value.
	 */
	WColor Brighter() {
		int r = GetRed();
		int g = GetGreen();
		int b = GetBlue();
		int alpha = GetAlpha();

		/* From 2D group:
		 * 1. black.brighter() should return grey
		 * 2. applying brighter to blue will always return blue, brighter
		 * 3. non pure color (non zero rgb) will eventually return white
		 */
		int i = (int) (1.0 / (1.0 - 0.7));
		if (r == 0 && g == 0 && b == 0) {
			return WColor(i, i, i, alpha);
		}
		if (r > 0 && r < i)
			r = i;
		if (g > 0 && g < i)
			g = i;
		if (b > 0 && b < i)
			b = i;

		return WColor(WMIN((int ) (r / 0.7), 255), WMIN((int ) (g / 0.7), 255),
				WMIN((int ) (b / 0.7), 255), alpha);
	}

	/**
	 * Creates a new <code>Color</code> that is a darker version of this
	 * <code>Color</code>.
	 * <p>
	 * This method applies an arbitrary scale factor to each of the three RGB
	 * components of this <code>Color</code> to create a darker version of
	 * this <code>Color</code>.
	 * The {@code alpha} value is preserved.
	 * Although <code>brighter</code> and
	 * <code>darker</code> are inverse operations, the results of a series
	 * of invocations of these two methods might be inconsistent because
	 * of rounding errors.
	 * @return  a new <code>Color</code> object that is
	 *                    a darker version of this <code>Color</code>
	 *                    with the same {@code alpha} value.
	 */
	WColor Darker() {
		return WColor(WMAX((int ) (GetRed() * 0.7), 0),
				WMAX((int ) (GetGreen() * 0.7), 0),
				WMAX((int ) (GetBlue() * 0.7), 0), GetAlpha());
	}
};
#endif /* SWTP_GRAPHICS_COLOR_H_ */
