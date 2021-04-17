/*
 * coolbar.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "coolbar.h"
#include "../widgets/toolkit.h"
#define MARGIN_WIDTH 4
#define GRABBER_WIDTH 2
#define MINIMUM_WIDTH ((2 * MARGIN_WIDTH) + GRABBER_WIDTH)

#define CHEVRON_HORIZONTAL_TRIM -1			//platform dependent values
#define CHEVRON_VERTICAL_TRIM -1
#define CHEVRON_LEFT_MARGIN 2
#define CHEVRON_IMAGE_WIDTH 8	//Width to draw the double arrow
#define ROW_SPACING 2
#define CLICK_DISTANCE 3
#define DEFAULT_COOLBAR_WIDTH 0
#define DEFAULT_COOLBAR_HEIGHT 0
/*
 * coolitem
 */
wresult _w_coolitem_compute_size(w_coolitem *coolitem, w_size *result,
		int wHint, int hHint) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	result->width = wHint, result->height = hHint;
	if (wHint == W_DEFAULT)
		result->width = 32;
	if (hHint == W_DEFAULT)
		result->height = 32;
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		result->height += MINIMUM_WIDTH;
	} else {
		result->width += MINIMUM_WIDTH;
	}
	return TRUE;
}
wresult _w_coolitem_get_bounds(w_coolitem *coolitem, w_rect *bounds) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	_w_coolitem_handle *_item = _W_COOLITEM(coolitem)->handle;
	_w_coolbar_fix_rectangle(W_COOLBAR(coolbar), bounds, &_item->itemBounds);
	return W_TRUE;
}
wresult _w_coolitem_get_control(w_coolitem *coolitem, w_control **control) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	_w_coolitem_handle *_item = _W_COOLITEM(coolitem)->handle;
	*control = _item->control;
	return W_TRUE;
}
wresult _w_coolitem_get_minimum_size(w_coolitem *coolitem, w_size *size) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	_w_coolitem_handle *_item = _W_COOLITEM(coolitem)->handle;
	_w_coolbar_fix_point(W_COOLBAR(coolbar), size, &_item->minimumSize);
	return W_TRUE;
}
wresult _w_coolitem_get_order(w_coolitem *coolitem) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	_w_coolitem_handle *_item = _W_COOLITEM(coolitem)->handle;
	return W_FALSE;
}
wresult _w_coolitem_get_preferred_size(w_coolitem *coolitem, w_size *size) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	_w_coolitem_handle *_item = _W_COOLITEM(coolitem)->handle;
	_w_coolbar_fix_point(W_COOLBAR(coolbar), size, &_item->preferredSize);
	return W_TRUE;
}
wresult _w_coolitem_get_size(w_coolitem *coolitem, w_size *size) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	_w_coolitem_handle *_item = _W_COOLITEM(coolitem)->handle;
	_w_coolbar_fix_point(W_COOLBAR(coolbar), size, &_item->itemBounds.sz);
	return W_TRUE;
}
wresult _w_coolitem_get_wrap_indice(w_coolitem *coolitem) {
	return W_FALSE;
}
void _w_coolitem_internal_internal_get_bounds(_w_coolitem_handle *item,
		w_rect *bounds) {
	memcpy(bounds, &item->itemBounds, sizeof(w_rect));
}
int _w_coolitem_internal_get_minimum_width(w_coolbar *coolbar,
		_w_coolitem_handle *_item) {
	int width = _item->minimumSize.width + MINIMUM_WIDTH;
	if ((_W_WIDGET(coolbar)->style & W_DROP_DOWN) != 0
			&& width < _item->preferredSize.width) {
		width += CHEVRON_IMAGE_WIDTH + CHEVRON_HORIZONTAL_TRIM
				+ CHEVRON_LEFT_MARGIN;
	}
	return width;
}
wresult _w_coolitem_set_control(w_coolitem *coolitem, w_control *control) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	_w_coolitem_handle *_item = _W_COOLITEM(coolitem)->handle;
	if (control != 0) {
		if (w_widget_is_ok(W_WIDGET(control)))
			return W_ERROR_INVALID_ARGUMENT;
		w_composite *parent;
		w_control_get_parent(control, &parent);
		if (parent != (w_composite*) coolbar)
			return W_ERROR_INVALID_PARENT;
	}
	_item->control = control;
	if (control != 0) {
		int controlWidth = _item->itemBounds.width - MINIMUM_WIDTH;
		if ((_W_WIDGET(coolbar)->style & W_DROP_DOWN) != 0
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
	}
	return W_TRUE;
}
void _w_coolitem_set_bounds(w_coolbar *coolbar, _w_coolitem_handle *_item,
		w_rect *bounds) {
	memcpy(&_item->itemBounds, bounds, sizeof(w_rect));
	w_rect rect, r;
	int y = bounds->y, height = bounds->height;
	if (_item->control != 0) {
		int controlWidth = bounds->width - MINIMUM_WIDTH;
		if ((_W_WIDGET(coolbar)->style & W_DROP_DOWN) != 0
				&& bounds->width < _item->preferredSize.width) {
			controlWidth -= CHEVRON_IMAGE_WIDTH + CHEVRON_HORIZONTAL_TRIM
					+ CHEVRON_LEFT_MARGIN;
		}
		if (height > _item->preferredSize.height) {
			y += (height - _item->preferredSize.height) / 2;
			height = _item->preferredSize.height;
		}
		r.x = bounds->x + MINIMUM_WIDTH;
		r.y = y;
		r.width = controlWidth;
		r.height = height;
		_w_coolbar_fix_rectangle(coolbar, &rect, &r);
		w_control_set_bounds(_item->control, &rect.pt, &rect.sz);
	}
	_w_coolitem_update_chevron(coolbar, _item);
}
wresult _w_coolitem_set_layout(w_coolitem *coolitem, int order, int wrapindices,
		w_size *sizes) {
	return W_FALSE;
}
wresult _w_coolitem_set_minimum_size(w_coolitem *coolitem, w_size *size) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	_w_coolitem_handle *_item = _W_COOLITEM(coolitem)->handle;
	_w_coolbar_fix_point(W_COOLBAR(coolbar), &_item->minimumSize, size);
	return W_TRUE;
}
wresult _w_coolitem_set_order(w_coolitem *coolitem, int order) {
	return W_FALSE;
}
wresult _w_coolitem_set_preferred_size(w_coolitem *coolitem, w_size *size) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	_w_coolitem_handle *_item = _W_COOLITEM(coolitem)->handle;
	_w_coolbar_fix_point(W_COOLBAR(coolbar), &_item->preferredSize, size);
	_item->preferredSize.width = WMAX(_item->preferredSize.width,
			MINIMUM_WIDTH);
	return W_TRUE;
}
wresult _w_coolitem_set_size(w_coolitem *coolitem, w_size *size) {
	w_widget *coolbar = _W_ITEM(coolitem)->parent;
	_w_coolitem_handle *_item = _W_COOLITEM(coolitem)->handle;
	w_size point, sz;
	_w_coolbar_fix_point(W_COOLBAR(coolbar), &point, size);
	int width = WMAX(point.width, _item->minimumSize.width + MINIMUM_WIDTH);
	int height = point.height;
	if (!_item->ideal) {
		_item->preferredSize.width = width;
		_item->preferredSize.height = height;
	}
	_item->itemBounds.width = _item->requestedWidth = width;
	_item->itemBounds.height = height;
	if (_item->control != 0) {
		int controlWidth = width - MINIMUM_WIDTH;
		if ((_W_WIDGET(coolbar)->style & W_DROP_DOWN) != 0
				&& width < _item->preferredSize.width) {
			controlWidth -= CHEVRON_IMAGE_WIDTH + CHEVRON_HORIZONTAL_TRIM
					+ CHEVRON_LEFT_MARGIN;
		}
		sz.width = controlWidth;
		sz.height = height;
		_w_coolbar_fix_point(W_COOLBAR(coolbar), &point, &sz);
		w_control_set_bounds(_item->control, 0, &point);
	}
	_w_coolbar_relayout(W_COOLBAR(coolbar));
	_w_coolitem_update_chevron(W_COOLBAR(coolbar), _item);
	return W_TRUE;
}
wresult _w_coolitem_set_wrap_indice(w_coolitem *coolitem, int wrap_indice) {
	return W_FALSE;
}
void _w_coolitem_update_chevron(w_coolbar *coolbar, _w_coolitem_handle *_item) {

}
/*
 * coolbar
 */
wuint64 _w_coolbar_check_style(w_widget *control, wuint64 style) {
	style |= W_NO_FOCUS;
	return (style | W_NO_REDRAW_RESIZE) & ~(W_VSCROLL | W_HSCROLL);
}
wresult _w_coolbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	w_size size, sz;
	sz.width = 0, sz.height = 0;
	_w_coolbar_wrap_items(W_COOLBAR(widget),
			(_W_WIDGET(widget)->style & W_VERTICAL) != 0 ? e->hHint : e->wHint);
	int flat = (_W_WIDGET(widget)->style & W_FLAT) != 0;
	_w_coolitem_handle *_item;
	_w_coolitem_handles *_items = _W_COOLBAR(widget)->items;
	if (_items != 0) {
		int i = 0, row = 0;
		int count = _items->count;
		while (i < count) {
			int rowWidth = 0, rowHeight = 0;
			while (i < count) {
				_item = &_items->items[i];
				rowWidth += _item->preferredSize.width;
				rowHeight = WMAX(rowHeight, _item->preferredSize.height);
				i++;
				if (_item->newrow)
					break;
			}
			sz.height += rowHeight;
			if (!flat && row > 0)
				sz.height += ROW_SPACING;
			sz.width = WMAX(sz.width, rowWidth);
			row++;
		}
	}
	_w_coolbar_wrap_items(W_COOLBAR(widget),
			_w_coolbar_get_width(W_COOLBAR(widget)));
	if (sz.width == 0)
		sz.width = DEFAULT_COOLBAR_WIDTH;
	if (sz.height == 0)
		sz.height = DEFAULT_COOLBAR_HEIGHT;
	_w_coolbar_fix_point(W_COOLBAR(widget), &size, &sz);
	if (e->wHint != W_DEFAULT)
		size.width = e->wHint;
	if (e->hHint != W_DEFAULT)
		size.height = e->hHint;
	w_rect trim, rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = size.width;
	rect.height = size.height;
	w_scrollable_compute_trim(W_SCROLLABLE(widget), &trim, &rect);
	e->size->width = trim.width;
	e->size->height = trim.height;
	return TRUE;
}
wresult _w_coolbar_create_handle(w_widget *widget, _w_control_priv *priv) {
	wresult result = _w_composite_create_handle(widget, priv);
	if (result > 0) {
		w_toolkit *toolkit = w_widget_get_toolkit(widget);
		int cursor;
		if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
			_W_WIDGET(widget)->style |= W_VERTICAL;
			cursor = W_CURSOR_SIZENS;
		} else {
			_W_WIDGET(widget)->style |= W_HORIZONTAL;
			cursor = W_CURSOR_SIZEWE;
		}
		_w_coolbar *_coolbar = _W_COOLBAR(widget);
		_coolbar->hoverCursor = w_toolkit_get_system_cursor(toolkit, cursor);
		_coolbar->dragCursor = w_toolkit_get_system_cursor(toolkit,
				W_CURSOR_SIZEALL);
		_coolbar->items = 0;
	}
	return result;
}
void _w_coolbar_draw_widget(w_control *control, w_graphics *gc,
		_w_control_priv *priv) {
	_w_coolitem_handle *_item;
	_w_coolitem_handles *_items = _W_COOLBAR(control)->items;
	if (_items == 0)
		return;
	w_toolkit *toolkit = w_widget_get_toolkit(W_WIDGET(control));
	w_color shadowColor = w_toolkit_get_system_color(toolkit,
			W_COLOR_WIDGET_NORMAL_SHADOW);
	w_color highlightColor = w_toolkit_get_system_color(toolkit,
			W_COLOR_WIDGET_HIGHLIGHT_SHADOW);
	wuint64 style = _W_WIDGET(control)->style;
	gboolean vertical = (style & W_VERTICAL) != 0;
	gboolean flat = (style & W_FLAT) != 0;
	int stopX = _w_coolbar_get_width(W_COOLBAR(control));
	w_rect rect, bounds, clipping, r;
	w_graphics_get_clipping_rect(gc, &clipping);
	int itemCount = _items->count;
	int i = 0, row = 0;
	while (i < itemCount) {
		memset(&bounds, 0, sizeof(w_rect));
		while (i < itemCount) {
			_item = &_items->items[i];
			_w_coolitem_internal_internal_get_bounds(_item, &bounds);
			_w_coolbar_fix_rectangle(W_COOLBAR(control), &rect, &bounds);
			//if (!w_rect_intersects(&clipping,&rect)) continue;
			gboolean nativeGripper = W_FALSE;

			/* Draw gripper. */
			if (!_W_COOLBAR(control)->isLocked) {
				r.x = bounds.x;
				r.y = bounds.y;
				r.width = MINIMUM_WIDTH;
				r.height = bounds.height;
				_w_coolbar_fix_rectangle(W_COOLBAR(control), &rect, &r);
				if (!flat)
					nativeGripper = _w_control_draw_draw_gripper(control, gc,
							&rect, vertical, priv);
				if (!nativeGripper) {
					int grabberTrim = 2;
					int grabberHeight = bounds.height - (2 * grabberTrim) - 1;
					w_graphics_set_foreground(gc, shadowColor);
					r.x = bounds.x + MARGIN_WIDTH;
					r.y = bounds.y + grabberTrim;
					r.width = 2;
					r.height = grabberHeight;
					_w_coolbar_fix_rectangle(W_COOLBAR(control), &rect, &r);
					w_graphics_draw_rectangle(gc, &rect);
					w_graphics_set_foreground(gc, highlightColor);
					r.x = bounds.x + MARGIN_WIDTH;
					r.y = bounds.y + grabberTrim + 1;
					r.width = bounds.x + MARGIN_WIDTH;
					r.height = bounds.y + grabberTrim + grabberHeight - 1;
					_w_coolbar_fix_rectangle(W_COOLBAR(control), &rect, &r);
					w_graphics_draw_line(gc, &rect.pt, (w_point*) &rect.sz);
					r.x = bounds.x + MARGIN_WIDTH;
					r.y = bounds.y + grabberTrim;
					r.width = bounds.x + MARGIN_WIDTH + 1;
					r.height = bounds.y + grabberTrim;
					_w_coolbar_fix_rectangle(W_COOLBAR(control), &rect, &r);
					w_graphics_draw_line(gc, &rect.pt, (w_point*) &rect.sz);
				}
			}

			/* Draw separator. */
			if (!flat && !nativeGripper && i != 0) {
				w_graphics_set_foreground(gc, shadowColor);
				r.x = bounds.x;
				r.y = bounds.y;
				r.width = bounds.x;
				r.height = bounds.y + bounds.height - 1;
				_w_coolbar_fix_rectangle(W_COOLBAR(control), &rect, &r);
				w_graphics_draw_line(gc, &rect.pt, (w_point*) &rect.sz);
				w_graphics_set_foreground(gc, highlightColor);
				r.x = bounds.x + 1;
				r.y = bounds.y;
				r.width = bounds.x + 1;
				r.height = bounds.y + bounds.height - 1;
				_w_coolbar_fix_rectangle(W_COOLBAR(control), &rect, &r);
				w_graphics_draw_line(gc, &rect.pt, (w_point*) &rect.sz);
			}
			i++;
			if (_item->newrow)
				break;
		}
		if (!flat) {
			/* Draw row separator. */
			int separatorY = bounds.y + bounds.height;
			w_graphics_set_foreground(gc, shadowColor);
			r.x = 0;
			r.y = separatorY;
			r.width = stopX;
			r.height = separatorY;
			_w_coolbar_fix_rectangle(W_COOLBAR(control), &rect, &r);
			w_graphics_draw_line(gc, &rect.pt, (w_point*) &rect.sz);
			w_graphics_set_foreground(gc, highlightColor);
			r.x = 0;
			r.y = separatorY + 1;
			r.width = stopX;
			r.height = separatorY + 1;
			_w_coolbar_fix_rectangle(W_COOLBAR(control), &rect, &r);
			w_graphics_draw_line(gc, &rect.pt, (w_point*) &rect.sz);
		}
	}
}
void _w_coolbar_fix_point(w_coolbar *coolbar, w_size *result, w_size *pt) {
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		result->width = pt->height;
		result->height = pt->width;
	} else {
		result->width = pt->width;
		result->height = pt->height;
	}
}
void _w_coolbar_fix_rectangle(w_coolbar *coolbar, w_rect *result,
		w_rect *rect) {
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
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
wresult _w_coolbar_get_item(w_coolbar *coolbar, int index, w_coolitem *item) {
	_w_coolitem_handles *_items = _W_COOLBAR(coolbar)->items;
	if (index >= 0 && index < _items->count) {
		_W_WIDGETDATA(item)->clazz = _W_COOLBAR_GET_ITEM_CLASS(coolbar);
		_W_ITEM(item)->parent = W_WIDGET(coolbar);
		_W_ITEM(item)->index = index;
		_W_COOLITEM(item)->handle = &_items->items[index];
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _w_coolbar_get_item_count(w_coolbar *coolbar) {
	_w_coolitem_handles *_items = _W_COOLBAR(coolbar)->items;
	if (_items != 0) {
		return _items->count;
	} else
		return 0;
}
wresult _w_coolbar_get_items(w_coolbar *coolbar, w_iterator *items) {
	return W_FALSE;
}
wresult _w_coolbar_get_locked(w_coolbar *coolbar) {
	return _W_COOLBAR(coolbar)->isLocked;
}
int _w_coolbar_get_width(w_coolbar *coolbar) {
	w_size size;
	w_control_get_bounds(W_CONTROL(coolbar), 0, &size);
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0)
		return size.height;
	return size.width;
}
wresult _w_coolbar_insert_item(w_coolbar *coolbar, w_coolitem *item, int style,
		int index) {
	int itemCount = _w_coolbar_get_item_count(coolbar), row = 0;
	if (!(0 <= index && index <= itemCount)) {
		index = itemCount;
		//return W_ERROR_INVALID_RANGE;
	}
	_w_coolitem_handle *_item;
	_w_coolitem_handles *_items = _W_COOLBAR(coolbar)->items;
	if (_items == 0) {
		const int _size = sizeof(_w_coolitem_handles)
				+ _W_COOLBAR_GROW * sizeof(_w_coolitem_handle);
		_items = malloc(_size);
		if (_items == 0)
			return W_ERROR_NO_MEMORY;
		_items->alloc = _W_COOLBAR_GROW;
		_items->count = 1;
		_item = &_items->items[0];
		_W_COOLBAR(coolbar)->items = _items;
	} else {
		if (_items->alloc <= _items->count) {
			const int _size = sizeof(_w_coolitem_handles)
					+ (_W_COOLBAR(coolbar)->items->alloc + _W_COOLBAR_GROW)
							* sizeof(_w_coolitem_handle);
			_w_coolitem_handles *_items = realloc(_items, _size);
			if (_items == 0)
				return W_ERROR_NO_MEMORY;
			_W_COOLBAR(coolbar)->items = _items;
			_items->alloc += _W_COOLBAR_GROW;
		}
		if (index != itemCount) {
			for (int i = itemCount; i >= index; i--) {
				memcpy(&_items[i + 1], &_items[i], sizeof(_w_coolitem_handle));
			}
		}
		_items->count++;
		_item = &_items->items[index];
	}
	memset(_item, 0, sizeof(_w_coolitem_handle));
	_item->requestedWidth = MINIMUM_WIDTH;
	_w_coolbar_layout_items(coolbar);
	if (item != 0) {
		_W_WIDGETDATA(item)->clazz = _W_COOLBAR_GET_ITEM_CLASS(coolbar);
		_W_ITEM(item)->parent = W_WIDGET(coolbar);
		_W_ITEM(item)->index = index;
		_W_COOLITEM(item)->handle = _item;
	}
	return TRUE;
}
void _w_coolbar_internal_redraw(w_coolbar *coolbar, int x, int y, int width,
		int height) {
	w_rect rect;
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		rect.x = y;
		rect.y = x;
	} else {
		rect.x = x;
		rect.y = y;
	}
	rect.width = width;
	rect.height = height;
	w_control_redraw(W_CONTROL(coolbar), &rect, FALSE);
}
wresult _w_coolbar_layout_items(w_coolbar *coolbar) {
	int y = 0, width;
	w_rect rect;
	w_scrollable_get_client_area(W_SCROLLABLE(coolbar), &rect);
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		width = rect.height;
	} else {
		width = rect.width;
	}
	_w_coolbar_wrap_items(coolbar, width);
	int rowSpacing =
			(_W_WIDGET(coolbar)->style & W_FLAT) != 0 ? 0 : ROW_SPACING;
	_w_coolitem_handle *_item;
	_w_coolitem_handles *_items = _W_COOLBAR(coolbar)->items;
	if (_items != 0) {
		int i = 0, row = 0, row_start, count;
		int itemcount = _items->count;
		while (i < itemcount) {
			int x = 0;
			row_start = i;

			/* determine the height and the available width for the row */
			int rowHeight = 0;
			int available = width;
			count = 0;
			while (i < itemcount) {
				_item = &_items->items[i];
				rowHeight = WMAX(rowHeight, _item->preferredSize.height);
				available -= _w_coolitem_internal_get_minimum_width(coolbar,
						_item);
				i++;
				count++;
				if (_item->newrow)
					break;
			}
			if (row > 0)
				y += rowSpacing;

			/* lay the items out */
			i = row_start;
			while (i < itemcount) {
				_item = &_items->items[i];
				int newWidth = available
						+ _w_coolitem_internal_get_minimum_width(coolbar,
								_item);
				if (i + 1 < count) {
					newWidth = WMIN(newWidth, _item->requestedWidth);
					available -= (newWidth
							- _w_coolitem_internal_get_minimum_width(coolbar,
									_item));
				}
				w_rect oldBounds, newBounds, damage;
				memcpy(&oldBounds, &_item->itemBounds, sizeof(w_rect));
				newBounds.x = x;
				newBounds.y = y;
				newBounds.width = newWidth;
				newBounds.height = rowHeight;
				if (!w_rect_equals(&oldBounds, &newBounds)) {
					_w_coolitem_set_bounds(coolbar, _item, &newBounds);

					memset(&damage, 0, sizeof(rect));
					/* Cases are in descending order from most area to redraw to least. */
					if (oldBounds.y != newBounds.y) {
						damage = newBounds;
						w_rect_add(&damage, &oldBounds);
						/* Redraw the row separator as well. */
						damage.y -= rowSpacing;
						damage.height += 2 * rowSpacing;
					} else if (oldBounds.height != newBounds.height) {
						/*
						 * Draw from the bottom of the gripper to the bottom of the new area.
						 * (Bottom of the gripper is -3 from the bottom of the item).
						 */
						damage.y = newBounds.y
								+ WMIN(oldBounds.height, newBounds.height) - 3;
						damage.height = newBounds.y + newBounds.height
								+ rowSpacing;
						damage.x = oldBounds.x - MARGIN_WIDTH;
						damage.width = oldBounds.width + MARGIN_WIDTH;
					} else if (oldBounds.x != newBounds.x) {
						/* Redraw only the difference between the separators. */
						damage.x = WMIN(oldBounds.x, newBounds.x);
						damage.width = abs(
								oldBounds.x - newBounds.x) + MINIMUM_WIDTH;
						damage.y = oldBounds.y;
						damage.height = oldBounds.height;
					}
					_w_coolbar_internal_redraw(coolbar, damage.x, damage.y,
							damage.width, damage.height);
				}
				x += newWidth;
				i++;
				if (_item->newrow)
					break;
			}
			y += rowHeight;
		}
	}
	return y;
}
void _w_coolbar_relayout(w_coolbar *coolbar) {
	w_size size;
	w_rect trim, rect;
	w_control_get_bounds(W_CONTROL(coolbar), 0, &size);
	int height = _w_coolbar_layout_items(coolbar);
	memset(&rect, 0, sizeof(w_rect));
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		rect.width = height;
		w_scrollable_compute_trim(W_SCROLLABLE(coolbar), &trim, &rect);
		if (height != size.width) {
			size.width = trim.width;
			_w_control_set_bounds(W_CONTROL(coolbar), 0, &size);
		}
	} else {
		rect.height = height;
		w_scrollable_compute_trim(W_SCROLLABLE(coolbar), &trim, &rect);
		if (height != size.height) {
			size.height = trim.height;
			_w_control_set_bounds(W_CONTROL(coolbar), 0, &size);
		}
	}
}
wresult _w_coolbar_set_locked(w_coolbar *coolbar, int locked) {
	int _locked = locked != 0;
	if (_W_COOLBAR(coolbar)->isLocked != _locked) {
		w_control_redraw(W_CONTROL(coolbar), 0, W_TRUE);
	}
	_W_COOLBAR(coolbar)->isLocked = _locked;
	return W_TRUE;
}
void _w_coolbar_wrap_items(w_coolbar *coolbar, int maxWidth) {
	_w_coolitem_handle *_item;
	_w_coolitem_handles *_items = _W_COOLBAR(coolbar)->items;
	if (_items == 0)
		return;
	int itemCount = _items->count;
	if (itemCount <= 1)
		return;
	int start = 0;
	int rowCount = 0, rowWidth = 0;
	start = 0;
	for (int i = 0; i < itemCount; i++) {
		_item = &_items->items[i];
		int itemWidth = _w_coolitem_internal_get_minimum_width(coolbar, _item);
		if ((i > 0 && _item->wrap)
				|| (maxWidth != W_DEFAULT && rowWidth + itemWidth > maxWidth)) {
			if (i == start) {
				start = i + 1;
				rowWidth = 0;
			} else {
				int count = i - start;
				start = i;
				rowWidth = itemWidth;
			}
			_item->newrow = TRUE;
			rowCount++;
		} else {
			_item->newrow = FALSE;
			rowWidth += itemWidth;
		}
	}
}

void _w_coolbar_class_init(struct _w_coolbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_coolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_coolbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_coolbar);
	/*
	 * public function
	 */
	clazz->get_item = _w_coolbar_get_item;
	clazz->get_item_count = _w_coolbar_get_item_count;
	clazz->get_items = _w_coolbar_get_items;
	clazz->get_locked = _w_coolbar_get_locked;
	clazz->insert_item = _w_coolbar_insert_item;
	clazz->set_locked = _w_coolbar_set_locked;
	/*
	 * item
	 */
	struct _w_coolitem_class *item = clazz->class_coolitem;
	_w_item_class_init(W_ITEM_CLASS(item));
	item->compute_size = _w_coolitem_compute_size;
	item->get_bounds = _w_coolitem_get_bounds;
	item->get_control = _w_coolitem_get_control;
	item->get_minimum_size = _w_coolitem_get_minimum_size;
	item->get_order = _w_coolitem_get_order;
	item->get_preferred_size = _w_coolitem_get_preferred_size;
	item->get_size = _w_coolitem_get_size;
	item->get_wrap_indice = _w_coolitem_get_wrap_indice;
	item->set_control = _w_coolitem_set_control;
	item->set_layout = _w_coolitem_set_layout;
	item->set_minimum_size = _w_coolitem_set_minimum_size;
	item->set_order = _w_coolitem_set_order;
	item->set_preferred_size = _w_coolitem_set_preferred_size;
	item->set_size = _w_coolitem_set_size;
	item->set_wrap_indice = _w_coolitem_set_wrap_indice;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.compute_size = _w_coolbar_compute_size;
	priv->widget.check_style = _w_coolbar_check_style;
	priv->widget.create_handle = _w_coolbar_create_handle;
	priv->draw_widget = _w_coolbar_draw_widget;

}
