/*
 * Widget.h
 *
 *  Created on: 25 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_WIDGET_H_
#define SWTP_WIDGETS_WIDGET_H_
#include "Event.h"
#include <string>
#include <atomic>
class WFrame;
class WAccessible;
class WComposite;
class WLayout;
class WToolTip;
class WControl;
class WToolkit;
class WTheme;

typedef WFrame WShell;

class SWTP_PUBLIC IWListener: public IDestruct {
public:
	virtual void IncRef() = 0;
	virtual void DecRef() = 0;
	virtual bool HandleEvent(WEvent *e)=0;
};

class SWTP_PUBLIC IWNotify: public IDestruct {
public:
	virtual bool OnNotifySelection(WEvent &e) = 0;
	virtual bool OnNotifyItemSelection(WEvent &e) = 0;
	virtual bool OnNotifyItemDispose(WEvent &e) = 0;
};
typedef bool (__thiscall IWNotify::*__SelectionAction)(WEvent &e);
typedef bool (__thiscall *SelectionAction)(void *_this, WEvent &e);
#define W_ACTION(x) ((__SelectionAction)&x)

class SWTP_PUBLIC WListenerBase: public IWListener {
protected:
	volatile int ref;
public:
	WListenerBase();
	~WListenerBase();
	void IncRef();
	void DecRef();
	WListenerBase* CreateRef();
};

#if __cplusplus >= 201103L
typedef std::function<bool(WEvent &e)> WListenerFunction;
typedef std::function<void(WEvent &e)> WSelectionFunction;
typedef std::function<void(class WMenuEvent &e)> WSelectionItemFunction;
class SWTP_PUBLIC WListener: public WListenerBase {
public:
	WListenerFunction function;
	bool HandleEvent(WEvent *e);
};
class SWTP_PUBLIC WSelectionListener: public WListenerBase {
public:
	WSelectionFunction function;
	bool HandleEvent(WEvent *e);
};
class SWTP_PUBLIC WSelectionItemListener: public WListenerBase {
public:
	WSelectionItemFunction function;
	bool HandleEvent(WEvent *e);
};
#endif
/**
 * This class is the abstract superclass of all user interface objects.
 * Widgets are created, disposed and issue notification to listeners
 * when events occur which affect them.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Dispose</dd>
 * </dl>
 * <p>
 * IMPORTANT: This class is intended to be subclassed <em>only</em>
 * within the SWT implementation. However, it has not been marked
 * final to allow those outside of the SWT development team to implement
 * patched versions of the class in order to get around specific
 * limitations in advance of when those limitations can be addressed
 * by the team.  Any class built using subclassing to access the internals
 * of this class will likely fail to compile or run between releases and
 * may be strongly platform specific. Subclassing should not be attempted
 * without an intimate and detailed understanding of the workings of the
 * hierarchy. No support is provided for user-written classes which are
 * implemented as subclasses of this class.
 * </p>
 */
class SWTP_PUBLIC WWidget: public IWNotify {
public:
	WWidget() {
		Init();
	}
	virtual ~WWidget();
private:
	WWidget(const WWidget &widget) {
		Init();
	}
	WWidget& operator=(const WWidget &widget) {
		return *this;
	}
protected:
	virtual w_class_id _GetClassID()=0;
	WResult PostEvent0(WEvent *e);
	virtual bool PostEvent(WEvent *e);
	virtual bool OnPlatformEvent(WPlatformEvent *e);
	virtual void OnFreeMemory(WEvent &e, WWidget *widget);
	virtual void OnDispose(WEvent &e);
	virtual void OnExec();
	bool OnNotifySelection(WEvent &e);
	bool OnNotifyItemSelection(WEvent &e);
	bool OnNotifyItemDispose(WEvent &e);
	void FreeListenerData();
	bool _OnSelection(WEvent &e);
protected:
	static WResult post_event_proc(w_widget *widget, w_event *event);
	static int exec_function(void *args);
	bool DefaultPostEvent(WEvent *e) {
		return w_widget_default_post_event(W_WIDGET(this), W_EVENT(e));
	}
	enum {
		__DATA_USER = 0,
		__DATA_NOTIFY = 1,
		__DATA_LISTENER = 2,

		__DATA_MASK_LISTENER = 0xFF000000,
		__DATA_FLAGS_C_FUNCTION = 0x01000000,
		__DATA_FLAGS_LISTENER = 0x02000000,
		__DATA_FLAGS_SELECTION_ACTION = 0x03000000,
	};
	/**
	 * index 0..4
	 *  0 : user data
	 *  1 : used as notify
	 *  2 : used as listener
	 *  3 : not used
	 *  4 : not used
	 */
	void* GetData(int index) {
		return w_widget_get_data(W_WIDGET(this), index);
	}
	void* SetData(int index, void *data) {
		return w_widget_set_data(W_WIDGET(this), index, data);
	}
public:
	virtual WResult Create(WToolkit *toolkit, WWidget *parent, wuint64 style);
	static void* GetItemData(WItem *item) {
		void *data;
		w_item_get_data(W_ITEM(item), &data);
		return data;
	}
	static bool SetItemData(WItem *item, void *data) {
		return _WReturnBool(w_item_set_data(W_ITEM(item), data));
	}
	bool IsOk() {
		return w_widget_is_ok(W_WIDGET(this)) > 0;
	}
	bool IsDisposed() {
		return !IsOk();
	}
	void Dispose() {
		w_widget_dispose(W_WIDGET(this));
	}
	int GetClassId() {
		return w_widget_class_id(W_WIDGET(this));
	}
	/**
	 * Returns the receiver's shell. For all controls other than
	 * shells, this simply returns the control's nearest ancestor
	 * shell. Shells return themselves, even if they are children
	 * of other shells.
	 *
	 * @return the receiver's shell
	 *
	 * @see #getParent
	 */
	WFrame* GetFrame() {
		WFrame *frame;
		w_widget_get_shell(W_WIDGET(this), (w_shell**) &frame);
		return frame;
	}
	WShell* GetShell() {
		return (WShell*) GetFrame();
	}
	WToolkit* GetToolkit() {
		return (WToolkit*) w_widget_get_toolkit(W_WIDGET(this));
	}
	WTheme* GetTheme() {
		return (WTheme*) w_widget_get_theme(W_WIDGET(this));
	}
	void AsyncExec(w_thread_start function, void *args) {
		w_toolkit_async_exec(w_widget_get_toolkit(W_WIDGET(this)), function,
				args);
	}
	void AsyncExec() {
		this->AsyncExec(WWidget::exec_function, this);
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
		return w_widget_get_style(W_WIDGET(this));
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
	 *
	 * @see #setData(Object)
	 */
	void* GetData() {
		return GetData(__DATA_USER);
	}
	IWNotify* GetNotify();
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
	 *
	 * @see #getData()
	 */
	void* SetData(void *data) {
		return SetData(__DATA_USER, data);
	}
	void SetNotify(IWNotify *notify);
	int GetId() {
		return w_widget_get_id(W_WIDGET(this));
	}
	void SetId(int id) {
		w_widget_set_id(W_WIDGET(this), id);
	}
	bool GetSelectionAction(void **_this, SelectionAction *function);
	void SetSelectionAction(void *_this, SelectionAction function);
	void SetSelectionAction(void *_this, __SelectionAction function);
	IWListener* GetListener();
	void SetListener(IWListener *listener);
#if __cplusplus >= 201103L
	void SetListenerFunction(const WListenerFunction &function);
	void operator <<=(const WListenerFunction &function) {
		SetListenerFunction(function);
	}
protected:
	void SetSelectionFunction(const WSelectionFunction &function);
#endif
	bool NotifySelection(WEvent &e);
	bool NotifyItemSelection(WEvent &e);
	bool NotifyItemDispose(WEvent &e);
private:
	void Init() {
		w_widget_init(W_WIDGET(this));
	}
	struct _w_widget_class *clazz;
	volatile int ref;
	wuint id;
	wuint64 style;
	wuint state;
	wuint state0;
	void *handle;
	w_widget_post_event_proc post_event;
	void *data[5];
};

namespace swt {
typedef ::WWidget Widget;
typedef ::IWListener IListener;
typedef ::WListenerBase ListenerBase;
#if __cplusplus >= 201103L
typedef ::WListenerFunction ListenerFunction;
typedef ::WSelectionFunction SelectionFunction;
typedef ::WListener Listener;
typedef ::WSelectionListener SelectionListener;
#endif
}  // namespace swt

#endif /* SWTP_WIDGETS_WIDGET_H_ */
