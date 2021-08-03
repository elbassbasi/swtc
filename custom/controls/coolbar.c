/*
 * coolbar.c
 *
 *  Created on: 26 juil. 2021
 *      Author: azeddine
 */
#include "coolbar.h"
/*
 *
 */
void _w_coolbar_fix_point(w_coolbar *coolbar, w_size *result, w_size *pt) {
	if ((W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		result->width = pt->height;
		result->height = pt->width;
	} else {
		result->width = pt->width;
		result->height = pt->height;
	}
}
void _w_coolbar_fix_rectangle(w_coolbar *coolbar, w_rect *result,
		w_rect *rect) {
	if ((W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		result->x = rect->y;
		result->y = rect->x;
		result->width = rect->height;
		result->height = rect->width;
	} else {
		result->x = rect->x;
		result->y = rect->y;
		result->width = rect->width;
		result->height = rect->height;
	}
}
wresult cw_coolitem_compute_size(w_coolitem *coolitem, w_size *_result,
		int wHint, int hHint) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_get_bounds(w_coolitem *coolitem, w_rect *bounds) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		_w_coolbar_fix_rectangle(W_COOLBAR(parent), bounds, &_item->itemBounds);
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_get_control(w_coolitem *coolitem, w_control **control) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	*control = 0;
	if (_item != 0) {
		*control = _item->control;
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_get_minimum_size(w_coolitem *coolitem, w_size *size) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		_w_coolbar_fix_point(W_COOLBAR(parent), size, &_item->minimumSize);
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_get_order(w_coolitem *coolitem) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_get_preferred_size(w_coolitem *coolitem, w_size *size) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		_w_coolbar_fix_point(W_COOLBAR(parent), size, &_item->preferredSize);
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_get_size(w_coolitem *coolitem, w_size *size) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		_w_coolbar_fix_point(W_COOLBAR(parent), size, &_item->itemBounds.sz);
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_get_wrap_indice(w_coolitem *coolitem) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		result = W_TRUE;
	}
	return result;
}
void _w_coolitem_internal_internal_get_bounds(cw_coolitem *item,
		w_rect *bounds) {
	memcpy(bounds, &item->itemBounds, sizeof(w_rect));
}
int _w_coolitem_internal_get_minimum_width(w_coolbar *coolbar,
		cw_coolitem *_item) {
	/*int width = _item->minimumSize.width + MINIMUM_WIDTH;
	if ((_W_WIDGET(coolbar)->style & W_DROP_DOWN) != 0
			&& width < _item->preferredSize.width) {
		width += CHEVRON_IMAGE_WIDTH + CHEVRON_HORIZONTAL_TRIM
				+ CHEVRON_LEFT_MARGIN;
	}
	return width;*/
}
wresult cw_coolitem_set_control(w_coolitem *coolitem, w_control *control) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	if (control != 0) {
			if (w_widget_is_ok(W_WIDGET(control)))
				return W_ERROR_INVALID_ARGUMENT;
			w_widget *_p;
			w_widget_get_parent(W_WIDGET(control), &_p);
			if (parent != _p)
				return W_ERROR_INVALID_PARENT;
		}
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		/*_item->control = control;
		if (control != 0) {
			int controlWidth = _item->itemBounds.width - MINIMUM_WIDTH;
			if ((W_WIDGET(parent)->style & W_DROP_DOWN) != 0
					&& _item->itemBounds.width < _item->preferredSize.width) {
				controlWidth -= CHEVRON_IMAGE_WIDTH + CHEVRON_HORIZONTAL_TRIM
						+ CHEVRON_LEFT_MARGIN;
			}
			w_rect bounds, r;
			r.x = _item->itemBounds.x + MINIMUM_WIDTH;
			r.y = _item->itemBounds.y;
			r.width = controlWidth;
			r.height = _item->itemBounds.height;
			_w_coolbar_fix_rectangle(W_COOLBAR(coolbar), &bounds, &r);
			w_control_set_bounds(control, &bounds.pt, &bounds.sz);
		}*/
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_set_layout(w_coolitem *coolitem, int order, int wrapindices,
		w_size *sizes) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_set_minimum_size(w_coolitem *coolitem, w_size *size) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_set_order(w_coolitem *coolitem, int order) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_set_preferred_size(w_coolitem *coolitem, w_size *size) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_set_size(w_coolitem *coolitem, w_size *size) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		result = W_TRUE;
	}
	return result;
}
wresult cw_coolitem_set_wrap_indice(w_coolitem *coolitem, int wrap_indice) {
	w_widget *parent = _W_ITEM(coolitem)->parent;
	int index = _W_ITEM(coolitem)->index;
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_coolitem *_item = (cw_coolitem*) w_array_get(_items, index,
			sizeof(cw_coolitem));
	if (_item != 0) {
		result = W_TRUE;
	}
	return result;
}
/*
 *
 */
wresult cw_coolbar_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	style |= W_NO_FOCUS;
	style = (style | W_NO_REDRAW_RESIZE) & ~(W_VSCROLL | W_HSCROLL);
	wresult result = cw_control_create(widget, parent, style, post_event,
			sizeof(struct cw_coolbar_priv));
	if (result > 0) {
		cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
		w_toolkit *toolkit = w_widget_get_toolkit(widget);
		int cursor;
		if ((style & W_VERTICAL) != 0) {
			cursor = W_CURSOR_SIZENS;
		} else {
			cursor = W_CURSOR_SIZEWE;
		}
		priv->hoverCursor = w_toolkit_get_system_cursor(toolkit, cursor);
		priv->dragCursor = w_toolkit_get_system_cursor(toolkit,
				W_CURSOR_SIZEALL);
		priv->items = 0;
	}
	return result;
}
wresult cw_coolbar_get_item(w_coolbar *coolbar, int index, w_coolitem *item) {
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(coolbar));
	return W_FALSE;
}
wresult cw_coolbar_get_item_count(w_coolbar *coolbar) {
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(coolbar));
	return w_array_get_count(priv->items, 0);
}
wresult cw_coolbar_get_items(w_coolbar *coolbar, w_iterator *items) {
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(coolbar));
	cw_coolitem *_items;
	int count = w_array_get_count(priv->items, (void**) &_items);
	w_iterator_array_create(items, _items, count, sizeof(cw_coolitem), 0, 0);
	return W_TRUE;
}
wresult cw_coolbar_get_locked(w_coolbar *coolbar) {
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(coolbar));
	return W_FALSE;
}
wresult cw_coolbar_insert_item(w_coolbar *coolbar, w_coolitem *item, int style,
		int index) {
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(coolbar));
	cw_coolitem *_item = w_array_add(&priv->items, index, sizeof(cw_coolitem),
			&index);
	if (item != 0) {
		W_WIDGETDATA(item)->clazz = W_COOLBAR_GET_ITEM_CLASS(coolbar);
		_W_ITEM(item)->parent = W_WIDGET(coolbar);
		_W_ITEM(item)->index = index;
	}
	return W_FALSE;
}
wresult cw_coolbar_set_locked(w_coolbar *coolbar, int locked) {
	cw_coolbar_priv *priv = cw_control_get_priv(W_CONTROL(coolbar));
	return W_FALSE;
}
/*
 *
 */
wresult cw_coolbar_post_event(w_widget *widget, w_event *e, int flags) {
	wresult result = W_FALSE;
	switch (e->type) {
	}
	if (result == W_FALSE) {
		return widget->clazz->parentClass->post_event(widget, e, W_EVENT_SEND);
	} else
		return result;
}
void cw_coolbar_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_coolbar_class *clazz) {
	cw_composite_init_class(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_coolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_coolbar);
	cw_composite_init_class_priv(toolkit, classId, W_COMPOSITE_CLASS(clazz),
			sizeof(struct cw_coolbar_priv));
	W_WIDGET_CLASS(clazz)->create = cw_coolbar_create;
	W_WIDGET_CLASS(clazz)->post_event = cw_coolbar_post_event;
	/*
	 * public function
	 */
	clazz->get_item = cw_coolbar_get_item;
	clazz->get_item_count = cw_coolbar_get_item_count;
	clazz->get_items = cw_coolbar_get_items;
	clazz->get_locked = cw_coolbar_get_locked;
	clazz->insert_item = cw_coolbar_insert_item;
	clazz->set_locked = cw_coolbar_set_locked;
	/*
	 * item
	 */
	struct _w_coolitem_class *item = clazz->class_coolitem;
	W_WIDGETDATA_CLASS(item)->toolkit = toolkit;
	_w_item_class_init(W_ITEM_CLASS(item));
	item->compute_size = cw_coolitem_compute_size;
	item->get_bounds = cw_coolitem_get_bounds;
	item->get_control = cw_coolitem_get_control;
	item->get_minimum_size = cw_coolitem_get_minimum_size;
	item->get_order = cw_coolitem_get_order;
	item->get_preferred_size = cw_coolitem_get_preferred_size;
	item->get_size = cw_coolitem_get_size;
	item->get_wrap_indice = cw_coolitem_get_wrap_indice;
	item->set_control = cw_coolitem_set_control;
	item->set_layout = cw_coolitem_set_layout;
	item->set_minimum_size = cw_coolitem_set_minimum_size;
	item->set_order = cw_coolitem_set_order;
	item->set_preferred_size = cw_coolitem_set_preferred_size;
	item->set_size = cw_coolitem_set_size;
	item->set_wrap_indice = cw_coolitem_set_wrap_indice;
}
