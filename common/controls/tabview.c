/*
 * tabview.c
 *
 *  Created on: 12 juin 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
/*
 * tabitem
 */
w_control* w_tabitem_get_control(w_tabitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		w_widget *widget;
		w_item_get_parent_widget(W_ITEM(item), &widget);
		if (widget != 0) {
			w_event_tabitem event;
			event.event.type = W_EVENT_ITEM_GET_CONTROL;
			event.event.platform_event = 0;
			event.event.data = 0;
			event.event.widget = widget;
			event.control = 0;
			event.item = item;
			w_widget_send_event(widget, (w_event*) &event);
			return event.control;
		}
	}
	return 0;
}
wresult w_tabitem_get_bounds(w_tabitem *item, w_rect *rect) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TABITEM_GET_CLASS(item)->get_bounds(item, rect);
	} else
		return W_ERROR_NO_HANDLES;
}
int w_tabitem_get_image(w_tabitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TABITEM_GET_CLASS(item)->get_image(item);
	} else
		return -1;
}
wresult w_tabitem_remove(w_tabitem *item, int destroy_Control) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TABITEM_GET_CLASS(item)->remove(item, destroy_Control);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tabitem_set_image(w_tabitem *item, int image) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TABITEM_GET_CLASS(item)->set_image(item, image);
	} else
		return W_ERROR_NO_HANDLES;
}

wresult w_tabitem_set_control(w_tabitem *item, w_control *control) {
	if (W_WIDGETDATA_CHECK(item)) {
		w_widget *widget;
		w_item_get_parent_widget(W_ITEM(item), &widget);
		if (widget != 0) {
			w_event_tabitem event;
			event.event.type = W_EVENT_ITEM_SET_CONTROL;
			event.event.platform_event = 0;
			event.event.data = 0;
			event.event.widget = widget;
			event.control = control;
			event.item = item;
			int ret = w_widget_send_event(widget, (w_event*) &event);
			W_TABITEM_GET_CLASS(item)->pack(item, control);
			return ret;
		}
		return W_FALSE;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tabitem_pack(w_tabitem *item, w_control *control) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TABITEM_GET_CLASS(item)->pack(item, control);
	} else
		return W_ERROR_NO_HANDLES;
}
/*
 * tabview
 */
wresult w_tabview_create(w_tabview *tabview, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(tabview), toolkit, W_WIDGET(parent), style,
			_W_CLASS_TABVIEW, post_event);
}
w_tabview* w_tabview_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_TABVIEW(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TABVIEW,post_event));
}
wresult w_tabview_get_imagelist(w_tabview *tabview, w_imagelist **imagelist) {
	wresult result = W_WIDGET_CHECK0(tabview);
	if (result > 0) {
		return W_TABVIEW_GET_CLASS(tabview)->get_imagelist(tabview, imagelist);
	} else
		return result;
}
wresult w_tabview_get_item(w_tabview *tabview, int index, w_tabitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGET_CHECK0(tabview);
	if (result > 0) {
		return W_TABVIEW_GET_CLASS(tabview)->get_item(tabview, index, item);
	} else
		return result;
}
wresult w_tabview_get_item_p(w_tabview *tabview, w_point *point,
		w_tabitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGET_CHECK0(tabview);
	if (result > 0) {
		return W_TABVIEW_GET_CLASS(tabview)->get_item_p(tabview, point, item);
	} else
		return result;
}
wresult w_tabview_get_item_count(w_tabview *tabview) {
	wresult result = W_WIDGET_CHECK0(tabview);
	if (result > 0) {
		return W_TABVIEW_GET_CLASS(tabview)->get_item_count(tabview);
	} else
		return result;
}
wresult w_tabview_get_items(w_tabview *tabview, w_iterator *items) {
	wresult result = W_WIDGET_CHECK0(tabview);
	if (result > 0) {
		return W_TABVIEW_GET_CLASS(tabview)->get_items(tabview, items);
	} else
		return result;
}
wresult w_tabview_get_selection(w_tabview *tabview, w_tabitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGET_CHECK0(tabview);
	if (result > 0) {
		return W_TABVIEW_GET_CLASS(tabview)->get_selection(tabview, item);
	} else
		return result;
}
wresult w_tabview_insert_item(w_tabview *tabview, w_tabitem *item, int index) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGET_CHECK0(tabview);
	if (result > 0) {
		return W_TABVIEW_GET_CLASS(tabview)->insert_item(tabview, item, index);
	} else
		return result;
}
wresult w_tabview_set_imagelist(w_tabview *tabview, w_imagelist *imagelist) {
	wresult result = W_WIDGET_CHECK0(tabview);
	if (result > 0) {
		return W_TABVIEW_GET_CLASS(tabview)->set_imagelist(tabview, imagelist);
	} else
		return result;
}

wresult w_tabview_set_selection(w_tabview *tabview, int index) {
	wresult result = W_WIDGET_CHECK0(tabview);
	if (result > 0) {
		return W_TABVIEW_GET_CLASS(tabview)->set_selection(tabview, index);
	} else
		return result;
}
