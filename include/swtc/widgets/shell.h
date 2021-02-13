/*
 * Name:        shell.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_SHELL_H_
#define SWT_WIDGETS_SHELL_H_
#include "canvas.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_button w_button;
typedef struct w_toolbar w_toolbar;
struct w_shell {
	w_canvas canvas;
	void *handle[0x30];
};
#define W_SHELL(x) ((w_shell*)x)

struct _w_shell_class {
	struct _w_canvas_class canvas;
	wresult (*close)(w_shell *shell);
	wresult (*force_active)(w_shell *shell);
	wresult (*get_default_button)(w_shell *shell,w_button** button);
	wresult (*get_alpha)(w_shell *shell);
	wresult (*get_full_screen)(w_shell *shell);
	wresult (*get_minimum_size)(w_shell *shell, w_size *size);
	wresult (*get_modified)(w_shell *shell);
	wresult (*get_images)(w_shell *shell, w_image *image, size_t length);
	wresult (*get_ime_input_mode)(w_shell *shell);
	wresult (*get_maximized)(w_shell *shell);
	wresult (*get_menu_bar)(w_shell *shell,w_menu** menu);
	wresult (*get_minimized)(w_shell *shell);
	wresult (*get_shells)(w_shell *shell, w_iterator *iterator);
	wresult (*get_text)(w_shell *shell, w_alloc alloc,void* user_data,int enc);
	wresult (*get_toolbar)(w_shell *shell,w_toolbar** toolbar);
	wresult (*open)(w_shell *shell);
	wresult (*set_active)(w_shell *shell);
	wresult (*set_alpha)(w_shell *shell, int alpha);
	wresult (*set_default_button)(w_shell *shell, w_button *button);
	wresult (*set_full_screen)(w_shell *shell, int fullScreen);
	wresult (*set_images)(w_shell *shell, w_image *image, size_t length);
	wresult (*set_ime_input_mode)(w_shell *shell, int mode);
	wresult (*set_maximized)(w_shell *shell, int maximized);
	wresult (*set_menu_bar)(w_shell *shell, w_menu *menu);
	wresult (*set_minimized)(w_shell *shell, int minimized);
	wresult (*set_minimum_size)(w_shell *shell, w_size *size);
	wresult (*set_modified)(w_shell *shell, int modified);
	wresult (*set_text)(w_shell *shell, const char *string, size_t length,int enc);
};
SWT_PUBLIC wresult w_shell_create(w_shell *shell, w_toolkit *toolkit,
		w_shell *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_shell* w_shell_new(w_toolkit *toolkit, w_shell *parent,
		wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_shell_close(w_shell *shell);
SWT_PUBLIC wresult w_shell_force_active(w_shell *shell);
SWT_PUBLIC wresult w_shell_get_default_button(w_shell *shell,w_button** button);
SWT_PUBLIC wresult w_shell_get_alpha(w_shell *shell);
SWT_PUBLIC wresult w_shell_get_full_screen(w_shell *shell);
SWT_PUBLIC wresult w_shell_get_minimum_size(w_shell *shell, w_size *size);
SWT_PUBLIC wresult w_shell_get_modified(w_shell *shell);
SWT_PUBLIC wresult w_shell_get_images(w_shell *shell, w_image *image,
		size_t length);
SWT_PUBLIC wresult w_shell_get_ime_input_mode(w_shell *shell);
SWT_PUBLIC wresult w_shell_get_maximized(w_shell *shell);
SWT_PUBLIC wresult w_shell_get_menu_bar(w_shell *shell,w_menu** menu);
SWT_PUBLIC wresult w_shell_get_minimized(w_shell *shell);
SWT_PUBLIC wresult w_shell_get_shells(w_shell *shell, w_iterator *iterator);
SWT_PUBLIC wresult w_shell_get_text(w_shell *shell, w_alloc alloc,void* user_data,int enc);
SWT_PUBLIC wresult w_shell_get_toolbar(w_shell *shell,w_toolbar** toolbar);
SWT_PUBLIC wresult w_shell_open(w_shell *shell);
SWT_PUBLIC wresult w_shell_set_active(w_shell *shell);
SWT_PUBLIC wresult w_shell_set_alpha(w_shell *shell, int alpha);
SWT_PUBLIC wresult w_shell_set_default_button(w_shell *shell, w_button *button);
SWT_PUBLIC wresult w_shell_set_full_screen(w_shell *shell, int fullScreen);
SWT_PUBLIC wresult w_shell_set_images(w_shell *shell, w_image *image,
		size_t length);
SWT_PUBLIC wresult w_shell_set_ime_input_mode(w_shell *shell, int mode);
SWT_PUBLIC wresult w_shell_set_maximized(w_shell *shell, int maximized);
SWT_PUBLIC wresult w_shell_set_menu_bar(w_shell *shell, w_menu *menu);
SWT_PUBLIC wresult w_shell_set_minimized(w_shell *shell, int minimized);
SWT_PUBLIC wresult w_shell_set_minimum_size(w_shell *shell, w_size *size);
SWT_PUBLIC wresult w_shell_set_modified(w_shell *shell, int modified);
SWT_PUBLIC wresult w_shell_set_text(w_shell *shell, const char *string,
		size_t length,int enc);

#define W_SHELL_CLASS(x) ((struct _w_shell_class*)x)
#define W_SHELL_GET_CLASS(x) ((struct _w_shell_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif

#endif /* SWT_WIDGETS_SHELL_H_ */
