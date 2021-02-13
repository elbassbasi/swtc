/*
 * TabFolder.h
 *
 *  Created on: 4 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_TABVIEW_H_
#define SWTP_CONTROLS_TABVIEW_H_
#include "../widgets/Composite.h"
class WTabView;
/**
 * Instances of this class represent a selectable user interface object
 * corresponding to a tab for a page in a tab folder.
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WTabItem: public WItem {
public:
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>TabFolder</code>), a style value
	 * describing its behavior and appearance, and the index
	 * at which to place it in the items maintained by its parent.
	 *
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param index the zero-relative index to store the receiver in its parent
	 */
	bool Create(WTabView *parent, const char *text, int index = -1) {
		return w_tabview_insert_item(W_TABVIEW(parent), W_TABITEM(this), index)
				> 0;
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent.
	 *
	 * @return the receiver's bounding rectangle
	 */
	bool GetBounds(WRect &bounds) {
		return w_tabitem_get_bounds(W_TABITEM(this), (w_rect*) &bounds) > 0;
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent.
	 *
	 * @return the receiver's bounding rectangle
	 */
	WRect GetBounds() {
		WRect bounds;
		GetBounds(bounds);
		return bounds;
	}
	/**
	 * Returns the control that is used to fill the client area of
	 * the tab folder when the user selects the tab item.  If no
	 * control has been set, return <code>null</code>.
	 * <p>
	 * @return the control
	 */
	WControl* GetControl() {
		return (WControl*) w_tabitem_get_control(W_TABITEM(this));
	}
	/**
	 * Returns the application defined widget data associated
	 * with the receiver, or null if it has not been set. The
	 * <em>widget data</em> is a single, unnamed field that is
	 * stored with every widget.
	 * <p>
	 * Applications may put arbitrary objects in this field. If
	 * the object stored in the widget data needs to be notified
	 * when the widget is disposed of, it is the application's
	 * responsibility to hook the Dispose event on the widget and
	 * do so.
	 * </p>
	 *
	 * @return the widget data
	 * @see #setData(Object)
	 */
	void* GetData() {
		return WItem::GetData();
	}
	/**
	 * Returns the receiver's image index if it has one, or -1
	 * if it does not.
	 *
	 * @return the receiver's image index
	 */
	int GetImage() {
		return w_tabitem_get_image(W_TABITEM(this));
	}
	/**
	 * Returns the receiver's parent, which must be a <code>TabFolder</code>.
	 *
	 * @return the receiver's parent
	 */
	WTabView* GetParent() {
		return (WTabView*) this->GetParentWidget();
	}
	/**
	 * Returns the receiver's tool tip text, or null if it has
	 * not been set.
	 *
	 * @return the receiver's tool tip text
	 */
	WString GetToolTipText() {
		WControl *control = GetControl();
		if (control != 0)
			return control->GetToolTipText();
		else
			return WString();
	}
	bool Remove(bool destroy_Control) {
		return w_tabitem_remove(W_TABITEM(this), destroy_Control) > 0;
	}
	bool Remove() {
		return Remove(true);
	}
	/**
	 * Sets the control that is used to fill the client area of
	 * the tab folder when the user selects the tab item.
	 * <p>
	 * @param control the new control (or null)
	 */
	bool SetControl(WControl *control) {
		return w_tabitem_set_control(W_TABITEM(this), W_CONTROL(control));
	}
	/**
	 * Sets the application defined widget data associated
	 * with the receiver to be the argument. The <em>widget
	 * data</em> is a single, unnamed field that is stored
	 * with every widget.
	 * <p>
	 * Applications may put arbitrary objects in this field. If
	 * the object stored in the widget data needs to be notified
	 * when the widget is disposed of, it is the application's
	 * responsibility to hook the Dispose event on the widget and
	 * do so.
	 * </p>
	 *
	 * @param data the widget data
	 * @see #getData()
	 */
	bool SetData(void *data) {
		return WItem::SetData(data);
	}
	/**
	 * Sets the receiver's image index to the argument, which may be
	 * -1 indicating that no image should be displayed.
	 *
	 * @param image the image to display on the receiver (may be -1)
	 */
	bool SetImage(int image) {
		return w_tabitem_set_image(W_TABITEM(this), image) > 0;
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
	 * @param tooltiptext the new tool tip text (or null)
	 */
	void SetToolTipText(const char *tooltiptext) {
		WControl *control = GetControl();
		if (control != 0)
			return control->SetToolTipText(tooltiptext);
	}
	bool Pack(WControl *control) {
		return w_tabitem_pack(W_TABITEM(this), W_CONTROL(control)) > 0;
	}
private:
	char handles[sizeof(w_tabitem) - sizeof(w_item)];
};
class WTabEvent: public WEvent {
public:
	WTabItem *item;
	WControl *control;
	WTabItem* GetItem() {
		return this->item;
	}
	WTabItem* operator->() {
		return this->item;
	}
};
/**
 * Instances of this class implement the notebook user interface
 * metaphor.  It allows the user to select a notebook page from
 * set of pages.
 * <p>
 * The item children that may be added to instances of this class
 * must be of type <code>TabItem</code>.
 * <code>Control</code> children are created and then set into a
 * tab item using <code>TabItem#setControl</code>.
 * </p><p>
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not make sense to set a layout on it.
 * </p><p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>TOP, BOTTOM</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles TOP and BOTTOM may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WTabView: public WComposite {
public:
	WTabView() {
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
	 * @see SWT#TOP
	 * @see SWT#BOTTOM
	 */
	WTabView(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	WImageList* GetImageList() {
		WImageList *imagelist;
		_get_imagelist(&imagelist);
		return imagelist;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WTabItem& GetItem(int index, WTabItem &item) {
		_get_item(index, &item);
		return item;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WTabItem GetItem(int index) {
		WTabItem item;
		return GetItem(index, (WTabItem&) item);
	}
	/**
	 * Returns the tab item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 *
	 * @param point the point used to locate the item
	 * @return the tab item at the given point, or empty if the point is not in a tab item
	 */
	WTabItem& GetItem(const WPoint &point, WTabItem &item) {
		_get_item_p(&point, &item);
		return item;
	}
	/**
	 * Returns the tab item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 *
	 * @param point the point used to locate the item
	 * @return the tab item at the given point, or null if the point is not in a tab item
	 */
	WTabItem GetItem(const WPoint &point) {
		WTabItem item;
		return GetItem(point, item);
	}
	/**
	 * Returns the tab item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 *
	 * @param x the point used to locate the item
	 * @param y the point used to locate the item
	 * @return the tab item at the given point, or null if the point is not in a tab item
	 */
	WTabItem GetItem(int x, int y) {
		WTabItem item;
		return GetItem(WPoint(x, y), item);
	}
	/**
	 * Returns the number of items contained in the receiver.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return _WReturnInt(_get_item_count());
	}
	/**
	 * Returns an array of <code>TabItem</code>s which are the items
	 * in the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 */
	bool GetItems(WIterator<WTabItem> &items) {
		return _WReturnBool(_get_items(&items));
	}
	/**
	 * Returns an array of <code>TabItem</code>s that are currently
	 * selected in the receiver. An empty array indicates that no
	 * items are selected.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its selection, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 * @return an array representing the selection
	 */
	WTabItem& GetSelection(WTabItem &item) {
		_get_selection(&item);
		return item;
	}
	/**
	 * Returns an array of <code>TabItem</code>s that are currently
	 * selected in the receiver. An empty array indicates that no
	 * items are selected.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its selection, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 * @return an array representing the selection
	 */
	WTabItem GetSelection() {
		WTabItem item;
		return GetSelection(item);
	}
	/**
	 * Returns the zero-relative index of the item which is currently
	 * selected in the receiver, or -1 if no item is selected.
	 *
	 * @return the index of the selected item
	 */
	int GetSelectionIndex() {
		return GetSelection().GetIndex();
	}
	/*
	 * Inserts a new page at the specified position.
	 *
	 * @param index	Specifies the position for the new page,or -1 to insert to last.
	 * @param text	Specifies the text for the new page.
	 *
	 */
	WTabItem& InsertItem(WTabItem &item, const char *text, int index) {
		_insert_item(&item, index);
		item.SetText(text);
		return item;
	}
	/*
	 * Inserts a new page at the specified position.
	 *
	 * @param index	Specifies the position for the new page,or -1 to insert to last.
	 * @param text	Specifies the text for the new page.
	 *
	 */
	WTabItem& InsertItem(const char *text, int index) {
		WTabItem item;
		return InsertItem(item, text, index);
	}
	/*
	 * Inserts a new page at the specified position.
	 *
	 * @param index	Specifies the position for the new page,or -1 to insert to last.
	 * @param text	Specifies the text for the new page.
	 *
	 */
	WTabItem& AppendItem(WTabItem &item, const char *text) {
		return InsertItem(item, text, -1);
	}
	/*
	 * Inserts a new page at the specified position.
	 *
	 * @param index	Specifies the position for the new page,or -1 to insert to last.
	 * @param text	Specifies the text for the new page.
	 *
	 */
	WTabItem& AppendItem(const char *text) {
		WTabItem item;
		return InsertItem(item, text, -1);
	}
	bool SetImageList(WImageList *imagelist) {
		return _WReturnBool(_set_imagelist(imagelist));
	}
	/**
	 * Selects the item at the given zero-relative index in the receiver.
	 * If the item at the index was already selected, it remains selected.
	 * The current selection is first cleared, then the new items are
	 * selected. Indices that are out of range are ignored.
	 *
	 * @param index the index of the item to select
	 */
	bool SetSelection(int index) {
		return _WReturnBool(_set_selection(index));
	}
	/**
	 * Sets the receiver's selection to the given item.
	 * The current selected is first cleared, then the new item is
	 * selected.
	 *
	 * @param item the item to select
	 */
	bool SetSelection(WTabItem &item) {
		return _WReturnBool(_set_selection(item.GetIndex()));
	}
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnItemSelection(WTabEvent &e);
	virtual bool OnItemGetControl(WTabEvent &e);
	virtual bool OnItemSetControl(WTabEvent &e);
	virtual bool OnItemClose(WTabEvent &e);
public:
	WResult _get_imagelist(WImageList **imagelist) {
		return w_tabview_get_imagelist(W_TABVIEW(this),
				(w_imagelist**) imagelist);
	}
	WResult _get_item(int index, WTabItem *item) {
		return w_tabview_get_item(W_TABVIEW(this), index, W_TABITEM(item));
	}
	WResult _get_item_p(const WPoint *point, WTabItem *item) {
		return w_tabview_get_item_p(W_TABVIEW(this), (w_point*) point,
				W_TABITEM(item));
	}
	WResult _get_item_count() {
		return w_tabview_get_item_count(W_TABVIEW(this));
	}
	WResult _get_items(WIterator<WTabItem> *items) {
		return w_tabview_get_items(W_TABVIEW(this), (w_iterator*) items);
	}
	WResult _get_selection(WTabItem *item) {
		return w_tabview_get_selection(W_TABVIEW(this), W_TABITEM(item));
	}
	WResult _insert_item(WTabItem *item, int index) {
		return w_tabview_insert_item(W_TABVIEW(this), W_TABITEM(item), index);
	}
	WResult _set_imagelist(WImageList *imagelist) {
		return w_tabview_set_imagelist(W_TABVIEW(this), W_IMAGELIST(imagelist));
	}
	WResult _set_selection(int index) {
		return w_tabview_set_selection(W_TABVIEW(this), index);
	}
private:
	void *handles[(sizeof(w_tabview) - sizeof(w_composite)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_TABVIEW_H_ */
