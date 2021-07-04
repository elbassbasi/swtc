/*
 * Name:        theme.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_THEME_H_
#define SWT_WIDGETS_THEME_H_
#include "event.h"
#ifdef __cplusplus
extern "C" {
#endif
/** classes  */
enum {
	W_THEME_CLASS_UNKNOWN = 0,
	W_THEME_CLASS_MENU,
	W_THEME_CLASS_MENUITEM,
	W_THEME_CLASS_BUTTON,
	W_THEME_CLASS_LABEL,
	W_THEME_CLASS_PROGRESSBAR,
	W_THEME_CLASS_SASH,
	W_THEME_CLASS_SLIDER,
	W_THEME_CLASS_SCROLLBAR,
	W_THEME_CLASS_TEXTEDIT,
	W_THEME_CLASS_COMPOSITE,
	W_THEME_CLASS_WEBVIEW,
	W_THEME_CLASS_TREEVIEW,
	W_THEME_CLASS_TREEITEM,
	W_THEME_CLASS_COLUMNITEM,
	W_THEME_CLASS_TABVIEW,
	W_THEME_CLASS_TABITEM,
	W_THEME_CLASS_COMBOBOX,
	W_THEME_CLASS_COOLBAR,
	W_THEME_CLASS_COOLITEM,
	W_THEME_CLASS_DATETIME,
	W_THEME_CLASS_EXPANDBAR,
	W_THEME_CLASS_EXPANDITEM,
	W_THEME_CLASS_GROUPBOX,
	W_THEME_CLASS_SPINNER,
	W_THEME_CLASS_LISTVIEW,
	W_THEME_CLASS_LISTITEM,
	W_THEME_CLASS_TOOLBAR,
	W_THEME_CLASS_TOOLITEM,
	W_THEME_CLASS_LAST,
};

enum {

	/** Part states */
	W_THEME_STATE_SELECTED = W_SELECTED,
	W_THEME_STATE_FOCUSED = W_FOCUSED,
	W_THEME_STATE_HOT = W_HOT,
	W_THEME_STATE_PRESSED = W_PRESSED,
	W_THEME_STATE_ACTIVE = W_ACTIVE,
	W_THEME_STATE_DISABLED = W_DISABLED,
	W_THEME_STATE_DEFAULTED = W_DEFAULTED,
	W_THEME_STATE_GRAYED = W_GRAYED,
	/* */
	W_THEME_STATE_PART_HOT = 1 << 11,
	W_THEME_STATE_PART_EXPANDED = 1 << 12,

	/** Text and Image drawing flags */
	W_THEME_DRAW_LEFT = W_DRAW_LEFT,
	W_THEME_DRAW_TOP = W_DRAW_TOP,
	W_THEME_DRAW_RIGHT = W_DRAW_RIGHT,
	W_THEME_DRAW_BOTTOM = W_DRAW_BOTTOM,
	W_THEME_DRAW_HCENTER = W_DRAW_HCENTER,
	W_THEME_DRAW_VCENTER = W_DRAW_VCENTER,
	W_THEME_DRAW_CENTER = W_DRAW_CENTRE,
	/*  Mask */
	W_THEME_MASK_BACKGROUND = 1 << 8,
	W_THEME_MASK_TEXT = 1 << 9,
	W_THEME_MASK_FOCUS = 1 << 10,
	W_THEME_MASK_IMAGE = 1 << 11,
	W_THEME_MASK_IMAGE_INDEX = 1 << 12,
	W_THEME_MASK_WIDTH = 1 << 13,
	W_THEME_MASK_HEIGTH = 1 << 14,
	W_THEME_MASK_ALL = W_THEME_MASK_BACKGROUND | W_THEME_MASK_TEXT
			| W_THEME_MASK_FOCUS | W_THEME_MASK_IMAGE | W_THEME_MASK_IMAGE_INDEX
			| W_THEME_MASK_WIDTH | W_THEME_MASK_HEIGTH,
	W_THEME_MASK_PART = 0xFF,

	/** Widget parts */
	W_THEME_PART_NOWHERE = -1,
	W_THEME_PART_WHOLE = 0,
	W_THEME_PART_TEXT = 1,
	W_THEME_PART_IMAGE = 2,
	/** ScrollBar ComboBox parts*/
	W_THEME_PART_ARROW = 3,
	W_THEME_PART_ARROW_UP = W_THEME_PART_ARROW,
	W_THEME_PART_ARROW_LEFT = W_THEME_PART_ARROW,
	W_THEME_PART_ARROW_DOWN = 4,
	W_THEME_PART_ARROW_RIGHT = W_THEME_PART_ARROW_DOWN,
	W_THEME_PART_TRACK_UP = 5,
	W_THEME_PART_TRACK_DOWN = 6,
	W_THEME_PART_TRACK_LEFT = W_THEME_PART_TRACK_UP,
	W_THEME_PART_TRACK_RIGHT = W_THEME_PART_TRACK_DOWN,
	W_THEME_PART_THUMB = 7,
	/* */
	W_THEME_PART_CLOSE = 3,
	W_THEME_PART_CHEVRON = 3,
};
struct w_theme {
	void *v_table; //not used in c
	struct _w_theme_class *clazz;
};
enum {
	W_ITEM_ATTR_MASK_TEXT = (1 << 16),
	W_ITEM_ATTR_MASK_FONT = (1 << 17),
	W_ITEM_ATTR_MASK_BACKGROUND = (1 << 18),
	W_ITEM_ATTR_MASK_FORGROUND = (1 << 19),
	W_ITEM_ATTR_MASK_ALL_NO_TEXT = (W_ITEM_ATTR_MASK_FONT
			| W_ITEM_ATTR_MASK_BACKGROUND | W_ITEM_ATTR_MASK_FORGROUND),
	W_ITEM_ATTR_MASK_ALL = (W_ITEM_ATTR_MASK_TEXT | W_ITEM_ATTR_MASK_ALL_NO_TEXT),
};
typedef struct w_item_attr {
	w_font *font;
	w_color background;
	w_color foreground;
	int image;
	int enc;
	union {
		struct {
			char *text;
			int length;
		};
		struct {
			w_alloc alloc;
			void *user_data;
		};
	};
} w_item_attr;
typedef struct w_scrollbar_value w_scrollbar_value;
typedef struct w_themedata {
	wuchar clazz;
	unsigned state :14;
	unsigned textflags :10;
	wuint style;
	w_graphics *gc;
	w_rect *bounds;
	w_rect *clientArea;
	union {
		w_imagelist *imagelist;
		w_image *image;
	};
	w_item_attr attr;
	union {
		w_scrollbar_value *range;
	};
} w_themedata;
SWT_PUBLIC void w_themedata_init(w_themedata *data, w_graphics *gc,
		w_rect *bounds);
typedef struct _w_themedata_class {
	void (*compute_trim)(w_theme *theme, int mask, w_themedata *data,
			w_rect *result);
	void (*draw)(w_theme *theme, int mask, w_themedata *data);
	void (*measure)(w_theme *theme, int mask, w_themedata *data,
			w_size *result);
	void (*get_bounds)(w_theme *theme, int mask, int part, w_themedata *data,
			w_rect *result);
	int (*hit)(w_theme *theme, int mask, w_themedata *data, w_point *position);
} _w_themedata_class;
typedef struct _w_theme_class {
	void (*dispose)(w_theme *theme);
	const char* (*get_name)(w_theme *theme);
	_w_themedata_class themedata;
	wresult (*get_color)(w_theme *theme, wuint colorid, w_color *color);
	wresult (*get_font)(w_theme *theme, w_font **font);
	wresult (*get_cursor)(w_theme *theme, wuint id, w_cursor **cursor);
	wresult (*get_image)(w_theme *theme, wuint id, w_image **image);
} _w_theme_class;
SWT_PUBLIC void w_theme_dispose(w_theme *theme);
SWT_PUBLIC const char* w_theme_get_name(w_theme *theme);
SWT_PUBLIC void w_theme_compute_trim(w_theme *theme, int mask,
		w_themedata *data, w_rect *result);
SWT_PUBLIC void w_theme_draw(w_theme *theme, int mask, w_themedata *data);
SWT_PUBLIC void w_theme_measure(w_theme *theme, int mask, w_themedata *data,
		w_size *result);
SWT_PUBLIC void w_theme_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result);
SWT_PUBLIC int w_theme_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position);
SWT_PUBLIC wresult w_theme_get_color(w_theme *theme, wuint colorid,
		w_color *color);
SWT_PUBLIC wresult w_theme_get_font(w_theme *theme, w_font **font);
SWT_PUBLIC wresult w_theme_get_cursor(w_theme *theme, wuint id,
		w_cursor **cursor);
SWT_PUBLIC wresult w_theme_get_image(w_theme *theme, wuint id, w_image **image);

#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_THEME_H_ */
