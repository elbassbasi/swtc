/*
 * button.c
 *
 *  Created on: 28 juin 2021
 *      Author: Azeddine
 */
#include "button.h"
wresult cw_button_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	wresult result = cw_control_create(widget, parent, style, post_event,
			sizeof(struct cw_button_priv));
	if (result < 0)
		return result;
	return result;
}
void cw_button_draw(w_widget *widget, w_graphics *gc) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	w_rect r;
	w_size sz;
	w_control_get_bounds(W_CONTROL(widget), &r.pt, &r.sz);
	r.x = 0;
	r.y = 0;
	w_theme *theme;
	w_widget_get_theme(widget, &theme);
	w_widget_get_theme(widget, &theme);
	w_themedata data;
	w_widget_init_themedata(widget, &data);
	data.gc = gc;
	data.bounds = &r;
	data.clazz = W_THEME_CLASS_BUTTON;
	data.clientArea = 0;
	data.state = priv->state;
	data.style = w_widget_get_style(widget);
	data.attr.text = priv->text;
	data.attr.enc = W_ENCODING_UTF8;
	w_theme_draw(theme, W_THEME_MASK_ALL, &data);
}
int cw_button_paint(w_widget *widget, w_event_paint *e) {
	cw_button_draw(widget, e->gc);
	return W_TRUE;
}
int cw_button_mousedown(w_widget *widget, w_event_mouse *e) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	priv->state |= W_PRESSED;
	w_graphics gc;
	w_graphics_init(&gc);
	w_control_get_graphics(W_CONTROL(widget), &gc);
	cw_button_draw(widget, &gc);
	w_graphics_dispose(&gc);
	return W_TRUE;
}
int cw_button_mouseup(w_widget *widget, w_event_mouse *e) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	priv->state &= ~W_PRESSED;
	w_graphics gc;
	w_graphics_init(&gc);
	w_control_get_graphics(W_CONTROL(widget), &gc);
	cw_button_draw(widget, &gc);
	w_graphics_dispose(&gc);
	return W_TRUE;
}
wresult cw_button_compute_size(w_widget *widget, w_event_compute_size *e) {
	if (e->wHint != W_DEFAULT && e->wHint != W_DEFAULT) {
		e->size->width = e->wHint;
		e->size->height = e->hHint;
		return W_TRUE;
	}
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(widget));
	w_graphics gc;
	w_graphics_init(&gc);
	w_control_get_graphics(W_CONTROL(widget), &gc);
	w_theme *theme;
	w_widget_get_theme(widget, &theme);
	w_themedata data;
	w_widget_init_themedata(widget, &data);
	data.gc = &gc;
	w_size result;
	data.clazz = W_THEME_CLASS_BUTTON;
	data.clientArea = 0;
	data.state = priv->state;
	data.style = w_widget_get_style(widget);
	data.attr.text = priv->text;
	data.attr.enc = W_ENCODING_UTF8;
	w_theme_measure(theme, W_THEME_MASK_ALL, &data, &result);
	w_graphics_dispose(&gc);
	if (e->wHint == W_DEFAULT)
		e->size->width = result.width;
	else
		e->size->width = e->wHint;
	if (e->hHint == W_DEFAULT)
		e->size->height = result.height;
	else
		e->size->height = e->hHint;
	return W_TRUE;
}
wresult cw_button_post_event(w_widget *widget, w_event *e, int flags) {
	switch (e->type) {
	case W_EVENT_PAINT:
		return cw_button_paint(widget, (w_event_paint*) e);
		break;
	case W_EVENT_MOUSEDOWN:
		return cw_button_mousedown(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_MOUSEUP:
		return cw_button_mouseup(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_COMPUTE_SIZE:
		return cw_button_compute_size(widget, (w_event_compute_size*) e);
		break;
	}
	return widget->clazz->parentClass->post_event(widget, e, W_EVENT_SEND);
}
wresult cw_button_get_alignment(w_button *button) {
	return 0;
}
wresult cw_button_get_grayed(w_button *button) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(button));
	return priv->state & W_GRAYED != 0;
}
wresult cw_button_get_image(w_button *button, w_image *image) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(button));
	if (w_image_is_ok(&priv->image)) {
		return w_image_copy(&priv->image, 0, image);
	}
	return W_FALSE;
}
wresult cw_button_get_selection(w_button *button) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(button));
	return priv->state & W_SELECTED != 0;
}
wresult cw_button_get_text(w_button *button, w_alloc alloc, void *user_data,
		int enc) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(button));
	return w_alloc_set_text(alloc, user_data, enc, priv->text, -1,
			W_ENCODING_UTF8);
}
wresult cw_button_set_alignment(w_button *button, int alignment) {
	return 0;
}
wresult cw_button_set_grayed(w_button *button, int grayed) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(button));
	if (grayed) {
		priv->state |= W_GRAYED;
	} else {
		priv->state &= ~W_GRAYED;
	}
	return W_TRUE;
}
wresult cw_button_set_image(w_button *button, w_image *image) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(button));
	if (w_image_is_ok(image)) {
		return w_image_copy(image, 0, &priv->image);
	}
	return W_FALSE;
}
wresult cw_button_set_selection(w_button *button, int selected) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(button));
	if (selected) {
		priv->state |= W_SELECTED;
	} else {
		priv->state &= ~W_SELECTED;
	}
	return W_TRUE;
}
wresult cw_button_set_text(w_button *button, const char *string, int length,
		int enc) {
	cw_button_priv *priv = cw_control_get_priv(W_CONTROL(button));
	if (priv->text != 0) {
		free(priv->text);
	}
	priv->text = strdup(string);
	w_control_redraw(W_CONTROL(button), 0, W_FALSE);
	return W_TRUE;
}
void cw_button_init_class(w_toolkit *toolkit, wushort classId,
		struct _w_widget_class *clazz) {
	cw_ccanvas_init_class_priv(toolkit, classId, clazz,
			sizeof(struct cw_button_priv));
	clazz->create = cw_button_create;
	clazz->post_event = cw_button_post_event;

	W_BUTTON_CLASS(clazz)->get_alignment = cw_button_get_alignment;
	W_BUTTON_CLASS(clazz)->get_grayed = cw_button_get_grayed;
	W_BUTTON_CLASS(clazz)->get_image = cw_button_get_image;
	W_BUTTON_CLASS(clazz)->get_selection = cw_button_get_selection;
	W_BUTTON_CLASS(clazz)->get_text = cw_button_get_text;
	W_BUTTON_CLASS(clazz)->set_alignment = cw_button_set_alignment;
	W_BUTTON_CLASS(clazz)->set_grayed = cw_button_set_grayed;
	W_BUTTON_CLASS(clazz)->set_image = cw_button_set_image;
	W_BUTTON_CLASS(clazz)->set_selection = cw_button_set_selection;
	W_BUTTON_CLASS(clazz)->set_text = cw_button_set_text;

}

