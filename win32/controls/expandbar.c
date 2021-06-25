/*
 * expandbar.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "expandbar.h"
#include "../widgets/toolkit.h"
#include <vsstyle.h>
#define TEXT_INSET 6
#define BORDER 1
#define CHEVRON_SIZE 24
/*
 * expand item
 */
wresult _w_expanditem_get_data(w_item *item, void **data) {
	w_widget *parent = _W_ITEM(item)->parent;
	int index = _W_ITEM(item)->index;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		*data = _items->items[index].userdata;
		return W_TRUE;
	}
	*data = 0;
	return W_FALSE;
}
wresult _w_expanditem_get_index(w_item *item) {
	return _W_ITEM(item)->index;
}
wresult _w_expanditem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		_w_expanditem_handle *_item = &_items->items[index];
		result = _win_text_set_0(_item->text, -1, alloc, user_data, enc);
	}
	return result;
}
wresult _w_expanditem_set_data(w_item *item, void *data) {
	w_widget *parent = _W_ITEM(item)->parent;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		_items->items[index].userdata = data;
		result = W_TRUE;
	}
	return result;
}
wresult _w_expanditem_set_text(w_item *item, const char *text, int length,
		int enc) {
	w_widget *parent = _W_ITEM(item)->parent;
	int index = _W_ITEM(item)->index;
	wresult result = W_FALSE;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		_w_expanditem_handle *_item = &_items->items[index];
		result = _win_text_copy(&_item->text, text, length, enc);
		_w_expanditem_redraw(W_EXPANDBAR(parent), _item, W_TRUE);
	}
	return result;
}
wresult _w_expanditem_get_control(w_expanditem *expanditem,
		w_control **control) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		*control = _items->items[index].control;
		return W_TRUE;
	}
	*control = 0;
	return W_FALSE;
}
wresult _w_expanditem_get_expanded(w_expanditem *expanditem) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		return _items->items[index].expanded;
	}
	return W_FALSE;
}
wresult _w_expanditem_get_header_height(w_expanditem *expanditem) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		int bandHeight = _w_expandbar_get_band_height(W_EXPANDBAR(parent));
		w_imagelist *imagelist = _W_EXPANDBAR(parent)->imagelist;
		if (imagelist != 0) {
			w_size sz;
			w_imagelist_get_size(imagelist, &sz);
			bandHeight = WMAX(bandHeight, sz.height);
		}
		return bandHeight;
	}
	return 0;
}
wresult _w_expanditem_get_height(w_expanditem *expanditem) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		return _items->items[index].rect.height;
	}
	return 0;
}
wresult _w_expanditem_get_image(w_expanditem *expanditem) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		return _items->items[index].image - 1;
	}
	return -1;
}
void _w_expanditem_redraw(w_expandbar *expandbar, _w_expanditem_handle *_item,
		int all) {
	HWND parentHandle = _W_WIDGET(expandbar)->handle;
	int headerHeight = _w_expandbar_get_band_height(expandbar);
	RECT rect;
	int left =
			all ? _item->rect.x : _item->rect.x + _item->rect.width
							- headerHeight;
	SetRect(&rect, left, _item->rect.y, _item->rect.x + _item->rect.width,
			_item->rect.y + headerHeight);
	InvalidateRect(parentHandle, &rect, TRUE);
	w_size imageSize;
	imageSize.width = 0;
	imageSize.height = 0;
	w_imagelist *imagelist = _W_EXPANDBAR(expandbar)->imagelist;
	if (imagelist != 0) {
		w_imagelist_get_size(imagelist, &imageSize);
	}
	if (imageSize.height > headerHeight) {
		SetRect(&rect, _item->rect.x + TEXT_INSET,
				_item->rect.y + headerHeight - imageSize.height,
				_item->rect.x + TEXT_INSET + imageSize.width, _item->rect.y);
		InvalidateRect(parentHandle, &rect, TRUE);
	}
	if (!_w_expandbar_is_app_themed(expandbar)) {
		SetRect(&rect, _item->rect.x, _item->rect.y + headerHeight,
				_item->rect.x + _item->rect.width,
				_item->rect.y + headerHeight + _item->rect.height + 1);
		InvalidateRect(parentHandle, &rect, TRUE);
	}
}
int _w_expanditem_get_preferred_width(w_expandbar *expandbar,
		_w_expanditem_handle *_item, HTHEME hTheme, HDC hDC, int imageWidth) {
	int width = TEXT_INSET * 2 + CHEVRON_SIZE;
	if (imageWidth != 0) {
		width += TEXT_INSET + imageWidth;
	}
	if (_item->text != 0) {
		WCHAR *str;
		int newlength;
		RECT rect;
		_win_text_fix(_item->text, -1, W_ENCODING_UTF8, &str, &newlength);
		if (hTheme != 0) {
			GetThemeTextExtent(hTheme, hDC, EBP_NORMALGROUPHEAD, 0, str,
					newlength, DT_SINGLELINE, NULL, &rect);
		} else {
			DrawTextW(hDC, str, newlength, &rect, DT_CALCRECT);
		}
		_win_text_free(_item->text, str, newlength);
		width += (rect.right - rect.left);
	}
	return width;
}
int _w_expanditem_is_hover(w_expandbar *expandbar, _w_expanditem_handle *_item,
		int bandHeight, int x, int y) {
	return _item->rect.x < x && x < (_item->rect.x + _item->rect.width)
			&& _item->rect.y < y && y < (_item->rect.y + bandHeight);
}
wresult _w_expanditem_set_control(w_expanditem *expanditem,
		w_control *control) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	if (control != 0) {
		if (w_widget_is_ok(W_WIDGET(control)) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (_W_CONTROL(control)->parent != W_COMPOSITE(parent))
			return W_ERROR_INVALID_PARENT;
	}
	int index = _W_ITEM(expanditem)->index;
	wresult result = W_FALSE;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		_w_expanditem_handle *_item = &_items->items[index];
		_item->control = control;
		if (control != 0) {
			int headerHeight = _w_expandbar_get_band_height(
					W_EXPANDBAR(parent));
			w_control_set_visible(control, _item->expanded);
			w_rect r;
			if (!_w_expandbar_is_app_themed(W_EXPANDBAR(parent))) {
				r.x = _item->rect.x + BORDER;
				r.y = _item->rect.y + headerHeight;
				r.width = WMAX(0, _item->rect.width - BORDER * 2);
				r.height = WMAX(0, _item->rect.height - BORDER);
			} else {
				r.x = _item->rect.x;
				r.y = _item->rect.y + headerHeight;
				r.width = _item->rect.width;
				r.height = _item->rect.height;
			}
			w_control_set_bounds(control, &r.pt, &r.sz);
		}
		result = W_TRUE;
	}
	return result;
}
void _w_expanditem_set_bounds(w_expandbar *expandbar,
		_w_expanditem_handle *_item, w_point *location, w_size *size) {
	_w_expanditem_redraw(expandbar, _item, W_TRUE);
	int headerHeight = _w_expandbar_get_band_height(expandbar);
	w_point *newlocation = 0, location_tmp;
	w_size *newsize = 0, size_tmp;
	if (location) {
		_item->rect.x = location->x;
		_item->rect.y = location->y;
		w_imagelist *imagelist = _W_EXPANDBAR(expandbar)->imagelist;
		if (imagelist != 0) {
			w_imagelist_get_size(imagelist, &size_tmp);
			if (size_tmp.height > headerHeight) {
				_item->rect.y += (size_tmp.height - headerHeight);
			}
		}
		_w_expanditem_redraw(expandbar, _item, W_TRUE);
		newlocation = &location_tmp;
		location_tmp.x = location->x;
		location_tmp.y = _item->rect.y;
	}
	if (size) {
		_item->rect.width = size->width;
		_item->rect.height = size->height;
		_w_expanditem_redraw(expandbar, _item, W_TRUE);
		newsize = &size_tmp;
		size_tmp.width = size->width;
		size_tmp.height = size->height;
	}
	w_control *control = _item->control;
	if (w_widget_is_ok(W_WIDGET(control))) {
		if (!_w_expandbar_is_app_themed(expandbar)) {
			location_tmp.x += BORDER;
			size_tmp.width = WMAX(0, size_tmp.width - BORDER * 2);
			size_tmp.height = WMAX(0, size_tmp.height - BORDER);
		}
		location_tmp.y += headerHeight;
		w_control_set_bounds(control, newlocation, newsize);
	}
}
wresult _w_expanditem_set_expanded(w_expanditem *expanditem, int expanded) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	wresult result = W_FALSE;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		_w_expanditem_handle *_item = &_items->items[index];
		_item->expanded = expanded;
		_w_expandbar_show_item(W_EXPANDBAR(parent), index, _item);
		result = W_TRUE;
	}
	return result;
}
wresult _w_expanditem_set_height(w_expanditem *expanditem, int height) {
	if (height < 0)
		return W_FALSE;
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	wresult result = W_FALSE;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		_w_expanditem_handle *_item = &_items->items[index];
		w_size sz;
		sz.width = _item->rect.width;
		sz.height = height;
		_w_expanditem_set_bounds(W_EXPANDBAR(parent), _item, 0, &sz);
		if (_item->expanded) {
			_w_expandbar_layout_items(W_EXPANDBAR(parent), index + 1);
			_w_expandbar_set_scrollbar(W_EXPANDBAR(parent));
		}
		result = W_TRUE;
	}
	return result;
}
wresult _w_expanditem_set_image(w_expanditem *expanditem, int image) {
	w_widget *parent = _W_ITEM(expanditem)->parent;
	int index = _W_ITEM(expanditem)->index;
	wresult result = W_FALSE;
	_w_expanditem_handles *_items = _W_EXPANDBAR(parent)->items;
	if (_items != 0 && index < _items->count) {
		_w_expanditem_handle *_item = &_items->items[index];
		if (image < 0)
			image = -1;
		_item->image = image + 1;
		_w_expanditem_redraw(W_EXPANDBAR(parent), _item, W_TRUE);
		result = W_TRUE;
	}
	return result;
}
/*
 * expand bar
 */
wuint64 _w_expandbar_check_style(w_widget *widget, wuint64 style) {
	style &= ~W_HSCROLL;
	return style | W_NO_BACKGROUND;
}
wresult _w_expandbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	w_rect rect;
	memset(&rect, 0, sizeof(rect));
	if (wHint == W_DEFAULT || hHint == W_DEFAULT) {
		HWND handle = _W_WIDGET(widget)->handle;
		_w_expanditem_handles *items = _W_EXPANDBAR(widget)->items;
		int itemCount = 0;
		if (items != 0)
			itemCount = items->count;
		if (itemCount > 0) {
			HDC hDC = GetDC(handle);
			HTHEME hTheme = 0;
			if (_w_expandbar_is_app_themed(W_EXPANDBAR(widget))) {
				hTheme = OpenThemeData(NULL, L"EXPLORERBAR");
			}
			HFONT hCurrentFont = 0, oldFont = 0;
			if (hTheme == 0) {
				if (_W_EXPANDBAR(widget)->hFont != 0) {
					hCurrentFont = _W_EXPANDBAR(widget)->hFont;
				} else {
					NONCLIENTMETRICSW info;
					info.cbSize = sizeof(info);
					if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &info,
							0)) {
						hCurrentFont = CreateFontIndirectW(&info.lfCaptionFont);
					}
				}
				if (hCurrentFont != 0) {
					oldFont = SelectObject(hDC, hCurrentFont);
				}
			}
			int spacing = _W_EXPANDBAR(widget)->spacing;
			rect.height += spacing;
			int headerHeight = _w_expandbar_get_band_height(
					W_EXPANDBAR(widget));
			w_imagelist *imagelist = _W_EXPANDBAR(widget)->imagelist;
			int imageWidth = 0;
			if (imagelist != 0) {
				w_size sz;
				w_imagelist_get_size(imagelist, &sz);
				headerHeight = WMAX(headerHeight, sz.height);
				imageWidth = sz.width;
			}
			_w_expanditem_handle *_item;
			for (int i = 0; i < itemCount; i++) {
				_item = &items->items[i];
				rect.height += headerHeight;
				if (_item->expanded)
					rect.height += _item->rect.height;
				rect.height += spacing;
				int preferredWidth = _w_expanditem_get_preferred_width(
						W_EXPANDBAR(widget), _item, hTheme, hDC, imageWidth);
				rect.width = WMAX(rect.width, preferredWidth);
			}
			if (hCurrentFont != 0) {
				SelectObject(hDC, oldFont);
				if (hCurrentFont != _W_EXPANDBAR(widget)->hFont)
					DeleteObject(hCurrentFont);
			}
			ReleaseDC(handle, hDC);
			if (hTheme != 0)
				CloseThemeData(hTheme);
		}
	}
	if (rect.width == 0)
		rect.width = DEFAULT_WIDTH;
	if (rect.height == 0)
		rect.height = DEFAULT_HEIGHT;
	if (wHint != W_DEFAULT)
		rect.width = wHint;
	if (hHint != W_DEFAULT)
		rect.height = hHint;
	w_event_compute_trim _e;
	w_rect trim;
	memset(&trim, 0, sizeof(w_rect));
	_e.event.type = W_EVENT_COMPUTE_TRIM;
	_e.event.platform_event = 0;
	_e.event.widget = widget;
	_e.event.time = 0;
	_e.rect = &rect;
	_e.result = &trim;
	_e.event.data = 0;
	wresult result = priv->compute_trim(widget, &_e, priv);
	e->size->width = trim.width;
	e->size->height = trim.height;
	return result;
}
wresult _w_expandbar_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result = _w_composite_create_handle(control, priv);
	if (result > 0) {
		_W_WIDGET(control)->state &= ~STATE_CANVAS;
		_W_WIDGET(control)->state |= STATE_TRACK_MOUSE;
		_W_EXPANDBAR(control)->focusItem = -1;
	}
	return result;
}
#define __POINT(x,y) ((x) | ((y) << 4) )
#define __POINT_X(x) (x & 0xF)
#define __POINT_Y(x) ((x >> 4) & 0xF)
wuchar _polyline_0_0[] = { __POINT(0, 0), __POINT(1, 0), __POINT(1, 1), __POINT(
		2, 1), __POINT(2, 2), __POINT(3, 2), __POINT(3, 3), __POINT(3, 2),
		__POINT(4, 2), __POINT(4, 1), __POINT(5, 1), __POINT(5, 0), __POINT(7,
				0) };
void _w_expandbar_draw_chevron(w_expandbar *expandbar,
		_w_expanditem_handle *_item, HDC hDC, RECT *rect,
		_w_control_priv *priv) {
	HBRUSH oldBrush = SelectObject(hDC, GetSysColorBrush(COLOR_BTNFACE));
	PatBlt(hDC, rect->left, rect->top, rect->right - rect->left,
			rect->bottom - rect->top, PATCOPY);
	SelectObject(hDC, oldBrush);
	rect->left += 4;
	rect->top += 4;
	rect->right -= 4;
	rect->bottom -= 4;
	int foreground_pixel = priv->get_foreground_pixel(W_CONTROL(expandbar),
			priv);
	HPEN hPen = CreatePen(PS_SOLID, 1, foreground_pixel);
	HPEN oldPen = SelectObject(hDC, hPen);
	POINT polyline1[14];
	POINT polyline2[14];
	const int polyline_length = sizeof(_polyline_0_0)
			/ sizeof(_polyline_0_0[0]);
	if (_item->expanded) {
		int px = rect->left + 5;
		int py = rect->top + 7;
		for (int i = 0; i < polyline_length; i++) {
			polyline1[i].x = px + __POINT_X(_polyline_0_0[i]);
			polyline1[i].y = py - __POINT_Y(_polyline_0_0[i]);
			polyline2[i].x = px + __POINT_X(_polyline_0_0[i]);
			polyline2[i].y = py + 4 - __POINT_Y(_polyline_0_0[i]);
		}
	} else {
		int px = rect->left + 5;
		int py = rect->top + 4;
		for (int i = 0; i < polyline_length; i++) {
			polyline1[i].x = px + __POINT_X(_polyline_0_0[i]);
			polyline1[i].y = py + __POINT_Y(_polyline_0_0[i]);
			polyline2[i].x = px + __POINT_X(_polyline_0_0[i]);
			polyline2[i].y = py + 4 + __POINT_Y(_polyline_0_0[i]);
		}
	}
	Polyline(hDC, polyline1, polyline_length);
	Polyline(hDC, polyline2, polyline_length);
	if (_item->hover) {
		HPEN whitePen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
		HPEN darkGrayPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
		SelectObject(hDC, whitePen);
		polyline1[0].x = rect->left;
		polyline1[0].y = rect->bottom;
		polyline1[1].x = rect->left;
		polyline1[1].y = rect->top;
		polyline1[2].x = rect->right;
		polyline1[2].y = rect->top;
		Polyline(hDC, polyline1, 3);
		SelectObject(hDC, darkGrayPen);
		polyline1[0].x = rect->right;
		polyline1[0].y = rect->top;
		polyline1[1].x = rect->right;
		polyline1[1].y = rect->bottom;
		polyline1[2].x = rect->left;
		polyline1[2].y = rect->bottom;
		Polyline(hDC, polyline1, 3);
		SelectObject(hDC, oldPen);
		DeleteObject(whitePen);
		DeleteObject(darkGrayPen);
	} else {
		SelectObject(hDC, oldPen);
	}
	DeleteObject(hPen);
}

void _w_expandbar_draw_item(w_expandbar *expandbar, _w_expanditem_handle *_item,
		w_graphics *gc, HTHEME hTheme, RECT *clipRect, _w_control_priv *priv,
		int drawFocus) {
	HDC hDC = _W_GRAPHICS(gc)->handle;
	int headerHeight = _w_expandbar_get_band_height(expandbar);
	RECT rect;
	SetRect(&rect, _item->rect.x, _item->rect.y,
			_item->rect.x + _item->rect.width, _item->rect.y + headerHeight);
	if (hTheme != 0) {
		DrawThemeBackground(hTheme, hDC, EBP_NORMALGROUPHEAD, 0, &rect,
				clipRect);
	} else {
		HBRUSH oldBrush = SelectObject(hDC, GetSysColorBrush(COLOR_BTNFACE));
		PatBlt(hDC, rect.left, rect.top, rect.right - rect.left,
				rect.bottom - rect.top, PATCOPY);
		SelectObject(hDC, oldBrush);
	}
	w_imagelist *imagelist = _W_EXPANDBAR(expandbar)->imagelist;
	if (_item->image > 0 && imagelist != 0) {
		rect.left += TEXT_INSET;
		w_size sz;
		w_imagelist_get_size(imagelist, &sz);
		w_point pt;
		pt.x = rect.left;
		if (sz.height > headerHeight) {
			pt.y = rect.top + headerHeight - sz.height;
		} else {
			pt.y = rect.top + (headerHeight - sz.height) / 2;
		}
		w_imagelist_draw(imagelist, gc, _item->image - 1, &pt, 0);
		rect.left += sz.width;
	}
	if (_item->text != 0) {
		rect.left += TEXT_INSET;
		WCHAR *str;
		int newlength;
		if ((_W_WIDGET(expandbar)->style & W_FLIP_TEXT_DIRECTION) != 0) {
			int bits = GetWindowLong(_W_WIDGET(expandbar)->handle, GWL_EXSTYLE);
			if ((bits & WS_EX_LAYOUTRTL) != 0) {
			} else {
			}
		} else {
			_win_text_fix(_item->text, -1, W_ENCODING_UTF8, &str, &newlength);
		}
		if (hTheme != 0) {
			DrawThemeText(hTheme, hDC, EBP_NORMALGROUPHEAD, 0, str, newlength,
			DT_VCENTER | DT_SINGLELINE, 0, &rect);
		} else {
			int oldBkMode = SetBkMode(hDC, TRANSPARENT);
			DrawTextW(hDC, str, newlength, &rect, DT_VCENTER | DT_SINGLELINE);
			SetBkMode(hDC, oldBkMode);
		}
		_win_text_free(_item->text, str, newlength);
	}
	int chevronSize = CHEVRON_SIZE;
	rect.left = rect.right - chevronSize;
	rect.top = _item->rect.y + (headerHeight - chevronSize) / 2;
	rect.bottom = rect.top + chevronSize;
	if (hTheme != 0) {
		int partID =
				_item->expanded ?
						EBP_NORMALGROUPCOLLAPSE : EBP_NORMALGROUPEXPAND;
		int stateID = _item->hover ? EBNGC_HOT : EBNGC_NORMAL;
		DrawThemeBackground(hTheme, hDC, partID, stateID, &rect, clipRect);
	} else {
		_w_expandbar_draw_chevron(expandbar, _item, hDC, &rect, priv);
	}
	if (drawFocus) {
		SetRect(&rect, _item->rect.x + 1, _item->rect.y + 1,
				_item->rect.x + _item->rect.width - 2,
				_item->rect.y + headerHeight - 2);
		DrawFocusRect(hDC, &rect);
	}
	if (_item->expanded) {
		if (!_w_expandbar_is_app_themed(expandbar)) {
			HPEN pen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
			HPEN oldPen = SelectObject(hDC, pen);
			POINT points[4];
			points[0].x = _item->rect.x;
			points[0].y = _item->rect.y + headerHeight;
			points[1].x = _item->rect.x;
			points[1].y = _item->rect.y + headerHeight + _item->rect.height;
			points[2].x = _item->rect.x + _item->rect.width - 1;
			points[2].y = _item->rect.y + headerHeight + _item->rect.height;
			points[3].x = _item->rect.x + _item->rect.width - 1;
			points[3].y = _item->rect.y + headerHeight - 1;
			Polyline(hDC, points, 4);
			SelectObject(hDC, oldPen);
			DeleteObject(pen);
		}
	}
}
void _w_expandbar_draw_widget(w_expandbar *expandbar, w_graphics *gc,
		RECT *clipRect, _w_control_priv *priv) {
	HTHEME hTheme = 0;
	HWND handle = _W_WIDGET(expandbar)->handle;
	HDC hdc = _W_GRAPHICS(gc)->handle;
	if (_w_expandbar_is_app_themed(expandbar)) {
		hTheme = OpenThemeData(NULL, L"EXPLORERBAR");
	}
	RECT rect;
	GetClientRect(handle, &rect);
	if (hTheme != 0) {
		DrawThemeBackground(hTheme, hdc, EBP_HEADERBACKGROUND, 0, &rect,
				clipRect);
	} else {
		priv->draw_background(W_CONTROL(expandbar), hdc, &rect, -1, 0, 0, priv);
	}
	boolean drawFocus = FALSE;
	if (handle == GetFocus()) {
		int uiState = SendMessageW(handle, WM_QUERYUISTATE, 0, 0);
		drawFocus = (uiState & UISF_HIDEFOCUS) == 0;
	}
	HFONT hCurrentFont = 0, oldFont = 0, hFont;
	hFont = _W_EXPANDBAR(expandbar)->hFont;
	if (hTheme == 0) {
		if (hFont != 0) {
			hCurrentFont = hFont;
		} else {
			NONCLIENTMETRICSW info;
			info.cbSize = sizeof(info);
			if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &info, 0)) {
				hCurrentFont = CreateFontIndirectW(&info.lfCaptionFont);
			}
		}
		if (hCurrentFont != 0) {
			oldFont = SelectObject(hdc, hCurrentFont);
		}
		if (_W_CONTROL(expandbar)->foreground != 0) {
			SetTextColor(hdc, _W_CONTROL(expandbar)->foreground & 0x00FFFFFF);
		}
	}
	_w_expanditem_handles *_items = _W_EXPANDBAR(expandbar)->items;
	int itemCount = 0;
	if (_items != 0)
		itemCount = _items->count;
	int focusItem = _W_EXPANDBAR(expandbar)->focusItem;
	for (int i = 0; i < itemCount; i++) {
		_w_expanditem_handle *_item = &_items->items[i];
		_w_expandbar_draw_item(expandbar, _item, gc, hTheme, clipRect, priv,
				i == focusItem && drawFocus);
	}
	if (hCurrentFont != 0) {
		SelectObject(hdc, oldFont);
		if (hCurrentFont != hFont)
			DeleteObject(hCurrentFont);
	}
	if (hTheme != 0) {
		CloseThemeData(hTheme);
	}
}
int _w_expandbar_get_band_height(w_expandbar *expandbar) {
	HFONT hFont = _W_EXPANDBAR(expandbar)->hFont;
	if (hFont == 0)
		return CHEVRON_SIZE;
	HWND handle = _W_WIDGET(expandbar)->handle;
	HDC hDC = GetDC(handle);
	HFONT oldHFont = SelectObject(hDC, hFont);
	TEXTMETRICW lptm;
	GetTextMetricsW(hDC, &lptm);
	SelectObject(hDC, oldHFont);
	ReleaseDC(handle, hDC);
	return WMAX(CHEVRON_SIZE, lptm.tmHeight + 4);
}
wresult _w_expandbar_get_imagelist(w_expandbar *expandbar,
		w_imagelist **imagelist) {
	*imagelist = _W_EXPANDBAR(expandbar)->imagelist;
	return W_TRUE;
}
wresult _w_expandbar_get_item(w_expandbar *expandbar, int index,
		w_expanditem *item) {
	wresult result = W_FALSE;
	_w_expanditem_handles *_items = _W_EXPANDBAR(expandbar)->items;
	if (_items != 0 && index < _items->count) {
		_W_WIDGETDATA(item)->clazz = 0;
		_W_ITEM(item)->parent = W_WIDGET(expandbar);
		_W_ITEM(item)->index = index;
	}
	return result;
}
wresult _w_expandbar_get_item_count(w_expandbar *expandbar) {
	_w_expanditem_handles *_items = _W_EXPANDBAR(expandbar)->items;
	int itemCount = 0;
	if (_items != 0)
		itemCount = _items->count;
	return itemCount;
}
wresult _w_expandbar_get_items(w_expandbar *expandbar, w_iterator *items) {
	return W_FALSE;
}
wresult _w_expandbar_get_spacing(w_expandbar *expandbar) {
	return _W_EXPANDBAR(expandbar)->spacing;
}
#define _W_EXPANDBAR_GROW 4
wresult _w_expandbar_insert_item(w_expandbar *expandbar, w_expanditem *item,
		int index) {
	_w_expanditem_handles *_items = _W_EXPANDBAR(expandbar)->items;
	int itemCount = 0, row = 0;
	if (_items != 0)
		itemCount = _items->count;
	if (!(0 <= index && index <= itemCount)) {
		index = itemCount;
	}
	_w_expanditem_handle *_item;
	if (_items == 0) {
		const int _size = sizeof(_w_expanditem_handles)
				+ _W_EXPANDBAR_GROW * sizeof(_w_expanditem_handle);
		_items = malloc(_size);
		if (_items == 0)
			return W_ERROR_NO_MEMORY;
		_items->alloc = _W_EXPANDBAR_GROW;
		_items->count = 1;
		_item = &_items->items[0];
		_W_EXPANDBAR(expandbar)->items = _items;
	} else {
		if (_items->alloc <= _items->count) {
			const int _size =
					sizeof(_w_expanditem_handles)
							+ (_W_EXPANDBAR(expandbar)->items->alloc
									+ _W_EXPANDBAR_GROW)
									* sizeof(_w_expanditem_handle);
			_w_expanditem_handles *_items = realloc(_items, _size);
			if (_items == 0)
				return W_ERROR_NO_MEMORY;
			_W_EXPANDBAR(expandbar)->items = _items;
			_items->alloc += _W_EXPANDBAR_GROW;
		}
		if (index != itemCount) {
			for (int i = itemCount; i >= index; i--) {
				memcpy(&_items[i + 1], &_items[i],
						sizeof(_w_expanditem_handle));
			}
		}
		_items->count++;
		_item = &_items->items[index];
	}
	memset(_item, 0, sizeof(_w_expanditem_handle));
	if (_W_EXPANDBAR(expandbar)->focusItem == -1)
		_W_EXPANDBAR(expandbar)->focusItem = index;

	RECT rect;
	GetWindowRect(_W_WIDGET(expandbar)->handle, &rect);
	_item->rect.width = WMAX(0,
			rect.right - rect.left - _W_EXPANDBAR(expandbar)->spacing * 2);
	_w_expandbar_layout_items(expandbar, index);
	_w_expandbar_set_scrollbar(expandbar);
	if (item != 0) {
		_W_WIDGETDATA(item)->clazz = _W_EXPANDBAR_GET_ITEM_CLASS(expandbar);
		_W_ITEM(item)->parent = W_WIDGET(expandbar);
		_W_ITEM(item)->index = index;
	}
	return W_TRUE;
}
int _w_expandbar_is_app_themed(w_expandbar *expandbar) {
	if (_W_CONTROL(expandbar)->background != 0)
		return W_FALSE;
	if (_W_CONTROL(expandbar)->foreground != 0)
		return W_FALSE;
	if ( _W_EXPANDBAR(expandbar)->hFont != 0)
		return W_FALSE;
	return _COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed();
}
void _w_expandbar_layout_items(w_expandbar *expandbar, int index) {
	_w_expanditem_handles *_items = _W_EXPANDBAR(expandbar)->items;
	int itemCount = 0;
	if (_items != 0)
		itemCount = _items->count;
	if (index < itemCount) {
		w_rect r;
		int spacing = _W_EXPANDBAR(expandbar)->spacing;
		r.y = spacing - _W_EXPANDBAR(expandbar)->yCurrentScroll;
		r.x = spacing;
		int headerHeight = _w_expandbar_get_band_height(expandbar);
		w_imagelist *imagelist = _W_EXPANDBAR(expandbar)->imagelist;
		if (imagelist != 0) {
			w_imagelist_get_size(imagelist, &r.sz);
			headerHeight = WMAX(headerHeight, r.sz.height);
		}
		_w_expanditem_handle *_item;
		for (int i = 0; i < index; i++) {
			_item = &_items->items[i];
			if (_item->expanded)
				r.y += _item->rect.height;
			r.y += headerHeight + spacing;
		}
		for (int i = index; i < itemCount; i++) {
			_item = &_items->items[i];
			_w_expanditem_set_bounds(expandbar, _item, &r.pt, 0);
			if (_item->expanded)
				r.y += _item->rect.height;
			r.y += headerHeight + spacing;
		}
	}
}
void _w_expandbar_show_item(w_expandbar *expandbar, int index,
		_w_expanditem_handle *_item) {
	w_control *control = _item->control;
	if (w_widget_is_ok(W_WIDGET(control)) > 0) {
		w_control_set_visible(control, _item->expanded);
	}
	_w_expanditem_redraw(expandbar, _item, TRUE);
	_w_expandbar_layout_items(expandbar, index + 1);
	_w_expandbar_set_scrollbar(expandbar);
}
void _w_expandbar_show_focus(w_expandbar *expandbar, int up) {
	int focusIndex = _W_EXPANDBAR(expandbar)->focusItem;
	if (focusIndex == -1)
		return;
	_w_expanditem_handles *_items = _W_EXPANDBAR(expandbar)->items;
	_w_expanditem_handle *focusItem = 0;
	if (_items != 0 && focusIndex < _items->count) {
		focusItem = &_items->items[focusIndex];
	}
	if (focusItem == 0)
		return;
	HWND handle = _W_WIDGET(expandbar)->handle;
	RECT rect;
	GetClientRect(handle, &rect);
	int height = rect.bottom - rect.top;
	int updateY = 0;
	if (up) {
		if (focusItem->rect.y < 0) {
			updateY = WMIN(_W_EXPANDBAR(expandbar)->yCurrentScroll,
					-focusItem->rect.y);
		}
	} else {
		int bandHeight = _w_expandbar_get_band_height(expandbar);
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
		_W_EXPANDBAR(expandbar)->yCurrentScroll = WMAX(0,
				_W_EXPANDBAR(expandbar)->yCurrentScroll - updateY);
		if ((_W_WIDGET(expandbar)->style & W_VSCROLL) != 0) {
			SCROLLINFO info;
			info.cbSize = sizeof(info);
			info.fMask = SIF_POS;
			info.nPos = _W_EXPANDBAR(expandbar)->yCurrentScroll;
			SetScrollInfo(handle, SB_VERT, &info, TRUE);
		}
		ScrollWindowEx(handle, 0, updateY, NULL, NULL, 0, NULL,
		SW_SCROLLCHILDREN | SW_INVALIDATE);
		int itemCount = _items->count;
		for (int i = 0; i < itemCount; i++) {
			_items->items[i].rect.y += updateY;
		}
	}
}
wresult _w_expandbar_set_imagelist(w_expandbar *expandbar,
		w_imagelist *imagelist) {
	if (imagelist != 0 && w_imagelist_is_ok(imagelist) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	_W_EXPANDBAR(expandbar)->imagelist = imagelist;
	return W_TRUE;
}
void _w_expandbar_set_scrollbar(w_expandbar *expandbar) {
	if ((_W_WIDGET(expandbar)->style & W_VSCROLL) == 0)
		return;
	_w_expanditem_handles *_items = _W_EXPANDBAR(expandbar)->items;
	int itemCount = 0;
	if (_items != 0)
		itemCount = _items->count;
	if (itemCount == 0)
		return;
	HWND handle = _W_WIDGET(expandbar)->handle;
	RECT rect;
	GetClientRect(handle, &rect);
	int height = rect.bottom - rect.top;
	_w_expanditem_handle *_item = &_items->items[itemCount - 1];
	int maxHeight = _item->rect.y + _w_expandbar_get_band_height(expandbar)
			+ _W_EXPANDBAR(expandbar)->spacing;
	if (_item->expanded)
		maxHeight += _item->rect.height;

	//claim bottom free space
	if (_W_EXPANDBAR(expandbar)->yCurrentScroll > 0 && height > maxHeight) {
		_W_EXPANDBAR(expandbar)->yCurrentScroll = WMAX(0,
				_W_EXPANDBAR(expandbar)->yCurrentScroll + maxHeight - height);
		_w_expandbar_layout_items(expandbar, 0);
	}
	maxHeight += _W_EXPANDBAR(expandbar)->yCurrentScroll;

	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	info.nMin = 0;
	info.nMax = maxHeight;
	info.nPage = height;
	info.nPos = WMIN(_W_EXPANDBAR(expandbar)->yCurrentScroll, info.nMax);
	if (info.nPage != 0)
		info.nPage++;
	SetScrollInfo(handle, SB_VERT, &info, TRUE);
}
wresult _w_expandbar_set_spacing(w_expandbar *expandbar, int spacing) {
	if (spacing < 0)
		return W_FALSE;
	if (spacing == _W_EXPANDBAR(expandbar)->spacing)
		return W_TRUE;
	_W_EXPANDBAR(expandbar)->spacing = spacing;
	HWND handle = _W_WIDGET(expandbar)->handle;
	RECT rect;
	GetClientRect(handle, &rect);
	w_size sz;
	sz.width = WMAX(0, (rect.right - rect.left) - spacing * 2);
	_w_expanditem_handles *_items = _W_EXPANDBAR(expandbar)->items;
	int itemCount = 0;
	if (_items != 0)
		itemCount = _items->count;
	for (int i = 0; i < itemCount; i++) {
		_w_expanditem_handle *_item = &_items->items[i];
		if (_item->rect.width != sz.width) {
			sz.height = _item->rect.height;
			_w_expanditem_set_bounds(expandbar, _item, 0, &sz);
		}
	}
	_w_expandbar_layout_items(expandbar, 0);
	_w_expandbar_set_scrollbar(expandbar);
	InvalidateRect(handle, NULL, TRUE);
	return W_TRUE;
}
const char* _w_expandbar_window_class(w_control *control,
		_w_control_priv *priv) {
	return WindowClass;
}
/*
 * messages
 */
wresult _EXPANDBAR_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_KEYDOWN(widget, e, priv);
	if (result)
		return result;
	int focusItem = _W_EXPANDBAR(widget)->focusItem;
	if (focusItem == -1)
		return result;
	_w_expanditem_handles *_items = _W_EXPANDBAR(widget)->items;
	_w_expanditem_handle *_item = 0;
	if (_items != 0 && focusItem < _items->count) {
		_item = &_items->items[focusItem];
	}
	if (_item == 0)
		return result;
	w_event_expand event;
	_w_expanditem item;
	switch (e->wparam) {
	case VK_SPACE:
	case VK_RETURN:
		event.event.type =
				_item->expanded ? W_EVENT_ITEM_COLLAPSE : W_EVENT_ITEM_EXPAND;
		event.event.widget = widget;
		event.event.platform_event = _EVENT_PLATFORM(e);
		event.event.time = 0;
		event.event.data = 0;
		event.item = W_EXPANDITEM(&item);
		_W_WIDGETDATA(&item)->clazz = 0;
		_W_ITEM(&item)->parent = widget;
		_W_ITEM(&item)->index = focusItem;
		_item->expanded = !_item->expanded;
		_w_widget_send_event(widget, W_EVENT(&event));
		_w_expandbar_show_item(W_EXPANDBAR(widget), focusItem, _item);
		e->result = 0;
		result = W_TRUE;
		break;
	case VK_UP: {
		if (focusItem > 0) {
			_w_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
			focusItem -= 1;
			_W_EXPANDBAR(widget)->focusItem = focusItem;
			_item = &_items->items[focusItem];
			_w_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
			_w_expandbar_show_focus(W_EXPANDBAR(widget), TRUE);
			e->result = 0;
			result = W_TRUE;
		}
		break;
	}
	case VK_DOWN: {
		if (focusItem < _items->count - 1) {
			_w_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
			focusItem += 1;
			_W_EXPANDBAR(widget)->focusItem = focusItem;
			_item = &_items->items[focusItem];
			_w_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
			_w_expandbar_show_focus(W_EXPANDBAR(widget), TRUE);
			e->result = 0;
			result = W_TRUE;
		}
		break;
	}
	}
	return result;
}
wresult _EXPANDBAR_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_KILLFOCUS(widget, e, priv);
	int focusItem = _W_EXPANDBAR(widget)->focusItem;
	if (focusItem != -1) {
		_w_expanditem_handles *_items = _W_EXPANDBAR(widget)->items;
		if (_items != 0 && focusItem < _items->count) {
			_w_expanditem_redraw(W_EXPANDBAR(widget), &_items->items[focusItem],
					W_TRUE);
		}
	}
	return result;
}
wresult _EXPANDBAR_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_LBUTTONDOWN(widget, e, priv);
	if (result == W_TRUE && e->result == 0)
		return result;
	int x = GET_X_LPARAM(e->lparam);
	int y = GET_Y_LPARAM(e->lparam);
	_w_expanditem_handles *_items = _W_EXPANDBAR(widget)->items;
	int itemCount = 0;
	if (_items != 0)
		itemCount = _items->count;
	int bandHeight = _w_expandbar_get_band_height(W_EXPANDBAR(widget));
	int focusItem = _W_EXPANDBAR(widget)->focusItem;
	for (int i = 0; i < itemCount; i++) {
		_w_expanditem_handle *_item = &_items->items[i];
		int hover = _w_expanditem_is_hover(W_EXPANDBAR(widget), _item,
				bandHeight, x, y);
		if (hover && focusItem != i) {
			if (focusItem != -1 && focusItem < itemCount) {
				_w_expanditem_redraw(W_EXPANDBAR(widget),
						&_items->items[focusItem], W_TRUE);
			}
			_W_EXPANDBAR(widget)->focusItem = i;
			_w_expanditem_redraw(W_EXPANDBAR(widget), _item, W_TRUE);
			w_control_force_focus(W_CONTROL(widget));
			break;
		}
	}
	return result;
}
wresult _EXPANDBAR_WM_LBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_LBUTTONUP(widget, e, priv);
	if (result == W_TRUE && e->result == 0)
		return result;
	int focusItem = _W_EXPANDBAR(widget)->focusItem;
	if (focusItem == -1)
		return result;
	int x = GET_X_LPARAM(e->lparam);
	int y = GET_Y_LPARAM(e->lparam);
	_w_expanditem_handle *_item = 0;
	_w_expanditem_handles *_items = _W_EXPANDBAR(widget)->items;
	if (_items != 0 && _items->count > focusItem) {
		_item = &_items->items[focusItem];
	}
	if (_item == 0)
		return result;
	int bandHeight = _w_expandbar_get_band_height(W_EXPANDBAR(widget));
	int hover = _w_expanditem_is_hover(W_EXPANDBAR(widget), _item, bandHeight,
			x, y);
	if (hover) {
		w_event_expand event;
		_w_expanditem item;
		event.event.type =
				_item->expanded ? W_EVENT_ITEM_COLLAPSE : W_EVENT_ITEM_EXPAND;
		event.event.widget = widget;
		event.event.platform_event = _EVENT_PLATFORM(e);
		event.event.time = 0;
		event.event.data = 0;
		event.item = W_EXPANDITEM(&item);
		_W_WIDGETDATA(&item)->clazz = 0;
		_W_ITEM(&item)->parent = widget;
		_W_ITEM(&item)->index = focusItem;
		_item->expanded = !_item->expanded;
		_w_widget_send_event(widget, W_EVENT(&event));
		_w_expandbar_show_item(W_EXPANDBAR(widget), focusItem, _item);
	}
	return result;
}
wresult _EXPANDBAR_WM_MOUSELEAVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_MOUSELEAVE(widget, e, priv);
	if (result)
		return result;
	_w_expanditem_handles *_items = _W_EXPANDBAR(widget)->items;
	int itemCount = 0;
	if (_items != 0)
		itemCount = _items->count;
	for (int i = 0; i < itemCount; i++) {
		_w_expanditem_handle *_item = &_items->items[i];
		if (_item->hover) {
			_item->hover = W_FALSE;
			_w_expanditem_redraw(W_EXPANDBAR(widget), _item, W_FALSE);
			break;
		}
	}
	return result;
}
wresult _EXPANDBAR_WM_MOUSEMOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_MOUSEMOVE(widget, e, priv);
	if (result == W_TRUE && e->result == 0)
		return result;
	int x = GET_X_LPARAM(e->lparam);
	int y = GET_Y_LPARAM(e->lparam);
	_w_expanditem_handles *_items = _W_EXPANDBAR(widget)->items;
	int itemCount = 0;
	if (_items != 0)
		itemCount = _items->count;
	int bandHeight = _w_expandbar_get_band_height(W_EXPANDBAR(widget));
	for (int i = 0; i < itemCount; i++) {
		_w_expanditem_handle *_item = &_items->items[i];
		int hover = _w_expanditem_is_hover(W_EXPANDBAR(widget), _item,
				bandHeight, x, y);
		if (_item->hover != hover) {
			_item->hover = hover;
			_w_expanditem_redraw(W_EXPANDBAR(widget), _item, W_FALSE);
		}
	}
	return result;
}
wresult _EXPANDBAR_WM_MOUSEWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	//return wmScrollWheel (true, wParam, lParam);
	return W_FALSE;
}
wresult _EXPANDBAR_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((_W_WIDGET(widget)->state & STATE_DISPOSE_SENT) == 0) {
		w_event_paint event;
		PAINTSTRUCT ps;
		_w_graphics gc;
		HDC hdc;
		RECT *rect, tmp;
		if (e->wparam == 0) {
			hdc = BeginPaint(e->hwnd, &ps);
			_w_graphics_init(W_GRAPHICS(&gc), hdc);
			_W_GRAPHICS(&gc)->ps = &ps;
			_W_GRAPHICS(&gc)->hwnd = e->hwnd;
			rect = &ps.rcPaint;
		} else {
			hdc = (HDC) e->wparam;
			_w_graphics_init(W_GRAPHICS(&gc), hdc);
			GetUpdateRect(e->hwnd, &tmp, FALSE);
			rect = &tmp;
		}
		int width = rect->right - rect->left;
		int height = rect->bottom - rect->top;
		if (width != 0 && height != 0) {
			_w_expandbar_draw_widget(W_EXPANDBAR(widget), W_GRAPHICS(&gc), rect,
					priv);
			event.event.type = W_EVENT_PAINT;
			event.event.widget = widget;
			event.event.platform_event = _EVENT_PLATFORM(e);
			event.event.time = 0;
			event.event.data = 0;
			event.gc = W_GRAPHICS(&gc);
			event.bounds.x = rect->left;
			event.bounds.y = rect->top;
			event.bounds.width = rect->right - rect->left;
			event.bounds.height = rect->bottom - rect->top;
			_w_widget_send_event(widget, W_EVENT(&event));
		}
		w_graphics_dispose(W_GRAPHICS(&gc));
		if (e->wparam == 0) {
			EndPaint(e->hwnd, &ps);
		}
	}
	e->result = 0;
	return W_TRUE;
}
wresult _EXPANDBAR_WM_PRINTCLIENT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_PRINTCLIENT(widget, e, priv);
	RECT rect;
	GetClientRect(e->hwnd, &rect);
	_w_graphics gc;
	HDC hdc;
	_w_graphics_init(W_GRAPHICS(&gc), hdc);
	int foreground = priv->get_foreground_pixel(W_CONTROL(widget), priv);
	w_graphics_set_foreground(W_GRAPHICS(&gc), foreground);
	_w_expandbar_draw_widget(W_EXPANDBAR(widget), W_GRAPHICS(&gc), &rect, priv);
	w_graphics_dispose(W_GRAPHICS(&gc));
	return result;
}
wresult _EXPANDBAR_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _CONTROL_WM_SETCURSOR(widget, e, priv);
	if (result)
		return result;
	int hitTest = LOWORD(e->lparam);
	if (hitTest == HTCLIENT) {
		_w_expanditem_handles *_items = _W_EXPANDBAR(widget)->items;
		int itemCount = 0;
		if (_items != 0)
			itemCount = _items->count;
		for (int i = 0; i < itemCount; i++) {
			_w_expanditem_handle *_item = &_items->items[i];
			if (_item->hover) {
				HCURSOR hCursor = LoadCursor(0, IDC_HAND);
				SetCursor(hCursor);
				e->result = 1;
				return W_TRUE;
			}
		}
	}
	return result;
}
wresult _EXPANDBAR_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_SETFOCUS(widget, e, priv);
	int focusItem = _W_EXPANDBAR(widget)->focusItem;
	if (focusItem != -1) {
		_w_expanditem_handles *_items = _W_EXPANDBAR(widget)->items;
		if (_items != 0 && focusItem < _items->count) {
			_w_expanditem_redraw(W_EXPANDBAR(widget), &_items->items[focusItem],
					W_TRUE);
		}
	}
	return result;
}
wresult _EXPANDBAR_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _COMPOSITE_WM_SIZE(widget, e, priv);
	RECT rect;
	GetClientRect(e->hwnd, &rect);
	w_size sz;
	sz.width = WMAX(0,
			(rect.right - rect.left) - _W_EXPANDBAR(widget)->spacing * 2);
	_w_expanditem_handles *_items = _W_EXPANDBAR(widget)->items;
	int itemCount = 0;
	if (_items != 0)
		itemCount = _items->count;
	_w_expanditem_handle *_item;
	for (int i = 0; i < itemCount; i++) {
		_item = &_items->items[i];
		if (_item->rect.width != sz.width) {
			sz.height = _item->rect.height;
			_w_expanditem_set_bounds(W_EXPANDBAR(widget), _item, 0, &sz);
		}
	}
	_w_expandbar_set_scrollbar(W_EXPANDBAR(widget));
	InvalidateRect(e->hwnd, NULL, TRUE);
	return result;
}
wresult _EXPANDBAR_WM_SCROLL(w_widget *widget, _w_event_platform *e,
		wresult update, _w_control_priv *priv) {
	wresult result = _SCROLLABLE_WM_SCROLL(widget, e, W_TRUE, priv);
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_POS;
	GetScrollInfo(e->hwnd, SB_VERT, &info);
	int updateY = _W_EXPANDBAR(widget)->yCurrentScroll - info.nPos;
	ScrollWindowEx(e->hwnd, 0, updateY, NULL, NULL, 0, NULL,
	SW_SCROLLCHILDREN | SW_INVALIDATE);
	_W_EXPANDBAR(widget)->yCurrentScroll = info.nPos;
	if (updateY != 0) {
		_w_expanditem_handles *_items = _W_EXPANDBAR(widget)->items;
		int itemCount = 0;
		if (_items != 0)
			itemCount = _items->count;
		_w_expanditem_handle *_item;
		for (int i = 0; i < itemCount; i++) {
			_item = &_items->items[i];
			_item->rect.y += updateY;
		}
	}
	return result;
}
void _w_expandbar_class_init(struct _w_expandbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_EXPANDBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_expandbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_expandbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_expandbar);
	/*
	 * public function
	 */
	clazz->get_imagelist = _w_expandbar_get_imagelist;
	clazz->get_item = _w_expandbar_get_item;
	clazz->get_item_count = _w_expandbar_get_item_count;
	clazz->get_items = _w_expandbar_get_items;
	clazz->get_spacing = _w_expandbar_get_spacing;
	clazz->insert_item = _w_expandbar_insert_item;
	clazz->set_imagelist = _w_expandbar_set_imagelist;
	clazz->set_spacing = _w_expandbar_set_spacing;
	/*
	 * expanditem
	 */
	struct _w_expanditem_class *item = clazz->class_expanditem;
	_w_item_class_init(W_ITEM_CLASS(item));
	W_ITEM_CLASS(item)->get_data = _w_expanditem_get_data;
	W_ITEM_CLASS(item)->get_text = _w_expanditem_get_text;
	W_ITEM_CLASS(item)->set_data = _w_expanditem_set_data;
	W_ITEM_CLASS(item)->set_text = _w_expanditem_set_text;
	item->get_control = _w_expanditem_get_control;
	item->get_expanded = _w_expanditem_get_expanded;
	item->get_header_height = _w_expanditem_get_header_height;
	item->get_height = _w_expanditem_get_height;
	item->set_control = _w_expanditem_set_control;
	item->set_expanded = _w_expanditem_set_expanded;
	item->set_height = _w_expanditem_set_height;
	item->set_image = _w_expanditem_set_image;
	/*
	 * priv
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->check_style = _w_expandbar_check_style;
	priv->compute_size = _w_expandbar_compute_size;
	priv->create_handle = _w_expandbar_create_handle;
	priv->window_class = _w_expandbar_window_class;
	_W_SCROLLABLE_PRIV(priv)->WM_SCROLL = _EXPANDBAR_WM_SCROLL;
	/*
	 * messages
	 */
	dispatch_message *msg = priv->messages;
	msg[_WM_KEYDOWN] = _EXPANDBAR_WM_KEYDOWN;
	msg[_WM_KILLFOCUS] = _EXPANDBAR_WM_KILLFOCUS;
	msg[_WM_LBUTTONDOWN] = _EXPANDBAR_WM_LBUTTONDOWN;
	msg[_WM_LBUTTONUP] = _EXPANDBAR_WM_LBUTTONUP;
	msg[_WM_MOUSELEAVE] = _EXPANDBAR_WM_MOUSELEAVE;
	msg[_WM_MOUSEMOVE] = _EXPANDBAR_WM_MOUSEMOVE;
	msg[_WM_MOUSEWHEEL] = _EXPANDBAR_WM_MOUSEWHEEL;
	msg[_WM_PAINT] = _EXPANDBAR_WM_PAINT;
	msg[_WM_PRINTCLIENT] = _EXPANDBAR_WM_PRINTCLIENT;
	msg[_WM_SETCURSOR] = _EXPANDBAR_WM_SETCURSOR;
	msg[_WM_SETFOCUS] = _EXPANDBAR_WM_SETFOCUS;
	msg[_WM_SIZE] = _EXPANDBAR_WM_SIZE;

}
