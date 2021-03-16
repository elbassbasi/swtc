/*
 * Menu.h
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_MENU_H_
#define SWTP_WIDGETS_MENU_H_
#include "Control.h"
/**
 * Instances of this class represent a selectable user interface object
 * that issues notification when pressed and released.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>CHECK, CASCADE, PUSH, RADIO, SEPARATOR</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Arm, Help, Selection</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles CHECK, CASCADE, PUSH, RADIO and SEPARATOR
 * may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WMenuItem: public WItem {
public:
	WMenuItem() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>Menu</code>) and a style value
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
	 * @param parent a menu control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 *
	 * @see SWT#CHECK
	 * @see SWT#CASCADE
	 * @see SWT#PUSH
	 * @see SWT#RADIO
	 * @see SWT#SEPARATOR
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WMenuItem& Create(WMenuItem *parent, const char *text, int style,
			int index) {
		return parent->Insert(*this, text, style, index);
	}
	/**
	 * Returns the widget accelerator.  An accelerator is the bit-wise
	 * OR of zero or more modifier masks and a key. Examples:
	 * <code>SWT.CONTROL | SWT.SHIFT | 'T', SWT.ALT | SWT.F2</code>.
	 * The default value is zero, indicating that the menu item does
	 * not have an accelerator.
	 *
	 * @return the accelerator or 0
	 */
	int GetAccelerator() {
		return _WReturnInt(_get_accelerator());
	}
	/**
	 * Returns <code>true</code> if the receiver is enabled, and
	 * <code>false</code> otherwise. A disabled menu item is typically
	 * not selectable from the user interface and draws with an
	 * inactive or "grayed" look.
	 *
	 * @return the receiver's enabled state
	 *
	 * @see #isEnabled
	 */
	bool GetEnabled() {
		return _WReturnBool(_get_enabled());
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WMenuItem& GetItem(wint index, WMenuItem &item) {
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
	WMenuItem GetItem(int index) {
		WMenuItem item;
		return GetItem(index, item);
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
	 * Returns a (possibly empty) array of <code>MenuItem</code>s which
	 * are the items in the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 */
	bool GetItems(WIterator<WMenuItem> &items) {
		return _WReturnBool(_get_items(items));
	}
	/**
	 * Gets the identifier associated with the receiver.
	 *
	 * @return the receiver's identifier
	 */
	wushort GetId() {
		return _WReturnInt(_get_id());
	}
	/**
	 * Returns the receiver's image if it has one, or null
	 * if it does not.
	 *
	 * @return the receiver's image
	 */
	bool GetImage(WImage &image) {
		return _WReturnBool(_get_image(&image));

	}
	/**
	 * Returns the receiver's cascade menu if it has one or null
	 * if it does not. Only <code>CASCADE</code> menu items can have
	 * a pull down menu. The sequence of key strokes, button presses
	 * and/or button releases that are used to request a pull down
	 * menu is platform specific.
	 *
	 * @return the receiver's menu
	 */
	WMenu* GetMenu() {
		return (WMenu*) GetParentWidget();
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Menu</code>.
	 *
	 * @return the receiver's parent
	 */
	bool GetParent(WMenuItem &parent) {
		return _WReturnBool(_get_parent_item(&parent));
	}
	/**
	 * Returns the receiver's style information.
	 * <p>
	 * Note that the value which is returned by this method <em>may
	 * not match</em> the value which was provided to the constructor
	 * when the receiver was created. This can occur when the underlying
	 * operating system does not support a particular combination of
	 * requested styles. For example, if the platform widget used to
	 * implement a particular SWT widget always has scroll bars, the
	 * result of calling this method would always have the
	 * <code>SWT.H_SCROLL</code> and <code>SWT.V_SCROLL</code> bits set.
	 * </p>
	 *
	 * @return the style bits
	 */
	int GetStyle() {
		return _WReturnInt(_get_style());
	}
	/**
	 * Returns <code>true</code> if the receiver is selected,
	 * and false otherwise.
	 * <p>
	 * When the receiver is of type <code>CHECK</code> or <code>RADIO</code>,
	 * it is selected when it is checked.
	 *
	 * @return the selection state
	 */
	bool GetSelection() {
		return _WReturnBool(_get_selection());
	}
	/**
	 * Returns <code>true</code> if the receiver is selected,
	 * and false otherwise.
	 * <p>
	 * When the receiver is of type <code>CHECK</code> or <code>RADIO</code>,
	 * it is selected when it is checked.
	 *
	 * @return the selection state
	 */
	bool IsChecked() {
		return GetSelection();
	}
	/**
	 * Returns <code>true</code> if the receiver is enabled and all
	 * of the receiver's ancestors are enabled, and <code>false</code>
	 * otherwise. A disabled menu item is typically not selectable from the
	 * user interface and draws with an inactive or "grayed" look.
	 *
	 * @return the receiver's enabled state
	 *
	 * @see #getEnabled
	 */
	bool IsEnabled() {
		return _WReturnBool(_is_enabled());
	}
	bool IsCascade() {
		return GetStyle() & W_CASCADE;
	}
	bool IsCheck() {
		return GetStyle() & W_CHECK;
	}
	bool IsRadio() {
		return GetStyle() & W_RADIO;
	}
	bool IsSeparator() {
		return GetStyle() & W_SEPARATOR;
	}
	bool IsPush() {
		return GetStyle() & W_PUSH;
	}
	/**
	 * remove and destroy the menu item from the menu.
	 */
	bool Remove() {
		return _WReturnBool(_remove());
	}
	/**
	 * remove and destroy the menu item from the this menu.
	 */
	bool RemoveItem(wint index) {
		return _WReturnBool(_remove_item(index));
	}
	/**
	 * Sets the widget accelerator.  An accelerator is the bit-wise
	 * OR of zero or more modifier masks and a key. Examples:
	 * <code>SWT.MOD1 | SWT.MOD2 | 'T', SWT.MOD3 | SWT.F2</code>.
	 * <code>SWT.CONTROL | SWT.SHIFT | 'T', SWT.ALT | SWT.F2</code>.
	 * The default value is zero, indicating that the menu item does
	 * not have an accelerator.
	 *
	 * @param accelerator an integer that is the bit-wise OR of masks and a key
	 */
	WMenuItem& SetAccelerator(wint accelerator) {
		_set_accelerator(accelerator);
		return *this;
	}
	/**
	 * Sets the selection state of the receiver.
	 * <p>
	 * When the receiver is of type <code>CHECK</code> or <code>RADIO</code>,
	 * it is selected when it is checked.
	 *
	 * @param selected the new selection state
	 */
	WMenuItem& SetCheck(bool check) {
		return SetSelection(check);
	}
	/**
	 * Enables the receiver if the argument is <code>true</code>,
	 * and disables it otherwise. A disabled menu item is typically
	 * not selectable from the user interface and draws with an
	 * inactive or "grayed" look.
	 *
	 * @param enabled the new enabled state
	 */
	WMenuItem& SetEnabled(bool enabled) {
		_set_enabled(enabled);
		return *this;
	}
	/**
	 * Sets the identifier associated with the receiver to the argument.
	 *
	 * @param id the new identifier. This must be a non-negative value. System-defined identifiers are negative values.
	 */
	WMenuItem& SetId(wushort id) {
		_set_id(id);
		return *this;
	}
	/**
	 * Sets the receiver's image to the argument, which may be
	 * null indicating that no image should be displayed.
	 * <p>
	 * Note: This operation is a <em>HINT</em> and is not supported on
	 * platforms that do not have this concept (for example, Windows NT).
	 * Furthermore, some platforms (such as GTK2), cannot display both
	 * a check box and an image at the same time.  Instead, they hide
	 * the image and display the check box. Some platforms (such as GTK3)
	 * support images alongside check boxes.
	 * </p>
	 *
	 * @param image the image to display on the receiver (may be null)
	 */
	WMenuItem& SetImage(const WImage &image) {
		_set_image(&image);
		return *this;
	}
	WMenuItem& SetImageIndex(WImageList *imagelist, int image) {
		_set_image_index(imagelist, image);
		return *this;
	}
	/**
	 * Sets the selection state of the receiver.
	 * <p>
	 * When the receiver is of type <code>CHECK</code> or <code>RADIO</code>,
	 * it is selected when it is checked.
	 *
	 * @param selected the new selection state
	 */
	WMenuItem& SetSelection(bool selected) {
		_set_selection(selected);
		return *this;
	}
public:
#if __cplusplus >= 201103L
	void SetSelectionFunction(const WSelectionItemFunction &function) {
		WItem::SetSelectionFunction(function);
	}
	void operator <<=(const WSelectionItemFunction &function) {
		WItem::SetSelectionFunction(function);
	}
#endif
	IWListener* GetListener() {
		return WItem::GetListener();
	}
	void SetListener(IWListener *listener) {
		WItem::SetListener(listener);
	}
public:
	WMenuItem& Insert(WMenuItem &item, int style, int index) {
		_insert(&item, style, index);
		return item;
	}
	WMenuItem& Insert(WMenuItem &item, const char *text, int style, int index) {
		Insert(item, style, index);
		item.SetText(text);
		return item;
	}
	WMenuItem Insert(const char *text, int style, int index) {
		WMenuItem item;
		return Insert(item, text, style, index);
	}
	WMenuItem InsertItem(const char *text, int index) {
		WMenuItem item;
		return Insert(item, text, W_PUSH, index);
	}
	WMenuItem& InsertItem(WMenuItem &item, const char *text, int index) {
		return Insert(item, text, W_PUSH, index);
	}
	WMenuItem InsertCheckItem(const char *text, int index) {
		WMenuItem item;
		return Insert(item, text, W_CHECK, index);
	}
	WMenuItem& InsertCheckItem(WMenuItem &item, const char *text, int index) {
		return Insert(item, text, W_CHECK, index);
	}
	WMenuItem InsertRadioItem(const char *text, int index) {
		WMenuItem item;
		return Insert(item, text, W_RADIO, index);
	}
	WMenuItem& InsertRadioItem(WMenuItem &item, const char *text, int index) {
		return Insert(item, text, W_RADIO, index);
	}
	WMenuItem InsertSeparator(int index) {
		WMenuItem item;
		return Insert(item, 0, W_SEPARATOR, index);
	}
	WMenuItem InsertSubMenu(const char *text, int index) {
		WMenuItem item;
		return Insert(item, text, W_CASCADE, index);
	}
	WMenuItem& InsertSubMenu(WMenuItem &item, const char *text, int index) {
		return Insert(item, text, W_CASCADE, index);
	}
public:
	WMenuItem& Append(WMenuItem &item, const char *text, int style) {
		return Insert(item, text, style, -1);
	}
	WMenuItem Append(const char *text, int style) {
		WMenuItem item;
		return Insert(item, text, style, -1);
	}
	WMenuItem AppendItem(const char *text) {
		WMenuItem item;
		return Append(item, text, W_PUSH);
	}
	WMenuItem& AppendItem(WMenuItem &item, const char *text) {
		return Append(item, text, W_PUSH);
	}
	WMenuItem AppendCheckItem(const char *text) {
		WMenuItem item;
		return Append(item, text, W_CHECK);
	}
	WMenuItem& AppendCheckItem(WMenuItem &item, const char *text) {
		return Append(item, text, W_CHECK);
	}
	WMenuItem AppendRadioItem(const char *text) {
		WMenuItem item;
		return Append(item, text, W_RADIO);
	}
	WMenuItem& AppendRadioItem(WMenuItem &item, const char *text) {
		return Append(item, text, W_RADIO);
	}
	WMenuItem AppendSeparator() {
		WMenuItem item;
		return Append(item, 0, W_SEPARATOR);
	}
	WMenuItem AppendSubMenu(const char *text) {
		WMenuItem item;
		return Append(item, text, W_CASCADE);
	}
	WMenuItem& AppendSubMenu(WMenuItem &item, const char *text) {
		return Append(item, text, W_CASCADE);
	}
public:
	WResult _insert(WMenuItem *item, int style, int index) {
		return w_menuitem_insert(W_MENUITEM(this), W_MENUITEM(item), style,
				index);
	}
	WResult _get_accelerator() {
		return w_menuitem_get_accelerator(W_MENUITEM(this));
	}
	WResult _get_enabled() {
		return w_menuitem_get_enabled(W_MENUITEM(this));
	}
	WResult _get_item(wuint index, WMenuItem *item) {
		return w_menuitem_get_item(W_MENUITEM(this), index, W_MENUITEM(item));
	}
	WResult _get_item_count() {
		return w_menuitem_get_item_count(W_MENUITEM(this));
	}
	WResult _get_items(WIterator<WMenuItem> &items) {
		return w_menuitem_get_items(W_MENUITEM(this), (w_iterator*) &items);
	}
	WResult _get_id() {
		return w_menuitem_get_id(W_MENUITEM(this));
	}
	WResult _get_image(WImage *image) {
		return w_menuitem_get_image(W_MENUITEM(this), W_IMAGE(image));
	}
	WResult _get_parent_item(WMenuItem *parent) {
		return w_menuitem_get_parent_item(W_MENUITEM(this), W_MENUITEM(parent));
	}
	WResult _get_selection() {
		return w_menuitem_get_selection(W_MENUITEM(this));
	}
	WResult _get_style() {
		return w_menuitem_get_style(W_MENUITEM(this));
	}
	WResult _is_enabled() {
		return w_menuitem_is_enabled(W_MENUITEM(this));
	}
	WResult _remove() {
		return w_menuitem_remove(W_MENUITEM(this));
	}
	WResult _remove_item(wuint index) {
		return w_menuitem_remove_item(W_MENUITEM(this), index);
	}
	WResult _set_accelerator(wuint accelerator) {
		return w_menuitem_set_accelerator(W_MENUITEM(this), accelerator);
	}
	WResult _set_enabled(int enabled) {
		return w_menuitem_set_enabled(W_MENUITEM(this), enabled);
	}
	WResult _set_id(wushort id) {
		return w_menuitem_set_id(W_MENUITEM(this), id);
	}
	WResult _set_image(const WImage *image) {
		return w_menuitem_set_image(W_MENUITEM(this), W_IMAGE(image));
	}
	WResult _set_image_index(WImageList *imagelist, int index) {
		return w_menuitem_set_image_index(W_MENUITEM(this),
				W_IMAGELIST(imagelist), index);
	}
	WResult _set_selection(int selected) {
		return w_menuitem_set_selection(W_MENUITEM(this), selected);
	}
private:
	char handles[sizeof(w_menuitem) - sizeof(WWidgetData)];
};
class SWTP_PUBLIC WMenuEvent: public WEvent {
public:
	WMenuItem *item;
	WMenuItem* operator->() {
		return this->item;
	}
};
struct WMenuItems;
/**
 * Instances of this class are user interface objects that contain
 * menu items.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>W_BAR, W_DROP_DOWN, W_POP_UP, W_NO_RADIO_GROUP</dd>
 * <dd>LEFT_TO_RIGHT, RIGHT_TO_LEFT</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Help, Hide, Show </dd>
 * </dl>
 * <p>
 * Note: Only one of W_BAR, W_DROP_DOWN and W_POP_UP may be specified.
 * Only one of W_LEFT_TO_RIGHT or W_RIGHT_TO_LEFT may be specified.
 * </p>
 */
class SWTP_PUBLIC WMenuBase: public WWidget {
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnHelp(WEvent &e)=0;
	virtual bool OnHide(WEvent &e)=0;
	virtual bool OnShow(WEvent &e)=0;
	virtual bool OnItemArm(WMenuEvent &e)=0;
	virtual bool OnItemHelp(WMenuEvent &e)=0;
	virtual bool OnItemSelection(WMenuEvent &e)=0;
	virtual bool OnItemDispose(WMenuEvent &e)=0;
	virtual bool OnItemAdded(WMenuEvent &e)=0;
public:
	WMenuBase() {
	}
	/**
	 * Constructs a new instance of this class given its parent,
	 * and sets the style for the instance so that the instance
	 * will be a popup menu on the given parent's shell.
	 * <p>
	 * After constructing a menu, it can be set into its parent
	 * using <code>parent.setMenu(menu)</code>.  In this case, the parent may
	 * be any control in the same widget tree as the parent.
	 * </p>
	 *
	 * @param parent a control which will be the parent of the new instance (cannot be null)
	 *
	 * @see SWT#POP_UP
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WControl *parent, int style) {
		return _WReturnBool(_create(parent, style, WWidget::post_event_proc));
	}
	/**
	 * Constructs a new instance of this class given its parent,
	 * and sets the style for the instance so that the instance
	 * will be a popup menu on the given parent's shell.
	 * <p>
	 * After constructing a menu, it can be set into its parent
	 * using <code>parent.setMenu(menu)</code>.  In this case, the parent may
	 * be any control in the same widget tree as the parent.
	 * </p>
	 *
	 * @param parent a control which will be the parent of the new instance (cannot be null)
	 *
	 * @see SWT#POP_UP
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool CreatePopUp(WControl *parent) {
		return Create(parent, W_POP_UP);
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>Decorations</code>) and a style value
	 * describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p><p>
	 * After constructing a menu or menuBar, it can be set into its parent
	 * using <code>parent.setMenu(menu)</code> or <code>parent.setMenuBar(menuBar)</code>.
	 * </p>
	 *
	 * @param parent a decorations control which will be the parent of the new instance (cannot be null)
	 * @param style the style of menu to construct
	 *
	 *
	 * @see SWT#BAR
	 * @see SWT#DROP_DOWN
	 * @see SWT#POP_UP
	 * @see SWT#NO_RADIO_GROUP
	 * @see SWT#LEFT_TO_RIGHT
	 * @see SWT#RIGHT_TO_LEFT
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(class WShell *parent) {
		return Create((WControl*) parent, W_BAR);
	}
	bool CreateBar(class WShell *parent) {
		return Create((WControl*) parent, W_BAR);
	}
	WRect& GetBounds(WRect &bounds) {
		_get_bounds(&bounds);
		return bounds;
	}
	WRect GetBounds() {
		WRect bounds;
		return GetBounds(bounds);
	}
	WImageList* GetImagelist() {
		WImageList *imagelist;
		_get_imagelist(&imagelist);
		return imagelist;
	}
	/**
	 * Returns the root item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WMenuItem& GetRoot(WMenuItem &rootitem) {
		w_menu_get_root(W_MENU(this), W_MENUITEM(&rootitem));
		return rootitem;
	}
	/**
	 * Returns the root item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WMenuItem GetRoot() {
		WMenuItem rootitem;
		GetRoot(rootitem);
		return rootitem;
	}
	/**
	 * Returns the orientation of the receiver, which will be one of the
	 * constants <code>SWT.LEFT_TO_RIGHT</code> or <code>SWT.RIGHT_TO_LEFT</code>.
	 *
	 * @return the orientation style
	 */
	int GetOrientation() {
		return _WReturnInt(_get_orientation());
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Decorations</code>.
	 *
	 * @return the receiver's parent
	 */
	WControl* GetParent() {
		WControl *control;
		_get_parent(&control);
		return (WControl*) control;
	}
	WShell* GetShell() {
		WShell *shell;
		_get_shell(&shell);
		return shell;
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
		return _WReturnBool(_get_visible());
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
		return _WReturnBool(_is_visible());
	}
	/**
	 * Sets the location of the receiver, which must be a popup,
	 * to the point specified by the arguments which are relative
	 * to the display.
	 * <p>
	 * Note that this is different from most widgets where the
	 * location of the widget is relative to the parent.
	 * </p><p>
	 * Note that the platform window manager ultimately has control
	 * over the location of popup menus.
	 * </p>
	 *
	 * @param x the new x coordinate for the receiver
	 * @param y the new y coordinate for the receiver
	 */
	bool SetLocation(int x, int y) {
		return SetLocation(WPoint(x, y));
	}
	/**
	 * Sets the location of the receiver, which must be a popup,
	 * to the point specified by the argument which is relative
	 * to the display.
	 * <p>
	 * Note that this is different from most widgets where the
	 * location of the widget is relative to the parent.
	 * </p><p>
	 * Note that the platform window manager ultimately has control
	 * over the location of popup menus.
	 * </p>
	 *
	 * @param location the new location for the receiver
	 */
	bool SetLocation(const WPoint &location) {
		return _WReturnBool(_set_location(&location));
	}
	/**
	 * Sets the orientation of the receiver, which must be one
	 * of the constants <code>SWT.LEFT_TO_RIGHT</code> or <code>SWT.RIGHT_TO_LEFT</code>.
	 * <p>
	 *
	 * @param orientation new orientation style
	 */
	bool SetOrientation(int orientation) {
		return _WReturnBool(_set_orientation(orientation));
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
		return _WReturnBool(_set_visible(visible));
	}
public:
	wresult _create(WControl *parent, wuint64 style,
			w_widget_post_event_proc post_event) {
		return w_menu_create(W_MENU(this), (w_control*) parent, style,
				post_event);
	}
	WResult _get_bounds(WRect *bounds) {
		return w_menu_get_bounds(W_MENU(this), (w_rect*) bounds);
	}
	WResult _get_imagelist(WImageList **imagelist) {
		return w_menu_get_imagelist(W_MENU(this), (w_imagelist**) imagelist);
	}
	WResult _get_root(WMenuItem *rootitem) {
		return w_menu_get_root(W_MENU(this), (w_menuitem*) rootitem);
	}
	WResult _get_orientation() {
		return w_menu_get_orientation(W_MENU(this));
	}
	WResult _get_parent(WControl **parent) {
		return w_menu_get_parent(W_MENU(this), (w_control**) parent);
	}
	WResult _get_shell(WShell **shell) {
		return w_menu_get_shell(W_MENU(this), (w_shell**) shell);
	}
	WResult _get_visible() {
		return w_menu_get_visible(W_MENU(this));
	}
	WResult _is_visible() {
		return w_menu_is_visible(W_MENU(this));
	}
	WResult _set_location(const WPoint *location) {
		return w_menu_set_location(W_MENU(this), (w_point*) location);
	}
	WResult _set_orientation(int orientation) {
		return w_menu_set_orientation(W_MENU(this), orientation);
	}
	WResult _set_visible(int visible) {
		return w_menu_set_visible(W_MENU(this), visible);
	}
private:
	void *handle[(sizeof(w_menu) - sizeof(w_widget)) / sizeof(void*)];
};
struct WMenuItems {
	int style;
	int accelerator;
	int image;
	const char *name;
	WControl::SelectionAction action;
};
class SWTP_PUBLIC WMenu: public WMenuBase {
public:
	WMenu() {
		this->items = 0;
		this->items_length = 0;
		this->notifyControl = 0;
	}
	bool CreateItems(WControl *notify, WImageList *imagelist, WMenuItems *items,
			size_t length);
	bool CreateItems(WImageList *imagelist, WMenuItems *items, size_t length) {
		return CreateItems(GetParent(), imagelist, items, length);
	}
	bool CreateItems(WMenuItems *items, size_t length) {
		return CreateItems(0, items, length);
	}
	bool CreateItems(WControl *notify, WImageList *imagelist,
			WMenuItems *items) {
		return CreateItems(GetParent(), imagelist, items, -1);
	}
	bool CreateItems(WControl *notify, WMenuItems *items) {
		return CreateItems(notify, 0, items);
	}
	bool CreateItems(WImageList *imagelist, WMenuItems *items) {
		return CreateItems(GetParent(), imagelist, items, -1);
	}
	bool CreateItems(WMenuItems *items) {
		return CreateItems(GetParent(), 0, items, -1);
	}
	WControl* GetNotifyControl() {
		return this->notifyControl;
	}
	void SetNotifyControl(WControl *control) {
		this->notifyControl = control;
	}
protected:
	void OnDispose(WEvent &e);
	bool OnHelp(WEvent &e);
	bool OnHide(WEvent &e);
	bool OnShow(WEvent &e);
	bool OnItemArm(WMenuEvent &e);
	bool OnItemHelp(WMenuEvent &e);
	bool OnItemSelection(WMenuEvent &e);
	bool OnItemDispose(WMenuEvent &e);
	bool OnItemAdded(WMenuEvent &e);
	bool Notify(WEvent &e);
protected:
	bool CreateSubItems(WMenuItem &parent, WImageList *imagelist,
			size_t &start);
	WMenuItems *items;
	size_t items_length;
	WControl *notifyControl;
};

#endif /* SWTP_WIDGETS_MENU_H_ */
