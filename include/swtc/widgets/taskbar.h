/*
 * Name:        taskbar.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_TASKBAR_H_
#define SWT_WIDGETS_TASKBAR_H_
#include "menu.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_taskbar {
	w_widget widget;
	void *handle[0x10];
} w_taskbar;

#define W_TASKBAR(x) ((w_taskbar*)x)
typedef struct w_taskitem {
	w_widget widget;
	void *handle[6];
} w_taskitem;
#define W_TASKITEM(x) ((w_taskitem*)x)
struct _w_taskitem_class {
	struct _w_widget_class widget;
	wresult (*get_menu)(w_taskitem *item,w_menu** menu);
	wresult (*get_overlay_image)(w_taskitem *item,w_image** image);
	wresult (*get_overlay_text)(w_taskitem *item,w_alloc alloc,void* user_data);
	wresult (*get_progress)(w_taskitem *item);
	wresult (*get_progress_state)(w_taskitem *item);
	wresult (*set_menu)(w_taskitem *item, w_menu *menu);
	wresult (*set_overlay_image)(w_taskitem *item, w_image *overlayImage);
	wresult (*set_overlay_text)(w_taskitem *item, const char *overlayText);
	wresult (*set_progress)(w_taskitem *item, int progress);
	wresult (*set_progress_state)(w_taskitem *item, int progressState);
};
#define W_TASKITEM_CLASS(x) ((struct _w_taskitem_class*)x)
#define W_TASKITEM_GET_CLASS(x) ((struct _w_taskitem_class*)W_WIDGETDATA_GET_CLASS(x))
struct _w_taskbar_class {
	struct _w_widget_class widget;
	struct _w_taskitem_class *class_taskbaritem;
	wresult (*get_item)(w_taskbar *taskbar, int index, w_taskitem **item);
	wresult (*get_item_shell)(w_taskbar *taskbar, w_shell *shell,
			w_taskitem **item);
	wresult (*get_item_count)(w_taskbar *taskbar);
	wresult (*get_items)(w_taskbar *taskbar, w_iterator *items);

};
#define W_TASKBAR_CLASS(x) ((struct _w_taskbar_class*)x)
#define W_TASKBAR_GET_CLASS(x) ((struct _w_taskbar_class*)W_WIDGET_GET_CLASS(x))
SWT_PUBLIC wresult w_taskitem_get_menu(w_taskitem *item,w_menu** menu);
SWT_PUBLIC wresult w_taskitem_get_overlay_image(w_taskitem *item,w_image** image);
SWT_PUBLIC wresult w_taskitem_get_overlay_text(w_taskitem *item,w_alloc alloc,void* user_data);
SWT_PUBLIC wresult w_taskitem_get_progress(w_taskitem *item);
SWT_PUBLIC wresult w_taskitem_get_progress_state(w_taskitem *item);
SWT_PUBLIC wresult w_taskitem_set_menu(w_taskitem *item, w_menu *menu);
SWT_PUBLIC wresult w_taskitem_set_overlay_image(w_taskitem *item,
		w_image *overlayImage);
SWT_PUBLIC wresult w_taskitem_set_overlay_text(w_taskitem *item,
		const char *overlayText);
SWT_PUBLIC wresult w_taskitem_set_progress(w_taskitem *item, int progress);
SWT_PUBLIC wresult w_taskitem_set_progress_state(w_taskitem *item,
		int progressState);
/*
 *
 */
SWT_PUBLIC wresult w_taskbar_get_item(w_taskbar *taskbar, int index,
		w_taskitem **item);
SWT_PUBLIC wresult w_taskbar_get_item_shell(w_taskbar *taskbar, w_shell *shell,
		w_taskitem **item);
SWT_PUBLIC wresult w_taskbar_get_item_count(w_taskbar *taskbar);
SWT_PUBLIC wresult w_taskbar_get_items(w_taskbar *taskbar, w_iterator *items);
#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_TASKBAR_H_ */
