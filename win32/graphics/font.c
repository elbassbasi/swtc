/*
 * Name:        font.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
/*
 * fontdata
 */
#define _LOGFONTW(x) ((LOGFONTW*)x)
void w_fontdata_init(w_fontdata *fontdata) {
	memset(fontdata, 0, sizeof(LOGFONTW));
}
wresult w_fontdata_create(w_fontdata *fontdata, const char *name, int length,
		int enc, int style, int size) {
	return w_fontdata_set(fontdata, name, length, enc, style, size);
}
void w_fontdata_dispose(w_fontdata *fontdata) {
	//nothing
}
int w_fontdata_get_height(w_fontdata *fontdata) {
	return _LOGFONTW(fontdata)->lfHeight;
}
wresult w_fontdata_get_name(w_fontdata *fontdata, w_alloc name, void *user_data,
		int enc) {
	return _win_text_set(_LOGFONTW(fontdata)->lfFaceName, -1, name, user_data,
			enc);
}
int w_fontdata_get_style(w_fontdata *fontdata) {
	int style = W_NORMAL;
	if (_LOGFONTW(fontdata)->lfWeight == 700)
		style |= W_BOLD;
	if (_LOGFONTW(fontdata)->lfItalic != 0)
		style |= W_ITALIC;
	return style;
}
wresult w_fontdata_set(w_fontdata *fontdata, const char *name, int length,
		int enc, int style, int size) {
	w_fontdata_set_name(fontdata, name, length, enc);
	w_fontdata_set_height(fontdata, size);
	w_fontdata_set_style(fontdata, style);
	return W_TRUE;
}
wresult w_fontdata_set_height(w_fontdata *fontdata, int height) {
	_LOGFONTW(fontdata)->lfHeight = height;
	return W_TRUE;
}
wresult w_fontdata_set_name(w_fontdata *fontdata, const char *name, int length,
		int enc) {
	const int lfFaceName_length = sizeof(_LOGFONTW(fontdata)->lfFaceName)
			/ sizeof(WCHAR);
	if ((enc & 0xFF) == W_ENCODING_UNICODE
			|| (enc & 0xFF) == W_ENCODING_PLATFORM) {
		if (length < 0)
			length = lstrlenW((WCHAR*) name);
		length = WMIN(lfFaceName_length - 1, length);
		lstrcpynW(_LOGFONTW(fontdata)->lfFaceName, (WCHAR*) name, length);
		_LOGFONTW(fontdata)->lfFaceName[length - 1] = 0;
	} else {
		size_t l = w_utf8_to_utf16(name, length,
		_LOGFONTW(fontdata)->lfFaceName, lfFaceName_length);
		_LOGFONTW(fontdata)->lfFaceName[lfFaceName_length - 1] = 0;
	}
	return W_TRUE;
}
wresult w_fontdata_set_style(w_fontdata *fontdata, int style) {
	if ((style & W_BOLD) == W_BOLD) {
		((LOGFONTW*) fontdata)->lfWeight = 700;
	} else {
		((LOGFONTW*) fontdata)->lfWeight = 0;
	}
	if ((style & W_ITALIC) == W_ITALIC) {
		((LOGFONTW*) fontdata)->lfItalic = 1;
	} else {
		((LOGFONTW*) fontdata)->lfItalic = 0;
	}
	return W_TRUE;
}
/*
 * font
 */
void w_font_dispose(w_font *font) {
	if (font != 0) {
		DeleteObject((HFONT) font);
	}
}
wresult w_font_is_ok(w_font *font) {
	if (font != 0) {
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult w_font_create(w_font **font, const char *name, int length, int enc,
		int style, int size) {
	LOGFONTW logfont;
	memset(&logfont, 0, sizeof(LOGFONTW));
	w_fontdata_set((w_fontdata*) &logfont, name, length, enc, style, size);
	return w_font_create_from_fontdata(font, (w_fontdata*) &logfont);
}
wresult w_font_create_from_fontdata(w_font **font, w_fontdata *fontdata) {
	*font = (w_font*) CreateFontIndirectW((LOGFONTW*) fontdata);
	if (*font == 0) {
		return W_ERROR_NO_HANDLES;
	} else
		return W_TRUE;
}
wresult w_font_get_fontdata(w_font *font, w_fontdata *fontdata) {
	GetObjectW((HFONT) font, sizeof(LOGFONTW), fontdata);
	return W_TRUE;
}
/*
 * font metrics
 */
#define _TEXTMETRIC(x) ((TEXTMETRICW*)x)
void w_fontmetrics_init(w_fontmetrics *metrics) {
	memset(metrics, 0, sizeof(TEXTMETRICW));
}
void w_fontmetrics_dispose(w_fontmetrics *metrics) {
	//nothings
}
int w_fontmetrics_get_ascent(w_fontmetrics *metrics) {
	return _TEXTMETRIC(metrics)->tmAscent
			- _TEXTMETRIC(metrics)->tmInternalLeading;
}
int w_fontmetrics_get_average_charwidth(w_fontmetrics *metrics) {
	return _TEXTMETRIC(metrics)->tmAveCharWidth;
}
int w_fontmetrics_get_descent(w_fontmetrics *metrics) {
	return _TEXTMETRIC(metrics)->tmDescent;
}
int w_fontmetrics_get_height(w_fontmetrics *metrics) {
	return _TEXTMETRIC(metrics)->tmHeight;
}
int w_fontmetrics_get_leading(w_fontmetrics *metrics) {
	return _TEXTMETRIC(metrics)->tmHeight - _TEXTMETRIC(metrics)->tmAscent
			+ _TEXTMETRIC(metrics)->tmInternalLeading
			- _TEXTMETRIC(metrics)->tmDescent;
}
