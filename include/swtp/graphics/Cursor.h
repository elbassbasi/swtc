/*
 * Cursor.h
 *
 *  Created on: 17 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_GRAPHICS_CURSOR_H_
#define SWTP_GRAPHICS_CURSOR_H_
#include "Image.h"
/**
 * Instances of this class manage operating system resources that
 * specify the appearance of the on-screen pointer. To create a
 * cursor you specify the device and either a simple cursor style
 * describing one of the standard operating system provided cursors
 * or the image and mask data for the desired appearance.
 * <p>
 * Application code must explicitly invoke the <code>Cursor.dispose()</code>
 * method to release the operating system resources managed by each instance
 * when those instances are no longer required.
 * </p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>
 *   CURSOR_ARROW, CURSOR_WAIT, CURSOR_CROSS, CURSOR_APPSTARTING, CURSOR_HELP,
 *   CURSOR_SIZEALL, CURSOR_SIZENESW, CURSOR_SIZENS, CURSOR_SIZENWSE, CURSOR_SIZEWE,
 *   CURSOR_SIZEN, CURSOR_SIZES, CURSOR_SIZEE, CURSOR_SIZEW, CURSOR_SIZENE, CURSOR_SIZESE,
 *   CURSOR_SIZESW, CURSOR_SIZENW, CURSOR_UPARROW, CURSOR_IBEAM, CURSOR_NO, CURSOR_HAND
 * </dd>
 * </dl>
 * <p>
 * Note: Only one of the above styles may be specified.
 * </p>
 */
class SWTP_PUBLIC WCursor: public WResource {
	WCursor() {
		w_cursor_init(W_CURSOR(this));
	}
	~WCursor() {
		w_cursor_dispose(W_CURSOR(this));
	}
	void Dispose() {
		w_cursor_dispose(W_CURSOR(this));
	}
	/**
	 * Constructs a new cursor given a device, image data describing
	 * the desired cursor appearance, and the x and y coordinates of
	 * the <em>hotspot</em> (that is, the point within the area
	 * covered by the cursor which is considered to be where the
	 * on-screen pointer is "pointing").
	 * <p>
	 * You must dispose the cursor when it is no longer required.
	 * </p>
	 *
	 * @param device the device on which to allocate the cursor
	 * @param source the image data for the cursor
	 * @param hotspotX the x coordinate of the cursor's hotspot
	 * @param hotspotY the y coordinate of the cursor's hotspot
	 *
	 * @see #dispose()
	 */
	bool Create(WImage &source, int hotspotX, int hotspotY) {
		return w_cursor_create(W_CURSOR(this), W_IMAGE(&source), hotspotX,
				hotspotY) > 0;
	}

};

#endif /* SWTP_GRAPHICS_CURSOR_H_ */
