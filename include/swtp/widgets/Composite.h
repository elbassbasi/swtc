/*
 * Composite.h
 *
 *  Created on: 25 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_COMPOSITE_H_
#define SWTP_WIDGETS_COMPOSITE_H_
#include "Scrollable.h"
/**
 * Instances of this class are controls which are capable
 * of containing other controls.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>NO_BACKGROUND, NO_FOCUS, NO_MERGE_PAINTS, NO_REDRAW_RESIZE, NO_RADIO_GROUP, EMBEDDED, DOUBLE_BUFFERED</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * <p>
 * Note: The <code>NO_BACKGROUND</code>, <code>NO_FOCUS</code>, <code>NO_MERGE_PAINTS</code>,
 * and <code>NO_REDRAW_RESIZE</code> styles are intended for use with <code>Canvas</code>.
 * They can be used with <code>Composite</code> if you are drawing your own, but their
 * behavior is undefined if they are used with subclasses of <code>Composite</code> other
 * than <code>Canvas</code>.
 * </p><p>
 * Note: The <code>CENTER</code> style, although undefined for composites, has the
 * same value as <code>EMBEDDED</code> which is used to embed widgets from other
 * widget toolkits into SWT.  On some operating systems (GTK), this may cause
 * the children of this composite to be obscured.
 * </p><p>
 * This class may be subclassed by custom control implementors
 * who are building controls that are constructed from aggregates
 * of other controls.
 * </p>
 */
class SWTP_PUBLIC WComposite: public WScrollable {
public:
	WComposite() {
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * and a style value describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p>
	 *
	 * @param parent a widget which will be the parent of the new instance (cannot be null)
	 * @param style the style of widget to construct
	 *
	 * @see SWT#NO_BACKGROUND
	 * @see SWT#NO_FOCUS
	 * @see SWT#NO_MERGE_PAINTS
	 * @see SWT#NO_REDRAW_RESIZE
	 * @see SWT#NO_RADIO_GROUP
	 * @see SWT#EMBEDDED
	 * @see SWT#DOUBLE_BUFFERED
	 * @see Widget#getStyle
	 */
	WComposite(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	WComposite(WComposite &parent, wuint64 style) {
		Create(&parent, style);
	}
	/**
	 * Fills the interior of the rectangle specified by the arguments,
	 * with the receiver's background.
	 *
	 * <p>The <code>offsetX</code> and <code>offsetY</code> are used to map from
	 * the <code>gc</code> origin to the origin of the parent image background. This is useful
	 * to ensure proper alignment of the image background.</p>
	 *
	 * @param gc the gc where the rectangle is to be filled
	 * @param x the x coordinate of the rectangle to be filled
	 * @param y the y coordinate of the rectangle to be filled
	 * @param width the width of the rectangle to be filled
	 * @param height the height of the rectangle to be filled
	 * @param offsetX the image background x offset
	 * @param offsetY the image background y offset
	 */
	void DrawBackground(WGraphics &gc, int x, int y, int width, int height,
			int offsetX, int offsetY) {
		DrawBackground(gc, WRect(x, y, width, height),
				WPoint(offsetX, offsetY));
	}
	void DrawBackground(WGraphics &gc, const WRect &rect,
			const WPoint &offset) {

	}
	/**
	 * Returns a (possibly empty) array containing the receiver's children.
	 * Children are returned in the order that they are drawn.  The topmost
	 * control appears at the beginning of the array.  Subsequent controls
	 * draw beneath this control and appear later in the array.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of children, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return an array of children
	 *
	 * @see Control#moveAbove
	 * @see Control#moveBelow
	 */
	void GetChildren(WIterator<WControl*> &children) {
		w_composite_get_children(W_COMPOSITE(this), (w_iterator*) &children);
	}
	/**
	 * Returns layout which is associated with the receiver, or
	 * null if one has not been set.
	 *
	 * @return the receiver's layout or null
	 */
	WLayout* GetLayout() {
		WLayout *layout;
		w_composite_get_layout(W_COMPOSITE(this), (w_layout**) &layout);
		return layout;
	}
	/**
	 * Returns <code>true</code> if the receiver has deferred
	 * the performing of layout, and <code>false</code> otherwise.
	 *
	 * @return the receiver's deferred layout state
	 *
	 * @see #setLayoutDeferred(bool)
	 * @see #isLayoutDeferred()
	 */
	bool GetLayoutDeferred() {
		return w_composite_get_layout_deferred(W_COMPOSITE(this));
	}
	/**
	 * Gets the (possibly empty) tabbing order for the control.
	 *
	 * @return tabList the ordered list of controls representing the tab order
	 *
	 * @see #setTabList
	 */
	void GetTabList(WIterator<WControl*> &tablist) {
		w_composite_get_tab_list(W_COMPOSITE(this), (w_iterator*) &tablist);
	}
	/**
	 * Returns <code>true</code> if the receiver or any ancestor
	 * up to and including the receiver's nearest ancestor shell
	 * has deferred the performing of layouts.  Otherwise, <code>false</code>
	 * is returned.
	 *
	 * @return the receiver's deferred layout state
	 *
	 * @see #setLayoutDeferred(bool)
	 * @see #getLayoutDeferred()
	 */
	bool IsLayoutDeferred() {
		return w_composite_is_layout_deferred(W_COMPOSITE(this));
	}
	/**
	 * If the receiver has a layout, asks the layout to <em>lay out</em>
	 * (that is, set the size and location of) the receiver's children.
	 * If the receiver does not have a layout, do nothing.
	 * <p>
	 * Use of this method is discouraged since it is the least-efficient
	 * way to trigger a layout. The use of <code>layout(true)</code>
	 * discards all cached layout information, even from controls which
	 * have not changed. It is much more efficient to invoke
	 * {@link Control#requestLayout()} on every control which has changed
	 * in the layout than it is to invoke this method on the layout itself.
	 * </p>
	 * <p>
	 * This is equivalent to calling <code>layout(true)</code>.
	 * </p>
	 * <p>
	 * Note: Layout is different from painting. If a child is
	 * moved or resized such that an area in the parent is
	 * exposed, then the parent will paint. If no child is
	 * affected, the parent will not paint.
	 * </p>
	 */
	void DoLayout() {
		DoLayout(true);
	}
	/**
	 * If the receiver has a layout, asks the layout to <em>lay out</em>
	 * (that is, set the size and location of) the receiver's children.
	 * If the argument is <code>true</code> the layout must not rely
	 * on any information it has cached about the immediate children. If it
	 * is <code>false</code> the layout may (potentially) optimize the
	 * work it is doing by assuming that none of the receiver's
	 * children has changed state since the last layout.
	 * If the receiver does not have a layout, do nothing.
	 * <p>
	 * It is normally more efficient to invoke {@link Control#requestLayout()}
	 * on every control which has changed in the layout than it is to invoke
	 * this method on the layout itself. Clients are encouraged to use
	 * {@link Control#requestLayout()} where possible instead of calling
	 * this method.
	 * </p>
	 * <p>
	 * If a child is resized as a result of a call to layout, the
	 * resize event will invoke the layout of the child.  The layout
	 * will cascade down through all child widgets in the receiver's widget
	 * tree until a child is encountered that does not resize.  Note that
	 * a layout due to a resize will not flush any cached information
	 * (same as <code>layout(false)</code>).
	 * </p>
	 * <p>
	 * Note: Layout is different from painting. If a child is
	 * moved or resized such that an area in the parent is
	 * exposed, then the parent will paint. If no child is
	 * affected, the parent will not paint.
	 * </p>
	 *
	 * @param changed <code>true</code> if the layout must flush its caches, and <code>false</code> otherwise
	 */
	void DoLayout(bool changed) {
		DoLayout(changed, false);
	}
	/**
	 * If the receiver has a layout, asks the layout to <em>lay out</em>
	 * (that is, set the size and location of) the receiver's children.
	 * If the changed argument is <code>true</code> the layout must not rely
	 * on any information it has cached about its children. If it
	 * is <code>false</code> the layout may (potentially) optimize the
	 * work it is doing by assuming that none of the receiver's
	 * children has changed state since the last layout.
	 * If the all argument is <code>true</code> the layout will cascade down
	 * through all child widgets in the receiver's widget tree, regardless of
	 * whether the child has changed size.  The changed argument is applied to
	 * all layouts.  If the all argument is <code>false</code>, the layout will
	 * <em>not</em> cascade down through all child widgets in the receiver's widget
	 * tree.  However, if a child is resized as a result of a call to layout, the
	 * resize event will invoke the layout of the child.  Note that
	 * a layout due to a resize will not flush any cached information
	 * (same as <code>layout(false)</code>).
	 * </p>
	 * <p>
	 * It is normally more efficient to invoke {@link Control#requestLayout()}
	 * on every control which has changed in the layout than it is to invoke
	 * this method on the layout itself. Clients are encouraged to use
	 * {@link Control#requestLayout()} where possible instead of calling
	 * this method.
	 * </p>
	 * <p>
	 * Note: Layout is different from painting. If a child is
	 * moved or resized such that an area in the parent is
	 * exposed, then the parent will paint. If no child is
	 * affected, the parent will not paint.
	 * </p>
	 *
	 * @param changed <code>true</code> if the layout must flush its caches, and <code>false</code> otherwise
	 * @param all <code>true</code> if all children in the receiver's widget tree should be laid out, and <code>false</code> otherwise
	 */
	void DoLayout(bool changed, bool all) {
		w_composite_do_layout(W_COMPOSITE(this), changed, all);
	}
	/**
	 * Sets the layout which is associated with the receiver to be
	 * the argument which may be null.
	 *
	 * @param layout the receiver's new layout or null
	 */
	void SetLayout(WLayout *layout) {
		w_composite_set_layout(W_COMPOSITE(this), (w_layout*) layout);
	}
	/**
	 * Sets the layout which is associated with the receiver to be
	 * the argument which may be null.
	 *
	 * @param layout the receiver's new layout or null
	 */
	void SetLayout(WLayout &layout) {
		SetLayout(&layout);
	}
	/**
	 * If the argument is <code>true</code>, causes subsequent layout
	 * operations in the receiver or any of its children to be ignored.
	 * No layout of any kind can occur in the receiver or any of its
	 * children until the flag is set to false.
	 * Layout operations that occurred while the flag was
	 * <code>true</code> are remembered and when the flag is set to
	 * <code>false</code>, the layout operations are performed in an
	 * optimized manner.  Nested calls to this method are stacked.
	 *
	 * @param defer the new defer state
	 *
	 * @see #layout(bool)
	 * @see #layout(Control[])
	 */
	void SetLayoutDeferred(bool defer) {
		w_composite_set_layout_deferred(W_COMPOSITE(this), defer);
	}
	static bool IsComposite(WWidget *widget) {
		return widget->GetClassId() >= _W_CLASS_COMPOSITE;
	}
	static WComposite* ToComposite(WWidget *widget) {
		if (IsComposite(widget))
			return (WComposite*) widget;
		else
			return 0;
	}
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnLayout(WEvent &e);
};

namespace swt {
typedef ::WComposite Composite;
}  // namespace swt

#endif /* SWTP_WIDGETS_COMPOSITE_H_ */
