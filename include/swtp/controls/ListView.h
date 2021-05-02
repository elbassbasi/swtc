/*
 * Table.h
 *
 *  Created on: 7 août 2019
 *      Author: azeddine El Bassbasi
 */
#ifndef SWTP_CONTROLS_TABLE_H_
#define SWTP_CONTROLS_TABLE_H_
#include "../widgets/Composite.h"
class WListView;
/**
 * Instances of this class represent a column in a table widget.
 * <p><dl>
 * <dt><b>Styles:</b></dt>
 * <dd>LEFT, RIGHT, CENTER</dd>
 * <dt><b>Events:</b></dt>
 * <dd> Move, Resize, Selection</dd>
 * </dl>
 * </p><p>
 * Note: Only one of the styles LEFT, RIGHT and CENTER may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WColumnItem: public WItem {
public:
	WColumnItem() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>Table</code>) and a style value
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
	 * @param style the style of control to construct
	 *
	 * @see SWT#LEFT
	 * @see SWT#RIGHT
	 * @see SWT#CENTER
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WListView *parent, const char *text) {
		return Create(parent, text, -1);
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>Table</code>), a style value
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
	 * <p>
	 * Note that due to a restriction on some platforms, the first column
	 * is always left aligned.
	 * </p>
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 * @param index the zero-relative index to store the receiver in its parent
	 *
	 * @see SWT#LEFT
	 * @see SWT#RIGHT
	 * @see SWT#CENTER
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WListView *parent, const char *text, int index) {
		WResult result = w_listviewbase_insert_column(W_LISTVIEWBASE(parent),
				W_COLUMNITEM(this), index);
		this->SetText(text);
		return _WReturnBool(result);
	}
	/**
	 * Returns a value which describes the position of the
	 * text or image in the receiver. The value will be one of
	 * <code>LEFT</code>, <code>RIGHT</code> or <code>CENTER</code>.
	 *
	 * @return the alignment
	 */
	int GetAlignment() {
		return _WReturnInt(_get_alignment());
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Table</code>.
	 *
	 * @return the receiver's parent
	 */
	WListView* GetParent() {
		return (WListView*) WItem::GetParentWidget();
	}
	/**
	 * Gets the moveable attribute. A column that is
	 * not moveable cannot be reordered by the user
	 * by dragging the header but may be reordered
	 * by the programmer.
	 *
	 * @return the moveable attribute
	 *
	 * @see Table#getColumnOrder()
	 * @see Table#setColumnOrder(int[])
	 * @see TableColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	bool GetMoveable() {
		return _WReturnBool(_get_moveable());
	}
	/**
	 * Gets the resizable attribute. A column that is
	 * not resizable cannot be dragged by the user but
	 * may be resized by the programmer.
	 *
	 * @return the resizable attribute
	 */
	bool GetResizable() {
		return _WReturnBool(_get_resizable());
	}
	/**
	 * Returns the receiver's tool tip text, or null if it has
	 * not been set.
	 *
	 * @return the receiver's tool tip text
	 */
	WString GetToolTipText() {
		w_string_ref *ref = 0;
		_get_tooltip_text(w_alloc_string_ref, &ref, W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Gets the width of the receiver.
	 *
	 * @return the width
	 */
	int GetWidth() {
		return _WReturnInt(_get_width());
	}
	/**
	 * Causes the receiver to be resized to its preferred size.
	 * For a composite, this involves computing the preferred size
	 * from its layout, if there is one.
	 */
	bool Pack() {
		return _WReturnBool(_pack());
	}
	/**
	 * Controls how text and images will be displayed in the receiver.
	 * The argument should be one of <code>LEFT</code>, <code>RIGHT</code>
	 * or <code>CENTER</code>.
	 * <p>
	 * Note that due to a restriction on some platforms, the first column
	 * is always left aligned.
	 * </p>
	 * @param alignment the new alignment
	 */
	bool SetAlignment(int alignment) {
		return _WReturnBool(_set_alignment(alignment));
	}
	bool SetImage(int image) {
		return _WReturnBool(_set_image(image));
	}
	/**
	 * Sets the moveable attribute.  A column that is
	 * moveable can be reordered by the user by dragging
	 * the header. A column that is not moveable cannot be
	 * dragged by the user but may be reordered
	 * by the programmer.
	 *
	 * @param moveable the moveable attribute
	 *
	 * @see WListView#setColumnOrder(int[])
	 * @see WListView#getColumnOrder()
	 * @see WListViewColumn#getMoveable()
	 * @see SWT#Move
	 */
	bool SetMoveable(bool moveable) {
		return _WReturnBool(_set_moveable(moveable));
	}
	/**
	 * Sets the resizable attribute.  A column that is
	 * resizable can be resized by the user dragging the
	 * edge of the header.  A column that is not resizable
	 * cannot be dragged by the user but may be resized
	 * by the programmer.
	 *
	 * @param resizable the resize attribute
	 */
	bool SetResizable(bool resizable) {
		return _WReturnBool(_set_resizable(resizable));
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
	bool SetToolTipText(const char *string) {
		return _WReturnBool(_set_tooltip_text(string, -1, W_ENCODING_UTF8));
	}
	/**
	 * Sets the width of the receiver.
	 *
	 * @param width the new width
	 */
	bool SetWidth(int width) {
		return _WReturnBool(_set_width(width));
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
	 * @see #SetData(void*)
	 */
	void* GetData() {
		return WWidget::GetItemData(this);
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
		return WWidget::SetItemData(this, data);
	}
public:
	WResult _get_alignment() {
		return w_columnitem_get_alignment(W_COLUMNITEM(this));
	}
	WResult _get_image() {
		return w_columnitem_get_image(W_COLUMNITEM(this));
	}
	WResult _get_moveable() {
		return w_columnitem_get_moveable(W_COLUMNITEM(this));
	}
	WResult _get_resizable() {
		return w_columnitem_get_resizable(W_COLUMNITEM(this));
	}
	WResult _get_tooltip_text(w_alloc alloc, void *user_data, int enc) {
		return w_columnitem_get_tooltip_text(W_COLUMNITEM(this), alloc,
				user_data, enc);
	}
	WResult _get_width() {
		return w_columnitem_get_width(W_COLUMNITEM(this));
	}
	WResult _pack() {
		return w_columnitem_pack(W_COLUMNITEM(this));
	}
	WResult _set_alignment(int alignment) {
		return w_columnitem_set_alignment(W_COLUMNITEM(this), alignment);
	}
	WResult _set_image(int image) {
		return w_columnitem_set_image(W_COLUMNITEM(this), image);
	}
	WResult _set_moveable(int moveable) {
		return w_columnitem_set_moveable(W_COLUMNITEM(this), moveable);
	}
	WResult _set_resizable(int resizable) {
		return w_columnitem_set_resizable(W_COLUMNITEM(this), resizable);
	}
	WResult _set_tooltip_text(const char *text, int length, int enc) {
		return w_columnitem_set_tooltip_text(W_COLUMNITEM(this), text, length,
				enc);
	}
	WResult _set_width(int width) {
		return w_columnitem_set_width(W_COLUMNITEM(this), width);
	}
private:
	void *handle[(sizeof(w_columnitem) - sizeof(w_item)) / sizeof(void*)];
};
/**
 * Instances of this class represent a selectable user interface object
 * that represents an item in a WListView.
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
class SWTP_PUBLIC WListItem: public WItem {
public:
	WListItem() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>WListView</code>) and a style value
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
	 * @param style the style of control to construct
	 *
	 * @see SWT
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WListView *parent, const char *string) {
		return Create(parent, string, -1);
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>WListView</code>), a style value
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
	 * @param style the style of control to construct
	 * @param index the zero-relative index to store the receiver in its parent
	 *
	 * @see SWT
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WListView *parent, const char *string, int index) {
		WResult result = w_listview_insert_item(W_LISTVIEW(parent),
				W_LISTITEM(this), index);
		this->SetText(string);
		return _WReturnBool(result);
	}
	/**
	 * Returns a rectangle describing the size and location of the receiver's
	 * text relative to its parent.
	 *
	 * @return the bounding rectangle of the receiver's text
	 */
	WRect& GetBounds(WRect &bounds) {
		_get_bounds(&bounds);
		return bounds;
	}
	/**
	 * Returns a rectangle describing the size and location of the receiver's
	 * text relative to its parent.
	 *
	 * @return the bounding rectangle of the receiver's text
	 */
	WRect GetBounds() {
		WRect bounds;
		return GetBounds((WRect&) bounds);
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent at a column in the WListView.
	 *
	 * @param index the index that specifies the column
	 * @return the receiver's bounding column rectangle
	 */
	WRect& GetBounds(int index, WRect &bounds) {
		_get_bounds_index(index, &bounds);
		return bounds;
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent at a column in the WListView.
	 *
	 * @param index the index that specifies the column
	 * @return the receiver's bounding column rectangle
	 */
	WRect GetBounds(int index) {
		WRect bounds;
		return GetBounds(index, (WRect&) bounds);
	}
	/**
	 * Returns <code>true</code> if the receiver is checked,
	 * and false otherwise.  When the parent does not have
	 * the <code>CHECK</code> style, return false.
	 *
	 * @return the checked state of the checkbox
	 */
	bool GetChecked() {
		return _WReturnBool(_get_checked());
	}
	/**
	 * Returns the receiver's parent, which must be a <code>WListView</code>.
	 *
	 * @return the receiver's parent
	 */
	WListView* GetParent() {
		return (WListView*) WItem::GetParentWidget();
	}
	bool SetImage(int image) {
		return _WReturnBool(_set_image(image));
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
	 * @see #SetData(void*)
	 */
	void* GetData() {
		return WWidget::GetItemData(this);
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
		return WWidget::SetItemData(this, data);
	}
public:
	WResult _get_bounds(WRect *bounds) {
		return w_listitem_get_bounds(W_LISTITEM(this), (w_rect*) bounds);
	}
	WResult _get_bounds_index(int index, WRect *bounds) {
		return w_listitem_get_bounds_index(W_LISTITEM(this), index,
				(w_rect*) bounds);
	}
	WResult _get_checked() {
		return w_listitem_get_checked(W_LISTITEM(this));
	}
	WResult _get_image() {
		return w_listitem_get_image(W_LISTITEM(this));
	}
	WResult _set_checked(int checked) {
		return w_listitem_set_checked(W_LISTITEM(this), checked);
	}
	WResult _set_image(int image) {
		return w_listitem_set_image(W_LISTITEM(this), image);
	}
private:
	void *handle[(sizeof(w_listitem) - sizeof(w_item)) / sizeof(void*)];
};
class WListAttr {
public:
	WFont *font;
	WColor background;
	WColor foreground;
};
class WTreeItem;
class WListEvent: public WEvent {
public:
	union {
		int detail;
		struct {
			unsigned doit :1;
			unsigned selected :1;
			unsigned focused :1;
			unsigned background :1;
			unsigned forground :1;
			unsigned hot :1;
		};
	};
	WRect *rect;
	WColumnItem *column;
	WListItem *item;
	WGraphics *gc;
	WValue *value;
	WListAttr *attr;
	WTreeItem* GetTreeItem() {
		return (WTreeItem*) item;
	}
	WListItem* GetListItem() {
		return item;
	}
	WListItem* GetItem() {
		return item;
	}
};
class SWTP_PUBLIC WListViewBase: public WComposite {
public:
	/**
	 * Clears all the items in the receiver. The text, icon and other
	 * attributes of the items are set to their default values. If the
	 * WListView was created with the <code>SWT.VIRTUAL</code> style, these
	 * attributes are requested again as needed.
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	bool ClearAll() {
		return _WReturnBool(_clear_all());
	}
	/**
	 * Deselects all selected items in the receiver.
	 */
	bool DeselectAll() {
		return _WReturnBool(_deselect_all());
	}
	/**
	 * Returns the column at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 * Columns are returned in the order that they were created.
	 * If no <code>TableColumn</code>s were created by the programmer,
	 * this method will throw <code>ERROR_INVALID_RANGE</code> despite
	 * the fact that a single column of data may be visible in the WListView.
	 * This occurs when the programmer uses the WListView like a list, adding
	 * items but never creating a column.
	 *
	 * @param index the index of the column to return
	 * @return the column at the given index
	 *
	 * @see WListView#getColumnOrder()
	 * @see WListView#setColumnOrder(int[])
	 * @see WListViewColumn#getMoveable()
	 * @see WListViewColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	WColumnItem& GetColumn(int index, WColumnItem &column) {
		_get_column(index, &column);
		return column;
	}
	/**
	 * Returns the column at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 * Columns are returned in the order that they were created.
	 * If no <code>TableColumn</code>s were created by the programmer,
	 * this method will throw <code>ERROR_INVALID_RANGE</code> despite
	 * the fact that a single column of data may be visible in the table.
	 * This occurs when the programmer uses the table like a list, adding
	 * items but never creating a column.
	 *
	 * @param index the index of the column to return
	 * @return the column at the given index
	 *
	 * @see WListView#getColumnOrder()
	 * @see WListView#setColumnOrder(int[])
	 * @see WListViewColumn#getMoveable()
	 * @see WListViewColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	WColumnItem GetColumn(int index) {
		WColumnItem item;
		_get_column(index, &item);
		return item;
	}
	/**
	 * Returns the number of columns contained in the receiver.
	 * If no <code>TableColumn</code>s were created by the programmer,
	 * this value is zero, despite the fact that visually, one column
	 * of items may be visible. This occurs when the programmer uses
	 * the table like a list, adding items but never creating a column.
	 *
	 * @return the number of columns
	 */
	int GetColumnCount() {
		return _WReturnInt(_get_column_count());
	}
	/**
	 * Returns an array of <code>TableColumn</code>s which are the
	 * columns in the receiver.  Columns are returned in the order
	 * that they were created.  If no <code>TableColumn</code>s were
	 * created by the programmer, the array is empty, despite the fact
	 * that visually, one column of items may be visible. This occurs
	 * when the programmer uses the table like a list, adding items but
	 * never creating a column.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 *
	 * @see WListView#getColumnOrder()
	 * @see WListView#setColumnOrder(int[])
	 * @see WListViewColumn#getMoveable()
	 * @see WListViewColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	bool GetColumns(WIterator<WColumnItem> &columns) {
		return _WReturnBool(_get_columns(&columns));
	}
	/**
	 * Returns the width in pixels of a grid line.
	 *
	 * @return the width of a grid line in pixels
	 */
	int GetGridLineWidth() {
		return _WReturnInt(_get_gridline_width());
	}
	/**
	 * Returns the height of the receiver's header
	 *
	 * @return the height of the header or zero if the header is not visible
	 */
	int GetHeaderHeight() {
		return _WReturnInt(_get_header_height());
	}
	WImageList* GetHeaderImageList() {
		WImageList *imagelist;
		_get_header_imagelist(&imagelist);
		return imagelist;
	}
	/**
	 * Returns <code>true</code> if the receiver's header is visible,
	 * and <code>false</code> otherwise.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, this method
	 * may still indicate that it is considered visible even though
	 * it may not actually be showing.
	 * </p>
	 *
	 * @return the receiver's header's visibility state
	 */
	bool GetHeaderVisible() {
		return _WReturnBool(_get_header_visible());
	}
	WImageList* GetImageList() {
		WImageList *imagelist;
		_get_imagelist(&imagelist);
		return imagelist;
	}
	/**
	 * Returns the height of the area which would be used to
	 * display <em>one</em> of the items in the receiver.
	 *
	 * @return the height of one item
	 */
	int GetItemHeight() {
		return _WReturnInt(_get_item_height());
	}
	/**
	 * Returns <code>true</code> if the receiver's lines are visible,
	 * and <code>false</code> otherwise. Note that some platforms draw
	 * grid lines while others may draw alternating row colors.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, this method
	 * may still indicate that it is considered visible even though
	 * it may not actually be showing.
	 * </p>
	 *
	 * @return the visibility state of the lines
	 */
	bool GetLinesVisible() {
		return _WReturnBool(_get_lines_visible());
	}
	/**
	 * Returns the number of selected items contained in the receiver.
	 *
	 * @return the number of selected items
	 */
	int GetSelectionCount() {
		return _WReturnBool(_get_selection_count());
	}
	/**
	 * Returns the column which shows the sort indicator for
	 * the receiver. The value may be null if no column shows
	 * the sort indicator.
	 *
	 * @return the sort indicator
	 *
	 * @see #setSortColumn(TableColumn)
	 */
	WColumnItem& GetSortColumn(WColumnItem &column) {
		_get_sort_column(&column);
		return column;
	}
	/**
	 * Returns the column which shows the sort indicator for
	 * the receiver. The value may be null if no column shows
	 * the sort indicator.
	 *
	 * @return the sort indicator
	 *
	 * @see #setSortColumn(TableColumn)
	 */
	WColumnItem& GetSortColumn(const WColumnItem &column = WColumnItem()) {
		return GetSortColumn((WColumnItem&) column);
	}
	/**
	 * Returns the direction of the sort indicator for the receiver.
	 * The value will be one of <code>UP</code>, <code>DOWN</code>
	 * or <code>NONE</code>.
	 *
	 * @return the sort direction
	 *
	 * @see #setSortDirection(int)
	 */
	int GetSortDirection() {
		return _WReturnInt(_get_sort_direction());
	}
	bool _InsertColumn(WColumnItem &column, const char *text, int index) {
		WResult result = _insert_column(&column, index);
		column.SetText(text);
		return _WReturnBool(result);
	}
	WColumnItem& InsertColumn(WColumnItem &column, const char *text,
			int index) {
		_InsertColumn(column, text, index);
		return column;
	}
	WColumnItem& AppendColumn(WColumnItem &column, const char *text) {
		_InsertColumn(column, text, -1);
		return column;
	}
	/**
	 * Removes all of the items from the receiver.
	 */
	bool RemoveAll() {
		return _WReturnBool(_remove_all());
	}
	/**
	 * Selects all of the items in the receiver.
	 * <p>
	 * If the receiver is single-select, do nothing.
	 * </p>
	 */
	bool SelectAll() {
		return _WReturnBool(_select_all());
	}
	bool SetHeaderImageList(WImageList *imagelist) {
		return _WReturnBool(_set_header_imagelist(imagelist));
	}
	/**
	 * Marks the receiver's header as visible if the argument is <code>true</code>,
	 * and marks it invisible otherwise.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, marking
	 * it visible may not actually cause it to be displayed.
	 * </p>
	 *
	 * @param show the new visibility state
	 */
	bool SetHeaderVisible(bool show) {
		return _WReturnBool(_set_header_visible(show));
	}
	bool SetImageList(WImageList *imagelist) {
		return _WReturnBool(_set_imagelist(imagelist));
	}
	/**
	 * Marks the receiver's lines as visible if the argument is <code>true</code>,
	 * and marks it invisible otherwise. Note that some platforms draw grid lines
	 * while others may draw alternating row colors.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, marking
	 * it visible may not actually cause it to be displayed.
	 * </p>
	 *
	 * @param show the new visibility state
	 */
	bool SetLinesVisible(bool show) {
		return _WReturnBool(_set_lines_visible(show));
	}
	/**
	 * Sets the column used by the sort indicator for the receiver. A null
	 * value will clear the sort indicator.  The current sort column is cleared
	 * before the new column is set.
	 *
	 * @param column the column used by the sort indicator or <code>null</code>
	 */
	bool SetSortColumn(WColumnItem &column) {
		return _WReturnBool(_set_sort_column(&column));
	}
	/**
	 * Sets the direction of the sort indicator for the receiver. The value
	 * can be one of <code>UP</code>, <code>DOWN</code> or <code>NONE</code>.
	 *
	 * @param direction the direction of the sort indicator
	 */
	bool SetSortDirection(int direction) {
		return _WReturnBool(_set_sort_direction(direction));
	}
	/**
	 * Shows the column.  If the column is already showing in the receiver,
	 * this method simply returns.  Otherwise, the columns are scrolled until
	 * the column is visible.
	 *
	 * @param column the column to be shown
	 */
	bool ShowColumn(WColumnItem &column) {
		return _WReturnBool(_show_column(&column));
	}
	/**
	 * Shows the selection.  If the selection is already showing in the receiver,
	 * this method simply returns.  Otherwise, the items are scrolled until
	 * the selection is visible.
	 *
	 * @see WListView#showItem(TableItem)
	 */
	bool ShowSelection() {
		return _WReturnBool(_show_selection());
	}
protected:
	w_class_id _GetClassID();
public:
	WResult _clear_all() {
		return w_listviewbase_clear_all(W_LISTVIEWBASE(this));
	}
	WResult _deselect_all() {
		return w_listviewbase_deselect_all(W_LISTVIEWBASE(this));
	}
	WResult _get_column(int index, WColumnItem *column) {
		return w_listviewbase_get_column(W_LISTVIEWBASE(this), index,
				W_COLUMNITEM(column));
	}
	WResult _get_column_count() {
		return w_listviewbase_get_column_count(W_LISTVIEWBASE(this));
	}
	WResult _get_columns(WIterator<WColumnItem> *columns) {
		return w_listviewbase_get_columns(W_LISTVIEWBASE(this),
				(w_iterator*) columns);
	}
	WResult _get_gridline_width() {
		return w_listviewbase_get_gridline_width(W_LISTVIEWBASE(this));
	}
	WResult _get_header_height() {
		return w_listviewbase_get_header_height(W_LISTVIEWBASE(this));
	}
	WResult _get_header_imagelist(WImageList **imagelist) {
		return w_listviewbase_get_header_imagelist(W_LISTVIEWBASE(this),
				(w_imagelist**) imagelist);
	}
	WResult _get_header_visible() {
		return w_listviewbase_get_header_visible(W_LISTVIEWBASE(this));
	}
	WResult _get_imagelist(WImageList **imagelist) {
		return w_listviewbase_get_imagelist(W_LISTVIEWBASE(this),
				(w_imagelist**) imagelist);
	}
	WResult _get_item_height() {
		return w_listviewbase_get_item_height(W_LISTVIEWBASE(this));
	}
	WResult _get_lines_visible() {
		return w_listviewbase_get_lines_visible(W_LISTVIEWBASE(this));
	}
	WResult _get_selection(w_iterator *selection) {
		return w_listviewbase_get_selection(W_LISTVIEWBASE(this), selection);
	}
	WResult _get_selection_count() {
		return w_listviewbase_get_selection_count(W_LISTVIEWBASE(this));
	}
	WResult _get_sort_column(WColumnItem *column) {
		return w_listviewbase_get_sort_column(W_LISTVIEWBASE(this),
				W_COLUMNITEM(column));
	}
	WResult _get_sort_direction() {
		return w_listviewbase_get_sort_direction(W_LISTVIEWBASE(this));
	}
	WResult _insert_column(WColumnItem *column, int index) {
		return w_listviewbase_insert_column(W_LISTVIEWBASE(this),
				W_COLUMNITEM(column), index);
	}
	WResult _remove_all() {
		return w_listviewbase_remove_all(W_LISTVIEWBASE(this));
	}
	WResult _select_all() {
		return w_listviewbase_select_all(W_LISTVIEWBASE(this));
	}
	wresult _set_header_imagelist(WImageList *imagelist) {
		return w_listviewbase_set_header_imagelist(W_LISTVIEWBASE(this),
				W_IMAGELIST(imagelist));
	}
	WResult _set_header_visible(int show) {
		return w_listviewbase_set_header_visible(W_LISTVIEWBASE(this), show);
	}
	WResult _set_imagelist(WImageList *imagelist) {
		return w_listviewbase_set_imagelist(W_LISTVIEWBASE(this),
				W_IMAGELIST(imagelist));
	}
	WResult _set_item_height(int itemHeight) {
		return w_listviewbase_set_item_height(W_LISTVIEWBASE(this), itemHeight);
	}
	WResult _set_lines_visible(int show) {
		return w_listviewbase_set_lines_visible(W_LISTVIEWBASE(this), show);
	}
	WResult _set_sort_column(WColumnItem *column) {
		return w_listviewbase_set_sort_column(W_LISTVIEWBASE(this),
				W_COLUMNITEM(column));
	}
	WResult _set_sort_direction(int direction) {
		return w_listviewbase_set_sort_direction(W_LISTVIEWBASE(this),
				direction);
	}
	WResult _show_column(WColumnItem *column) {
		return w_listviewbase_show_column(W_LISTVIEWBASE(this),
				W_COLUMNITEM(column));
	}
	WResult _show_selection() {
		return w_listviewbase_show_selection(W_LISTVIEWBASE(this));
	}
	WResult _sort() {
		return w_listviewbase_sort(W_LISTVIEWBASE(this));
	}
private:
	void *handles[(sizeof(w_listviewbase) - sizeof(w_composite)) / sizeof(void*)];
};
/**
 * Instances of this class implement a selectable user interface
 * object that displays a list of images and strings and issues
 * notification when selected.
 * <p>
 * The item children that may be added to instances of this class
 * must be of type <code>TableItem</code>.
 * </p><p>
 * Style <code>VIRTUAL</code> is used to create a <code>Table</code> whose
 * <code>TableItem</code>s are to be populated by the client on an on-demand basis
 * instead of up-front.  This can provide significant performance improvements for
 * tables that are very large or for which <code>TableItem</code> population is
 * expensive (for example, retrieving values from an external source).
 *
 *
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not normally make sense to add <code>Control</code> children to
 * it, or set a layout on it, unless implementing something like a cell
 * editor.
 * </p><p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>SINGLE, MULTI, CHECK, FULL_SELECTION, HIDE_SELECTION, VIRTUAL, NO_SCROLL</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection, DefaultSelection, SetData, MeasureItem, EraseItem, PaintItem</dd>
 * </dl>
 * </p><p>
 * Note: Only one of the styles SINGLE, and MULTI may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */

class SWTP_PUBLIC WListView: public WListViewBase {
public:
	WListView() {

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
	 * @see SWT#HIDE_SELECTION
	 * @see SWT#VIRTUAL
	 * @see SWT#NO_SCROLL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WListView(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	WListItem& AppendItem(WListItem &item, const char *text) {
		return InsertItem(item, text, -1);
	}
	/**
	 * Clears the item at the given zero-relative index in the receiver.
	 * The text, icon and other attributes of the item are set to the default
	 * value.  If the table was created with the <code>SWT.VIRTUAL</code> style,
	 * these attributes are requested again as needed.
	 *
	 * @param index the index of the item to clear
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	bool Clear(int index) {
		return _WReturnInt(_clear_index(index));
	}
	/**
	 * Removes the items from the receiver which are between the given
	 * zero-relative start and end indices (inclusive).  The text, icon
	 * and other attributes of the items are set to their default values.
	 * If the table was created with the <code>SWT.VIRTUAL</code> style,
	 * these attributes are requested again as needed.
	 *
	 * @param start the start index of the item to clear
	 * @param end the end index of the item to clear
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	bool Clear(int start, int end) {
		return _WReturnInt(_clear_range(start, end));
	}
	/**
	 * Clears the items at the given zero-relative indices in the receiver.
	 * The text, icon and other attributes of the items are set to their default
	 * values.  If the table was created with the <code>SWT.VIRTUAL</code> style,
	 * these attributes are requested again as needed.
	 *
	 * @param indices the array of indices of the items
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	bool Clear(int *indices, size_t length) {
		return _WReturnInt(_clear_indices(indices, length));
	}
	/**
	 * Deselects the items at the given zero-relative indices in the receiver.
	 * If the item at the given zero-relative index in the receiver
	 * is selected, it is deselected.  If the item at the index
	 * was not selected, it remains deselected. Indices that are out
	 * of range and duplicate indices are ignored.
	 *
	 * @param indices the array of indices for the items to deselect
	 */
	bool Deselect(int *indices, size_t length) {
		return _WReturnInt(_deselect_indices(indices, length));
	}
	/**
	 * Deselects the item at the given zero-relative index in the receiver.
	 * If the item at the index was already deselected, it remains
	 * deselected. Indices that are out of range are ignored.
	 *
	 * @param index the index of the item to deselect
	 */
	bool Deselect(int index) {
		return _WReturnInt(_deselect_index(index));
	}
	/**
	 * Deselects the items at the given zero-relative indices in the receiver.
	 * If the item at the given zero-relative index in the receiver
	 * is selected, it is deselected.  If the item at the index
	 * was not selected, it remains deselected.  The range of the
	 * indices is inclusive. Indices that are out of range are ignored.
	 *
	 * @param start the start index of the items to deselect
	 * @param end the end index of the items to deselect
	 */
	bool Deselect(int start, int end) {
		return _WReturnInt(_deselect_range(start, end));
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WListItem& GetItem(int index, WListItem &item) {
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
	WListItem& GetItem(int index) {
		WListItem item;
		return GetItem(index, item);
	}
	/**
	 * Returns the item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 * <p>
	 * The item that is returned represents an item that could be selected by the user.
	 * For example, if selection only occurs in items in the first column, then null is
	 * returned if the point is outside of the item.
	 * Note that the SWT.FULL_SELECTION style hint, which specifies the selection policy,
	 * determines the extent of the selection.
	 * </p>
	 *
	 * @param point the point used to locate the item
	 * @return the item at the given point, or null if the point is not in a selectable item
	 */
	WListItem& GetItem(const WPoint &point, WListItem &item) {
		_get_item_from_point(&point, &item);
		return item;
	}
	/**
	 * Returns the item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 * <p>
	 * The item that is returned represents an item that could be selected by the user.
	 * For example, if selection only occurs in items in the first column, then null is
	 * returned if the point is outside of the item.
	 * Note that the SWT.FULL_SELECTION style hint, which specifies the selection policy,
	 * determines the extent of the selection.
	 * </p>
	 *
	 * @param point the point used to locate the item
	 * @return the item at the given point, or null if the point is not in a selectable item
	 */
	WListItem GetItem(const WPoint &point) {
		WListItem item;
		return GetItem(point, (WListItem&) item);
	}
	/**
	 * Returns the item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 * <p>
	 * The item that is returned represents an item that could be selected by the user.
	 * For example, if selection only occurs in items in the first column, then null is
	 * returned if the point is outside of the item.
	 * Note that the SWT.FULL_SELECTION style hint, which specifies the selection policy,
	 * determines the extent of the selection.
	 * </p>
	 *
	 * @param point the point used to locate the item
	 * @return the item at the given point, or null if the point is not in a selectable item
	 */
	WListItem GetItem(int x, int y) {
		WListItem item;
		return GetItem(WPoint(x, y), (WListItem&) item);
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
	 * Returns a (possibly empty) array of <code>TableItem</code>s which
	 * are the items in the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 */
	bool GetItems(WIterator<WListItem> &items) {
		return _WReturnBool(_get_items(&items));
	}
	/**
	 * Returns an array of <code>TableItem</code>s that are currently
	 * selected in the receiver. The order of the items is unspecified.
	 * An empty array indicates that no items are selected.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its selection, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 * @return an array representing the selection
	 */
	bool GetSelection(WIterator<WListItem> &items) {
		return _WReturnBool(_get_selection((w_iterator*) &items));
	}
	/**
	 * Returns the number of selected items contained in the receiver.
	 *
	 * @return the number of selected items
	 */
	int GetSelectionCount() {
		return _WReturnInt(_get_selection_count());
	}
	/**
	 * Returns the zero-relative index of the item which is currently
	 * selected in the receiver, or -1 if no item is selected.
	 *
	 * @return the index of the selected item
	 */
	int GetSelectionIndex() {
		return _WReturnIntIndex(_get_selection_index());
	}
	/**
	 * Returns the zero-relative index of the item which is currently
	 * at the top of the receiver. This index can change when items are
	 * scrolled or new items are added or removed.
	 *
	 * @return the index of the top item
	 */
	int GetTopIndex() {
		return _WReturnInt(_get_top_index());
	}
	bool _InsertItem(WListItem &item, const char *text, int index) {
		WResult result = _insert_item(&item, index);
		item.SetText(text);
		return _WReturnBool(result);
	}
	WListItem& InsertItem(WListItem &item, const char *text, int index) {
		_insert_item(&item, index);
		item.SetText(text);
		return item;
	}
	/**
	 * Returns <code>true</code> if the item is selected,
	 * and <code>false</code> otherwise.  Indices out of
	 * range are ignored.
	 *
	 * @param index the index of the item
	 * @return the selection state of the item at the index
	 */
	bool IsSelected(int index) {
		return _WReturnBool(_is_selected(index));
	}
	/**
	 * Removes the items from the receiver's list at the given
	 * zero-relative indices.
	 *
	 * @param indices the array of indices of the items
	 */
	bool Remove(int *indices, size_t length) {
		return _WReturnBool(_remove_indices(indices, length));
	}
	/**
	 * Removes the item from the receiver at the given
	 * zero-relative index.
	 *
	 * @param index the index for the item
	 */
	bool Remove(int index) {
		return _WReturnBool(_remove_index(index));
	}
	/**
	 * Removes the items from the receiver which are
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
	 * Selects the items at the given zero-relative indices in the receiver.
	 * The current selection is not cleared before the new items are selected.
	 * <p>
	 * If the item at a given index is not selected, it is selected.
	 * If the item at a given index was already selected, it remains selected.
	 * Indices that are out of range and duplicate indices are ignored.
	 * If the receiver is single-select and multiple indices are specified,
	 * then all indices are ignored.
	 * </p>
	 *
	 * @param indices the array of indices for the items to select
	 *
	 * @see Table#setSelection(int[])
	 */
	bool Select(int *indices, size_t length) {
		return _WReturnBool(_select_indices(indices, length));
	}
	/**
	 * Selects the item at the given zero-relative index in the receiver.
	 * If the item at the index was already selected, it remains
	 * selected. Indices that are out of range are ignored.
	 *
	 * @param index the index of the item to select
	 */
	bool Select(int index) {
		return _WReturnBool(_select_index(index));
	}
	/**
	 * Selects the items in the range specified by the given zero-relative
	 * indices in the receiver. The range of indices is inclusive.
	 * The current selection is not cleared before the new items are selected.
	 * <p>
	 * If an item in the given range is not selected, it is selected.
	 * If an item in the given range was already selected, it remains selected.
	 * Indices that are out of range are ignored and no items will be selected
	 * if start is greater than end.
	 * If the receiver is single-select and there is more than one item in the
	 * given range, then all indices are ignored.
	 * </p>
	 *
	 * @param start the start of the range
	 * @param end the end of the range
	 *
	 * @see Table#setSelection(int,int)
	 */
	bool Select(int start, int end) {
		return _WReturnBool(_select_range(start, end));
	}
	/**
	 * Selects the items at the given zero-relative indices in the receiver.
	 * The current selection is cleared before the new items are selected.
	 * <p>
	 * Indices that are out of range and duplicate indices are ignored.
	 * If the receiver is single-select and multiple indices are specified,
	 * then all indices are ignored.
	 * </p>
	 *
	 * @param indices the indices of the items to select
	 *
	 * @see Table#deselectAll()
	 * @see Table#select(int[])
	 */
	bool SetSelection(int *indices, size_t length) {
		return _WReturnBool(_set_selection_indices(indices, length));
	}
	/**
	 * Selects the item at the given zero-relative index in the receiver.
	 * The current selection is first cleared, then the new item is selected.
	 *
	 * @param index the index of the item to select
	 *
	 * @see Table#deselectAll()
	 * @see Table#select(int)
	 */
	bool SetSelection(int index) {
		return _WReturnBool(_set_selection_index(index));
	}
	/**
	 * Selects the items in the range specified by the given zero-relative
	 * indices in the receiver. The range of indices is inclusive.
	 * The current selection is cleared before the new items are selected.
	 * <p>
	 * Indices that are out of range are ignored and no items will be selected
	 * if start is greater than end.
	 * If the receiver is single-select and there is more than one item in the
	 * given range, then all indices are ignored.
	 * </p>
	 *
	 * @param start the start index of the items to select
	 * @param end the end index of the items to select
	 *
	 * @see Table#deselectAll()
	 * @see Table#select(int,int)
	 */
	bool SetSelection(int start, int end) {
		return _WReturnBool(_set_selection_range(start, end));
	}
	/**
	 * Sets the zero-relative index of the item which is currently
	 * at the top of the receiver. This index can change when items
	 * are scrolled or new items are added and removed.
	 *
	 * @param index the index of the top item
	 */
	bool SetTopIndex(int index) {
		return _WReturnBool(_set_top_index(index));
	}
	/**
	 * Shows the item.  If the item is already showing in the receiver,
	 * this method simply returns.  Otherwise, the items are scrolled until
	 * the item is visible.
	 *
	 * @param item the item to be shown
	 *
	 * @see Table#showSelection()
	 */
	bool ShowItem(WListItem &item) {
		return _WReturnBool(_show_item(&item));
	}
public:
	WResult _clear_index(int index) {
		return w_listview_clear_index(W_LISTVIEW(this), index);
	}
	WResult _clear_indices(int *indices, int length) {
		return w_listview_clear_indices(W_LISTVIEW(this), indices, length);
	}
	WResult _clear_item(WListItem *item) {
		return w_listview_clear_item(W_LISTVIEW(this), W_LISTITEM(item));
	}
	WResult _clear_range(int start, int end) {
		return w_listview_clear_range(W_LISTVIEW(this), start, end);
	}
	WResult _deselect_index(int index) {
		return w_listview_deselect_index(W_LISTVIEW(this), index);
	}
	WResult _deselect_indices(int *indices, int length) {
		return w_listview_deselect_indices(W_LISTVIEW(this), indices, length);
	}
	WResult _deselect_item(WListItem *item) {
		return w_listview_deselect_item(W_LISTVIEW(this), W_LISTITEM(item));
	}
	WResult _deselect_range(int start, int end) {
		return w_listview_deselect_range(W_LISTVIEW(this), start, end);
	}
	WResult _get_item(int index, WListItem *item) {
		return w_listview_get_item(W_LISTVIEW(this), index, W_LISTITEM(item));
	}
	WResult _get_item_count() {
		return w_listview_get_item_count(W_LISTVIEW(this));
	}
	WResult _get_item_from_point(const WPoint *point, WListItem *item) {
		return w_listview_get_item_from_point(W_LISTVIEW(this),
				(w_point*) point, W_LISTITEM(item));
	}
	WResult _get_items(WIterator<WListItem> *items) {
		return w_listview_get_items(W_LISTVIEW(this), (w_iterator*) items);
	}
	WResult _get_selection_index() {
		return w_listview_get_selection_index(W_LISTVIEW(this));
	}
	WResult _get_top_index() {
		return w_listview_get_top_index(W_LISTVIEW(this));
	}
	WResult _get_top_item(WListItem *topitem) {
		return w_listview_get_top_item(W_LISTVIEW(this), W_LISTITEM(topitem));
	}
	WResult _is_selected(int index) {
		return w_listview_is_selected(W_LISTVIEW(this), index);
	}
	WResult _insert_item(WListItem *item, int index) {
		return w_listview_insert_item(W_LISTVIEW(this), W_LISTITEM(item), index);
	}
	WResult _remove_index(int index) {
		return w_listview_remove_index(W_LISTVIEW(this), index);
	}
	WResult _remove_indices(int *indices, int length) {
		return w_listview_remove_indices(W_LISTVIEW(this), indices, length);
	}
	WResult _remove_item(WListItem *item) {
		return w_listview_remove_item(W_LISTVIEW(this), W_LISTITEM(item));
	}
	WResult _remove_range(int start, int end) {
		return w_listview_remove_range(W_LISTVIEW(this), start, end);
	}
	WResult _select_index(int index) {
		return w_listview_select_index(W_LISTVIEW(this), index);
	}
	WResult _select_indices(int *indices, int length) {
		return w_listview_select_indices(W_LISTVIEW(this), indices, length);
	}
	WResult _select_item(WListItem *item) {
		return w_listview_select_item(W_LISTVIEW(this), W_LISTITEM(item));
	}
	WResult _select_range(int start, int end) {
		return w_listview_select_range(W_LISTVIEW(this), start, end);
	}
	WResult _set_insert_mark(WListItem *item, int before) {
		return w_listview_set_insert_mark(W_LISTVIEW(this), W_LISTITEM(item),
				before);
	}
	WResult _set_selection_index(int index) {
		return w_listview_set_selection_index(W_LISTVIEW(this), index);
	}
	WResult _set_selection_indices(int *indices, int length) {
		return w_listview_set_selection_indices(W_LISTVIEW(this), indices,
				length);
	}
	WResult _set_selection_item(WListItem *item) {
		return w_listview_set_selection_item(W_LISTVIEW(this), W_LISTITEM(item));
	}
	WResult _set_selection_range(int start, int end) {
		return w_listview_set_selection_range(W_LISTVIEW(this), start, end);
	}
	WResult _set_top_index(int index) {
		return w_listview_set_top_index(W_LISTVIEW(this), index);
	}
	WResult _set_top_item(WListItem *item) {
		return w_listview_set_top_item(W_LISTVIEW(this), W_LISTITEM(item));
	}
	WResult _show_item(WListItem *item) {
		return w_listview_show_item(W_LISTVIEW(this), W_LISTITEM(item));
	}
private:
	//void *handles[(sizeof(w_listview) - sizeof(w_listviewbase)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_TABLE_H_ */
