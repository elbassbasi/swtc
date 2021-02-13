/*
 * theme.c
 *
 *  Created on: 7 nov. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
void w_theme_dispose(w_theme *theme) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->dispose(theme);
	}
}
const char* w_theme_get_name(w_theme *theme) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->get_name(theme);
	}else return 0;
}
void w_theme_compute_trim(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *result) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->compute_trim(theme, data, gc, result);
	}
}
void w_theme_draw_background(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->draw_background(theme, data, gc, bounds);
	}
}
void w_theme_draw_focus(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->draw_focus(theme, data, gc, bounds);
	}
}
void w_theme_draw_image(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds, w_image *image, int flags) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->draw_image(theme, data, gc, bounds, image, flags);
	}
}
void w_theme_draw_text(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds, const char *text, int length, int flags) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->draw_text(theme, data, gc, bounds, text, length, flags);
	}
}
void w_theme_get_bounds(w_theme *theme, w_themedata *data, int part,
		w_rect *bounds, w_rect *result) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->get_bounds(theme, data, part, bounds, result);
	}
}
int w_theme_get_selection(w_theme *theme, w_themedata *data, w_point *offset,
		w_rect *bounds) {
	if (theme != 0 && theme->clazz != 0) {
		return theme->clazz->get_selection(theme, data, offset, bounds);
	} else
		return 0;
}
int w_theme_hit_background(w_theme *theme, w_themedata *data, w_point *position,
		w_rect *bounds) {
	if (theme != 0 && theme->clazz != 0) {
		return theme->clazz->hit_background(theme, data, position, bounds);
	} else
		return 0;
}
void w_theme_measure_text(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds, w_rect *result, const char *text, size_t length,
		int flags) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->measure_text(theme, data, gc, bounds, result, text,
				length, flags);
	}
}
w_color w_theme_get_color(w_theme *theme, wuint id) {
	if (theme != 0 && theme->clazz != 0) {
		return theme->clazz->get_color(theme, id);
	} else
		return 0;
}
w_font* w_theme_get_font(w_theme *theme) {
	if (theme != 0 && theme->clazz != 0) {
		return theme->clazz->get_font(theme);
	} else {
		return 0;
	}
}
w_cursor* w_theme_get_cursor(w_theme *theme, wuint id) {
	if (theme != 0 && theme->clazz != 0) {
		return theme->clazz->get_cursor(theme, id);
	} else
		return 0;
}
w_image* w_theme_get_image(w_theme *theme, wuint id) {
	if (theme != 0 && theme->clazz != 0) {
		return theme->clazz->get_image(theme, id);
	} else
		return 0;
}
wuchar themeclasses0[_W_THEME_CLASS_LAST] = { W_THEME_CLASS_UNKNOWN,
		W_THEME_CLASS_MENU, W_THEME_CLASS_BUTTON, W_THEME_CLASS_LABEL,
		W_THEME_CLASS_PROGRESSBAR, W_THEME_CLASS_SASH,
		W_THEME_CLASS_SLIDER, W_THEME_CLASS_SCROLLBAR,
		W_THEME_CLASS_TEXTEDIT, W_THEME_CLASS_COMPOSITE,
		W_THEME_CLASS_WEBVIEW, W_THEME_CLASS_TREEVIEW, W_THEME_CLASS_TABVIEW,
		W_THEME_CLASS_TABITEM, W_THEME_CLASS_COMBOBOX, W_THEME_CLASS_COOLBAR,
		W_THEME_CLASS_DATETIME, W_THEME_CLASS_EXPANDBAR, W_THEME_CLASS_GROUPBOX,
		W_THEME_CLASS_SPINNER, W_THEME_CLASS_LISTVIEW, W_THEME_CLASS_TOOLBAR,
		W_THEME_CLASS_TOOLITEM, };
wuchar themeclasses[W_THEME_CLASS_LAST];
wuint w_theme_internal_class_id(wuint clazz) {
	if (themeclasses[W_THEME_CLASS_BUTTON] == 0) {
		for (wuint i = 0; i < _W_THEME_CLASS_LAST; i++) {
			themeclasses[themeclasses0[i]] = i;
		}
	}
	if (clazz >= W_THEME_CLASS_LAST)
		return 0;
	else
		return themeclasses[clazz];
}
