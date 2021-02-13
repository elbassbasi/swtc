/*
 * Name:        font.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
#include "../widgets/toolkit.h"
/*
 * fontdata
 */
void w_fontdata_init(w_fontdata *fontdata) {
	memset(fontdata, 0, sizeof(_w_fontdata) + 1);
}
wresult w_fontdata_create(w_fontdata *fontdata, const char *name, int length,
		int enc, int style, int size) {
	return w_fontdata_set(fontdata, name, length, enc, style, size);
}
void w_fontdata_dispose(w_fontdata *fontdata) {
	//nothing
}
int w_fontdata_get_height(w_fontdata *fontdata) {
	return _W_FONTDATA(fontdata)->height;
}
wresult w_fontdata_get_name(w_fontdata *fontdata, w_alloc name, void *user_data,
		int enc) {
	size_t l = strlen(_W_FONTDATA(fontdata)->name);
	char *ret = 0;
	size_t newl = name(user_data, l + 1, (void**) &ret);
	if (ret != 0) {
		memcpy(ret, _W_FONTDATA(fontdata)->name, newl - 1);
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
	_W_FONTDATA(fontdata)->height = height;
	return W_TRUE;
}
wresult w_fontdata_set_name(w_fontdata *fontdata, const char *name, int length,
		int enc) {
	int l = strlen(name);
	const int family_l = sizeof(w_fontdata) - sizeof(struct _w_fontdata);
	if (l >= (family_l - 1))
		l = family_l - 1;
	memcpy(_W_FONTDATA(fontdata)->name, name, l);
	_W_FONTDATA(fontdata)->name[l] = 0;
	return W_TRUE;
}
wresult w_fontdata_set_style(w_fontdata *fontdata, int style) {
	_W_FONTDATA(fontdata)->style = style;
	return W_TRUE;
}
/*
 * font
 */
void _w_font_add_traits_0(w_font *font, NSMutableAttributedString *attrStr,
		NSRange *range) {
	_w_font_desc *desc = _W_FONT(font)->desc;
	if ((desc->extraTraits & NSBoldFontMask) != 0) {
		NSMutableAttributedString_addAttribute(attrStr,
				(NSString*) _NSStrokeWidthAttributeName,
				(NSObject*) mac_toolkit->SYNTHETIC_BOLD, range);
	}
	if ((desc->extraTraits & NSItalicFontMask) != 0) {
		NSMutableAttributedString_addAttribute(attrStr,
				(NSString*) _NSObliquenessAttributeName,
				(NSObject*) mac_toolkit->SYNTHETIC_ITALIC, range);
	}
}
void _w_font_add_traits_1(w_font *font, NSMutableDictionary *dict) {
	_w_font_desc *desc = _W_FONT(font)->desc;
	if ((desc->extraTraits & NSBoldFontMask) != 0) {
		NSMutableDictionary_setObject(dict,
				(NSObject*) mac_toolkit->SYNTHETIC_BOLD,
				(NSObject*) _NSStrokeWidthAttributeName);
	}
	if ((desc->extraTraits & NSItalicFontMask) != 0) {
		NSMutableDictionary_setObject(dict,
				(NSObject*) mac_toolkit->SYNTHETIC_ITALIC,
				(NSObject*) _NSObliquenessAttributeName);
	}
}
void w_font_init(w_font *font) {
	_W_FONT(font)->handle = 0;
	_W_FONT(font)->desc = 0;
}
void w_font_dispose(w_font *font) {
	if (_W_FONT(font)->handle != 0) {
		NSObject_release(NSOBJECT(_W_FONT(font)->handle));
		if (_W_FONT(font)->desc != 0)
			free(_W_FONT(font)->desc);
		_W_FONT(font)->handle = 0;
		_W_FONT(font)->desc = 0;
	}
}
wresult w_font_is_ok(w_font *font) {
	return _W_FONT(font)->handle != 0;
}
wresult w_font_create_0(w_font *font, NSString *family, int style, int height,
		NSString *nsName) {
	if (height < 0)
		return W_ERROR_INVALID_ARGUMENT;
	float size = height;
	NSFont *handle = 0;
	if (nsName != 0) {
		handle = NSFont_fontWithName(nsName, size);
	} else {
		NSFont *nsFont = NSFont_fontWithName(family, size);
		if (nsFont == 0)
			nsFont = NSFont_systemFontOfSize(size);
		NSFontManager *manager = NSFontManager_sharedFontManager();
		_w_font_desc *desc = calloc(1, sizeof(_w_font_desc));
		if (desc == 0)
			return W_ERROR_NO_MEMORY;
		_W_FONT(font)->desc = desc;
		if (nsFont != 0) {
			if ((style & (W_BOLD | W_ITALIC)) == 0) {
				handle = nsFont;
			} else {
				int traits = 0;
				if ((style & W_ITALIC) != 0)
					traits |= NSItalicFontMask;
				if ((style & W_BOLD) != 0)
					traits |= NSBoldFontMask;
				handle = NSFontManager_convertFont(manager, nsFont, traits);
				if ((style & W_ITALIC) != 0
						&& (handle == 0
								|| (NSFontManager_traitsOfFont(manager, handle)
										& NSItalicFontMask) == 0)) {
					traits &= ~NSItalicFontMask;
					handle = 0;
					if ((style & W_BOLD) != 0) {
						handle = NSFontManager_convertFont(manager, nsFont,
								traits);
					}
				}
				if ((style & W_BOLD) != 0 && handle == 0) {
					traits &= ~NSBoldFontMask;
					if ((style & W_ITALIC) != 0) {
						traits |= NSItalicFontMask;
						handle = NSFontManager_convertFont(manager, nsFont,
								traits);
					}
				}
				if (handle == 0)
					handle = nsFont;
			}
		}
		if (handle == 0) {
			handle = NSFont_systemFontOfSize(size);
		}
		if ((style & W_ITALIC) != 0
				&& (NSFontManager_traitsOfFont(manager, handle)
						& NSItalicFontMask) == 0) {
			desc->extraTraits |= NSItalicFontMask;
		}
		if ((style & W_BOLD) != 0
				&& (NSFontManager_traitsOfFont(manager, handle) & NSBoldFontMask)
						== 0) {
			desc->extraTraits |= NSBoldFontMask;
		}
	}
	if (handle == 0) {
		handle = mac_toolkit->systemFont.handle;
	}
	NSObject_retain(NSOBJECT(handle));
	_W_FONT(font)->handle = handle;
	return W_TRUE;
}
wresult w_font_create(w_font *font, const char *name, int length, int enc,
		int style, int height) {
	w_font_dispose(font);
	NSString *family = NSString_new(name, length, enc);
	wresult result = w_font_create_0(font, family, style, height, 0);
	NSObject_release(NSOBJECT(family));
	return result;
}
wresult w_font_create_from_fontdata(w_font *font, w_fontdata *fontdata) {
	w_font_dispose(font);
	_w_fontdata *d = (_w_fontdata*) fontdata;
	NSString *family = NSString_new(d->name, -1, W_ENCODING_UTF8);
	NSString *nsName = d->nsName;
	wresult result = w_font_create_0(font, family, d->style, d->height, nsName);
	NSObject_release(NSOBJECT(family));
	return result;
}
wresult w_font_get_fontdata(w_font *font, w_fontdata *fontdata) {
	NSFont *handle = _W_FONT(font)->handle;
	_w_font_desc *desc = _W_FONT(font)->desc;
	_w_fontdata *d = (_w_fontdata*) fontdata;
	NSString *family = NSFont_familyName(handle);
	d->nsName = NSFont_fontName(handle);
	NSFontManager *manager = NSFontManager_sharedFontManager();
	int traits = NSFontManager_traitsOfFont(manager, handle);
	int style = W_NORMAL;
	if ((traits & NSItalicFontMask) != 0)
		style |= W_ITALIC;
	if ((traits & NSBoldFontMask) != 0)
		style |= W_BOLD;
	if ((desc->extraTraits & NSItalicFontMask) != 0)
		style |= W_ITALIC;
	if ((desc->extraTraits & NSBoldFontMask) != 0)
		style |= W_BOLD;
	d->style = style;
	d->height = NSFont_pointSize(handle);
	return W_TRUE;
}
/*
 * font metrics
 */
void w_fontmetrics_init(w_fontmetrics *metrics) {
	memset(metrics, 0, sizeof(_w_font_metrics));
}
void w_fontmetrics_dispose(w_fontmetrics *metrics) {
	//nothings
}
int w_fontmetrics_get_ascent(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->ascent;
}
int w_fontmetrics_get_average_charwidth(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->averageCharWidth;
}
int w_fontmetrics_get_descent(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->descent;
}
int w_fontmetrics_get_height(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->height;
}
int w_fontmetrics_get_leading(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->leading;
}
