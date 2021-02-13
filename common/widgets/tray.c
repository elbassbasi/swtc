/*
 * tray.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
wresult w_trayitem_get_tooltip(w_trayitem *trayitem, w_tooltip **tooltip) {
	wresult result = W_WIDGETDATA_CHECK0(trayitem);
	if (result > 0) {
		return W_TRAYITEM_GET_CLASS(trayitem)->get_tooltip(trayitem, tooltip);
	} else
		return result;
}
wresult w_trayitem_get_tooltip_text(w_trayitem *trayitem, w_alloc alloc,
		void *user_data) {
	wresult result = W_WIDGETDATA_CHECK0(trayitem);
	if (result > 0) {
		return W_TRAYITEM_GET_CLASS(trayitem)->get_tooltip_text(trayitem, alloc,
				user_data);
	} else
		return result;
}
wresult w_trayitem_get_visible(w_trayitem *trayitem) {
	wresult result = W_WIDGETDATA_CHECK0(trayitem);
	if (result > 0) {
		return W_TRAYITEM_GET_CLASS(trayitem)->get_visible(trayitem);
	} else
		return result;
}
wresult w_trayitem_set_image(w_trayitem *trayitem, w_image *image) {
	wresult result = W_WIDGETDATA_CHECK0(trayitem);
	if (result > 0) {
		return W_TRAYITEM_GET_CLASS(trayitem)->set_image(trayitem, image);
	} else
		return result;
}
wresult w_trayitem_set_tooltip(w_trayitem *trayitem, w_tooltip *toolTip) {
	wresult result = W_WIDGETDATA_CHECK0(trayitem);
	if (result > 0) {
		return W_TRAYITEM_GET_CLASS(trayitem)->set_tooltip(trayitem, toolTip);
	} else
		return result;
}
wresult w_trayitem_set_tooltip_text(w_trayitem *trayitem, const char *string) {
	wresult result = W_WIDGETDATA_CHECK0(trayitem);
	if (result > 0) {
		return W_TRAYITEM_GET_CLASS(trayitem)->set_tooltip_text(trayitem,
				string);
	} else
		return result;
}
wresult w_trayitem_set_visible(w_trayitem *trayitem, int visible) {
	wresult result = W_WIDGETDATA_CHECK0(trayitem);
	if (result > 0) {
		return W_TRAYITEM_GET_CLASS(trayitem)->set_visible(trayitem, visible);
	} else
		return result;
}
/*
 *
 */
wresult w_tray_get_item(w_tray *tray, int index, w_trayitem **item) {
	wresult result = W_WIDGET_CHECK0(tray);
	if (result > 0) {
		return W_TRAY_GET_CLASS(tray)->get_item(tray, index, item);
	} else
		return result;
}
wresult w_tray_get_item_count(w_tray *tray) {
	wresult result = W_WIDGET_CHECK0(tray);
	if (result > 0) {
		return W_TRAY_GET_CLASS(tray)->get_item_count(tray);
	} else
		return result;
}
wresult w_tray_get_items(w_tray *tray, w_iterator *items) {
	wresult result = W_WIDGET_CHECK0(tray);
	if (result > 0) {
		return W_TRAY_GET_CLASS(tray)->get_items(tray, items);
	} else
		return result;
}
