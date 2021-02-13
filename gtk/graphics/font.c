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
void w_fontdata_init(w_fontdata *fontdata) {
	memset(fontdata, 0, sizeof(struct _w_fontdata));
}
wresult w_fontdata_create(w_fontdata *fontdata, const char *name, int length,
		int enc, int style, int size) {
	return w_fontdata_set(fontdata, name, length, enc, style, size);
}
void w_fontdata_dispose(w_fontdata *fontdata) {
	//nothing
}
int w_fontdata_get_height(w_fontdata *fontdata) {
	return _W_FONTDATA(fontdata)->size;
}
wresult w_fontdata_get_name(w_fontdata *fontdata, w_alloc name, void *user_data,
		int enc) {
	size_t l = strlen(_W_FONTDATA(fontdata)->family);
	char *ret = 0;
	size_t newl = name(user_data, l + 1, (void**) &ret);
	if (ret != 0) {
		memcpy(ret, _W_FONTDATA(fontdata)->family, newl - 1);
		ret[newl - 1] = 0;
		return W_TRUE;
	}
	return W_FALSE;
}
int w_fontdata_get_style(w_fontdata *fontdata) {
	return _W_FONTDATA(fontdata)->style;
}
wresult w_fontdata_set(w_fontdata *fontdata, const char *name, int length,
		int enc, int style, int size) {
	w_fontdata_set_name(fontdata, name, length, enc);
	w_fontdata_set_height(fontdata, size);
	w_fontdata_set_style(fontdata, style);
	return W_TRUE;
}
wresult w_fontdata_set_height(w_fontdata *fontdata, int height) {
	_W_FONTDATA(fontdata)->size = height;
	return W_TRUE;
}
wresult w_fontdata_set_name(w_fontdata *fontdata, const char *name, int length,
		int enc) {
	int l = strlen(name);
	const int family_l = sizeof(w_fontdata) - sizeof(struct _w_fontdata);
	if (l >= (family_l - 1))
		l = family_l - 1;
	memcpy(_W_FONTDATA(fontdata)->family, name, l);
	_W_FONTDATA(fontdata)->family[l] = 0;
	return W_TRUE;
}
wresult w_fontdata_set_style(w_fontdata *fontdata, int style) {
	_W_FONTDATA(fontdata)->style = PANGO_STYLE_NORMAL;
	int pangoWeight = PANGO_WEIGHT_NORMAL;
	if ((style & W_ITALIC) != 0)
		_W_FONTDATA(fontdata)->style = PANGO_STYLE_ITALIC;
	if ((style & W_ROMAN) != 0)
		_W_FONTDATA(fontdata)->style = PANGO_STYLE_OBLIQUE;
	if ((style & W_BOLD) != 0)
		_W_FONTDATA(fontdata)->weight = PANGO_WEIGHT_BOLD;
	return W_TRUE;
}
/*
 * font
 */
void w_font_init(w_font *font) {
	_W_FONT(font)->handle = 0;
}
void w_font_dispose(w_font *font) {
	if (_W_FONT(font)->handle != 0) {
		pango_font_description_free(_W_FONT(font)->handle);
		_W_FONT(font)->handle = 0;
	}
}
wresult w_font_is_ok(w_font *font) {
	return _W_FONT(font)->handle != 0;
}
wresult w_font_create(w_font *font, const char *name, int length, int enc,
		int style, int size) {
	w_font_dispose(font);
	if (name == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (size < 0)
		return W_ERROR_INVALID_ARGUMENT;
	_W_FONT(font)->handle = pango_font_description_new();
	if (_W_FONT(font)->handle == 0)
		return W_ERROR_NO_HANDLES;
	pango_font_description_set_family(_W_FONT(font)->handle, name);
	if (size > 0) {
		pango_font_description_set_size(_W_FONT(font)->handle,
				(int) (0.5f + size * PANGO_SCALE));
	}
	pango_font_description_set_stretch(_W_FONT(font)->handle,
			PANGO_STRETCH_NORMAL);
	PangoStyle pangoStyle = PANGO_STYLE_NORMAL;
	PangoWeight pangoWeight = PANGO_WEIGHT_NORMAL;
	if ((style & W_ITALIC) != 0)
		pangoStyle = PANGO_STYLE_ITALIC;
	if ((style & W_ROMAN) != 0)
		pangoStyle = PANGO_STYLE_OBLIQUE;
	if ((style & W_BOLD) != 0)
		pangoWeight = PANGO_WEIGHT_BOLD;
	pango_font_description_set_style(_W_FONT(font)->handle, pangoStyle);
	pango_font_description_set_weight(_W_FONT(font)->handle, pangoWeight);
	return W_TRUE;
}
wresult w_font_create_from_fontdata(w_font *font, w_fontdata *fontdata) {
	w_font_dispose(font);
	_W_FONT(font)->handle = pango_font_description_new();
	if (_W_FONT(font)->handle == 0)
		return W_ERROR_NO_HANDLES;
	pango_font_description_set_family(_W_FONT(font)->handle,
	_W_FONTDATA(fontdata)->family);
	pango_font_description_set_size(_W_FONT(font)->handle,
			(int) (0.5f + _W_FONTDATA(fontdata)->size * PANGO_SCALE));
	pango_font_description_set_stretch(_W_FONT(font)->handle,
			PANGO_STRETCH_NORMAL);
	pango_font_description_set_style(_W_FONT(font)->handle,
	_W_FONTDATA(fontdata)->style);
	pango_font_description_set_weight(_W_FONT(font)->handle,
	_W_FONTDATA(fontdata)->weight);
	return W_TRUE;
}
wresult w_font_get_fontdata(w_font *font, w_fontdata *fontdata) {
	if (_W_FONT(font)->handle == 0)
		return W_ERROR_NO_HANDLES;
	w_fontdata_set_name(fontdata,
			pango_font_description_get_family(_W_FONT(font)->handle), -1,
			W_ENCODING_UTF8);
	_W_FONTDATA(fontdata)->size = pango_font_description_get_size(
	_W_FONT(font)->handle) / PANGO_SCALE;
	_W_FONTDATA(fontdata)->stretch = pango_font_description_get_stretch(
	_W_FONT(font)->handle);
	_W_FONTDATA(fontdata)->style = pango_font_description_get_style(
	_W_FONT(font)->handle);
	_W_FONTDATA(fontdata)->weight = pango_font_description_get_weight(
	_W_FONT(font)->handle);
	return W_TRUE;
}
/*
 * font metrics
 */
void w_fontmetrics_init(w_fontmetrics *metrics) {
	_W_FONTMETRICS(metrics)->ascentInPoints = 0;
	_W_FONTMETRICS(metrics)->averageCharWidthInPoints = 0;
	_W_FONTMETRICS(metrics)->descentInPoints = 0;
}
void w_fontmetrics_dispose(w_fontmetrics *metrics) {
	//nothings
}
int w_fontmetrics_get_ascent(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->ascentInPoints;
}
int w_fontmetrics_get_average_charwidth(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->averageCharWidthInPoints;
}
int w_fontmetrics_get_descent(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->descentInPoints;
}
int w_fontmetrics_get_height(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->ascentInPoints
			+ _W_FONTMETRICS(metrics)->descentInPoints;
}
int w_fontmetrics_get_leading(w_fontmetrics *metrics) {
	return 0; // Pango has no concept of "leading"
}
