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
typedef void (*_win32_theme_draw_background)(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds, wuint clazz);
typedef struct _win32_theme_clazz_info {
	_win32_theme_part_id partId;
	_win32_theme_draw_background draw_background;
} _win32_theme_clazz_info;
void _win32_themedata_part_id(w_theme *theme, w_themedata *data, int *ids,
		int part) {

}
void _win32_theme_button_part_id(w_theme *theme, w_themedata *data, int *ids,
		int part) {
	int state = data->state;
	int style = data->style;
	int iPartId = 0, iStateId = 0;
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		if ((style & W_PUSH) != 0) {
			iPartId = BP_PUSHBUTTON;
			iStateId = PBS_NORMAL;
			if ((state & W_THEME_DEFAULTED) != 0
					&& (state & W_THEME_ACTIVE) != 0)
				iStateId = PBS_DEFAULTED;
			if ((state & W_THEME_HOT) != 0)
				iStateId = PBS_HOT;
			if ((state & W_THEME_PRESSED) != 0)
				iStateId = PBS_PRESSED;
			if ((state & W_THEME_DISABLED) != 0)
				iStateId = PBS_DISABLED;
		}
		if ((style & W_RADIO) != 0) {
			iPartId = BP_RADIOBUTTON;
		}
		if ((style & W_CHECK) != 0) {
			iPartId = BP_CHECKBOX;
		}
		if ((style & (W_CHECK | W_RADIO)) != 0) {
			if ((state & W_THEME_SELECTED) != 0) {
				iStateId = CBS_CHECKEDNORMAL;
				if ((state & W_THEME_HOT) != 0)
					iStateId = CBS_CHECKEDHOT;
				if ((state & W_THEME_PRESSED) != 0)
					iStateId = CBS_CHECKEDPRESSED;
				if ((state & W_THEME_DISABLED) != 0)
					iStateId = CBS_CHECKEDDISABLED;
			} else {
				iStateId = CBS_UNCHECKEDNORMAL;
				if ((state & W_THEME_HOT) != 0)
					iStateId = CBS_UNCHECKEDHOT;
				if ((state & W_THEME_PRESSED) != 0)
					iStateId = CBS_UNCHECKEDPRESSED;
				if ((state & W_THEME_DISABLED) != 0)
					iStateId = CBS_UNCHECKEDDISABLED;
			}
		}
	} else {
		if ((style & W_PUSH) != 0) {
			iPartId = DFC_BUTTON;
			iStateId = DFCS_BUTTONPUSH;
			if ((state & W_THEME_DEFAULTED) != 0
					&& (state & W_THEME_ACTIVE) != 0)
				iStateId = PBS_DEFAULTED;
			if ((state & W_THEME_HOT) != 0)
				iStateId |= DFCS_HOT;
			if ((state & W_THEME_PRESSED) != 0)
				iStateId |= DFCS_PUSHED;
			if ((state & W_THEME_DISABLED) != 0)
				iStateId |= DFCS_INACTIVE;
		}
		if ((style & W_RADIO) != 0) {
			iPartId = DFC_BUTTON;
			iStateId = DFCS_BUTTONRADIO;
		}
		if ((style & W_CHECK) != 0) {
			iPartId = DFC_BUTTON;
			iStateId = DFCS_BUTTONCHECK;
		}
		if ((style & (W_CHECK | W_RADIO)) != 0) {
			if ((state & W_THEME_SELECTED) != 0) {
				iStateId |= DFCS_CHECKED;
				if ((state & W_THEME_HOT) != 0)
					iStateId |= DFCS_HOT;
				if ((state & W_THEME_PRESSED) != 0)
					iStateId |= DFCS_PUSHED;
				if ((state & W_THEME_DISABLED) != 0)
					iStateId |= DFCS_INACTIVE;
			} else {
				//iStateId |= CBS_UNCHECKEDNORMAL;
				if ((state & W_THEME_HOT) != 0)
					iStateId |= DFCS_HOT;
				if ((state & W_THEME_PRESSED) != 0)
					iStateId |= DFCS_PUSHED;
				if ((state & W_THEME_DISABLED) != 0)
					iStateId |= DFCS_INACTIVE;
			}
		}
	}
	ids[0] = iPartId;
	ids[1] = iStateId;
}
void _win32_theme_button_draw_background(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds, wuint clazz) {
	RECT rect;
	int part[2];
	rect.left = bounds->x;
	rect.right = bounds->x + bounds->width;
	rect.top = bounds->y;
	rect.bottom = bounds->y + bounds->height;
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		HTHEME hTheme = OpenThemeData(NULL, L"BUTTON");
		if (hTheme != 0) {
			_win32_theme_button_part_id(theme, data, part,
					W_THEME_WIDGET_WHOLE);
			if ((data->style & (W_CHECK | W_RADIO)) != 0) {
				SIZE size;
				GetThemePartSize(hTheme, _W_GRAPHICS(gc)->handle, part[0],
						part[1], &rect, TS_DRAW, &size);
				rect.right = rect.left + size.cx;
				DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, part[0],
						part[1], &rect, 0);
				rect.left = rect.right + 3;
				rect.right = rect.left + bounds->width - size.cx - 3;
			} else {
				DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, part[0],
						part[1], &rect, 0);
			}
			if (data->clientArea != 0) {
				RECT contentRect;
				GetThemeBackgroundContentRect(hTheme, _W_GRAPHICS(gc)->handle,
						part[0], part[1], &rect, &contentRect);
				data->clientArea->x = contentRect.left;
				data->clientArea->y = contentRect.top;
				data->clientArea->width = contentRect.right - contentRect.left;
				data->clientArea->height = contentRect.bottom - contentRect.top;
			}
		}
		CloseThemeData(hTheme);
	} else {
		_win32_theme_button_part_id(theme, data, part, W_THEME_WIDGET_WHOLE);
		DrawFrameControl(_W_GRAPHICS(gc)->handle, &rect, part[0], part[1]);
	}
}
_win32_theme_clazz_info __win32_theme_clazz_info[] = {
		[_W_THEME_CLASS_INTERNAL_UNKNOWN] = { }, //
		[_W_THEME_CLASS_INTERNAL_MENU] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_BUTTON] = { _win32_theme_button_part_id,
				_win32_theme_button_draw_background }, //
		[_W_THEME_CLASS_INTERNAL_LABEL] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_PROGRESSBAR] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_SASH] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_SLIDER] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_SCROLLBAR] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_TEXTEDIT] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_COMPOSITE] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_BROWSER] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_TREEVIEW] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_TABVIEW] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_TABITEM] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_COMBOBOX] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_COOLBAR] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_DATETIME] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_EXPANDBAR] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_GROUPBOX] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_SPINNER] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_LISTVIEW] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_TOOLBAR] = { _win32_themedata_part_id, }, //
		[_W_THEME_CLASS_INTERNAL_TOOLITEM] = { _win32_themedata_part_id, }, //
		};
void _win32_theme_dispose(w_theme *theme) {
}
const char* _win32_theme_get_name(w_theme *theme) {
	return "win32";
}
void _win32_theme_compute_trim(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *result) {
}
void _win32_theme_draw_background_0(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds) {
	wuint id = w_theme_internal_class_id(data->clazz);
	_win32_theme_draw_background draw_background =
			__win32_theme_clazz_info[id].draw_background;
	if (draw_background != 0) {
		draw_background(theme, data, gc, bounds, id);
	}
}
void _win32_theme_draw_focus(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds) {
}
void _win32_theme_draw_image(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds, w_image *image, int flags) {
}
void _win32_theme_draw_text(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds, const char *text, int length, int flags) {
	_win32_theme_part_id part_id = _win32_theme_button_part_id;
	size_t charsength = w_utf8_to_utf16(text, length, 0, 0);
	WCHAR *chars = _w_toolkit_malloc((charsength + 1) * sizeof(WCHAR));
	if (chars != 0) {
		w_utf8_to_utf16(text, length, chars, charsength + 1);
		int textFlags = DT_SINGLELINE;
		if ((flags & W_THEME_DRAW_LEFT) != 0)
			textFlags |= DT_LEFT;
		if ((flags & W_THEME_DRAW_HCENTER) != 0)
			textFlags |= DT_CENTER;
		if ((flags & W_THEME_DRAW_RIGHT) != 0)
			textFlags |= DT_RIGHT;
		if ((flags & W_THEME_DRAW_TOP) != 0)
			textFlags |= DT_TOP;
		if ((flags & W_THEME_DRAW_BOTTOM) != 0)
			textFlags |= DT_BOTTOM;
		if ((flags & W_THEME_DRAW_VCENTER) != 0)
			textFlags |= DT_VCENTER;
		RECT rect;
		rect.left = bounds->x;
		rect.right = bounds->x + bounds->width;
		rect.top = bounds->y;
		rect.bottom = bounds->y + bounds->height;
		/*HFONT oldFont = (HFONT) SelectObject(_W_GRAPHICS(gc)->handle,
		 win_toolkit->systemfont.handle);*/
		if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
			HTHEME hTheme = OpenThemeData(NULL, L"BUTTON");
			int part[2];
			part_id(theme,data, part, W_THEME_WIDGET_WHOLE);
			int iPartId = part[0];
			int iStateId = part[1];
			DrawThemeText(hTheme, _W_GRAPHICS(gc)->handle, iPartId, iStateId,
					chars, charsength, textFlags, 0, &rect);
		} else {
			RECT r;
			memcpy(&r, &rect, sizeof(RECT));
			DrawTextW(_W_GRAPHICS(gc)->handle, chars, charsength, &r,
					textFlags /*| DT_CALCRECT*/);
			/*int width = r.right - r.left;
			 int height = r.bottom - r.top;
			 HBITMAP hBitmap = CreateCompatibleBitmap(_W_GRAPHICS(gc)->handle,
			 width, height);
			 if (hBitmap != 0) {
			 HDC memDC = CreateCompatibleDC(_W_GRAPHICS(gc)->handle);
			 HBITMAP hOldBitmap = SelectObject(memDC, hBitmap);
			 PatBlt(memDC, 0, 0, width, height, BLACKNESS);
			 SetBkMode(memDC, TRANSPARENT);
			 SelectObject(memDC,
			 GetCurrentObject(_W_GRAPHICS(gc)->handle, OBJ_FONT));
			 DrawTextW(memDC, chars, charsength, &r, textFlags);
			 BitBlt(_W_GRAPHICS(gc)->handle, 0, 0, width,
			 height, memDC, 0, 0, SRCINVERT);
			 SelectObject(memDC, hOldBitmap);
			 DeleteDC(memDC);
			 DeleteObject(hBitmap);
			 }*/
		}
		//SelectObject(_W_GRAPHICS(gc)->handle, oldFont);
		_w_toolkit_free(chars, (charsength + 1) * sizeof(WCHAR));
	}
}
void _win32_theme_get_bounds(w_theme *theme, w_themedata *data, int part,
		w_rect *bounds, w_rect *result) {
}
int _win32_theme_get_selection(w_theme *theme, w_themedata *data,
		w_point *offset, w_rect *bounds) {
	return 0;
}
int _win32_theme_hit_background(w_theme *theme, w_themedata *data,
		w_point *position, w_rect *bounds) {
	return 0;
}
void _win32_theme_measure_text(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds, w_rect *result, const char *text,
		size_t length, int flags) {
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
_w_theme_class _win32_theme_clazz = { _win32_theme_dispose,
		_win32_theme_get_name, _win32_theme_compute_trim,
		_win32_theme_draw_background_0, _win32_theme_draw_focus,
		_win32_theme_draw_image, _win32_theme_draw_text,
		_win32_theme_get_bounds, _win32_theme_get_selection,
		_win32_theme_hit_background, _win32_theme_measure_text,
		_win32_theme_get_color, _win32_theme_get_font, _win32_theme_get_cursor,
		_win32_theme_get_image };
void _w_theme_init() {
	win_toolkit->theme = (w_theme*) &win_toolkit->win32theme;
	win_toolkit->win32theme.theme.clazz = &_win32_theme_clazz;

}
