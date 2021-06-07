/*
 * Tray.h
 *
 *  Created on: 10 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_TRAY_H_
#define SWTP_WIDGETS_TRAY_H_
#include "Widget.h"
class WTray;
/**
 * Instances of this class represent icons that can be placed on the
 * system tray or task bar status area.
 * <p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>DefaultSelection, MenuDetect, Selection</dd>
 * </dl>
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WTrayItem: public WWidget {
public:
	WTrayItem() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>Tray</code>) and a style value
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
	WTrayItem(WTray *parent, int style) {

	}
	bool Create(WTray *parent, int style) {
		return WWidget::Create(0,(WWidget*) parent, style);
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Tray</code>.
	 *
	 * @return the receiver's parent
	 */
	WTray* GetParent() {
		WTray *tray;
		w_trayitem_get_parent(W_TRAYITEM(this), (w_tray**) &tray);
		return tray;
	}
	/**
	 * Returns the receiver's tool tip, or null if it has
	 * not been set.
	 *
	 * @return the receiver's tool tip text
	 */
	WToolTip* GetToolTip() {
		WToolTip *tooltip;
		w_trayitem_get_tooltip(W_TRAYITEM(this), (w_tooltip**) &tooltip);
		return tooltip;
	}
	/**
	 * Returns the receiver's tool tip text, or null if it has
	 * not been set.
	 *
	 * @return the receiver's tool tip text
	 */
	WString GetToolTipText() {
		w_string_ref *ref = 0;
		w_trayitem_get_tooltip_text(W_TRAYITEM(this), w_alloc_string_ref, &ref,
				W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Returns <code>true</code> if the receiver is visible and
	 * <code>false</code> otherwise.
	 *
	 * @return the receiver's visibility
	 */
	bool GetVisible() {
		return w_trayitem_get_visible(W_TRAYITEM(this));
	}
	/**
	 * Sets the receiver's image.
	 *
	 * @param image the new image
	 */
	bool SetImage(WImage *image) {
		return w_trayitem_set_image(W_TRAYITEM(this), W_IMAGE(image)) > 0;
	}

	/**
	 * Sets the receiver's tool tip to the argument, which
	 * may be null indicating that no tool tip should be shown.
	 *
	 * @param toolTip the new tool tip (or null)
	 */
	bool SetToolTip(WToolTip *toolTip) {
		return w_trayitem_set_tooltip(W_TRAYITEM(this), W_TOOLTIP(toolTip)) > 0;
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
		return w_trayitem_set_tooltip_text(W_TRAYITEM(this), string, -1,
				W_ENCODING_UTF8) > 0;
	}
	/**
	 * Makes the receiver visible if the argument is <code>true</code>,
	 * and makes it invisible otherwise.
	 *
	 * @param visible the new visibility state
	 */
	bool SetVisible(bool visible) {
		return w_trayitem_set_visible(W_TRAYITEM(this), visible) > 0;
	}
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnTrayItemSelection(WEvent &e);
	virtual bool OnTrayItemDefaultSelection(WEvent &e);
	virtual bool OnTrayItemMenuDetect(WEvent &e);
	virtual bool OnShow(WEvent &e);
	virtual bool OnHide(WEvent &e);
private:
	void *handles[(sizeof(w_trayitem) - sizeof(w_widget)) / sizeof(void*)];
};
/**
 * Instances of this class represent the system tray that is part
 * of the task bar status area on some operating systems.
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
 *
 * @see Display#getSystemTray
 */
class SWTP_PUBLIC WTray: public WWidget {
private:
	friend class WToolkit;
	WTray() {
	}
public:
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WTrayItem* GetItem(int index) {
		WTrayItem *item;
		w_tray_get_item(W_TRAY(this), index, (w_trayitem**) &item);
		return item;
	}
	/**
	 * Returns the number of items contained in the receiver.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return w_tray_get_item_count(W_TRAY(this));
	}
	/**
	 * Returns an array of <code>TrayItem</code>s which are the items
	 * in the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 */
	void GetItems(WIterator<WTrayItem*> &items) {
		w_tray_get_items(W_TRAY(this), (w_iterator*) &items);
	}
private:
	void *handles[(sizeof(w_tray) - sizeof(w_widget)) / sizeof(void*)];
};

#endif /* SWTP_WIDGETS_TRAY_H_ */
