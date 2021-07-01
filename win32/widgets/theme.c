/*
 * Name:        theme.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
#include "vsstyle.h"
#include "vssym32.h"
typedef void (*_win32_theme_part_id)(w_theme *theme, w_themedata *data,
		int *ids, int part);
/* expandbar */
#define EXPANDBAR_TEXT_INSET 6
#define EXPANDBAR_BORDER 1
#define EXPANDBAR_CHEVRON_SIZE 24
void _win32_themedata_expandbar_compute_trim(w_theme *theme, int mask,
		w_themedata *data, w_rect *result) {
}
void _win32_themedata_expandbar_draw(w_theme *theme, int mask,
		w_themedata *data) {
	HTHEME hTheme = 0;
	HDC hDC = _W_GRAPHICS(data->gc)->handle;
	/*if (_w_expandbar_is_app_themed(expandbar)) {
	 hTheme = OpenThemeData(NULL, L"EXPLORERBAR");
	 }*/
	RECT rect, clipRect;
	rect.left = data->bounds->x;
	rect.top = data->bounds->y;
	rect.right = data->bounds->x + data->bounds->width;
	rect.bottom = data->bounds->y + data->bounds->height;
	if (hTheme != 0) {
		DrawThemeBackground(hTheme, hDC, EBP_HEADERBACKGROUND, 0, &rect,
				&clipRect);
	} else {
		DWORD pixel = GetSysColor(COLOR_BTNFACE);
		HPALETTE hPalette = win_toolkit->hPalette;
		if (hPalette != 0) {
			SelectPalette(hDC, hPalette, FALSE);
			RealizePalette(hDC);
		}
		FillRect(hDC, &rect,
				_w_control_find_brush(pixel & 0x00FFFFFF, BS_SOLID));
	}
	HFONT hCurrentFont = 0, oldFont = 0, hFont;
	hFont = (HFONT) data->attr.font;
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
			oldFont = SelectObject(hDC, hCurrentFont);
		}
		if (data->attr.foreground != 0) {
			SetTextColor(hDC, data->attr.foreground & 0x00FFFFFF);
		}
	}
	if (hCurrentFont != 0) {
		SelectObject(hDC, oldFont);
		if (hCurrentFont != hFont)
			DeleteObject(hCurrentFont);
	}
	if (hTheme != 0) {
		CloseThemeData(hTheme);
	}
}
void _win32_themedata_expandbar_measure(w_theme *theme, int mask,
		w_themedata *data, w_size *result) {
}
void _win32_themedata_expandbar_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
}
int _win32_themedata_expandbar_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
}
/* expanditem */
void _win32_themedata_expanditem_compute_trim(w_theme *theme, int mask,
		w_themedata *data, w_rect *result) {
}
#define __POINT(x,y) ((x) | ((y) << 4) )
#define __POINT_X(x) (x & 0xF)
#define __POINT_Y(x) ((x >> 4) & 0xF)
wuchar _polyline_0_0[] = { __POINT(0, 0), __POINT(1, 0), __POINT(1, 1), __POINT(
		2, 1), __POINT(2, 2), __POINT(3, 2), __POINT(3, 3), __POINT(3, 2),
		__POINT(4, 2), __POINT(4, 1), __POINT(5, 1), __POINT(5, 0), __POINT(7,
				0) };
void _win32_themedata_expanditem_draw_chevron(w_theme *theme, int mask,
		w_themedata *data, RECT *rect) {
	HDC hDC = _W_GRAPHICS(data->gc)->handle;
	HBRUSH oldBrush = SelectObject(hDC, GetSysColorBrush(COLOR_BTNFACE));
	PatBlt(hDC, rect->left, rect->top, rect->right - rect->left,
			rect->bottom - rect->top, PATCOPY);
	SelectObject(hDC, oldBrush);
	rect->left += 4;
	rect->top += 4;
	rect->right -= 4;
	rect->bottom -= 4;
	int foreground_pixel = GetSysColor(COLOR_WINDOWTEXT);
	HPEN hPen = CreatePen(PS_SOLID, 1, foreground_pixel);
	HPEN oldPen = SelectObject(hDC, hPen);
	POINT polyline1[14];
	POINT polyline2[14];
	const int polyline_length = sizeof(_polyline_0_0)
			/ sizeof(_polyline_0_0[0]);
	if ((data->state & W_THEME_STATE_PART_EXPANDED) != 0) {
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
	if ((data->state & W_THEME_STATE_PART_HOT) != 0) {
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
void _win32_themedata_expanditem_draw(w_theme *theme, int mask,
		w_themedata *data) {
	HDC hDC = _W_GRAPHICS(data->gc)->handle;
	HFONT font;
	if (data->attr.font == 0) {
		font = (HFONT) w_toolkit_get_system_font(W_TOOLKIT(win_toolkit));
	} else {
		font = (HFONT) data->attr.font;
	}
	HFONT oldFont = SelectObject(hDC, font);
	HTHEME hTheme = 0;
	RECT rect;
	rect.left = data->bounds->x;
	rect.top = data->bounds->y;
	rect.right = data->bounds->x + data->bounds->width;
	rect.bottom = data->bounds->y + data->bounds->height;
	int headerHeight = data->bounds->height;
	if (hTheme != 0) {
		DrawThemeBackground(hTheme, hDC, EBP_NORMALGROUPHEAD, 0, &rect, 0);
	} else {
		HBRUSH oldBrush = SelectObject(hDC, GetSysColorBrush(COLOR_BTNFACE));
		PatBlt(hDC, rect.left, rect.top, rect.right - rect.left,
				rect.bottom - rect.top, PATCOPY);
		SelectObject(hDC, oldBrush);
	}
	w_imagelist *imagelist = data->imagelist;
	if (data->attr.image >= 0 && imagelist != 0) {
		rect.left += EXPANDBAR_TEXT_INSET;
		w_size sz;
		w_imagelist_get_size(imagelist, &sz);
		w_point pt;
		pt.x = rect.left;
		if (sz.height > headerHeight) {
			pt.y = rect.top + headerHeight - sz.height;
		} else {
			pt.y = rect.top + (headerHeight - sz.height) / 2;
		}
		w_imagelist_draw(imagelist, data->gc, data->attr.image, &pt, 0);
		rect.left += sz.width;
	}
	if (data->attr.text != 0) {
		rect.left += EXPANDBAR_TEXT_INSET;
		WCHAR *str;
		int newlength;
		_win_text_fix(data->attr.text, -1, W_ENCODING_UTF8, &str, &newlength);
		if (hTheme != 0) {
			DrawThemeText(hTheme, hDC, EBP_NORMALGROUPHEAD, 0, str, newlength,
			DT_VCENTER | DT_SINGLELINE, 0, &rect);
		} else {
			int oldBkMode = SetBkMode(hDC, TRANSPARENT);
			DrawTextW(hDC, str, newlength, &rect, DT_VCENTER | DT_SINGLELINE);
			SetBkMode(hDC, oldBkMode);
		}
		_win_text_free(data->attr.text, str, newlength);
	}
	int chevronSize = EXPANDBAR_CHEVRON_SIZE;
	rect.left = rect.right - chevronSize;
	rect.top = data->bounds->y + (headerHeight - chevronSize) / 2;
	rect.bottom = rect.top + chevronSize;
	if (hTheme != 0) {
		int partID =
				(data->state & W_THEME_STATE_PART_EXPANDED) != 0 ?
						EBP_NORMALGROUPCOLLAPSE : EBP_NORMALGROUPEXPAND;
		int stateID =
				(data->state & W_THEME_STATE_PART_HOT) != 0 ?
						EBNGC_HOT : EBNGC_NORMAL;
		RECT clipRect;
		DrawThemeBackground(hTheme, hDC, partID, stateID, &rect, &clipRect);
	} else {
		_win32_themedata_expanditem_draw_chevron(theme, mask, data, &rect);
	}
	if ((data->state & W_THEME_STATE_FOCUSED) != 0) {
		SetRect(&rect, data->bounds->x + 1, data->bounds->y + 1,
				data->bounds->x + data->bounds->width - 2,
				data->bounds->y + headerHeight - 2);
		DrawFocusRect(hDC, &rect);
	}
	if ((data->state & W_THEME_STATE_PART_EXPANDED) != 0) {
		//if (!_w_expandbar_is_app_themed(expandbar)) {
		HPEN pen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
		HPEN oldPen = SelectObject(hDC, pen);
		POINT points[4];
		points[0].x = data->bounds->x;
		points[0].y = data->bounds->y + headerHeight;
		points[1].x = data->bounds->x;
		points[1].y = data->bounds->y + headerHeight + data->bounds->height;
		points[2].x = data->bounds->x + data->bounds->width - 1;
		points[2].y = data->bounds->y + headerHeight + data->bounds->height;
		points[3].x = data->bounds->x + data->bounds->width - 1;
		points[3].y = data->bounds->y + headerHeight - 1;
		Polyline(hDC, points, 4);
		SelectObject(hDC, oldPen);
		DeleteObject(pen);
		//}
	}
	SelectObject(hDC, oldFont);
}
void _win32_themedata_expanditem_measure(w_theme *theme, int mask,
		w_themedata *data, w_size *result) {
	int width = EXPANDBAR_TEXT_INSET * 2 + EXPANDBAR_CHEVRON_SIZE;
	int heigth = EXPANDBAR_CHEVRON_SIZE;
	w_imagelist *imagelist = data->imagelist;
	if (imagelist != 0) {
		w_size sz;
		w_imagelist_get_size(imagelist, &sz);
		if (sz.width != 0) {
			width += EXPANDBAR_TEXT_INSET + sz.width;
		}
	}
	if (data->attr.text != 0) {
		HDC hDC = _W_GRAPHICS(data->gc)->handle;
		HTHEME hTheme = 0;
		WCHAR *str;
		int newlength;
		RECT rect;
		_win_text_fix(data->attr.text, -1, W_ENCODING_UTF8, &str, &newlength);
		if (hTheme != 0) {
			GetThemeTextExtent(hTheme, hDC, EBP_NORMALGROUPHEAD, 0, str,
					newlength, DT_SINGLELINE, NULL, &rect);
		} else {
			DrawTextW(hDC, str, newlength, &rect, DT_CALCRECT);
		}
		_win_text_free(data->attr.text, str, newlength);
		width += (rect.right - rect.left);
	}
	result->width = width;
	result->height = heigth;
}
void _win32_themedata_expanditem_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
}
int _win32_themedata_expanditem_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
}
_w_themedata_class _win32_themedata_fun[W_THEME_CLASS_LAST] = {
		[W_THEME_CLASS_UNKNOWN] = { }, //
		[W_THEME_CLASS_MENU] = { }, //
		[W_THEME_CLASS_MENUITEM] = { }, //
		[W_THEME_CLASS_BUTTON] = { }, //
		[W_THEME_CLASS_LABEL] = { }, //
		[W_THEME_CLASS_PROGRESSBAR] = { }, //
		[W_THEME_CLASS_SASH] = { }, //
		[W_THEME_CLASS_SLIDER] = { }, //
		[W_THEME_CLASS_SCROLLBAR] = { }, //
		[W_THEME_CLASS_TEXTEDIT] = { }, //
		[W_THEME_CLASS_COMPOSITE] = { }, //
		[W_THEME_CLASS_WEBVIEW] = { }, //
		[W_THEME_CLASS_TREEVIEW] = { }, //
		[W_THEME_CLASS_TREEITEM] = { }, //
		[W_THEME_CLASS_COLUMNITEM] = { }, //
		[W_THEME_CLASS_TABVIEW] = { }, //
		[W_THEME_CLASS_TABITEM] = { }, //
		[W_THEME_CLASS_COMBOBOX] = { }, //
		[W_THEME_CLASS_COOLBAR] = { }, //
		[W_THEME_CLASS_COOLITEM] = { }, //
		[W_THEME_CLASS_DATETIME] = { }, //
		[W_THEME_CLASS_EXPANDBAR] = { _win32_themedata_expandbar_compute_trim,
				_win32_themedata_expandbar_draw,
				_win32_themedata_expandbar_measure,
				_win32_themedata_expandbar_get_bounds,
				_win32_themedata_expandbar_hit }, //
		[W_THEME_CLASS_EXPANDITEM] = { _win32_themedata_expanditem_compute_trim,
				_win32_themedata_expanditem_draw,
				_win32_themedata_expanditem_measure,
				_win32_themedata_expanditem_get_bounds,
				_win32_themedata_expanditem_hit }, //
		[W_THEME_CLASS_GROUPBOX] = { }, //
		[W_THEME_CLASS_SPINNER] = { }, //
		[W_THEME_CLASS_LISTVIEW] = { }, //
		[W_THEME_CLASS_LISTITEM] = { }, //
		[W_THEME_CLASS_TOOLBAR] = { }, //
		[W_THEME_CLASS_TOOLITEM] = { }, //
		};
_win32_theme_part_id _win32_themedata_part_fun[W_THEME_CLASS_LAST] = {
		[W_THEME_CLASS_UNKNOWN] = 0, //
		[W_THEME_CLASS_MENU] = 0, //
		[W_THEME_CLASS_MENUITEM] = 0, //
		[W_THEME_CLASS_BUTTON] = 0, //
		[W_THEME_CLASS_LABEL] = 0, //
		[W_THEME_CLASS_PROGRESSBAR] = 0, //
		[W_THEME_CLASS_SASH] = 0, //
		[W_THEME_CLASS_SLIDER] = 0, //
		[W_THEME_CLASS_SCROLLBAR] = 0, //
		[W_THEME_CLASS_TEXTEDIT] = 0, //
		[W_THEME_CLASS_COMPOSITE] = 0, //
		[W_THEME_CLASS_WEBVIEW] = 0, //
		[W_THEME_CLASS_TREEVIEW] = 0, //
		[W_THEME_CLASS_TREEITEM] = 0, //
		[W_THEME_CLASS_COLUMNITEM] = 0, //
		[W_THEME_CLASS_TABVIEW] = 0, //
		[W_THEME_CLASS_TABITEM] = 0, //
		[W_THEME_CLASS_COMBOBOX] = 0, //
		[W_THEME_CLASS_COOLBAR] = 0, //
		[W_THEME_CLASS_COOLITEM] = 0, //
		[W_THEME_CLASS_DATETIME] = 0, //
		[W_THEME_CLASS_EXPANDBAR] = 0, //
		[W_THEME_CLASS_EXPANDITEM] = 0, //
		[W_THEME_CLASS_GROUPBOX] = 0, //
		[W_THEME_CLASS_SPINNER] = 0, //
		[W_THEME_CLASS_LISTVIEW] = 0, //
		[W_THEME_CLASS_LISTITEM] = 0, //
		[W_THEME_CLASS_TOOLBAR] = 0, //
		[W_THEME_CLASS_TOOLITEM] = 0, //
		};
void _win32_theme_dispose(w_theme *theme) {
}
const char* _win32_theme_get_name(w_theme *theme) {
	return "win32";
}
void _win32_theme_compute_trim(w_theme *theme, int mask, w_themedata *data,
		w_rect *result) {
}
void _win32_theme_draw(w_theme *theme, int mask, w_themedata *data) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& _win32_themedata_fun[data->clazz].draw != 0) {
		_win32_themedata_fun[data->clazz].draw(theme, mask, data);
	}
}
void _win32_theme_measure(w_theme *theme, int mask, w_themedata *data,
		w_size *result) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& _win32_themedata_fun[data->clazz].measure != 0) {
		_win32_themedata_fun[data->clazz].measure(theme, mask, data, result);
	}
}
void _win32_theme_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& _win32_themedata_fun[data->clazz].get_bounds) {
		_win32_themedata_fun[data->clazz].get_bounds(theme, mask, part, data,
				result);
	}
}
int _win32_theme_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& _win32_themedata_fun[data->clazz].hit != 0) {
		return _win32_themedata_fun[data->clazz].hit(theme, mask, data,
				position);
	}
	return W_THEME_PART_NOWHERE;
}
w_color _win32_theme_get_color(w_theme *theme, wuint colorid) {
	return 0;
}
w_font* _win32_theme_get_font(w_theme *theme) {
	return 0;
}
w_cursor* _win32_theme_get_cursor(w_theme *theme, wuint id) {
	return 0;
}
w_image* _win32_theme_get_image(w_theme *theme, wuint id) {
	return 0;
}
_w_theme_class _win32_theme_clazz = { //
		_win32_theme_dispose, //
				_win32_theme_get_name, //
				{ _win32_theme_compute_trim, //
						_win32_theme_draw, //
						_win32_theme_measure, //
						_win32_theme_get_bounds, //
						_win32_theme_hit }, //
				_win32_theme_get_color, //
				_win32_theme_get_font, //
				_win32_theme_get_cursor, //
				_win32_theme_get_image };
void _w_theme_init() {
	win_toolkit->theme = (w_theme*) &win_toolkit->win32theme;
	win_toolkit->win32theme.theme.clazz = &_win32_theme_clazz;

}
