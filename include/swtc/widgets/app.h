/*
 * Name:        app.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */

#ifndef SWT_WIDGETS_APP_H
#define SWT_WIDGETS_APP_H
#include "toolkit.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _w_app {
	w_disposable *disposable;
	int argc;
	char **argv;
	w_toolkit *platformToolkit;
	w_toolkit *defaultToolkit;
	w_toolkit *customToolkit;
	w_array *themes;
} _w_app;
typedef struct w_app {
	_w_app *app;
} w_app;
typedef w_theme* (*themes_creates_function)(void *args);
typedef struct themes_function_create {
	const char *name;
	themes_creates_function create;
} themes_function_create;

SWT_PUBLIC void w_app_create(w_app *app, int argc, char **argv);
SWT_PUBLIC w_app* w_app_get();
SWT_PUBLIC void w_app_dispose(w_app *app);
SWT_PUBLIC const char* w_app_get_executable_file(w_app *app);
SWT_PUBLIC wresult w_app_get_executable_path(w_app *app, w_alloc alloc,
		void *userdata, int enc);
SWT_PUBLIC wresult w_app_get_current_directory(w_app *app, w_alloc alloc,
		void *userdata, int enc);
SWT_PUBLIC w_toolkit* w_app_get_platform_toolkit(w_app *app);
SWT_PUBLIC w_toolkit* w_app_get_default_toolkit(w_app *app);
SWT_PUBLIC w_toolkit* w_app_set_default_toolkit(w_app *app, w_toolkit *toolkit);
SWT_PUBLIC w_toolkit* w_app_get_custom_toolkit(w_app *app);
SWT_PUBLIC w_toolkit* w_app_set_custom_toolkit(w_app *app, w_toolkit *toolkit);
SWT_PUBLIC w_toolkit* w_app_load_toolkit(w_app *app, const char *module,
		const char *function);
SWT_PUBLIC wresult w_app_load_themes(w_app *app, const char *module,
		const char *function);
SWT_PUBLIC wresult w_app_registre_theme(w_app *app, const char *name,
		themes_creates_function create);
SWT_PUBLIC w_theme* w_app_create_theme(w_app *app, const char *name);

SWT_PUBLIC void w_app_dispose_all();
SWT_PUBLIC void w_app_dispose_registre(w_disposable *disposable);
SWT_PUBLIC void w_app_dispose_remove(w_disposable *disposable);

#ifdef __cplusplus
}
#endif
#endif //SWT_WIDGETS_APP_H
