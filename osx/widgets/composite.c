/*
 * Name:        composite.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "composite.h"
#include "toolkit.h"
void _w_composite_minimum_size(w_composite *composite, w_size *size, int wHint,
		int hHint, int changed) {
	w_iterator children;
	w_iterator_init(&children);
	w_composite_get_children(composite, &children);
	/*
	 * Since getClientArea can be overridden by subclasses, we cannot
	 * call getClientAreaInPixels directly.
	 */
	w_rect clientArea, rect;
	w_scrollable_get_client_area(W_SCROLLABLE(composite), &clientArea);
	int width = 0, height = 0;
	w_control *child = 0;
	while (w_iterator_next(&children, &child)) {
		if (child != 0) {
			w_control_get_bounds(child, &rect.pt, &rect.sz);
			width = WMAX(width, rect.x - clientArea.x + rect.width);
			height = WMAX(height, rect.y - clientArea.y + rect.height);
		}
	}
	size->width = width;
	size->height = height;
}
wresult _w_composite_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	//display.runSkin();
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	if (_W_COMPOSITE(widget)->layout != 0) {
		if (wHint == W_DEFAULT || hHint == W_DEFAULT) {
			//changed |= (state & LAYOUT_CHANGED) != 0;
			w_layout_compute_size(_W_COMPOSITE(widget)->layout,
					W_COMPOSITE(widget), e->size, wHint, hHint, e->changed);
			//state &= ~LAYOUT_CHANGED;
		} else {
			e->size->width = wHint;
			e->size->height = hHint;
		}
	} else {
		_w_composite_minimum_size(W_COMPOSITE(widget), e->size, wHint, hHint,
				W_TRUE);
		if (e->size->width == 0)
			e->size->width = DEFAULT_WIDTH;
		if (e->size->height == 0)
			e->size->height = DEFAULT_HEIGHT;
	}
	if (wHint != W_DEFAULT)
		e->size->width = wHint;
	if (hHint != W_DEFAULT)
		e->size->height = hHint;
	w_rect rect, trim;
	rect.x = 0;
	rect.y = 0;
	rect.width = e->size->width;
	rect.height = e->size->height;
	w_scrollable_compute_trim(W_SCROLLABLE(widget), &trim, &rect);
	e->size->width = trim.width;
	e->size->height = trim.height;
	return W_TRUE;
}
wresult _w_composite_create_handle(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_CANVAS;
	wuint64 style = _W_WIDGET(widget)->style;
	int scrolled = (style & (W_VSCROLL | W_HSCROLL)) != 0;
	if (!scrolled)
		_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
	NSScrollView *scrollView = 0;
	if (scrolled || priv->has_border(widget, priv) > 0) {
		scrollView = SWTScrollView_new(widget);
		NSScrollView_setDrawsBackground(scrollView, W_FALSE);
		if ((style & W_HSCROLL) != 0)
			NSScrollView_setHasHorizontalScroller(scrollView, W_TRUE);
		if ((style & W_VSCROLL) != 0)
			NSScrollView_setHasVerticalScroller(scrollView, W_TRUE);
		NSScrollView_setBorderType(scrollView,
				(style & W_BORDER) != 0 ? NSBezelBorder : NSNoBorder);
	}
	NSView *view = SWTView_new(widget);
	NSRect frame;
	memset(&frame, 0, sizeof(frame));
	NSView_initWithFrame(view, &frame);
	NSView_setFocusRingType(view, NSFocusRingTypeExterior);
	if (scrollView != 0) {
		NSView *contentView = NSScrollView_contentView(scrollView);
		NSView_setAutoresizesSubviews(contentView, W_TRUE);
		NSView_setAutoresizingMask((NSView*) view,
				NSViewWidthSizable | NSViewHeightSizable);
		NSScrollView_setDocumentView(scrollView, view);
		_W_WIDGET(widget)->handle = (NSView*) scrollView;
	} else {
		_W_WIDGET(widget)->handle = view;
	}
	return W_TRUE;
}
w_composite* _w_composite_find_deferred_control(w_composite *composite,
		_w_composite_priv *priv) {
	if ( _W_COMPOSITE(composite)->layoutCount > 0) {
		return composite;
	} else {
		w_composite *parent = _W_CONTROL(composite)->parent;
		_w_composite_priv *ppriv = _W_COMPOSITE_GET_PRIV(parent);
		return ppriv->find_deferred_control(parent, ppriv);
	}
}
wresult _w_composite_mark_layout(w_control *_this, int flags,
		_w_control_priv *priv) {
	//if (layout != null) {
	_W_WIDGET(_this)->state |= STATE_LAYOUT_NEEDED;
	if (flags & W_CHANGED)
		_W_WIDGET(_this)->state |= STATE_LAYOUT_CHANGED;
	//}
	if (flags & W_ALL) {
		NSView *view = priv->get_view(W_WIDGET(_this));
		NSArray *views = NSView_subviews(view);
		size_t count = NSArray_count(views);
		w_control *child;
		for (size_t i = 0; i < count; i++) {
			view = (NSView*) NSArray_objectAtIndex(views, i);
			child = (w_control*) _w_widget_find_control(view);
			if (child != 0) {
				_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(child);
				cpriv->mark_layout(child, flags, cpriv);
			}
		}
	}
	return W_TRUE;
}
wresult _w_composite_update_layout(w_control *_this, int flags,
		_w_control_priv *priv) {
	w_composite *parent =
	_W_COMPOSITE_PRIV(priv)->find_deferred_control(W_COMPOSITE(_this),
			_W_COMPOSITE_PRIV(priv));
	if (parent != 0) {
		_W_WIDGET(parent)->state |= STATE_LAYOUT_CHILD;
		return W_TRUE;
	}
	if ((_W_WIDGET(_this)->state & STATE_LAYOUT_NEEDED) != 0) {
		int changed = (_W_WIDGET(_this)->state & STATE_LAYOUT_CHANGED) != 0;
		_W_WIDGET(_this)->state &=
				~(STATE_LAYOUT_NEEDED | STATE_LAYOUT_CHANGED);
		//display.runSkin();
		//layout.layout(_this, changed);
		w_event _e;
		_e.type = W_EVENT_LAYOUTDETECT;
		_e.platform_event = 0;
		_e.widget = W_WIDGET(_this);
		_e.data = 0;
		_w_widget_post_event(W_WIDGET(_this), &_e);
	}
	if (flags & W_ALL) {
		_W_WIDGET(_this)->state &= ~STATE_LAYOUT_CHILD;
		NSView *view = priv->get_view(W_WIDGET(_this));
		NSArray *views = NSView_subviews(view);
		size_t count = NSArray_count(views);
		w_control *child;
		for (size_t i = 0; i < count; i++) {
			view = (NSView*) NSArray_objectAtIndex(views, i);
			child = (w_control*) _w_widget_find_control(view);
			if (child != 0) {
				_w_control_priv *cpriv = _W_CONTROL_GET_PRIV(child);
				cpriv->update_layout(child, flags, cpriv);
			}
		}
	}
	return W_TRUE;
}
/*
 * iterator of children control
 */
typedef struct _w_composite_children {
	w_basic_iterator iter;
	w_composite *composite;
	NSArray *views;
	int i;
} _w_composite_children;
wresult _w_composite_children_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_composite_children_next(w_iterator *it, void *obj) {
	_w_composite_children *iter = (_w_composite_children*) it;
	*((w_widget**) obj) = 0;
	if (iter->i == 0)
		return W_FALSE;
	iter->i--;
	NSView *view = (NSView*) NSArray_objectAtIndex(iter->views, iter->i);
	if (view == 0)
		return W_FALSE;
	*((w_widget**) obj) = _w_widget_find_control(view);
	if (*((w_widget**) obj) == 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_composite_children_reset(w_iterator *it) {
	_w_composite_children *iter = (_w_composite_children*) it;
	iter->i = NSArray_count(iter->views);
	return W_TRUE;
}
wresult _w_composite_children_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_composite_children_get_count(w_iterator *it) {
	_w_composite_children *iter = (_w_composite_children*) it;
	return NSArray_count(iter->views);
}
_w_iterator_class _w_composite_children_class = { //
		_w_composite_children_close, //
				_w_composite_children_next, //
				_w_composite_children_reset, //
				_w_composite_children_remove, //
				_w_composite_children_get_count //
		};
wresult _w_composite_get_children(w_composite *composite, w_iterator *it) {
	_w_composite_children *iter = (_w_composite_children*) it;
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(composite);
	NSView *view = priv->content_view(W_WIDGET(composite), priv);
	it->base.clazz = &_w_composite_children_class;
	iter->composite = composite;
	iter->views = NSView_subviews(view);
	iter->i = NSArray_count(iter->views);
	return W_TRUE;
}
wresult _w_composite_get_layout(w_composite *composite, w_layout **layout) {
	*layout = _W_COMPOSITE(composite)->layout;
	return W_TRUE;
}
wresult _w_composite_get_layout_deferred(w_composite *composite) {
	return W_FALSE;
}
wresult _w_composite_get_tab_list(w_composite *composite, w_iterator *it) {
	return W_FALSE;
}
wresult _w_composite_is_layout_deferred(w_composite *composite) {
	return W_FALSE;
}
wresult _w_composite_do_layout(w_composite *composite, int changed, int all) {
	return W_FALSE;
}
wresult _w_composite_layout_changed(w_composite *_this, w_control **changed,
		size_t length, int flags) {
	return W_FALSE;
}
wresult _w_composite_set_bounds(w_control *control, w_point *location,
		w_size *size) {
	wresult ret = _w_control_set_bounds(control, location, size);
	if (size != 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
		//if (layout != null) {
		priv->mark_layout(control, 0, priv);
		priv->update_layout(control, 0, priv);
		//}
	}
	return ret;
}
wresult _w_composite_set_layout(w_composite *composite, w_layout *layout) {
	_W_COMPOSITE(composite)->layout = layout;
	return W_FALSE;
}
wresult _w_composite_set_layout_deferred(w_composite *composite, int defer) {
	return W_FALSE;
}
/*
 * messages
 */
void _w_composite_class_init(struct _w_composite_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMPOSITE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_composite_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_composite);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_composite);
	/*
	 * functions
	 */
	W_CONTROL_CLASS(clazz)->set_bounds = _w_composite_set_bounds;
	clazz->get_children = _w_composite_get_children;
	clazz->get_layout = _w_composite_get_layout;
	clazz->get_layout_deferred = _w_composite_get_layout_deferred;
	clazz->get_tab_list = _w_composite_get_tab_list;
	clazz->is_layout_deferred = _w_composite_is_layout_deferred;
	clazz->do_layout = _w_composite_do_layout;
	clazz->layout_changed = _w_composite_layout_changed;
	clazz->set_layout = _w_composite_set_layout;
	clazz->set_layout_deferred = _w_composite_set_layout_deferred;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->create_handle = _w_composite_create_handle;
	_W_WIDGET_PRIV(priv)->compute_size = _w_composite_compute_size;
	priv->mark_layout = _w_composite_mark_layout;
	priv->update_layout = _w_composite_update_layout;
	_W_COMPOSITE_PRIV(priv)->find_deferred_control =
			_w_composite_find_deferred_control;
	/*
	 * messages
	 */
}
