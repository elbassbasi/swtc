/*
 * Scrollable.h
 *
 *  Created on: 25 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_SCROLLABLE_H_
#define SWTP_WIDGETS_SCROLLABLE_H_
#include "Control.h"
#include "Menu.h"
class WScrollable;
typedef w_scrollbar_value WScrollBarValue;
/**
 * Instances of this class are selectable user interface
 * objects that represent a range of positive, numeric values.
 * <p>
 * At any given moment, a given scroll bar will have a
 * single 'selection' that is considered to be its
 * value, which is constrained to be within the range of
 * values the scroll bar represents (that is, between its
 * <em>minimum</em> and <em>maximum</em> values).
 * </p><p>
 * Typically, scroll bars will be made up of five areas:
 * <ol>
 * <li>an arrow button for decrementing the value</li>
 * <li>a page decrement area for decrementing the value by a larger amount</li>
 * <li>a <em>thumb</em> for modifying the value by mouse dragging</li>
 * <li>a page increment area for incrementing the value by a larger amount</li>
 * <li>an arrow button for incrementing the value</li>
 * </ol>
 * Based on their style, scroll bars are either <code>HORIZONTAL</code>
 * (which have a left facing button for decrementing the value and a
 * right facing button for incrementing it) or <code>VERTICAL</code>
 * (which have an upward facing button for decrementing the value
 * and a downward facing buttons for incrementing it).
 * </p><p>
 * On some platforms, the size of the scroll bar's thumb can be
 * varied relative to the magnitude of the range of values it
 * represents (that is, relative to the difference between its
 * maximum and minimum values). Typically, this is used to
 * indicate some proportional value such as the ratio of the
 * visible area of a document to the total amount of space that
 * it would take to display it. SWT supports setting the thumb
 * size even if the underlying platform does not, but in this
 * case the appearance of the scroll bar will not change.
 * </p><p>
 * Scroll bars are created by specifying either <code>H_SCROLL</code>,
 * <code>V_SCROLL</code> or both when creating a <code>Scrollable</code>.
 * They are accessed from the <code>Scrollable</code> using
 * <code>getHorizontalBar</code> and <code>getVerticalBar</code>.
 * </p><p>
 * Note: Scroll bars are not Controls.  On some platforms, scroll bars
 * that appear as part of some standard controls such as a text or list
 * have no operating system resources and are not children of the control.
 * For this reason, scroll bars are treated specially.  To create a control
 * that looks like a scroll bar but has operating system resources, use
 * <code>Slider</code>.
 * </p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>HORIZONTAL, VERTICAL</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles HORIZONTAL and VERTICAL may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 *
 * @see Slider
 * @see Scrollable
 * @see Scrollable#getHorizontalBar
 * @see Scrollable#getVerticalBar
 * @see <a href="http://www.eclipse.org/swt/examples.php">SWT Example: ControlExample</a>
 * @see <a href="http://www.eclipse.org/swt/">Sample code and further information</a>
 * @noextend This class is not intended to be subclassed by clients.
 */
class SWTP_PUBLIC WScrollBar: public WWidgetData {
public:
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
		return w_scrollbar_get_enabled(W_SCROLLBAR(this));
	}
	/**
	 * Returns the amount that the receiver's value will be
	 * modified by when the up/down (or right/left) arrows
	 * are pressed.
	 *
	 * @return the increment
	 *
	 */
	int GetIncrement() {
		return w_scrollbar_get_increment(W_SCROLLBAR(this));
	}
	/**
	 * Returns the maximum value which the receiver will allow.
	 *
	 * @return the maximum
	 *
	 */
	int GetMaximum() {
		return w_scrollbar_get_maximum(W_SCROLLBAR(this));
	}
	/**
	 * Returns the minimum value which the receiver will allow.
	 *
	 * @return the minimum
	 *
	 */
	int GetMinimum() {
		return w_scrollbar_get_minimum(W_SCROLLBAR(this));
	}
	/**
	 * Returns the amount that the receiver's value will be
	 * modified by when the page increment/decrement areas
	 * are selected.
	 *
	 * @return the page increment
	 *
	 */
	int GetPageIncrement() {
		return w_scrollbar_get_page_increment(W_SCROLLBAR(this));
	}
	/**
	 * Returns the receiver's parent, which must be a Scrollable.
	 *
	 * @return the receiver's parent
	 *
	 */
	WScrollable* GetParent() {
		WScrollable *parent;
		w_scrollbar_get_parent(W_SCROLLBAR(this), (w_scrollable**) &parent);
		return parent;
	}
	/**
	 * Returns the single 'selection' that is the receiver's value.
	 *
	 * @return the selection
	 *
	 */
	int GetSelection() {
		return w_scrollbar_get_selection(W_SCROLLBAR(this));
	}
	/**
	 * Returns a point describing the receiver's size. The
	 * x coordinate of the result is the width of the receiver.
	 * The y coordinate of the result is the height of the
	 * receiver.
	 *
	 * @return the receiver's size
	 *
	 */
	WSize& GetSize(WSize &size) {
		w_scrollbar_get_size(W_SCROLLBAR(this), (w_size*) &size);
		return size;
	}
	/**
	 * Returns a point describing the receiver's size. The
	 * x coordinate of the result is the width of the receiver.
	 * The y coordinate of the result is the height of the
	 * receiver.
	 *
	 * @return the receiver's size
	 */
	WSize GetSize() {
		WSize size;
		GetSize((WSize&) size);
		return size;
	}
	/**
	 * Returns the receiver's thumb value.
	 *
	 * @return the thumb value
	 *
	 * @see ScrollBar
	 */
	int GetThumb() {
		return w_scrollbar_get_thumb(W_SCROLLBAR(this));
	}
	/**
	 * Gets the receiver's selection, minimum value, maximum
	 * value, thumb, increment and page increment all at once.
	 * <p>
	 * Note: This is similar to getting the values individually
	 * using the appropriate methods, but may be implemented in a
	 * more efficient fashion on some platforms.
	 * </p>
	 *
	 * @param value the all value of scrollbar
	 *
	 */
	void GetValues(WScrollBarValue &value) {
		w_scrollbar_get_values(W_SCROLLBAR(this), &value);
	}
	/**
	 * Returns a rectangle describing the size and location of the
	 * receiver's thumb relative to its parent.
	 *
	 * @return the thumb bounds, relative to the {@link #getParent() parent}
	 *
	 * @since 3.6
	 */
	WRect& GetThumbBounds(WRect &thumbBounds) {
		w_scrollbar_get_thumb_bounds(W_SCROLLBAR(this), (w_rect*) &thumbBounds);
		return thumbBounds;
	}
	/**
	 * Returns a rectangle describing the size and location of the
	 * receiver's thumb relative to its parent.
	 *
	 * @return the thumb bounds, relative to the {@link #getParent() parent}
	 *
	 * @since 3.6
	 */
	WRect GetThumbBounds() {
		WRect thumbBounds;
		GetThumbBounds(thumbBounds);
		return thumbBounds;
	}
	/**
	 * Returns a rectangle describing the size and location of the
	 * receiver's thumb track relative to its parent. This rectangle
	 * comprises the areas 2, 3, and 4 as described in {@link ScrollBar}.
	 *
	 * @return the thumb track bounds, relative to the {@link #getParent() parent}
	 *
	 * @since 3.6
	 */
	WRect& GetThumbTrackBounds(WRect &ThumbTrackBounds) {
		w_scrollbar_get_thumb_track_bounds(W_SCROLLBAR(this),
				(w_rect*) &ThumbTrackBounds);
		return ThumbTrackBounds;
	}
	/**
	 * Returns a rectangle describing the size and location of the
	 * receiver's thumb track relative to its parent. This rectangle
	 * comprises the areas 2, 3, and 4 as described in {@link ScrollBar}.
	 *
	 * @return the thumb track bounds, relative to the {@link #getParent() parent}
	 *
	 * @since 3.6
	 */
	WRect& GetThumbTrackBounds(const WRect &ThumbTrackBounds = WRect()) {
		return GetThumbTrackBounds((WRect&) ThumbTrackBounds);
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
		return w_scrollbar_get_visible(W_SCROLLBAR(this));
	}
	/**
	 * Returns <code>true</code> if the receiver is enabled and all
	 * of the receiver's ancestors are enabled, and <code>false</code>
	 * otherwise. A disabled control is typically not selectable from the
	 * user interface and draws with an inactive or "grayed" look.
	 *
	 * @return the receiver's enabled state
	 *
	 * @see #getEnabled
	 */
	bool IsEnabled() {
		return w_scrollbar_is_enabled(W_SCROLLBAR(this));
	}
	/**
	 * Returns <code>true</code> if the receiver is visible and all
	 * of the receiver's ancestors are visible and <code>false</code>
	 * otherwise.
	 *
	 * @return the receiver's visibility state
	 *
	 * @see #getVisible
	 */
	bool IsVisible() {
		return w_scrollbar_is_visible(W_SCROLLBAR(this));
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
		w_scrollbar_set_enabled(W_SCROLLBAR(this), enabled);
	}
	/**
	 * Sets the amount that the receiver's value will be
	 * modified by when the up/down (or right/left) arrows
	 * are pressed to the argument, which must be at least
	 * one.
	 *
	 * @param value the new increment (must be greater than zero)
	 */
	void SetIncrement(int value) {
		w_scrollbar_set_increment(W_SCROLLBAR(this), value);
	}
	/**
	 * Sets the maximum. If this value is negative or less than or
	 * equal to the minimum, the value is ignored. If necessary, first
	 * the thumb and then the selection are adjusted to fit within the
	 * new range.
	 *
	 * @param value the new maximum
	 */
	void SetMaximum(int value) {
		w_scrollbar_set_maximum(W_SCROLLBAR(this), value);
	}
	/**
	 * Sets the minimum value. If this value is negative or greater
	 * than or equal to the maximum, the value is ignored. If necessary,
	 * first the thumb and then the selection are adjusted to fit within
	 * the new range.
	 *
	 * @param value the new minimum
	 */
	void SetMinimum(int value) {
		w_scrollbar_set_minimum(W_SCROLLBAR(this), value);
	}
	/**
	 * Sets the amount that the receiver's value will be
	 * modified by when the page increment/decrement areas
	 * are selected to the argument, which must be at least
	 * one.
	 *
	 * @param value the page increment (must be greater than zero)
	 */
	void SetPageIncrement(int value) {
		w_scrollbar_set_page_increment(W_SCROLLBAR(this), value);
	}
	/**
	 * Sets the single <em>selection</em> that is the receiver's
	 * value to the argument which must be greater than or equal
	 * to zero.
	 *
	 * @param selection the new selection (must be zero or greater)
	 */
	void SetSelection(int selection) {
		w_scrollbar_set_selection(W_SCROLLBAR(this), selection);
	}
	/**
	 * Sets the thumb value. The thumb value should be used to represent
	 * the size of the visual portion of the current range. This value is
	 * usually the same as the page increment value.
	 * <p>
	 * This new value will be ignored if it is less than one, and will be
	 * clamped if it exceeds the receiver's current range.
	 * </p>
	 *
	 * @param value the new thumb value, which must be at least one and not
	 * larger than the size of the current range
	 */
	void SetThumb(int value) {
		w_scrollbar_set_thumb(W_SCROLLBAR(this), value);
	}
	/**
	 * Sets the receiver's selection, minimum value, maximum
	 * value, thumb, increment and page increment all at once.
	 * <p>
	 * Note: This is similar to setting the values individually
	 * using the appropriate methods, but may be implemented in a
	 * more efficient fashion on some platforms.
	 * </p>
	 *
	 * @param value the all value
	 */
	void SetValues(WScrollBarValue &value) {
		w_scrollbar_set_values(W_SCROLLBAR(this), &value);
	}
	/**
	 * Sets the receiver's selection, minimum value, maximum
	 * value, thumb, increment and page increment all at once.
	 * <p>
	 * Note: This is similar to setting the values individually
	 * using the appropriate methods, but may be implemented in a
	 * more efficient fashion on some platforms.
	 * </p>
	 *
	 * @param selection the new selection value
	 * @param minimum the new minimum value
	 * @param maximum the new maximum value
	 * @param thumb the new thumb value
	 * @param increment the new increment value
	 * @param pageIncrement the new pageIncrement value
	 */
	void SetValues(int selection, int minimum, int maximum, int thumb,
			int increment, int pageIncrement) {
		WScrollBarValue value;
		value.selection = selection;
		value.minimum = minimum;
		value.maximum = maximum;
		value.thumb = thumb;
		value.increment = increment;
		value.pageIncrement = pageIncrement;
		SetValues(value);
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
		w_scrollbar_set_visible(W_SCROLLBAR(this), visible);
	}
private:
	void *handles[(sizeof(w_scrollbar) - sizeof(w_widgetdata)) / sizeof(void*)];

};
class WScrollBarEvent: public WEvent {
public:
	int detail;
	WScrollBar *scrollbar;
	WScrollBar* operator ->() {
		return this->scrollbar;
	}
};
/**
 * This class is the abstract superclass of all classes which
 * represent controls that have standard scroll bars.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>H_SCROLL, V_SCROLL</dd>
 * <dt><b>Events:</b>
 * <dd>(none)</dd>
 * </dl>
 * <p>
 * IMPORTANT: This class is intended to be subclassed <em>only</em>
 * within the SWT implementation.
 * </p>
 *
 * @see <a href="http://www.eclipse.org/swt/">Sample code and further information</a>
 * @noextend This class is not intended to be subclassed by clients.
 */
class SWTP_PUBLIC WScrollable: public WControl {
protected:
	bool PostEvent(WEvent *e);
	virtual bool OnComputeTrim(w_event_compute_trim &e);
	virtual bool OnGetClientArea(w_event_client_area &e);
	virtual bool OnHScroll(WScrollBarEvent &e);
	virtual bool OnVScroll(WScrollBarEvent &e);
public:
	WScrollable() {

	}
	/**
	 * Given a desired <em>client area</em> for the receiver
	 * (as described by the arguments), returns the bounding
	 * rectangle which would be required to produce that client
	 * area.
	 * <p>
	 * In other words, it returns a rectangle such that, if the
	 * receiver's bounds were set to that rectangle, the area
	 * of the receiver which is capable of displaying data
	 * (that is, not covered by the "trimmings") would be the
	 * rectangle described by the arguments (relative to the
	 * receiver's parent).
	 * </p>
	 *
	 * @param result the required bounds to produce the given client area
	 * @param r the desired rectangle the client area
	 *
	 * @see #getClientArea
	 */
	WRect& ComputeTrim(WRect &result, const WRect &r) {
		w_scrollable_compute_trim(W_SCROLLABLE(this), (w_rect*) &result,
				(w_rect*) &r);
		return result;
	}
	/**
	 * Given a desired <em>client area</em> for the receiver
	 * (as described by the arguments), returns the bounding
	 * rectangle which would be required to produce that client
	 * area.
	 * <p>
	 * In other words, it returns a rectangle such that, if the
	 * receiver's bounds were set to that rectangle, the area
	 * of the receiver which is capable of displaying data
	 * (that is, not covered by the "trimmings") would be the
	 * rectangle described by the arguments (relative to the
	 * receiver's parent).
	 * </p>
	 *
	 * @param x the desired x coordinate of the client area
	 * @param y the desired y coordinate of the client area
	 * @param width the desired width of the client area
	 * @param height the desired height of the client area
	 * @return the required bounds to produce the given client area
	 *
	 * @see #getClientArea
	 */
	WRect& ComputeTrim(const WRect &r, const WRect &result = WRect()) {
		return ComputeTrim((WRect&) result, r);
	}
	/**
	 * Given a desired <em>client area</em> for the receiver
	 * (as described by the arguments), returns the bounding
	 * rectangle which would be required to produce that client
	 * area.
	 * <p>
	 * In other words, it returns a rectangle such that, if the
	 * receiver's bounds were set to that rectangle, the area
	 * of the receiver which is capable of displaying data
	 * (that is, not covered by the "trimmings") would be the
	 * rectangle described by the arguments (relative to the
	 * receiver's parent).
	 * </p>
	 *
	 * @param x the desired x coordinate of the client area
	 * @param y the desired y coordinate of the client area
	 * @param width the desired width of the client area
	 * @param height the desired height of the client area
	 * @return the required bounds to produce the given client area
	 *
	 * @see #getClientArea
	 */
	WRect& ComputeTrim(WRect &result, int x, int y, int width, int height) {
		WRect r(x, y, width, height);
		return ComputeTrim(result, r);
	}
	/**
	 * Given a desired <em>client area</em> for the receiver
	 * (as described by the arguments), returns the bounding
	 * rectangle which would be required to produce that client
	 * area.
	 * <p>
	 * In other words, it returns a rectangle such that, if the
	 * receiver's bounds were set to that rectangle, the area
	 * of the receiver which is capable of displaying data
	 * (that is, not covered by the "trimmings") would be the
	 * rectangle described by the arguments (relative to the
	 * receiver's parent).
	 * </p>
	 *
	 * @param x the desired x coordinate of the client area
	 * @param y the desired y coordinate of the client area
	 * @param width the desired width of the client area
	 * @param height the desired height of the client area
	 * @return the required bounds to produce the given client area
	 *
	 * @see #getClientArea
	 */
	WRect ComputeTrim(int x, int y, int width, int height) {
		WRect result;
		return ComputeTrim(result, x, y, width, height);
	}
	/**
	 * Returns a rectangle which describes the area of the
	 * receiver which is capable of displaying data (that is,
	 * not covered by the "trimmings").
	 *
	 * @return the client area
	 *
	 * @see #computeTrim
	 */
	WRect& GetClientArea(WRect &clientArea) {
		w_scrollable_get_client_area(W_SCROLLABLE(this), (w_rect*) &clientArea);
		return clientArea;
	}
	/**
	 * Returns a rectangle which describes the area of the
	 * receiver which is capable of displaying data (that is,
	 * not covered by the "trimmings").
	 *
	 * @return the client area
	 *
	 * @see #computeTrim
	 */
	WRect GetClientArea() {
		WRect clientArea;
		return GetClientArea(clientArea);
	}
	/**
	 * Returns the receiver's horizontal scroll bar if it has
	 * one, and null if it does not.
	 *
	 * @return the horizontal scroll bar (or null)
	 */
	WScrollBar& GetHorizontalBar(WScrollBar &scrollBar) {
		w_scrollable_get_horizontal_bar(W_SCROLLABLE(this),
				W_SCROLLBAR(&scrollBar));
		return scrollBar;
	}
	/**
	 * Returns the receiver's horizontal scroll bar if it has
	 * one, and null if it does not.
	 *
	 * @return the horizontal scroll bar (or null)
	 */
	WScrollBar GetHorizontalBar() {
		WScrollBar scrollBar;
		return GetHorizontalBar(scrollBar);
	}
	/**
	 * Returns the mode of the receiver's scrollbars. This will be
	 * <em>bitwise</em> OR of one or more of the constants defined in class
	 * <code>SWT</code>.<br>
	 * <li><code>SWT.SCROLLBAR_OVERLAY</code> - if receiver
	 * uses overlay scrollbars</li>
	 * <li><code>SWT.NONE</code> - otherwise</li>
	 *
	 * @return the mode of scrollbars
	 *
	 * @see SWT#SCROLLBAR_OVERLAY
	 */
	int GetScrollbarsMode() {
		return w_scrollable_get_scrollbars_mode(W_SCROLLABLE(this));
	}
	/**
	 * Returns the receiver's vertical scroll bar if it has
	 * one, and null if it does not.
	 *
	 * @return the vertical scroll bar (or null)
	 */
	WScrollBar& GetVerticalBar(WScrollBar &scrollBar) {
		w_scrollable_get_vertical_bar(W_SCROLLABLE(this),
				W_SCROLLBAR(&scrollBar));
		return scrollBar;
	}
	/**
	 * Returns the receiver's vertical scroll bar if it has
	 * one, and null if it does not.
	 *
	 * @return the vertical scroll bar (or null)
	 */
	WScrollBar GetVerticalBar() {
		WScrollBar scrollBar;
		return GetVerticalBar(scrollBar);
	}
public:
	static bool IsScrollable(WWidget *widget) {
		return widget->GetClassId() >= _W_CLASS_SCROLLABLE;
	}
	static WScrollable* ToScrollable(WWidget *widget) {
		if (IsScrollable(widget))
			return (WScrollable*) widget;
		else
			return 0;
	}
public:
private:
	void *handles[(sizeof(w_scrollable) - sizeof(w_control)) / sizeof(void*)];
};

namespace swt {
typedef ::WScrollable Scrollable;
typedef ::WScrollBarValue ScrollBarValue;
typedef ::WScrollBar ScrollBar;
}  // namespace swt
#endif /* SWTP_WIDGETS_SCROLLABLE_H_ */
