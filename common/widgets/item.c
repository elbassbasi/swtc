/*
 * item.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
wresult w_widgetdata_check(w_widgetdata *obj) {
	if (obj != 0 && obj->clazz != 0) {
		w_toolkit *toolkit = obj->clazz->toolkit;
		return W_TOOLKIT_GET_CLASS(toolkit)->check_widgetdata(toolkit, obj);
	} else
		return W_ERROR_NO_HANDLES;
}
void w_widgetdata_init(w_widgetdata *obj) {
	obj->clazz = 0;
}
wresult w_widgetdata_is_ok(w_widgetdata *obj) {
	wresult result = W_WIDGETDATA_CHECK0(obj);
	if (obj != 0 && obj->clazz != 0) {
		return obj->clazz->is_ok(obj);
	} else {
		return W_FALSE;
	}
}
wresult w_widgetdata_close(w_widgetdata *obj) {
	wresult result = W_WIDGETDATA_CHECK0(obj);
	if (result > 0) {
		obj->clazz->close(obj);
		obj->clazz = 0;
		return W_TRUE;
	} else {
		return result;
	}
}
wresult w_widgetdata_copy(w_widgetdata *from, w_widgetdata *to) {
	w_widgetdata_close(to);
	wresult result = W_WIDGETDATA_CHECK0(from);
	if (result > 0) {
		return from->clazz->copy(from, to);
	} else {
		return result;
	}
}
wresult w_widgetdata_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
	wresult result = W_WIDGETDATA_CHECK0(obj1);
	if (result > 0) {
		return obj1->clazz->equals(obj1, obj2);
	} else {
		return result;
	}
}
wresult w_widgetdata_init_copy(w_widgetdata *from, w_widgetdata *to) {
	wresult result = W_WIDGETDATA_CHECK0(from);
	if (result > 0) {
		return from->clazz->copy(from, to);
	} else {
		to->clazz = 0;
		return result;
	}
}
/*
 * private
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
}
/*
 * item
 */
wresult w_item_get_parent_widget(w_item *item, w_widget **parent) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_ITEM_GET_CLASS(item)->get_parent_widget(item, parent);
	} else {
		return result;
	}
}
wresult w_item_get_data(w_item *item, void **data) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_ITEM_GET_CLASS(item)->get_data(item, data);
	} else {
		return result;
	}
}
wresult w_item_get_index(w_item *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_ITEM_GET_CLASS(item)->get_index(item);
	} else {
		return result;
	}
}
wresult w_item_get_text(w_item *item, w_alloc alloc, void *user_data, int enc) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_ITEM_GET_CLASS(item)->get_text(item, alloc, user_data, enc);
	} else {
		return result;
	}
}
wresult w_item_set_data(w_item *item, void *data) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_ITEM_GET_CLASS(item)->set_data(item, data);
	} else {
		return result;
	}
}
wresult w_item_set_text(w_item *item, const char *text, int length, int enc) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_ITEM_GET_CLASS(item)->set_text(item, text, length, enc);
	} else {
		return result;
	}
}
/*
 * private
 */
wresult _w_item_copy(w_widgetdata *from, w_widgetdata *to) {
	W_WIDGETDATA(to)->clazz = W_WIDGETDATA(from)->clazz;
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
