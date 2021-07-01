/*
 * theme.c
 *
 *  Created on: 7 nov. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
void w_themedata_init(w_themedata *data, w_graphics *gc, w_rect *bounds) {
	memset(data, 0, sizeof(w_themedata));
	data->gc = gc;
	data->bounds = bounds;
	data->attr.image = -1;
	data->attr.enc = W_ENCODING_UTF8;
	data->attr.length = -1;
}
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
void w_theme_compute_trim(w_theme *theme, int mask, w_themedata *data,
		w_rect *result) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->themedata.compute_trim(theme, mask, data, result);
	}
}
void w_theme_draw(w_theme *theme, int mask, w_themedata *data) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->themedata.draw(theme, mask, data);
	}
}
void w_theme_measure(w_theme *theme, int mask, w_themedata *data,
		w_size *result) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->themedata.measure(theme, mask, data, result);
	}
}
void w_theme_get_bounds(w_theme *theme, int mask, int part, w_themedata *data,
		w_rect *result) {
	if (theme != 0 && theme->clazz != 0) {
		theme->clazz->themedata.get_bounds(theme, mask, part, data, result);
	}
}
int w_theme_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
	if (theme != 0 && theme->clazz != 0) {
		return theme->clazz->themedata.hit(theme, mask, data, position);
	} else
		return -1;
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
