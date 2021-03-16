/*
 * item.c
 *
 *  Created on: 16 mars 2021
 *      Author: Azeddine
 */
#include "item.h"
#include "toolkit.h"
/*
 * widgetdata
 */
wresult _w_widgetdata_is_ok(w_widgetdata *obj) {
	return W_TRUE;
}
wresult _w_widgetdata_close(w_widgetdata *obj) {
	return W_TRUE;
}
wresult _w_widgetdata_copy(w_widgetdata *from, w_widgetdata *to) {
	return W_FALSE;
}
wresult _w_widgetdata_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
	return obj1 == obj2;
}
void _w_widgetdata_class_init(struct _w_widgetdata_class *clazz) {
	clazz->is_ok = _w_widgetdata_is_ok;
	clazz->close = _w_widgetdata_close;
	clazz->copy = _w_widgetdata_copy;
	clazz->equals = _w_widgetdata_equals;
	clazz->toolkit = W_TOOLKIT(win_toolkit);
}
/*
 * item
 */
wresult _w_item_copy(w_widgetdata *from, w_widgetdata *to) {
	_W_WIDGETDATA(to)->clazz = _W_WIDGETDATA(from)->clazz;
	_W_ITEM(to)->parent = _W_ITEM(from)->parent;
	_W_ITEM(to)->index = _W_ITEM(from)->index;
	return W_TRUE;
}
wresult _w_item_get_parent_widget(w_item *item, w_widget **parent) {
	*parent = _W_ITEM(item)->parent;
	return W_TRUE;
}
wresult _w_item_get_data(w_item *item, void **data) {
	*data = 0;
	return W_TRUE;
}
wresult _w_item_get_index(w_item *item) {
	return _W_ITEM(item)->index;
}
wresult _w_item_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_item_set_data(w_item *item, void *data) {
	return W_FALSE;
}
wresult _w_item_set_text(w_item *item, const char *text, int length, int enc) {
	return W_FALSE;
}
void _w_item_class_init(struct _w_item_class *clazz) {
	_w_widgetdata_class_init(W_WIDGETDATA_CLASS(clazz));
	W_WIDGETDATA_CLASS(clazz)->copy = _w_item_copy;
	clazz->get_parent_widget = _w_item_get_parent_widget;
	clazz->get_data = _w_item_get_data;
	clazz->get_index = _w_item_get_index;
	clazz->get_text = _w_item_get_text;
	clazz->set_data = _w_item_set_data;
	clazz->set_text = _w_item_set_text;
}




