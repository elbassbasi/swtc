/*
 * WTreeView.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_TREEVIEW_H_
#define SWTP_CONTROLS_TREEVIEW_H_
#include "../widgets/Composite.h"
class WTreeView;
/**
 * Instances of this class represent a selectable user interface object
 * that represents a hierarchy of tree items in a tree widget.
 *
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WTreeItem: public WListItem {
public:
	/**
	 * Clears the item at the given zero-relative index in the receiver.
	 * The text, icon and other attributes of the item are set to the default
	 * value.  If the tree was created with the <code>W_VIRTUAL</code> style,
	 * these attributes are requested again as needed.
	 *
	 * @param index the index of the item to clear
	 * @param all <code>true</code> if all child items of the indexed item should be
	 * cleared recursively, and <code>false</code> otherwise
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	bool Clear(int index, bool all) {
		return _WReturnBool(_clear(index, all));
	}
	/**
	 * Clears all the items in the receiver. The text, icon and other
	 * attributes of the items are set to their default values. If the
	 * tree was created with the <code>W_VIRTUAL</code> style, these
	 * attributes are requested again as needed.
	 *
	 * @param all <code>true</code> if all child items should be cleared
	 * recursively, and <code>false</code> otherwise
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	bool ClearAll(bool all) {
		return _WReturnBool(_clear_all(all));
	}
	/**
	 * Returns <code>true</code> if the receiver is expanded,
	 * and false otherwise.
	 * <p>
	 *
	 * @return the expanded state
	 */
	bool GetExpanded() {
		return _WReturnBool(_get_expanded());
	}
	WTreeItem& GetFirstChild(WTreeItem &firstchild) {
		_get_first_child(&firstchild);
		return firstchild;
	}
	WTreeItem& GetFirstChild(const WTreeItem &firstchild = WTreeItem()) {
		return GetFirstChild((WTreeItem&) firstchild);
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WTreeItem& GetItem(int index, WTreeItem &item) {
		w_treeitem_get_item(W_TREEITEM(this), index, W_TREEITEM(&item));
		return item;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WTreeItem GetItem(int index) {
		WTreeItem item;
		return GetItem(index, (WTreeItem&) item);
	}
	/**
	 * Returns the number of items contained in the receiver
	 * that are direct item children of the receiver.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return _WReturnInt(_get_item_count());
	}
	/**
	 * Returns a (possibly empty) array of <code>TreeItem</code>s which
	 * are the direct item children of the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the receiver's items
	 */
	bool GetItems(WIterator<WTreeItem> &items) {
		return _WReturnBool(_get_items(&items));
	}
	WTreeItem& GetLastChild(WTreeItem &lastchild) {
		_get_last_child(&lastchild);
		return lastchild;
	}
	WTreeItem& GetLastChild(const WTreeItem &lastchild = WTreeItem()) {
		return GetLastChild((WTreeItem&) lastchild);
	}
	WTreeItem& GetNextSibling(WTreeItem &nextsibling) {
		_get_next_sibling(&nextsibling);
		return nextsibling;
	}
	WTreeItem& GetNextSibling(const WTreeItem &nextsibling = WTreeItem()) {
		return GetNextSibling((WTreeItem&) nextsibling);
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Tree</code>.
	 *
	 * @return the receiver's parent
	 */
	WTreeView* GetParent() {
		return (WTreeView*) WItem::GetParentWidget();
	}
	/**
	 * Returns the receiver's parent item, which must be a
	 * <code>TreeItem</code> or null when the receiver is a
	 * root.
	 *
	 * @return the receiver's parent item
	 */
	WTreeItem& GetParentItem(WTreeItem &parentitem) {
		_get_parent_item(&parentitem);
		return parentitem;
	}
	/**
	 * Returns the receiver's parent item, which must be a
	 * <code>TreeItem</code> or null when the receiver is a
	 * root.
	 *
	 * @return the receiver's parent item
	 */
	WTreeItem& GetParentItem(const WTreeItem &parentitem = WTreeItem()) {
		return GetParentItem((WTreeItem&) parentitem);
	}
	WTreeItem& GetPrevSibling(WTreeItem &prevsibling) {
		_get_prev_sibling(&prevsibling);
		return prevsibling;
	}
	WTreeItem& GetPrevSibling(const WTreeItem &prevsibling = WTreeItem()) {
		return GetPrevSibling((WTreeItem&) prevsibling);
	}
	/*
	 *
	 */
	WTreeItem& InsertItem(WTreeItem &item, int index) {
		_insert_item(&item, index);
		return item;
	}
	WTreeItem& InsertItem(WTreeItem &item, const char *text, int index) {
		InsertItem(item, index);
		item.SetText(text);
		return item;
	}
	WTreeItem InsertItem(int index) {
		WTreeItem item;
		return InsertItem(item, index);
	}
	WTreeItem InsertItem(const char *text, int index) {
		WTreeItem item;
		return InsertItem(item, text, index);
	}
	WTreeItem& AppendItem(WTreeItem &item) {
		return InsertItem(item, -1);
	}
	WTreeItem& AppendItem(WTreeItem &item, const char *text) {
		return InsertItem(item, text, -1);
	}
	WTreeItem AppendItem(const char *text) {
		WTreeItem item;
		return InsertItem(item, text, -1);
	}
	WTreeItem AppendItem() {
		WTreeItem item;
		return InsertItem(item, -1);
	}
	/**
	 * Removes all of the items from the receiver.
	 * <p>
	 */
	bool RemoveAll() {
		return _WReturnBool(_remove_all());
	}
	/**
	 * Sets the expanded state of the receiver.
	 * <p>
	 *
	 * @param expanded the new expanded state
	 */
	bool SetExpanded(bool expanded) {
		return _WReturnBool(_set_expanded(expanded));
	}
	bool SetHasChildren() {
		return _WReturnBool(_set_has_children());
	}
	bool SetItemCount(int count) {
		return _WReturnBool(_set_item_count(count));
	}
public:
	WResult _clear(int index, int all) {
		return w_treeitem_clear(W_TREEITEM(this), index, all);
	}
	WResult _clear_all(int all) {
		return w_treeitem_clear_all(W_TREEITEM(this), all);
	}
	WResult _get_expanded() {
		return w_treeitem_get_expanded(W_TREEITEM(this));
	}
	WResult _get_first_child(WTreeItem *child) {
		return w_treeitem_get_first_child(W_TREEITEM(this), W_TREEITEM(child));
	}
	WResult _get_item(int index, WTreeItem *subitem) {
		return w_treeitem_get_item(W_TREEITEM(this), index, W_TREEITEM(subitem));
	}
	WResult _get_item_count() {
		return w_treeitem_get_item_count(W_TREEITEM(this));
	}
	WResult _get_items(WIterator<WTreeItem> *items) {
		return w_treeitem_get_items(W_TREEITEM(this), (w_iterator*) items);
	}
	WResult _get_last_child(WTreeItem *child) {
		return w_treeitem_get_last_child(W_TREEITEM(this), W_TREEITEM(child));
	}
	WResult _get_next_sibling(WTreeItem *next) {
		return w_treeitem_get_next_sibling(W_TREEITEM(this), W_TREEITEM(next));
	}
	WResult _get_parent_item(WTreeItem *parent) {
		return w_treeitem_get_parent_item(W_TREEITEM(this), W_TREEITEM(parent));
	}
	WResult _get_prev_sibling(WTreeItem *prev) {
		return w_treeitem_get_prev_sibling(W_TREEITEM(this), W_TREEITEM(prev));
	}
	WResult _insert_item(WTreeItem *subitem, int index) {
		return w_treeitem_insert_item(W_TREEITEM(this), W_TREEITEM(subitem),
				index);
	}
	WResult _insert_item_after(WTreeItem *subitem, WTreeItem *after) {
		return w_treeitem_insert_item_after(W_TREEITEM(this),
				W_TREEITEM(subitem), W_TREEITEM(after));
	}
	WResult _insert_item_before(WTreeItem *subitem, WTreeItem *before) {
		return w_treeitem_insert_item_before(W_TREEITEM(this),
				W_TREEITEM(subitem), W_TREEITEM(before));
	}
	WResult _remove_all() {
		return w_treeitem_remove_all(W_TREEITEM(this));
	}
	WResult _set_expanded(int expanded) {
		return w_treeitem_set_expanded(W_TREEITEM(this), expanded);
	}
	WResult _set_has_children() {
		return w_treeitem_set_has_children(W_TREEITEM(this));
	}
	WResult _set_item_count(int count) {
		return w_treeitem_set_item_count(W_TREEITEM(this), count);
	}
};
typedef WListEvent WTreeEvent;
/**
 * Instances of this class provide a selectable user interface object
 * that displays a hierarchy of items and issues notification when an
 * item in the hierarchy is selected.
 * <p>
 * The item children that may be added to instances of this class
 * must be of type <code>TreeItem</code>.
 * </p><p>
 * Style <code>VIRTUAL</code> is used to create a <code>Tree</code> whose
 * <code>TreeItem</code>s are to be populated by the client on an on-demand basis
 * instead of up-front.  This can provide significant performance improvements for
 * trees that are very large or for which <code>TreeItem</code> population is
 * expensive (for example, retrieving values from an external source).
 * </p><p>
 * </p><p>
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not normally make sense to add <code>Control</code> children to
 * it, or set a layout on it, unless implementing something like a cell
 * editor.
 * </p><p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>SINGLE, MULTI, CHECK, FULL_SELECTION, VIRTUAL, NO_SCROLL</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection, DefaultSelection, Collapse, Expand, SetData, MeasureItem, EraseItem, PaintItem</dd>
 * </dl>
 * </p><p>
 * Note: Only one of the styles SINGLE and MULTI may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WTreeView: public WListViewBase {
public:
	WTreeView() {

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
	 * @see SWT#SINGLE
	 * @see SWT#MULTI
	 * @see SWT#CHECK
	 * @see SWT#FULL_SELECTION
	 * @see SWT#VIRTUAL
	 * @see SWT#NO_SCROLL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WTreeView(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	/**
	 * Clears the item at the given zero-relative index in the receiver.
	 * The text, icon and other attributes of the item are set to the default
	 * value.  If the tree was created with the <code>W_VIRTUAL</code> style,
	 * these attributes are requested again as needed.
	 *
	 * @param index the index of the item to clear
	 * @param all <code>true</code> if all child items of the indexed item should be
	 * cleared recursively, and <code>false</code> otherwise
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	void Clear(int index, bool all) {
		GetRootItem().Clear(index, all);
	}
	/**
	 * Deselects an item in the receiver.  If the item was already
	 * deselected, it remains deselected.
	 *
	 * @param item the item to be deselected
	 */
	bool Deselect(WTreeItem &item) {
		return _WReturnBool(_deselect(&item));
	}
	/**
	 * Returns the item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 * <p>
	 * The item that is returned represents an item that could be selected by the user.
	 * For example, if selection only occurs in items in the first column, then null is
	 * returned if the point is outside of the item.
	 * Note that the W_FULL_SELECTION style hint, which specifies the selection policy,
	 * determines the extent of the selection.
	 * </p>
	 *
	 * @param point the point used to locate the item
	 * @return the item at the given point, or null if the point is not in a selectable item
	 */
	WTreeItem& GetItem(WTreeItem &item, const WPoint &point) {
		_get_item_from_point(&point, &item);
		return item;
	}
	WTreeItem& GetItem(WTreeItem &item, int x, int y) {
		return GetItem(item, WPoint(x, y));
	}
	/**
	 * Returns the item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 * <p>
	 * The item that is returned represents an item that could be selected by the user.
	 * For example, if selection only occurs in items in the first column, then null is
	 * returned if the point is outside of the item.
	 * Note that the W_FULL_SELECTION style hint, which specifies the selection policy,
	 * determines the extent of the selection.
	 * </p>
	 *
	 * @param point the point used to locate the item
	 * @return the item at the given point, or null if the point is not in a selectable item
	 */
	WTreeItem GetItem(const WPoint &point) {
		WTreeItem item;
		return GetItem(item, point);
	}
	/**
	 * Returns the receiver's parent item, which must be a
	 * <code>TreeItem</code> or null when the receiver is a
	 * root.
	 *
	 * @return the receiver's parent item
	 */
	WTreeItem& GetParentItem(WTreeItem &item) {
		_get_parent_item(&item);
		return item;
	}
	/**
	 * Returns the receiver's parent item, which must be a
	 * <code>TreeItem</code> or null when the receiver is a
	 * root.
	 *
	 * @return the receiver's parent item
	 */
	WTreeItem GetParentItem() {
		WTreeItem item;
		return GetParentItem(item);
	}
	/**
	 * Returns the root item, which must be a
	 * <code>TreeItem</code>
	 *
	 * @return the root item
	 */
	WTreeItem& GetRootItem(WTreeItem &root) {
		_get_root_item(&root);
		return root;
	}
	/**
	 * Returns the root item, which must be a
	 * <code>TreeItem</code>
	 *
	 * @return the root item
	 */
	WTreeItem GetRootItem() {
		WTreeItem root;
		return GetRootItem(root);
	}
	/**
	 * Returns an array of <code>TreeItem</code>s that are currently
	 * selected in the receiver. The order of the items is unspecified.
	 * An empty array indicates that no items are selected.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its selection, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 * @return an array representing the selection
	 */
	bool GetSelection(WIterator<WTreeItem> &selection) {
		return _WReturnBool(_get_selection((w_iterator*) &selection));
	}
	/**
	 * Returns the item which is currently at the top of the receiver.
	 * This item can change when items are expanded, collapsed, scrolled
	 * or new items are added or removed.
	 *
	 * @return the item at the top of the receiver
	 */
	WTreeItem& GetTopItem(WTreeItem &topitem) {
		_get_top_item(&topitem);
		return topitem;
	}
	/**
	 * Returns the item which is currently at the top of the receiver.
	 * This item can change when items are expanded, collapsed, scrolled
	 * or new items are added or removed.
	 *
	 * @return the item at the top of the receiver
	 */
	WTreeItem GetTopItem() {
		WTreeItem topitem;
		return GetTopItem(topitem);
	}
	/**
	 * Display a mark indicating the point at which an item will be inserted.
	 * The drop insert item has a visual hint to show where a dragged item
	 * will be inserted when dropped on the tree.
	 *
	 * @param item the insert item.  Null will clear the insertion mark.
	 * @param before true places the insert mark above 'item'. false places
	 *	the insert mark below 'item'.
	 */
	bool SetInsertMark(WTreeItem &item, bool before) {
		return _WReturnBool(_set_insert_mark(&item, before));
	}
	/**
	 * Sets the height of the area which would be used to
	 * display <em>one</em> of the items in the tree.
	 *
	 * @param itemHeight the height of one item
	 */
	bool SetItemHeight(int itemHeight) {
		return _WReturnBool(_set_item_height(itemHeight));
	}
	/**
	 * Selects an item in the receiver.  If the item was already
	 * selected, it remains selected.
	 *
	 * @param item the item to be selected
	 */
	bool Select(WTreeItem &item) {
		return _WReturnBool(_select(&item));
	}
	/**
	 * Sets the receiver's selection to the given item.
	 * The current selection is cleared before the new item is selected.
	 * <p>
	 * If the item is not in the receiver, then it is ignored.
	 * </p>
	 *
	 * @param item the item to select
	 */
	bool SetSelection(WTreeItem &item) {
		return _WReturnBool(_set_selection(&item));
	}
	/**
	 * Sets the item which is currently at the top of the receiver.
	 * This item can change when items are expanded, collapsed, scrolled
	 * or new items are added or removed.
	 *
	 * @param item the item to be shown
	 *
	 * @see WTreeView#getTopItem()
	 */
	bool SetTopItem(WTreeItem &item) {
		return _WReturnBool(_set_top_item(&item));
	}
	/**
	 * Shows the item.  If the item is already showing in the receiver,
	 * this method simply returns.  Otherwise, the items are scrolled
	 * and expanded until the item is visible.
	 *
	 * @param item the item to be shown
	 *
	 * @see WTreeView#showSelection()
	 */
	bool ShowItem(WTreeItem &item) {
		return _WReturnBool(_show_item(&item));
	}
	bool Sort() {
		return _WReturnBool(_sort());
	}
	static bool IsTree(WWidget *widget) {
		return widget->GetClassId() == _W_CLASS_TREEVIEW;
	}
public:
	/*
	 *
	 */
	WTreeItem& InsertItem(WTreeItem &item, int index) {
		WTreeItem rootitem;
		GetRootItem(rootitem).InsertItem(item, index);
		return item;
	}
	WTreeItem& InsertItem(WTreeItem &item, const char *text, int index) {
		InsertItem(item, index);
		item.SetText(text);
		return item;
	}
	WTreeItem InsertItem(int index) {
		WTreeItem item;
		return InsertItem(item, index);
	}
	WTreeItem InsertItem(const char *text, int index) {
		WTreeItem item;
		return InsertItem(item, text, index);
	}
	WTreeItem& AppendItem(WTreeItem &item) {
		return InsertItem(item, -1);
	}
	WTreeItem& AppendItem(WTreeItem &item, const char *text) {
		return InsertItem(item, text, -1);
	}
	WTreeItem AppendItem(const char *text) {
		WTreeItem item;
		return InsertItem(item, text, -1);
	}
	WTreeItem AppendItem() {
		WTreeItem item;
		return InsertItem(item, -1);
	}
public:
	WResult _clear(WTreeItem *item) {
		return w_treeview_clear(W_TREEVIEW(this), W_TREEITEM(item));
	}
	WResult _deselect(WTreeItem *item) {
		return w_treeview_deselect(W_TREEVIEW(this), W_TREEITEM(item));
	}
	WResult _get_item_from_point(const WPoint *point, WTreeItem *item) {
		return w_treeview_get_item_from_point(W_TREEVIEW(this),
				(w_point*) point, W_TREEITEM(item));
	}
	WResult _get_parent_item(WTreeItem *item) {
		return w_treeview_get_parent_item(W_TREEVIEW(this), W_TREEITEM(item));
	}
	WResult _get_root_item(WTreeItem *root) {
		return w_treeview_get_root_item(W_TREEVIEW(this), W_TREEITEM(root));
	}
	WResult _get_top_item(WTreeItem *topitem) {
		return w_treeview_get_top_item(W_TREEVIEW(this), W_TREEITEM(topitem));
	}
	WResult _remove(WTreeItem *item) {
		return w_treeview_remove(W_TREEVIEW(this), W_TREEITEM(item));
	}
	WResult _set_insert_mark(WTreeItem *item, int before) {
		return w_treeview_set_insert_mark(W_TREEVIEW(this), W_TREEITEM(item),
				before);
	}
	WResult _set_selection(WTreeItem *item) {
		return w_treeview_set_selection(W_TREEVIEW(this), W_TREEITEM(item));
	}
	WResult _set_top_item(WTreeItem *item) {
		return w_treeview_set_top_item(W_TREEVIEW(this), W_TREEITEM(item));
	}
	WResult _select(WTreeItem *item) {
		return w_treeview_select(W_TREEVIEW(this), W_TREEITEM(item));
	}
	WResult _show_item(WTreeItem *item) {
		return w_treeview_show_item(W_TREEVIEW(this), W_TREEITEM(item));
	}
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnItemExpand(WListEvent &e);
	virtual bool OnItemCollapse(WListEvent &e);
private:
	//void *handles[(sizeof(w_treeview) - sizeof(w_listviewbase)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_TREE_H_ */
