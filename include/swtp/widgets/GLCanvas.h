/*
 * GLCanvas.h
 *
 *  Created on: 1 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_GLCANVAS_H_
#define SWTP_WIDGETS_GLCANVAS_H_
#include "Canvas.h"
class WGLCanvas;
/**
 * The WGLData class is a device-independent description
 * of the pixel format attributes of a GL drawable.
 *
 * @see WGLCanvas
 */
class WGLData {
public:
	WGLData() {
		memset(this, 0, sizeof(WGLData));
	}
	/**
	 * Specifies a double-buffered surface.  During context
	 * creation, only double-buffered formats are considered
	 * when set to true.
	 */
	bool doubleBuffer :1;

	/**
	 * Specifies a stereo surface.  During context creation,
	 * only stereo formats are considered when set to true.
	 */
	bool stereo :1;

	/**
	 * The size in bits of the color buffer's red channel.
	 * During context creation, this specifies the minimum
	 * required red bits.
	 */
	int redSize;

	/**
	 * The size in bits of the color buffer's green channel.
	 * During context creation, this specifies the minimum
	 * required green bits.
	 */
	int greenSize;

	/**
	 * The size in bits of the color buffer's blue channel.
	 * During context creation, this specifies the minimum
	 * required blue bits.
	 */
	int blueSize;

	/**
	 * The size in bits of the color buffer's alpha channel.
	 * During context creation, this specifies the minimum
	 * required alpha bits.
	 */
	int alphaSize;

	/**
	 * The size in bits of the depth buffer.  During context
	 * creation, the smallest depth buffer of at least the
	 * specified value is preferred, or zero for no depth
	 * buffer.
	 */
	int depthSize;

	/**
	 * The desired number of stencil bitplanes.  During
	 * context creation, the smallest stencil buffer of at
	 * least the specified value is preferred, or zero for
	 * no stencil buffer.
	 */
	int stencilSize;

	/**
	 * The size in bits of the accumulation buffer's red
	 * channel. During context creation, this specifies the
	 * minimum required red bits.
	 */
	int accumRedSize;

	/**
	 * The size in bits of the accumulation buffer's green
	 * channel. During context creation, this specifies the
	 * minimum required green bits.
	 */
	int accumGreenSize;

	/**
	 * The size in bits of the accumulation buffer's blue
	 * channel. During context creation, this specifies the
	 * minimum required blue bits.
	 */
	int accumBlueSize;

	/**
	 * The size in bits of the accumulation buffer's alpha
	 * channel. During context creation, this specifies the
	 * minimum required alpha bits.
	 */
	int accumAlphaSize;

	/**
	 * The number of multisample buffers used by this context.
	 * During context creation, this specifies the minimum
	 * number of multisample buffers requested.
	 */
	int sampleBuffers;

	/**
	 * The number of samples accepted in the multisample buffer.
	 * During creation, pixel formats with the smallest number of
	 * samples that meets or exceeds the specified minimum number
	 * are preferred.
	 */
	int samples;

	/**
	 * Another GLCanvas whose texture namespace and display lists
	 * should be shared.
	 *
	 * @since 3.5
	 */
	WGLCanvas *shareContext;
};

/**
 * WGLCanvas is a widget capable of displaying OpenGL content.
 *
 * @see GLData
 */
class SWTP_PUBLIC WGLCanvas: public WCanvas {
public:
	WGLCanvas() {

	}
	/**
	 * Create a GLCanvas widget using the attributes described in the GLData
	 * object provided.
	 *
	 * @param parent a composite widget
	 * @param style the bitwise OR'ing of widget styles
	 * @param data the requested attributes of the GLCanvas
	 */
	WGLCanvas(WComposite *parent, wuint64 style, WGLData &data) {
		Create(parent, style, data);
	}
	/**
	 * Create a GLCanvas widget using the attributes described in the GLData
	 * object provided.
	 *
	 * @param parent a composite widget
	 * @param style the bitwise OR'ing of widget styles
	 * @param data the requested attributes of the GLCanvas
	 */
	bool Create(WComposite *parent, wuint64 style, WGLData &data) {
		return Create(0, parent, style, data);
	}
	bool Create(WToolkit *toolkit, WComposite *parent, wuint64 style,
			WGLData &data) {
		wresult ret = Create(parent, style, toolkit);
		if (ret > 0) {
			w_glcanvas_init_gldata(W_GLCANVAS(this), (w_gldata*) &data);
		}
		return ret > 0;
	}
	WResult Create(WComposite *parent, wuint64 style,
				WToolkit *toolkit);
	/**
	 * Returns a GLData object describing the created context.
	 *
	 * @return GLData description of the OpenGL context attributes
	 */
	bool GetGLData(WGLData &data) {
		return w_glcanvas_get_gldata(W_GLCANVAS(this), (w_gldata*) &data) > 0;
	}
	/**
	 * Returns a boolean indicating whether the receiver's OpenGL context
	 * is the current context.
	 *
	 * @return true if the receiver holds the current OpenGL context,
	 * false otherwise
	 */
	bool IsCurrent() {
		return w_glcanvas_is_current(W_GLCANVAS(this));
	}
	/**
	 * Sets the OpenGL context associated with this GLCanvas to be the
	 * current GL context.
	 */
	bool SetCurrent() {
		return w_glcanvas_set_current(W_GLCANVAS(this)) > 0;
	}
	/**
	 * Swaps the front and back color buffers.
	 */
	bool SwapBuffers() {
		return w_glcanvas_swap_buffers(W_GLCANVAS(this)) > 0;
	}

};

#endif /* SWTP_WIDGETS_GLCANVAS_H_ */
