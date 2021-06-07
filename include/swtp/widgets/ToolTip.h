/*
 * ToolTip.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_TOOLTIP_H_
#define SWTP_WIDGETS_TOOLTIP_H_
#include "Widget.h"
/**
 * Instances of this class represent popup windows that are used
 * to inform or warn the user.
 * <p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>BALLOON, ICON_ERROR, ICON_INFORMATION, ICON_WARNING</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection</dd>
 * </dl>
 * </p><p>
 * Note: Only one of the styles ICON_ERROR, ICON_INFORMATION,
 * and ICON_WARNING may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WToolTip: public WWidget {
public:
	WToolTip() {

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
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 *
	 * @see SWT#BALLOON
	 * @see SWT#ICON_ERROR
	 * @see SWT#ICON_INFORMATION
	 * @see SWT#ICON_WARNING
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WToolTip(WFrame *parent, int style) {
		Create(parent, style | W_FREE_MEMORY);
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
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 *
	 * @see SWT#BALLOON
	 * @see SWT#ICON_ERROR
	 * @see SWT#ICON_INFORMATION
	 * @see SWT#ICON_WARNING
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WFrame *parent, int style) {
		return Create(0, parent, style);
	}
	bool Create(WToolkit *toolkit, WFrame *parent, int style) {
		return w_tooltip_create(W_TOOLTIP(this), (w_toolkit*) toolkit,
				W_COMPOSITE(parent), style, WWidget::post_event_proc) > 0;
	}
	/**
	 * Returns <code>true</code> if the receiver is automatically
	 * hidden by the platform, and <code>false</code> otherwise.
	 *
	 * @return the receiver's auto hide state
	 */
	bool GetAutoHide() {
		return w_tooltip_get_auto_hide(W_TOOLTIP(this));
	}
	/**
	 * Returns the receiver's message, which will be an empty
	 * string if it has never been set.
	 *
	 * @return the receiver's message
	 */
	WString GetMessage() {
		w_string_ref *ref = 0;
		w_tooltip_get_message(W_TOOLTIP(this), w_alloc_string_ref, &ref,
				W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Shell</code>.
	 *
	 * @return the receiver's parent
	 */
	WFrame* GetParent() {
		WFrame *parent;
		w_tooltip_get_parent(W_TOOLTIP(this), (w_shell**) &parent);
		return parent;
	}
	/**
	 * Returns the receiver's text, which will be an empty
	 * string if it has never been set.
	 *
	 * @return the receiver's text
	 */
	WString GetText() {
		w_string_ref *ref = 0;
		w_tooltip_get_text(W_TOOLTIP(this), w_alloc_string_ref, &ref,
				W_ENCODING_UTF8);
		return ref;
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
		return w_tooltip_get_visible(W_TOOLTIP(this));
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
		return w_tooltip_is_visible(W_TOOLTIP(this));
	}
	/**
	 * Makes the receiver hide automatically when <code>true</code>,
	 * and remain visible when <code>false</code>.
	 *
	 * @param autoHide the auto hide state
	 *
	 * @see #getVisible
	 * @see #setVisible
	 */
	bool SetAutoHide(bool autoHide) {
		return w_tooltip_set_autohide(W_TOOLTIP(this), autoHide) > 0;
	}
	/**
	 * Sets the location of the receiver, which must be a tooltip,
	 * to the point specified by the arguments which are relative
	 * to the display.
	 * <p>
	 * Note that this is different from most widgets where the
	 * location of the widget is relative to the parent.
	 * </p>
	 *
	 * @param x the new x coordinate for the receiver
	 * @param y the new y coordinate for the receiver
	 */
	bool SetLocation(int x, int y) {
		return SetLocation(WPoint(x, y));
	}
	/**
	 * Sets the location of the receiver, which must be a tooltip,
	 * to the point specified by the argument which is relative
	 * to the display.
	 * <p>
	 * Note that this is different from most widgets where the
	 * location of the widget is relative to the parent.
	 * </p><p>
	 * Note that the platform window manager ultimately has control
	 * over the location of tooltips.
	 * </p>
	 *
	 * @param location the new location for the receiver
	 */
	bool SetLocation(const WPoint &location) {
		return w_tooltip_set_location(W_TOOLTIP(this), (w_point*) &location) > 0;
	}
	/**
	 * Sets the receiver's message.
	 *
	 * @param string the new message
	 */
	bool SetMessage(const char *string) {
		return w_tooltip_set_message(W_TOOLTIP(this), string, -1,
				W_ENCODING_UTF8) > 0;
	}
	/**
	 * Sets the receiver's text.
	 *
	 * @param string the new text
	 */
	bool SetText(const char *string) {
		return w_tooltip_set_text(W_TOOLTIP(this), string, -1, W_ENCODING_UTF8)
				> 0;
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
	bool SetVisible(bool visible) {
		return w_tooltip_set_visible(W_TOOLTIP(this), visible) > 0;
	}
private:
	void *handles[(sizeof(w_tooltip) - sizeof(w_widget)) / sizeof(void*)];
};

#endif /* SWTP_WIDGETS_TOOLTIP_H_ */
