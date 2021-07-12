/*
 * metal.h
 *
 *  Created on: 30 juin 2021
 *      Author: Azeddine
 */

#ifndef CUSTOM_THEMES_METAL_H_
#define CUSTOM_THEMES_METAL_H_
#include <swtc.h>
typedef struct w_theme_metal {
    void *v_table; //not used in c
	struct _w_theme_class *clazz;
	w_font* font;
	float ratio1;
	float ratio2;
	w_color primary1;
	w_color primary2;
	w_color primary3;
	w_color secondary1;
	w_color secondary2;
	w_color secondary3;
	w_color gradiant;
	w_color colors[W_COLOR_TRANSPARENT];
}w_theme_metal;

w_theme* w_theme_metal_create();
void w_theme_metal_draw_flush_3d_border(w_theme *theme, w_graphics *gc, w_rect *rect);
void w_theme_metal_draw_gradient(w_theme *theme, w_graphics *gc, w_rect *rect,int vertical );

void w_theme_metal_dispose(w_theme *theme);
wresult w_theme_metal_open_data(w_theme *theme, int clazz_id,
		w_themedata *data);
#endif /* CUSTOM_THEMES_METAL_H_ */
