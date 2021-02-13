/*
 * Tracker.h
 *
 *  Created on: 10 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_TRACKER_H_
#define SWTP_WIDGETS_TRACKER_H_
#include "Widget.h"
/**
 *  Instances of this class implement rubber banding rectangles that are
 *  drawn onto a parent <code>Composite</code> or <code>Display</code>.
 *  These rectangles can be specified to respond to mouse and key events
 *  by either moving or resizing themselves accordingly.  Trackers are
 *  typically used to represent window geometries in a lightweight manner.
 *
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>LEFT, RIGHT, UP, DOWN, RESIZE</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Move, Resize</dd>
 * </dl>
 * <p>
 * Note: Rectangle move behavior is assumed unless RESIZE is specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WTracker: public WWidget {
public:
	WTracker(){

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
	 * @see SWT#LEFT
	 * @see SWT#RIGHT
	 * @see SWT#UP
	 * @see SWT#DOWN
	 * @see SWT#RESIZE
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WTracker(WComposite *parent, int style) {
		Create(parent,style);
	}
	/**
	 * Constructs a new instance of this class given the display
	 * to create it on and a style value describing its behavior
	 * and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p><p>
	 * Note: Currently, null can be passed in for the display argument.
	 * This has the effect of creating the tracker on the currently active
	 * display if there is one. If there is no current display, the
	 * tracker is created on a "default" display. <b>Passing in null as
	 * the display argument is not considered to be good coding style,
	 * and may not be supported in a future release of SWT.</b>
	 * </p>
	 *
	 * @param display the display to create the tracker on
	 * @param style the style of control to construct
	 *
	 * @see SWT#LEFT
	 * @see SWT#RIGHT
	 * @see SWT#UP
	 * @see SWT#DOWN
	 * @see SWT#RESIZE
	 */
	WTracker(WToolkit *toolkit, int style) {
		Create(toolkit,style);
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
	 * @see SWT#LEFT
	 * @see SWT#RIGHT
	 * @see SWT#UP
	 * @see SWT#DOWN
	 * @see SWT#RESIZE
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WComposite *parent, int style) {
		return Create(0,parent,style);
	}
	/**
	 * Constructs a new instance of this class given the display
	 * to create it on and a style value describing its behavior
	 * and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p><p>
	 * Note: Currently, null can be passed in for the display argument.
	 * This has the effect of creating the tracker on the currently active
	 * display if there is one. If there is no current display, the
	 * tracker is created on a "default" display. <b>Passing in null as
	 * the display argument is not considered to be good coding style,
	 * and may not be supported in a future release of SWT.</b>
	 * </p>
	 *
	 * @param display the display to create the tracker on
	 * @param style the style of control to construct
	 *
	 * @see SWT#LEFT
	 * @see SWT#RIGHT
	 * @see SWT#UP
	 * @see SWT#DOWN
	 * @see SWT#RESIZE
	 */
	bool Create(WToolkit *toolkit, int style) {
		return Create(toolkit,0,style);
	}
	bool Create(WToolkit *toolkit, WComposite *parent, int style) {
		return w_tracker_create(W_TRACKER(this), (w_toolkit*) toolkit,
				W_COMPOSITE(parent), style, WWidget::post_event_proc) > 0;
	}
	/**
	 * Stops displaying the tracker rectangles.  Note that this is not considered
	 * to be a cancelation by the user.
	 */
	bool Close() {
		return w_tracker_close(W_TRACKER(this)) > 0;
	}
	/**
	 * Returns the bounds that are being drawn, expressed relative to the parent
	 * widget.  If the parent is a <code>Display</code> then these are screen
	 * coordinates.
	 *
	 * @return the bounds of the Rectangles being drawn
	 */
	void GetRectangles(WIterator<WRect> &rects) {
		w_tracker_get_rectangles(W_TRACKER(this), (w_iterator*) &rects);
	}
	/**
	 * Returns <code>true</code> if the rectangles are drawn with a stippled line, <code>false</code> otherwise.
	 *
	 * @return the stippled effect of the rectangles
	 */
	bool GetStippled() {
		return w_tracker_get_stippled(W_TRACKER(this));
	}
	/**
	 * Displays the Tracker rectangles for manipulation by the user.  Returns when
	 * the user has either finished manipulating the rectangles or has cancelled the
	 * Tracker.
	 *
	 * @return <code>true</code> if the user did not cancel the Tracker, <code>false</code> otherwise
	 */
	bool Open() {
		return w_tracker_open(W_TRACKER(this));
	}
	/**
	 * Sets the <code>Cursor</code> of the Tracker.  If this cursor is <code>null</code>
	 * then the cursor reverts to the default.
	 *
	 * @param newCursor the new <code>Cursor</code> to display
	 */
	bool SetCursor(WCursor *value) {
		return w_tracker_set_cursor(W_TRACKER(this), (w_cursor*) value) > 0;
	}
	/**
	 * Specifies the rectangles that should be drawn, expressed relative to the parent
	 * widget.  If the parent is a Display then these are screen coordinates.
	 *
	 * @param rectangles the bounds of the rectangles to be drawn
	 */
	bool SetRectangles(WRect *rectangles, size_t length) {
		return w_tracker_set_rectangles(W_TRACKER(this), (w_rect*) rectangles,
				length) > 0;
	}
	/**
	 * Changes the appearance of the line used to draw the rectangles.
	 *
	 * @param stippled <code>true</code> if rectangle should appear stippled
	 */
	bool SetStippled(bool stippled) {
		return w_tracker_set_stippled(W_TRACKER(this), stippled) > 0;
	}
private:
	void *handles[(sizeof(w_tracker) - sizeof(w_widget)) / sizeof(void*)];
};

#endif /* SWTP_WIDGETS_TRACKER_H_ */
