/*
 * CoolBar.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_COOLBAR_H_
#define SWTP_CONTROLS_COOLBAR_H_
#include "../widgets/Composite.h"
class WCoolBar;
/**
 * Instances of this class are selectable user interface
 * objects that represent the dynamically positionable
 * areas of a <code>CoolBar</code>.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>DROP_DOWN</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection</dd>
 * </dl>
 */
class SWTP_PUBLIC WCoolItem: public WItem {
public:
	WCoolItem() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>CoolBar</code>) and a style value
	 * describing its behavior and appearance. The item is added
	 * to the end of the items maintained by its parent.
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
	 *
	 * @see SWT#DROP_DOWN
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WCoolBar *parent) {
		return w_coolbar_insert_item(W_COOLBAR(parent), W_COOLITEM(this),
				W_NONE, -1) > 0;
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>CoolBar</code>), a style value
	 * describing its behavior and appearance, and the index
	 * at which to place it in the items maintained by its parent.
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
	 * @param index the zero-relative index at which to store the receiver in its parent
	 *
	 * @see SWT#DROP_DOWN
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WCoolBar *parent, int index) {
		return w_coolbar_insert_item(W_COOLBAR(parent), W_COOLITEM(this),
				W_NONE, index) > 0;
	}
	/**
	 * Returns the preferred size of the receiver.
	 * <p>
	 * The <em>preferred size</em> of a <code>CoolItem</code> is the size that
	 * it would best be displayed at. The width hint and height hint arguments
	 * allow the caller to ask the instance questions such as "Given a particular
	 * width, how high does it need to be to show all of the contents?"
	 * To indicate that the caller does not wish to constrain a particular
	 * dimension, the constant <code>SWT.DEFAULT</code> is passed for the hint.
	 * </p>
	 *
	 * @param wHint the width hint (can be <code>SWT.DEFAULT</code>)
	 * @param hHint the height hint (can be <code>SWT.DEFAULT</code>)
	 * @return the preferred size
	 *
	 * @see Layout
	 * @see #getBounds
	 * @see #getSize
	 * @see Control#getBorderWidth
	 * @see Scrollable#computeTrim
	 * @see Scrollable#getClientArea
	 */
	WSize& ComputeSize(WSize &result, int wHint, int hHint) {
		w_coolitem_compute_size(W_COOLITEM(this), (w_size*) &result, wHint,
				hHint);
		return result;
	}
	/**
	 * Returns the preferred size of the receiver.
	 * <p>
	 * The <em>preferred size</em> of a <code>CoolItem</code> is the size that
	 * it would best be displayed at. The width hint and height hint arguments
	 * allow the caller to ask the instance questions such as "Given a particular
	 * width, how high does it need to be to show all of the contents?"
	 * To indicate that the caller does not wish to constrain a particular
	 * dimension, the constant <code>SWT.DEFAULT</code> is passed for the hint.
	 * </p>
	 *
	 * @param wHint the width hint (can be <code>SWT.DEFAULT</code>)
	 * @param hHint the height hint (can be <code>SWT.DEFAULT</code>)
	 * @return the preferred size
	 *
	 * @see Layout
	 * @see #getBounds
	 * @see #getSize
	 * @see Control#getBorderWidth
	 * @see Scrollable#computeTrim
	 * @see Scrollable#getClientArea
	 */
	WSize& ComputeSize(int wHint, int hHint, const WSize &size = WSize()) {
		return ComputeSize((WSize&) size, wHint, hHint);
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent.
	 *
	 * @return the receiver's bounding rectangle
	 */
	WRect& GetBounds(WRect &bounds) {
		w_coolitem_get_bounds(W_COOLITEM(this), (w_rect*) &bounds);
		return bounds;
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent.
	 *
	 * @return the receiver's bounding rectangle
	 */
	WRect& GetBounds(const WRect &bounds = WRect()) {
		return GetBounds((WRect&) bounds);
	}
	/**
	 * Returns the control that is associated with the receiver.
	 *
	 * @return the control that is contained by the receiver
	 */
	WControl* GetControl() {
		return (WControl*) w_coolitem_get_control(W_COOLITEM(this));
	}
	/**
	 * Returns the minimum size that the cool item can
	 * be resized to using the cool item's gripper.
	 *
	 * @return a point containing the minimum width and height of the cool item, in pixels
	 */
	WSize& GetMinimumSize(WSize &size) {
		w_coolitem_get_minimum_size(W_COOLITEM(this), (w_size*) &size);
		return size;
	}
	/**
	 * Returns the minimum size that the cool item can
	 * be resized to using the cool item's gripper.
	 *
	 * @return a point containing the minimum width and height of the cool item, in pixels
	 */
	WSize& GetMinimumSize(const WSize &size = WSize()) {
		return GetMinimumSize((WSize&) size);
	}
	/**
	 * Returns the receiver's parent, which must be a <code>CoolBar</code>.
	 *
	 * @return the receiver's parent
	 */
	WCoolBar* GetParent() {
		return (WCoolBar*) WItem::GetParentWidget();
	}
	/**
	 * Returns a point describing the receiver's ideal size.
	 * The x coordinate of the result is the ideal width of the receiver.
	 * The y coordinate of the result is the ideal height of the receiver.
	 *
	 * @return the receiver's ideal size
	 */
	WSize& GetPreferredSize(WSize &size) {
		w_coolitem_get_preferred_size(W_COOLITEM(this), (w_size*) &size);
		return size;
	}
	/**
	 * Returns a point describing the receiver's ideal size.
	 * The x coordinate of the result is the ideal width of the receiver.
	 * The y coordinate of the result is the ideal height of the receiver.
	 *
	 * @return the receiver's ideal size
	 */
	WSize& GetPreferredSize(const WSize &size = WSize()) {
		return GetPreferredSize((WSize&) size);
	}
	/**
	 * Returns a point describing the receiver's size. The
	 * x coordinate of the result is the width of the receiver.
	 * The y coordinate of the result is the height of the
	 * receiver.
	 *
	 * @return the receiver's size
	 */
	WSize& GetSize(WSize &size) {
		w_coolitem_get_size(W_COOLITEM(this), (w_size*) &size);
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
	WSize& GetSize(const WSize &size = WSize()) {
		return GetSize((WSize&) size);
	}
	/**
	 * Sets the control that is associated with the receiver
	 * to the argument.
	 *
	 * @param control the new control that will be contained by the receiver
	 */
	bool SetControl(WControl *control) {
		return w_coolitem_set_control(W_COOLITEM(this), W_CONTROL(control)) > 0;
	}
	/**
	 * Sets the minimum size that the cool item can be resized to
	 * using the cool item's gripper, to the point specified by the arguments.
	 *
	 * @param width the minimum width of the cool item, in pixels
	 * @param height the minimum height of the cool item, in pixels
	 */
	bool SetMinimumSize(int width, int height) {
		return SetMinimumSize(WSize(width, height));
	}
	/**
	 * Sets the minimum size that the cool item can be resized to
	 * using the cool item's gripper, to the point specified by the argument.
	 *
	 * @param size a point representing the minimum width and height of the cool item, in pixels
	 */
	bool SetMinimumSize(const WSize &size) {
		return w_coolitem_set_minimum_size(W_COOLITEM(this), (w_size*) &size)
				> 0;
	}
	/**
	 * Sets the receiver's ideal size to the point specified by the arguments.
	 *
	 * @param width the new ideal width for the receiver
	 * @param height the new ideal height for the receiver
	 */
	bool SetPreferredSize(int width, int height) {
		return SetPreferredSize(WSize(width, height));
	}
	/**
	 * Sets the receiver's ideal size to the point specified by the argument.
	 *
	 * @param size the new ideal size for the receiver
	 */
	bool SetPreferredSize(const WSize &size) {
		return w_coolitem_set_preferred_size(W_COOLITEM(this), (w_size*) &size)
				> 0;
	}
	/**
	 * Sets the receiver's size to the point specified by the arguments.
	 * <p>
	 * Note: Attempting to set the width or height of the
	 * receiver to a negative number will cause that
	 * value to be set to zero instead.
	 * </p>
	 *
	 * @param width the new width for the receiver
	 * @param height the new height for the receiver
	 */
	bool SetSize(int width, int height) {
		return SetSize(WSize(width, height));
	}
	/**
	 * Sets the receiver's size to the point specified by the argument.
	 * <p>
	 * Note: Attempting to set the width or height of the
	 * receiver to a negative number will cause them to be
	 * set to zero instead.
	 * </p>
	 *
	 * @param size the new size for the receiver
	 */
	bool SetSize(const WSize &size) {
		return w_coolitem_set_size(W_COOLITEM(this), (w_size*) &size) > 0;
	}
private:
	void *handles[(sizeof(w_coolitem) - sizeof(w_item)) / sizeof(void*)];
};
/**
 * Instances of this class provide an area for dynamically
 * positioning the items they contain.
 * <p>
 * The item children that may be added to instances of this class
 * must be of type <code>CoolItem</code>.
 * </p><p>
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not make sense to add <code>Control</code> children to it,
 * or set a layout on it.
 * </p><p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>FLAT, HORIZONTAL, VERTICAL</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * </p><p>
 * Note: Only one of the styles HORIZONTAL and VERTICAL may be specified.
 * </p>
 */
class SWTP_PUBLIC WCoolBar: public WComposite {
public:
	WCoolBar() {

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
	 * @see SWT
	 * @see SWT#FLAT
	 * @see SWT#HORIZONTAL
	 * @see SWT#VERTICAL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WCoolBar(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	/**
	 * Returns the item that is currently displayed at the given,
	 * zero-relative index. Throws an exception if the index is
	 * out of range.
	 *
	 * @param index the visual index of the item to return
	 * @return the item at the given visual index
	 */
	WCoolItem& GetItem(int index, WCoolItem &item) {
		w_coolbar_get_item(W_COOLBAR(this), index, W_COOLITEM(&item));
		return item;
	}
	/**
	 * Returns the item that is currently displayed at the given,
	 * zero-relative index. Throws an exception if the index is
	 * out of range.
	 *
	 * @param index the visual index of the item to return
	 * @return the item at the given visual index
	 */
	WCoolItem& GetItem(int index, const WCoolItem &item = WCoolItem()) {
		return GetItem(index, (WCoolItem&) item);
	}
	/**
	 * Returns the number of items contained in the receiver.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return w_coolbar_get_item_count(W_COOLBAR(this));
	}
	/**
	 * Returns an array of <code>CoolItem</code>s in the order
	 * in which they are currently being displayed.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the receiver's items in their current visual order
	 */
	void GetItems(WIterator<WCoolItem> &items) {
		w_coolbar_get_items(W_COOLBAR(this), (w_iterator*) &items);
	}
	/**
	 * Returns whether or not the receiver is 'locked'. When a coolbar
	 * is locked, its items cannot be repositioned.
	 *
	 * @return true if the coolbar is locked, false otherwise
	 */
	bool GetLocked() {
		return w_coolbar_get_locked(W_COOLBAR(this));
	}
	/**
	 * Sets whether or not the receiver is 'locked'. When a coolbar
	 * is locked, its items cannot be repositioned.
	 *
	 * @param locked lock the coolbar if true, otherwise unlock the coolbar
	 */
	bool SetLocked(bool locked) {
		return w_coolbar_set_locked(W_COOLBAR(this), locked) > 0;
	}
public:
	WCoolItem& InsertItem(WCoolItem &item, int style, int index) {
		w_coolbar_insert_item(W_COOLBAR(this), W_COOLITEM(&item), style, index);
		return item;
	}
	void InsertItem(int style, int index) {
		w_coolbar_insert_item(W_COOLBAR(this), 0, style, index);
	}
	WCoolItem& InsertItem(WCoolItem &item, int index) {
		return InsertItem(item, W_NONE, index);
	}
	void InsertItem(int index) {
		InsertItem(index, W_NONE);
	}
	WCoolItem& AppendItem(WCoolItem &item) {
		return InsertItem(item, -1);
	}
	void AppendItem() {
		return InsertItem(-1);
	}
protected:
	w_class_id _GetClassID();
private:
	void *handles[(sizeof(w_coolbar) - sizeof(w_composite)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_COOLBAR_H_ */
