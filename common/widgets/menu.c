/*
 * menu.c
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
/*
 * menuitem
 */
wresult w_menuitem_insert(w_menuitem *parent, w_menuitem *item, int style,
		int index) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGETDATA_CHECK0(parent);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(parent)->insert(parent, item, style, index);
	} else {
		return result;
	}
}
wresult w_menuitem_get_accelerator(w_menuitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->get_accelerator(item);
	} else {
		return result;
	}
}
wresult w_menuitem_get_enabled(w_menuitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->get_enabled(item);
	} else {
		return result;
	}
}
wresult w_menuitem_get_item(w_menuitem *item, wuint index,
		w_menuitem *sub_item) {
	w_widgetdata_close(W_WIDGETDATA(sub_item));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->get_item(item, index, sub_item);
	} else {
		return result;
	}
}
wresult w_menuitem_get_item_count(w_menuitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->get_item_count(item);
	} else {
		return result;
	}
}
wresult w_menuitem_get_items(w_menuitem *item, w_iterator *items) {
	w_iterator_close(items);
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->get_items(item, items);
	} else {
		return result;
	}
}
wresult w_menuitem_get_id(w_menuitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->get_id(item);
	} else {
		return result;
	}
}
wresult w_menuitem_get_image(w_menuitem *item, w_image *image) {
	w_image_dispose(image);
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->get_image(item, image);
	} else {
		return result;
	}
}
wresult w_menuitem_get_parent_item(w_menuitem *item, w_menuitem *parent) {
	w_widgetdata_close(W_WIDGETDATA(parent));
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->get_parent_item(item, parent);
	} else {
		return result;
	}
}
wresult w_menuitem_get_selection(w_menuitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->get_selection(item);
	} else {
		return result;
	}
}
wresult w_menuitem_get_style(w_menuitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->get_style(item);
	} else {
		return result;
	}
}
wresult w_menuitem_is_enabled(w_menuitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->is_enabled(item);
	} else {
		return result;
	}
}
wresult w_menuitem_remove(w_menuitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->remove(item);
	} else {
		return result;
	}
}
wresult w_menuitem_remove_item(w_menuitem *item, int index) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->remove_item(item, index);
	} else {
		return result;
	}
}
wresult w_menuitem_set_accelerator(w_menuitem *item, int accelerator) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->set_accelerator(item, accelerator);
	} else {
		return result;
	}
}
wresult w_menuitem_set_enabled(w_menuitem *item, int enabled) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->set_enabled(item, enabled);
	} else {
		return result;
	}
}
wresult w_menuitem_set_id(w_menuitem *item, unsigned short id) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->set_id(item, id);
	} else {
		return result;
	}
}
wresult w_menuitem_set_image(w_menuitem *item, w_image *image) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->set_image(item, image);
	} else {
		return result;
	}
}
wresult w_menuitem_set_image_index(w_menuitem *item, int index) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->set_image_index(item, index);
	} else {
		return result;
	}
}
wresult w_menuitem_set_selection(w_menuitem *item, int selected) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_MENUITEM_GET_CLASS(item)->set_selection(item, selected);
	} else {
		return result;
	}
}
/*
 * menu
 */
wresult w_menu_create(w_menu *menu, w_control *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(menu), 0, W_WIDGET(parent), style,
			_W_CLASS_MENU, post_event);
}
w_menu* w_menu_new(w_control *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return (w_menu*) _w_widget_new(0, W_WIDGET(parent), style, _W_CLASS_MENU,
			post_event);
}
wresult w_menu_get_bounds(w_menu *menu, w_rect *bounds) {
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->get_bounds(menu, bounds);
	} else {
		memset(bounds, 0, sizeof(w_rect));
		return result;
	}
}
wresult w_menu_get_imagelist(w_menu *menu, w_imagelist **imagelist) {
	*imagelist = 0;
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->get_imagelist(menu, imagelist);
	} else {
		return result;
	}
}
wresult w_menu_get_root(w_menu *menu, w_menuitem *rootitem) {
	w_widgetdata_close(W_WIDGETDATA(rootitem));
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->get_root(menu, rootitem);
	} else {
		return result;
	}
}
wresult w_menu_get_orientation(w_menu *menu) {
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->get_orientation(menu);
	} else {
		return result;
	}
}
wresult w_menu_get_parent(w_menu *menu, w_control **parent) {
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->get_parent(menu, parent);
	} else {
		return result;
	}
}
wresult w_menu_get_shell(w_menu *menu, w_shell **shell) {
	*shell = 0;
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		w_control *parent;
		result = W_MENU_GET_CLASS(menu)->get_parent(menu, &parent);
		if (result > 0) {
			return w_control_get_shell(parent, shell);
		}
	}
	return result;
}
wresult w_menu_get_visible(w_menu *menu) {
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->get_visible(menu);
	} else {
		return result;
	}
}
wresult w_menu_is_visible(w_menu *menu) {
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->is_visible(menu);
	} else {
		return result;
	}
}
wresult w_menu_set_imagelist(w_menu *menu, w_imagelist *imagelist) {
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->set_imagelist(menu, imagelist);
	} else {
		return result;
	}
}
wresult w_menu_set_location(w_menu *menu, w_point *location) {
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->set_location(menu, location);
	} else {
		return result;
	}
}
wresult w_menu_set_orientation(w_menu *menu, int orientation) {
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->set_orientation(menu, orientation);
	} else {
		return result;
	}
}
wresult w_menu_set_visible(w_menu *menu, int visible) {
	wresult result = W_WIDGET_CHECK0(menu);
	if (result > 0) {
		return W_MENU_GET_CLASS(menu)->set_visible(menu, visible);
	} else {
		return result;
	}
}
