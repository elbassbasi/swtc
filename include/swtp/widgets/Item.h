/*
 * Item.h
 *
 *  Created on: 5 févr. 2021
 *      Author: azeddine
 */

#ifndef SWTP_WIDGETS_ITEM_H_
#define SWTP_WIDGETS_ITEM_H_
#include "Widget.h"
class SWTP_PUBLIC WWidgetData {
private:
	inline void __Init() {
		((w_widgetdata*) this)->clazz = 0;
	}
public:
	WWidgetData() {
		__Init();
	}
	WWidgetData(const WWidgetData &resourcedata) {
		__Init();
		w_widgetdata_copy((w_widgetdata*) &resourcedata, (w_widgetdata*) this);
	}
	~WWidgetData() {
		w_widgetdata_close((w_widgetdata*) this);
	}
	bool IsOk() const {
		return w_widgetdata_is_ok((w_widgetdata*) this) > 0;
	}
	void Close() const {
		w_widgetdata_close((w_widgetdata*) this);
	}
public:
	WWidgetData& operator=(const WWidgetData &resourcedata) {
		w_widgetdata_copy((w_widgetdata*) &resourcedata, (w_widgetdata*) this);
		return *this;
	}
	bool operator ==(const WWidgetData &resourcedata) const {
		return w_widgetdata_equals((w_widgetdata*) this,
				(w_widgetdata*) &resourcedata);
	}
	bool operator !=(const WWidgetData &resourcedata) const {
		return !w_widgetdata_equals((w_widgetdata*) this,
				(w_widgetdata*) &resourcedata);
	}
private:
	struct _w_widgetdata_class *clazz;
};
class SWTP_PUBLIC WItem: public WWidgetData {
protected:
	friend class WWidget;
	WWidget* GetParentWidget() {
		WWidget *parent;
		_get_parent_widget(&parent);
		return parent;
	}
	IWListener* GetListener() {
		return (IWListener*) GetData();
	}
	void SetListener(IWListener *listener);
#if __cplusplus >= 201103L
	void SetListenerFunction(const WListenerFunction &function);
	void SetSelectionFunction(const WSelectionItemFunction &function);
#endif
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
	int GetIndex() {
		return _WReturnInt(_get_index());
	}
	/**
	 * Returns the receiver's text, which will be an empty
	 * string if it has never been set.
	 *
	 * @return the receiver's text
	 */
	WString GetText() {
		WString str;
		_get_text(w_alloc_string_ref, &str.ref, W_ENCODING_UTF8);
		return str;
	}
	/**
	 * Sets the receiver's text.
	 * <p>
	 * Note: If control characters like '\n', '\t' etc. are used
	 * in the string, then the behavior is platform dependent.
	 * </p>
	 * @param string the new text
	 */
	bool SetText(const char *text, int length) {
		return _WReturnBool(_set_text(text, length, W_ENCODING_UTF8));
	}
	/**
	 * Sets the receiver's text.
	 * <p>
	 * Note: If control characters like '\n', '\t' etc. are used
	 * in the string, then the behavior is platform dependent.
	 * </p>
	 * @param string the new text
	 */
	bool SetText(const char *text) {
		return SetText(text, -1);
	}
public:
	WResult _get_parent_widget(WWidget **parent) {
		return w_item_get_parent_widget(W_ITEM(this), (w_widget**) parent);
	}
	WResult _get_data(void **data) {
		return w_item_get_data(W_ITEM(this), data);
	}
	WResult _get_index() {
		return w_item_get_index(W_ITEM(this));
	}
	WResult _get_text(w_alloc alloc, void *user_data, int enc) {
		return w_item_get_text(W_ITEM(this), alloc, user_data, enc);
	}
	WResult _set_data(void *data) {
		return w_item_set_data(W_ITEM(this), data);
	}
	WResult _set_text(const char *text, int length, int enc) {
		return w_item_set_text(W_ITEM(this), text, length, enc);
	}
private:
	//void *handles[(sizeof(w_item) - sizeof(w_widgetdata)) / sizeof(void*)];
};
#endif /* SWTP_WIDGETS_ITEM_H_ */
