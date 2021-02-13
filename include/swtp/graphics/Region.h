/*
 * Region.h
 *
 *  Created on: 17 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_GRAPHICS_REGION_H_
#define SWTP_GRAPHICS_REGION_H_
#include "Rect.h"

/**
 * Instances of this class represent areas of an x-y coordinate
 * system that are aggregates of the areas covered by a number
 * of polygons.
 * <p>
 * Application code must explicitly invoke the <code>Region.dispose()</code>
 * method to release the operating system resources managed by each instance
 * when those instances are no longer required.
 * </p>
 */
class WRegion: public WResource {
public:

	/**
	 * Constructs a new empty region.
	 * <p>
	 * You must dispose the region when it is no longer required.
	 * </p>
	 *
	 * @see #dispose()
	 */
	WRegion() {
		w_region_init(W_REGION(this));
	}
	~WRegion() {
		w_region_dispose(W_REGION(this));
	}
	void Dispose() {
		w_region_dispose(W_REGION(this));
	}

	/**
	 * Constructs a new empty region.
	 * <p>
	 * You must dispose the region when it is no longer required.
	 * </p>
	 *
	 * @see #dispose()
	 */
	bool Create() {
		return w_region_create(W_REGION(this)) > 0;
	}

	/**
	 * Adds the given polygon to the collection of polygons
	 * the receiver maintains to describe its area.
	 *
	 * @param pointArray points that describe the polygon to merge with the receiver
	 *
	 */
	bool Add(WPoint *points, size_t count) {
		return w_region_add_points(W_REGION(this), (w_point*) points, count) > 0;
	}

	/**
	 * Adds the given rectangle to the collection of polygons
	 * the receiver maintains to describe its area.
	 *
	 * @param rect the rectangle to merge with the receiver
	 */
	bool Add(const WRect &rect) {
		return w_region_add_rectangle(W_REGION(this), (w_rect*) &rect) > 0;
	}

	/**
	 * Adds the given rectangle to the collection of polygons
	 * the receiver maintains to describe its area.
	 *
	 * @param x the x coordinate of the rectangle
	 * @param y the y coordinate of the rectangle
	 * @param width the width coordinate of the rectangle
	 * @param height the height coordinate of the rectangle
	 */
	bool Add(int x, int y, int width, int height) {
		return Add(WRect(x, y, width, height));
	}

	/**
	 * Adds all of the polygons which make up the area covered
	 * by the argument to the collection of polygons the receiver
	 * maintains to describe its area.
	 *
	 * @param region the region to merge
	 */
	bool Add(WRegion &region) {
		return w_region_add_region(W_REGION(this), W_REGION(&region)) > 0;
	}

	/**
	 * Returns <code>true</code> if the point specified by the
	 * arguments is inside the area specified by the receiver,
	 * and <code>false</code> otherwise.
	 *
	 * @param x the x coordinate of the point to test for containment
	 * @param y the y coordinate of the point to test for containment
	 * @return <code>true</code> if the region contains the point and <code>false</code> otherwise
	 */
	bool Contains(int x, int y) {
		return Contains(WPoint(x, y));
	}

	/**
	 * Returns <code>true</code> if the given point is inside the
	 * area specified by the receiver, and <code>false</code>
	 * otherwise.
	 *
	 * @param pt the point to test for containment
	 * @return <code>true</code> if the region contains the point and <code>false</code> otherwise
	 */
	bool Contains(const WPoint &pt) {
		return w_region_contains(W_REGION(this), (w_point*) &pt);
	}
	/**
	 * Returns a rectangle which represents the rectangular
	 * union of the collection of polygons the receiver
	 * maintains to describe its area.
	 *
	 * @return a bounding rectangle for the region
	 *
	 * @see Rectangle#union
	 */
	WRect& GetBounds(WRect &bounds) {
		w_region_get_bounds(W_REGION(this), (w_rect*) &bounds);
		return bounds;
	}
	/**
	 * Returns a rectangle which represents the rectangular
	 * union of the collection of polygons the receiver
	 * maintains to describe its area.
	 *
	 * @return a bounding rectangle for the region
	 *
	 * @see Rectangle#union
	 */
	WRect& GetBounds(const WRect &bounds = WRect()) {
		return GetBounds((WRect&) bounds);
	}

	/**
	 * Intersects the given rectangle to the collection of polygons
	 * the receiver maintains to describe its area.
	 *
	 * @param rect the rectangle to intersect with the receiver
	 */
	bool Intersect(const WRect &rect) {
		return w_region_intersect_rect(W_REGION(this), (w_rect*) &rect) > 0;
	}

	/**
	 * Intersects the given rectangle to the collection of polygons
	 * the receiver maintains to describe its area.
	 *
	 * @param x the x coordinate of the rectangle
	 * @param y the y coordinate of the rectangle
	 * @param width the width coordinate of the rectangle
	 * @param height the height coordinate of the rectangle
	 */
	bool Intersect(int x, int y, int width, int height) {
		return Intersect(WRect(x, y, width, height));
	}

	/**
	 * Intersects all of the polygons which make up the area covered
	 * by the argument to the collection of polygons the receiver
	 * maintains to describe its area.
	 *
	 * @param region the region to intersect
	 */
	bool Intersect(WRegion &region) {
		return w_region_intersect_region(W_REGION(this), W_REGION(&region)) > 0;
	}

	/**
	 * Returns <code>true</code> if the rectangle described by the
	 * arguments intersects with any of the polygons the receiver
	 * maintains to describe its area, and <code>false</code> otherwise.
	 *
	 * @param x the x coordinate of the origin of the rectangle
	 * @param y the y coordinate of the origin of the rectangle
	 * @param width the width of the rectangle
	 * @param height the height of the rectangle
	 * @return <code>true</code> if the rectangle intersects with the receiver, and <code>false</code> otherwise
	 *
	 * @see Rectangle#intersects(Rectangle)
	 */
	bool Intersects(int x, int y, int width, int height) {
		return Intersects(WRect(x, y, width, height));
	}
	/**
	 * Returns <code>true</code> if the given rectangle intersects
	 * with any of the polygons the receiver maintains to describe
	 * its area and <code>false</code> otherwise.
	 *
	 * @param rect the rectangle to test for intersection
	 * @return <code>true</code> if the rectangle intersects with the receiver, and <code>false</code> otherwise
	 *
	 * @see Rectangle#intersects(Rectangle)
	 */
	bool Intersects(const WRect &rect) {
		return w_region_intersects(W_REGION(this), (w_rect*) &rect) > 0;
	}

	/**
	 * Returns <code>true</code> if the region has been disposed,
	 * and <code>false</code> otherwise.
	 * <p>
	 * This method gets the dispose state for the region.
	 * When a region has been disposed, it is an error to
	 * invoke any other method (except {@link #dispose()}) using the region.
	 *
	 * @return <code>true</code> when the region is disposed, and <code>false</code> otherwise
	 */
	bool IsDisposed() {
		return !IsOk();
	}
	bool IsOk() {
		return w_region_is_ok(W_REGION(this));
	}

	/**
	 * Returns <code>true</code> if the receiver does not cover any
	 * area in the (x, y) coordinate plane, and <code>false</code> if
	 * the receiver does cover some area in the plane.
	 *
	 * @return <code>true</code> if the receiver is empty, and <code>false</code> otherwise
	 */
	bool IsEmpty() {
		return w_region_is_empty(W_REGION(this));
	}

	/**
	 * Subtracts the given polygon from the collection of polygons
	 * the receiver maintains to describe its area.
	 *
	 * @param pointArray points that describe the polygon to merge with the receiver
	 */
	bool Subtract(WPoint *points, size_t count) {
		return w_region_subtract_points(W_REGION(this), (w_point*) points,
				count) > 0;
	}
	/**
	 * Subtracts the given rectangle from the collection of polygons
	 * the receiver maintains to describe its area.
	 *
	 * @param rect the rectangle to subtract from the receiver
	 */
	bool Subtract(const WRect &rect) {
		return w_region_subtract_rectangle(W_REGION(this), (w_rect*) &rect) > 0;
	}

	/**
	 * Subtracts the given rectangle from the collection of polygons
	 * the receiver maintains to describe its area.
	 *
	 * @param x the x coordinate of the rectangle
	 * @param y the y coordinate of the rectangle
	 * @param width the width coordinate of the rectangle
	 * @param height the height coordinate of the rectangle
	 */
	bool Subtract(int x, int y, int width, int height) {
		return Subtract(WRect(x, y, width, height));
	}

	/**
	 * Subtracts all of the polygons which make up the area covered
	 * by the argument from the collection of polygons the receiver
	 * maintains to describe its area.
	 *
	 * @param region the region to subtract
	 */
	bool Subtract(WRegion &region) {
		return w_region_subtract_region(W_REGION(this), W_REGION(&region)) > 0;
	}

	/**
	 * Translate all of the polygons the receiver maintains to describe
	 * its area by the specified point.
	 *
	 * @param x the x coordinate of the point to translate
	 * @param y the y coordinate of the point to translate
	 */
	bool Translate(int x, int y) {
		return Translate(WPoint(x, y));
	}

	/**
	 * Translate all of the polygons the receiver maintains to describe
	 * its area by the specified point.
	 *
	 * @param pt the point to translate
	 */
	bool Translate(const WPoint &pt) {
		return w_region_translate(W_REGION(this), (w_point*) &pt) > 0;
	}
private:
	void *handles[sizeof(w_region) / sizeof(void*)];
};

#endif /* SWTP_GRAPHICS_REGION_H_ */
