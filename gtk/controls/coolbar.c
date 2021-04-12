/*
 * coolbar.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "coolbar.h"
#include "../widgets/toolkit.h"
/*
 * coolitem
 */
wresult _w_coolitem_compute_size(w_coolitem *coolitem, w_size *result,
		int wHint, int hHint) {
	return W_FALSE;
}
wresult _w_coolitem_get_bounds(w_coolitem *coolitem, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_coolitem_get_control(w_coolitem *coolitem, w_control **control) {
	return W_FALSE;
}
wresult _w_coolitem_get_minimum_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_get_order(w_coolitem *coolitem) {
	return W_FALSE;
}
wresult _w_coolitem_get_preferred_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_get_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_get_wrap_indice(w_coolitem *coolitem) {
	return W_FALSE;
}
wresult _w_coolitem_set_control(w_coolitem *coolitem, w_control *control) {
	return W_FALSE;
}
wresult _w_coolitem_set_layout(w_coolitem *coolitem, int order, int wrapindices,
		w_size *sizes) {
	return W_FALSE;
}
wresult _w_coolitem_set_minimum_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_set_order(w_coolitem *coolitem, int order) {
	return W_FALSE;
}
wresult _w_coolitem_set_preferred_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_set_size(w_coolitem *coolitem, w_size *size) {
	return W_FALSE;
}
wresult _w_coolitem_set_wrap_indice(w_coolitem *coolitem, int wrap_indice) {
	return W_FALSE;
}
/*
 * coolbar
 */
wuint64 _w_coolbar_check_style(w_widget *control, wuint64 style) {
	style |= W_NO_FOCUS;
	return (style | W_NO_REDRAW_RESIZE) & ~(W_VSCROLL | W_HSCROLL);
}
wresult _w_coolbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _w_coolbar_create_handle(w_widget *widget, _w_control_priv *priv) {
	wresult result = _w_composite_create_handle(widget, priv);
	return result;
}
wresult _w_coolbar_get_item(w_coolbar *coolbar, int index, w_coolitem *item) {
	return W_FALSE;
}
wresult _w_coolbar_get_item_count(w_coolbar *coolbar) {
	return W_FALSE;
}
wresult _w_coolbar_get_items(w_coolbar *coolbar, w_iterator *items) {
	return W_FALSE;
}
wresult _w_coolbar_get_locked(w_coolbar *coolbar) {
	return W_FALSE;
}
wresult _w_coolbar_insert_item(w_coolbar *coolbar, w_coolitem *item, int style,
		int index) {
	return W_FALSE;
}
wresult _w_coolbar_set_locked(w_coolbar *coolbar, int locked) {
	return W_FALSE;
}

void _w_coolbar_class_init(struct _w_coolbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_coolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_coolbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_coolbar);
	/*
	 * public function
	 */
	clazz->get_item = _w_coolbar_get_item;
	clazz->get_item_count = _w_coolbar_get_item_count;
	clazz->get_items = _w_coolbar_get_items;
	clazz->get_locked = _w_coolbar_get_locked;
	clazz->insert_item = _w_coolbar_insert_item;
	clazz->set_locked = _w_coolbar_set_locked;
	/*
	 * item
	 */
	struct _w_coolitem_class *item = clazz->class_coolitem;
	_w_item_class_init(W_ITEM_CLASS(item));
	item->compute_size = _w_coolitem_compute_size;
	item->get_bounds = _w_coolitem_get_bounds;
	item->get_control = _w_coolitem_get_control;
	item->get_minimum_size = _w_coolitem_get_minimum_size;
	item->get_order = _w_coolitem_get_order;
	item->get_preferred_size = _w_coolitem_get_preferred_size;
	item->get_size = _w_coolitem_get_size;
	item->get_wrap_indice = _w_coolitem_get_wrap_indice;
	item->set_control = _w_coolitem_set_control;
	item->set_layout = _w_coolitem_set_layout;
	item->set_minimum_size = _w_coolitem_set_minimum_size;
	item->set_order = _w_coolitem_set_order;
	item->set_preferred_size = _w_coolitem_set_preferred_size;
	item->set_size = _w_coolitem_set_size;
	item->set_wrap_indice = _w_coolitem_set_wrap_indice;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.compute_size = _w_coolbar_compute_size;
	priv->widget.check_style = _w_coolbar_check_style;
	priv->widget.create_handle = _w_coolbar_create_handle;

}
