/*
 * Name:        menu.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_MENU_H_
#define SWT_WIDGETS_MENU_H_
#include "widget.h"
#ifdef __cplusplus
extern "C" {
#endif
struct w_menu {
	w_widget widget;
	void *handle[0x10];
};
typedef struct w_menuitem {
	w_widgetdata widgetdata;
	void *handle[6];
} w_menuitem;
typedef struct w_event_menu {
	w_event event;
	w_menuitem *item;
} w_event_menu;
struct _w_menuitem_class {
	struct _w_item_class item;
	wresult (*for_all_children)(w_menuitem *parent, w_widget_callback callback,
			void *user_data, int flags);
	wresult (*get_accelerator)(w_menuitem *item);
	wresult (*get_enabled)(w_menuitem *item);
	wresult (*get_item)(w_menuitem *item, wuint index, w_menuitem *sub_item);
	wresult (*get_item_count)(w_menuitem *item);
	wresult (*get_items)(w_menuitem *item, w_iterator *items);
	wresult (*get_id)(w_menuitem *item, int mask);
	wresult (*get_image)(w_menuitem *item, w_image *image);
	wresult (*get_parent_item)(w_menuitem *item, w_menuitem *parent);
	wresult (*get_selection)(w_menuitem *item);
	wresult (*get_style)(w_menuitem *item);
	wresult (*insert)(w_menuitem *parent, w_menuitem *item, int style,
			int index);
	wresult (*is_enabled)(w_menuitem *item);
	wresult (*remove)(w_menuitem *item);
	wresult (*remove_item)(w_menuitem *item, wuint index);
	wresult (*set_accelerator)(w_menuitem *item, wuint accelerator);
	wresult (*set_enabled)(w_menuitem *item, int enabled);
	wresult (*set_id)(w_menuitem *item, int mask, wushort id);
	wresult (*set_image)(w_menuitem *item, w_image *image);
	wresult (*set_image_index)(w_menuitem *item, w_imagelist *imagelist,
			int index);
	wresult (*set_selection)(w_menuitem *item, int selected);
};
struct _w_menu_class {
	struct _w_widget_class widget;
	struct _w_menuitem_class *class_menuitem;
	wresult (*get_bounds)(w_menu *menu, w_rect *bounds);
	wresult (*get_root)(w_menu *menu, w_menuitem *rootitem);
	wresult (*get_orientation)(w_menu *menu);
	wresult (*get_parent)(w_menu *menu, w_control **parent);
	wresult (*get_visible)(w_menu *menu);
	wresult (*is_visible)(w_menu *menu);
	wresult (*set_id_mask)(w_menu *menu, int mask);
	wresult (*set_location)(w_menu *menu, w_point *location);
	wresult (*set_orientation)(w_menu *menu, int orientation);
	wresult (*set_visible)(w_menu *menu, int visible);
};
/*
 * menuitem
 */
SWT_PUBLIC wresult w_menuitem_for_all_children(w_menuitem *parent,
		w_widget_callback callback, void *user_data, int flags);
SWT_PUBLIC wresult w_menuitem_insert(w_menuitem *parent, w_menuitem *item,
		int style, int index);
SWT_PUBLIC wresult w_menuitem_get_accelerator(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_get_enabled(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_get_item(w_menuitem *item, wuint index,
		w_menuitem *sub_item);
SWT_PUBLIC wresult w_menuitem_get_item_count(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_get_items(w_menuitem *item, w_iterator *items);
SWT_PUBLIC wresult w_menuitem_get_id(w_menuitem *item, int mask);
SWT_PUBLIC wresult w_menuitem_get_image(w_menuitem *item, w_image *image);
SWT_PUBLIC wresult w_menuitem_get_parent_item(w_menuitem *item,
		w_menuitem *parent);
SWT_PUBLIC wresult w_menuitem_get_selection(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_get_style(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_is_enabled(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_remove(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_remove_item(w_menuitem *item, int index);
SWT_PUBLIC wresult w_menuitem_set_accelerator(w_menuitem *item,
		int accelerator);
SWT_PUBLIC wresult w_menuitem_set_enabled(w_menuitem *item, int enabled);
SWT_PUBLIC wresult w_menuitem_set_id(w_menuitem *item, int mask, wushort id);
SWT_PUBLIC wresult w_menuitem_set_image(w_menuitem *item, w_image *image);
SWT_PUBLIC wresult w_menuitem_set_image_index(w_menuitem *item,
		w_imagelist *imagelist, int index);
SWT_PUBLIC wresult w_menuitem_set_selection(w_menuitem *item, int selected);
/*
 * menu
 */
SWT_PUBLIC wresult w_menu_create(w_menu *menu, w_control *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_menu* w_menu_new(struct w_control *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_menu_get_bounds(w_menu *menu, w_rect *bounds);
SWT_PUBLIC wresult w_menu_get_root(w_menu *menu, w_menuitem *rootitem);
SWT_PUBLIC wresult w_menu_get_orientation(w_menu *menu);
SWT_PUBLIC wresult w_menu_get_parent(w_menu *menu, w_control **parent);
SWT_PUBLIC wresult w_menu_get_visible(w_menu *menu);
SWT_PUBLIC wresult w_menu_is_visible(w_menu *menu);
SWT_PUBLIC wresult w_menu_set_id_mask(w_menu *menu, int mask);
SWT_PUBLIC wresult w_menu_set_location(w_menu *menu, w_point *location);
SWT_PUBLIC wresult w_menu_set_orientation(w_menu *menu, int orientation);
SWT_PUBLIC wresult w_menu_set_visible(w_menu *menu, int visible);

#define W_MENU(x) ((w_menu*)x)
#define W_MENU_CLASS(x) ((struct _w_menu_class*)x)
#define W_MENU_GET_CLASS(x) ((struct _w_menu_class*)W_WIDGET_GET_CLASS(x))
#define _W_MENU_GET_ITEM_CLASS(menu) (W_WIDGETDATA_CLASS(W_MENU_GET_CLASS(menu)->class_menuitem))

#define W_MENUITEM(x) ((w_menuitem*)x)
#define W_MENUITEM_CLASS(x) ((struct _w_menuitem_class*)x)
#define W_MENUITEM_GET_CLASS(x) ((struct _w_menuitem_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif

#endif /* SWT_WIDGETS_MENU_H_ */
