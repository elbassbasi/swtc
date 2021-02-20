/*
 * item.h
 *
 *  Created on: 19 févr. 2021
 *      Author: azeddine
 */

#ifndef GTK_WIDGETS_ITEM_H_
#define GTK_WIDGETS_ITEM_H_
#include "widget.h"
/*
 * widgetdata
 */
typedef struct _w_widgetdata {
	struct _w_widgetdata_class *clazz;
} _w_widgetdata;
#define _W_WIDGETDATA(x) ((_w_widgetdata*)x)
wresult _w_widgetdata_is_ok(w_widgetdata *obj);
wresult _w_widgetdata_close(w_widgetdata *obj);
wresult _w_widgetdata_copy(w_widgetdata *from, w_widgetdata *to);
wresult _w_widgetdata_equals(w_widgetdata *obj1, w_widgetdata *obj2);
void _w_widgetdata_class_init(struct _w_widgetdata_class *clazz);
/*
 * item
 */
typedef struct _w_item {
	_w_widgetdata widgetdata;
	w_widget *parent;
	wuint index;
} _w_item;
#define _W_ITEM(x) ((_w_item*)x)
wresult _w_item_copy(w_widgetdata *from, w_widgetdata *to);
wresult _w_item_get_parent_widget(w_item *item, w_widget **parent);
wresult _w_item_get_data(w_item *item, void **data);
wresult _w_item_get_index(w_item *item);
wresult _w_item_get_text(w_item *item, w_alloc alloc, void *user_data, int enc);
wresult _w_item_set_data(w_item *item, void *data);
wresult _w_item_set_text(w_item *item, const char *text, int length, int enc);
void _w_item_class_init(struct _w_item_class *clazz);



#endif /* GTK_WIDGETS_ITEM_H_ */
