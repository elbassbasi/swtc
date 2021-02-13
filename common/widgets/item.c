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
