/*
 * expandbar.c
 *
 *  Created on: 28 juin 2021
 *      Author: Azeddine
 */
#include "expandbar.h"
#define TEXT_INSET 6
#define BORDER 1
#define CHEVRON_SIZE 24
/*
 * expand item
 */
wresult cw_expanditem_get_data(w_item *item, void **data) {
	w_widget *parent = _W_ITEM(item)->parent;
	int index = _W_ITEM(item)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		*data = _item->userdata;
		return W_TRUE;
	}
	*data = 0;
	return W_FALSE;
}
wresult cw_expanditem_get_index(w_item *item) {
	return _W_ITEM(item)->index;
}
wresult cw_expanditem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	int index = _W_ITEM(item)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		w_alloc_set_text(alloc, user_data, enc, _item->text, -1,
				W_ENCODING_UTF8);
	}
	return result;
}
wresult cw_expanditem_set_data(w_item *item, void *data) {
	w_widget *parent = _W_ITEM(item)->parent;
	int index = _W_ITEM(item)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		_item->userdata = data;
		result = W_TRUE;
	}
	return result;
}
wresult cw_expanditem_set_text(w_item *item, const char *text, int length,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	int index = _W_ITEM(item)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		if (_item->text != 0) {
			free(_item->text);
			_item->text = 0;
		}
		w_alloc_set_text(w_alloc_buffer_new, &_item->text, W_ENCODING_UTF8,
				text, length, enc);
		cw_expanditem_redraw(W_EXPANDBAR(parent), _item, W_TRUE);
	}
	return result;
}
wresult cw_expanditem_get_control(w_expanditem *expanditem,
		w_control **control) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		*control = _item->control;
		return W_TRUE;
	}
	*control = 0;
	return W_FALSE;
}
wresult cw_expanditem_get_expanded(w_expanditem *expanditem) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		return _item->expanded;
	}
	return W_FALSE;
}
wresult cw_expanditem_get_header_height(w_expanditem *expanditem) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		int bandHeight = priv->band_height;
		w_imagelist *imagelist = priv->imagelist;
		if (imagelist != 0) {
			w_size sz;
			w_imagelist_get_size(imagelist, &sz);
			bandHeight = WMAX(bandHeight, sz.height);
		}
		return bandHeight;
	}
	return 0;
}
wresult cw_expanditem_get_height(w_expanditem *expanditem) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		return _item->rect.height;
	}
	return 0;
}
wresult cw_expanditem_get_image(w_expanditem *expanditem) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		return _item->image - 1;
	}
	return -1;
}
void cw_expanditem_init_themedata(w_widget *expandbar, cw_expanditem *_item,
		w_themedata *data, w_graphics *gc, w_rect *bounds) {
	w_widget_init_themedata(expandbar, data);
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	data->gc = gc;
	data->bounds = bounds;
	data->clazz = W_THEME_CLASS_EXPANDITEM;
	data->state = 0;
	data->imagelist = priv->imagelist;
	if (_item != 0) {
		data->attr.text = _item->text;
		data->attr.image = _item->image - 1;
		if (_item->expanded) {
			data->state |= W_THEME_STATE_PART_EXPANDED;
		}
		if (_item->hover) {
			data->state |= W_THEME_STATE_PART_HOT;
		}
	}
}
void cw_expanditem_redraw(w_expandbar *expandbar, cw_expanditem *_item,
		int all) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	int headerHeight = priv->band_height;
	w_rect rect;
	rect.x =
			all ? _item->rect.x : _item->rect.x + _item->rect.width
							- headerHeight;
	rect.y = _item->rect.y;
	rect.width = _item->rect.width;
	rect.height = headerHeight;
	w_control_redraw(W_CONTROL(expandbar), &rect, W_TRUE);
	w_size imageSize;
	imageSize.width = 0;
	imageSize.height = 0;
	w_imagelist *imagelist = priv->imagelist;
	if (imagelist != 0) {
		w_imagelist_get_size(imagelist, &imageSize);
	}
	if (imageSize.height > headerHeight) {
		rect.x = _item->rect.x + TEXT_INSET;
		rect.y = _item->rect.y + headerHeight - imageSize.height;
		rect.width = imageSize.width;
		rect.height = headerHeight - imageSize.height;
		w_control_redraw(W_CONTROL(expandbar), &rect, W_TRUE);
	}
}
int cw_expanditem_get_preferred_width(w_expandbar *expandbar,
		cw_expanditem *_item, w_graphics *gc, int imageWidth) {
	w_theme *theme;
	w_size size;
	w_widget_get_theme(W_WIDGET(expandbar), &theme);
	w_themedata data;
	cw_expanditem_init_themedata(W_WIDGET(expandbar), _item, &data, gc, 0);
	w_theme_measure(theme, W_THEME_MASK_ALL, &data, &size);
	return size.width;
}
int cw_expanditem_is_hover(w_expandbar *expandbar, cw_expanditem *_item,
		int bandHeight, int x, int y) {
	return _item->rect.x < x && x < (_item->rect.x + _item->rect.width)
			&& _item->rect.y < y && y < (_item->rect.y + bandHeight);
}
wresult cw_expanditem_set_control(w_expanditem *expanditem,
		w_control *control) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	if (control != 0) {
		if (w_widget_is_ok(W_WIDGET(control)) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		w_widget *_parent;
		w_widget_get_parent(W_WIDGET(control), &_parent);
		if (_parent != parent)
			return W_ERROR_INVALID_PARENT;
	}
	int index = _W_ITEM(expanditem)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		_item->control = control;
		if (control != 0) {
			int headerHeight = priv->band_height;
			w_control_set_visible(control, _item->expanded);
			w_rect r;
			r.x = _item->rect.x;
			r.y = _item->rect.y + headerHeight;
			r.width = _item->rect.width;
			r.height = _item->rect.height;
			w_control_set_bounds(control, &r.pt, &r.sz);
		}
		result = W_TRUE;
	}
	return result;
}
void cw_expanditem_set_bounds(w_expandbar *expandbar, cw_expanditem *_item,
		w_point *location, w_size *size) {
	cw_expanditem_redraw(expandbar, _item, W_TRUE);
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	int headerHeight = priv->band_height;
	w_point *newlocation = 0, location_tmp;
	w_size *newsize = 0, size_tmp;
	if (location) {
		_item->rect.x = location->x;
		_item->rect.y = location->y;
		w_imagelist *imagelist = priv->imagelist;
		if (imagelist != 0) {
			w_imagelist_get_size(imagelist, &size_tmp);
			if (size_tmp.height > headerHeight) {
				_item->rect.y += (size_tmp.height - headerHeight);
			}
		}
		cw_expanditem_redraw(expandbar, _item, W_TRUE);
		newlocation = &location_tmp;
		location_tmp.x = location->x;
		location_tmp.y = _item->rect.y;
	}
	if (size) {
		_item->rect.width = size->width;
		_item->rect.height = size->height;
		cw_expanditem_redraw(expandbar, _item, W_TRUE);
		newsize = &size_tmp;
		size_tmp.width = size->width;
		size_tmp.height = size->height;
	}
	w_control *control = _item->control;
	if (w_widget_is_ok(W_WIDGET(control))) {
		location_tmp.y += headerHeight;
		w_control_set_bounds(control, newlocation, newsize);
	}
}
wresult cw_expanditem_set_expanded(w_expanditem *expanditem, int expanded) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		_item->expanded = expanded;
		cw_expandbar_show_item(W_EXPANDBAR(parent), index, _item);
		result = W_TRUE;
	}
	return result;
}
wresult cw_expanditem_set_height(w_expanditem *expanditem, int height) {
	if (height < 0)
		return W_FALSE;
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		w_size sz;
		sz.width = _item->rect.width;
		sz.height = height;
		cw_expanditem_set_bounds(W_EXPANDBAR(parent), _item, 0, &sz);
		if (_item->expanded) {
			cw_expandbar_layout_items(W_EXPANDBAR(parent), index + 1);
			cw_expandbar_set_scrollbar(W_EXPANDBAR(parent));
		}
		result = W_TRUE;
	}
	return result;
}
wresult cw_expanditem_set_image(w_expanditem *expanditem, int image) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(parent));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	cw_expanditem *_item = (cw_expanditem*) w_array_get(_items, index,
			sizeof(cw_expanditem));
	if (_item != 0) {
		if (image < 0)
			image = -1;
		_item->image = image + 1;
		cw_expanditem_redraw(W_EXPANDBAR(parent), _item, W_TRUE);
		result = W_TRUE;
	}
	return result;
}
/*
 * expand bar
 */
wuint64 cw_expandbar_check_style(w_widget *widget, wuint64 style) {
	style &= ~W_HSCROLL;
	return style | W_NO_BACKGROUND;
}
wresult cw_expandbar_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	style &= ~W_HSCROLL;
	style |= W_NO_BACKGROUND;
	wresult result = cw_control_create(widget, parent, style, post_event,
			sizeof(struct cw_expandbar_priv));
	if (result > 0) {
		cw_expandbar_update_band_heigth(widget);
	}
	return result;

}
wresult cw_expandbar_compute_size(w_widget *widget, w_event_compute_size *e) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	w_rect rect;
	memset(&rect, 0, sizeof(rect));
	if (wHint == W_DEFAULT || hHint == W_DEFAULT) {
		cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
		w_array *items = priv->items;
		cw_expanditem *_item;
		int itemCount = w_array_get_count(items, (void**) &_item);
		if (itemCount > 0) {
			w_graphics gc;
			w_graphics_init(&gc);
			w_control_get_graphics(W_CONTROL(widget), &gc);
			int spacing = priv->spacing;
			rect.height += spacing;
			int headerHeight = priv->band_height;
			w_imagelist *imagelist = priv->imagelist;
			int imageWidth = 0;
			if (imagelist != 0) {
				w_size sz;
				w_imagelist_get_size(imagelist, &sz);
				headerHeight = WMAX(headerHeight, sz.height);
				imageWidth = sz.width;
			}
			for (int i = 0; i < itemCount; i++) {
				rect.height += headerHeight;
				if (_item->expanded)
					rect.height += _item->rect.height;
				rect.height += spacing;
				int preferredWidth = cw_expanditem_get_preferred_width(
						W_EXPANDBAR(widget), _item, &gc, imageWidth);
				rect.width = WMAX(rect.width, preferredWidth);
				_item++;
			}
			w_graphics_dispose(&gc);
		}
	}
	if (rect.width == 0)
		rect.width = 64;
	if (rect.height == 0)
		rect.height = 64;
	if (wHint != W_DEFAULT)
		rect.width = wHint;
	if (hHint != W_DEFAULT)
		rect.height = hHint;
	w_rect trim;
	memcpy(&trim, &rect, sizeof(w_rect));
	wresult result = w_scrollable_compute_trim(W_SCROLLABLE(widget), &trim,
			&rect);
	e->size->width = trim.width;
	e->size->height = trim.height;
	return result;
}
wresult cw_expandbar_get_imagelist(w_expandbar *expandbar,
		w_imagelist **imagelist) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	*imagelist = priv->imagelist;
	return W_TRUE;
}
wresult cw_expandbar_get_item(w_expandbar *expandbar, int index,
		w_expanditem *item) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	wresult result = W_FALSE;
	w_array *_items = priv->items;
	if (_items != 0 && index < _items->count) {
		W_WIDGETDATA(item)->clazz = 0;
		_W_ITEM(item)->parent = W_WIDGET(expandbar);
		_W_ITEM(item)->index = index;
	}
	return result;
}
wresult cw_expandbar_get_item_count(w_expandbar *expandbar) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	w_array *_items = priv->items;
	int itemCount = w_array_get_count(_items, 0);
	return itemCount;
}
wresult cw_expandbar_get_items(w_expandbar *expandbar, w_iterator *items) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	return W_FALSE;
}
wresult cw_expandbar_get_spacing(w_expandbar *expandbar) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	return priv->spacing;
}
wresult cw_expandbar_insert_item(w_expandbar *expandbar, w_expanditem *item,
		int index) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	cw_expanditem *_item = w_array_add(&priv->items, index,
			sizeof(cw_expanditem), &index);
	if (_item == 0)
		return W_ERROR_NO_MEMORY;
	if (priv->focusItem == -1)
		priv->focusItem = index;
	w_rect rect;
	w_control_get_bounds(W_CONTROL(expandbar), 0, &rect.sz);
	_item->rect.width = WMAX(0, rect.width - priv->spacing * 2);
	cw_expandbar_layout_items(expandbar, index);
	cw_expandbar_set_scrollbar(expandbar);
	if (item != 0) {
		W_WIDGETDATA(item)->clazz = W_EXPANDBAR_GET_ITEM_CLASS(expandbar);
		_W_ITEM(item)->parent = W_WIDGET(expandbar);
		_W_ITEM(item)->index = index;
	}
	return W_TRUE;
}
void cw_expandbar_layout_items(w_expandbar *expandbar, int index) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	w_array *_items = priv->items;
	cw_expanditem *_item;
	int itemCount = w_array_get_count(_items, (void**) &_item);
	if (index < itemCount) {
		w_rect r;
		int spacing = priv->spacing;
		r.y = spacing - priv->yCurrentScroll;
		r.x = spacing;
		int headerHeight = priv->band_height;
		w_imagelist *imagelist = priv->imagelist;
		if (imagelist != 0) {
			w_imagelist_get_size(imagelist, &r.sz);
			headerHeight = WMAX(headerHeight, r.sz.height);
		}
		for (int i = 0; i < index; i++) {
			if (_item->expanded)
				r.y += _item->rect.height;
			r.y += headerHeight + spacing;
			_item++;
		}
		for (int i = index; i < itemCount; i++) {
			cw_expanditem_set_bounds(expandbar, _item, &r.pt, 0);
			if (_item->expanded)
				r.y += _item->rect.height;
			r.y += headerHeight + spacing;
			_item++;
		}
	}
}
void cw_expandbar_show_item(w_expandbar *expandbar, int index,
		cw_expanditem *_item) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	w_control *control = _item->control;
	if (w_widget_is_ok(W_WIDGET(control)) > 0) {
		w_control_set_visible(control, _item->expanded);
	}
	cw_expanditem_redraw(expandbar, _item, W_TRUE);
	cw_expandbar_layout_items(expandbar, index + 1);
	cw_expandbar_set_scrollbar(expandbar);
}
void cw_expandbar_show_focus(w_expandbar *expandbar, int up) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	int focusIndex = priv->focusItem;
	if (focusIndex == -1)
		return;
	w_array *_items = priv->items;
	cw_expanditem *focusItem = w_array_get(_items, focusIndex,
			sizeof(cw_expanditem));
	if (focusItem == 0)
		return;
	w_rect rect;
	w_control_get_bounds(W_CONTROL(expandbar), 0, &rect.sz);
	int height = rect.height;
	int updateY = 0;
	if (up) {
		if (focusItem->rect.y < 0) {
			updateY = WMIN(priv->yCurrentScroll, -focusItem->rect.y);
		}
	} else {
		int bandHeight = priv->band_height;
		int itemHeight = focusItem->rect.y + bandHeight;
		if (focusItem->expanded) {
			if (height >= bandHeight + focusItem->rect.height) {
				itemHeight += focusItem->rect.height;
			}
		}
		if (itemHeight > height) {
			updateY = height - itemHeight;
		}
	}
	if (updateY != 0) {
		priv->yCurrentScroll = WMAX(0, priv->yCurrentScroll - updateY);
		wuint64 style = w_widget_get_style(W_WIDGET(expandbar));
		if ((style & W_VSCROLL) != 0) {
			w_scrollbar scrollbar;
			w_widgetdata_init(W_WIDGETDATA(&scrollbar));
			w_scrollable_get_vertical_bar(W_SCROLLABLE(expandbar), &scrollbar);
			if (w_widgetdata_is_ok(W_WIDGETDATA(&scrollbar)) > 0) {
				w_scrollbar_set_selection(&scrollbar,
						WMIN(priv->yCurrentScroll, priv->yCurrentScroll));
			}
		}
		/*ScrollWindowEx(handle, 0, updateY, NULL, NULL, 0, NULL,
		 SW_SCROLLCHILDREN | SW_INVALIDATE);*/
		cw_expanditem *_item;
		int itemCount = w_array_get_count(_items, (void**) &_item);
		for (int i = 0; i < itemCount; i++) {
			_item->rect.y += updateY;
			_item++;
		}
	}
}
wresult cw_expandbar_set_imagelist(w_expandbar *expandbar,
		w_imagelist *imagelist) {
	if (imagelist != 0 && w_imagelist_is_ok(imagelist) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	priv->imagelist = imagelist;
	return W_TRUE;
}
void cw_expandbar_set_scrollbar(w_expandbar *expandbar) {
	wuint64 style = w_widget_get_style(W_WIDGET(expandbar));
	if ((style & W_VSCROLL) == 0)
		return;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	w_array *_items = priv->items;
	int itemCount = w_array_get_count(_items, 0);
	if (itemCount == 0)
		return;
	cw_expanditem *_item = w_array_get(_items, itemCount - 1,
			sizeof(cw_expanditem));
	w_rect rect;
	w_scrollable_get_client_area(W_SCROLLABLE(expandbar), &rect);
	int height = rect.height;
	int maxHeight = _item->rect.y + priv->band_height + priv->spacing;
	if (_item->expanded)
		maxHeight += _item->rect.height;

	//claim bottom free space
	if (priv->yCurrentScroll > 0 && height > maxHeight) {
		priv->yCurrentScroll = WMAX(0,
				priv->yCurrentScroll + maxHeight - height);
		cw_expandbar_layout_items(expandbar, 0);
	}
	maxHeight += priv->yCurrentScroll;

	w_scrollbar scrollbar;
	w_widgetdata_init(W_WIDGETDATA(&scrollbar));
	w_scrollable_get_vertical_bar(W_SCROLLABLE(expandbar), &scrollbar);
	if (w_widgetdata_is_ok(W_WIDGETDATA(&scrollbar)) > 0) {
		w_scrollbar_set_minimum(&scrollbar, 0);
		w_scrollbar_set_maximum(&scrollbar, maxHeight);
		w_scrollbar_set_thumb(&scrollbar, height);
		w_scrollbar_set_selection(&scrollbar,
				WMIN(priv->yCurrentScroll, maxHeight));
		w_scrollbar_set_visible(&scrollbar, W_TRUE);
	}
}
wresult cw_expandbar_set_spacing(w_expandbar *expandbar, int spacing) {
	if (spacing < 0)
		return W_FALSE;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	if (spacing == priv->spacing)
		return W_TRUE;
	priv->spacing = spacing;
	w_rect rect;
	w_scrollable_get_client_area(W_SCROLLABLE(expandbar), &rect);
	w_size sz;
	sz.width = WMAX(0, rect.width - spacing * 2);
	w_array *_items = priv->items;
	cw_expanditem *_item;
	int itemCount = w_array_get_count(_items, (void**) &_item);
	for (int i = 0; i < itemCount; i++) {
		if (_item->rect.width != sz.width) {
			sz.height = _item->rect.height;
			cw_expanditem_set_bounds(expandbar, _item, 0, &sz);
		}
		_item++;
	}
	cw_expandbar_layout_items(expandbar, 0);
	cw_expandbar_set_scrollbar(expandbar);
	w_control_redraw(W_CONTROL(expandbar), 0, W_TRUE);
	return W_TRUE;
}
void cw_expandbar_update_band_heigth(w_widget *expandbar) {
	w_graphics gc;
	w_fontmetrics fontMetrics;
	w_graphics_init(&gc);
	w_fontmetrics_init(&fontMetrics);
	w_control_get_graphics(W_CONTROL(expandbar), &gc);
	w_graphics_get_font_metrics(&gc, &fontMetrics);
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(expandbar));
	int fontHeight = w_fontmetrics_get_height(&fontMetrics) + 4;
	w_theme *theme;
	w_themedata data;
	w_widget_get_theme(expandbar, &theme);
	cw_expanditem_init_themedata(expandbar, 0, &data, &gc, 0);
	w_rect r;
	w_theme_get_bounds(theme, W_THEME_MASK_ALL, W_THEME_PART_CHEVRON, &data,
			&r);
	priv->band_height = WMAX(fontHeight + 4, r.height);
	w_fontmetrics_dispose(&fontMetrics);
	w_graphics_dispose(&gc);
}
/*
 * event
 */
void cw_expandbar_draw_item(w_widget *widget, cw_expanditem *_item,
		w_theme *theme, w_graphics *gc, w_rect *clipRect, int drawFocus) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	int headerHeight = priv->band_height;
	w_rect rect, result;
	rect.x = _item->rect.x;
	rect.y = _item->rect.y;
	rect.width = clipRect->width;
	rect.height = headerHeight;
	w_themedata data;
	cw_expanditem_init_themedata(widget, _item, &data, gc, &rect);
	if (drawFocus) {
		data.state |= W_THEME_STATE_FOCUSED;
	}
	w_theme_draw(theme, W_THEME_MASK_ALL, &data);
}
wresult cw_expandbar_paint(w_widget *widget, w_event_paint *e) {
	w_graphics *gc = e->gc;
	w_theme *theme;
	w_themedata data;
	w_rect rect;
	w_scrollable_get_client_area(W_SCROLLABLE(widget), &rect);
	w_widget_get_theme(widget, &theme);
	w_themedata_init(&data, gc, &rect);
	data.clazz = W_THEME_CLASS_EXPANDBAR;
	data.style = w_widget_get_style(widget);
	w_control_get_font(W_CONTROL(widget), &data.attr.font);
	w_theme_draw(theme, W_THEME_MASK_ALL, &data);
	w_toolkit *toolkit = w_widget_get_toolkit(widget);
	int drawFocus = W_FALSE;
	if (W_CONTROL(widget) == w_toolkit_get_focus_control(toolkit)) {
		drawFocus = W_TRUE;
	}
	w_font *font;
	w_control_get_font(W_CONTROL(widget), &font);
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	w_array *_items = priv->items;
	cw_expanditem *_item;
	int itemCount = w_array_get_count(_items, (void**) &_item);
	int focusItem = priv->focusItem;
	for (int i = 0; i < itemCount; i++) {
		cw_expandbar_draw_item(widget, _item, theme, gc, &rect,
				i == focusItem && drawFocus);
		_item++;
	}
	return W_FALSE;
}
wresult cw_expandbar_notifychange(w_widget *widget, w_event_notifychange *e) {
	if (e->id == W_NOTIFYCHANGE_FONT)
		cw_expandbar_update_band_heigth(widget);
	return W_TRUE;
}
/* mouse event */
wresult cw_expandbar_mousedown(w_widget *widget, w_event_mouse *e) {
	if (e->button != 1)
		return W_FALSE;
	int x = e->x;
	int y = e->y;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	w_array *_items = priv->items;
	cw_expanditem *_item, *_focusitem = 0;
	int itemCount = w_array_get_count(_items, (void**) &_item);
	int focusItem = priv->focusItem;
	if (focusItem > 0)
		_focusitem = w_array_get(_items, focusItem, sizeof(cw_expanditem));
	int bandHeight = priv->band_height;
	for (int i = 0; i < itemCount; i++) {
		int hover = cw_expanditem_is_hover(W_EXPANDBAR(widget), _item,
				bandHeight, x, y);
		if (hover && focusItem != i) {
			if (_focusitem != 0) {
				cw_expanditem_redraw(W_EXPANDBAR(widget), _focusitem, W_TRUE);
			}
			priv->focusItem = i;
			cw_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
			w_control_force_focus(W_CONTROL(widget));
			break;
		}
		_item++;
	}
	return W_FALSE;
}
wresult cw_expandbar_mouseup(w_widget *widget, w_event_mouse *e) {
	if (e->button != 1)
		return W_FALSE;
	int x = e->x;
	int y = e->y;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	int focusItem = priv->focusItem;
	if (focusItem == -1)
		return W_FALSE;
	cw_expanditem *_item;
	w_array *_items = priv->items;
	_item = w_array_get(_items, focusItem, sizeof(cw_expanditem));
	if (_item == 0)
		return W_FALSE;
	int bandHeight = priv->band_height;
	int hover = cw_expanditem_is_hover(W_EXPANDBAR(widget), _item, bandHeight,
			x, y);
	if (hover) {
		w_event_expand event;
		_w_item item;
		event.event.type =
				_item->expanded ? W_EVENT_ITEM_COLLAPSE : W_EVENT_ITEM_EXPAND;
		event.event.widget = widget;
		event.event.platform_event = _EVENT_PLATFORM(e);
		event.event.time = 0;
		event.event.data = 0;
		event.item = W_EXPANDITEM(&item);
		W_WIDGETDATA(&item)->clazz = 0;
		_W_ITEM(&item)->parent = widget;
		_W_ITEM(&item)->index = focusItem;
		_item->expanded = !_item->expanded;
		w_widget_post_event(widget, W_EVENT(&event), W_EVENT_SEND);
		cw_expandbar_show_item(W_EXPANDBAR(widget), focusItem, _item);
	}
	return W_FALSE;
}
wresult cw_expandbar_mouseexit(w_widget *widget, w_event_mouse *e) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	w_array *_items = priv->items;
	cw_expanditem *_item;
	int itemCount = w_array_get_count(_items, (void**) &_item);
	for (int i = 0; i < itemCount; i++) {
		if (_item->hover) {
			_item->hover = W_FALSE;
			cw_expanditem_redraw(W_EXPANDBAR(widget), _item, W_FALSE);
			break;
		}
		_item++;
	}
	return W_FALSE;
}
wresult cw_expandbar_mousemove(w_widget *widget, w_event_mouse *e) {
	int x = e->x;
	int y = e->y;
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	w_array *_items = priv->items;
	cw_expanditem *_item;
	int itemCount = w_array_get_count(_items, (void**) &_item);
	int bandHeight = priv->band_height;
	for (int i = 0; i < itemCount; i++) {
		int hover = cw_expanditem_is_hover(W_EXPANDBAR(widget), _item,
				bandHeight, x, y);
		if (_item->hover != hover) {
			_item->hover = hover;
			cw_expanditem_redraw(W_EXPANDBAR(widget), _item, W_FALSE);
		}
		_item++;
	}
	return W_FALSE;
}
wresult cw_expandbar_mousewhell(w_widget *widget, w_event_mouse *e) {
	return W_FALSE;
}
wresult cw_expandbar_focusin(w_widget *widget, w_event *e) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	int focusItem = priv->focusItem;
	if (focusItem != -1) {
		w_array *_items = priv->items;
		cw_expanditem *_item = w_array_get(_items, focusItem,
				sizeof(cw_expanditem));
		if (_item != 0) {
			cw_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
		}
	}
	return W_FALSE;
}
wresult cw_expandbar_focusout(w_widget *widget, w_event *e) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	int focusItem = priv->focusItem;
	if (focusItem != -1) {
		w_array *_items = priv->items;
		cw_expanditem *_item = w_array_get(_items, focusItem,
				sizeof(cw_expanditem));
		if (_item != 0) {
			cw_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
		}
	}
	return W_FALSE;
}
wresult cw_expandbar_keydown(w_widget *widget, w_event_key *e) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	int focusItem = priv->focusItem;
	if (focusItem == -1)
		return W_FALSE;
	w_array *_items = priv->items;
	cw_expanditem *_item = w_array_get(_items, focusItem,
			sizeof(cw_expanditem));
	if (_item == 0)
		return W_FALSE;
	w_event_expand event;
	_w_item item;
	switch (e->keycode) {
	case ' ':
	case W_KEYPAD_CR:
		event.event.type =
				_item->expanded ? W_EVENT_ITEM_COLLAPSE : W_EVENT_ITEM_EXPAND;
		event.event.widget = widget;
		event.event.platform_event = _EVENT_PLATFORM(e);
		event.event.time = 0;
		event.event.data = 0;
		event.item = W_EXPANDITEM(&item);
		W_WIDGETDATA(&item)->clazz = 0;
		_W_ITEM(&item)->parent = widget;
		_W_ITEM(&item)->index = focusItem;
		_item->expanded = !_item->expanded;
		w_widget_post_event(widget, W_EVENT(&event), W_EVENT_SEND);
		cw_expandbar_show_item(W_EXPANDBAR(widget), focusItem, _item);
		break;
	case W_ARROW_UP: {
		if (focusItem > 0) {
			cw_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
			focusItem -= 1;
			priv->focusItem = focusItem;
			_item = w_array_get(_items, focusItem, sizeof(cw_expanditem));
			cw_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
			cw_expandbar_show_focus(W_EXPANDBAR(widget), W_TRUE);
		}
		break;
	}
	case W_ARROW_DOWN: {
		if (focusItem < _items->count - 1) {
			cw_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
			focusItem += 1;
			priv->focusItem = focusItem;
			_item = w_array_get(_items, focusItem, sizeof(cw_expanditem));
			cw_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
			cw_expandbar_show_focus(W_EXPANDBAR(widget), W_TRUE);
		}
		break;
	}
	}
	return W_FALSE;
}
wresult cw_expandbar_resize(w_widget *widget, w_event *e) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	w_rect rect;
	w_scrollable_get_client_area(W_SCROLLABLE(widget), &rect);
	rect.width = WMAX(0, rect.width - priv->spacing * 2);
	cw_expanditem *_item;
	int itemCount = w_array_get_count(priv->items, (void**) &_item);
	for (int i = 0; i < itemCount; i++) {
		if (_item->rect.width != rect.width) {
			rect.height = _item->rect.height;
			cw_expanditem_set_bounds(W_EXPANDBAR(widget), _item, 0, &rect.sz);
		}
		_item++;
	}
	cw_expandbar_set_scrollbar(W_EXPANDBAR(widget));
	return W_FALSE;
}
wresult cw_expandbar_vscroll(w_widget *widget, w_event_scrollbar *e) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	priv->yCurrentScroll = w_scrollbar_get_selection(e->scrollbar);
	cw_expandbar_layout_items(W_EXPANDBAR(widget), 0);
	return W_FALSE;
}
wresult cw_expandbar_traverse(w_widget *widget, w_event_key *e) {
	cw_expandbar_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	return W_FALSE;
}
wresult cw_expandbar_post_event(w_widget *widget, w_event *e) {
	switch (e->type) {
	case W_EVENT_PAINT:
		return cw_expandbar_paint(widget, (w_event_paint*) e);
		break;
	case W_EVENT_NOTIFYCHANGE:
		return cw_expandbar_notifychange(widget, (w_event_notifychange*) e);
		break;
	case W_EVENT_COMPUTE_SIZE:
		return cw_expandbar_compute_size(widget, (w_event_compute_size*) e);
		break;
	case W_EVENT_KEYDOWN:
		return cw_expandbar_keydown(widget, (w_event_key*) e);
		break;
	case W_EVENT_FOCUSIN:
		return cw_expandbar_focusin(widget, e);
		break;
	case W_EVENT_FOCUSOUT:
		return cw_expandbar_focusout(widget, e);
		break;
	case W_EVENT_MOUSEDOWN:
		return cw_expandbar_mousedown(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_MOUSEUP:
		return cw_expandbar_mouseup(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_MOUSEEXIT:
		return cw_expandbar_mouseexit(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_MOUSEMOVE:
		return cw_expandbar_mousemove(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_MOUSEWHEEL:
		return cw_expandbar_mousewhell(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_RESIZE:
		return cw_expandbar_resize(widget, e);
		break;
	case W_EVENT_VSCROLL:
		return cw_expandbar_vscroll(widget, (w_event_scrollbar*) e);
		break;
	case W_EVENT_TRAVERSE:
		return cw_expandbar_traverse(widget, (w_event_key*) e);
		break;
	}
	return widget->clazz->parentClass->post_event(widget, e, W_EVENT_SEND);
}
void cw_expandbar_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_expandbar_class *clazz) {
	cw_composite_init_class(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_EXPANDBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_expandbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_expandbar);
	cw_composite_init_class_priv(toolkit, classId, W_COMPOSITE_CLASS(clazz),
			sizeof(struct cw_expandbar_priv));
	W_WIDGET_CLASS(clazz)->create = cw_expandbar_create;
	W_WIDGET_CLASS(clazz)->post_event = cw_expandbar_post_event;
	/*
	 * public function
	 */
	clazz->get_imagelist = cw_expandbar_get_imagelist;
	clazz->get_item = cw_expandbar_get_item;
	clazz->get_item_count = cw_expandbar_get_item_count;
	clazz->get_items = cw_expandbar_get_items;
	clazz->get_spacing = cw_expandbar_get_spacing;
	clazz->insert_item = cw_expandbar_insert_item;
	clazz->set_imagelist = cw_expandbar_set_imagelist;
	clazz->set_spacing = cw_expandbar_set_spacing;
	/*
	 * expanditem
	 */
	struct _w_expanditem_class *item = clazz->class_expanditem;
	W_WIDGETDATA_CLASS(item)->toolkit = W_WIDGET_CLASS(clazz)->toolkit;
	_w_item_class_init(W_ITEM_CLASS(item));
	W_ITEM_CLASS(item)->get_data = cw_expanditem_get_data;
	W_ITEM_CLASS(item)->get_text = cw_expanditem_get_text;
	W_ITEM_CLASS(item)->set_data = cw_expanditem_set_data;
	W_ITEM_CLASS(item)->set_text = cw_expanditem_set_text;
	item->get_control = cw_expanditem_get_control;
	item->get_expanded = cw_expanditem_get_expanded;
	item->get_header_height = cw_expanditem_get_header_height;
	item->get_height = cw_expanditem_get_height;
	item->set_control = cw_expanditem_set_control;
	item->set_expanded = cw_expanditem_set_expanded;
	item->set_height = cw_expanditem_set_height;
	item->set_image = cw_expanditem_set_image;
}

