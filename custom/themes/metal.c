/*
 * metal.c
 *
 *  Created on: 30 juin 2021
 *      Author: Azeddine
 */
#include "metal.h"
void w_theme_metal_draw_flush_3d_border(w_theme *theme, w_graphics *gc,
		w_rect *rect) {
	w_theme_metal *metal = (w_theme_metal*) theme;
	w_rect r;
	r.x = rect->x;
	r.y = rect->y;
	w_graphics_set_foreground(gc, metal->secondary1);
	r.width = rect->width - 2;
	r.height = rect->height - 2;
	w_graphics_draw_rectangle(gc, &r);
	w_graphics_set_foreground(gc, 0xFFFFFFFF);
	r.x = rect->x + 1;
	r.y = rect->y + 1;
	w_graphics_draw_rectangle(gc, &r);
	w_graphics_set_foreground(gc, metal->secondary3);
	r.x = rect->x + 0;
	r.y = rect->y + rect->height - 1;
	r.width = rect->x + 1;
	r.height = rect->y + rect->height - 1;
	w_graphics_draw_line(gc, &r.pt, (w_point*) &r.sz);
	r.x = rect->x + rect->width - 1;
	r.y = rect->y;
	r.width = rect->x + rect->width - 2;
	r.height = rect->y + 1;
	w_graphics_draw_line(gc, &r.pt, (w_point*) &r.sz);
}
void w_theme_metal_draw_gradient(w_theme *theme, w_graphics *gc, w_rect *rect,
		int vertical) {
	w_theme_metal *metal = (w_theme_metal*) theme;
	w_rect r;
	int mid;
	int mid2;
	int wh;
	if (vertical) {
		mid = (int) (metal->ratio1 * rect->height);
		mid2 = (int) (metal->ratio2 * rect->height);
		wh = rect->height;
	} else {
		mid = (int) (metal->ratio1 * rect->width);
		mid2 = (int) (metal->ratio2 * rect->width);
		wh = rect->width;
	}
	w_color c1 = metal->gradiant, c2 = W_COLOR_WHITE, c3 = metal->primary3;
	r.x = rect->x;
	r.y = rect->y;
	if (mid > 0) {
		w_graphics_set_background(gc, c2);
		w_graphics_set_foreground(gc, c1);
		if (vertical) {
			r.width = rect->width;
			r.height = mid;
		} else {
			r.width = mid;
			r.height = rect->height;
		}
		w_graphics_fill_gradientrectangle(gc, &r, vertical);
	}
	if (mid2 > 0) {
		w_graphics_set_foreground(gc, c2);
		if (vertical) {
			r.x = rect->x;
			r.y = rect->y + mid;
			r.width = rect->width;
			r.height = mid2;
		} else {
			r.x = rect->x + mid;
			r.y = rect->y;
			r.width = mid2;
			r.height = rect->height;
		}
		w_graphics_fill_rectangle(gc, &r);
	}
	if (mid > 0) {
		w_graphics_set_background(gc, c1);
		w_graphics_set_foreground(gc, c2);
		if (vertical) {
			r.x = rect->x;
			r.y = rect->y + mid + mid2;
			r.width = rect->width;
			r.height = mid;
		} else {
			r.x = rect->x + mid + mid2;
			r.y = rect->y;
			r.width = mid;
			r.height = rect->height;
		}
		w_graphics_fill_gradientrectangle(gc, &r, vertical);
	}
	if (wh - mid * 2 - mid2 > 0) {
		w_graphics_set_background(gc, c3);
		w_graphics_set_foreground(gc, c1);
		if (vertical) {
			r.x = rect->x;
			r.y = rect->y + mid * 2 + mid2;
			r.width = rect->width;
			r.height = rect->height - mid * 2 - mid2;
		} else {
			r.x = rect->x + mid * 2 + mid2;
			r.y = rect->y;
			r.width = rect->width - mid * 2 - mid2;
			r.height = rect->height;
		}
		w_graphics_fill_gradientrectangle(gc, &r, vertical);
	}
}
void metal_button_draw_background_push(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds) {
	if (data->state & W_PRESSED) {
		w_graphics_set_background(gc, ((w_theme_metal*) theme)->primary2);
		w_graphics_fill_rectangle(gc, bounds);
		return;
	}
	w_theme_metal_draw_gradient(theme, gc, bounds, W_TRUE);
	w_theme_metal_draw_flush_3d_border(theme, gc, bounds);
	if (data->attr.text != 0) {
		w_graphics_set_foreground(gc, W_COLOR_BLACK);
		w_graphics_draw_text(gc, data->attr.text, data->attr.length, bounds,
				W_DRAW_TRANSPARENT, data->attr.enc);
	}
}
void metal_button_draw_background_radio(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds) {
	w_rect r;
	w_point p1, p2;
	if (data->style & W_CHECK) {
		r.x = bounds->x + 3;
		r.y = bounds->y + 5;
		r.width = 2;
		r.height = 13 - 8;
		w_graphics_fill_rectangle(gc, &r);
		p1.x = bounds->x + (13 - 4);
		p1.y = bounds->y + 3;
		p2.x = bounds->x + 5;
		p2.y = bounds->y + (13 - 6);
		w_graphics_draw_line(gc, &p1, &p2);
		p1.x = bounds->x + (13 - 4);
		p1.y = bounds->y + 4;
		p2.x = bounds->x + 5;
		p2.y = bounds->y + (13 - 5);
		w_graphics_draw_line(gc, &p1, &p2);
	} else {
		r.x = bounds->x + 1;
		r.y = bounds->y + 1;
		r.width = 6;
		r.height = 6;
		w_graphics_fill_oval(gc, &r);
	}
}
void metal_button_draw_background_arrow(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds) {
	int width = 4;
	int height = 8;
	w_point pt[3];
	pt[0].x = bounds->x;
	pt[0].y = bounds->y;
	pt[1].x = bounds->x + width;
	pt[1].y = bounds->y + (height / 2);
	pt[2].x = bounds->x;
	pt[2].y = bounds->y + height;
	w_graphics_fill_polygon(gc, pt, 3);
}
/* button */
void w_theme_metal_button_compute_trim(w_theme *theme, int mask,
		w_themedata *data, w_rect *result) {
}
void w_theme_metal_button_draw(w_theme *theme, int mask, w_themedata *data) {
	w_rect tmp, *last;
	last = data->clientArea;
	data->clientArea = &tmp;
	if ((data->style & W_CHECK) != 0 || (data->style & W_RADIO) != 0) {
		metal_button_draw_background_radio(theme, data, data->gc, data->bounds);
	} else if (data->style & W_ARROW) {
		metal_button_draw_background_arrow(theme, data, data->gc, data->bounds);
	} else {
		metal_button_draw_background_push(theme, data, data->gc, data->bounds);
	}
	data->clientArea = last;
	if (last != 0) {
		memcpy(last, &tmp, sizeof(tmp));
	}
}
void w_theme_metal_button_measure(w_theme *theme, int mask, w_themedata *data,
		w_size *result) {
	w_font *font = data->attr.font;
	if (font == 0) {
		w_theme_get_font(theme, &font);
	}
	w_graphics_set_font(data->gc, font);
	w_graphics_text_extent(data->gc, data->attr.text, data->attr.length, result,
			data->textflags, data->attr.enc);
}
void w_theme_metal_button_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
}
int w_theme_metal_button_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
}
/* expandbar */
#define EXPANDBAR_TEXT_INSET 6
#define EXPANDBAR_BORDER 1
#define EXPANDBAR_CHEVRON_SIZE 24
void w_theme_metal_expandbar_compute_trim(w_theme *theme, int mask,
		w_themedata *data, w_rect *result) {
}
void w_theme_metal_expandbar_draw(w_theme *theme, int mask, w_themedata *data) {
}
void w_theme_metal_expandbar_measure(w_theme *theme, int mask,
		w_themedata *data, w_size *result) {
}
void w_theme_metal_expandbar_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
}
int w_theme_metal_expandbar_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
}
/* expanditem */
#define __POINT(x,y) ((x) | ((y) << 4) )
#define __POINT_X(x) (x & 0xF)
#define __POINT_Y(x) ((x >> 4) & 0xF)
wuchar _polyline_0_0[] = { __POINT(0, 0), __POINT(1, 0), __POINT(1, 1), __POINT(
		2, 1), __POINT(2, 2), __POINT(3, 2), __POINT(3, 3), __POINT(3, 2),
		__POINT(4, 2), __POINT(4, 1), __POINT(5, 1), __POINT(5, 0), __POINT(7,
				0) };
void w_theme_metal_expanditem_compute_trim(w_theme *theme, int mask,
		w_themedata *data, w_rect *result) {
}
void w_theme_metal_expanditem_draw_chevron(w_theme *theme, int mask,
		w_themedata *data, w_rect *rect) {
	w_point polyline1[14];
	w_point polyline2[14];
	const int polyline_length = sizeof(_polyline_0_0)
			/ sizeof(_polyline_0_0[0]);
	if ((data->state & W_THEME_STATE_PART_EXPANDED) != 0) {
		int px = rect->x + 5;
		int py = rect->y + 7;
		for (int i = 0; i < polyline_length; i++) {
			polyline1[i].x = px + __POINT_X(_polyline_0_0[i]);
			polyline1[i].y = py - __POINT_Y(_polyline_0_0[i]);
			polyline2[i].x = px + __POINT_X(_polyline_0_0[i]);
			polyline2[i].y = py + 4 - __POINT_Y(_polyline_0_0[i]);
		}
	} else {
		int px = rect->x + 5;
		int py = rect->y + 4;
		for (int i = 0; i < polyline_length; i++) {
			polyline1[i].x = px + __POINT_X(_polyline_0_0[i]);
			polyline1[i].y = py + __POINT_Y(_polyline_0_0[i]);
			polyline2[i].x = px + __POINT_X(_polyline_0_0[i]);
			polyline2[i].y = py + 4 + __POINT_Y(_polyline_0_0[i]);
		}
	}
	w_color color;
	w_theme_get_color(theme, W_COLOR_TITLE_FOREGROUND, &color);
	w_graphics_set_foreground(data->gc, color);
	w_graphics_draw_polyline(data->gc, polyline1, polyline_length);
	w_graphics_draw_polyline(data->gc, polyline2, polyline_length);
}
void w_theme_metal_expanditem_draw(w_theme *theme, int mask,
		w_themedata *data) {
	w_graphics *gc = data->gc;
	w_rect *bounds = data->bounds;
	int headerHeight = data->headerHeight;
	w_rect r;
	memcpy(&r, bounds, sizeof(r));
	w_color title_background;
	w_color title_forground;
	w_color title_background_gradient;
	w_theme_get_color(theme, W_COLOR_TITLE_BACKGROUND, &title_background);
	w_theme_get_color(theme, W_COLOR_TITLE_BACKGROUND_GRADIENT,
			&title_background_gradient);
	w_graphics_set_foreground(gc, title_background);
	w_graphics_set_background(gc, title_background_gradient);
	r.height = headerHeight;
	w_graphics_fill_gradientrectangle(gc, &r, W_TRUE);
	if ((data->state & W_THEME_STATE_PART_EXPANDED) != 0) {
		w_point pt;
		w_graphics_set_foreground(gc, title_background_gradient);
		r.x = bounds->x;
		r.y = bounds->y + headerHeight;
		pt.x = bounds->x;
		pt.y = bounds->y + headerHeight + bounds->height - 1;
		w_graphics_draw_line(gc, &r.pt, &pt);
		//r.x = bounds->x;
		r.y = bounds->y + headerHeight + bounds->height - 1;
		pt.x = bounds->x + bounds->width - 1;
		//pt.y = bounds->y + headerHeight + bounds->height - 1;
		w_graphics_draw_line(gc, &r.pt, &pt);
		r.x = bounds->x + bounds->width - 1;
		//r.y = bounds->y + headerHeight + bounds->height - 1;
		//pt.x = bounds->x + bounds->width - 1;
		pt.y = bounds->y + headerHeight;
		w_graphics_draw_line(gc, &r.pt, &pt);
	}
	int drawX = bounds->x;
	if ((mask & W_THEME_MASK_IMAGE_INDEX) != 0 && data->attr.image >= 0
			&& data->imagelist != 0) {
		drawX += EXPANDBAR_TEXT_INSET;
		w_imagelist_get_size(data->imagelist, &r.sz);
		r.x = drawX;
		if (r.height > headerHeight) {
			r.y = bounds->y + headerHeight - r.height;
		} else {
			r.y = bounds->y + (headerHeight - r.height) / 2;
		}
		w_imagelist_draw(data->imagelist, gc, data->attr.image, &r.pt,
				data->state);
		drawX += r.width;
	}
	if ((mask & W_THEME_MASK_TEXT) != 0 && data->attr.text != 0) {
		r.x = drawX;
		r.y = bounds->y;
		r.width = bounds->width + data->bounds->x - drawX;
		r.height = -1;
		drawX += EXPANDBAR_TEXT_INSET;
		w_color text_color = data->attr.foreground;
		if (text_color == 0) {
			w_theme_get_color(theme, W_COLOR_TITLE_FOREGROUND, &text_color);
		}
		w_graphics_set_foreground(gc, text_color);
		w_graphics_draw_text(gc, data->attr.text, data->attr.length, &r,
				W_DRAW_TRANSPARENT, data->attr.enc);
	}
	int chevronSize = EXPANDBAR_CHEVRON_SIZE;
	r.x = bounds->x + bounds->width - chevronSize;
	r.y = bounds->y + (headerHeight - chevronSize) / 2;
	r.width = bounds->width;
	r.height = headerHeight;
	w_theme_metal_expanditem_draw_chevron(theme, mask, data, &r);
	if ((data->state & W_THEME_STATE_FOCUSED) != 0) {
		r.x = bounds->x + 1;
		r.y = bounds->y + 1;
		r.width = bounds->width - 2;
		r.height = headerHeight - 2;
		//gc.drawFocus(x + 1, y + 1, width - 2, headerHeight - 2);
	}
}
void w_theme_metal_expanditem_measure(w_theme *theme, int mask,
		w_themedata *data, w_size *result) {
	int width = EXPANDBAR_TEXT_INSET * 2 + EXPANDBAR_CHEVRON_SIZE;
	int heigth = EXPANDBAR_CHEVRON_SIZE;
	w_imagelist *imagelist = data->imagelist;
	if (imagelist != 0) {
		w_size sz;
		w_imagelist_get_size(imagelist, &sz);
		if (sz.width != 0) {
			width += EXPANDBAR_TEXT_INSET + sz.width;
		}
	}
	if (data->attr.text != 0) {
		w_graphics *gc = data->gc;
		w_size sz;
		w_graphics_text_extent(gc, data->attr.text, -1, &sz, 0, data->attr.enc);
		width += sz.width;
	}
	result->width = width;
	result->height = heigth;
}
void w_theme_metal_expanditem_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
	memset(result, 0, sizeof(w_rect));
}
int w_theme_metal_expanditem_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
}
/*
 *
 */
_w_themedata_class w_theme_metal_fun[W_THEME_CLASS_LAST] = {
		[W_THEME_CLASS_UNKNOWN] = { }, //
		[W_THEME_CLASS_MENU] = { }, //
		[W_THEME_CLASS_MENUITEM] = { }, //
		[W_THEME_CLASS_BUTTON] = { w_theme_metal_button_compute_trim,
				w_theme_metal_button_draw, w_theme_metal_button_measure,
				w_theme_metal_button_get_bounds, w_theme_metal_button_hit }, //
		[W_THEME_CLASS_LABEL] = { }, //
		[W_THEME_CLASS_PROGRESSBAR] = { }, //
		[W_THEME_CLASS_SASH] = { }, //
		[W_THEME_CLASS_SLIDER] = { }, //
		[W_THEME_CLASS_SCROLLBAR] = { }, //
		[W_THEME_CLASS_TEXTEDIT] = { }, //
		[W_THEME_CLASS_COMPOSITE] = { }, //
		[W_THEME_CLASS_WEBVIEW] = { }, //
		[W_THEME_CLASS_TREEVIEW] = { }, //
		[W_THEME_CLASS_TREEITEM] = { }, //
		[W_THEME_CLASS_COLUMNITEM] = { }, //
		[W_THEME_CLASS_TABVIEW] = { }, //
		[W_THEME_CLASS_TABITEM] = { }, //
		[W_THEME_CLASS_COMBOBOX] = { }, //
		[W_THEME_CLASS_COOLBAR] = { }, //
		[W_THEME_CLASS_COOLITEM] = { }, //
		[W_THEME_CLASS_DATETIME] = { }, //
		[W_THEME_CLASS_EXPANDBAR
				] = { w_theme_metal_expandbar_compute_trim,
						w_theme_metal_expandbar_draw,
						w_theme_metal_expandbar_measure,
						w_theme_metal_expandbar_get_bounds,
						w_theme_metal_expandbar_hit }, //
		[W_THEME_CLASS_EXPANDITEM] = { w_theme_metal_expanditem_compute_trim,
				w_theme_metal_expanditem_draw, w_theme_metal_expanditem_measure,
				w_theme_metal_expanditem_get_bounds,
				w_theme_metal_expanditem_hit }, //
		[W_THEME_CLASS_GROUPBOX] = { }, //
		[W_THEME_CLASS_SPINNER] = { }, //
		[W_THEME_CLASS_LISTVIEW] = { }, //
		[W_THEME_CLASS_LISTITEM] = { }, //
		[W_THEME_CLASS_TOOLBAR] = { }, //
		[W_THEME_CLASS_TOOLITEM] = { }, //
		};
void w_theme_metal_dispose(w_theme *theme) {
	free(theme);
}
const char* w_theme_metal_get_name(w_theme *theme) {
	return "metal";
}
void w_theme_metal_compute_trim(w_theme *theme, int mask, w_themedata *data,
		w_rect *result) {
}
void w_theme_metal_draw(w_theme *theme, int mask, w_themedata *data) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& w_theme_metal_fun[data->clazz].draw != 0) {
		w_theme_metal_fun[data->clazz].draw(theme, mask, data);
	}
}
void w_theme_metal_measure(w_theme *theme, int mask, w_themedata *data,
		w_size *result) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& w_theme_metal_fun[data->clazz].measure != 0) {
		w_theme_metal_fun[data->clazz].measure(theme, mask, data, result);
	}
}
void w_theme_metal_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& w_theme_metal_fun[data->clazz].get_bounds) {
		w_theme_metal_fun[data->clazz].get_bounds(theme, mask, part, data,
				result);
	}
}
int w_theme_metal_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& w_theme_metal_fun[data->clazz].hit != 0) {
		return w_theme_metal_fun[data->clazz].hit(theme, mask, data, position);
	}
	return W_THEME_PART_NOWHERE;
}
wresult w_theme_metal_get_color(w_theme *theme, wuint colorid, w_color *color) {
	*color = 0x00000000;
	w_theme_metal *t = (w_theme_metal*) theme;
	switch (colorid) {
	case W_COLOR_TITLE_FOREGROUND:
		*color = W_COLOR_BLACK;
		break;
	case W_COLOR_TITLE_BACKGROUND:
		*color = t->primary2;
		break;
	case W_COLOR_TITLE_BACKGROUND_GRADIENT:
		*color = t->gradiant;
		break;
	default:
		break;
	}
	return W_FALSE;
}
wresult w_theme_metal_get_font(w_theme *theme, w_font **font) {
	w_theme_metal *t = (w_theme_metal*) theme;
	*font = t->font;
	return W_TRUE;
}
wresult w_theme_metal_get_cursor(w_theme *theme, wuint id, w_cursor **cursor) {
	w_theme_metal *t = (w_theme_metal*) theme;
	if (id <= W_CURSOR_HAND) {
		//*cursor = (w_cursor*) &t->cursors[id];
		return W_TRUE;
	} else {
		*cursor = 0;
		return W_FALSE;
	}
}
wresult w_theme_metal_get_image(w_theme *theme, wuint id, w_image **image) {
	w_theme_metal *t = (w_theme_metal*) theme;
	*image = 0;
	return W_FALSE;
}
struct _w_theme_class metal_clazz = { //
		w_theme_metal_dispose, //
				w_theme_metal_get_name, //
				{ w_theme_metal_compute_trim, //
						w_theme_metal_draw, //
						w_theme_metal_measure, //
						w_theme_metal_get_bounds, //
						w_theme_metal_hit }, //
				w_theme_metal_get_color, //
				w_theme_metal_get_font, //
				w_theme_metal_get_cursor, //
				w_theme_metal_get_image };
w_theme* w_theme_metal_create(void *args) {
	w_theme_metal *metal = malloc(sizeof(w_theme_metal));
	if (metal == 0)
		return 0;
	w_toolkit *toolkit = w_app_get_platform_toolkit(w_app_get());
	metal->font = w_toolkit_get_system_font(toolkit);
	metal->clazz = &metal_clazz;
	metal->ratio1 = 0.3;
	metal->ratio2 = 0;
	metal->primary1 = 0xFFBF8263;
	metal->primary2 = 0xFFCCB8A3;
	metal->primary3 = 0xFFE5CFB8;
	metal->secondary1 = 0xFF998A7A;
	metal->secondary2 = 0xFFE5CFB8;
	metal->secondary3 = 0xFFEEEEEE;
	metal->gradiant = 0xFFF3E8DD;
	return (w_theme*) metal;
}
wresult w_toolkit_registre_themes() {
	return w_app_registre_theme(w_app_get(), "metal", w_theme_metal_create);
}
