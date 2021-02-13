/*
 * Rect.h
 *
 *  Created on: 24 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_GRAPHICS_RECT_H_
#define SWTP_GRAPHICS_RECT_H_
#include "../core/Resource.h"
class WRect;
class WPoint {
public:
	int x;
	int y;
	WPoint() {
		this->x = 0;
		this->y = 0;
	}
	WPoint(int x, int y) {
		this->x = x;
		this->y = y;
	}
	WPoint(const WPoint &p) {
		this->x = p.x;
		this->y = p.y;
	}
	inline WPoint(const WRect &r);
	bool Equals(const WPoint &p) {
		return (p.x == this->x) && (p.y == this->y);
	}
	bool operator==(const WPoint &p) {
		return this->Equals(p);
	}
	bool operator!=(const WPoint &p) {
		return !this->Equals(p);
	}
	char* ToChars(const char *name, char *text, size_t length) {
		snprintf(text, length, "%s{%d,%d}", name, this->x, this->y);
		return text;
	}
	char* ToChars(char *text, size_t length) {
		return ToChars("", text, length);
	}
};
class WPointF {
public:
	float x;
	float y;
	WPointF() {
		this->x = 0;
		this->y = 0;
	}
	WPointF(float x, float y) {
		this->x = x;
		this->y = y;
	}
	WPointF(const WPointF &pt) {
		this->x = pt.x;
		this->y = pt.y;
	}
};
class WSize {
public:
	int width;
	int height;
	WSize() {
		this->width = 0;
		this->height = 0;
	}
	WSize(int width, int height) {
		this->width = width;
		this->height = height;
	}
	WSize(const WSize &sz) {
		this->width = sz.width;
		this->height = sz.height;
	}
	inline WSize(const WRect &r);
	char* ToChars(const char *name, char *text, size_t length) {
		snprintf(text, length, "%s{%d,%d}", name, this->width, this->height);
		return text;
	}
	char* ToChars(char *text, size_t length) {
		return ToChars("", text, length);
	}
};
class WRange {
public:
	int begin;
	int end;
	WRange() {
		this->begin = 0;
		this->end = 0;
	}
	WRange(int begin, int end) {
		this->begin = begin;
		this->end = end;
	}
	char* ToChars(const char *name, char *text, size_t length) {
		snprintf(text, length, "%s{%d,%d}", name, this->begin, this->end);
		return text;
	}
	char* ToChars(char *text, size_t length) {
		return ToChars("", text, length);
	}
};
/**
 * Instances of this class represent rectangular areas in an
 * (x, y) coordinate system. The top left corner of the rectangle
 * is specified by its x and y values, and the extent of the
 * rectangle is specified by its width and height.
 * <p>
 * The coordinate space for rectangles and points is considered
 * to have increasing values downward and to the right from its
 * origin making this the normal, computer graphics oriented notion
 * of (x, y) coordinates rather than the strict mathematical one.
 * </p>
 * <p>
 * The hashCode() method in this class uses the values of the public
 * fields to compute the hash value. When storing instances of the
 * class in hashed collections, do not modify these fields after the
 * object has been inserted.
 * </p>
 * <p>
 * Application code does <em>not</em> need to explicitly release the
 * resources managed by each instance when those instances are no longer
 * required, and thus no <code>dispose()</code> method is provided.
 * </p>
 *
 * @see Point
 */
class WRect {
public:
    union {
    struct {
	/**
	 * the x coordinate of the rectangle
	 */
	int x;
	/**
	 * the y coordinate of the rectangle
	 */
	int y;
	/**
	 * the width of the rectangle
	 */
	int width;
	/**
	 * the height of the rectangle
	 */
	int height;
    };
    struct {
        w_point origin;
        w_size size;
    };
    };
	WRect() {
		this->x = 0;
		this->y = 0;
		this->width = 0;
		this->height = 0;
	}
	/**
	 * Construct a new instance of this class given the
	 * x, y, width and height values.
	 *
	 * @param x the x coordinate of the origin of the rectangle
	 * @param y the y coordinate of the origin of the rectangle
	 * @param width the width of the rectangle
	 * @param height the height of the rectangle
	 */
	WRect(int x, int y, int width, int height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	WRect(const WRect &r) {
		this->x = r.x;
		this->y = r.y;
		this->width = r.width;
		this->height = r.height;
	}
	WRect(const WPoint &pt, const WSize &sz) {
		this->x = pt.x;
		this->y = pt.y;
		this->width = sz.width;
		this->height = sz.height;
	}
    WPoint& GetOrigin(){
        return (WPoint&)this->origin;
    }
    const WPoint& GetOrigin() const{
        return (const WPoint&)this->origin;
    }
    WSize& GetSize(){
        return (WSize&)this->size;
    }
    const WSize& GetSize() const{
        return (const WSize&)this->size;
    }
	bool operator ==(const WRect &rect) {
		return this->Equals(rect);
	}
	bool operator !=(const WRect &rect) {
		return !this->Equals(rect);
	}
	/**
	 * Destructively replaces the x, y, width and height values
	 * in the receiver with ones which represent the union of the
	 * rectangles specified by the receiver and the given rectangle.
	 * <p>
	 * The union of two rectangles is the smallest single rectangle
	 * that completely covers both of the areas covered by the two
	 * given rectangles.
	 * </p>
	 *
	 * @param rect the rectangle to merge with the receiver
	 */
	void Add(const WRect &rect) {
		w_rect_add((w_rect*) this, (w_rect*) &rect);
	}
	/**
	 * Returns <code>true</code> if the point specified by the
	 * arguments is inside the area specified by the receiver,
	 * and <code>false</code> otherwise.
	 *
	 * @param x the x coordinate of the point to test for containment
	 * @param y the y coordinate of the point to test for containment
	 * @return <code>true</code> if the rectangle contains the point and <code>false</code> otherwise
	 */
	bool Contains(int x, int y) {
		return w_rect_contains((w_rect*) this, x, y);
	}
	/**
	 * Returns <code>true</code> if the given point is inside the
	 * area specified by the receiver, and <code>false</code>
	 * otherwise.
	 *
	 * @param pt the point to test for containment
	 * @return <code>true</code> if the rectangle contains the point and <code>false</code> otherwise
	 */
	bool Contains(const WPoint &pt) {
		return Contains(pt.x, pt.y);
	}
	/**
	 * Compares the argument to the receiver, and returns true
	 * if they represent the <em>same</em> object using a class
	 * specific comparison.
	 *
	 * @param object the object to compare with this object
	 * @return <code>true</code> if the object is the same as this object and <code>false</code> otherwise
	 */
	bool Equals(const WRect &rect) {
		return w_rect_equals((w_rect*) this, (w_rect*) &rect);
	}
	/**
	 * Destructively replaces the x, y, width and height values
	 * in the receiver with ones which represent the intersection of the
	 * rectangles specified by the receiver and the given rectangle.
	 *
	 * @param rect the rectangle to intersect with the receiver
	 */
	void Intersect(const WRect &rect) {
		w_rect_intersect((w_rect*) this, (w_rect*) &rect);
	}
	/**
	 * Returns a new rectangle which represents the intersection
	 * of the receiver and the given rectangle.
	 * <p>
	 * The intersection of two rectangles is the rectangle that
	 * covers the area which is contained within both rectangles.
	 * </p>
	 *
	 * @param rect the rectangle to intersect with the receiver
	 * @return the intersection of the receiver and the argument
	 */
	void Intersection(WRect &result, const WRect &rect) {
		w_rect_intersection((w_rect*) &result, (w_rect*) this, (w_rect*) &rect);
	}
	/**
	 * Returns a new rectangle which represents the intersection
	 * of the receiver and the given rectangle.
	 * <p>
	 * The intersection of two rectangles is the rectangle that
	 * covers the area which is contained within both rectangles.
	 * </p>
	 *
	 * @param rect the rectangle to intersect with the receiver
	 * @return the intersection of the receiver and the argument
	 */
	WRect Intersection(const WRect &rect) {
		WRect result;
		Intersection(result, rect);
		return result;
	}
	/**
	 * Returns <code>true</code> if the rectangle described by the
	 * arguments intersects with the receiver and <code>false</code>
	 * otherwise.
	 * <p>
	 * Two rectangles intersect if the area of the rectangle
	 * representing their intersection is not empty.
	 * </p>
	 *
	 * @param x the x coordinate of the origin of the rectangle
	 * @param y the y coordinate of the origin of the rectangle
	 * @param width the width of the rectangle
	 * @param height the height of the rectangle
	 * @return <code>true</code> if the rectangle intersects with the receiver, and <code>false</code> otherwise
	 */
	bool Intersects(int x, int y, int width, int height) {
		return Intersects(WRect(x, y, width, height));
	}
	/**
	 * Returns <code>true</code> if the given rectangle intersects
	 * with the receiver and <code>false</code> otherwise.
	 * <p>
	 * Two rectangles intersect if the area of the rectangle
	 * representing their intersection is not empty.
	 * </p>
	 *
	 * @param rect the rectangle to test for intersection
	 * @return <code>true</code> if the rectangle intersects with the receiver, and <code>false</code> otherwise
	 */
	bool Intersects(const WRect &rect) {
		return w_rect_intersects((w_rect*) this, (w_rect*) &rect);
	}
	/**
	 * Returns <code>true</code> if the receiver does not cover any
	 * area in the (x, y) coordinate plane, and <code>false</code> if
	 * the receiver does cover some area in the plane.
	 * <p>
	 * A rectangle is considered to <em>cover area</em> in the
	 * (x, y) coordinate plane if both its width and height are
	 * non-zero.
	 * </p>
	 *
	 * @return <code>true</code> if the receiver is empty, and <code>false</code> otherwise
	 */
	bool IsEmpty() {
		return w_rect_is_empty((w_rect*) this);
	}
	/**
	 * Returns a new rectangle which represents the union of
	 * the receiver and the given rectangle.
	 * <p>
	 * The union of two rectangles is the smallest single rectangle
	 * that completely covers both of the areas covered by the two
	 * given rectangles.
	 * </p>
	 *
	 * @param rect the rectangle to perform union with
	 * @return the union of the receiver and the argument
	 */
	void Union(WRect &result, const WRect &rect) {
		w_rect_union((w_rect*) &result, (w_rect*) this, (w_rect*) &rect);
	}
	/**
	 * Returns a new rectangle which represents the union of
	 * the receiver and the given rectangle.
	 * <p>
	 * The union of two rectangles is the smallest single rectangle
	 * that completely covers both of the areas covered by the two
	 * given rectangles.
	 * </p>
	 *
	 * @param rect the rectangle to perform union with
	 * @return the union of the receiver and the argument
	 */
	WRect Union(const WRect &rect) {
		WRect result;
		Union(result, rect);
		return result;
	}
	char* ToChars(const char *name, char *text, size_t length) {
		snprintf(text, length, "%s{%d,%d,%d,%d}", name, this->x, this->y,
				this->width, this->height);
		return text;
	}
	char* ToChars(char *text, size_t length) {
		return ToChars("", text, length);
	}

};
class WRectF {
public:
	float x;
	float y;
	float width;
	float height;
	WRectF() {
		this->x = 0;
		this->y = 0;
		this->width = 0;
		this->height = 0;
	}
	WRectF(float x, float y, float width, float height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	WRectF(const WRectF &r) {
		this->x = r.x;
		this->y = r.y;
		this->width = r.width;
		this->height = r.height;
	}
};
inline WPoint::WPoint(const WRect &r) {
	this->x = r.x;
	this->y = r.y;
}

inline WSize::WSize(const WRect &r) {
	this->width = r.width;
	this->height = r.height;
}
namespace swt {
typedef ::WPoint Point;
typedef ::WSize Size;
typedef ::WRect Rect;
}  // namespace swt

#endif /* SWTP_GRAPHICS_RECT_H_ */
