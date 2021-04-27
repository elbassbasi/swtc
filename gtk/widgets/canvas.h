/*
 * Name:        canvas.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_WIDGETS_CANVAS_H_
#define GTK_WIDGETS_CANVAS_H_
#include "composite.h"
/*
 * states
 */
#define STATE_CANVAS_END (STATE_COMPOSITE_END + 0)
typedef struct _w_caret {
	struct _w_caret_class *clazz;
	w_canvas *parent;
	w_rect bounds;
	unsigned isVisible :1;
	unsigned isShowing :1;
	int blinkRate;
	w_image image;
	w_font *font;
} _w_caret;
#define _W_CARET(x) ((_w_caret*)x)
typedef struct _w_ime {
	struct _w_ime_class *clazz;
	w_canvas *parent;
	int caretOffset;
	int startOffset;
	int commitCount;
	char *text;
	char *preeditString;
	PangoAttrList *pangoAttrs;
	unsigned inComposition :1;
} _w_ime;
#define _W_IME(x) ((_w_ime*)x)
/*
 * canvas
 */
typedef struct _w_canvas {
	_w_composite composite;
	w_caret *caret;
	w_ime *ime;
} _w_canvas;
#define _W_CANVAS(x) ((_w_canvas*)x)
/*
 * private
 */
typedef struct _w_canvas_priv _w_canvas_priv;
struct _w_canvas_priv {
	_w_composite_priv composite;
};
#define _W_CANVAS_PRIV(x) ((_w_canvas_priv*)x)
#define _W_CANVAS_GET_PRIV(x) ((_w_canvas_priv*)_w_widget_get_priv(W_WIDGET(x)))
/*
 * functions
 */
/*
 * caret
 */
wresult _w_caret_blink_caret(w_caret *caret);
wresult _w_caret_draw_caret(w_caret *caret);
wresult _w_caret_get_bounds(w_caret *caret, w_point *location, w_size *size);
wresult _w_caret_get_font(w_caret *caret, w_font **font);
wresult _w_caret_get_image(w_caret *caret, w_image *image);
w_canvas* _w_caret_get_parent(w_caret *caret);
wresult _w_caret_get_visible(w_caret *caret);
wresult _w_caret_hide_caret(w_caret *caret);
wresult _w_caret_is_visible(w_caret *caret);
wresult _w_caret_is_focus_caret(w_caret *caret);
gboolean _w_caret_proc(gpointer clientData);
void _w_caret_set_current_caret(w_caret *caret);
void _w_caret_kill_focus(w_caret *caret);
void _w_caret_copy(w_widgetdata *from, w_widgetdata *to);
wresult _w_caret_equals(w_widgetdata *obj1, w_widgetdata *obj2);
void _w_caret_close(w_widgetdata *widgetdata);
wresult _w_caret_set_bounds(w_caret *caret, w_point *location, w_size *size);
void _w_caret_set_focus(w_caret *caret);
wresult _w_caret_set_font(w_caret *caret, w_font *font);
wresult _w_caret_set_image(w_caret *caret, w_image *image);
wresult _w_caret_set_visible(w_caret *caret, int visible);
wresult _w_caret_show_caret(w_caret *caret);
/*
 * IME
 */
int _w_ime_get_caret_offset(w_ime *ime);
int _w_ime_get_commit_count(w_ime *ime);
int _w_ime_get_composition_offset(w_ime *ime);
size_t _w_ime_get_ranges(w_ime *ime, w_range *ranges, w_textstyle *styles,
		size_t _length);
wresult _w_ime_get_text(w_ime *ime, w_alloc text, void *user_data);
wresult _w_ime_get_wide_caret(w_ime *ime);
void _w_ime_copy(w_widgetdata *from, w_widgetdata *to);
wresult _w_ime_equals(w_widgetdata *obj1, w_widgetdata *obj2);
void _w_ime_close(w_widgetdata *widgetdata);
wresult _w_ime_set_composition_offset(w_ime *ime, int offset);
/*
 * canvas
 */
w_caret* _w_canvas_get_caret(w_canvas *canvas);
wresult _w_canvas_create_caret(w_canvas *canvas, w_caret *caret);
wresult _w_canvas_create_ime(w_canvas *canvas, w_ime *ime);
wresult _w_canvas_get_imcaret_pos(w_control *control, w_point *pos,
		_w_control_priv *priv);
void _w_canvas_redraw_widget(w_control *control, w_rect *rect, int flags,
		_w_control_priv *priv);
wresult _w_canvas_scroll(w_canvas *canvas, w_point *_dest, w_rect *_rect,
		int all);
wresult _w_canvas_set_bounds(w_control *control, w_point *location,
		w_size *size, _w_control_priv *priv);
void _w_canvas_set_foreground_color(w_control *control, GtkWidget *handle,
		GdkRGBA *rgba, _w_control_priv *priv);
wresult _w_canvas_set_caret(w_canvas *canvas, w_caret *caret);
wresult _w_canvas_set_ime(w_canvas *canvas, w_ime *ime);
w_ime* _w_canvas_get_ime(w_canvas *canvas);
wresult _w_canvas_set_font(w_control *control, w_font *font);
void _w_canvas_update_caret(w_canvas *canvas);
/*
 * signals
 */
gboolean _gtk_ime_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_ime_commit(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_ime_preedit_changed(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_canvas_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_canvas_commit(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_canvas_draw(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_canvas_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_canvas_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
gboolean _gtk_canvas_preedit_changed(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
void _w_canvas_class_init(struct _w_canvas_class *clazz);
/*
 * messages
 */
#endif /* GTK_WIDGETS_CANVAS_H_ */
