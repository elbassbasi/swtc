/*
 * ToolBar.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_TOOLBAR_H_
#define SWTP_CONTROLS_TOOLBAR_H_
#include "../widgets/Composite.h"
class WToolBar;
/**
 * Instances of this class represent a selectable user interface object
 * that represents a button in a tool bar.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>PUSH, CHECK, RADIO, SEPARATOR, DROP_DOWN</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles CHECK, PUSH, RADIO, SEPARATOR and DROP_DOWN
 * may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WToolItem: public WItem {
public:
	WToolItem() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>ToolBar</code>) and a style value
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
	 * @see SWT#PUSH
	 * @see SWT#CHECK
	 * @see SWT#RADIO
	 * @see SWT#SEPARATOR
	 * @see SWT#DROP_DOWN
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WToolItem(WToolBar *parent, int style) {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>ToolBar</code>), a style value
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
	 * @see SWT#PUSH
	 * @see SWT#CHECK
	 * @see SWT#RADIO
	 * @see SWT#SEPARATOR
	 * @see SWT#DROP_DOWN
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WToolItem(WToolBar *parent, int style, int index) {

	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent.
	 *
	 * @return the receiver's bounding rectangle
	 */
	WRect& GetBounds(WRect &rect) {
		w_toolitem_get_bounds(W_TOOLITEM(this), (w_rect*) &rect);
		return rect;
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent.
	 *
	 * @return the receiver's bounding rectangle
	 */
	WRect GetBounds() {
		WRect rect;
		return GetBounds(rect);
	}
	/**
	 * Returns the control that is used to fill the bounds of
	 * the item when the item is a <code>SEPARATOR</code>.
	 *
	 * @return the control
	 */
	WControl* GetControl() {
		WControl *control;
		w_toolitem_get_control(W_TOOLITEM(this), (w_control**) &control);
		return control;
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
		return w_toolitem_get_enabled(W_TOOLITEM(this));
	}
	wushort GetID() {
		return w_toolitem_get_id(W_TOOLITEM(this));
	}
	int GetImage() {
		return w_toolitem_get_image(W_TOOLITEM(this));
	}
	WMenu* GetMenu() {
		WMenu *menu;
		w_toolitem_get_menu(W_TOOLITEM(this), (w_menu**) &menu);
		return menu;
	}
	/**
	 * Returns the receiver's parent, which must be a <code>ToolBar</code>.
	 *
	 * @return the receiver's parent
	 */
	WToolBar* GetParent() {
		return (WToolBar*) WItem::GetParentWidget();
	}
	/**
	 * Returns <code>true</code> if the receiver is selected,
	 * and false otherwise.
	 * <p>
	 * When the receiver is of type <code>CHECK</code> or <code>RADIO</code>,
	 * it is selected when it is checked (which some platforms draw as a
	 * pushed in button). If the receiver is of any other type, this method
	 * returns false.
	 * </p>
	 *
	 * @return the selection state
	 */
	bool GetSelection() {
		return w_toolitem_get_selection(W_TOOLITEM(this));
	}
	wuint64 GetStyle() {
		return w_toolitem_get_style(W_TOOLITEM(this));
	}
	/**
	 * Returns the receiver's tool tip text, or null if it has not been set.
	 *
	 * @return the receiver's tool tip text
	 */
	WString GetToolTipText() {
		w_string_ref *ref = 0;
		w_toolitem_get_tooltip_text(W_TOOLITEM(this), w_alloc_string_ref, &ref,
				W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Gets the width of the receiver.
	 *
	 * @return the width
	 */
	int GetWidth() {
		return w_toolitem_get_width(W_TOOLITEM(this));
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
		return w_toolitem_is_enabled(W_TOOLITEM(this));
	}
	/**
	 * Sets the control that is used to fill the bounds of
	 * the item when the item is a <code>SEPARATOR</code>.
	 *
	 * @param control the new control
	 */
	bool SetControl(WControl *control) {
		return w_toolitem_set_control(W_TOOLITEM(this), W_CONTROL(control)) > 0;
	}
	bool SetControl(WControl &control) {
		return SetControl(&control);
	}
	bool SetControlWithPreferredWith(WControl *control) {
		WSize size;
		control->ComputeSize(size, W_DEFAULT, W_DEFAULT);
		SetWidth(size.width);
		return SetControl(control);
	}
	bool SetControlWithPreferredWith(WControl &control) {
		return SetControlWithPreferredWith(&control);
	}
	/**
	 * Enables the receiver if the argument is <code>true</code>,
	 * and disables it otherwise.
	 * <p>
	 * A disabled control is typically
	 * not selectable from the user interface and draws with an
	 * inactive or "grayed" look.
	 * </p>
	 *
	 * @param enabled the new enabled state
	 */
	bool SetEnabled(bool enabled) {
		return w_toolitem_set_enabled(W_TOOLITEM(this), enabled) > 0;
	}
	bool SetID(wushort id) {
		return w_toolitem_set_id(W_TOOLITEM(this), id) > 0;
	}
	bool SetImage(int image) {
		return w_toolitem_set_image(W_TOOLITEM(this), image) > 0;
	}
	bool SetMenu(WMenu *menu) {
		return w_toolitem_set_menu(W_TOOLITEM(this), W_MENU(menu)) > 0;
	}
	bool SetMenu(WMenu &menu) {
		return SetMenu(&menu);
	}
	/**
	 * Sets the selection state of the receiver.
	 * <p>
	 * When the receiver is of type <code>CHECK</code> or <code>RADIO</code>,
	 * it is selected when it is checked (which some platforms draw as a
	 * pushed in button).
	 * </p>
	 *
	 * @param selected the new selection state
	 */
	bool SetSelection(bool selected) {
		return w_toolitem_set_selection(W_TOOLITEM(this), selected) > 0;
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
		return w_toolitem_set_tooltip_text(W_TOOLITEM(this), string, -1,
				W_ENCODING_UTF8) > 0;
	}
	/**
	 * Sets the width of the receiver, for <code>SEPARATOR</code> ToolItems.
	 *
	 * @param width the new width. If the new value is <code>SWT.DEFAULT</code>,
	 * the width is a fixed-width area whose amount is determined by the platform.
	 * If the new value is 0 a vertical or horizontal line will be drawn, depending
	 * on the setting of the corresponding style bit (<code>SWT.VERTICAL</code> or
	 * <code>SWT.HORIZONTAL</code>). If the new value is <code>SWT.SEPARATOR_FILL</code>
	 * a variable-width space is inserted that acts as a spring between the two adjoining
	 * items which will push them out to the extent of the containing ToolBar.
	 */
	bool SetWidth(int width) {
		return w_toolitem_set_width(W_TOOLITEM(this), width) > 0;
	}
private:
	void *handles[sizeof(w_toolitem) / sizeof(void*)];
};
class WToolBarEvent: public WEvent {
public:
	union {
		int detail;
		struct {
			unsigned doit :1;
		};
	};
	WPoint location;
	WToolItem *item;
	union {
		WMenu *menu;
		WControl *control;
	};
};
/**
 * Instances of this class support the layout of selectable
 * tool bar items.
 * <p>
 * The item children that may be added to instances of this class
 * must be of type <code>ToolItem</code>.
 * </p><p>
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not make sense to add <code>Control</code> children to it,
 * or set a layout on it.
 * </p><p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>FLAT, WRAP, RIGHT, HORIZONTAL, VERTICAL, SHADOW_OUT</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles HORIZONTAL and VERTICAL may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WToolBar: public WComposite {
public:
	using WControl::Create;
	WToolBar() {

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
	 * @see SWT#FLAT
	 * @see SWT#WRAP
	 * @see SWT#RIGHT
	 * @see SWT#HORIZONTAL
	 * @see SWT#SHADOW_OUT
	 * @see SWT#VERTICAL
	 * @see Widget#checkSubclass()
	 * @see Widget#getStyle()
	 */
	WToolBar(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	WResult Create(WToolkit *toolkit, WComposite *parent, wuint64 style);
	WImageList* GetImageList() {
		WImageList *imagelist;
		w_toolbar_get_imagelist(W_TOOLBAR(this), (w_imagelist**) &imagelist);
		return imagelist;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WToolItem& GetItem(int index, WToolItem &item) {
		w_toolbar_get_item(W_TOOLBAR(this), index, W_TOOLITEM(&item));
		return item;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WToolItem& GetItem(int index, const WToolItem &item = WToolItem()) {
		return GetItem(index, (WToolItem&) item);
	}
	/**
	 * Returns the item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 *
	 * @param point the point used to locate the item
	 * @return the item at the given point
	 */
	WToolItem& GetItem(const WPoint &point, WToolItem &item) {
		w_toolbar_get_item_from_point(W_TOOLBAR(this), (w_point*) &point,
				W_TOOLITEM(&item));
		return item;
	}
	/**
	 * Returns the item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 *
	 * @param point the point used to locate the item
	 * @return the item at the given point
	 */
	WToolItem& GetItem(const WPoint &point, const WToolItem &item =
			WToolItem()) {
		return GetItem(point, (WToolItem&) item);
	}
	/**
	 * Returns the number of items contained in the receiver.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return w_toolbar_get_item_count(W_TOOLBAR(this));
	}
	/**
	 * Returns an array of <code>ToolItem</code>s which are the items
	 * in the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 */
	void GetItems(WIterator<WToolItem> &items) {
		w_toolbar_get_items(W_TOOLBAR(this), (w_iterator*) &items);
	}
	/**
	 * Returns the number of rows in the receiver. When
	 * the receiver has the <code>WRAP</code> style, the
	 * number of rows can be greater than one.  Otherwise,
	 * the number of rows is always one.
	 *
	 * @return the number of items
	 */
	int GetRowCount() {
		return w_toolbar_get_row_count(W_TOOLBAR(this));
	}
	static bool IsToolBar(WWidget *widget) {
		return widget->GetClassId() == _W_CLASS_TOOLBAR;
	}
	static WToolBar* ToToolBar(WWidget *widget) {
		if (IsComposite(widget))
			return (WToolBar*) widget;
		else
			return 0;
	}
public:
	WToolItem& Insert(WToolItem &item, int style, int index) {
		w_toolbar_insert_item(W_TOOLBAR(this), W_TOOLITEM(&item), style, index);
		return item;
	}
	WToolItem& Insert(WToolItem &item, const char *text, int style, int index) {
		Insert(item, style, index);
		item.SetText(text);
		return item;
	}
	WToolItem& InsertPush(WToolItem &item, const char *text, int index) {
		return Insert(item, text, W_PUSH, index);
	}
	WToolItem& InsertCheck(WToolItem &item, const char *text, int index) {
		return Insert(item, text, W_CHECK, index);
	}
	WToolItem& InsertRadion(WToolItem &item, const char *text, int index) {
		return Insert(item, text, W_RADIO, index);
	}
	WToolItem& InsertDropDown(WToolItem &item, const char *text, int index) {
		return Insert(item, text, W_DROP_DOWN, index);
	}
	/*
	 * insert with text null
	 */
	WToolItem& InsertPush(WToolItem &item, int index) {
		return Insert(item, W_PUSH, index);
	}
	WToolItem& InsertCheck(WToolItem &item, int index) {
		return Insert(item, W_CHECK, index);
	}
	WToolItem& InsertRadion(WToolItem &item, int index) {
		return Insert(item, W_RADIO, index);
	}
	WToolItem& InsertDropDown(WToolItem &item, int index) {
		return Insert(item, W_DROP_DOWN, index);
	}
	WToolItem& InsertSeparator(WToolItem &item, int index) {
		return Insert(item, W_SEPARATOR, index);
	}
	/*
	 * append
	 */
	WToolItem& Append(WToolItem &item, const char *text, int style) {
		return Insert(item, text, style, -1);
	}
	WToolItem& AppendPush(WToolItem &item, const char *text) {
		return Append(item, text, W_PUSH);
	}
	WToolItem& AppendCheck(WToolItem &item, const char *text) {
		return Append(item, text, W_CHECK);
	}
	WToolItem& AppendRadion(WToolItem &item, const char *text) {
		return Append(item, text, W_RADIO);
	}
	WToolItem& AppendDropDown(WToolItem &item, const char *text) {
		return Append(item, text, W_DROP_DOWN);
	}
	/*
	 * append
	 */
	WToolItem& Append(WToolItem &item, int style) {
		return Insert(item, style, -1);
	}
	WToolItem& AppendPush(WToolItem &item) {
		return Append(item, W_PUSH);
	}
	WToolItem& AppendCheck(WToolItem &item) {
		return Append(item, W_CHECK);
	}
	WToolItem& AppendRadion(WToolItem &item) {
		return Append(item, W_RADIO);
	}
	WToolItem& AppendDropDown(WToolItem &item) {
		return Append(item, W_DROP_DOWN);
	}
	WToolItem& AppendSeparator(WToolItem &item) {
		return Append(item, W_SEPARATOR);
	}
public:
	bool SetImageList(WImageList *imagelist) {
		return w_toolbar_set_imagelist(W_TOOLBAR(this),
				(w_imagelist*) imagelist) > 0;
	}
protected:
	w_class_id _GetClassID();
	virtual bool PostEvent(WEvent *e);
	virtual bool OnItemSelection(WToolBarEvent &e);
	virtual bool OnItemGetMenu(WToolBarEvent &e);
	virtual bool OnItemSetMenu(WToolBarEvent &e);
	virtual bool OnItemGetControl(WToolBarEvent &e);
	virtual bool OnItemSetControl(WToolBarEvent &e);
private:
	void *handles[(sizeof(w_toolbar) - sizeof(w_composite)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_TOOLBAR_H_ */
