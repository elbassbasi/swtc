/*
 * Combo.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_COMBOBOX_H_
#define SWTP_CONTROLS_COMBOBOX_H_
#include "../widgets/Composite.h"
class SWTP_PUBLIC WComboItem: public WItem {
public:
	int GetImage() {
		return _WReturnInt(_get_image());
	}
	/**
	 * Sets the image for multiple columns in the tree.
	 *
	 * @param images the array of new images
	 */
	bool SetImage(int image) {
		return _WReturnBool(_set_image(image));
	}
public:
	WResult _get_image() {
		return w_comboitem_get_image(W_COMBOITEM(this));
	}
	WResult _set_image(int image) {
		return w_comboitem_set_image(W_COMBOITEM(this), image);
	}
private:
	void *handle[(sizeof(w_comboitem) - sizeof(w_item)) / sizeof(void*)];
};
/**
 * Instances of this class are controls that allow the user
 * to choose an item from a list of items, or optionally
 * enter a new value by typing it into an editable text
 * field. Often, <code>Combo</code>s are used in the same place
 * where a single selection <code>List</code> widget could
 * be used but space is limited. A <code>Combo</code> takes
 * less space than a <code>List</code> widget and shows
 * similar information.
 * <p>
 * Note: Since <code>Combo</code>s can contain both a list
 * and an editable text field, it is possible to confuse methods
 * which access one versus the other (compare for example,
 * <code>clearSelection()</code> and <code>deselectAll()</code>).
 * The API documentation is careful to indicate either "the
 * receiver's list" or the "the receiver's text field" to
 * distinguish between the two cases.
 * </p><p>
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not make sense to add children to it, or set a layout on it.
 * </p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>DROP_DOWN, READ_ONLY, SIMPLE</dd>
 * <dt><b>Events:</b></dt>
 * <dd>DefaultSelection, Modify, Selection, Verify, OrientationChange</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles DROP_DOWN and SIMPLE may be specified.
 * </p>
 *
 * @see List
 */
class SWTP_PUBLIC WComboBox: public WComposite {
public:
	WComboBox() {

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
	 * @see SWT#DROP_DOWN
	 * @see SWT#READ_ONLY
	 * @see SWT#SIMPLE
	 * @see Widget#getStyle
	 */
	WComboBox(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	/**
	 * Adds the argument to the receiver's list at the given
	 * zero-relative index.
	 * <p>
	 * Note: To add an item at the end of the list, use the
	 * result of calling <code>getItemCount()</code> as the
	 * index or use <code>add(String)</code>.
	 * </p>
	 *
	 * @param string the new item
	 * @param index the index for the item
	 *
	 * @see #add(String)
	 */
	WComboItem& Add(WComboItem &item, const char *string, int index) {
		_insert_item(&item, index);
		item.SetText(string);
		return item;
	}
	/**
	 * Adds the argument to the receiver's list at the given
	 * zero-relative index.
	 * <p>
	 * Note: To add an item at the end of the list, use the
	 * result of calling <code>getItemCount()</code> as the
	 * index or use <code>add(String)</code>.
	 * </p>
	 *
	 * @param string the new item
	 * @param index the index for the item
	 *
	 * @see #add(String)
	 */
	WComboItem& Add(const char *string, int index, const WComboItem &item =
			WComboItem()) {
		return Add((WComboItem&) item, string, index);
	}
	/**
	 * Adds the argument to the end of the receiver's list.
	 *
	 * @param string the new item
	 *
	 * @see #add(String,int)
	 */
	WComboItem& Add(WComboItem &item, const char *string) {
		return Add(item, string, -1);
	}
	/**
	 * Adds the argument to the end of the receiver's list.
	 *
	 * @param string the new item
	 *
	 * @see #add(String,int)
	 */
	WComboItem& Add(const char *string, const WComboItem &item = WComboItem()) {
		return Add(string, -1, item);
	}
	/**
	 * Sets the selection in the receiver's text field to an empty
	 * selection starting just before the first character. If the
	 * text field is editable, this has the effect of placing the
	 * i-beam at the start of the text.
	 * <p>
	 * Note: To clear the selected items in the receiver's list,
	 * use <code>deselectAll()</code>.
	 * </p>
	 *
	 * @see #deselectAll
	 */
	bool ClearSelection() {
		return _WReturnBool(_clear_selection());
	}
	/**
	 * Copies the selected text.
	 * <p>
	 * The current selection is copied to the clipboard.
	 * </p>
	 */
	bool Copy() {
		return _WReturnBool(_copy());
	}
	/**
	 * Cuts the selected text.
	 * <p>
	 * The current selection is first copied to the
	 * clipboard and then deleted from the widget.
	 * </p>
	 */
	bool Cut() {
		return _WReturnBool(_cut());
	}
	/**
	 * Deselects the item at the given zero-relative index in the receiver's
	 * list.  If the item at the index was already deselected, it remains
	 * deselected. Indices that are out of range are ignored.
	 *
	 * @param index the index of the item to deselect
	 */
	bool Deselect(int index) {
		return _WReturnBool(_deselect(index));
	}
	/**
	 * Deselects all selected items in the receiver's list.
	 * <p>
	 * Note: To clear the selection in the receiver's text field,
	 * use <code>clearSelection()</code>.
	 * </p>
	 *
	 * @see #clearSelection
	 */
	bool DeselectAll() {
		return _WReturnBool(_deselect_all());
	}
	WImageList* GetImageList() {
		WImageList *imagelist;
		_get_imagelist(&imagelist);
		return imagelist;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver's list. Throws an exception if the index is out
	 * of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WComboItem& GetItem(int index, WComboItem &item) {
		_get_item(index, &item);
		return item;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver's list. Throws an exception if the index is out
	 * of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WComboItem& GetItem(int index, const WComboItem &item = WComboItem()) {
		return GetItem(index, (WComboItem&) item);
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver's list. Throws an exception if the index is out
	 * of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WString GetItemText(int index) {
		return GetItem(index).GetText();
	}
	/**
	 * Returns the number of items contained in the receiver's list.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return _WReturnInt(_get_item_count());
	}
	/**
	 * Returns the height of the area which would be used to
	 * display <em>one</em> of the items in the receiver's list.
	 *
	 * @return the height of one item
	 */
	int GetItemHeight() {
		return _WReturnInt(_get_item_height());
	}
	/**
	 * Returns a (possibly empty) array of <code>String</code>s which are
	 * the items in the receiver's list.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver's list
	 */
	bool GetItems(WIterator<WComboItem> &items) {
		return _WReturnBool(_get_items(&items));
	}
	/**
	 * Returns <code>true</code> if the receiver's list is visible,
	 * and <code>false</code> otherwise.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, this method
	 * may still indicate that it is considered visible even though
	 * it may not actually be showing.
	 * </p>
	 *
	 * @return the receiver's list's visibility state
	 */
	bool GetListVisible() {
		return _WReturnBool(_get_list_visible());
	}
	/**
	 * Marks the receiver's list as visible if the argument is <code>true</code>,
	 * and marks it invisible otherwise.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, marking
	 * it visible may not actually cause it to be displayed.
	 * </p>
	 *
	 * @param visible the new visibility state
	 */
	bool SetListVisible(bool visible) {
		return _WReturnBool(_set_list_visible(visible));
	}
	/**
	 * Returns a <code>Point</code> whose x coordinate is the
	 * character position representing the start of the selection
	 * in the receiver's text field, and whose y coordinate is the
	 * character position representing the end of the selection.
	 * An "empty" selection is indicated by the x and y coordinates
	 * having the same value.
	 * <p>
	 * Indexing is zero based.  The range of a selection is from
	 * 0..N where N is the number of characters in the widget.
	 * </p>
	 *
	 * @return a point representing the selection start and end
	 */
	WPoint& GetSelection(WPoint &pt) {
		_get_selection(&pt);
		return pt;
	}
	/**
	 * Returns a <code>Point</code> whose x coordinate is the
	 * character position representing the start of the selection
	 * in the receiver's text field, and whose y coordinate is the
	 * character position representing the end of the selection.
	 * An "empty" selection is indicated by the x and y coordinates
	 * having the same value.
	 * <p>
	 * Indexing is zero based.  The range of a selection is from
	 * 0..N where N is the number of characters in the widget.
	 * </p>
	 *
	 * @return a point representing the selection start and end
	 */
	WPoint& GetSelection(const WPoint &pt = WPoint()) {
		return GetSelection((WPoint&) pt);
	}
	/**
	 * Returns the zero-relative index of the item which is currently
	 * selected in the receiver's list, or -1 if no item is selected.
	 *
	 * @return the index of the selected item
	 */
	int GetSelectionIndex() {
		return _WReturnIntIndex(_get_selection_index());
	}
	/**
	 * Returns a string containing a copy of the contents of the
	 * receiver's text field, or an empty string if there are no
	 * contents.
	 *
	 * @return the receiver's text
	 */
	WString GetText() {
		w_string_ref *ref = 0;
		_get_text(w_alloc_string_ref, &ref, W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Returns the height of the receivers's text field.
	 *
	 * @return the text height
	 */
	int GetTextHeight() {
		return _WReturnInt(_get_text_height());
	}
	/**
	 * Returns the maximum number of characters that the receiver's
	 * text field is capable of holding. If this has not been changed
	 * by <code>setTextLimit()</code>, it will be the constant
	 * <code>Combo.LIMIT</code>.
	 *
	 * @return the text limit
	 *
	 * @see #LIMIT
	 */
	int GetTextLimit() {
		return _WReturnInt(_get_text_limit());
	}
	/**
	 * Gets the number of items that are visible in the drop
	 * down portion of the receiver's list.
	 * <p>
	 * Note: This operation is a hint and is not supported on
	 * platforms that do not have this concept.
	 * </p>
	 *
	 * @return the number of items that are visible
	 */
	int GetVisibleItemCount() {
		return _WReturnInt(_get_visible_item_count());
	}
	/**
	 * Searches the receiver's list starting at the given,
	 * zero-relative index until an item is found that is equal
	 * to the argument, and returns the index of that item. If
	 * no item is found or the starting index is out of range,
	 * returns -1.
	 *
	 * @param string the search item
	 * @param start the zero-relative index at which to begin the search
	 * @return the index of the item
	 */
	int IndexOf(const char *string, int start) {
		return _WReturnIntIndex(_indexof(string, -1, W_ENCODING_UTF8, start));
	}
	/**
	 * Searches the receiver's list starting at the first item
	 * (index 0) until an item is found that is equal to the
	 * argument, and returns the index of that item. If no item
	 * is found, returns -1.
	 *
	 * @param string the search item
	 * @return the index of the item
	 */
	int IndexOf(const char *string) {
		return IndexOf(string, 0);
	}
	/**
	 * Pastes text from clipboard.
	 * <p>
	 * The selected text is deleted from the widget
	 * and new text inserted from the clipboard.
	 * </p>
	 */
	bool Paste() {
		return _WReturnBool(_paste());
	}
	/**
	 * Removes the item from the receiver's list at the given
	 * zero-relative index.
	 *
	 * @param index the index for the item
	 *
	 */
	bool Remove(int index) {
		return _WReturnBool(_remove_index(index));
	}
	/**
	 * Removes the items from the receiver's list which are
	 * between the given zero-relative start and end
	 * indices (inclusive).
	 *
	 * @param start the start of the range
	 * @param end the end of the range
	 */
	bool Remove(int start, int end) {
		return _WReturnBool(_remove_range(start, end));
	}
	/**
	 * Searches the receiver's list starting at the first item
	 * until an item is found that is equal to the argument,
	 * and removes that item from the list.
	 *
	 * @param string the item to remove
	 */
	bool Remove(const char *string) {
		return _WReturnBool(_remove_string(string, -1, W_ENCODING_UTF8));
	}
	/**
	 * Removes all of the items from the receiver's list and clear the
	 * contents of receiver's text field.
	 * <p>
	 */
	bool RemoveAll() {
		return _WReturnBool(_remove_all());
	}
	/**
	 * Selects the item at the given zero-relative index in the receiver's
	 * list.  If the item at the index was already selected, it remains
	 * selected. Indices that are out of range are ignored.
	 *
	 * @param index the index of the item to select
	 */
	bool Select(int index) {
		return _WReturnBool(_select(index));
	}
	bool SetImageList(WImageList *imagelist) {
		return _WReturnBool(_set_imagelist(imagelist));
	}
	/**
	 * Sets the text of the item in the receiver's list at the given
	 * zero-relative index to the string argument.
	 *
	 * @param index the index for the item
	 * @param string the new text for the item
	 */
	bool SetItem(int index, const char *string) {
		WComboItem item;
		GetItem(index, item);
		return item.SetText(string);
	}
	/**
	 * Sets the selection in the receiver's text field to the
	 * range specified by the argument whose x coordinate is the
	 * start of the selection and whose y coordinate is the end
	 * of the selection.
	 *
	 * @param selection a point representing the new selection start and end
	 */
	bool SetSelection(const WPoint &selection) {
		return _WReturnBool(_set_selection(&selection));
	}
	/**
	 * Sets the contents of the receiver's text field to the
	 * given string.
	 * <p>
	 * This call is ignored when the receiver is read only and
	 * the given string is not in the receiver's list.
	 * </p>
	 * <p>
	 * Note: The text field in a <code>Combo</code> is typically
	 * only capable of displaying a single line of text. Thus,
	 * setting the text to a string containing line breaks or
	 * other special characters will probably cause it to
	 * display incorrectly.
	 * </p>
	 *
	 * @param string the new text
	 */
	bool SetText(const char *string) {
		return _WReturnBool(_set_text(string, -1, W_ENCODING_UTF8));
	}
	/**
	 * Sets the maximum number of characters that the receiver's
	 * text field is capable of holding to be the argument.
	 * <p>
	 * To reset this value to the default, use <code>setTextLimit(Combo.LIMIT)</code>.
	 * Specifying a limit value larger than <code>Combo.LIMIT</code> sets the
	 * receiver's limit to <code>Combo.LIMIT</code>.
	 * </p>
	 * @param limit new text limit
	 *
	 * @see #LIMIT
	 */
	bool SetTextLimit(int limit) {
		return _WReturnBool(_set_text_limit(limit));
	}
	/**
	 * Sets the number of items that are visible in the drop
	 * down portion of the receiver's list.
	 * <p>
	 * Note: This operation is a hint and is not supported on
	 * platforms that do not have this concept.
	 * </p>
	 *
	 * @param count the new number of items to be visible
	 */
	bool SetVisibleItemCount(int count) {
		return _WReturnBool(_set_visible_item_count(count));
	}
protected:
	w_class_id _GetClassID();
public:
	WResult _clear_selection() {
		return w_combobox_clear_selection(W_COMBOBOX(this));
	}
	WResult _copy() {
		return w_combobox_copy(W_COMBOBOX(this));
	}
	WResult _cut() {
		return w_combobox_cut(W_COMBOBOX(this));
	}
	WResult _deselect(int index) {
		return w_combobox_deselect(W_COMBOBOX(this), index);
	}
	WResult _deselect_all() {
		return w_combobox_deselect_all(W_COMBOBOX(this));
	}
	WResult _get_imagelist(WImageList **imagelist) {
		return w_combobox_get_imagelist(W_COMBOBOX(this),
				(w_imagelist**) imagelist);
	}
	WResult _get_item(int index, WComboItem *item) {
		return w_combobox_get_item(W_COMBOBOX(this), index, W_COMBOITEM(item));
	}
	WResult _get_item_count() {
		return w_combobox_get_item_count(W_COMBOBOX(this));
	}
	WResult _get_item_height() {
		return w_combobox_get_item_height(W_COMBOBOX(this));
	}
	WResult _get_items(WIterator<WComboItem> *items) {
		return w_combobox_get_items(W_COMBOBOX(this), (w_iterator*) items);
	}
	WResult _get_list_visible() {
		return w_combobox_get_list_visible(W_COMBOBOX(this));
	}
	WResult _get_selection(WPoint *pt) {
		return w_combobox_get_selection(W_COMBOBOX(this), (w_point*) pt);
	}
	WResult _get_selection_index() {
		return w_combobox_get_selection_index(W_COMBOBOX(this));
	}
	WResult _get_text(w_alloc alloc, void *user_data, int enc) {
		return w_combobox_get_text(W_COMBOBOX(this), alloc, user_data, enc);
	}
	WResult _get_text_height() {
		return w_combobox_get_text_height(W_COMBOBOX(this));
	}
	WResult _get_text_limit() {
		return w_combobox_get_text_limit(W_COMBOBOX(this));
	}
	WResult _get_visible_item_count() {
		return w_combobox_get_visible_item_count(W_COMBOBOX(this));
	}
	WResult _indexof(const char *text, int length, int enc, int start) {
		return w_combobox_indexof(W_COMBOBOX(this), text, length, enc, start);
	}
	WResult _insert_item(WComboItem *item, int index) {
		return w_combobox_insert_item(W_COMBOBOX(this), W_COMBOITEM(item),
				index);
	}
	WResult _paste() {
		return w_combobox_paste(W_COMBOBOX(this));
	}
	WResult _remove_index(int index) {
		return w_combobox_remove_index(W_COMBOBOX(this), index);
	}
	WResult _remove_range(int start, int end) {
		return w_combobox_remove_range(W_COMBOBOX(this), start, end);
	}
	WResult _remove_string(const char *string, int length, int enc) {
		return w_combobox_remove_string(W_COMBOBOX(this), string, length, enc);
	}
	WResult _remove_all() {
		return w_combobox_remove_all(W_COMBOBOX(this));
	}
	WResult _select(int index) {
		return w_combobox_select(W_COMBOBOX(this), index);
	}
	WResult _set_imagelist(WImageList *imagelist) {
		return w_combobox_set_imagelist(W_COMBOBOX(this),
				W_IMAGELIST(imagelist));
	}
	WResult _set_list_visible(int visible) {
		return w_combobox_set_list_visible(W_COMBOBOX(this), visible);
	}
	WResult _set_selection(const WPoint *selection) {
		return w_combobox_set_selection(W_COMBOBOX(this), (w_point*) selection);
	}
	WResult _set_text(const char *text, int length, int enc) {
		return w_combobox_set_text(W_COMBOBOX(this), text, length, enc);
	}
	WResult _set_text_limit(int limit) {
		return w_combobox_set_text_limit(W_COMBOBOX(this), limit);
	}
	WResult _set_visible_item_count(int count) {
		return w_combobox_set_visible_item_count(W_COMBOBOX(this), count);
	}
private:
	void *handles[(sizeof(w_combobox) - sizeof(w_composite)) / sizeof(void*)];
};
#endif /* SWTP_CONTROLS_COMBO_H_ */
