/*
 * Name:        menu.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "shell.h"
#include "menu.h"
#include "toolkit.h"
/*
 * menuitem
 */
NSMenu* _w_menuitem_submenu(w_menuitem *item) {
	NSMenuItem *_i = _W_MENUITEM(item)->handle;
	if (_i == 0) {
		w_widget *_p = _W_ITEM(item)->parent;
		return (NSMenu*) _W_WIDGET(_p)->handle;
	} else {
		return NSMenuItem_submenu(_i);
	}
}
wresult _w_menuitem_copy(w_widgetdata *from, w_widgetdata *to) {
	_w_item_copy(from, to);
	return W_TRUE;
}
wresult _w_menuitem_get_data(w_item *item, void **data) {
	return W_FALSE;
}
wresult _w_menuitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_menuitem_set_data(w_item *item, void *data) {
	return W_FALSE;
}
wresult _w_menuitem_set_text(w_item *item, const char *text, int length,
		int enc) {
    NSMenuItem* nsitem = _W_MENUITEM(item)->handle;
    NSString* str = NSString_new(text, length, enc);
    if(str == 0) str = mac_toolkit->emptyString;
    NSMenuItem_setTitle(nsitem, str);
    return W_TRUE;
}
wresult _w_menuitem_insert(w_menuitem *parent, w_menuitem *item, int style,
		int index) {
	NSMenu *menu = _w_menuitem_submenu(parent);
	if (menu == 0)
		return W_ERROR_ITEM_NOT_ADDED;
	int add = W_TRUE;
	NSMenuItem *nsItem = 0;
	if ((style & W_SEPARATOR) != 0) {
		nsItem = NSMenuItem_separatorItem();
		NSObject_retain(nsItem);
	} else {
		NSString *empty = mac_toolkit->emptyString;
		nsItem = NSMenuItem_initWithTitle(empty, 0, empty);
		//NSMenuItem_setTarget(nsItem, NSOBJECT(nsItem));
		/*NSMenuItem_setAction(nsItem, aSelector);
		 nsItem.setAction(OS.sel_sendSelection);*/
		if (style & W_CASCADE) {
			NSMenu *submenu = NSMenu_initWithTitle(mac_toolkit->emptyString);
			NSMenuItem_setSubmenu(nsItem, submenu);
		}
	}
	if (add) {
        int count = NSMenu_numberOfItems(menu);
        if(index < 0 || index >= count){
            NSMenu_addItem(menu, nsItem);
            index = count;
        }else{
            NSMenu_insertItem(menu, nsItem, index);
        }
	}
	if (item != 0) {
		_W_WIDGETDATA(item)->clazz = _W_WIDGETDATA(parent)->clazz;
		_W_ITEM(item)->parent = _W_ITEM(parent)->parent;
		_W_ITEM(item)->index = index;
		_W_MENUITEM(item)->handle = nsItem;
	}
	return W_TRUE;
}
wresult _w_menuitem_get_accelerator(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_get_enabled(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_get_item(w_menuitem *item, wuint index,
		w_menuitem *sub_item) {
	return W_FALSE;
}
wresult _w_menuitem_get_item_count(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_get_items(w_menuitem *item, w_iterator *items) {
	return W_FALSE;
}
wresult _w_menuitem_get_id(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_get_image(w_menuitem *item, w_image *image) {
	return W_FALSE;
}
wresult _w_menuitem_get_parent_item(w_menuitem *item, w_menuitem *parent) {
	return W_FALSE;
}
wresult _w_menuitem_get_selection(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_get_style(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_is_enabled(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_remove(w_menuitem *item) {
	return W_FALSE;
}
wresult _w_menuitem_remove_item(w_menuitem *item, wuint index) {
	return W_FALSE;
}
wresult _w_menuitem_set_accelerator(w_menuitem *item, wuint accelerator) {
	return W_FALSE;
}
wresult _w_menuitem_set_enabled(w_menuitem *item, int enabled) {
	return W_FALSE;
}
wresult _w_menuitem_set_id(w_menuitem *item, wushort id) {
	return W_FALSE;
}
wresult _w_menuitem_set_image(w_menuitem *item, w_image *image) {
	return W_FALSE;
}
wresult _w_menuitem_set_image_index(w_menuitem *item, int index) {
	return W_FALSE;
}
wresult _w_menuitem_set_selection(w_menuitem *item, int selected) {
	return W_FALSE;
}
/*
 * menu
 */
wresult _w_menu_get_bounds(w_menu *menu, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_menu_get_imagelist(w_menu *menu, w_imagelist **imagelist) {
	return W_FALSE;
}
wresult _w_menu_get_root(w_menu *menu, w_menuitem *rootitem) {
	_W_WIDGETDATA(rootitem)->clazz = _W_MENU_GET_ITEM_CLASS(menu);
	_W_ITEM(rootitem)->parent = W_WIDGET(menu);
	_W_ITEM(rootitem)->index = -1;
	_W_MENUITEM(rootitem)->handle = 0;
	return W_TRUE;
}
wresult _w_menu_get_orientation(w_menu *menu) {
	return W_FALSE;
}
wresult _w_menu_get_parent(w_menu *menu, w_control **parent) {
	return W_FALSE;
}
wresult _w_menu_get_shell(w_menu *menu, w_shell **shell) {
	return W_FALSE;
}
wresult _w_menu_get_visible(w_menu *menu) {
	return W_FALSE;
}
wresult _w_menu_is_visible(w_menu *menu) {
	return W_FALSE;
}
wresult _w_menu_set_imagelist(w_menu *menu, w_imagelist *imagelist) {
	return W_FALSE;
}
wresult _w_menu_set_location(w_menu *menu, w_point *location) {
	return W_FALSE;
}
wresult _w_menu_set_orientation(w_menu *menu, int orientation) {
	return W_FALSE;
}
wresult _w_menu_set_visible(w_menu *menu, int visible) {
	return W_FALSE;
}
wresult _w_menu_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	NSMenu *nsMenu = NSMenu_initWithTitle(mac_toolkit->emptyString);
	NSMenu_setAutoenablesItems(nsMenu, W_FALSE);
	//NSMenu_setDelegate(nsMenu,(NSObject*) nsMenu);
	_W_WIDGET(widget)->handle = (NSView*) nsMenu;
	_W_MENU(widget)->parent = (w_control*) parent;
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	return W_TRUE;
}
wresult _w_menu_post_event(w_widget *widget, w_event *e) {
	return W_FALSE;
}
void _w_menu_class_init(struct _w_menu_class *clazz) {
	_w_widget_class_init(W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_MENU;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_menu_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_menu);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_menu);
	/*
	 * functions of menu
	 */
	W_WIDGET_CLASS(clazz)->create = _w_menu_create;
	W_WIDGET_CLASS(clazz)->post_event = _w_menu_post_event;
	clazz->get_bounds = _w_menu_get_bounds;
	clazz->get_imagelist = _w_menu_get_imagelist;
	clazz->get_root = _w_menu_get_root;
	clazz->get_orientation = _w_menu_get_orientation;
	clazz->get_parent = _w_menu_get_parent;
	clazz->get_visible = _w_menu_get_visible;
	clazz->is_visible = _w_menu_is_visible;
	clazz->set_imagelist = _w_menu_set_imagelist;
	clazz->set_location = _w_menu_set_location;
	clazz->set_orientation = _w_menu_set_orientation;
	clazz->set_visible = _w_menu_set_visible;
	/*
	 * function of menu item
	 */
	struct _w_menuitem_class *item = clazz->class_menuitem;
	_w_item_class_init(W_ITEM_CLASS(item));
	W_WIDGETDATA_CLASS(item)->copy = _w_menuitem_copy;
	W_ITEM_CLASS(item)->get_data = _w_menuitem_get_data;
	W_ITEM_CLASS(item)->get_text = _w_menuitem_get_text;
	W_ITEM_CLASS(item)->set_data = _w_menuitem_set_data;
	W_ITEM_CLASS(item)->set_text = _w_menuitem_set_text;
	item->insert = _w_menuitem_insert;
	item->get_accelerator = _w_menuitem_get_accelerator;
	item->get_enabled = _w_menuitem_get_enabled;
	item->get_item = _w_menuitem_get_item;
	item->get_item_count = _w_menuitem_get_item_count;
	item->get_items = _w_menuitem_get_items;
	item->get_id = _w_menuitem_get_id;
	item->get_image = _w_menuitem_get_image;
	item->get_parent_item = _w_menuitem_get_parent_item;
	item->get_selection = _w_menuitem_get_selection;
	item->get_style = _w_menuitem_get_style;
	item->is_enabled = _w_menuitem_is_enabled;
	item->remove = _w_menuitem_remove;
	item->remove_item = _w_menuitem_remove_item;
	item->set_accelerator = _w_menuitem_set_accelerator;
	item->set_enabled = _w_menuitem_set_enabled;
	item->set_id = _w_menuitem_set_id;
	item->set_image = _w_menuitem_set_image;
	item->set_image_index = _w_menuitem_set_image_index;
	item->set_selection = _w_menuitem_set_selection;
}
