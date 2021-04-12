/*
 * coolbar.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc/swtc.h>
wresult w_coolbar_create(w_coolbar *coolbar, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(coolbar), toolkit, W_WIDGET(parent), style,
			_W_CLASS_COOLBAR, post_event);
}
w_coolbar* w_coolbar_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_COOLBAR(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_COOLBAR,post_event));
}
wresult w_coolbar_get_item(w_coolbar *coolbar, int index, w_coolitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGET_CHECK0(coolbar);
	if (result > 0) {
		return W_COOLBAR_GET_CLASS(coolbar)->get_item(coolbar, index, item);
	} else
		return result;
}
wresult w_coolbar_get_item_count(w_coolbar *coolbar) {
	wresult result = W_WIDGET_CHECK0(coolbar);
	if (result > 0) {
		return W_COOLBAR_GET_CLASS(coolbar)->get_item_count(coolbar);
	} else
		return result;
}
wresult w_coolbar_get_items(w_coolbar *coolbar, w_iterator *items) {
	w_iterator_close(items);
	wresult result = W_WIDGET_CHECK0(coolbar);
	if (result > 0) {
		return W_COOLBAR_GET_CLASS(coolbar)->get_items(coolbar, items);
	} else
		return result;
}
wresult w_coolbar_get_locked(w_coolbar *coolbar) {
	wresult result = W_WIDGET_CHECK0(coolbar);
	if (result > 0) {
		return W_COOLBAR_GET_CLASS(coolbar)->get_locked(coolbar);
	} else
		return result;
}
wresult w_coolbar_insert_item(w_coolbar *coolbar, w_coolitem *item, int style,
		int index) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGET_CHECK0(coolbar);
	if (result > 0) {
		return W_COOLBAR_GET_CLASS(coolbar)->insert_item(coolbar, item, style,
				index);
	} else
		return result;
}
wresult w_coolbar_set_locked(w_coolbar *coolbar, int locked) {
	wresult result = W_WIDGET_CHECK0(coolbar);
	if (result > 0) {
		return W_COOLBAR_GET_CLASS(coolbar)->set_locked(coolbar, locked);
	} else
		return result;
}
/*
 *
 */
wresult w_coolitem_compute_size(w_coolitem *coolitem, w_size *res, int wHint,
		int hHint) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->compute_size(coolitem, res,
				wHint, hHint);
	} else
		return result;
}
wresult w_coolitem_get_bounds(w_coolitem *coolitem, w_rect *bounds) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->get_bounds(coolitem, bounds);
	} else
		return result;
}
wresult w_coolitem_get_control(w_coolitem *coolitem, w_control **control) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->get_control(coolitem, control);
	} else
		return result;
}
wresult w_coolitem_get_minimum_size(w_coolitem *coolitem, w_size *size) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->get_minimum_size(coolitem, size);
	} else
		return result;
}
wresult w_coolitem_get_order(w_coolitem *coolitem) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->get_order(coolitem);
	} else
		return result;
}
wresult w_coolitem_get_preferred_size(w_coolitem *coolitem, w_size *size) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->get_preferred_size(coolitem,
				size);
	} else
		return result;
}
wresult w_coolitem_get_size(w_coolitem *coolitem, w_size *size) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->get_size(coolitem, size);
	} else
		return result;
}
wresult w_coolitem_get_wrap_indice(w_coolitem *coolitem) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->get_wrap_indice(coolitem);
	} else
		return result;
}
wresult w_coolitem_set_control(w_coolitem *coolitem, w_control *control) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->set_control(coolitem, control);
	} else
		return result;
}
wresult w_coolitem_set_layout(w_coolitem *coolitem, int order, int wrapindices,
		w_size *sizes) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->set_layout(coolitem, order,
				wrapindices, sizes);
	} else
		return result;
}
wresult w_coolitem_set_minimum_size(w_coolitem *coolitem, w_size *size) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->set_minimum_size(coolitem, size);
	} else
		return result;
}
wresult w_coolitem_set_order(w_coolitem *coolitem, int order) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->set_order(coolitem, order);
	} else
		return result;
}
wresult w_coolitem_set_preferred_size(w_coolitem *coolitem, w_size *size) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->set_preferred_size(coolitem,
				size);
	} else
		return result;
}
wresult w_coolitem_set_size(w_coolitem *coolitem, w_size *size) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->set_size(coolitem, size);
	} else
		return result;
}
wresult w_coolitem_set_wrap_indice(w_coolitem *coolitem, int wrap_indice) {
	wresult result = W_WIDGETDATA_CHECK0(coolitem);
	if (result > 0) {
		return W_COOLITEM_GET_CLASS(coolitem)->set_wrap_indice(coolitem,
				wrap_indice);
	} else
		return result;
}
