/*
 * toolbar.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_toolbar_create(w_toolbar *toolbar, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(toolbar), toolkit, W_WIDGET(parent), style,
			_W_CLASS_TOOLBAR, post_event);
}
w_toolbar* w_toolbar_new(struct w_toolkit *toolkit, w_composite *parent,
                         wuint64 style, w_widget_post_event_proc post_event) {
	return W_TOOLBAR(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TOOLBAR,post_event));
}

w_imagelist* w_toolbar_get_imagelist(w_toolbar *toolbar){
	wresult result = W_WIDGET_CHECK0(toolbar);
	if (result > 0) {
		return W_TOOLBAR_GET_CLASS(toolbar)->get_imagelist(toolbar);
	} else
		return 0;
}
wresult w_toolbar_get_item(w_toolbar *toolbar, int index, w_toolitem *item) {
	wresult result = W_WIDGET_CHECK0(toolbar);
	if (result > 0) {
		return W_TOOLBAR_GET_CLASS(toolbar)->get_item(toolbar, index, item);
	} else
		return W_FALSE;
}
wresult w_toolbar_get_item_from_point(w_toolbar *toolbar, w_point *point,
		w_toolitem *item) {
	wresult result = W_WIDGET_CHECK0(toolbar);
	if (result > 0) {
		return W_TOOLBAR_GET_CLASS(toolbar)->get_item_from_point(toolbar, point,
				item);
	} else
		return W_FALSE;
}
int w_toolbar_get_item_count(w_toolbar *toolbar) {
	wresult result = W_WIDGET_CHECK0(toolbar);
	if (result > 0) {
		return W_TOOLBAR_GET_CLASS(toolbar)->get_item_count(toolbar);
	} else
		return result;
}
void w_toolbar_get_items(w_toolbar *toolbar, w_iterator *items) {
	w_iterator_close(items);
	wresult result = W_WIDGET_CHECK0(toolbar);
	if (result > 0) {
		return W_TOOLBAR_GET_CLASS(toolbar)->get_items(toolbar, items);
	}
}
int w_toolbar_get_row_count(w_toolbar *toolbar) {
	wresult result = W_WIDGET_CHECK0(toolbar);
	if (result > 0) {
		return W_TOOLBAR_GET_CLASS(toolbar)->get_row_count(toolbar);
	} else
		return 0;
}
wresult w_toolbar_insert(w_toolbar *toolbar, w_toolitem *item,
		const char *text, int style, int index) {
	wresult result = W_WIDGET_CHECK0(toolbar);
	if (result > 0) {
		return W_TOOLBAR_GET_CLASS(toolbar)->insert(toolbar, item, text, style,
				index);
	} else
		return result;
}

wresult w_toolbar_set_imagelist(w_toolbar *toolbar,w_imagelist* imagelist){
	wresult result = W_WIDGET_CHECK0(toolbar);
	if (result > 0) {
		return W_TOOLBAR_GET_CLASS(toolbar)->set_imagelist(toolbar,imagelist);
	} else
		return result;
}
wresult w_toolitem_get_bounds(w_toolitem *item, w_rect *rect) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->get_bounds(item, rect);
	} else
		return result;
}
w_control* w_toolitem_get_control(w_toolitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->get_control(item);
	} else
		return 0;
}
wresult w_toolitem_get_enabled(w_toolitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->get_enabled(item);
	} else
		return W_FALSE;
}
wresult w_toolitem_get_selection(w_toolitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->get_selection(item);
	} else
		return W_FALSE;
}
wresult w_toolitem_get_tooltip_text(w_toolitem *item,w_alloc alloc,void* user_data) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->get_tooltip_text(item, alloc,user_data);
	} else
		return result;
}
int w_toolitem_get_width(w_toolitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->get_width(item);
	} else
		return 0;
}
wresult w_toolitem_is_enabled(w_toolitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->is_enabled(item);
	} else
		return W_FALSE;
}
wresult w_toolitem_set_control(w_toolitem *item, w_control *control) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->set_control(item, control);
	} else
		return result;
}
wresult w_toolitem_set_enabled(w_toolitem *item, int enabled) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->set_enabled(item, enabled);
	} else
		return result;
}
wresult w_toolitem_set_image(w_toolitem *item, int image) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->set_image(item, image);
	} else
		return result;
}
wresult w_toolitem_set_selection(w_toolitem *item, int selected) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->set_selection(item, selected);
	} else
		return result;
}
wresult w_toolitem_set_tooltip_text(w_toolitem *item, const char *string) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->set_tooltip_text(item, string);
	} else
		return result;
}
wresult w_toolitem_set_width(w_toolitem *item, int width) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TOOLITEM_GET_CLASS(item)->set_width(item, width);
	} else
		return result;
}
