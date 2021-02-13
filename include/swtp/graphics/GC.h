/*
 * Graphics.h
 *
 *  Created on: 24 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_GC_GRAPHICS_H_
#define SWTP_GC_GRAPHICS_H_
#include "ImageList.h"
#include "Cursor.h"
#include "Pattern.h"
#include "Font.h"
#include "Region.h"
#include "Transform.h"
#include "Path.h"
class WLineAttributes;
class WControl;

/**
 * Class <code>GC</code> is where all of the drawing capabilities that are
 * supported by SWT are located. Instances are used to draw on either an
 * <code>Image</code>, a <code>Control</code>, or directly on a <code>Display</code>.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>LEFT_TO_RIGHT, RIGHT_TO_LEFT</dd>
 * </dl>
 *
 * <p>
 * The SWT drawing coordinate system is the two-dimensional space with the origin
 * (0,0) at the top left corner of the drawing area and with (x,y) values increasing
 * to the right and downward respectively.
 * </p>
 *
 * <p>
 * The result of drawing on an image that was created with an indexed
 * palette using a color that is not in the palette is platform specific.
 * Some platforms will match to the nearest color while other will draw
 * the color itself. This happens because the allocated image might use
 * a direct palette on platforms that do not support indexed palette.
 * </p>
 *
 * <p>
 * Application code must explicitly invoke the <code>GC.dispose()</code>
 * method to release the operating system resources managed by each instance
 * when those instances are no longer required. This is <em>particularly</em>
 * important on Windows95 and Windows98 where the operating system has a limited
 * number of device contexts available.
 * </p>
 *
 * <p>
 * Note: Only one of LEFT_TO_RIGHT and RIGHT_TO_LEFT may be specified.
 * </p>
 *
 * @see org.eclipse.swt.events.PaintEvent
 */
class WGraphics: public WResource {
protected:
	void Init() {
		w_graphics_init(W_GRAPHICS(this));
	}
public:
	/**
	 * Prevents uninitialized instances from being created outside the package.
	 */
	WGraphics() {
		Init();
	}
	WGraphics(const WSurface &surface) {
		Init();
		Create(surface);
	}
	WGraphics(const WSurface *surface) {
		Init();
		Create(surface);
	}
	WGraphics(WControl *control) {
		Init();
		Create(control);
	}
	~WGraphics() {
		w_graphics_dispose(W_GRAPHICS(this));
	}
	void Dispose() {
		w_graphics_dispose(W_GRAPHICS(this));
	}
	/**
	 * Constructs a new instance of this class which has been
	 * configured to draw on the specified drawable. Sets the
	 * foreground color, background color and font in the GC
	 * to match those in the drawable.
	 * <p>
	 * You must dispose the graphics context when it is no longer required.
	 * </p>
	 * @see #dispose()
	 */
	bool Create() {
		return w_graphics_create(W_GRAPHICS(this)) > 0;
	}
	/**
	 * Constructs a new instance of this class which has been
	 * configured to draw on the specified drawable. Sets the
	 * foreground color, background color and font in the GC
	 * to match those in the drawable.
	 * <p>
	 * You must dispose the graphics context when it is no longer required.
	 * </p>
	 * @see #dispose()
	 */
	bool Create(WControl *control) {
		return w_control_get_graphics(W_CONTROL(control), W_GRAPHICS(this)) > 0;
	}
	/**
	 * Constructs a new instance of this class which has been
	 * configured to draw on the specified drawable. Sets the
	 * foreground color, background color and font in the GC
	 * to match those in the drawable.
	 * <p>
	 * You must dispose the graphics context when it is no longer required.
	 * </p>
	 * @see #dispose()
	 */
	bool Create(const WSurface &surface) {
		return Create(&surface);
	}
	bool Create(const WSurface *surface) {
		return w_surface_get_graphics(W_SURFACE(surface), W_GRAPHICS(this)) > 0;
	}
	/**
	 * Copies a rectangular area of the receiver at the specified
	 * position into the image, which must be of type <code>SWT.BITMAP</code>.
	 *
	 * @param image the image to copy into
	 * @param x the x coordinate in the receiver of the area to be copied
	 * @param y the y coordinate in the receiver of the area to be copied
	 */
	void CopyArea(WSurface &bitmap, int x, int y) {
		w_graphics_copy_area_to_image(W_GRAPHICS(this), W_SURFACE(&bitmap), x,
				y);
	}
	/**
	 * Copies a rectangular area of the receiver at the source
	 * position onto the receiver at the destination position.
	 *
	 * @param srcX the x coordinate in the receiver of the area to be copied
	 * @param srcY the y coordinate in the receiver of the area to be copied
	 * @param width the width of the area to copy
	 * @param height the height of the area to copy
	 * @param destX the x coordinate in the receiver of the area to copy to
	 * @param destY the y coordinate in the receiver of the area to copy to
	 */
	void CopyArea(int srcX, int srcY, int width, int height, int destX,
			int destY) {
		CopyArea(srcX, srcY, width, height, destX, destY, true);
	}
	/**
	 * Copies a rectangular area of the receiver at the source
	 * position onto the receiver at the destination position.
	 *
	 * @param srcX the x coordinate in the receiver of the area to be copied
	 * @param srcY the y coordinate in the receiver of the area to be copied
	 * @param width the width of the area to copy
	 * @param height the height of the area to copy
	 * @param destX the x coordinate in the receiver of the area to copy to
	 * @param destY the y coordinate in the receiver of the area to copy to
	 * @param paint if <code>true</code> paint events will be generated for old and obscured areas
	 */
	void CopyArea(int srcX, int srcY, int width, int height, int destX,
			int destY, bool paint) {
		w_graphics_copy_area(W_GRAPHICS(this), srcX, srcY, width, height, destX,
				destY, paint);
	}
	/**
	 * Draws the outline of a circular or elliptical arc
	 * within the specified rectangular area.
	 * <p>
	 * The resulting arc begins at <code>startAngle</code> and extends
	 * for <code>arcAngle</code> degrees, using the current color.
	 * Angles are interpreted such that 0 degrees is at the 3 o'clock
	 * position. A positive value indicates a counter-clockwise rotation
	 * while a negative value indicates a clockwise rotation.
	 * </p><p>
	 * The center of the arc is the center of the rectangle whose origin
	 * is (<code>x</code>, <code>y</code>) and whose size is specified by the
	 * <code>width</code> and <code>height</code> arguments.
	 * </p><p>
	 * The resulting arc covers an area <code>width + 1</code> pixels wide
	 * by <code>height + 1</code> pixels tall.
	 * </p>
	 *
	 * @param x the x coordinate of the upper-left corner of the arc to be drawn
	 * @param y the y coordinate of the upper-left corner of the arc to be drawn
	 * @param width the width of the arc to be drawn
	 * @param height the height of the arc to be drawn
	 * @param startAngle the beginning angle
	 * @param arcAngle the angular extent of the arc, relative to the start angle
	 */
	void DrawArc(WRect &rect, int startAngle, int arcAngle) {
		w_graphics_draw_arc(W_GRAPHICS(this), (w_rect*) &rect, startAngle,
				arcAngle);
	}
	/**
	 * Draws the outline of a circular or elliptical arc
	 * within the specified rectangular area.
	 * <p>
	 * The resulting arc begins at <code>startAngle</code> and extends
	 * for <code>arcAngle</code> degrees, using the current color.
	 * Angles are interpreted such that 0 degrees is at the 3 o'clock
	 * position. A positive value indicates a counter-clockwise rotation
	 * while a negative value indicates a clockwise rotation.
	 * </p><p>
	 * The center of the arc is the center of the rectangle whose origin
	 * is (<code>x</code>, <code>y</code>) and whose size is specified by the
	 * <code>width</code> and <code>height</code> arguments.
	 * </p><p>
	 * The resulting arc covers an area <code>width + 1</code> pixels wide
	 * by <code>height + 1</code> pixels tall.
	 * </p>
	 *
	 * @param x the x coordinate of the upper-left corner of the arc to be drawn
	 * @param y the y coordinate of the upper-left corner of the arc to be drawn
	 * @param width the width of the arc to be drawn
	 * @param height the height of the arc to be drawn
	 * @param startAngle the beginning angle
	 * @param arcAngle the angular extent of the arc, relative to the start angle
	 */
	void DrawArc(int x, int y, int width, int height, int startAngle,
			int arcAngle) {
		WRect rect(x, y, width, height);
		DrawArc(rect, startAngle, arcAngle);
	}
	/**
	 * Draws the given image in the receiver at the specified
	 * coordinates.
	 *
	 * @param image the image to draw
	 * @param x the x coordinate of where to draw
	 * @param y the y coordinate of where to draw
	 */
	void DrawImage(WImage &image, WRect &src, WRect &dest) {
		DrawImage(image, src, dest, 0);
	}
	void DrawImage(WImage &image, WRect &src, WRect &dest, int state) {
		w_graphics_draw_image(W_GRAPHICS(this), W_IMAGE(&image), (w_rect*) &src,
				(w_rect*) &dest, state);
	}
	void DrawSurface(WSurface &surface, WRect &src, WRect &dest) {
		w_graphics_draw_surface(W_GRAPHICS(this), W_SURFACE(&surface),
				(w_rect*) &src, (w_rect*) &dest);
	}

	/**
	 * Copies a rectangular area from the source image into a (potentially
	 * different sized) rectangular area in the receiver. If the source
	 * and destination areas are of differing sizes, then the source
	 * area will be stretched or shrunk to fit the destination area
	 * as it is copied. The copy fails if any part of the source rectangle
	 * lies outside the bounds of the source image, or if any of the width
	 * or height arguments are negative.
	 *
	 * @param image the source image
	 * @param srcX the x coordinate in the source image to copy from
	 * @param srcY the y coordinate in the source image to copy from
	 * @param srcWidth the width in points to copy from the source
	 * @param srcHeight the height in points to copy from the source
	 * @param destX the x coordinate in the destination to copy to
	 * @param destY the y coordinate in the destination to copy to
	 * @param destWidth the width in points of the destination rectangle
	 * @param destHeight the height in points of the destination rectangle
	 */
	void DrawImage(WImage &image, int srcX, int srcY, int srcWidth,
			int srcHeight, int destX, int destY, int destWidth,
			int destHeight) {
		WRect src(srcX, srcY, srcWidth, srcHeight);
		WRect dest(destX, destY, destWidth, destHeight);
		DrawImage(image, src, dest);
	}
	void DrawSurface(WSurface &surface, int srcX, int srcY, int srcWidth,
			int srcHeight, int destX, int destY, int destWidth,
			int destHeight) {
		WRect src(srcX, srcY, srcWidth, srcHeight);
		WRect dest(destX, destY, destWidth, destHeight);
		DrawSurface(surface, src, dest);
	}
	/**
	 * Draws the given image in the receiver at the specified
	 * coordinates.
	 *
	 * @param image the image to draw
	 * @param x the x coordinate of where to draw
	 * @param y the y coordinate of where to draw
	 */
	void DrawImage(WImage &image, int x, int y) {
		DrawImage(image, 0, 0, -1, -1, x, y, -1, -1);
	}
	void DrawSurface(WSurface &surface, int x, int y) {
		DrawSurface(surface, 0, 0, -1, -1, x, y, -1, -1);
	}
	/**
	 * Draws a line, using the foreground color, between the points
	 * (<code>x1</code>, <code>y1</code>) and (<code>x2</code>, <code>y2</code>).
	 *
	 * @param x1 the first point's x coordinate
	 * @param y1 the first point's y coordinate
	 * @param x2 the second point's x coordinate
	 * @param y2 the second point's y coordinate
	 */
	void DrawLine(WPoint &pt1, WPoint &pt2) {
		w_graphics_draw_line(W_GRAPHICS(this), (w_point*) &pt1,
				(w_point*) &pt2);
	}
	/**
	 * Draws a line, using the foreground color, between the points
	 * (<code>x1</code>, <code>y1</code>) and (<code>x2</code>, <code>y2</code>).
	 *
	 * @param x1 the first point's x coordinate
	 * @param y1 the first point's y coordinate
	 * @param x2 the second point's x coordinate
	 * @param y2 the second point's y coordinate
	 */
	void DrawLine(int x1, int y1, int x2, int y2) {
		WPoint pt1(x1, y1), pt2(x2, y2);
		DrawLine(pt1, pt2);
	}
	/**
	 * Draws the outline of an oval, using the foreground color,
	 * within the specified rectangular area.
	 * <p>
	 * The result is a circle or ellipse that fits within the
	 * rectangle specified by the <code>x</code>, <code>y</code>,
	 * <code>width</code>, and <code>height</code> arguments.
	 * </p><p>
	 * The oval covers an area that is <code>width + 1</code>
	 * pixels wide and <code>height + 1</code> pixels tall.
	 * </p>
	 *
	 * @param x the x coordinate of the upper left corner of the oval to be drawn
	 * @param y the y coordinate of the upper left corner of the oval to be drawn
	 * @param width the width of the oval to be drawn
	 * @param height the height of the oval to be drawn
	 */
	void DrawOval(WRect &rect) {
		w_graphics_draw_oval(W_GRAPHICS(this), (w_rect*) &rect);
	}
	/**
	 * Draws the outline of an oval, using the foreground color,
	 * within the specified rectangular area.
	 * <p>
	 * The result is a circle or ellipse that fits within the
	 * rectangle specified by the <code>x</code>, <code>y</code>,
	 * <code>width</code>, and <code>height</code> arguments.
	 * </p><p>
	 * The oval covers an area that is <code>width + 1</code>
	 * pixels wide and <code>height + 1</code> pixels tall.
	 * </p>
	 *
	 * @param x the x coordinate of the upper left corner of the oval to be drawn
	 * @param y the y coordinate of the upper left corner of the oval to be drawn
	 * @param width the width of the oval to be drawn
	 * @param height the height of the oval to be drawn
	 */
	void DrawOval(int x, int y, int width, int height) {
		WRect rect(x, y, width, height);
		DrawOval(rect);
	}
	/**
	 * Draws the path described by the parameter.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 *
	 * @param path the path to draw
	 *
	 * @see Path
	 */
	void DrawPath(WPath &path) {
		w_graphics_draw_path(W_GRAPHICS(this), (w_path*) &path);
	}
	/**
	 * Draws a pixel, using the foreground color, at the specified
	 * point (<code>x</code>, <code>y</code>).
	 * <p>
	 * Note that the receiver's line attributes do not affect this
	 * operation.
	 * </p>
	 *
	 * @param x the point's x coordinate
	 * @param y the point's y coordinate
	 */
	void DrawPoint(WPoint &pt) {
		w_graphics_draw_point(W_GRAPHICS(this), (w_point*) &pt);
	}
	/**
	 * Draws a pixel, using the foreground color, at the specified
	 * point (<code>x</code>, <code>y</code>).
	 * <p>
	 * Note that the receiver's line attributes do not affect this
	 * operation.
	 * </p>
	 *
	 * @param x the point's x coordinate
	 * @param y the point's y coordinate
	 */
	void DrawPoint(int x, int y) {
		WPoint pt(x, y);
		DrawPoint(pt);
	}
	/**
	 * Draws the closed polygon which is defined by the specified array
	 * of integer coordinates, using the receiver's foreground color. The array
	 * contains alternating x and y values which are considered to represent
	 * points which are the vertices of the polygon. Lines are drawn between
	 * each consecutive pair, and between the first pair and last pair in the
	 * array.
	 *
	 * @param pointArray an array of alternating x and y values which are the vertices of the polygon
	 */
	void DrawPolygon(WPoint *points, size_t count) {
		w_graphics_draw_polygon(W_GRAPHICS(this), (w_point*) points, count);
	}
	void DrawPolygon(size_t count, ...) {
		va_list list;
		va_start(list, count);
		w_graphics_draw_polygonv(W_GRAPHICS(this), count, list);
		va_end(list);
	}
	/**
	 * Draws the polyline which is defined by the specified array
	 * of integer coordinates, using the receiver's foreground color. The array
	 * contains alternating x and y values which are considered to represent
	 * points which are the corners of the polyline. Lines are drawn between
	 * each consecutive pair, but not between the first pair and last pair in
	 * the array.
	 *
	 * @param pointArray an array of alternating x and y values which are the corners of the polyline
	 */
	void DrawPolyline(WPoint *points, size_t count) {
		w_graphics_draw_polyline(W_GRAPHICS(this), (w_point*) points, count);
	}
	void DrawPolyline(size_t count, ...) {
		va_list list;
		va_start(list, count);
		w_graphics_draw_polylinev(W_GRAPHICS(this), count, list);
		va_end(list);
	}
	/**
	 * Draws the outline of the specified rectangle, using the receiver's
	 * foreground color. The left and right edges of the rectangle are at
	 * <code>rect.x</code> and <code>rect.x + rect.width</code>. The top
	 * and bottom edges are at <code>rect.y</code> and
	 * <code>rect.y + rect.height</code>.
	 *
	 * @param rect the rectangle to draw
	 */
	void DrawRectangle(WRect &rect) {
		w_graphics_draw_rectangle(W_GRAPHICS(this), (w_rect*) &rect);
	}
	/**
	 * Draws the outline of the rectangle specified by the arguments,
	 * using the receiver's foreground color. The left and right edges
	 * of the rectangle are at <code>x</code> and <code>x + width</code>.
	 * The top and bottom edges are at <code>y</code> and <code>y + height</code>.
	 *
	 * @param x the x coordinate of the rectangle to be drawn
	 * @param y the y coordinate of the rectangle to be drawn
	 * @param width the width of the rectangle to be drawn
	 * @param height the height of the rectangle to be drawn
	 */
	void DrawRectangle(int x, int y, int width, int height) {
		WRect rect(x, y, width, height);
		DrawRectangle(rect);
	}
	/**
	 * Draws the outline of the round-cornered rectangle specified by
	 * the arguments, using the receiver's foreground color. The left and
	 * right edges of the rectangle are at <code>x</code> and <code>x + width</code>.
	 * The top and bottom edges are at <code>y</code> and <code>y + height</code>.
	 * The <em>roundness</em> of the corners is specified by the
	 * <code>arcWidth</code> and <code>arcHeight</code> arguments, which
	 * are respectively the width and height of the ellipse used to draw
	 * the corners.
	 *
	 * @param x the x coordinate of the rectangle to be drawn
	 * @param y the y coordinate of the rectangle to be drawn
	 * @param width the width of the rectangle to be drawn
	 * @param height the height of the rectangle to be drawn
	 * @param arcWidth the width of the arc
	 * @param arcHeight the height of the arc
	 */
	void DrawRoundRectangle(WRect &rect, int arcWidth, int arcHeight) {
		w_graphics_draw_roundrectangle(W_GRAPHICS(this), (w_rect*) &rect,
				arcWidth, arcHeight);
	}
	/**
	 * Draws the outline of the round-cornered rectangle specified by
	 * the arguments, using the receiver's foreground color. The left and
	 * right edges of the rectangle are at <code>x</code> and <code>x + width</code>.
	 * The top and bottom edges are at <code>y</code> and <code>y + height</code>.
	 * The <em>roundness</em> of the corners is specified by the
	 * <code>arcWidth</code> and <code>arcHeight</code> arguments, which
	 * are respectively the width and height of the ellipse used to draw
	 * the corners.
	 *
	 * @param x the x coordinate of the rectangle to be drawn
	 * @param y the y coordinate of the rectangle to be drawn
	 * @param width the width of the rectangle to be drawn
	 * @param height the height of the rectangle to be drawn
	 * @param arcWidth the width of the arc
	 * @param arcHeight the height of the arc
	 */
	void DrawRoundRectangle(int x, int y, int width, int height, int arcWidth,
			int arcHeight) {
		WRect rect(x, y, width, height);
		DrawRoundRectangle(rect, arcWidth, arcHeight);
	}
	/**
	 * Draws the given string, using the receiver's current font and
	 * foreground color. No tab expansion or carriage return processing
	 * will be performed. The background of the rectangular area where
	 * the string is being drawn will be filled with the receiver's
	 * background color.
	 *
	 * @param string the string to be drawn
	 * @param x the x coordinate of the top left corner of the rectangular area where the string is to be drawn
	 * @param y the y coordinate of the top left corner of the rectangular area where the string is to be drawn
	 */
	void DrawString(const char *string, int x, int y) {
		DrawText(string, x, y, 0);
	}
	/**
	 * Draws the given string, using the receiver's current font and
	 * foreground color. No tab expansion or carriage return processing
	 * will be performed. If <code>isTransparent</code> is <code>true</code>,
	 * then the background of the rectangular area where the string is being
	 * drawn will not be modified, otherwise it will be filled with the
	 * receiver's background color.
	 *
	 * @param string the string to be drawn
	 * @param x the x coordinate of the top left corner of the rectangular area where the string is to be drawn
	 * @param y the y coordinate of the top left corner of the rectangular area where the string is to be drawn
	 * @param isTransparent if <code>true</code> the background will be transparent, otherwise it will be opaque
	 */
	void DrawStringTransparent(const char *string, int x, int y) {
		DrawText(string, x, y, W_DRAW_TRANSPARENT);
	}
	/**
	 * Draws the given string, using the receiver's current font and
	 * foreground color. Tab expansion and carriage return processing
	 * are performed. The background of the rectangular area where
	 * the text is being drawn will be filled with the receiver's
	 * background color.
	 *
	 * @param string the string to be drawn
	 * @param x the x coordinate of the top left corner of the rectangular area where the text is to be drawn
	 * @param y the y coordinate of the top left corner of the rectangular area where the text is to be drawn
	 */
	void DrawText(const char *string, int x, int y) {
		DrawText(string, x, y, W_DRAW_DELIMITER | W_DRAW_TAB);
	}
	/**
	 * Draws the given string, using the receiver's current font and
	 * foreground color. Tab expansion and carriage return processing
	 * are performed. The background of the rectangular area where
	 * the text is being drawn will be filled with the receiver's
	 * background color.
	 *
	 * @param string the string to be drawn
	 * @param x the x coordinate of the top left corner of the rectangular area where the text is to be drawn
	 * @param y the y coordinate of the top left corner of the rectangular area where the text is to be drawn
	 */
	void DrawTextTransparent(const char *string, int x, int y) {
		DrawText(string, x, y,
				W_DRAW_DELIMITER | W_DRAW_TAB | W_DRAW_TRANSPARENT);
	}
	/**
	 * Draws the given string, using the receiver's current font and
	 * foreground color. Tab expansion, line delimiter and mnemonic
	 * processing are performed according to the specified flags. If
	 * <code>flags</code> includes <code>DRAW_TRANSPARENT</code>,
	 * then the background of the rectangular area where the text is being
	 * drawn will not be modified, otherwise it will be filled with the
	 * receiver's background color.
	 * <p>
	 * The parameter <code>flags</code> may be a combination of:
	 * <dl>
	 * <dt><b>DRAW_DELIMITER</b></dt>
	 * <dd>draw multiple lines</dd>
	 * <dt><b>DRAW_TAB</b></dt>
	 * <dd>expand tabs</dd>
	 * <dt><b>DRAW_MNEMONIC</b></dt>
	 * <dd>underline the mnemonic character</dd>
	 * <dt><b>DRAW_TRANSPARENT</b></dt>
	 * <dd>transparent background</dd>
	 * </dl>
	 * </p>
	 *
	 * @param string the string to be drawn
	 * @param x the x coordinate of the top left corner of the rectangular area where the text is to be drawn
	 * @param y the y coordinate of the top left corner of the rectangular area where the text is to be drawn
	 * @param flags the flags specifying how to process the text
	 */
	void DrawText(const char *string, int x, int y, int flags) {
		DrawText(string, -1, x, y, flags);
	}
	/**
	 * Draws the given string, using the receiver's current font and
	 * foreground color. Tab expansion, line delimiter and mnemonic
	 * processing are performed according to the specified flags. If
	 * <code>flags</code> includes <code>DRAW_TRANSPARENT</code>,
	 * then the background of the rectangular area where the text is being
	 * drawn will not be modified, otherwise it will be filled with the
	 * receiver's background color.
	 * <p>
	 * The parameter <code>flags</code> may be a combination of:
	 * <dl>
	 * <dt><b>DRAW_DELIMITER</b></dt>
	 * <dd>draw multiple lines</dd>
	 * <dt><b>DRAW_TAB</b></dt>
	 * <dd>expand tabs</dd>
	 * <dt><b>DRAW_MNEMONIC</b></dt>
	 * <dd>underline the mnemonic character</dd>
	 * <dt><b>DRAW_TRANSPARENT</b></dt>
	 * <dd>transparent background</dd>
	 * </dl>
	 * </p>
	 *
	 * @param string the string to be drawn
	 * @param x the x coordinate of the top left corner of the rectangular area where the text is to be drawn
	 * @param y the y coordinate of the top left corner of the rectangular area where the text is to be drawn
	 * @param flags the flags specifying how to process the text
	 */
	void DrawText(const char *string, size_t length, int x, int y, int flags) {
		DrawText(string, length, WRect(x, y, -1, -1), flags);
	}
	/**
	 * Draws the given string, using the receiver's current font and
	 * foreground color. Tab expansion, line delimiter and mnemonic
	 * processing are performed according to the specified flags. If
	 * <code>flags</code> includes <code>DRAW_TRANSPARENT</code>,
	 * then the background of the rectangular area where the text is being
	 * drawn will not be modified, otherwise it will be filled with the
	 * receiver's background color.
	 * <p>
	 * The parameter <code>flags</code> may be a combination of:
	 * <dl>
	 * <dt><b>DRAW_DELIMITER</b></dt>
	 * <dd>draw multiple lines</dd>
	 * <dt><b>DRAW_TAB</b></dt>
	 * <dd>expand tabs</dd>
	 * <dt><b>DRAW_MNEMONIC</b></dt>
	 * <dd>underline the mnemonic character</dd>
	 * <dt><b>DRAW_TRANSPARENT</b></dt>
	 * <dd>transparent background</dd>
	 * </dl>
	 * </p>
	 *
	 * @param string the string to be drawn
	 * @param x the x coordinate of the top left corner of the rectangular area where the text is to be drawn
	 * @param y the y coordinate of the top left corner of the rectangular area where the text is to be drawn
	 * @param flags the flags specifying how to process the text
	 */
	void DrawText(const char *string, size_t length, const WRect &rect,
			int flags) {
		w_graphics_draw_text(W_GRAPHICS(this), string, length, (w_rect*) &rect,
				flags, W_ENCODING_UTF8);
	}
	/**
	 * Fills the interior of a circular or elliptical arc within
	 * the specified rectangular area, with the receiver's background
	 * color.
	 * <p>
	 * The resulting arc begins at <code>startAngle</code> and extends
	 * for <code>arcAngle</code> degrees, using the current color.
	 * Angles are interpreted such that 0 degrees is at the 3 o'clock
	 * position. A positive value indicates a counter-clockwise rotation
	 * while a negative value indicates a clockwise rotation.
	 * </p><p>
	 * The center of the arc is the center of the rectangle whose origin
	 * is (<code>x</code>, <code>y</code>) and whose size is specified by the
	 * <code>width</code> and <code>height</code> arguments.
	 * </p><p>
	 * The resulting arc covers an area <code>width + 1</code> pixels wide
	 * by <code>height + 1</code> pixels tall.
	 * </p>
	 *
	 * @param x the x coordinate of the upper-left corner of the arc to be filled
	 * @param y the y coordinate of the upper-left corner of the arc to be filled
	 * @param width the width of the arc to be filled
	 * @param height the height of the arc to be filled
	 * @param startAngle the beginning angle
	 * @param arcAngle the angular extent of the arc, relative to the start angle
	 *
	 * @see #drawArc
	 */
	void FillArc(WRect &rect, int startAngle, int arcAngle) {
		w_graphics_fill_arc(W_GRAPHICS(this), (w_rect*) &rect, startAngle,
				arcAngle);
	}
	/**
	 * Fills the interior of a circular or elliptical arc within
	 * the specified rectangular area, with the receiver's background
	 * color.
	 * <p>
	 * The resulting arc begins at <code>startAngle</code> and extends
	 * for <code>arcAngle</code> degrees, using the current color.
	 * Angles are interpreted such that 0 degrees is at the 3 o'clock
	 * position. A positive value indicates a counter-clockwise rotation
	 * while a negative value indicates a clockwise rotation.
	 * </p><p>
	 * The center of the arc is the center of the rectangle whose origin
	 * is (<code>x</code>, <code>y</code>) and whose size is specified by the
	 * <code>width</code> and <code>height</code> arguments.
	 * </p><p>
	 * The resulting arc covers an area <code>width + 1</code> pixels wide
	 * by <code>height + 1</code> pixels tall.
	 * </p>
	 *
	 * @param x the x coordinate of the upper-left corner of the arc to be filled
	 * @param y the y coordinate of the upper-left corner of the arc to be filled
	 * @param width the width of the arc to be filled
	 * @param height the height of the arc to be filled
	 * @param startAngle the beginning angle
	 * @param arcAngle the angular extent of the arc, relative to the start angle
	 *
	 * @see #drawArc
	 */
	void FillArc(int x, int y, int width, int height, int startAngle,
			int arcAngle) {
		WRect rect(x, y, width, height);
		FillArc(rect, startAngle, arcAngle);
	}
	/**
	 * Fills the interior of the specified rectangle with a gradient
	 * sweeping from left to right or top to bottom progressing
	 * from the receiver's foreground color to its background color.
	 *
	 * @param x the x coordinate of the rectangle to be filled
	 * @param y the y coordinate of the rectangle to be filled
	 * @param width the width of the rectangle to be filled, may be negative
	 *        (inverts direction of gradient if horizontal)
	 * @param height the height of the rectangle to be filled, may be negative
	 *        (inverts direction of gradient if vertical)
	 * @param vertical if true sweeps from top to bottom, else
	 *        sweeps from left to right
	 *
	 * @see #drawRectangle(int, int, int, int)
	 */
	void FillGradientRectangle(WRect &rect, bool vertical) {
		w_graphics_fill_gradientrectangle(W_GRAPHICS(this), (w_rect*) &rect,
				vertical);
	}
	/**
	 * Fills the interior of the specified rectangle with a gradient
	 * sweeping from left to right or top to bottom progressing
	 * from the receiver's foreground color to its background color.
	 *
	 * @param x the x coordinate of the rectangle to be filled
	 * @param y the y coordinate of the rectangle to be filled
	 * @param width the width of the rectangle to be filled, may be negative
	 *        (inverts direction of gradient if horizontal)
	 * @param height the height of the rectangle to be filled, may be negative
	 *        (inverts direction of gradient if vertical)
	 * @param vertical if true sweeps from top to bottom, else
	 *        sweeps from left to right
	 *
	 * @see #drawRectangle(int, int, int, int)
	 */
	void FillGradientRectangle(int x, int y, int width, int height,
			bool vertical) {
		WRect rect(x, y, width, height);
		FillGradientRectangle(rect, vertical);
	}
	/**
	 * Fills the interior of an oval, within the specified
	 * rectangular area, with the receiver's background
	 * color.
	 *
	 * @param x the x coordinate of the upper left corner of the oval to be filled
	 * @param y the y coordinate of the upper left corner of the oval to be filled
	 * @param width the width of the oval to be filled
	 * @param height the height of the oval to be filled
	 *
	 * @see #drawOval
	 */
	void FillOval(WRect &rect) {
		w_graphics_fill_oval(W_GRAPHICS(this), (w_rect*) &rect);
	}
	/**
	 * Fills the interior of an oval, within the specified
	 * rectangular area, with the receiver's background
	 * color.
	 *
	 * @param x the x coordinate of the upper left corner of the oval to be filled
	 * @param y the y coordinate of the upper left corner of the oval to be filled
	 * @param width the width of the oval to be filled
	 * @param height the height of the oval to be filled
	 *
	 * @see #drawOval
	 */
	void FillOval(int x, int y, int width, int height) {
		WRect rect(x, y, width, height);
		FillOval(rect);
	}
	/**
	 * Fills the path described by the parameter.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 *
	 * @param path the path to fill
	 *
	 * @see Path
	 */
	void FillPath(WPath &path) {
		w_graphics_fill_path(W_GRAPHICS(this), (w_path*) &path);
	}
	/**
	 * Fills the interior of the closed polygon which is defined by the
	 * specified array of integer coordinates, using the receiver's
	 * background color. The array contains alternating x and y values
	 * which are considered to represent points which are the vertices of
	 * the polygon. Lines are drawn between each consecutive pair, and
	 * between the first pair and last pair in the array.
	 *
	 * @param pointArray an array of alternating x and y values which are the vertices of the polygon
	 *
	 * @see #drawPolygon
	 */
	void FillPolygon(WPoint *points, size_t count) {
		w_graphics_fill_polygon(W_GRAPHICS(this), (w_point*) points, count);
	}
	void FillPolygon(size_t count, ...) {
		va_list list;
		va_start(list, count);
		w_graphics_fill_polygonv(W_GRAPHICS(this), count, list);
		va_end(list);
	}
	/**
	 * Fills the interior of the rectangle specified by the arguments,
	 * using the receiver's background color.
	 *
	 * @param x the x coordinate of the rectangle to be filled
	 * @param y the y coordinate of the rectangle to be filled
	 * @param width the width of the rectangle to be filled
	 * @param height the height of the rectangle to be filled
	 *
	 * @see #drawRectangle(int, int, int, int)
	 */
	void FillRectangle(int x, int y, int width, int height) {
		WRect rect(x, y, width, height);
		FillRectangle(rect);
	}
	/**
	 * Fills the interior of the specified rectangle, using the receiver's
	 * background color.
	 *
	 * @param rect the rectangle to be filled
	 *
	 * @see #drawRectangle(int, int, int, int)
	 */
	void FillRectangle(const WRect &rect) {
		w_graphics_fill_rectangle(W_GRAPHICS(this), (w_rect*) &rect);
	}
	/**
	 * Fills the interior of the round-cornered rectangle specified by
	 * the arguments, using the receiver's background color.
	 *
	 * @param x the x coordinate of the rectangle to be filled
	 * @param y the y coordinate of the rectangle to be filled
	 * @param width the width of the rectangle to be filled
	 * @param height the height of the rectangle to be filled
	 * @param arcWidth the width of the arc
	 * @param arcHeight the height of the arc
	 *
	 * @see #drawRoundRectangle
	 */
	void FillRoundRectangle(WRect &rect, int arcWidth, int arcHeight) {
		w_graphics_fill_roundrectangle(W_GRAPHICS(this), (w_rect*) &rect,
				arcWidth, arcHeight);
	}
	/**
	 * Fills the interior of the round-cornered rectangle specified by
	 * the arguments, using the receiver's background color.
	 *
	 * @param x the x coordinate of the rectangle to be filled
	 * @param y the y coordinate of the rectangle to be filled
	 * @param width the width of the rectangle to be filled
	 * @param height the height of the rectangle to be filled
	 * @param arcWidth the width of the arc
	 * @param arcHeight the height of the arc
	 *
	 * @see #drawRoundRectangle
	 */
	void FillRoundRectangle(int x, int y, int width, int height, int arcWidth,
			int arcHeight) {
		WRect rect(x, y, width, height);
		FillRoundRectangle(rect, arcWidth, arcHeight);
	}
	/**
	 * Returns the <em>advance width</em> of the specified character in
	 * the font which is currently selected into the receiver.
	 * <p>
	 * The advance width is defined as the horizontal distance the cursor
	 * should move after printing the character in the selected font.
	 * </p>
	 *
	 * @param ch the character to measure
	 * @return the distance in the x direction to move past the character before painting the next
	 */
	int GetAdvanceWidth(int ch) {
		return w_graphics_get_advance_width(W_GRAPHICS(this), ch);
	}
	/**
	 * Returns <code>true</code> if receiver is using the operating system's
	 * advanced graphics subsystem.  Otherwise, <code>false</code> is returned
	 * to indicate that normal graphics are in use.
	 * <p>
	 * Advanced graphics may not be installed for the operating system.  In this
	 * case, <code>false</code> is always returned.  Some operating system have
	 * only one graphics subsystem.  If this subsystem supports advanced graphics,
	 * then <code>true</code> is always returned.  If any graphics operation such
	 * as alpha, antialias, patterns, interpolation, paths, clipping or transformation
	 * has caused the receiver to switch from regular to advanced graphics mode,
	 * <code>true</code> is returned.  If the receiver has been explicitly switched
	 * to advanced mode and this mode is supported, <code>true</code> is returned.
	 * </p>
	 *
	 * @return the advanced value
	 *
	 * @see #setAdvanced
	 */
	bool GetAdvanced() {
		return w_graphics_get_advanced(W_GRAPHICS(this));
	}
	/**
	 * Returns the receiver's alpha value. The alpha value
	 * is between 0 (transparent) and 255 (opaque).
	 *
	 * @return the alpha value
	 */
	int GetAlpha() {
		return w_graphics_get_alpha(W_GRAPHICS(this));
	}
	/**
	 * Returns the receiver's anti-aliasing setting value, which will be
	 * one of <code>SWT.DEFAULT</code>, <code>SWT.OFF</code> or
	 * <code>SWT.ON</code>. Note that this controls anti-aliasing for all
	 * <em>non-text drawing</em> operations.
	 *
	 * @return the anti-aliasing setting
	 *
	 * @see #getTextAntialias
	 */
	int GetAntialias() {
		return w_graphics_get_antialias(W_GRAPHICS(this));
	}
	/**
	 * Returns the background color.
	 *
	 * @return the receiver's background color
	 */
	WColor GetBackground() {
		return w_graphics_get_background(W_GRAPHICS(this));
	}
	/**
	 * Returns the background pattern. The default value is
	 * <code>null</code>.
	 *
	 * @return the receiver's background pattern
	 *
	 * @see Pattern
	 */
	WPattern* GetBackgroundPattern() {
		return (WPattern*) w_graphics_get_background_pattern(W_GRAPHICS(this));
	}
	/**
	 * Returns the width of the specified character in the font
	 * selected into the receiver.
	 * <p>
	 * The width is defined as the space taken up by the actual
	 * character, not including the leading and tailing whitespace
	 * or overhang.
	 * </p>
	 *
	 * @param ch the character to measure
	 * @return the width of the character
	 */
	int GetCharWidth(int ch) {
		return w_graphics_get_char_width(W_GRAPHICS(this), ch);
	}

	/**
	 * Returns the bounding rectangle of the receiver's clipping
	 * region. If no clipping region is set, the return value
	 * will be a rectangle which covers the entire bounds of the
	 * object the receiver is drawing on.
	 *
	 * @return the bounding rectangle of the clipping region
	 */
	WRect& GetClipping(WRect &rect) {
		w_graphics_get_clipping_rect(W_GRAPHICS(this), (w_rect*) &rect);
		return rect;
	}

	/**
	 * Returns the bounding rectangle of the receiver's clipping
	 * region. If no clipping region is set, the return value
	 * will be a rectangle which covers the entire bounds of the
	 * object the receiver is drawing on.
	 *
	 * @return the bounding rectangle of the clipping region
	 */
	WRect& GetClipping(const WRect &rect = WRect()) {
		return GetClipping((WRect&) rect);
	}
	/**
	 * Sets the region managed by the argument to the current
	 * clipping region of the receiver.
	 *
	 * @param region the region to fill with the clipping region
	 */
	void GetClipping(WRegion &region) {
		w_graphics_get_clipping_region(W_GRAPHICS(this), (w_region*) &region);
	}
	/**
	 * Returns the receiver's fill rule, which will be one of
	 * <code>SWT.FILL_EVEN_ODD</code> or <code>SWT.FILL_WINDING</code>.
	 *
	 * @return the receiver's fill rule
	 */
	int GetFillRule() {
		return w_graphics_get_fill_rule(W_GRAPHICS(this));
	}
	/**
	 * Returns the font currently being used by the receiver
	 * to draw and measure text.
	 *
	 * @return the receiver's font
	 */
	WFont* GetFont() {
		return (WFont*) w_graphics_get_font(W_GRAPHICS(this));
	}
	/**
	 * Returns a FontMetrics which contains information
	 * about the font currently being used by the receiver
	 * to draw and measure text.
	 *
	 * @return font metrics for the receiver's font
	 */
	void GetFontMetrics(WFontMetrics &fontMetrics) {
		w_graphics_get_font_metrics(W_GRAPHICS(this),
				(w_fontmetrics*) &fontMetrics);
	}
	/**
	 * Returns the receiver's foreground color.
	 *
	 * @return the color used for drawing foreground things
	 */
	WColor GetForeground() {
		return w_graphics_get_foreground(W_GRAPHICS(this));
	}
	/**
	 * Returns the foreground pattern. The default value is
	 * <code>null</code>.
	 *
	 * @return the receiver's foreground pattern
	 *
	 * @see Pattern
	 */
	WPattern* GetForegroundPattern() {
		return (WPattern*) w_graphics_get_foreground_pattern(W_GRAPHICS(this));
	}
	/**
	 * Returns the receiver's interpolation setting, which will be one of
	 * <code>SWT.DEFAULT</code>, <code>SWT.NONE</code>,
	 * <code>SWT.LOW</code> or <code>SWT.HIGH</code>.
	 *
	 * @return the receiver's interpolation setting
	 */
	int GetInterpolation() {
		return w_graphics_get_interpolation(W_GRAPHICS(this));
	}
	/**
	 * Returns the receiver's line attributes.
	 *
	 * @return the line attributes used for drawing lines
	 */
	WLineAttributes* GetLineAttributes() {
		return (WLineAttributes*) w_graphics_get_line_attributes(
				W_GRAPHICS(this));
	}
	/**
	 * Returns the receiver's line cap style, which will be one
	 * of the constants <code>SWT.CAP_FLAT</code>, <code>SWT.CAP_ROUND</code>,
	 * or <code>SWT.CAP_SQUARE</code>.
	 *
	 * @return the cap style used for drawing lines
	 */
	int GetLineCap() {
		return w_graphics_get_line_cap(W_GRAPHICS(this));
	}
	/**
	 * Returns the receiver's line dash style. The default value is
	 * <code>null</code>.
	 *
	 * @return the line dash style used for drawing lines
	 */
	bool GetLineDash(int **dashes, int *length) {
		return w_graphics_get_line_dash(W_GRAPHICS(this), dashes, length) > 0;
	}
	/**
	 * Returns the receiver's line join style, which will be one
	 * of the constants <code>SWT.JOIN_MITER</code>, <code>SWT.JOIN_ROUND</code>,
	 * or <code>SWT.JOIN_BEVEL</code>.
	 *
	 * @return the join style used for drawing lines
	 */
	int GetLineJoin() {
		return w_graphics_get_line_join(W_GRAPHICS(this));
	}
	/**
	 * Returns the receiver's line style, which will be one
	 * of the constants <code>SWT.LINE_SOLID</code>, <code>SWT.LINE_DASH</code>,
	 * <code>SWT.LINE_DOT</code>, <code>SWT.LINE_DASHDOT</code> or
	 * <code>SWT.LINE_DASHDOTDOT</code>.
	 *
	 * @return the style used for drawing lines
	 */
	int GetLineStyle() {
		return w_graphics_get_line_style(W_GRAPHICS(this));
	}
	/**
	 * Returns the width that will be used when drawing lines
	 * for all of the figure drawing operations (that is,
	 * <code>drawLine</code>, <code>drawRectangle</code>,
	 * <code>drawPolyline</code>, and so forth.
	 *
	 * @return the receiver's line width
	 */
	int GetLineWidth() {
		return w_graphics_get_line_width(W_GRAPHICS(this));
	}
	/**
	 * Returns the receiver's style information.
	 * <p>
	 * Note that the value which is returned by this method <em>may
	 * not match</em> the value which was provided to the constructor
	 * when the receiver was created. This can occur when the underlying
	 * operating system does not support a particular combination of
	 * requested styles.
	 * </p>
	 *
	 * @return the style bits
	 */
	int GetStyle() {
		return w_graphics_get_style(W_GRAPHICS(this));
	}
	/**
	 * Returns the receiver's text drawing anti-aliasing setting value,
	 * which will be one of <code>SWT.DEFAULT</code>, <code>SWT.OFF</code> or
	 * <code>SWT.ON</code>. Note that this controls anti-aliasing
	 * <em>only</em> for text drawing operations.
	 *
	 * @return the anti-aliasing setting
	 *
	 * @see #getAntialias
	 */
	int GetTextAntialias() {
		return w_graphics_get_text_antialias(W_GRAPHICS(this));
	}
	/**
	 * Sets the parameter to the transform that is currently being
	 * used by the receiver.
	 *
	 * @param transform the destination to copy the transform into
	 *
	 * @see Transform
	 */
	void GetTransform(WTransform &transform) {
		w_graphics_get_transform(W_GRAPHICS(this), (w_transform*) &transform);
	}
	/**
	 * Returns <code>true</code> if this GC is drawing in the mode
	 * where the resulting color in the destination is the
	 * <em>exclusive or</em> of the color values in the source
	 * and the destination, and <code>false</code> if it is
	 * drawing in the mode where the destination color is being
	 * replaced with the source color value.
	 *
	 * @return <code>true</code> true if the receiver is in XOR mode, and false otherwise
	 */
	bool GetXORMode() {
		return w_graphics_get_xor_mode(W_GRAPHICS(this));
	}
	/**
	 * Returns <code>true</code> if the receiver has a clipping
	 * region set into it, and <code>false</code> otherwise.
	 * If this method returns false, the receiver will draw on all
	 * available space in the destination. If it returns true,
	 * it will draw only in the area that is covered by the region
	 * that can be accessed with <code>getClipping(region)</code>.
	 *
	 * @return <code>true</code> if the GC has a clipping region, and <code>false</code> otherwise
	 */
	bool IsClipped() {
		return w_graphics_is_clipped(W_GRAPHICS(this));
	}
	/**
	 * Returns <code>true</code> if the GC has been disposed,
	 * and <code>false</code> otherwise.
	 * <p>
	 * This method gets the dispose state for the GC.
	 * When a GC has been disposed, it is an error to
	 * invoke any other method (except {@link #dispose()}) using the GC.
	 *
	 * @return <code>true</code> when the GC is disposed and <code>false</code> otherwise
	 */
	bool IsDisposed() {
		return !IsOk();
	}
	bool IsOk() {
		return w_graphics_is_ok(W_GRAPHICS(this));
	}
	/**
	 * Sets the receiver to always use the operating system's advanced graphics
	 * subsystem for all graphics operations if the argument is <code>true</code>.
	 * If the argument is <code>false</code>, the advanced graphics subsystem is
	 * no longer used, advanced graphics state is cleared and the normal graphics
	 * subsystem is used from now on.
	 * <p>
	 * Normally, the advanced graphics subsystem is invoked automatically when
	 * any one of the alpha, antialias, patterns, interpolation, paths, clipping
	 * or transformation operations in the receiver is requested.  When the receiver
	 * is switched into advanced mode, the advanced graphics subsystem performs both
	 * advanced and normal graphics operations.  Because the two subsystems are
	 * different, their output may differ.  Switching to advanced graphics before
	 * any graphics operations are performed ensures that the output is consistent.
	 * </p><p>
	 * Advanced graphics may not be installed for the operating system.  In this
	 * case, this operation does nothing.  Some operating system have only one
	 * graphics subsystem, so switching from normal to advanced graphics does
	 * nothing.  However, switching from advanced to normal graphics will always
	 * clear the advanced graphics state, even for operating systems that have
	 * only one graphics subsystem.
	 * </p>
	 *
	 * @param advanced the new advanced graphics state
	 *
	 * @see #setAlpha
	 * @see #setAntialias
	 * @see #setBackgroundPattern
	 * @see #setClipping(Path)
	 * @see #setForegroundPattern
	 * @see #setLineAttributes
	 * @see #setInterpolation
	 * @see #setTextAntialias
	 * @see #setTransform
	 * @see #getAdvanced
	 */
	void SetAdvanced(bool advanced) {
		w_graphics_set_advanced(W_GRAPHICS(this), advanced);
	}
	/**
	 * Sets the receiver's anti-aliasing value to the parameter,
	 * which must be one of <code>SWT.DEFAULT</code>, <code>SWT.OFF</code>
	 * or <code>SWT.ON</code>. Note that this controls anti-aliasing for all
	 * <em>non-text drawing</em> operations.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 *
	 * @param antialias the anti-aliasing setting
	 *
	 * @see #getAdvanced
	 * @see #setAdvanced
	 * @see #setTextAntialias
	 */
	void SetAntialias(int antialias) {
		w_graphics_set_antialias(W_GRAPHICS(this), antialias);
	}
	/**
	 * Sets the receiver's alpha value which must be
	 * between 0 (transparent) and 255 (opaque).
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 * @param alpha the alpha value
	 *
	 * @see #getAdvanced
	 * @see #setAdvanced
	 */
	void SetAlpha(int alpha) {
		w_graphics_set_alpha(W_GRAPHICS(this), alpha);
	}
	/**
	 * Sets the background color. The background color is used
	 * for fill operations and as the background color when text
	 * is drawn.
	 *
	 * @param color the new background color for the receiver
	 */
	void SetBackground(w_color color) {
		w_graphics_set_background(W_GRAPHICS(this), color);
	}
	/**
	 * Sets the background color. The background color is used
	 * for fill operations and as the background color when text
	 * is drawn.
	 *
	 * @param color the new background color for the receiver
	 */
	void SetBackground(WColor color) {
		w_graphics_set_background(W_GRAPHICS(this), color);
	}
	/**
	 * Sets the background pattern. The default value is <code>null</code>.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 *
	 * @param pattern the new background pattern
	 *
	 * @see Pattern
	 * @see #getAdvanced
	 * @see #setAdvanced
	 */
	void SetBackgroundPattern(WPattern &pattern) {
		w_graphics_set_background_pattern(W_GRAPHICS(this),
				(w_pattern*) &pattern);
	}
	/**
	 * Sets the area of the receiver which can be changed
	 * by drawing operations to the rectangular area specified
	 * by the arguments.
	 *
	 * @param x the x coordinate of the clipping rectangle
	 * @param y the y coordinate of the clipping rectangle
	 * @param width the width of the clipping rectangle
	 * @param height the height of the clipping rectangle
	 */
	void SetClipping(int x, int y, int width, int height) {
		WRect rect(x, y, width, height);
		SetClipping(rect);
	}
	/**
	 * Sets the area of the receiver which can be changed
	 * by drawing operations to the path specified
	 * by the argument.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 *
	 * @param path the clipping path.
	 *
	 * @see Path
	 * @see #getAdvanced
	 * @see #setAdvanced
	 */
	void SetClipping(WPath &path) {
		w_graphics_set_clipping_path(W_GRAPHICS(this), (w_path*) &path);
	}
	/**
	 * Sets the area of the receiver which can be changed
	 * by drawing operations to the rectangular area specified
	 * by the argument.  Specifying <code>null</code> for the
	 * rectangle reverts the receiver's clipping area to its
	 * original value.
	 *
	 * @param rect the clipping rectangle or <code>null</code>
	 */
	void SetClipping(WRect &rect) {
		w_graphics_set_clipping_rect(W_GRAPHICS(this), (w_rect*) &rect);
	}
	/**
	 * Sets the area of the receiver which can be changed
	 * by drawing operations to the region specified
	 * by the argument.  Specifying <code>null</code> for the
	 * region reverts the receiver's clipping area to its
	 * original value.
	 *
	 * @param region the clipping region or <code>null</code>
	 */
	void SetClipping(WRegion &region) {
		w_graphics_set_clipping_region(W_GRAPHICS(this), (w_region*) &region);
	}
	/**
	 * Sets the receiver's fill rule to the parameter, which must be one of
	 * <code>SWT.FILL_EVEN_ODD</code> or <code>SWT.FILL_WINDING</code>.
	 *
	 * @param rule the new fill rule
	 */
	void SetFillRule(int rule) {
		w_graphics_set_fill_rule(W_GRAPHICS(this), rule);
	}
	/**
	 * Sets the font which will be used by the receiver
	 * to draw and measure text to the argument. If the
	 * argument is null, then a default font appropriate
	 * for the platform will be used instead.
	 *
	 * @param font the new font for the receiver, or null to indicate a default font
	 */
	void SetFont(WFont *font) {
		w_graphics_set_font(W_GRAPHICS(this), (w_font*) font);
	}
	/**
	 * Sets the foreground color. The foreground color is used
	 * for drawing operations including when text is drawn.
	 *
	 * @param color the new foreground color for the receiver
	 */
	void SetForeground(w_color color) {
		w_graphics_set_foreground(W_GRAPHICS(this), color);
	}
	/**
	 * Sets the foreground color. The foreground color is used
	 * for drawing operations including when text is drawn.
	 *
	 * @param color the new foreground color for the receiver
	 */
	void SetForeground(WColor color) {
		w_graphics_set_foreground(W_GRAPHICS(this), color);
	}
	/**
	 * Sets the foreground pattern. The default value is <code>null</code>.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 * @param pattern the new foreground pattern
	 *
	 * @see Pattern
	 * @see #getAdvanced
	 * @see #setAdvanced
	 */
	void SetForegroundPattern(WPattern &pattern) {
		w_graphics_set_foreground_pattern(W_GRAPHICS(this),
				(w_pattern*) &pattern);
	}
	/**
	 * Sets the receiver's interpolation setting to the parameter, which
	 * must be one of <code>SWT.DEFAULT</code>, <code>SWT.NONE</code>,
	 * <code>SWT.LOW</code> or <code>SWT.HIGH</code>.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 *
	 * @param interpolation the new interpolation setting
	 *
	 * @see #getAdvanced
	 * @see #setAdvanced
	 */
	void SetInterpolation(int interpolation) {
		w_graphics_set_interpolation(W_GRAPHICS(this), interpolation);
	}
	/**
	 * Sets the receiver's line attributes.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 * @param attributes the line attributes
	 *
	 * @see LineAttributes
	 * @see #getAdvanced
	 * @see #setAdvanced
	 */
	void SetLineAttributes(WLineAttributes &attributes) {
		w_graphics_set_line_attributes(W_GRAPHICS(this),
				(w_line_attributes*) &attributes);
	}
	/**
	 * Sets the receiver's line cap style to the argument, which must be one
	 * of the constants <code>W_CAP_FLAT</code>, <code>W_CAP_ROUND</code>,
	 * or <code>W_CAP_SQUARE</code>.
	 *
	 * @param cap the cap style to be used for drawing lines
	 */
	void SetLineCap(int cap) {
		w_graphics_set_line_cap(W_GRAPHICS(this), cap);
	}
	void SetLineCapFlat() {
		return SetLineCap(W_CAP_FLAT);
	}
	void SetLineCapRound() {
		return SetLineCap(W_CAP_ROUND);
	}
	void SetLineCapSquare() {
		return SetLineCap(W_CAP_SQUARE);
	}
	/**
	 * Sets the receiver's line dash style to the argument. The default
	 * value is <code>null</code>. If the argument is not <code>null</code>,
	 * the receiver's line style is set to <code>SWT.LINE_CUSTOM</code>, otherwise
	 * it is set to <code>SWT.LINE_SOLID</code>.
	 *
	 * @param dashes the dash style to be used for drawing lines
	 */
	void SetLineDash(int *dashes, size_t count) {
		w_graphics_set_line_dash(W_GRAPHICS(this), dashes, count);
	}
	/**
	 * Sets the receiver's line join style to the argument, which must be one
	 * of the constants <code>W_JOIN_MITER</code>, <code>W_JOIN_ROUND</code>,
	 * or <code>W_JOIN_BEVEL</code>.
	 *
	 * @param join the join style to be used for drawing lines
	 */
	void SetLineJoin(int join) {
		w_graphics_set_line_join(W_GRAPHICS(this), join);
	}
	void SetLineJoinMiter() {
		return SetLineJoin(W_JOIN_MITER);
	}
	void SetLineJoinRound() {
		return SetLineJoin(W_JOIN_ROUND);
	}
	void SetLineJoinBevel() {
		return SetLineJoin(W_JOIN_BEVEL);
	}
	/**
	 * Sets the receiver's line style to the argument, which must be one
	 * of the constants <code>SWT.LINE_SOLID</code>, <code>SWT.LINE_DASH</code>,
	 * <code>SWT.LINE_DOT</code>, <code>SWT.LINE_DASHDOT</code> or
	 * <code>SWT.LINE_DASHDOTDOT</code>.
	 *
	 * @param lineStyle the style to be used for drawing lines
	 */
	void SetLineStyle(int lineStyle) {
		w_graphics_set_line_style(W_GRAPHICS(this), lineStyle);
	}
	void SetLineStyleSolid() {
		return SetLineStyle(W_LINE_SOLID);
	}
	void SetLineStyleDash() {
		return SetLineStyle(W_LINE_DASH);
	}
	void SetLineStyleDot() {
		return SetLineStyle(W_LINE_DOT);
	}
	void SetLineStyleDashDot() {
		return SetLineStyle(W_LINE_DASHDOT);
	}
	void SetLineStyleDashDotDot() {
		return SetLineStyle(W_LINE_DASHDOTDOT);
	}
	/**
	 * Sets the width that will be used when drawing lines
	 * for all of the figure drawing operations (that is,
	 * <code>drawLine</code>, <code>drawRectangle</code>,
	 * <code>drawPolyline</code>, and so forth.
	 * <p>
	 * Note that line width of zero is used as a hint to
	 * indicate that the fastest possible line drawing
	 * algorithms should be used. This means that the
	 * output may be different from line width one.
	 * </p>
	 *
	 * @param lineWidth the width of a line
	 */
	void SetLineWidth(int lineWidth) {
		w_graphics_set_line_width(W_GRAPHICS(this), lineWidth);
	}
	/**
	 * If the argument is <code>true</code>, puts the receiver
	 * in a drawing mode where the resulting color in the destination
	 * is the <em>exclusive or</em> of the color values in the source
	 * and the destination, and if the argument is <code>false</code>,
	 * puts the receiver in a drawing mode where the destination color
	 * is replaced with the source color value.
	 * <p>
	 * Note that this mode in fundamentally unsupportable on certain
	 * platforms, notably Carbon (Mac OS X). Clients that want their
	 * code to run on all platforms need to avoid this method.
	 * </p>
	 *
	 * @param xor if <code>true</code>, then <em>xor</em> mode is used, otherwise <em>source copy</em> mode is used
	 *
	 * @deprecated this functionality is not supported on some platforms
	 */
	void SetXORMode(bool xor_) {
		w_graphics_set_xor_mode(W_GRAPHICS(this), xor_);
	}
	/**
	 * Sets the receiver's text anti-aliasing value to the parameter,
	 * which must be one of <code>SWT.DEFAULT</code>, <code>SWT.OFF</code>
	 * or <code>SWT.ON</code>. Note that this controls anti-aliasing only
	 * for all <em>text drawing</em> operations.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 *
	 * @param antialias the anti-aliasing setting
	 *
	 * @see #getAdvanced
	 * @see #setAdvanced
	 * @see #setAntialias
	 */
	void SetTextAntialias(int antialias) {
		w_graphics_set_text_antialias(W_GRAPHICS(this), antialias);
	}
	/**
	 * Sets the transform that is currently being used by the receiver. If
	 * the argument is <code>null</code>, the current transform is set to
	 * the identity transform.
	 * <p>
	 * This operation requires the operating system's advanced
	 * graphics subsystem which may not be available on some
	 * platforms.
	 * </p>
	 *
	 * @param transform the transform to set
	 *
	 * @see Transform
	 * @see #getAdvanced
	 * @see #setAdvanced
	 */
	void SetTransform(const WTransform *transform) {
		w_graphics_set_transform(W_GRAPHICS(this), (w_transform*) transform);
	}
	void SetTransform(const WTransform &transform) {
		SetTransform(&transform);
	}
	/**
	 * Returns the extent of the given string. No tab
	 * expansion or carriage return processing will be performed.
	 * <p>
	 * The <em>extent</em> of a string is the width and height of
	 * the rectangular area it would cover if drawn in a particular
	 * font (in this case, the current font in the receiver).
	 * </p>
	 *
	 * @param string the string to measure
	 * @return a point containing the extent of the string
	 */
	WSize StringExtent(const char *string) {
		return TextExtent(string, 0);
	}
	/**
	 * Returns the extent of the given string. Tab expansion and
	 * carriage return processing are performed.
	 * <p>
	 * The <em>extent</em> of a string is the width and height of
	 * the rectangular area it would cover if drawn in a particular
	 * font (in this case, the current font in the receiver).
	 * </p>
	 *
	 * @param string the string to measure
	 * @return a point containing the extent of the string
	 */
	WSize TextExtent(const char *string) {
		return TextExtent(string, W_DRAW_DELIMITER | W_DRAW_TAB);
	}
	/**
	 * Returns the extent of the given string. Tab expansion, line
	 * delimiter and mnemonic processing are performed according to
	 * the specified flags, which can be a combination of:
	 * <dl>
	 * <dt><b>DRAW_DELIMITER</b></dt>
	 * <dd>draw multiple lines</dd>
	 * <dt><b>DRAW_TAB</b></dt>
	 * <dd>expand tabs</dd>
	 * <dt><b>DRAW_MNEMONIC</b></dt>
	 * <dd>underline the mnemonic character</dd>
	 * <dt><b>DRAW_TRANSPARENT</b></dt>
	 * <dd>transparent background</dd>
	 * </dl>
	 * <p>
	 * The <em>extent</em> of a string is the width and height of
	 * the rectangular area it would cover if drawn in a particular
	 * font (in this case, the current font in the receiver).
	 * </p>
	 *
	 * @param string the string to measure
	 * @param flags the flags specifying how to process the text
	 * @return a point containing the extent of the string
	 */
	WSize TextExtent(const char *string, int flags) {
		WSize size;
		TextExtent(string, -1, size, flags);
		return size;
	}
	/**
	 * Returns the extent of the given string. Tab expansion, line
	 * delimiter and mnemonic processing are performed according to
	 * the specified flags, which can be a combination of:
	 * <dl>
	 * <dt><b>DRAW_DELIMITER</b></dt>
	 * <dd>draw multiple lines</dd>
	 * <dt><b>DRAW_TAB</b></dt>
	 * <dd>expand tabs</dd>
	 * <dt><b>DRAW_MNEMONIC</b></dt>
	 * <dd>underline the mnemonic character</dd>
	 * <dt><b>DRAW_TRANSPARENT</b></dt>
	 * <dd>transparent background</dd>
	 * </dl>
	 * <p>
	 * The <em>extent</em> of a string is the width and height of
	 * the rectangular area it would cover if drawn in a particular
	 * font (in this case, the current font in the receiver).
	 * </p>
	 *
	 * @param string the string to measure
	 * @param flags the flags specifying how to process the text
	 * @return a point containing the extent of the string
	 */
	void TextExtent(const char *string, size_t length, WSize &size, int flags) {
		w_graphics_text_extent(W_GRAPHICS(this), string, length,
				(w_size*) &size, flags, W_ENCODING_UTF8);
	}
private:
	char handle[sizeof(w_graphics)];
};

namespace swt {
typedef ::WGraphics Graphics;
}  // namespace swt

#endif /* SWTP_GC_GRAPHICS_H_ */
