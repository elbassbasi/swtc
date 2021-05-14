/*
 * Control.h
 *
 *  Created on: 25 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_CONTROL_H_
#define SWTP_WIDGETS_CONTROL_H_
#include "Item.h"
class WLayout;
class WLayoutData;
/**
 * Control is the abstract superclass of all windowed user interface classes.
 * <p>
 * <dl>
 * <dt><b>Styles:</b>
 * <dd>BORDER</dd>
 * <dd>LEFT_TO_RIGHT, RIGHT_TO_LEFT, FLIP_TEXT_DIRECTION</dd>
 * <dt><b>Events:</b>
 * <dd>DragDetect, FocusIn, FocusOut, Help, KeyDown, KeyUp, MenuDetect, MouseDoubleClick, MouseDown, MouseEnter,
 *     MouseExit, MouseHover, MouseUp, MouseMove, MouseWheel, MouseHorizontalWheel, MouseVerticalWheel, Move,
 *     Paint, Resize, Traverse</dd>
 * </dl>
 * </p><p>
 * Only one of LEFT_TO_RIGHT or RIGHT_TO_LEFT may be specified.
 * </p>
 */
class SWTP_PUBLIC WControl: public WWidget {
public:
	friend class WLayout;
	typedef bool (WControl::*SelectionAction)(WEvent *e);
	WControl() {

	}
	virtual WResult Create(WToolkit *toolkit, WComposite *parent,
			wuint64 style);
	virtual bool Create(WComposite *parent, wuint64 style);
	bool Create(WComposite &parent, wuint64 style) {
		return Create(&parent, style);
	}
	virtual WControl* GetControl(WComposite *parent);
	bool GetGraphics(WGraphics &gc) {
		return _get_graphics(gc) > 0;
	}
	/**
	 * Returns the orientation of the receiver, which will be one of the
	 * constants <code>W_LEFT_TO_RIGHT</code> or <code>W_RIGHT_TO_LEFT</code>.
	 *
	 * @return the orientation style
	 */
	int GetOrientation() {
		int orientation = _get_orientation();
		return orientation > 0 ? orientation : 0;
	}
	/**
	 * Returns the text direction of the receiver, which will be one of the
	 * constants <code>W_LEFT_TO_RIGHT</code> or <code>W_RIGHT_TO_LEFT</code>.
	 *
	 * @return the text direction style
	 */
	int GetTextDirection() {
		int text_direction = _get_text_direction();
		return text_direction > 0 ? text_direction : 0;
	}
	/**
	 * Prints the receiver and all children.
	 *
	 * @param gc the gc where the drawing occurs
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise
	 */
	bool Print(WGraphics &gc) {
		return _print(gc) > 0;
	}
	/**
	 * Returns the preferred size (in points) of the receiver.
	 * <p>
	 * The <em>preferred size</em> of a control is the size that it would
	 * best be displayed at. The width hint and height hint arguments
	 * allow the caller to ask a control questions such as "Given a particular
	 * width, how high does the control need to be to show all of the contents?"
	 * To indicate that the caller does not wish to constrain a particular
	 * dimension, the constant <code>W_DEFAULT</code> is passed for the hint.
	 * </p>
	 *
	 * @param wHint the width hint (can be <code>W_DEFAULT</code>)
	 * @param hHint the height hint (can be <code>W_DEFAULT</code>)
	 * @return the preferred size of the control
	 *
	 * @see Layout
	 * @see #getBorderWidth
	 * @see #getBounds
	 * @see #getSize
	 * @see #pack(boolean)
	 * @see "computeTrim, getClientArea for controls that implement them"
	 */
	WSize& ComputeSize(WSize &result, int wHint, int hHint) {
		_compute_size(result, wHint, hHint);
		return result;
	}
	WSize& ComputeSize(WSize &result) {
		return ComputeSize(result, W_DEFAULT, W_DEFAULT);
	}
	/**
	 * Returns the preferred size (in points) of the receiver.
	 * <p>
	 * The <em>preferred size</em> of a control is the size that it would
	 * best be displayed at. The width hint and height hint arguments
	 * allow the caller to ask a control questions such as "Given a particular
	 * width, how high does the control need to be to show all of the contents?"
	 * To indicate that the caller does not wish to constrain a particular
	 * dimension, the constant <code>W_DEFAULT</code> is passed for the hint.
	 * </p>
	 *
	 * @param wHint the width hint (can be <code>W_DEFAULT</code>)
	 * @param hHint the height hint (can be <code>W_DEFAULT</code>)
	 * @return the preferred size of the control
	 * @see Layout
	 * @see #getBorderWidth
	 * @see #getBounds
	 * @see #getSize
	 * @see #pack(boolean)
	 * @see "computeTrim, getClientArea for controls that implement them"
	 */
	WSize ComputeSize(int wHint, int hHint) {
		WSize result;
		return ComputeSize(result, wHint, hHint);
	}
	WSize ComputeSize() {
		WSize result;
		return ComputeSize(result);
	}
	/**
	 * Returns the accessible object for the receiver.
	 * <p>
	 * If this is the first time this object is requested,
	 * then the object is created and returned. The object
	 * returned by getAccessible() does not need to be disposed.
	 * </p>
	 *
	 * @return the accessible object
	 * @see Accessible#addAccessibleListener
	 * @see Accessible#addAccessibleControlListener
	 */
	WAccessible* GetAccessible() {
		WAccessible *accessible;
		_get_accessible(accessible);
		return accessible;
	}
	/**
	 * Returns a rectangle describing the receiver's size and location in points
	 * relative to its parent (or its display if its parent is null),
	 * unless the receiver is a shell. In this case, the location is
	 * relative to the display.
	 *
	 * @return the receiver's bounding rectangle
	 */
	WRect& GetBounds(WRect &bounds) {
		_get_bounds(&bounds.GetOrigin(), &bounds.GetSize());
		return bounds;
	}
	/**
	 * Returns a rectangle describing the receiver's size and location in points
	 * relative to its parent (or its display if its parent is null),
	 * unless the receiver is a shell. In this case, the location is
	 * relative to the display.
	 *
	 * @return the receiver's bounding rectangle
	 */
	WRect GetBounds() {
		WRect bounds;
		return GetBounds(bounds);
	}
	/**
	 * Sets the receiver's size and location in points to the rectangular
	 * area specified by the argument. The <code>x</code> and
	 * <code>y</code> fields of the rectangle are relative to
	 * the receiver's parent (or its display if its parent is null).
	 * <p>
	 * Note: Attempting to set the width or height of the
	 * receiver to a negative number will cause that
	 * value to be set to zero instead.
	 * </p>
	 * <p>
	 * Note: On GTK, attempting to set the width or height of the
	 * receiver to a number higher or equal 2^14 will cause them to be
	 * set to (2^14)-1 instead.
	 * </p>
	 *
	 * @param rect the new bounds for the receiver
	 */
	void SetBounds(const WRect &rect) {
		_set_bounds(&rect.GetOrigin(), &rect.GetSize());
	}
	/**
	 * Sets the receiver's size and location in points to the rectangular
	 * area specified by the arguments. The <code>x</code> and
	 * <code>y</code> arguments are relative to the receiver's
	 * parent (or its display if its parent is null), unless
	 * the receiver is a shell. In this case, the <code>x</code>
	 * and <code>y</code> arguments are relative to the display.
	 * <p>
	 * Note: Attempting to set the width or height of the
	 * receiver to a negative number will cause that
	 * value to be set to zero instead.
	 * </p>
	 * <p>
	 * Note: On GTK, attempting to set the width or height of the
	 * receiver to a number higher or equal 2^14 will cause them to be
	 * set to (2^14)-1 instead.
	 * </p>
	 *
	 * @param x the new x coordinate for the receiver
	 * @param y the new y coordinate for the receiver
	 * @param width the new width for the receiver
	 * @param height the new height for the receiver
	 */
	void SetBounds(int x, int y, int width, int height) {
		SetBounds(WRect(x, y, width, height));
	}
	/**
	 * Returns a point describing the receiver's location relative
	 * to its parent in points (or its display if its parent is null), unless
	 * the receiver is a shell. In this case, the point is
	 * relative to the display.
	 *
	 * @return the receiver's location
	 */
	WPoint& GetLocation(WPoint &location) {
		_get_bounds(&location, 0);
		return location;
	}
	/**
	 * Returns a point describing the receiver's location relative
	 * to its parent in points (or its display if its parent is null), unless
	 * the receiver is a shell. In this case, the point is
	 * relative to the display.
	 *
	 * @return the receiver's location
	 */
	WPoint GetLocation() {
		WPoint location;
		_get_bounds(&location, 0);
		return location;
	}
	/**
	 * Sets the receiver's location to the point specified by
	 * the arguments which are relative to the receiver's
	 * parent (or its display if its parent is null), unless
	 * the receiver is a shell. In this case, the point is
	 * relative to the display.
	 *
	 * @param location the new location for the receiver
	 */
	void SetLocation(const WPoint &location) {
		_set_bounds(&location, 0);
	}
	/**
	 * Sets the receiver's location to the point specified by
	 * the arguments which are relative to the receiver's
	 * parent (or its display if its parent is null), unless
	 * the receiver is a shell. In this case, the point is
	 * relative to the display.
	 *
	 * @param x the new x coordinate for the receiver
	 * @param y the new y coordinate for the receiver
	 */
	void SetLocation(int x, int y) {
		WPoint location(x, y);
		SetLocation(location);
	}
	/**
	 * Returns a point describing the receiver's size in points. The
	 * x coordinate of the result is the width of the receiver.
	 * The y coordinate of the result is the height of the
	 * receiver.
	 *
	 * @return the receiver's size
	 */
	WSize& GetSize(WSize &size) {
		_get_bounds(0, &size);
		return size;
	}
	/**
	 * Returns a point describing the receiver's size in points. The
	 * x coordinate of the result is the width of the receiver.
	 * The y coordinate of the result is the height of the
	 * receiver.
	 *
	 * @return the receiver's size
	 */
	WSize GetSize() {
		WSize size;
		_get_bounds(0, &size);
		return size;
	}
	/**
	 * Sets the receiver's size to the point specified by the argument.
	 * <p>
	 * Note: Attempting to set the width or height of the
	 * receiver to a negative number will cause them to be
	 * set to zero instead.
	 * </p>
	 * <p>
	 * Note: On GTK, attempting to set the width or height of the
	 * receiver to a number higher or equal 2^14 will cause them to be
	 * set to (2^14)-1 instead.
	 * </p>
	 *
	 * @param size the new size in points for the receiver
	 */
	void SetSize(const WSize &size) {
		_set_bounds(0, &size);
	}
	/**
	 * Sets the receiver's size to the point specified by the arguments.
	 * <p>
	 * Note: Attempting to set the width or height of the
	 * receiver to a negative number will cause that
	 * value to be set to zero instead.
	 * </p>
	 * <p>
	 * Note: On GTK, attempting to set the width or height of the
	 * receiver to a number higher or equal 2^14 will cause them to be
	 * set to (2^14)-1 instead.
	 * </p>
	 *
	 * @param width the new width in points for the receiver
	 * @param height the new height in points for the receiver
	 */
	void SetSize(int width, int height) {
		WSize sz(width, height);
		SetSize(sz);
	}
	/**
	 * Sets the shape of the control to the region specified
	 * by the argument.  When the argument is null, the
	 * default shape of the control is restored.
	 *
	 * @param region the region that defines the shape of the control (or null)
	 *
	 * @since 3.4
	 */
	void SetRegion(const WRegion &region) {
		_set_region(region);
	}
	/**
	 * Moves the receiver above the specified control in the
	 * drawing order. If the argument is null, then the receiver
	 * is moved to the top of the drawing order. The control at
	 * the top of the drawing order will not be covered by other
	 * controls even if they occupy intersecting areas.
	 *
	 * @param control the sibling control (or null)
	 *
	 * @see Control#moveBelow
	 * @see Composite#getChildren
	 */
	void MoveAbove(WControl *control) {
		_move_above(control);
	}
	/**
	 * Moves the receiver below the specified control in the
	 * drawing order. If the argument is null, then the receiver
	 * is moved to the bottom of the drawing order. The control at
	 * the bottom of the drawing order will be covered by all other
	 * controls which occupy intersecting areas.
	 *
	 * @param control the sibling control (or null)
	 *
	 * @see Control#moveAbove
	 * @see Composite#getChildren
	 */
	void MoveBelow(WControl *control) {
		_move_below(control);
	}
	/**
	 * Causes the receiver to be resized to its preferred size.
	 * For a composite, this involves computing the preferred size
	 * from its layout, if there is one.
	 * <p>
	 * If the changed flag is <code>true</code>, it indicates that the receiver's
	 * <em>contents</em> have changed, therefore any caches that a layout manager
	 * containing the control may have been keeping need to be flushed. When the
	 * control is resized, the changed flag will be <code>false</code>, so layout
	 * manager caches can be retained.
	 * </p>
	 *
	 * @param changed whether or not the receiver's contents have changed
	 *
	 * @see #computeSize(int, int, boolean)
	 */
	void Pack(bool changed = true) {
		_pack(changed);
	}
	/**
	 * Returns a point which is the result of converting the
	 * argument, which is specified in display relative coordinates,
	 * to coordinates relative to the receiver.
	 * <p>
	 * NOTE: To properly map a rectangle or a corner of a rectangle on a right-to-left platform, use
	 * {@link Display#map(Control, Control, Rectangle)}.
	 * </p>
	 *
	 * @param x the x coordinate in points to be translated
	 * @param y the y coordinate in points to be translated
	 * @return the translated coordinates
	 */
	WPoint& ToControl(WPoint &result, int x, int y) {
		WPoint pt(x, y);
		return ToControl(result, pt);
	}
	/**
	 * Returns a point which is the result of converting the
	 * argument, which is specified in display relative coordinates,
	 * to coordinates relative to the receiver.
	 * <p>
	 * NOTE: To properly map a rectangle or a corner of a rectangle on a right-to-left platform, use
	 * {@link Display#map(Control, Control, Rectangle)}.
	 * </p>
	 *
	 * @param point the point to be translated (must not be null)
	 * @return the translated coordinates
	 */
	WPoint& ToControl(WPoint &result, const WPoint &point) {
		_to_control(result, point);
		return result;
	}
	/**
	 * Returns a point which is the result of converting the
	 * argument, which is specified in display relative coordinates,
	 * to coordinates relative to the receiver.
	 * <p>
	 * NOTE: To properly map a rectangle or a corner of a rectangle on a right-to-left platform, use
	 * {@link Display#map(Control, Control, Rectangle)}.
	 * </p>
	 *
	 * @param x the x coordinate in points to be translated
	 * @param y the y coordinate in points to be translated
	 * @return the translated coordinates
	 */
	WPoint ToControl(int x, int y) {
		WPoint result;
		return ToControl(result, x, y);
	}
	/**
	 * Returns a point which is the result of converting the
	 * argument, which is specified in display relative coordinates,
	 * to coordinates relative to the receiver.
	 * <p>
	 * NOTE: To properly map a rectangle or a corner of a rectangle on a right-to-left platform, use
	 * {@link Display#map(Control, Control, Rectangle)}.
	 * </p>
	 *
	 * @param point the point to be translated (must not be null)
	 * @return the translated coordinates
	 */
	WPoint ToControl(const WPoint &point) {
		WPoint result;
		ToControl(result, point);
		return result;
	}
	/**
	 * Returns a point which is the result of converting the
	 * argument, which is specified in coordinates relative to
	 * the receiver, to display relative coordinates.
	 * <p>
	 * NOTE: To properly map a rectangle or a corner of a rectangle on a right-to-left platform, use
	 * {@link Display#map(Control, Control, Rectangle)}.
	 * </p>
	 *
	 * @param x the x coordinate to be translated
	 * @param y the y coordinate to be translated
	 * @return the translated coordinates
	 */
	WPoint& ToDisplay(WPoint &result, int x, int y) {
		WPoint pt(x, y);
		return ToDisplay(result, pt);
	}
	/**
	 * Returns a point which is the result of converting the
	 * argument, which is specified in coordinates relative to
	 * the receiver, to display relative coordinates.
	 * <p>
	 * NOTE: To properly map a rectangle or a corner of a rectangle on a right-to-left platform, use
	 * {@link Display#map(Control, Control, Rectangle)}.
	 * </p>
	 *
	 * @param point the point to be translated (must not be null)
	 * @return the translated coordinates
	 */
	WPoint& ToDisplay(WPoint &result, const WPoint &point) {
		_to_display(result, point);
		return result;
	}
	/**
	 * Returns a point which is the result of converting the
	 * argument, which is specified in coordinates relative to
	 * the receiver, to display relative coordinates.
	 * <p>
	 * NOTE: To properly map a rectangle or a corner of a rectangle on a right-to-left platform, use
	 * {@link Display#map(Control, Control, Rectangle)}.
	 * </p>
	 *
	 * @param x the x coordinate to be translated
	 * @param y the y coordinate to be translated
	 * @return the translated coordinates
	 */
	WPoint ToDisplay(int x, int y) {
		WPoint result;
		return ToDisplay(result, x, y);
	}
	/**
	 * Returns a point which is the result of converting the
	 * argument, which is specified in coordinates relative to
	 * the receiver, to display relative coordinates.
	 * <p>
	 * NOTE: To properly map a rectangle or a corner of a rectangle on a right-to-left platform, use
	 * {@link Display#map(Control, Control, Rectangle)}.
	 * </p>
	 *
	 * @param point the point to be translated (must not be null)
	 * @return the translated coordinates
	 */
	WPoint ToDisplay(const WPoint &point) {
		WPoint result;
		return ToDisplay(result, point);
	}
	/**
	 * Detects a drag and drop gesture.  This method is used
	 * to detect a drag gesture when called from within a mouse
	 * down listener.
	 *
	 * <p>By default, a drag is detected when the gesture
	 * occurs anywhere within the client area of a control.
	 * Some controls, such as tables and trees, override this
	 * behavior.  In addition to the operating system specific
	 * drag gesture, they require the mouse to be inside an
	 * item.  Custom widget writers can use <code>setDragDetect</code>
	 * to disable the default detection, listen for mouse down,
	 * and then call <code>dragDetect()</code> from within the
	 * listener to conditionally detect a drag.
	 * </p>
	 *
	 * @param event the mouse down event
	 *
	 * @return <code>true</code> if the gesture occurred, and <code>false</code> otherwise.
	 *
	 * @see DragDetectListener
	 * @see #addDragDetectListener
	 *
	 * @see #getDragDetect
	 * @see #setDragDetect
	 *
	 * @since 3.3
	 */
	bool DragDetect(WMouseEvent &event) {
		return _drag_detect(event) > 0;
	}
	/**
	 * Forces the receiver to have the <em>keyboard focus</em>, causing
	 * all keyboard events to be delivered to it.
	 *
	 * @return <code>true</code> if the control got focus, and <code>false</code> if it was unable to.
	 *
	 * @see #setFocus
	 */
	bool ForceFocus() {
		return _force_focus();
	}
	/**
	 * Returns the receiver's background color.
	 * <p>
	 * Note: This operation is a hint and may be overridden by the platform.
	 * For example, on some versions of Windows the background of a TabFolder,
	 * is a gradient rather than a solid color.
	 * </p>
	 * @return the background color
	 */
	WColor GetBackground() {
		w_color background;
		_get_background(&background);
		return background;
	}
	/**
	 * Returns the receiver's border width in points.
	 *
	 * @return the border width
	 */
	int GetBorderWidth() {
		int border_width = _get_border_width();
		return border_width > 0 ? border_width : 0;
	}
	/**
	 * Returns the receiver's cursor, or null if it has not been set.
	 * <p>
	 * When the mouse pointer passes over a control its appearance
	 * is changed to match the control's cursor.
	 * </p>
	 *
	 * @return the receiver's cursor or <code>null</code>
	 */
	WCursor* GetCursor() {
		WCursor *cursor;
		_get_cursor(&cursor);
		return cursor;
	}
	/**
	 * Returns <code>true</code> if the receiver is detecting
	 * drag gestures, and  <code>false</code> otherwise.
	 *
	 * @return the receiver's drag detect state
	 */
	bool GetDragDetect() {
		return _get_drag_detect() > 0;
	}
	/**
	 * Returns <code>true</code> if the receiver is enabled, and
	 * <code>false</code> otherwise. A disabled control is typically
	 * not selectable from the user interface and draws with an
	 * inactive or "grayed" look.
	 *
	 * @return the receiver's enabled state
	 *
	 * @see #isEnabled
	 */
	bool GetEnabled() {
		return _get_enabled() > 0;
	}
	/**
	 * Returns the font that the receiver will use to paint textual information.
	 *
	 * @return the receiver's font
	 */
	WFont* GetFont() {
		WFont *font;
		_get_font(&font);
		return font;
	}
	/**
	 * Returns the foreground color that the receiver will use to draw.
	 *
	 * @return the receiver's foreground color
	 */
	WColor GetForeground() {
		w_color foreground;
		_get_foreground(&foreground);
		return foreground;
	}
	/**
	 * Returns the receiver's pop up menu if it has one, or null
	 * if it does not. All controls may optionally have a pop up
	 * menu that is displayed when the user requests one for
	 * the control. The sequence of key strokes, button presses
	 * and/or button releases that are used to request a pop up
	 * menu is platform specific.
	 *
	 * @return the receiver's menu
	 */
	WMenu* GetMenu() {
		WMenu *menu;
		_get_menu(&menu);
		return menu;
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Composite</code>
	 * or null when the receiver is a shell that was created with null or
	 * a display for a parent.
	 *
	 * @return the receiver's parent
	 */
	WComposite* GetParent() {
		WComposite *parent;
		_get_parent(&parent);
		return parent;
	}
	/**
	 * Returns the region that defines the shape of the control,
	 * or null if the control has the default shape.
	 *
	 * @return the region that defines the shape of the shell (or null)
	 */
	bool GetRegion(WRegion &region) {
		return _get_region(region) > 0;
	}
	/**
	 * Returns the receiver's shell. For all controls other than
	 * shells, this simply returns the control's nearest ancestor
	 * shell. Shells return themselves, even if they are children
	 * of other shells.
	 *
	 * @return the receiver's shell
	 *
	 * @see #getParent
	 */
	WFrame* GetFrame() {
		WFrame *frame;
		_get_shell(&frame);
		return frame;
	}
	/**
	 * Returns the receiver's tool tip text, or null if it has
	 * not been set.
	 *
	 * @return the receiver's tool tip text
	 */
	WString GetToolTipText() {
		WString str;
		_get_tooltip_text(w_alloc_string_ref, &str.ref, W_ENCODING_UTF8);
		return str;
	}
	/**
	 * Returns <code>true</code> if this control is set to send touch events, or
	 * <code>false</code> if it is set to send gesture events instead.  This method
	 * also returns <code>false</code> if a touch-based input device is not detected
	 * (this can be determined with <code>Display#getTouchEnabled()</code>).  Use
	 * {@link #setTouchEnabled(boolean)} to switch the events that a control sends
	 * between touch events and gesture events.
	 *
	 * @return <code>true</code> if the control is set to send touch events, or <code>false</code> otherwise
	 *
	 * @see #setTouchEnabled
	 * @see Display#getTouchEnabled
	 *
	 * @since 3.7
	 */
	bool GetTouchEnabled() {
		return _get_touch_enabled() > 0;
	}
	/**
	 * Returns <code>true</code> if the receiver is visible, and
	 * <code>false</code> otherwise.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, this method
	 * may still indicate that it is considered visible even though
	 * it may not actually be showing.
	 * </p>
	 *
	 * @return the receiver's visibility state
	 */
	bool GetVisible() {
		return _get_visible() > 0;
	}
	/**
	 * Returns <code>true</code> if the underlying operating
	 * system supports this reparenting, otherwise <code>false</code>
	 *
	 * @return <code>true</code> if the widget can be reparented, otherwise <code>false</code>
	 */
	bool IsReparentable() {
		return _is_reparentable() > 0;
	}
	/**
	 * Returns <code>true</code> if the receiver is enabled and all
	 * ancestors up to and including the receiver's nearest ancestor
	 * shell are enabled.  Otherwise, <code>false</code> is returned.
	 * A disabled control is typically not selectable from the user
	 * interface and draws with an inactive or "grayed" look.
	 *
	 * @return the receiver's enabled state
	 *
	 * @see #getEnabled
	 */
	bool IsEnabled() {
		return _is_enabled() > 0;
	}
	/**
	 * Returns <code>true</code> if the receiver has the user-interface
	 * focus, and <code>false</code> otherwise.
	 *
	 * @return the receiver's focus state
	 */
	bool IsFocusControl() {
		return _is_focus_control() > 0;
	}
	/**
	 * Returns <code>true</code> if the receiver is visible and all
	 * ancestors up to and including the receiver's nearest ancestor
	 * shell are visible. Otherwise, <code>false</code> is returned.
	 *
	 * @return the receiver's visibility state
	 *
	 * @see #getVisible
	 */
	bool IsVisible() {
		return _is_visible() > 0;
	}
	bool KillTimer(wushort id = 1) {
		return _kill_timer(id) > 0;
	}
	/**
	 * Requests that this control and all of its ancestors be repositioned by
	 * their layouts at the earliest opportunity. This should be invoked after
	 * modifying the control in order to inform any dependent layouts of
	 * the change.
	 * <p>
	 * The control will not be repositioned synchronously. This method is
	 * fast-running and only marks the control for future participation in
	 * a deferred layout.
	 * <p>
	 * Invoking this method multiple times before the layout occurs is an
	 * inexpensive no-op.
	 *
	 */
	void RequestLayout() {
		_request_layout();
	}

	/**
	 * Causes the entire bounds of the receiver to be marked
	 * as needing to be redrawn. The next time a paint request
	 * is processed, the control will be completely painted,
	 * including the background.
	 *
	 * @see #update()
	 * @see PaintListener
	 * @see SWT#Paint
	 * @see SWT#NO_BACKGROUND
	 * @see SWT#NO_REDRAW_RESIZE
	 * @see SWT#NO_MERGE_PAINTS
	 * @see SWT#DOUBLE_BUFFERED
	 */
	void Redraw() {
		_redraw(0, false);
	}
	/**
	 * Causes the rectangular area of the receiver specified by
	 * the arguments to be marked as needing to be redrawn.
	 * The next time a paint request is processed, that area of
	 * the receiver will be painted, including the background.
	 * If the <code>all</code> flag is <code>true</code>, any
	 * children of the receiver which intersect with the specified
	 * area will also paint their intersecting areas. If the
	 * <code>all</code> flag is <code>false</code>, the children
	 * will not be painted.
	 *
	 * @param x the x coordinate of the area to draw
	 * @param y the y coordinate of the area to draw
	 * @param width the width of the area to draw
	 * @param height the height of the area to draw
	 * @param all <code>true</code> if children should redraw, and <code>false</code> otherwise
	 *
	 * @see #update()
	 * @see PaintListener
	 * @see SWT#Paint
	 * @see SWT#NO_BACKGROUND
	 * @see SWT#NO_REDRAW_RESIZE
	 * @see SWT#NO_MERGE_PAINTS
	 * @see SWT#DOUBLE_BUFFERED
	 */
	void Redraw(const WRect &rect, bool all) {
		_redraw(&rect, all);
	}
	/**
	 * Causes the rectangular area of the receiver specified by
	 * the arguments to be marked as needing to be redrawn.
	 * The next time a paint request is processed, that area of
	 * the receiver will be painted, including the background.
	 * If the <code>all</code> flag is <code>true</code>, any
	 * children of the receiver which intersect with the specified
	 * area will also paint their intersecting areas. If the
	 * <code>all</code> flag is <code>false</code>, the children
	 * will not be painted.
	 *
	 * @param x the x coordinate of the area to draw
	 * @param y the y coordinate of the area to draw
	 * @param width the width of the area to draw
	 * @param height the height of the area to draw
	 * @param all <code>true</code> if children should redraw, and <code>false</code> otherwise
	 *
	 * @see #update()
	 * @see PaintListener
	 * @see SWT#Paint
	 * @see SWT#NO_BACKGROUND
	 * @see SWT#NO_REDRAW_RESIZE
	 * @see SWT#NO_MERGE_PAINTS
	 * @see SWT#DOUBLE_BUFFERED
	 */
	void Redraw(int x, int y, int width, int height, bool all) {
		WRect r;
		r.x = x;
		r.y = y;
		r.width = width;
		r.height = height;
		Redraw(r, all);
	}
	/**
	 * Sets the receiver's background color to the color specified
	 * by the argument, or to the default system color for the control
	 * if the argument is null.
	 * <p>
	 * Note: This operation is a hint and may be overridden by the platform.
	 * For example, on MAC the background of a Button cannot be changed.
	 * </p>
	 * @param color the new color (or null)
	 */
	void SetBackground(WColor color) {
		_set_background(color.color);
	}
	/**
	 * If the argument is <code>true</code>, causes the receiver to have
	 * all mouse events delivered to it until the method is called with
	 * <code>false</code> as the argument.  Note that on some platforms,
	 * a mouse button must currently be down for capture to be assigned.
	 *
	 * @param capture <code>true</code> to capture the mouse, and <code>false</code> to release it
	 */
	void SetCapture(bool capture) {
		_set_capture(capture);
	}
	/**
	 * Sets the receiver's cursor to the cursor specified by the
	 * argument, or to the default cursor for that kind of control
	 * if the argument is null.
	 * <p>
	 * When the mouse pointer passes over a control its appearance
	 * is changed to match the control's cursor.
	 * </p>
	 *
	 * @param cursor the new cursor (or null)
	 */
	void SetCursor(WCursor *cursor) {
		_set_cursor(cursor);
	}
	/**
	 * Sets the receiver's drag detect state. If the argument is
	 * <code>true</code>, the receiver will detect drag gestures,
	 * otherwise these gestures will be ignored.
	 *
	 * @param dragDetect the new drag detect state
	 */
	void SetDragDetect(bool dragDetect) {
		_set_drag_detect(dragDetect);
	}
	/**
	 * Enables the receiver if the argument is <code>true</code>,
	 * and disables it otherwise. A disabled control is typically
	 * not selectable from the user interface and draws with an
	 * inactive or "grayed" look.
	 *
	 * @param enabled the new enabled state
	 */
	void SetEnabled(bool enabled) {
		_set_enabled(enabled);
	}
	/**
	 * Causes the receiver to have the <em>keyboard focus</em>,
	 * such that all keyboard events will be delivered to it.  Focus
	 * reassignment will respect applicable platform constraints.
	 *
	 * @return <code>true</code> if the control got focus, and <code>false</code> if it was unable to.
	 *
	 * @see #forceFocus
	 */
	bool SetFocus() {
		return _set_focus() > 0;
	}
	/**
	 * Sets the font that the receiver will use to paint textual information
	 * to the font specified by the argument, or to the default font for that
	 * kind of control if the argument is null.
	 *
	 * @param font the new font (or null)
	 */
	void SetFont(WFont *font) {
		_set_font(font);
	}
	/**
	 * Sets the receiver's foreground color to the color specified
	 * by the argument, or to the default system color for the control
	 * if the argument is null.
	 * <p>
	 * Note: This operation is a hint and may be overridden by the platform.
	 * </p>
	 * @param color the new color (or null)
	 */
	void SetForeground(WColor color) {
		_set_foreground(color.color);
	}
	/**
	 * Sets the receiver's pop up menu to the argument.
	 * All controls may optionally have a pop up
	 * menu that is displayed when the user requests one for
	 * the control. The sequence of key strokes, button presses
	 * and/or button releases that are used to request a pop up
	 * menu is platform specific.
	 * <p>
	 * Note: Disposing of a control that has a pop up menu will
	 * dispose of the menu.  To avoid this behavior, set the
	 * menu to null before the control is disposed.
	 * </p>
	 *
	 * @param menu the new pop up menu
	 */
	void SetMenu(WMenu *menu) {
		_set_menu(menu);
	}
	/**
	 * Sets the orientation of the receiver, which must be one
	 * of the constants <code>W_LEFT_TO_RIGHT</code> or <code>W_RIGHT_TO_LEFT</code>.
	 * <p>
	 *
	 * @param orientation new orientation style
	 */
	void SetOrientation(int orientation) {
		_set_orientation(orientation);
	}
	/**
	 * Changes the parent of the widget to be the one provided if
	 * the underlying operating system supports this feature.
	 * Returns <code>true</code> if the parent is successfully changed.
	 *
	 * @param parent the new parent for the control.
	 * @return <code>true</code> if the parent is changed and <code>false</code> otherwise.
	 */
	bool SetParent(WComposite *parent) {
		return _set_parent(parent) > 0;
	}
	/**
	 * If the argument is <code>false</code>, causes subsequent drawing
	 * operations in the receiver to be ignored. No drawing of any kind
	 * can occur in the receiver until the flag is set to true.
	 * Graphics operations that occurred while the flag was
	 * <code>false</code> are lost. When the flag is set to <code>true</code>,
	 * the entire widget is marked as needing to be redrawn.  Nested calls
	 * to this method are stacked.
	 * <p>
	 * Note: This operation is a hint and may not be supported on some
	 * platforms or for some widgets.
	 * </p>
	 *
	 * @param redraw the new redraw state
	 *
	 * @see #redraw(int, int, int, int, boolean)
	 * @see #update()
	 */
	void SetRedraw(bool redraw) {
		_set_redraw(redraw);
	}
	/**
	 * Sets the base text direction (a.k.a. "paragraph direction") of the receiver,
	 * which must be one of the constants <code>W_LEFT_TO_RIGHT</code>,
	 * <code>W_RIGHT_TO_LEFT</code>, or <code>W_AUTO_TEXT_DIRECTION</code>.
	 * <p>
	 * <code>setOrientation</code> would override this value with the text direction
	 * that is consistent with the new orientation.
	 * </p>
	 * <p>
	 * <b>Warning</b>: This API is currently only implemented on Windows.
	 * It doesn't set the base text direction on GTK and Cocoa.
	 * </p>
	 *
	 * @param textDirection the base text direction style
	 *
	 * @see SWT#LEFT_TO_RIGHT
	 * @see SWT#RIGHT_TO_LEFT
	 * @see SWT#AUTO_TEXT_DIRECTION
	 * @see SWT#FLIP_TEXT_DIRECTION
	 */
	void SetTextDirection(int textDirection) {
		_set_text_direction(textDirection);
	}
	bool SetTimer(wint64 ms, wushort id = 1) {
		return _set_timer(ms, id) > 0;
	}
	/**
	 * Sets the receiver's tool tip text to the argument, which
	 * may be null indicating that the default tool tip for the
	 * control will be shown. For a control that has a default
	 * tool tip, such as the Tree control on Windows, setting
	 * the tool tip text to an empty string replaces the default,
	 * causing no tool tip text to be shown.
	 * <p>
	 * The mnemonic indicator (character '&amp;') is not displayed in a tool tip.
	 * To display a single '&amp;' in the tool tip, the character '&amp;' can be
	 * escaped by doubling it in the string.
	 * </p>
	 *
	 * @param string the new tool tip text (or null)
	 */
	void SetToolTipText(const char *string, int length) {
		_set_tooltip_text(string, length, W_ENCODING_UTF8);
	}
	/**
	 * Sets the receiver's tool tip text to the argument, which
	 * may be null indicating that the default tool tip for the
	 * control will be shown. For a control that has a default
	 * tool tip, such as the Tree control on Windows, setting
	 * the tool tip text to an empty string replaces the default,
	 * causing no tool tip text to be shown.
	 * <p>
	 * The mnemonic indicator (character '&amp;') is not displayed in a tool tip.
	 * To display a single '&amp;' in the tool tip, the character '&amp;' can be
	 * escaped by doubling it in the string.
	 * </p>
	 *
	 * @param string the new tool tip text (or null)
	 */
	void SetToolTipText(const char *string) {
		return SetToolTipText(string, -1);
	}
	/**
	 * Sets whether this control should send touch events (by default controls do not).
	 * Setting this to <code>false</code> causes the receiver to send gesture events
	 * instead.  No exception is thrown if a touch-based input device is not
	 * detected (this can be determined with <code>Display#getTouchEnabled()</code>).
	 *
	 * @param enabled the new touch-enabled state
	 * @see Display#getTouchEnabled
	 */
	void SetTouchEnabled(bool enabled) {
		_set_touch_enabled(enabled);
	}
	/**
	 * Marks the receiver as visible if the argument is <code>true</code>,
	 * and marks it invisible otherwise.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, marking
	 * it visible may not actually cause it to be displayed.
	 * </p>
	 *
	 * @param visible the new visibility state
	 */
	void SetVisible(bool visible) {
		_set_visible(visible);
	}
	/**
	 * Based on the argument, perform one of the expected platform
	 * traversal action. The argument should be one of the constants:
	 * <code>W_TRAVERSE_ESCAPE</code>, <code>W_TRAVERSE_RETURN</code>,
	 * <code>W_TRAVERSE_TAB_NEXT</code>, <code>W_TRAVERSE_TAB_PREVIOUS</code>,
	 * <code>W_TRAVERSE_ARROW_NEXT</code>, <code>W_TRAVERSE_ARROW_PREVIOUS</code>,
	 * <code>W_TRAVERSE_PAGE_NEXT</code> and <code>W_TRAVERSE_PAGE_PREVIOUS</code>.
	 *
	 * @param traversal the type of traversal
	 * @return true if the traversal succeeded
	 */
	bool Traverse(int traversal) {
		return _traverse(traversal, 0) > 0;
	}
	/**
	 * Performs a platform traversal action corresponding to a <code>KeyDown</code> event.
	 *
	 * <p>Valid traversal values are
	 * <code>W_TRAVERSE_NONE</code>, <code>W_TRAVERSE_MNEMONIC</code>,
	 * <code>W_TRAVERSE_ESCAPE</code>, <code>W_TRAVERSE_RETURN</code>,
	 * <code>W_TRAVERSE_TAB_NEXT</code>, <code>W_TRAVERSE_TAB_PREVIOUS</code>,
	 * <code>W_TRAVERSE_ARROW_NEXT</code>, <code>W_TRAVERSE_ARROW_PREVIOUS</code>,
	 * <code>W_TRAVERSE_PAGE_NEXT</code> and <code>W_TRAVERSE_PAGE_PREVIOUS</code>.
	 * If <code>traversal</code> is <code>W_TRAVERSE_NONE</code> then the Traverse
	 * event is created with standard values based on the KeyDown event.  If
	 * <code>traversal</code> is one of the other traversal constants then the Traverse
	 * event is created with this detail, and its <code>doit</code> is taken from the
	 * KeyDown event.
	 * </p>
	 *
	 * @param traversal the type of traversal, or <code>W_TRAVERSE_NONE</code> to compute
	 * this from <code>event</code>
	 * @param event the KeyDown event
	 *
	 * @return <code>true</code> if the traversal succeeded
	 */
	bool Traverse(int traversal, WKeyEvent *event) {
		return _traverse(traversal, event);
	}
	/**
	 * Forces all outstanding paint requests for the widget
	 * to be processed before this method returns. If there
	 * are no outstanding paint request, this method does
	 * nothing.
	 * <p>
	 * Note: This method does not cause a redraw.
	 * </p>
	 *
	 * @see #redraw()
	 * @see #redraw(int, int, int, int, boolean)
	 * @see PaintListener
	 * @see SWT#Paint
	 */
	bool Update() {
		return _update() > 0;
	}
	void SetLayoutData(const WLayoutData &data) {
		_layout_set_data(data);
	}
	void GetLayoutData(WLayoutData &data) {
		_layout_get_data(data);
	}
public:
	static bool IsControl(WWidget *widget) {
		return widget->GetClassId() >= _W_CLASS_CONTROL;
	}
	static WControl* ToControl(WWidget *widget) {
		if (IsControl(widget))
			return (WControl*) widget;
		else
			return 0;
	}
protected:
	bool PostEvent(WEvent *e);
	virtual bool OnResize(WEvent &e);
	virtual bool OnMove(WEvent &e);
	virtual bool OnDragDetect(WMouseEvent &e);
	virtual bool OnFocusIn(WEvent &e);
	virtual bool OnFocusOut(WEvent &e);
	virtual bool OnHelp(WEvent &e);
	virtual bool OnKeyUp(WKeyEvent &e);
	virtual bool OnKeyDown(WKeyEvent &e);
	virtual bool OnTraverse(WKeyEvent &e);
	virtual bool OnMenuDetect(WMenuDetectEvent &e);
	virtual bool OnMouseDown(WMouseEvent &e);
	virtual bool OnMouseUp(WMouseEvent &e);
	virtual bool OnMouseDoubleClick(WMouseEvent &e);
	virtual bool OnMouseEnter(WMouseEvent &e);
	virtual bool OnMouseExit(WMouseEvent &e);
	virtual bool OnMouseHover(WMouseEvent &e);
	virtual bool OnMouseMove(WMouseEvent &e);
	virtual bool OnMouseWheel(WMouseEvent &e);
	virtual bool OnPaint(WPaintEvent &e);
	virtual bool OnTimer(WTimerEvent &e);
	virtual bool OnTooltipText(WTooltipTextEvent &e);
protected:
	virtual bool OnGesture(WGestureEvent &e);
	virtual bool OnTouch(WTouchEvent &e);
	virtual bool OnComputeSize(w_event_compute_size *e);
	virtual bool OnMenuNotify(WEvent &e);
	virtual bool OnReserved1(WEvent &e);
	virtual bool OnReserved2(WEvent &e);
	virtual bool OnReserved3(WEvent &e);
	virtual bool OnReserved4(WEvent &e);
	bool Notify(WEvent &e);
protected:
	/**
	 * allocate the layout data associated with the receiver to the argument.
	 *
	 * @param layoutData the new layout data for the receiver.
	 */
	void* NewLayoutData(size_t size) {
		void *data;
		_new_layout_data(&data, size);
		return data;
	}
	/**
	 * Returns layout data which is associated with the receiver.
	 *
	 * @return the receiver's layout data
	 */
	void* GetLayoutData() {
		void *data;
		_get_layout_data(&data);
		return data;
	}
public:
	/**
	 * allocate the layout data associated with the receiver to the argument.
	 *
	 * @param layoutData the new layout data for the receiver.
	 */
	static void* NewLayoutData(WControl *c, size_t size) {
		return c->NewLayoutData(size);
	}
	/**
	 * Returns layout data which is associated with the receiver.
	 *
	 * @return the receiver's layout data
	 */
	static void* GetLayoutData(WControl *c) {
		return c->GetLayoutData();
	}
public:
	WResult _get_graphics(WGraphics &gc) {
		return w_control_get_graphics(W_CONTROL(this), W_GRAPHICS(&gc));
	}
	WResult _get_orientation() {
		return w_control_get_orientation(W_CONTROL(this));
	}
	WResult _set_orientation(int orientation) {
		return w_control_set_orientation(W_CONTROL(this), orientation);
	}
	WResult _get_text_direction() {
		return w_control_get_text_direction(W_CONTROL(this));
	}
	WResult _set_text_direction(int textDirection) {
		return w_control_set_text_direction(W_CONTROL(this), textDirection);
	}
	WResult _print(WGraphics &gc) {
		return w_control_print(W_CONTROL(this), W_GRAPHICS(&gc));
	}
	WResult _compute_size(const WSize &size, int wHint, int hHint) {
		return w_control_compute_size(W_CONTROL(this), (w_size*) &size, wHint,
				hHint);
	}
	WResult _compute_size_0(const WSize &size, int wHint, int hHint,
			int changed) {
		return w_control_compute_size_0(W_CONTROL(this), (w_size*) &size, wHint,
				hHint, changed);
	}
	WResult _get_accessible(WAccessible *&accessible) {
		return w_control_get_accessible(W_CONTROL(this),
				(w_accessible**) &accessible);
	}
	WResult _get_bounds(WPoint *location, WSize *size) {
		return w_control_get_bounds(W_CONTROL(this), (w_point*) location,
				(w_size*) size);
	}
	WResult _set_bounds(const WPoint *location, const WSize *size) {
		return w_control_set_bounds(W_CONTROL(this), (w_point*) location,
				(w_size*) size);
	}
	WResult _get_region(WRegion &region) {
		return w_control_get_region(W_CONTROL(this), (w_region*) &region);
	}
	WResult _set_region(const WRegion &region) {
		return w_control_set_region(W_CONTROL(this), (w_region*) &region);
	}
	WResult _move_above(WControl *control) {
		return w_control_move_above(W_CONTROL(this), (w_control*) control);
	}
	WResult _move_below(WControl *control) {
		return w_control_move_below(W_CONTROL(this), (w_control*) control);
	}
	WResult _pack(bool changed) {
		return w_control_pack(W_CONTROL(this), changed);
	}
	WResult _to_control(WPoint &result, const WPoint &point) {
		return w_control_to_control(W_CONTROL(this), (w_point*) &result,
				(w_point*) &point);
	}
	WResult _to_display(WPoint &result, const WPoint &point) {
		return w_control_to_display(W_CONTROL(this), (w_point*) &result,
				(w_point*) &point);
	}
	WResult _drag_detect(WMouseEvent &event) {
		return w_control_drag_detect(W_CONTROL(this), (w_event_mouse*) &event);
	}
	WResult _get_drag_detect() {
		return w_control_get_drag_detect(W_CONTROL(this));
	}
	WResult _set_drag_detect(bool dragDetect) {
		return w_control_set_drag_detect(W_CONTROL(this), dragDetect);
	}
	WResult _set_focus() {
		return w_control_set_focus(W_CONTROL(this));
	}
	WResult _force_focus() {
		return w_control_force_focus(W_CONTROL(this));
	}
	WResult _is_focus_control() {
		return w_control_is_focus_control(W_CONTROL(this));
	}
	WResult _get_border_width() {
		return w_control_get_border_width(W_CONTROL(this));
	}
	WResult _get_cursor(WCursor **cursor) {
		return w_control_get_cursor(W_CONTROL(this), (w_cursor**) cursor);
	}
	WResult _set_cursor(WCursor *cursor) {
		return w_control_set_cursor(W_CONTROL(this), (w_cursor*) cursor);
	}
	WResult _get_enabled() {
		return w_control_get_enabled(W_CONTROL(this));
	}
	WResult _set_enabled(bool enabled) {
		return w_control_set_enabled(W_CONTROL(this), enabled);
	}
	WResult _is_enabled() {
		return w_control_is_enabled(W_CONTROL(this));
	}
	WResult _get_font(WFont **font) {
		return w_control_get_font(W_CONTROL(this), (w_font**) font);
	}
	WResult _set_font(WFont *font) {
		return w_control_set_font(W_CONTROL(this), (w_font*) font);
	}
	WResult _get_background(w_color *background) {
		return w_control_get_background(W_CONTROL(this), background);
	}
	WResult _set_background(w_color background) {
		return w_control_set_background(W_CONTROL(this), background);
	}
	WResult _get_foreground(w_color *foreground) {
		return w_control_get_foreground(W_CONTROL(this), foreground);
	}
	WResult _set_foreground(w_color foreground) {
		return w_control_set_foreground(W_CONTROL(this), foreground);
	}
	WResult _get_layout_data(void **data) {
		return w_control_get_layout_data(W_CONTROL(this), data);
	}
	WResult _new_layout_data(void **data, size_t size) {
		return w_control_new_layout_data(W_CONTROL(this), data, size);
	}
	WResult _layout_set_data(const WLayoutData &data) {
		return w_control_layout_set_data(W_CONTROL(this),
				(const w_layout_data*) &data);
	}
	WResult _layout_get_data(WLayoutData &data) {
		return w_control_layout_get_data(W_CONTROL(this),
				(w_layout_data*) &data);
	}
	WResult _request_layout() {
		return w_control_request_layout(W_CONTROL(this));
	}
	WResult _get_menu(WMenu **menu) {
		return w_control_get_menu(W_CONTROL(this), (w_menu**) menu);
	}
	WResult _set_menu(WMenu *menu) {
		return w_control_set_menu(W_CONTROL(this), (w_menu*) menu);
	}
	WResult _get_parent(WComposite **parent) {
		return w_control_get_parent(W_CONTROL(this), (w_composite**) parent);
	}
	WResult _set_parent(WComposite *parent) {
		return w_control_set_parent(W_CONTROL(this), (w_composite*) parent);
	}
	WResult _is_reparentable() {
		return w_control_is_reparentable(W_CONTROL(this));
	}
	WResult _get_shell(WFrame **shell) {
		return w_control_get_shell(W_CONTROL(this), (w_shell**) shell);
	}
	WResult _get_tooltip_text(w_alloc alloc, void *user_data, int enc) {
		return w_control_get_tooltip_text(W_CONTROL(this), alloc, user_data,
				enc);
	}
	WResult _set_tooltip_text(const char *text, int length, int enc) {
		return w_control_set_tooltip_text(W_CONTROL(this), text, length, enc);
	}
	WResult _get_touch_enabled() {
		return w_control_get_touch_enabled(W_CONTROL(this));
	}
	WResult _set_touch_enabled(bool enabled) {
		return w_control_set_touch_enabled(W_CONTROL(this), enabled);
	}
	WResult _get_visible() {
		return w_control_get_visible(W_CONTROL(this));
	}
	WResult _set_visible(bool visible) {
		return w_control_set_visible(W_CONTROL(this), visible);
	}
	WResult _is_visible() {
		return w_control_is_visible(W_CONTROL(this));
	}
	WResult _redraw(const WRect *rect, bool all) {
		return w_control_redraw(W_CONTROL(this), (w_rect*) rect, all);
	}
	WResult _set_redraw(bool redraw) {
		return w_control_set_redraw(W_CONTROL(this), redraw);
	}
	WResult _set_capture(bool capture) {
		return w_control_set_capture(W_CONTROL(this), capture);
	}
	WResult _traverse(int traversal, WKeyEvent *event) {
		return w_control_traverse(W_CONTROL(this), traversal,
				(w_event_key*) event);
	}
	WResult _update() {
		return w_control_update(W_CONTROL(this));
	}
	WResult _set_timer(wint64 ms, wushort id) {
		return w_control_set_timer(W_CONTROL(this), ms, id);
	}
	WResult _kill_timer(wushort id) {
		return w_control_kill_timer(W_CONTROL(this), id);
	}
	WResult _set_tab(bool tab) {
		return w_control_set_tab(W_CONTROL(this), tab);
	}
	WResult _get_tab() {
		return w_control_get_tab(W_CONTROL(this));
	}
	wresult _create_dragsource(w_dragsource *dragsource, wuint64 style,
			w_widget_post_event_proc post_event) {
		return w_control_create_dragsource(W_CONTROL(this), dragsource, style,
				post_event);
	}
	wresult _create_droptarget(w_droptarget *droptarget, wuint64 style,
			w_widget_post_event_proc post_event) {
		return w_control_create_droptarget(W_CONTROL(this), droptarget, style,
				post_event);
	}
private:
	void *handles[(sizeof(w_control) - sizeof(w_widget)) / sizeof(void*)];
};
#define W_ACTION(x) ((WControl::SelectionAction)&x)
namespace swt {
typedef ::WControl Control;
}  // namespace swt

#endif /* SWTP_WIDGETS_CONTROL_H_ */
