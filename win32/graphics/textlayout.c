/*
 * Name:        textlayout.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
#include "../widgets/toolkit.h"
#include <mlang.h>
#include <usp10.h>
#define MERGE_MAX 512
#define TOO_MANY_RUNS 1024
#define LTR_MARK 0x200E
#define RTL_MARK 0x200F
GUID _CLSID_CMultiLanguage = { 0x275c23e2, 0x3747, 0x11d0, { 0x9f, 0xea, 0x00,
		0xaa, 0x00, 0x3f, 0x86, 0x46 } };
GUID _IID_IMLangFontLink2 = { 0xDCCFC162, 0x2B38, 0x11d2, { 0xB7, 0xEC, 0x00,
		0xC0, 0x4F, 0x8F, 0x5D, 0x9A } };
typedef struct _w_textlayout_style {
	struct _w_textlayout_style *next;
	w_textstyle_range style;
} _w_textlayout_style;
typedef struct _w_textlayout_line {
	struct _w_textlayout_run *next_line;
	int lineOffset;
	int lineY;
	int lineWidth;
	int lineRunCount;
} _w_textlayout_line;
typedef struct _w_textlayout_run_complex {
	int x;
	int width;
	int glyphCount;
	SCRIPT_ANALYSIS analysis;
	SCRIPT_CACHE psc;
	HFONT fallbackFont;
} _w_textlayout_run_complex;
typedef struct _w_textlayout_run_glyph {
	int glyphCount;
	int pslaCount;
	SCRIPT_LOGATTR *psla; //length
	WORD *glyphs; //maxGlyphs
	WORD *clusters; //maxGlyphs
	SCRIPT_VISATTR *visAttrs; //maxGlyphs
	int *advances; //glyphCount
	GOFFSET *goffsets; //glyphCount
	int *justify; //glyphCount
} _w_textlayout_run_glyph;
enum {
	_W_LAYOUT_NEW_LINE = 1 << 13, //
	_W_LAYOUT_NEW_CLUSTER = 1 << 14, //
};
typedef struct _w_textlayout_run _w_textlayout_run;
struct _w_textlayout_run {
	_w_textlayout_run *next;
	_w_textlayout_run *prev;
	int flags;
	wushort start;
	wushort length;
	wushort size;
	char data[0];

};
_w_textlayout_line* _w_textlayout_run_get_line(_w_textlayout_run *run) {
	if (run->flags & _W_LAYOUT_NEW_LINE) {
		return (_w_textlayout_line*) &run->data[0];
	} else {
		return 0;
	}
}
typedef struct _w_textlayout_text {
	int length; //in utf16
	int utf8_length;
	union {
		struct {

		};
		int flags;
	};
	_w_textlayout_run *runs;
	_w_textlayout_style *styles;
	wchar text[0];
} _w_textlayout_text;
void _w_textlayout_style_override(_w_textlayout_style *st, w_textstyle *style) {
	if (style->mask_metrics) {
		st->style.style.mask_metrics = 1;
		memcpy(&st->style.style.metrics, &style->metrics,
				sizeof(w_glyphmetrics));
	}
	if (style->mask_rise) {
		st->style.style.mask_rise = 1;
		st->style.style.rise = style->rise;
	}
	if (style->mask_font) {
		st->style.style.mask_font = 1;
		st->style.style.font = style->font;
	}
	if (style->mask_foreground) {
		st->style.style.mask_foreground = 1;
		st->style.style.foreground = style->foreground;
	}
	if (style->mask_background) {
		st->style.style.mask_background = 1;
		st->style.style.background = style->background;
	}
	if (style->mask_underline) {
		st->style.style.mask_underline = 1;
		st->style.style.underline = style->underline;
		st->style.style.underlineStyle = style->underlineStyle;
		st->style.style.underlineColor = style->underlineColor;
	}
	if (style->mask_strikeout) {
		st->style.style.mask_strikeout = 1;
		st->style.style.strikeout = style->strikeout;
		st->style.style.strikeoutColor = style->strikeoutColor;
	}
}
wresult _w_textlayout_run_realloc(w_textlayout *textlayout,
		_w_textlayout_run **run, size_t newSize) {
	_w_textlayout_run *newrun, *next, *prev, *lastrun;
	lastrun = *run;
	next = lastrun->next;
	prev = lastrun->prev;
	newrun = realloc(lastrun, newSize);
	if (newrun == 0) {
		return W_ERROR_NO_MEMORY;
	}
	if (newrun != lastrun) {
		newrun->next = next;
		newrun->prev = prev;
		if (next != 0) {
			next->prev = newrun;
		}
		if (prev != 0) {
			prev->next = newrun;
		} else {
			_w_textlayout_text *_txt = _W_TEXTLAYOUT(textlayout)->text;
			_txt->runs = newrun;
		}
		*run = newrun;
	}
	newrun->size = newSize;
	return W_TRUE;
}
_w_textlayout_run* _w_textlayout_run_new(w_textlayout *textlayout,
		_w_textlayout_run *prev, size_t size) {
	_w_textlayout_run *run, *next;
	run = calloc(size, 1);
	if (run == 0)
		return 0;
	if (prev == 0) {
		_w_textlayout_text *_txt = _W_TEXTLAYOUT(textlayout)->text;
		_txt->runs = run;
		run->prev = 0;
		run->next = 0;
	} else {
		run->next = prev->next;
		run->prev = prev;
		prev->next = run;
		if (run->next != 0) {
			run->next->prev = run;
		}
	}
	run->size = size;
	return run;
}
/*
 *  Resolves text direction. If the nominal direction is LTR or RTL, no
 *  resolution is needed; if the nominal direction is "auto", have BidiUtil
 *  resolve it according to the first strong bidi character.
 */
int _w_textlayout_resolve_text_direction(w_textlayout *textlayout) {
	if (_W_TEXTLAYOUT(textlayout)->textDirection == W_AUTO_TEXT_DIRECTION) {
		_w_textlayout_text *_txt = _W_TEXTLAYOUT(textlayout)->text;
		int length = _txt->length;
		if (_txt->length == 0)
			return W_NONE;
		WCHAR *text = _txt->text;
		WCHAR rtlProbe[3] = { ' ', ' ', '1' };
		/*
		 * "Wide" version of win32 API can also run even on non-Unicode Windows,
		 * hence need for IsUnicode check here.
		 */
		WCHAR ltrProbe[3] = { 0x202b, 'a', ' ' };
		WCHAR numberProbe[3] = { 0x05d0, ' ', ' ' };
		GCP_RESULTSW result;
		memset(&result, 0, sizeof(result));
		result.lStructSize = sizeof(result);
		int nGlyphs = result.nGlyphs = 3;
		UINT lpOrder[3];
		result.lpOrder = lpOrder;
		HDC hdc = GetDC(NULL);
		int textDirection = W_NONE;
		for (int i = 0; i < length; i++) {
			char ch = text[i];
			rtlProbe[0] = ch;
			GetCharacterPlacementW(hdc, rtlProbe, 3, 0, &result, GCP_REORDER);
			if (result.lpOrder[0] == 2) {
				textDirection = W_RIGHT_TO_LEFT;
				break;
			}
			ltrProbe[2] = ch;
			GetCharacterPlacementW(hdc, ltrProbe, 3, 0, &result, GCP_REORDER);
			if (result.lpOrder[0] == 1) {
				numberProbe[2] = ch;
				GetCharacterPlacementW(hdc, numberProbe, 3, 0, &result,
				GCP_REORDER);
				if (result.lpOrder[0] == 0) {
					textDirection = W_LEFT_TO_RIGHT;
					break;
				}
			}
		}
		ReleaseDC(0, hdc);
		return textDirection;
	} else
		return _W_TEXTLAYOUT(textlayout)->textDirection;
}
void w_textlayout_runs_free(w_textlayout *textlayout) {

}
wresult w_textlayout_runs_merge(w_textlayout *textlayout, SCRIPT_ITEM *items,
		int itemCount, HDC srcHdc) {
	_w_textlayout_text *_txt = _W_TEXTLAYOUT(textlayout)->text;
	WCHAR *segmentsText = _txt->text;
	int start = 0, end = _txt->length, itemIndex = 0, styleIndex = 0;
	int itemLimit = -1;
	int nextItemIndex = 0;
	boolean linkBefore = W_FALSE;
	boolean merge = itemCount > TOO_MANY_RUNS;
	const SCRIPT_PROPERTIES *sp, **ppSp;
	int piNumScripts;
	ScriptGetProperties(&ppSp, &piNumScripts);
	_w_textlayout_run *_item, *prev = 0;
	_w_textlayout_style *style = _txt->styles, *_style;
	int _start, _lineBreak, _tab;
	SCRIPT_ANALYSIS _analysis;
	while (start < end) {
		_start = start;
		memcpy(&_analysis, &items[itemIndex].a, sizeof(SCRIPT_ANALYSIS));
		if (linkBefore) {
			_analysis.fLinkBefore = TRUE;
			linkBefore = FALSE;
		}
		wchar ch = segmentsText[start];
		_lineBreak = FALSE;
		_tab = FALSE;
		switch (ch) {
		case '\r':
		case '\n':
			_lineBreak = TRUE;
			break;
		case '\t':
			_tab = TRUE;
			break;
		}
		if (itemLimit == -1) {
			nextItemIndex = itemIndex + 1;
			itemLimit = items[nextItemIndex].iCharPos;
			if (nextItemIndex < itemCount && ch == '\r'
					&& segmentsText[itemLimit] == '\n') {
				nextItemIndex = itemIndex + 2;
				itemLimit = items[nextItemIndex].iCharPos;
			}
			if (nextItemIndex < itemCount && merge) {
				if (!_lineBreak) {
					sp = ppSp[_analysis.eScript];
					if (!sp->fComplex || _tab) {
						for (int i = 0; i < MERGE_MAX; i++) {
							if (nextItemIndex == itemCount)
								break;
							wchar c = segmentsText[itemLimit];
							if (c == '\n' || c == '\r')
								break;
							if (c == '\t' && c != _tab)
								break;
							sp = ppSp[items[nextItemIndex].a.eScript];
							if (!_tab && sp->fComplex)
								break;
							nextItemIndex++;
							itemLimit = items[nextItemIndex].iCharPos;
						}
					}
				}
			}
		}
		int styleLimit = -1;
		if (style != 0) {
			if (_start >= style->style.range.end) {
				style = style->next;
			}
			if (style != 0) {
				if (_start >= style->style.range.start) {
					_style = style;
					styleLimit = style->style.range.end;
				} else {
					_style = 0;
					styleLimit = style->style.range.start;
				}
			} else
				_style = 0;
		} else
			_style = 0;
		if (styleLimit == -1)
			styleLimit = end;
		if (styleLimit <= itemLimit) {
			styleIndex++;
			start = styleLimit;
			if (start < itemLimit && 0 < start && start < end) {
				wchar pChar = segmentsText[start - 1];
				wchar tChar = segmentsText[start];
				if (IsCharAlphaW(pChar) && IsCharAlphaW(tChar)) {
					_analysis.fLinkAfter = TRUE;
					linkBefore = TRUE;
				}
			}
		}
		if (itemLimit <= styleLimit) {
			itemIndex = nextItemIndex;
			start = itemLimit;
			itemLimit = -1;
		}
		_item = _w_textlayout_run_new(textlayout, prev, 0);
		if (_item == 0)
			return W_ERROR_NO_MEMORY;
		/*_item->start = _start;
		 _item->style = _style;
		 _item->lineBreak = _lineBreak;
		 _item->tab = _tab;
		 memcpy(&_item->analysis, &_analysis, sizeof(_analysis));
		 _item->length = start - _start;
		 prev = _item;
		 SelectObject(srcHdc, _w_textlayout_get_item_font(textlayout, _item));
		 w_textlayout_shape(textlayout, srcHdc, _item);*/
	}
	return W_TRUE;
}
wresult w_textlayout_runs_itemize(w_textlayout *textlayout, HDC srcHdc) {
	_w_textlayout_text *_txt = _W_TEXTLAYOUT(textlayout)->text;
	if (_txt->text == 0)
		return W_FALSE;
	wresult result = W_TRUE;
	WCHAR *segmentsText = _txt->text;
	int length = _txt->length;
	SCRIPT_CONTROL scriptControl;
	SCRIPT_STATE scriptState;
	memset(&scriptState, 0, sizeof(scriptState));
	memset(&scriptControl, 0, sizeof(scriptControl));
	int MAX_ITEM = length + 1;

	if ((_w_textlayout_resolve_text_direction(textlayout) & W_RIGHT_TO_LEFT)
			!= 0) {
		scriptState.uBidiLevel = 1;
		scriptState.fArabicNumContext = TRUE;
	}
	SCRIPT_ITEM *pItems = (SCRIPT_ITEM*) _w_toolkit_malloc(
			MAX_ITEM * sizeof(SCRIPT_ITEM));
	if (pItems == 0)
		return W_ERROR_NO_MEMORY;
	int pcItems;
	ScriptApplyDigitSubstitution(NULL, &scriptControl, &scriptState);

	HRESULT hr = ScriptItemize(segmentsText, length, MAX_ITEM, &scriptControl,
			&scriptState, pItems, &pcItems);
	if (hr != S_OK) {
		if (hr == E_OUTOFMEMORY) {
			result = W_ERROR_NO_MEMORY;
		} else
			result = W_ERROR_UNSPECIFIED;
	} else {
		wresult result = w_textlayout_runs_merge(textlayout, pItems, pcItems,
				srcHdc);
		if (result < 0)
			w_textlayout_runs_free(textlayout);
	}
	_w_toolkit_free(pItems, MAX_ITEM * sizeof(SCRIPT_ITEM));
	return result;
}
wresult w_textlayout_runs_compute(w_textlayout *textlayout, HDC hDC) {

}
int w_textlayout_get_line_indent(w_textlayout *textlayout, int lineIndex);
void w_textstyle_init(w_textstyle *style) {
	memset(style, 0, sizeof(w_textstyle));
	style->metrics.width = -1;
	style->metrics.ascent = -1;
	style->metrics.descent = -1;
}
void w_textlayout_init(w_textlayout *textlayout) {
	_W_TEXTLAYOUT(textlayout)->created = FALSE;
}
void w_textlayout_dispose(w_textlayout *textlayout) {
}
wresult w_textlayout_isok(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
	if (textlayout == 0)
		return W_FALSE;
	if (_W_TEXTLAYOUT(textlayout)->created == FALSE)
		return W_FALSE;
	return W_TRUE;
}
wresult w_textlayout_create(w_textlayout *textlayout) {
	if (textlayout == 0)
		return W_FALSE;
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
	if (layout->created == TRUE)
		w_textlayout_dispose(textlayout);
	memset(textlayout, 0, sizeof(struct _w_textlayout));
	layout->wrapWidth = -1;
	layout->ascentInPixels = -1;
	layout->descentInPixels = -1;
	layout->orientation = W_LEFT_TO_RIGHT;
	layout->textDirection = W_LEFT_TO_RIGHT;
	layout->created = TRUE;
	return W_TRUE;
}
wresult w_textlayout_draw(w_textlayout *textlayout, w_graphics *gc, int x,
		int y, w_text_selection *selection, int flags) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
void w_textlayout_draw_border(w_textlayout *textlayout, w_graphics *gc, int x,
		int y, int start, int end, int style, w_color color) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_alignment(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_ascent(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
void w_textlayout_get_bounds(w_textlayout *textlayout, int start, int end,
		w_rect *rect, int enc) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_descent(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
w_font* w_textlayout_get_font(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_indent(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_get_justify(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_level(w_textlayout *textlayout, int offset, int enc) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
void w_textlayout_get_line_bounds(w_textlayout *textlayout, int lineIndex,
		w_rect *rect) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_line_count(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_line_index(w_textlayout *textlayout, int offset, int enc) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_get_line_metrics(w_textlayout *textlayout, int lineIndex,
		w_fontmetrics *fontmetrics) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_line_offset(w_textlayout *textlayout, int lineIndex,
		int enc) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_get_location(w_textlayout *textlayout, int offset,
		int trailing, w_point *pt, int enc) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_next_offset(w_textlayout *textlayout, int offset,
		int movement, int enc) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_offset(w_textlayout *textlayout, w_point *point,
		int *trailing, int trailing_count, int enc) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_orientation(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_previous_offset(w_textlayout *textlayout, int offset,
		int movement, int enc) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_get_ranges(w_textlayout *textlayout, w_iterator *ranges) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_spacing(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_get_style(w_textlayout *textlayout, int offset,
		w_textstyle *style, int enc) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_tabs(w_textlayout *textlayout, int *tabs, int count) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_get_text(w_textlayout *textlayout, w_alloc text,
		void *user_data) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_text_direction(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_width(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
int w_textlayout_get_wrap_indent(w_textlayout *textlayout) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_alignment(w_textlayout *textlayout, int alignment) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_ascent(w_textlayout *textlayout, int ascent) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_descent(w_textlayout *textlayout, int descent) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_font(w_textlayout *textlayout, w_font *font) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_indent(w_textlayout *textlayout, int indent) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_justify(w_textlayout *textlayout, int justify) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_orientation(w_textlayout *textlayout,
		int orientation) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_spacing(w_textlayout *textlayout, int spacing) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_style(w_textlayout *textlayout, w_textstyle *style,
		int start, int end, int enc) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
	if (layout == 0)
		return W_ERROR_NO_HANDLES;
	if (layout->created == FALSE)
		return W_ERROR_NO_HANDLES;
	_w_textlayout_text *_txt = layout->text;
	if (_txt == 0)
		return W_FALSE;
	if (_txt->length == 0)
		return W_ERROR_INVALID_RANGE;
	if (start > end)
		return W_FALSE;
	int length = _txt->length;
	start = WMIN(WMAX(0, start), length);
	end = WMIN(WMAX(0, end), length);

	_w_textlayout_style *item = _txt->styles;
	_w_textlayout_style *prev = 0, *pprev = 0;
	_w_textlayout_style *modifyStart = 0;
	_w_textlayout_style *modifyStartPrev = 0;
	int lastend = -1;
	while (item != 0) {
		if (item->style.range.start >= start) {
			if (item->style.range.start == start) {
				modifyStart = item;
				modifyStartPrev = prev;
			} else {
				modifyStart = prev;
				modifyStartPrev = pprev;
			}
			break;
		}
		pprev = prev;
		prev = item;
		item = item->next;
	}
	if (modifyStart == 0) {
		modifyStart = malloc(sizeof(_w_textlayout_style));
		modifyStart->style.style.flags = 0;
		if (prev != 0) {
			modifyStart->next = prev->next;
			prev->next = modifyStart;
		} else {
			modifyStart->next = _txt->styles;
			_txt->styles = modifyStart;
		}
		modifyStart->style.range.start = start;
		if (modifyStart->next != 0) {
			int _end = modifyStart->next->style.range.start - 1;
			if (_end > end) {
				modifyStart->style.range.end = end;
				_w_textlayout_style_override(modifyStart, style);
				return W_TRUE;
			} else {
				modifyStart->style.range.end = _end;
			}

		} else {
			modifyStart->style.range.end = end;
			_w_textlayout_style_override(modifyStart, style);
			return W_TRUE;
		}
	}
	item = modifyStart;
	prev = modifyStartPrev;
	while (item != 0) {
		if (item->style.range.start > end) {
			break;
		}
		_w_textlayout_style_override(item, style);
		if ((lastend + 1) != item->style.range.start) {
			_w_textlayout_style *_item = malloc(sizeof(_w_textlayout_style));
			_item->style.style.flags = 0;
			if (prev != 0) {
				_item->next = prev->next;
				prev->next = item;
			} else {
				_item->next = _txt->styles;
				_txt->styles = _item;
			}
			_item->style.range.start = lastend + 1;
			_item->style.range.end = item->style.range.start - 1;
			_w_textlayout_style_override(_item, style);

		}
		lastend = item->style.range.end;
		prev = item;
		item = item->next;
	}
	return W_TRUE;
}
wresult w_textlayout_set_tabs(w_textlayout *textlayout, int *tabs, int count) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_text(w_textlayout *textlayout, const char *text,
		int count) {
	return w_textlayout_set_text_segments(textlayout, text, count, 0, 0, 0);
}
wresult w_textlayout_set_text_segments(w_textlayout *textlayout,
		const char *text, int len, int *segments, wuint *segmentschars,
		int nsegments) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
	if (textlayout == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_TEXTLAYOUT(textlayout)->created == FALSE)
		return W_ERROR_NO_HANDLES;
	if (_W_TEXTLAYOUT(textlayout)->text != 0) {
		free(_W_TEXTLAYOUT(textlayout)->text);
	}
	wchar *_text;
	int length = 0, _length, m;
	const char *in = text;
	while ((len < 0 || (m = text + len - in) > 0) && *in) {
		wuint wc = w_utf8_get_char_extended(in, len < 0 ? 6 : m);
		if (wc < 0x10000) {
			length += 1;
		} else {
			length += 2;
		}
		in = w_utf8_next(in);
	}
	_length = length + nsegments;
	const int __size = sizeof(WCHAR);
	layout->text = malloc(
			sizeof(struct _w_textlayout_text) + (_length + 1) * __size);
	if (layout->text == 0)
		return W_ERROR_NO_MEMORY;
	_w_textlayout_text *_txt = layout->text;
	_txt->runs = 0;
	_txt->styles = 0;
	_txt->length = _length;
	_text = _txt->text;
	if (nsegments == 0 || segments == 0) {
		length = 0;
		const char *in = text;
		while ((len < 0 || (m = text + len - in) > 0) && *in) {
			wint wc = w_utf8_get_char_extended(in, len < 0 ? 6 : m);
			if (wc < 0x10000) {
				_text[length++] = wc;
			} else {
				_text[length++] = (wc - 0x10000) / 0x400 + 0xd800;
				_text[length++] = (wc - 0x10000) % 0x400 + 0xdc00;
			}
			in = w_utf8_next(in);
		}
		_text[length] = 0;
	} else {
		int charCount = 0, segmentCount = 0;
		wint defaultSeparator =
				(_w_textlayout_resolve_text_direction(textlayout)
						& W_RIGHT_TO_LEFT) != 0 ? 0x200F : 0x200E;
		wchar *newChars = _txt->text;
		const char *oldChars = text;

		while (charCount < len) {
			if (segmentCount < nsegments
					&& charCount == segments[segmentCount]) {
				wchar separator =
						segmentschars != 0 ?
								segmentschars[segmentCount] : defaultSeparator;
				newChars[charCount + segmentCount++] = separator;
			} else {
				wint wc = w_utf8_get_next_char(oldChars, &oldChars);
				if (wc < 0x10000) {
					newChars[charCount + segmentCount] = wc;
				} else {
					newChars[charCount + segmentCount] = (wc - 0x10000) / 0x400
							+ 0xd800;
					charCount++;
					newChars[charCount + segmentCount] = (wc - 0x10000) % 0x400
							+ 0xdc00;
				}
				charCount++;
			}
		}
		while (segmentCount < nsegments) {
			segments[segmentCount] = charCount;
			wchar separator =
					segmentschars != 0 ?
							segmentschars[segmentCount] : defaultSeparator;
			newChars[charCount + segmentCount++] = separator;
		}
	}
	layout->is_complex = ScriptIsComplex(_text, _txt->length,
	SIC_COMPLEX) == S_OK;
	return W_TRUE;
}
wresult w_textlayout_set_text_direction(w_textlayout *textlayout,
		int textDirection) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_width(w_textlayout *textlayout, int width) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
wresult w_textlayout_set_wrap_indent(w_textlayout *textlayout, int wrapIndent) {
	_w_textlayout *layout = _W_TEXTLAYOUT(textlayout);
}
