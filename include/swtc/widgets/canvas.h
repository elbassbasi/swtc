/*
 * Name:        canvas.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_CANVAS_H_
#define SWT_WIDGETS_CANVAS_H_
#include "composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_caret w_caret;
typedef struct w_ime w_ime;
typedef struct w_canvas w_canvas;
/*
 * caret
 */
struct w_caret {
	w_widgetdata widgetdata;
	void *handle[0x10];
};
struct _w_caret_class {
	struct _w_widgetdata_class widgetdata;
	wresult (*get_bounds)(w_caret *caret, w_point *location, w_size *size);
	wresult (*get_font)(w_caret *caret, w_font **font);
	wresult (*get_image)(w_caret *caret, w_image *image);
	wresult (*get_parent)(w_caret *caret, w_canvas **parent);
	wresult (*get_visible)(w_caret *caret);
	wresult (*is_visible)(w_caret *caret);
	wresult (*set_bounds)(w_caret *caret, w_point *location, w_size *size);
	wresult (*set_font)(w_caret *caret, w_font *font);
	wresult (*set_image)(w_caret *caret, w_image *image);
	wresult (*set_visible)(w_caret *caret, int visible);
};
SWT_PUBLIC wresult w_caret_get_bounds(w_caret *caret, w_point *location,
		w_size *size);
SWT_PUBLIC wresult w_caret_get_font(w_caret *caret, w_font **font);
SWT_PUBLIC wresult w_caret_get_image(w_caret *caret, w_image *image);
SWT_PUBLIC wresult w_caret_get_location(w_caret *caret, w_point *location);
SWT_PUBLIC wresult w_caret_get_parent(w_caret *caret, w_canvas **parent);
SWT_PUBLIC wresult w_caret_get_size(w_caret *caret, w_size *size);
SWT_PUBLIC wresult w_caret_get_visible(w_caret *caret);
SWT_PUBLIC wresult w_caret_is_visible(w_caret *caret);
SWT_PUBLIC wresult w_caret_set_bounds(w_caret *caret, w_point *location,
		w_size *size);
SWT_PUBLIC wresult w_caret_set_font(w_caret *caret, w_font *font);
SWT_PUBLIC wresult w_caret_set_image(w_caret *caret, w_image *image);
SWT_PUBLIC wresult w_caret_set_visible(w_caret *caret, int visible);
#define W_CARET(x) ((w_caret*)x)
#define W_CARET_CLASS(x) ((struct _w_caret_class*)x)
#define W_CARET_GET_CLASS(x) ((struct _w_caret_class*)W_WIDGETDATA_GET_CLASS(x))
/*
 * IME
 */
struct w_ime {
	w_widgetdata widgetdata;
	void *handle[0x10];
};
struct _w_ime_class {
	struct _w_widgetdata_class widgetdata;
	wresult (*get_caret_offset)(w_ime *ime);
	wresult (*get_commit_count)(w_ime *ime);
	wresult (*get_composition_offset)(w_ime *ime);
	wresult (*get_ranges)(w_ime *ime, w_iterator *ranges);
	wresult (*get_text)(w_ime *ime, w_alloc alloc, void *user_data);
	wresult (*get_wide_caret)(w_ime *ime);
	wresult (*set_composition_offset)(w_ime *ime, int offset);
};
typedef struct w_event_ime {
	w_event event;
	int detail;
	w_range range;
	const char *text;
} w_event_ime;
SWT_PUBLIC wresult w_ime_get_caret_offset(w_ime *ime);
SWT_PUBLIC wresult w_ime_get_commit_count(w_ime *ime);
SWT_PUBLIC wresult w_ime_get_composition_offset(w_ime *ime);
SWT_PUBLIC wresult w_ime_get_ranges(w_ime *ime, w_iterator *ranges);
SWT_PUBLIC wresult w_ime_get_text(w_ime *ime, w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_ime_get_wide_caret(w_ime *ime);
SWT_PUBLIC wresult w_ime_set_composition_offset(w_ime *ime, int offset);
#define W_IME(x) ((w_ime*)x)
#define W_IME_CLASS(x) ((struct _w_ime_class*)x)
#define W_IME_GET_CLASS(x) ((struct _w_ime_class*)W_WIDGETDATA_GET_CLASS(x))
/*
 * canvas
 */
struct w_canvas {
	w_composite composite;
};
struct _w_canvas_class {
	struct _w_composite_class composite;
	wresult (*create_caret)(w_canvas *canvas, w_caret *caret);
	wresult (*create_ime)(w_canvas *canvas, w_ime *ime);
	wresult (*scroll)(w_canvas *canvas, w_point *dest, w_rect *rect, int all);
	wresult (*get_caret)(w_canvas *canvas, w_caret **caret);
	wresult (*set_caret)(w_canvas *canvas, w_caret *caret);
	wresult (*set_ime)(w_canvas *canvas, w_ime *ime);
	wresult (*get_ime)(w_canvas *canvas, w_ime **ime);
};
#define W_CANVAS(x) ((w_canvas*)x)
#define W_CANVAS_CLASS(x) ((struct _w_canvas_class*)x)
#define W_CANVAS_GET_CLASS(x) ((struct _w_canvas_class*)W_WIDGET_GET_CLASS(x))
SWT_PUBLIC wresult w_canvas_create(w_canvas *canvas, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_canvas* w_canvas_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_canvas_create_caret(w_canvas *canvas, w_caret *caret);
SWT_PUBLIC wresult w_canvas_create_ime(w_canvas *canvas, w_ime *ime);
SWT_PUBLIC wresult w_canvas_scroll(w_canvas *canvas, w_point *dest,
		w_rect *rect, int all);
SWT_PUBLIC wresult w_canvas_get_caret(w_canvas *canvas, w_caret **caret);
SWT_PUBLIC wresult w_canvas_set_caret(w_canvas *canvas, w_caret *caret);
SWT_PUBLIC wresult w_canvas_set_ime(w_canvas *canvas, w_ime *ime);
SWT_PUBLIC wresult w_canvas_get_ime(w_canvas *canvas, w_ime **ime);

#ifdef __cplusplus
}
#endif

#endif /* SWT_WIDGETS_CANVAS_H_ */
