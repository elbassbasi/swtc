/*
 * Name:        swtp.cpp
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#define SWTPBUILD 1
#include <swtp.h>
#include <wchar.h>
#include <functional>
IDestruct::~IDestruct() {

}
size_t w_alloc_std_string(void *user_data, size_t size, void **string) {
	std::string *str = (std::string*) user_data;
	str->resize(size);
	*string = (void*) str->c_str();
	return size;
}
size_t w_alloc_new(void *user_data, size_t size, void **ptr) {
	*ptr = new char[size];
	*((void**) user_data) = *ptr;
	return size;
}
size_t WFileDialogSelectedFile::_alloc(void *user_data, size_t size,
		void **buf) {
	WFileDialogSelectedFile *file = (WFileDialogSelectedFile*) user_data;
	if (file->mem_alloc < size) {
		char *text = (char*) realloc(file->text, size);
		if (text != 0) {
			file->text = text;
			file->mem_alloc = size;
		}
	}
	*buf = file->text;
	return file->mem_alloc;
}

/*
 * Widget
 */
bool IWNotify::_InvokeAction(IWNotify *notify, IWNotify::SelectionAction action,
		WEvent &e) {
	return (notify->*action)(e);
}
int IWNotify::exec_function(void *user_data) {
	((IWNotify*) user_data)->OnNotifyExec();
	return true;
}
WWidget::~WWidget() {
	Dispose();
}
bool WWidget::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_PLATFORM:
		return OnPlatformEvent((WPlatformEvent*) e);
		break;
	case W_EVENT_DISPOSE:
		OnDispose(*e);
		return true;
		break;
	default:
		return false;
		break;
	}
}

bool WWidget::OnPlatformEvent(WPlatformEvent *e) {
	return false;
}
void WWidget::OnFreeMemory(WEvent &e, WWidget *widget) {
	if (widget != 0)
		delete widget;
}
void WWidget::OnDispose(WEvent &e) {
	FreeListenerData();
}
IWNotify* WWidget::GetNotify() {
	if ((this->state & __DATA_MASK_LISTENER) == __DATA_FLAGS_SELECTION_ACTION) {
		return 0;
	}
	return (IWNotify*) GetData(__DATA_NOTIFY);
}

void WWidget::SetNotify(IWNotify *notify) {
	if ((this->state & __DATA_MASK_LISTENER) == __DATA_FLAGS_SELECTION_ACTION) {
		this->state &= ~__DATA_FLAGS_SELECTION_ACTION;
		SetData(__DATA_LISTENER, 0);
	}
	SetData(__DATA_NOTIFY, notify);
}
bool WWidget::GetSelectionAction(IWNotify **obj,
		IWNotify::SelectionAction *function) {
	if ((this->state & __DATA_MASK_LISTENER) == __DATA_FLAGS_SELECTION_ACTION) {
		*obj = (IWNotify*) GetData(__DATA_NOTIFY);
		*function =
				*((IWNotify::SelectionAction*) &this->data[__DATA_LISTENER]);
		return true;
	} else {
		*obj = 0;
		*function = 0;
		return false;
	}
}
void WWidget::SetSelectionAction(IWNotify *obj,
		IWNotify::SelectionAction function) {
	FreeListenerData();
	SetData(__DATA_NOTIFY, obj);
	*((IWNotify::SelectionAction*) &this->data[__DATA_LISTENER]) = function;
	this->state &= ~__DATA_MASK_LISTENER;
	this->state |= __DATA_FLAGS_SELECTION_ACTION;
}

void WWidget::FreeListenerData() {
	IWListener *listenner = GetListener();
	if (listenner != 0) {
		listenner->DecRef();
	}
}
IWListener* WWidget::GetListener() {
	if ((this->state & __DATA_MASK_LISTENER) == __DATA_FLAGS_LISTENER) {
		return (IWListener*) GetData(__DATA_LISTENER);
	} else {
		return 0;
	}
}
void WWidget::SetListener(IWListener *listener) {
	IWListener *last = GetListener();
	if (listener != 0)
		listener->IncRef();
	if (last != 0)
		last->DecRef();
	FreeListenerData();
	SetData(__DATA_LISTENER, listener);
	this->state &= ~__DATA_MASK_LISTENER;
	this->state |= __DATA_FLAGS_LISTENER;
}
void WItem::SetListener(IWListener *listener) {
	IWListener *last = GetListener();
	SetData(listener);
	if (listener != 0)
		listener->IncRef();
	if (last != 0)
		last->DecRef();
}

WResult WWidget::post_event_proc(w_widget *widget, w_event *event) {
	return ((WWidget*) widget)->PostEvent0((WEvent*) event);
}
WResult WWidget::PostEvent0(WEvent *e) {
	bool ret = false;
	if (e->type == W_EVENT_PLATFORM) {
		ret = this->OnPlatformEvent((WPlatformEvent*) e);
	} else if (e->type == W_EVENT_FREE_MEMORY) {
		OnFreeMemory(*e, (WWidget*) e->widget);
		return false;
	} else {
		IWListener *listener = this->GetListener();
		if (listener != 0) {
			ret = listener->HandleEvent(e);
		}
		if (!ret) {
			ret = this->PostEvent((WEvent*) e);
		}
	}
	if (e->type == W_EVENT_DISPOSE) {
		SetListener(0);
	}
	return ret;
}
#if __cplusplus >= 201103L
void WWidget::SetListenerFunction(const WListenerFunction &function) {
	WListener *listener = new WListener();
	listener->function = function;
	listener->CreateRef();
	SetListener(listener);
}
void WWidget::SetSelectionFunction(const WSelectionFunction &function) {
	WSelectionListener *listener = new WSelectionListener();
	listener->function = function;
	listener->CreateRef();
	SetListener(listener);
}
bool WListener::HandleEvent(WEvent *e) {
	return this->function(*e);
}

bool WSelectionListener::HandleEvent(WEvent *e) {
	if (e->type == W_EVENT_SELECTION) {
		this->function(*e);
		return true;
	} else
		return false;
}

bool WSelectionItemListener::HandleEvent(WEvent *e) {
	if (e->type == W_EVENT_ITEM_SELECTION) {
		this->function(*((WMenuEvent*) e));
		return true;
	} else
		return false;
}
void WItem::SetListenerFunction(const WListenerFunction &function) {
	WListener *listener = new WListener();
	listener->function = function;
	listener->CreateRef();
	SetListener(listener);
}

void WItem::SetSelectionFunction(const WSelectionItemFunction &function) {
	WSelectionItemListener *listener = new WSelectionItemListener();
	listener->function = function;
	listener->CreateRef();
	SetListener(listener);
}
#endif
bool WWidget::_OnSelection(WEvent &e) {
	IWNotify *notify = GetNotify();
	if (notify != 0) {
		return notify->OnNotifySelection(e);
	}
	IWNotify::SelectionAction action;
	if (GetSelectionAction(&notify, &action)) {
		if (notify != 0 && action != 0) {
			return IWNotify::_InvokeAction(notify, action, e);
		}
	}
	return false;
}

WResult WWidget::Create(WToolkit *toolkit, WWidget *parent, wuint64 style) {
	WResult result = _w_widget_create(W_WIDGET(this), W_TOOLKIT(toolkit),
			W_WIDGET(parent), style, _GetClassID(), WWidget::post_event_proc);
	if (result > 0) {
		SetNotify(parent);
	}
	return result;
}
bool WWidget::OnNotifySelection(WEvent &e) {
	return false;
}

bool WWidget::OnNotifyItemSelection(WEvent &e) {
	return false;
}

bool WWidget::OnNotifyItemDispose(WEvent &e) {
	return false;
}

bool WWidget::NotifySelection(WEvent &e) {
	IWNotify *notify = GetNotify();
	if (notify != 0) {
		return notify->OnNotifySelection(e);
	}
	return false;
}
bool WWidget::NotifyItemSelection(WEvent &e) {
	IWNotify *notify = GetNotify();
	if (notify != 0) {
		return notify->OnNotifyItemSelection(e);
	}
	return false;
}

bool WWidget::NotifyItemDispose(WEvent &e) {
	IWNotify *notify = GetNotify();
	if (notify != 0) {
		return notify->OnNotifyItemDispose(e);
	}
	return false;
}

WListenerBase::WListenerBase() {
	this->ref = -1;
}
WListenerBase::~WListenerBase() {
}
void WListenerBase::IncRef() {
	if (this->ref >= 0) {
		this->ref = watomic_int_fetch_add(&this->ref, 1);
	}
}
void WListenerBase::DecRef() {
	if (this->ref > 0) {
		this->ref = watomic_int_fetch_sub(&this->ref, 1);
		if (this->ref == 0)
			delete this;
	}
}

WListenerBase* WListenerBase::CreateRef() {
	if (this->ref < 0)
		this->ref = 0;
	return this;
}

void WWidget::OnNotifyExec() {
}
/*
 * DropTarget
 */
bool WDropTarget::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_DRAGENTER:
		return OnDragEnter(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	case W_EVENT_DRAGOVER:
		return OnDragOver(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	case W_EVENT_DRAGLEAVE:
		return OnDragLeave(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	case W_EVENT_DRAGOPERATIONCHANGED:
		return OnDragOperationChanged(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	case W_EVENT_DROPACCEPT:
		return OnDropAccept(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	case W_EVENT_DROP:
		return OnDrop(reinterpret_cast<WDropTargetEvent&>(*e));
		break;
	default:
		return WWidget::PostEvent(e);
		break;
	}
}

bool WDropTarget::OnDragEnter(WDropTargetEvent &e) {
	return false;
}

bool WDropTarget::OnDragOver(WDropTargetEvent &e) {
	return false;
}

bool WDropTarget::OnDragLeave(WDropTargetEvent &e) {
	return false;
}

bool WDropTarget::OnDragOperationChanged(WDropTargetEvent &e) {
	return false;
}

bool WDropTarget::OnDropAccept(WDropTargetEvent &e) {
	return false;
}

w_class_id WDropTarget::_GetClassID() {
	return _W_CLASS_DROPTARGET;
}

bool WDropTarget::OnDrop(WDropTargetEvent &e) {
	return false;
}
/*
 * DragSource
 */
bool WDragSource::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_DRAGSTART:
		return OnDragStart(reinterpret_cast<WDragSourceEvent&>(*e));
		break;
	case W_EVENT_DRAGSETDATA:
		return OnDragSetData(reinterpret_cast<WDragSourceEvent&>(*e));
		break;
	case W_EVENT_DRAGFINISHED:
		return OnDragFinished(reinterpret_cast<WDragSourceEvent&>(*e));
		break;
	default:
		return WWidget::PostEvent(e);
		break;
	}
}

bool WDragSource::OnDragStart(WDragSourceEvent &e) {
	return false;
}
bool WDragSource::OnDragSetData(WDragSourceEvent &e) {
	return false;
}

w_class_id WDragSource::_GetClassID() {
	return _W_CLASS_DRAGSOURCE;
}

bool WDragSource::OnDragFinished(WDragSourceEvent &e) {
	return false;
}
/*
 * Menu
 */
w_class_id WMenuBase::_GetClassID() {
	return _W_CLASS_MENU;
}
bool WMenuBase::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_ITEM_SELECTION:
		return OnItemSelection(*static_cast<WMenuEvent*>(e));
		break;
	case W_EVENT_ITEM_DISPOSE:
		return OnItemDispose(*static_cast<WMenuEvent*>(e));
		break;
	case W_EVENT_ITEM_ARM:
		return OnItemArm(*static_cast<WMenuEvent*>(e));
		break;
	case W_EVENT_ITEM_HELP:
		return OnItemHelp(*static_cast<WMenuEvent*>(e));
		break;
	case W_EVENT_HELP:
		return OnHelp(*static_cast<WMenuEvent*>(e));
		break;
	case W_EVENT_HIDE:
		return OnHide(*static_cast<WMenuEvent*>(e));
		break;
	case W_EVENT_SHOW:
		return OnShow(*static_cast<WMenuEvent*>(e));
		break;
	default:
		return WWidget::PostEvent(e);
		break;
	}
}

bool WMenu::OnHelp(WEvent &e) {
	return false;
}

bool WMenu::OnHide(WEvent &e) {
	return false;
}

bool WMenu::OnShow(WEvent &e) {
	return false;
}

bool WMenu::OnItemArm(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener)
		return listener->HandleEvent(static_cast<WEvent*>(&e));
	else
		return false;
}

bool WMenu::OnItemHelp(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener)
		return listener->HandleEvent(static_cast<WEvent*>(&e));
	else
		return false;
}

bool WMenu::OnItemSelection(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener)
		return listener->HandleEvent(static_cast<WEvent*>(&e));
	else {
		if (this->items != 0) {
			wushort id = e.item->GetId();
			IWNotify::SelectionAction action = this->items[id].action;
			IWNotify *notify = (IWNotify*) GetData(__DATA_NOTIFY);
			if (action != 0 && notify != 0) {
				return IWNotify::_InvokeAction(notify, action, e);
			}
		}
	}
	return NotifyItemSelection(e);
}

bool WMenu::OnItemDispose(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener) {
		bool ret = listener->HandleEvent(static_cast<WEvent*>(&e));
		e->SetListener(0);
		return ret;
	} else {
		return NotifyItemDispose(e);
	}
}

bool WMenu::OnItemAdded(WMenuEvent &e) {
	IWListener *listener = e->GetListener();
	if (listener)
		return listener->HandleEvent(static_cast<WEvent*>(&e));
	else
		return false;
}

void WMenu::OnDispose(WEvent &e) {
	WWidget::OnDispose(e);
	this->items = 0;
}

bool WMenu::CreateItems(IWNotify *notify, WImageList *imagelist,
		WMenuItems *items, size_t length) {
	WMenuItem item;
	SetNotify(notify);
	GetRoot(item);
	size_t start = 0;
	this->items = items;
	this->items_length = length;
	bool ret = CreateSubItems(item, imagelist, start);
	if (length == -1) {
		this->items_length = start;
	}
	return ret;
}
bool WMenu::CreateSubItems(WMenuItem &parent, WImageList *imagelist,
		size_t &start) {
	WMenuItem item;
	while (1) {
		if (start >= items_length)
			return true;
		if (items[start].style == 0)
			return true;
		if (items[start].style == W_CASCADE_END) {
			start++;
			return true;
		}
		parent.Append(item, items[start].name, items[start].style);
		item.SetId(start);
		if (items[start].accelerator != 0) {
			item.SetAccelerator(items[start].accelerator);
		}
		if (items[start].image >= 0) {
			item.SetImageIndex(imagelist, items[start].image);
		}
		if (items[start].style & W_CASCADE) {
			start++;
			CreateSubItems(item, imagelist, start);
		} else
			start++;
	}
}

/*
 * Control
 */
WResult WControl::Create(WToolkit *toolkit, WComposite *parent, wuint64 style) {
	return WWidget::Create(toolkit, parent, style);
}

bool WControl::Create(WComposite *parent, wuint64 style) {
	return Create(0, parent, style);
}

WControl* WControl::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->Create(parent, W_NONE);
	}
	return this;
}

bool WControl::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_RESIZE:
		return OnResize(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_MOVE:
		return OnMove(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_DRAGDETECT:
		return OnDragDetect(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_FOCUSIN:
		return OnFocusIn(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_FOCUSOUT:
		return OnFocusOut(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_HELP:
		return OnHelp(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_KEYUP:
		return OnKeyUp(static_cast<WKeyEvent&>(*e));
		break;
	case W_EVENT_KEYDOWN:
		return OnKeyDown(static_cast<WKeyEvent&>(*e));
		break;
	case W_EVENT_TRAVERSE:
		return OnTraverse(static_cast<WKeyEvent&>(*e));
		break;
	case W_EVENT_MENUDETECT:
		return OnMenuDetect(static_cast<WMenuDetectEvent&>(*e));
		break;
	case W_EVENT_MOUSEDOWN:
		return OnMouseDown(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEUP:
		return OnMouseUp(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEDOUBLECLICK:
		return OnMouseDoubleClick(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEENTER:
		return OnMouseEnter(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEEXIT:
		return OnMouseExit(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEHOVER:
		return OnMouseHover(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEMOVE:
		return OnMouseMove(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_MOUSEWHEEL:
		return OnMouseWheel(static_cast<WMouseEvent&>(*e));
		break;
	case W_EVENT_PAINT:
		return OnPaint(reinterpret_cast<WPaintEvent&>(*e));
		break;
	case W_EVENT_TIMER:
		return OnTimer(static_cast<WTimerEvent&>(*e));
		break;
	case W_EVENT_GESTURE:
		return OnGesture(static_cast<WGestureEvent&>(*e));
		break;
	case W_EVENT_TOUCH:
		return OnTouch(static_cast<WTouchEvent&>(*e));
		break;
	case W_EVENT_COMPUTE_SIZE:
		return OnComputeSize((w_event_compute_size*) e);
		break;
	case W_EVENT_TOOLTIP_TEXT:
		return OnTooltipText(reinterpret_cast<WTooltipTextEvent&>(*e));
		break;
	default:
		return WWidget::PostEvent(e);
		break;
	}
}

bool WControl::OnComputeSize(w_event_compute_size *e) {
	return false; //w_widget_post_event(W_WIDGET(this), (w_event*) e, W_EVENT_SEND);
}

bool WControl::OnResize(WEvent &e) {
	return false;
}

bool WControl::OnMove(WEvent &e) {
	return false;
}

bool WControl::OnPaint(WPaintEvent &e) {
	return false;
}

bool WControl::OnMenuNotify(WEvent &e) {
	return false;
}

bool WControl::OnDragDetect(WMouseEvent &e) {
	return false;
}

bool WControl::OnFocusIn(WEvent &e) {
	return false;
}

bool WControl::OnFocusOut(WEvent &e) {
	return false;
}

bool WControl::OnHelp(WEvent &e) {
	return false;
}

bool WControl::OnKeyUp(WKeyEvent &e) {
	return false;
}

bool WControl::OnKeyDown(WKeyEvent &e) {
	return false;
}

bool WControl::OnTraverse(WKeyEvent &e) {
	return false;
}

bool WControl::OnMenuDetect(WMenuDetectEvent &e) {
	e.menu = GetMenu();
	return true;
}

bool WControl::OnMouseDown(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseUp(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseDoubleClick(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseEnter(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseExit(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseHover(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseMove(WMouseEvent &e) {
	return false;
}

bool WControl::OnMouseWheel(WMouseEvent &e) {
	return false;
}

bool WControl::OnGesture(WGestureEvent &e) {
	return false;
}

bool WControl::OnTouch(WTouchEvent &e) {
	return false;
}

bool WControl::OnReserved1(WEvent &e) {
	return false;
}

bool WControl::OnReserved2(WEvent &e) {
	return false;
}

bool WControl::OnReserved3(WEvent &e) {
	return false;
}

bool WControl::OnReserved4(WEvent &e) {
	return false;
}

bool WControl::OnTimer(WTimerEvent &e) {
	return false;
}

bool WControl::OnTooltipText(WTooltipTextEvent &e) {
	return false;
}
/*
 * Scrollable
 */
bool WScrollable::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_HSCROLL:
		return this->OnHScroll(*static_cast<WScrollBarEvent*>(e));
		break;
	case W_EVENT_VSCROLL:
		return this->OnVScroll(*static_cast<WScrollBarEvent*>(e));
		break;
	case W_EVENT_COMPUTE_TRIM:
		return this->OnComputeTrim(*reinterpret_cast<w_event_compute_trim*>(e));
		break;
	case W_EVENT_CLIENT_AREA:
		return this->OnGetClientArea(*reinterpret_cast<w_event_client_area*>(e));
		break;
	default:
		return WControl::PostEvent(e);
		break;
	}
}

bool WScrollable::OnHScroll(WScrollBarEvent &e) {
	return false;
}

bool WScrollable::OnVScroll(WScrollBarEvent &e) {
	return false;
}

bool WScrollable::OnComputeTrim(w_event_compute_trim &e) {
	return false;
}

bool WScrollable::OnGetClientArea(w_event_client_area &e) {
	return false;
}
/*
 * Composite
 */
w_class_id WComposite::_GetClassID() {
	return _W_CLASS_COMPOSITE;
}
bool WComposite::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_LAYOUTDETECT:
		return OnLayout(static_cast<WEvent&>(*e));
		break;
	default:
		return WScrollable::PostEvent(e);
		break;
	}

}

bool WComposite::OnLayout(WEvent &e) {
	return false;
}
/*
 * canvas
 */
w_class_id WCanvas::_GetClassID() {
	return _W_CLASS_CANVAS;
}
bool WCanvas::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_IMECOMPOSITION:
		return OnImeComposition(static_cast<WImeEvent&>(*e));
		break;
	default:
		return WComposite::PostEvent(e);
		break;
	}
}
bool WCanvas::OnImeComposition(WImeEvent &e) {
	return false;
}
/*
 *
 */
WResult WGLCanvas::Create(WComposite *parent, wuint64 style,
		WToolkit *toolkit) {
	return w_glcanvas_create(W_GLCANVAS(this), (w_toolkit*) toolkit,
			W_COMPOSITE(parent), style, WWidget::post_event_proc);
}
/*
 * WShell
 */
w_class_id WFrame::_GetClassID() {
	return _W_CLASS_SHELL;
}
bool WFrame::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_CLOSE:
		return OnClose(*e);
		break;
	case W_EVENT_ICONIFY:
		return OnIconify(*e);
		break;
	case W_EVENT_DEICONIFY:
		return OnDeiconify(*e);
		break;
	case W_EVENT_ACTIVATE:
		return OnActivate(*e);
		break;
	case W_EVENT_DEACTIVATE:
		return OnDeactivate(*e);
		break;
	default:
		return WCanvas::PostEvent(e);
		break;
	}
}

bool WFrame::OnClose(WEvent &e) {
	return false;
}

bool WFrame::OnIconify(WEvent &e) {
	return false;
}

bool WFrame::OnDeiconify(WEvent &e) {
	return false;
}

bool WFrame::OnActivate(WEvent &e) {
	return false;
}

bool WFrame::OnDeactivate(WEvent &e) {
	return false;
}

bool WFrame::OnNotify(WEvent &e) {
	return false;
}
/*
 * layout class
 */
void w_layout_cpp_compute_size(w_layout *layout, struct w_composite *composite,
		w_size *size, int wHint, int hHint, int flushCache) {
	((WLayout*) layout)->ComputeSize((WComposite*) composite, *((WSize*) size),
			wHint, hHint, flushCache);

}
wresult w_layout_cpp_flush_cache(w_layout *layout, w_control *control) {
	return ((WLayout*) layout)->FlushCache((WControl*) control);
}
void w_layout_cpp_do_layout(w_layout *layout, struct w_composite *composite,
		int flushCache) {
	((WLayout*) layout)->DoLayout((WComposite*) composite, flushCache);
}
void w_layout_cpp_set_data(w_layout *layout, w_control *control,
		const w_layout_data *data) {
	((WLayout*) layout)->SetData((WControl*) control, *((WLayoutData*) data));
}
void w_layout_cpp_get_data(w_layout *layout, w_control *control,
		w_layout_data *data) {
	((WLayout*) layout)->GetData((WControl*) control, *((WLayoutData*) data));
}
struct _w_layout_class __w_layout_cpp_class = { //
		w_layout_cpp_compute_size, //
				w_layout_cpp_flush_cache, //
				w_layout_cpp_do_layout, //
				w_layout_cpp_set_data, //
				w_layout_cpp_get_data //
		};
/*
 * WLayout
 */
void WLayout::Init() {
	((w_layout*) this)->clazz = &__w_layout_cpp_class;
}

WLayout::~WLayout() {
}
bool WLayout::FlushCache(WControl *control) {
	return false;
}

void WLayout::GetData(WControl *control, WLayoutData &data) {
}

void WLayout::SetData(WControl *control, const WLayoutData &data) {
}
/*
 * WFillLayout
 */
void WFillLayout::Init(int type) {
	w_layout_fill_init((w_layout_fill*) this, type);
	WLayout::Init();
}
WFillLayout::WFillLayout() {
	Init(W_HORIZONTAL);
}

WFillLayout::WFillLayout(int type) {
	Init(type);
}

void WFillLayout::ComputeSize(WComposite *composite, WSize &result, int wHint,
		int hHint, bool flushCache) {
	w_layout_fill_compute_size((w_layout*) this, (w_composite*) composite,
			(w_size*) &result, wHint, hHint, flushCache);
}

void WFillLayout::DoLayout(WComposite *composite, bool flushCache) {
	w_layout_fill_do_layout((w_layout*) this, (w_composite*) composite,
			flushCache);
}
/*
 * RowLayout
 */

void WRowLayout::ComputeSize(WComposite *composite, WSize &result, int wHint,
		int hHint, bool flushCache) {
	w_layout_row_compute_size((w_layout*) this, (w_composite*) composite,
			(w_size*) &result, wHint, hHint, flushCache);
}

void WRowLayout::DoLayout(WComposite *composite, bool flushCache) {
	w_layout_row_do_layout((w_layout*) this, (w_composite*) composite,
			flushCache);
}

void WRowLayout::SetData(WControl *control, const WLayoutData &data) {
	w_layout_row_set_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}

void WRowLayout::GetData(WControl *control, WLayoutData &data) {
	w_layout_row_get_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}
/*
 * WGridLayout
 */
void WGridLayout::Init(int numColumns, bool makeColumnsEqualWidth) {
	w_layout_grid_init((w_layout_grid*) this, numColumns,
			makeColumnsEqualWidth);
	WLayout::Init();
}
WGridLayout::WGridLayout() {
	Init();
}

WGridLayout::WGridLayout(int numColumns, bool makeColumnsEqualWidth) {
	Init(numColumns, makeColumnsEqualWidth);
}
void WGridLayout::ComputeSize(WComposite *composite, WSize &result, int wHint,
		int hHint, bool flushCache) {
	w_layout_grid_compute_size((w_layout*) this, (w_composite*) composite,
			(w_size*) &result, wHint, hHint, flushCache);
}
void WGridLayout::DoLayout(WComposite *composite, bool flushCache) {
	w_layout_grid_do_layout((w_layout*) this, (w_composite*) composite,
			flushCache);
}
void WGridLayout::SetData(WControl *control, const WLayoutData &data) {
	w_layout_grid_set_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}
void WGridLayout::GetData(WControl *control, WLayoutData &data) {
	w_layout_grid_get_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}
/*
 * FormLayout
 */

WFormLayout::WFormLayout() {
	Init();
}

void WFormLayout::ComputeSize(WComposite *composite, WSize &result, int wHint,
		int hHint, bool flushCache) {
	w_layout_form_compute_size((w_layout*) this, (w_composite*) composite,
			(w_size*) &result, wHint, hHint, flushCache);
}

void WFormLayout::DoLayout(WComposite *composite, bool flushCache) {
	w_layout_form_do_layout((w_layout*) this, (w_composite*) composite,
			flushCache);
}

void WFormLayout::SetData(WControl *control, const WLayoutData &data) {
	w_layout_form_set_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}

void WFormLayout::GetData(WControl *control, WLayoutData &data) {
	w_layout_form_get_data((w_layout*) this, (w_control*) control,
			(w_layout_data*) &data);
}
/*
 *
 */
void WSashLayoutBase::ComputeSize(WComposite *composite, WSize &result,
		int wHint, int hHint, bool flushCache) {
	w_layout_sash_compute_size((w_layout*) this, W_COMPOSITE(composite),
			(w_size*) &result, wHint, hHint, flushCache);
}

void WSashLayoutBase::DoLayout(WComposite *composite, bool flushCache) {
	w_layout_sash_do_layout((w_layout*) this, W_COMPOSITE(composite),
			flushCache);
}
WSashLayoutBase::~WSashLayoutBase() {
	w_layout_sash_free((w_layout_sash*) this);
}
void WSashLayoutBase::GetData(WControl *control, WLayoutData &data) {
	void *d;
	w_control_get_layout_data(W_CONTROL(control), &d);
	if (d != 0) {
		((WSashLayoutData*) &data)->weight = ((WSashLayoutData*) d)->weight;
	} else {
		((WSashLayoutData*) &data)->weight = ((200 << 16) + 999) / 1000;
	}
}

void WSashLayoutBase::SetData(WControl *control, const WLayoutData &data) {
	void *d;
	w_control_new_layout_data(W_CONTROL(control), &d, sizeof(WSashLayoutData));
	if (d != 0) {
		((WSashLayoutData*) d)->weight = ((WSashLayoutData*) &data)->weight;
	}
}
/*
 * WWebView
 */
w_class_id WWebView::_GetClassID() {
	return _W_CLASS_WEBVIEW;
}
/*
 * Button
 */
bool WButton::PostEvent(WEvent *e) {
	if (e->type == W_EVENT_SELECTION) {
		return OnSelection(reinterpret_cast<WSelectionEvent&>(*e));
	} else {
		return WControl::PostEvent(e);
	}
}
w_class_id WButton::_GetClassID() {
	return _W_CLASS_BUTTON;
}

bool WButton::OnSelection(WSelectionEvent &e) {
	return _OnSelection(e);
}
/*
 * WComboBox
 */
w_class_id WComboBox::_GetClassID() {
	return _W_CLASS_COMBOBOX;
}

/*
 * CoolBar
 */
w_class_id WCoolBar::_GetClassID() {
	return _W_CLASS_COOLBAR;
}
/*
 * WDateTime
 */
w_class_id WDateTime::_GetClassID() {
	return _W_CLASS_DATETIME;
}
/*
 * WExpandBar
 */

w_class_id WExpandBar::_GetClassID() {
	return _W_CLASS_EXPANDBAR;
}
/*
 * WGroup
 */
w_class_id WGroupBox::_GetClassID() {
	return _W_CLASS_GROUPBOX;
}
/*
 * WLabel
 */
w_class_id WLabel::_GetClassID() {
	return _W_CLASS_LABEL;
}
/*
 *
 */
w_class_id WProgressBar::_GetClassID() {
	return _W_CLASS_PROGRESSBAR;
}
/*
 *	WSash
 */
w_class_id WSash::_GetClassID() {
	return _W_CLASS_SASH;
}
bool WSash::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_SELECTION:
		return OnSelection(static_cast<WSashEvent&>(*e));
		break;
	default:
		return WControl::PostEvent(e);
		break;
	}
}

bool WSash::OnSelection(WSashEvent &e) {
	return _OnSelection(e);
}
/*
 *
 */
w_class_id WSlider::_GetClassID() {
	return _W_CLASS_SLIDER;
}
bool WSlider::PostEvent(WEvent *e) {
	if (e->type == W_EVENT_SELECTION) {
		return OnSelection(reinterpret_cast<WSelectionEvent&>(*e));
	} else {
		return WControl::PostEvent(e);
	}
}

bool WSlider::OnSelection(WSelectionEvent &e) {
	return _OnSelection(e);
}
/*
 *
 */
w_class_id WSpinner::_GetClassID() {
	return _W_CLASS_SPINNER;
}
/*
 *	WTabFolder
 */
w_class_id WTabView::_GetClassID() {
	return _W_CLASS_TABVIEW;
}
bool WTabView::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_ITEM_SELECTION:
		return OnItemSelection(static_cast<WTabEvent&>(*e));
		break;
	case W_EVENT_ITEM_GET_CONTROL:
		return OnItemGetControl(static_cast<WTabEvent&>(*e));
		break;
	case W_EVENT_ITEM_SET_CONTROL:
		return OnItemSetControl(static_cast<WTabEvent&>(*e));
		break;
	case W_EVENT_ITEM_CLOSE:
		return OnItemClose(static_cast<WTabEvent&>(*e));
		break;
	}
	return WComposite::PostEvent(e);
}

bool WTabView::OnItemSelection(WTabEvent &e) {
	return NotifyItemSelection(e);
}

bool WTabView::OnItemGetControl(WTabEvent &e) {
	e.control = (WControl*) e->GetData();
	return true;
}

bool WTabView::OnItemSetControl(WTabEvent &e) {
	e->SetData(e.control);
	return true;
}

bool WTabView::OnItemClose(WTabEvent &e) {
	return false;
}
/*
 *
 */
w_class_id WListViewBase::_GetClassID() {
	return _W_CLASS_LISTVIEW;
}
bool WListViewBase::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_ITEM_SELECTION:
		return OnItemSelection(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_DEFAULTSELECTION:
		return OnItemDefaultSelection(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_PAINT:
		return OnItemPaint(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_ERASE:
		return OnItemErase(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_MEASURE:
		return OnItemMeasure(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_GET_TEXT:
		return OnItemGetText(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_SET_TEXT:
		return OnItemSetText(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_GET_ATTR:
		return OnItemGetAttr(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_SET_ATTR:
		return OnItemSetAttr(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_DISPOSE:
		return OnItemDispose(static_cast<WTreeEvent&>(*e));
		break;
	}
	return WComposite::PostEvent(e);
}

bool WListViewBase::OnItemSelection(WListEvent &e) {
	return false;
}

bool WListViewBase::OnItemDefaultSelection(WListEvent &e) {
	return false;
}

bool WListViewBase::OnItemGetText(WListEvent &e) {
	return false;
}

bool WListViewBase::OnItemSetText(WListEvent &e) {
	return false;
}

bool WListViewBase::OnItemGetAttr(WListEvent &e) {
	return false;
}

bool WListViewBase::OnItemSetAttr(WListEvent &e) {
	return false;
}

bool WListViewBase::OnItemMeasure(WListEvent &e) {
	return false;
}

bool WListViewBase::OnItemErase(WListEvent &e) {
	return false;
}

bool WListViewBase::OnItemPaint(WListEvent &e) {
	return false;
}

bool WListViewBase::OnItemDispose(WListEvent &e) {
	return false;
}
/*
 *
 */
w_class_id WTextEdit::_GetClassID() {
	return _W_CLASS_TEXTEDIT;
}
bool WTextEdit::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_MODIFY:
		return OnModify(reinterpret_cast<WTextEditEvent&>(*e));
		break;
	case W_EVENT_DEFAULTSELECTION:
		return OnDefaultSelection(reinterpret_cast<WTextEditEvent&>(*e));
		break;
	case W_EVENT_VERIFY:
		return OnVerify(reinterpret_cast<WTextEditEvent&>(*e));
		break;
	default:
		return WScrollable::PostEvent(e);
		break;
	}
}

bool WTextEdit::OnModify(WTextEditEvent &e) {
	return false;
}

bool WTextEdit::OnDefaultSelection(WTextEditEvent &e) {
	return false;
}

bool WTextEdit::OnVerify(WTextEditEvent &e) {
	return false;
}
/*
 *
 */

w_class_id WToolBar::_GetClassID() {
	return _W_CLASS_TOOLBAR;
}

bool WToolBar::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_ITEM_SELECTION:
		return OnItemSelection(static_cast<WToolBarEvent&>(*e));
		break;
	case W_EVENT_ITEM_GET_MENU:
		return OnItemGetMenu(static_cast<WToolBarEvent&>(*e));
		break;
	case W_EVENT_ITEM_SET_MENU:
		return OnItemSetMenu(static_cast<WToolBarEvent&>(*e));
		break;
	case W_EVENT_ITEM_GET_CONTROL:
		return OnItemGetControl(static_cast<WToolBarEvent&>(*e));
		break;
	case W_EVENT_ITEM_SET_CONTROL:
		return OnItemSetControl(static_cast<WToolBarEvent&>(*e));
		break;
	}
	return WComposite::PostEvent(e);
}

bool WToolBar::OnItemSelection(WToolBarEvent &e) {
	return NotifyItemSelection(e);
}

bool WToolBar::OnItemGetMenu(WToolBarEvent &e) {
	if (e.item->GetStyle() & W_DROP_DOWN) {
		e.menu = (WMenu*) GetItemData(e.item);
		return true;
	} else
		return false;
}

bool WToolBar::OnItemSetMenu(WToolBarEvent &e) {
	if (e.item->GetStyle() & W_DROP_DOWN) {
		SetItemData(e.item, e.menu);
		return true;
	} else
		return false;
}

bool WToolBar::OnItemGetControl(WToolBarEvent &e) {
	if (e.item->GetStyle() & W_SEPARATOR) {
		e.control = (WControl*) GetItemData(e.item);
		return true;
	} else
		return false;
}

bool WToolBar::OnItemSetControl(WToolBarEvent &e) {
	if (e.item->GetStyle() & W_SEPARATOR) {
		SetItemData(e.item, e.control);
		return true;
	} else
		return false;
}
/*
 * WTreeView
 */
w_class_id WTreeView::_GetClassID() {
	return _W_CLASS_TREEVIEW;
}
bool WTreeView::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_ITEM_EXPAND:
		return OnItemExpand(static_cast<WTreeEvent&>(*e));
		break;
	case W_EVENT_ITEM_COLLAPSE:
		return OnItemCollapse(static_cast<WTreeEvent&>(*e));
		break;
	}
	return WListViewBase::PostEvent(e);
}
bool WTreeView::OnItemExpand(WTreeEvent &e) {
	return false;
}
bool WTreeView::OnItemCollapse(WTreeEvent &e) {
	return false;
}
/*
 * TrayItem
 */
w_class_id WTrayItem::_GetClassID() {
	return _W_CLASS_TRAYITEM;
}
bool WTrayItem::PostEvent(WEvent *e) {
	switch (e->type) {
	case W_EVENT_SELECTION:
		return OnTrayItemSelection(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_DEFAULTSELECTION:
		return OnTrayItemDefaultSelection(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_MENUDETECT:
		return OnTrayItemMenuDetect(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_SHOW:
		return OnShow(static_cast<WEvent&>(*e));
		break;
	case W_EVENT_HIDE:
		return OnHide(static_cast<WEvent&>(*e));
		break;
	}
	return WWidget::PostEvent(e);
}

bool WTrayItem::OnTrayItemSelection(WEvent &e) {
	return false;
}

bool WTrayItem::OnTrayItemDefaultSelection(WEvent &e) {
	return false;
}

bool WTrayItem::OnTrayItemMenuDetect(WEvent &e) {
	return false;
}

bool WTrayItem::OnShow(WEvent &e) {
	return false;
}

bool WTrayItem::OnHide(WEvent &e) {
	return false;
}
/*
 * Iterator
 */
wresult w_iterator_cpp_close(w_iterator *it) {
	IWIterator *i = reinterpret_cast<IWIterator*>(it);
	i->~IWIterator();
	return true;
}
wresult w_iterator_cpp_next(w_iterator *it, void *obj) {
	return reinterpret_cast<IWIterator*>(it)->Next(obj);
}
wresult w_iterator_cpp_reset(w_iterator *it) {
	return reinterpret_cast<IWIterator*>(it)->Reset();
}
wresult w_iterator_cpp_remove(w_iterator *it) {
	return reinterpret_cast<IWIterator*>(it)->Remove();
}
size_t w_iterator_cpp_get_count(w_iterator *it) {
	return reinterpret_cast<IWIterator*>(it)->GetCount();
}
_w_iterator_class w_iterator_cpp_clazz = { //
		w_iterator_cpp_close, //
				w_iterator_cpp_next, //
				w_iterator_cpp_reset, //
				w_iterator_cpp_remove, //
				w_iterator_cpp_get_count //
		};

IWIterator::IWIterator() {
	this->clazz = &w_iterator_cpp_clazz;
}

IWIterator::~IWIterator() {
}

WResult IWIterator::Reset() {
	return false;
}

WResult IWIterator::Remove() {
	return false;
}

size_t IWIterator::GetCount() {
	return 0;
}
wresult _w_stream_cpp_close(w_stream *stream) {
	return reinterpret_cast<IWStream*>(stream)->Close();
}
wresult _w_stream_cpp_read(w_stream *stream, void *pv, size_t cb,
		size_t *pcbRead) {
	return reinterpret_cast<IWStream*>(stream)->Read(pv, cb, pcbRead);
}
wresult _w_stream_cpp_write(w_stream *stream, const void *pv, size_t cb,
		size_t *pcbWritten) {
	return reinterpret_cast<IWStream*>(stream)->Write(pv, cb, pcbWritten);
}
wresult _w_stream_cpp_seek(w_stream *stream, wuint64 dlibMove, int dwOrigin) {
	return reinterpret_cast<IWStream*>(stream)->Seek(dlibMove, dwOrigin);
}
wresult _w_stream_cpp_tell(w_stream *stream, wuint64 *pos) {
	return reinterpret_cast<IWStream*>(stream)->Tell(pos);
}
wresult _w_stream_cpp_rewind(w_stream *stream) {
	return reinterpret_cast<IWStream*>(stream)->Rewind();
}
wresult _w_stream_cpp_set_size(w_stream *stream, wuint64 libNewSize) {
	return reinterpret_cast<IWStream*>(stream)->SetSize(libNewSize);
}
wresult _w_stream_cpp_get_size(w_stream *stream, wuint64 *libNewSize) {
	return reinterpret_cast<IWStream*>(stream)->GetSize(libNewSize);
}
_w_stream_class _w_stream_cpp_class = { _w_stream_cpp_close, _w_stream_cpp_read,
		_w_stream_cpp_write, _w_stream_cpp_seek, _w_stream_cpp_tell,
		_w_stream_cpp_rewind, _w_stream_cpp_set_size, _w_stream_cpp_get_size };
IWStream::IWStream() {
	this->clazz = &_w_stream_cpp_class;
}

IWStream::~IWStream() {
}
/*
 * thread
 */
IWRunnable::~IWRunnable() {
}
WThread::WThread() {
	Init();
}
WThread::~WThread() {
	this->Dispose();
}
void WThread::Run() {
}
int WThread::w_thread_runnable_start(void *args) {
	IWRunnable *runnable = (IWRunnable*) args;
	runnable->Run();
	return 0;
}
#if __cplusplus >= 201103L
class WRunnableLambda: public IWRunnable {
public:
	WRunnable function;
	WRunnableLambda() {

	}
	WRunnableLambda(WRunnable &function) {
		this->function = function;
	}
	void Run() {
		this->function();
		delete this;
	}
};
bool WThread::Create(WRunnable &run) {
	WRunnableLambda *lambda = new WRunnableLambda(run);
	return Create(lambda);
}
#endif
/*
 * WString
 */
extern "C" {
struct w_string_ref_ {
	unsigned sign :8; //must be 0
	unsigned length :24;
	wuint ref;
	char data[1];
};

struct w_string_ref_ empty_string_ = { 0, 0, (wuint) -1, { 0 } };
w_string_ref *empty_string = (w_string_ref*) &empty_string_;
w_string_ref* w_string_new(size_t length) {
	if (length == 0)
		return empty_string;
	w_string_ref *ref = (w_string_ref*) malloc(
			sizeof(w_string_ref) + length + 1);
	if (ref != 0) {
		ref->sign = 0;
		ref->ref = 0;
		ref->length = length;
		ref->data[length] = 0;
	}
	return ref;
}
const char* w_string_get_chars(struct w_string_ref *str) {
	if (str == 0)
		return 0;
	else if (str->sign == 0) {
		return str->data;
	} else
		return (const char*) str;
}
const char* w_string_get_chars_not_null(struct w_string_ref *str) {
	if (str == 0)
		return "";
	else if (str->sign == 0) {
		return str->data;
	} else
		return (const char*) str;
}
size_t w_string_get_length(struct w_string_ref *str) {
	if (str == 0)
		return 0;
	else if (str->sign == 0)
		return str->length;
	else
		return strlen((const char*) str);
}
void w_string_init_from_chars(struct w_string_ref **str, const char *chars,
		int copy) {
	if (chars == 0) {
		*str = 0;
		return;
	} else if (chars[0] == 0) {
		*str = empty_string;
		return;
	} else {
		if (copy) {
			int length = strlen(chars);
			w_string_ref *ref = w_string_new(length);
			if (ref != 0) {
				memcpy(ref->data, chars, length);
			}
			*str = ref;
		} else
			*str = (struct w_string_ref*) chars;
	}
}
void w_string_set_chars(struct w_string_ref **str, const char *chars,
		int copy) {
	w_string_ref_dec(*str);
	w_string_init_from_chars(str, chars, copy);
}
void w_string_init_from_wchars(struct w_string_ref **str,
		const wchar_t *wchars) {
	if (wchars == 0) {
		*str = 0;
	} else if (wchars[0] == 0) {
		*str = empty_string;
	} else {
		int l = wcslen(wchars);
		int length = w_utf8_from_utf16(wchars, l, 0, 0);
		w_string_ref *ref = w_string_new(length);
		if (ref != 0) {
			w_utf8_from_utf16(wchars, l, ref->data, length);
		}
		*str = ref;
	}
}
void w_string_set_wchars(struct w_string_ref **str, const wchar_t *wchars) {
	w_string_ref_dec(*str);
	w_string_init_from_wchars(str, wchars);
}
void w_string_init_from_ref(w_string_ref **str, w_string_ref *s, int clone) {
	*str = 0;
	w_string_move(str, s, clone);
}
void w_string_move(w_string_ref **str, w_string_ref *s, int clone) {
	w_string_ref *last = *str;
	if (s == 0) {
		*str = s;
	} else if (s->sign == 0) {
		if (s->ref >= 0)
			s->ref++;
		*str = s;
	} else {
		w_string_set_chars(str, (const char*) s, clone);
	}
	w_string_ref_dec(last);
}
void w_string_copy(w_string_ref **str, w_string_ref *s) {
	w_string_ref_dec(*str);
	if (s == 0) {
		*str = s;
	} else if (s->sign == 0) {
		w_string_ref *ref = w_string_new(s->length);
		if (ref != 0) {
			memcpy(ref->data, s->data, s->length);
		}
		*str = ref;
	} else {
		w_string_set_chars(str, (const char*) s, W_TRUE);
	}
}
void w_string_copy_chars(w_string_ref **ref, const char *str, size_t size) {
	w_string_ref_dec(*ref);
	*ref = w_string_new(size);
	if (*ref != 0) {
		memcpy((*ref)->data, str, size);
	}
}
void w_string_ref_inc(w_string_ref *str) {
	if (str != 0 && str->sign == 0 && str->ref >= 0) {
		str->ref++;
	}
}
w_string_ref* w_string_ref_dec(w_string_ref *str) {
	if (str != 0 && str->sign == 0 && str->ref >= 0) {
		str->ref--;
		if (str->ref <= 0) {
			free(str);
			return 0;
		}
	}
	return str;
}
void w_string_sprintf_0(w_string_ref **str, const char *format, va_list args) {
	w_string_ref_dec(*str);
	va_list cp;
	va_copy(cp, args);
	int count = vsnprintf(0, 0, format, cp);
	va_end(cp);
	w_string_ref *ref = w_string_new(count);
	*str = ref;
	if (ref == 0)
		return;
	vsnprintf(ref->data, count, format, args);
	va_end(args);
}

void w_string_sprintf(w_string_ref **str, const char *format, ...) {
	va_list args;
	va_start(args, format);
	w_string_sprintf_0(str, format, args);
	va_end(args);
}
void w_string_concat(w_string_ref **str, w_string_ref *s1, w_string_ref *s2) {
	w_string_ref_dec(*str);
	int l1, l2;
	l1 = w_string_get_length(s1);
	l2 = w_string_get_length(s2);
	w_string_ref *ref = w_string_new(l1 + l2);
	*str = ref;
	if (ref != 0) {
		memcpy(ref->data, w_string_get_chars(s1), l1);
		memcpy(&ref->data[l1], w_string_get_chars(s2), l2);
		ref->data[l1 + l2] = 0;
	}
}

size_t w_alloc_string_ref(void *user_data, size_t size, void **string) {
	w_string_ref *ref = w_string_new(size);
	*((w_string_ref**) user_data) = ref;
	if (ref != 0) {
		*string = ref->data;
		return size;
	} else {
		*string = 0;
		return 0;
	}
}
}
