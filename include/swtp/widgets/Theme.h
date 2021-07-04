/*
 * Theme.h
 *
 *  Created on: 8 nov. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_THEME_H_
#define SWTP_WIDGETS_THEME_H_
#include "Event.h"
class WTheme;
class WItemAttr {
public:
	WFont *font;
	WColor background;
	WColor foreground;
	int image;
	int enc;
	union {
		struct {
			const char *text;
			int length;
		};
		struct {
			w_alloc alloc;
			void *user_data;
		};
	};
	bool SetText(const char *text, int length) {
		return _WReturnBool(_set_text(text, length, W_ENCODING_UTF8));
	}
	bool SetText(const char *text) {
		return SetText(text, -1);
	}
	bool SetTextV(const char *format, va_list args) {
		return _WReturnBool(_printf(W_ENCODING_UTF8, format, args));
	}
	bool SetTextV(const char *format, ...) {
		va_list args;
		va_start(args, format);
		bool ret = SetTextV(format, args);
		va_end(args);
		return ret;
	}
public:
	WResult _set_text(const char *text, int length, int enc) {
		return w_alloc_set_text(alloc, user_data, this->enc, text, length, enc);
	}
	WResult _printf(int enc, const char *format, va_list args) {
		return w_alloc_printf(alloc, user_data, this->enc, enc, format, args);
	}
};
typedef w_scrollbar_value WScrollBarValue;
class WBasicThemeData {
public:
	wuchar clazz;
	unsigned state :14;
	unsigned textflags :10;
	wuint style;
	WGraphics *gc;
	WRect *bounds;
	WRect *clientArea;
	union {
		WImageList *imagelist;
		WImage *image;
	};
	WItemAttr attr;
	union {
		WScrollBarValue *range;
	};
protected:
	void Init(WGraphics *gc, WRect *bounds) {
		w_themedata_init((w_themedata*) this, (w_graphics*) gc,
				(w_rect*) bounds);
	}
public:
	WBasicThemeData() {
		Init(0, 0);
	}
	WBasicThemeData(WGraphics *gc, WRect *bounds) {
		Init(gc, bounds);
	}
	void ComputeTrim(WTheme *theme, int mask, WRect &result) {
		w_theme_compute_trim((w_theme*) theme, mask, (w_themedata*) this,
				(w_rect*) &result);
	}
	void Draw(WTheme *theme, int mask) {
		w_theme_draw((w_theme*) theme, mask, (w_themedata*) this);
	}
	void Measure(WTheme *theme, int mask, WSize &result) {
		w_theme_measure((w_theme*) theme, mask, (w_themedata*) this,
				(w_size*) &result);
	}
	void GetBounds(WTheme *theme, int mask, int part, w_rect *result) {
		w_theme_get_bounds((w_theme*) theme, mask, part, (w_themedata*) this,
				(w_rect*) &result);
	}
	int Hit(WTheme *theme, int mask, w_themedata *data, WPoint &position) {
		return w_theme_hit((w_theme*) theme, mask, data, (w_point*) &position);
	}
	void DrawAll(WTheme *theme) {
		Draw(theme, W_THEME_MASK_ALL);
	}
	void DrawBackground(WTheme *theme) {
		Draw(theme, W_THEME_MASK_BACKGROUND);
	}
	void DrawFocus(WTheme *theme) {
		Draw(theme, W_THEME_MASK_FOCUS);
	}
	void DrawImage(WTheme *theme) {
		Draw(theme, W_THEME_MASK_IMAGE);
	}
	void DrawText(WTheme *theme) {
		Draw(theme, W_THEME_MASK_TEXT);
	}
	void SetText(const char *text, int length, int enc) {
		this->attr.text = text;
		this->attr.length = length;
		this->attr.enc = enc;
	}
	void SetText(const char *text, int length) {
		SetText(text, length, W_ENCODING_UTF8);
	}
	void SetText(const char *text) {
		SetText(text, -1);
	}
	void SetDrawLeft() {
		this->textflags |= W_THEME_DRAW_LEFT;
	}
	void SetDrawRight() {
		this->textflags = W_THEME_DRAW_RIGHT;
	}
	void SetDrawCenter() {
		this->textflags = W_THEME_DRAW_CENTER;
	}
public:
	void SetStateNull() {
		this->state = 0;
	}
	void SetStateSelected() {
		this->state |= W_SELECTED;
	}
	void SetStateFocused() {
		this->state |= W_FOCUSED;
	}
	void SetStateHot() {
		this->state |= W_HOT;
	}
	void SetStatePressed() {
		this->state |= W_PRESSED;
	}
	void SetStateActive() {
		this->state |= W_ACTIVE;
	}
	void SetStateDisabled() {
		this->state |= W_DISABLED;
	}
	void SetStateDefaulted() {
		this->state |= W_DEFAULTED;
	}
	void SetStateGrayed() {
		this->state |= W_GRAYED;
	}
};
class WThemeData: public WBasicThemeData {
public:
	union {
		struct {
			int selection;
			int minimum;
			int maximum;
			int increment;
			int pageIncrement;
			int thumb;
		} range;
		struct {
			WRect *tabsArea;
			int tabsWidth;
			int tabsHeight;
			int selectedX;
			int selectedWidth;
			int spacing;
		} tab;
		struct {
			int position;
		} tabitem;
		struct {
			int headerWidth;
			int headerHeight;
			WRect *headerArea;
		} group;
	};
public:
	WThemeData() {
	}
};

class WButtonThemeData: public WBasicThemeData {
public:
	void SetStylePush() {
		this->style = W_PUSH;
	}
	WButtonThemeData() {
		memset(this, 0, sizeof(WButtonThemeData));
		this->clazz = W_THEME_CLASS_BUTTON;
	}
};
class WRangeThemeData: public WBasicThemeData {
public:
	int selection;
	int minimum;
	int maximum;
	int increment;
	int pageIncrement;
	int thumb;
public:
	WRangeThemeData() {
		memset(this, 0, sizeof(WRangeThemeData));
	}
};
class WProgressBarThemeData: public WRangeThemeData {
public:
	WProgressBarThemeData() {
		this->clazz = W_THEME_CLASS_PROGRESSBAR;
	}
};
class SWTP_PUBLIC WTheme: public WResource {
protected:
	WTheme() {
	}
public:
	void Dispose() {
		w_theme_dispose((w_theme*) this);
	}
	const char* GetName() {
		return w_theme_get_name((w_theme*) this);
	}
	w_color GetColor(wuint id) {
		w_color color;
		w_theme_get_color((w_theme*) this, id, &color);
		return color;
	}
	WFont* GetFont() {
		WFont *font;
		w_theme_get_font((w_theme*) this, (w_font**) &font);
		return font;
	}
	WCursor* GetCursor(wuint id) {
		WCursor *cursor;
		w_theme_get_cursor((w_theme*) this, id, (w_cursor**) &cursor);
		return cursor;
	}
	WImage* GetImage(wuint id) {
		WImage *image;
		w_theme_get_image((w_theme*) this, id, (w_image**) &image);
		return image;
	}
};

#endif /* SWTP_WIDGETS_THEME_H_ */
