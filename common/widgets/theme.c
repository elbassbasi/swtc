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
		return theme->clazz->get_name(theme);
	} else
		return 0;
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
void w_theme_draw_image_index(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds, w_imagelist *imagelist, int index, int flags) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->draw_image_index(theme, data, gc, bounds, imagelist,
				index, flags);
	}
}
void w_theme_draw_text(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds, const char *text, int length, int enc, int flags) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->draw_text(theme, data, gc, bounds, text, length, enc,
				flags);
	}
}
void w_theme_get_bounds(w_theme *theme, w_themedata *data, w_rect *bounds,
		w_rect *result) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->get_bounds(theme, data, bounds, result);
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
wuchar themeclasses0[_W_THEME_CLASS_INTERNAL_LAST] = {
		[_W_THEME_CLASS_INTERNAL_UNKNOWN] = W_THEME_CLASS_UNKNOWN, //
		[_W_THEME_CLASS_INTERNAL_MENU] =W_THEME_CLASS_MENU, //
		[_W_THEME_CLASS_INTERNAL_BUTTON] =W_THEME_CLASS_BUTTON, //
		[_W_THEME_CLASS_INTERNAL_LABEL] =W_THEME_CLASS_LABEL, //
		[_W_THEME_CLASS_INTERNAL_PROGRESSBAR] =W_THEME_CLASS_PROGRESSBAR, //
		[_W_THEME_CLASS_INTERNAL_SASH] =W_THEME_CLASS_SASH, //
		[_W_THEME_CLASS_INTERNAL_SLIDER] =W_THEME_CLASS_SLIDER, //
		[_W_THEME_CLASS_INTERNAL_SCROLLBAR] =W_THEME_CLASS_SCROLLBAR, //
		[_W_THEME_CLASS_INTERNAL_TEXTEDIT] =W_THEME_CLASS_TEXTEDIT, //
		[_W_THEME_CLASS_INTERNAL_COMPOSITE] =W_THEME_CLASS_COMPOSITE, //
		[_W_THEME_CLASS_INTERNAL_WEBVIEW] =W_THEME_CLASS_WEBVIEW, //
		[_W_THEME_CLASS_INTERNAL_TREEVIEW] =W_THEME_CLASS_TREEVIEW, //
		[_W_THEME_CLASS_INTERNAL_TABVIEW] =W_THEME_CLASS_TABVIEW, //
		[_W_THEME_CLASS_INTERNAL_COMBOBOX] =W_THEME_CLASS_COMBOBOX, //
		[_W_THEME_CLASS_INTERNAL_COOLBAR] =W_THEME_CLASS_COOLBAR, //
		[_W_THEME_CLASS_INTERNAL_DATETIME] =W_THEME_CLASS_DATETIME, //
		[_W_THEME_CLASS_INTERNAL_EXPANDBAR] =W_THEME_CLASS_EXPANDBAR, //
		[_W_THEME_CLASS_INTERNAL_GROUPBOX] =W_THEME_CLASS_GROUPBOX, //
		[_W_THEME_CLASS_INTERNAL_SPINNER] =W_THEME_CLASS_SPINNER, //
		[_W_THEME_CLASS_INTERNAL_LISTVIEW] =W_THEME_CLASS_LISTVIEW, //
		[_W_THEME_CLASS_INTERNAL_TOOLBAR] =W_THEME_CLASS_TOOLBAR, //
		};
wuchar themeclasses[W_THEME_CLASS_LAST];
wuint w_theme_internal_class_id(wuint clazz) {
	if (themeclasses[W_THEME_CLASS_BUTTON] == 0) {
		for (wuint i = 0; i < _W_THEME_CLASS_INTERNAL_LAST; i++) {
			themeclasses[themeclasses0[i]] = i;
		}
	}
	if (clazz >= W_THEME_CLASS_LAST)
		return 0;
	else
		return themeclasses[clazz];
}
