/*
 * ExpandBar.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_EXPANDBAR_H_
#define SWTP_CONTROLS_EXPANDBAR_H_
#include "../widgets/Composite.h"
class WExpandBar;
/**
 * Instances of this class represent a selectable user interface object
 * that represents a expandable item in a expand bar.
 * <p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 *
 * @see ExpandBar
 */
class SWTP_PUBLIC WExpandItem: public WItem {
public:
	WExpandItem() {

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
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WExpandItem(WExpandBar *parent, int style) {

	}
	/**
	 * Constructs a new instance of this class given its parent, a
	 * style value describing its behavior and appearance, and the index
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
	 * @param style the style of control to construct
	 * @param index the zero-relative index to store the receiver in its parent
	 *
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WExpandItem(WExpandBar *parent, int style, int index) {

	}
	/**
	 * Returns the control that is shown when the item is expanded.
	 * If no control has been set, return <code>null</code>.
	 *
	 * @return the control
	 */
	WControl* GetControl() {
		WControl *c;
		w_expanditem_get_control(W_EXPANDITEM(this), (w_control**) &c);
		return c;
	}
	/**
	 * Returns <code>true</code> if the receiver is expanded,
	 * and false otherwise.
	 *
	 * @return the expanded state
	 */
	bool GetExpanded() {
		return w_expanditem_get_expanded(W_EXPANDITEM(this));
	}
	/**
	 * Returns the height of the receiver's header
	 *
	 * @return the height of the header
	 */
	int GetHeaderHeight() {
		return w_expanditem_get_header_height(W_EXPANDITEM(this));
	}
	/**
	 * Gets the height of the receiver.
	 *
	 * @return the height
	 */
	int GetHeight() {
		return w_expanditem_get_height(W_EXPANDITEM(this));
	}
	/**
	 * Returns the receiver's parent, which must be a <code>ExpandBar</code>.
	 *
	 * @return the receiver's parent
	 */
	WExpandBar* GetParent() {
		WExpandBar *parent;
		w_item_get_parent_widget(W_ITEM(this), (w_widget**) &parent);
		return parent;
	}
	/**
	 * Sets the control that is shown when the item is expanded.
	 *
	 * @param control the new control (or null)
	 */
	bool SetControl(WControl *control) {
		return w_expanditem_set_control(W_EXPANDITEM(this), W_CONTROL(control))
				> 0;
	}
	/**
	 * Sets the expanded state of the receiver.
	 *
	 * @param expanded the new expanded state
	 */
	bool SetExpanded(bool expanded) {
		return w_expanditem_set_expanded(W_EXPANDITEM(this), expanded) > 0;
	}
	/**
	 * Sets the height of the receiver. This is height of the item when it is expanded,
	 * excluding the height of the header.
	 *
	 * @param height the new height
	 */
	bool SetHeight(int height) {
		return w_expanditem_set_height(W_EXPANDITEM(this), height) > 0;
	}
	bool SetImage(int image) {
		return w_expanditem_set_image(W_EXPANDITEM(this), image) > 0;
	}
private:
	void *handles[sizeof(w_expanditem) / sizeof(void*)];
};
/**
 * Instances of this class support the layout of selectable
 * expand bar items.
 * <p>
 * The item children that may be added to instances of this class
 * must be of type <code>ExpandItem</code>.
 * </p><p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>V_SCROLL</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Expand, Collapse</dd>
 * </dl>
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 *
 * @see ExpandItem
 * @see ExpandEvent
 * @see ExpandListener
 * @see ExpandAdapter
 */
class SWTP_PUBLIC WExpandBar: public WComposite {
public:
	WExpandBar() {

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
	 * @see SWT#V_SCROLL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WExpandBar(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	WImageList* GetImageList() {
		WImageList *imagelist;
		w_expandbar_get_imagelist(W_EXPANDBAR(this),
				(w_imagelist**) &imagelist);
		return imagelist;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WExpandItem& GetItem(int index, WExpandItem &item) {
		w_expandbar_get_item(W_EXPANDBAR(this), index, W_EXPANDITEM(&item));
		return item;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WExpandItem GetItem(int index) {
		WExpandItem item;
		return GetItem(index, item);
	}
	/**
	 * Returns the number of items contained in the receiver.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return w_expandbar_get_item_count(W_EXPANDBAR(this));
	}
	/**
	 * Returns an array of <code>ExpandItem</code>s which are the items
	 * in the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 */
	void GetItems(WIterator<WExpandItem> &items) {
		w_expandbar_get_items(W_EXPANDBAR(this), (w_iterator*) &items);
	}
	/**
	 * Returns the receiver's spacing.
	 *
	 * @return the spacing
	 */
	int GetSpacing() {
		return w_expandbar_get_spacing(W_EXPANDBAR(this));
	}
	bool SetImageList(WImageList *imagelist) {
		return w_expandbar_set_imagelist(W_EXPANDBAR(this),
				W_IMAGELIST(imagelist));
	}
	/**
	 * Sets the receiver's spacing. Spacing specifies the number of pixels allocated around
	 * each item.
	 *
	 * @param spacing the spacing around each item
	 */
	bool SetSpacing(int spacing) {
		return w_expandbar_set_spacing(W_EXPANDBAR(this), spacing) > 0;
	}
public:
	WExpandItem& InsertItem(WExpandItem &item, int index) {
		w_expandbar_insert_item(W_EXPANDBAR(this), W_EXPANDITEM(&item), index);
		return item;
	}
	WExpandItem& InsertItem(WExpandItem &item, const char *text, int index) {
		InsertItem(item, index);
		item.SetText(text);
		return item;
	}
	WExpandItem& AppendItem(WExpandItem &item, const char *text) {
		return InsertItem(item, text, -1);
	}
protected:
	w_class_id _GetClassID();
private:
	void *handles[(sizeof(w_expandbar) - sizeof(w_composite)) / sizeof(void*)];
};
#endif /* SWTP_CONTROLS_EXPANDBAR_H_ */
