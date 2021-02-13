/*
 * taskbar.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
wresult w_taskitem_get_menu(w_taskitem *item, w_menu **menu) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TASKITEM_GET_CLASS(item)->get_menu(item, menu);
	} else
		return result;
}
wresult w_taskitem_get_overlay_image(w_taskitem *item, w_image **image) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TASKITEM_GET_CLASS(item)->get_overlay_image(item, image);
	} else
		return result;
}
wresult w_taskitem_get_overlay_text(w_taskitem *item, w_alloc alloc,
		void *user_data) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TASKITEM_GET_CLASS(item)->get_overlay_text(item, alloc,
				user_data);
	} else
		return result;
}
wresult w_taskitem_get_progress(w_taskitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TASKITEM_GET_CLASS(item)->get_progress(item);
	} else
		return result;
}
wresult w_taskitem_get_progress_state(w_taskitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TASKITEM_GET_CLASS(item)->get_progress_state(item);
	} else
		return result;
}
wresult w_taskitem_set_menu(w_taskitem *item, w_menu *menu) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TASKITEM_GET_CLASS(item)->set_menu(item, menu);
	} else
		return result;
}
wresult w_taskitem_set_overlay_image(w_taskitem *item, w_image *overlayImage) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TASKITEM_GET_CLASS(item)->set_overlay_image(item, overlayImage);
	} else
		return result;
}
wresult w_taskitem_set_overlay_text(w_taskitem *item, const char *overlayText) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TASKITEM_GET_CLASS(item)->set_overlay_text(item, overlayText);
	} else
		return result;
}
wresult w_taskitem_set_progress(w_taskitem *item, int progress) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TASKITEM_GET_CLASS(item)->set_progress(item, progress);
	} else
		return result;
}
wresult w_taskitem_set_progress_state(w_taskitem *item, int progressState) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TASKITEM_GET_CLASS(item)->set_progress_state(item,
				progressState);
	} else
		return result;
}
/*
 *
 */
wresult w_taskbar_get_item(w_taskbar *taskbar, int index, w_taskitem **item) {
	wresult result = W_WIDGET_CHECK0(taskbar);
	if (result > 0) {
		return W_TASKBAR_GET_CLASS(taskbar)->get_item(taskbar, index, item);
	} else
		return result;
}
wresult w_taskbar_get_item_shell(w_taskbar *taskbar, w_shell *shell,
		w_taskitem **item) {
	wresult result = W_WIDGET_CHECK0(taskbar);
	if (result > 0) {
		return W_TASKBAR_GET_CLASS(taskbar)->get_item_shell(taskbar, shell,
				item);
	} else
		return result;
}
wresult w_taskbar_get_item_count(w_taskbar *taskbar) {
	wresult result = W_WIDGET_CHECK0(taskbar);
	if (result > 0) {
		return W_TASKBAR_GET_CLASS(taskbar)->get_item_count(taskbar);
	} else
		return result;
}
wresult w_taskbar_get_items(w_taskbar *taskbar, w_iterator *items) {
	wresult result = W_WIDGET_CHECK0(taskbar);
	if (result > 0) {
		return W_TASKBAR_GET_CLASS(taskbar)->get_items(taskbar, items);
	} else
		return result;
}
