/*
 * Theme.h
 *
 *  Created on: 8 nov. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_THEME_H_
#define SWTP_WIDGETS_THEME_H_
#include "Widget.h"
class WTheme;
class WBasicThemeData {
public:
	wuint clazz;
	wuint state;
	wuint64 style;
	w_rect *clientArea;
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
			w_rect *tabsArea;
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
			w_rect *headerArea;
		} group;
	};
public:
	void Init() {
		memset(this, 0, sizeof(WThemeData));
	}
	WThemeData() {
		Init();
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
	void ComputeTrim(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			WRect &result) {
		w_theme_compute_trim((w_theme*) this, (w_themedata*) &data,
				W_GRAPHICS(&gc), (w_rect*) &result);
	}
	void DrawBackground(WBasicThemeData &data, WGraphics &gc, WRect &bounds) {
		w_theme_draw_background((w_theme*) this, (w_themedata*) &data,
				W_GRAPHICS(&gc), (w_rect*) &bounds);
	}
	void DrawFocus(WBasicThemeData &data, WGraphics &gc, WRect &bounds) {
		w_theme_draw_focus((w_theme*) this, (w_themedata*) &data,
				W_GRAPHICS(&gc), (w_rect*) &bounds);
	}
	void DrawImage(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			WImage &image, int flags) {
		w_theme_draw_image((w_theme*) this, (w_themedata*) &data,
				W_GRAPHICS(&gc), (w_rect*) &bounds, (w_image*) &image, flags);
	}
	void DrawText(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text, int length, int flags) {
		w_theme_draw_text((w_theme*) this, (w_themedata*) &data,
				W_GRAPHICS(&gc), (w_rect*) &bounds, text, length, flags);
	}
	void DrawText(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text, int flags) {
		DrawText(data, gc, bounds, text, -1, flags);
	}
	void DrawTextLeft(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text, int length) {
		return DrawText(data, gc, bounds, text, length, W_THEME_DRAW_LEFT);
	}
	void DrawTextLeft(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text) {
		return DrawTextLeft(data, gc, bounds, text,-1);
	}
	void DrawTextTop(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text, int length) {
		return DrawText(data, gc, bounds, text, length, W_THEME_DRAW_TOP);
	}
	void DrawTextTop(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text) {
		return DrawTextTop(data, gc, bounds, text, -1);
	}
	void DrawTextRight(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text, int length) {
		return DrawText(data, gc, bounds, text, length, W_THEME_DRAW_RIGHT);
	}
	void DrawTextRight(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text) {
		return DrawTextRight(data, gc, bounds, text, -1);
	}
	void DrawTextBottom(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text, int length) {
		return DrawText(data, gc, bounds, text, length, W_THEME_DRAW_BOTTOM);
	}
	void DrawTextBottom(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text) {
		return DrawTextBottom(data, gc, bounds, text, -1);
	}
	void DrawTextHCenter(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text, int length) {
		return DrawText(data, gc, bounds, text, length, W_THEME_DRAW_HCENTER);
	}
	void DrawTextHCenter(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text) {
		return DrawTextHCenter(data, gc, bounds, text, -1);
	}
	void DrawTextVCenter(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text, int length) {
		return DrawText(data, gc, bounds, text, length, W_THEME_DRAW_VCENTER);
	}
	void DrawTextVCenter(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text) {
		return DrawTextVCenter(data, gc, bounds, text, -1);
	}
	void DrawTextCenter(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text, int length) {
		return DrawText(data, gc, bounds, text, length, W_THEME_DRAW_CENTER);
	}
	void DrawTextCenter(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			const char *text) {
		return DrawTextCenter(data, gc, bounds, text, -1);
	}
	void GetBounds(WBasicThemeData &data, int part, WRect &bounds,
			WRect &result) {
		w_theme_get_bounds((w_theme*) this, (w_themedata*) &data, part,
				(w_rect*) &bounds, (w_rect*) &result);
	}
	int GetSelection(WBasicThemeData &data, WPoint &offset, WRect &bounds) {
		return w_theme_get_selection((w_theme*) this, (w_themedata*) &data,
				(w_point*) &offset, (w_rect*) &bounds);
	}
	int HitBackground(WBasicThemeData &data, WPoint &position, WRect &bounds) {
		return w_theme_hit_background((w_theme*) this, (w_themedata*) &data,
				(w_point*) &position, (w_rect*) &bounds);
	}
	void MeasureText(WBasicThemeData &data, WGraphics &gc, WRect &bounds,
			WRect &result, const char *text, size_t length, int flags) {
		w_theme_measure_text((w_theme*) this, (w_themedata*) &data,
				W_GRAPHICS(&gc), (w_rect*) &bounds, (w_rect*) &result, text,
				length, flags);
	}
	w_color GetColor(wuint id) {
		return w_theme_get_color((w_theme*) this, id);
	}
	WFont* GetFont() {
		return (WFont*) w_theme_get_font((w_theme*) this);
	}
	WCursor* GetCursor(wuint id) {
		return (WCursor*) w_theme_get_cursor((w_theme*) this, id);
	}
	WImage* GetImage(wuint id) {
		return (WImage*) w_theme_get_image((w_theme*) this, id);
	}
};

#endif /* SWTP_WIDGETS_THEME_H_ */
