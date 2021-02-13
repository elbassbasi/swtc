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
typedef struct w_app {
	w_disposable *disposable;
	int argc;
	char **argv;
	char *exepath;
	char *currentdir;
	w_toolkit *systemToolkit;
	w_toolkit *defaultToolkit;
	void *reserved[0x10];
} w_app;
SWT_PUBLIC void w_app_init(w_app *app);
SWT_PUBLIC void w_app_init_with_args(w_app *app, int argc, char **argv);
SWT_PUBLIC w_app* w_app_get();
SWT_PUBLIC void w_app_dispose(w_app *app);
SWT_PUBLIC const char* w_app_get_executable_file(w_app *app);
SWT_PUBLIC const char* w_app_get_executable_path(w_app *app);
SWT_PUBLIC const char* w_app_get_current_directory(w_app *app);
SWT_PUBLIC w_toolkit* w_app_get_platform_toolkit(w_app* app);
SWT_PUBLIC w_toolkit* w_app_get_default_toolkit(w_app* app);
SWT_PUBLIC w_toolkit* w_app_set_default_toolkit(w_app* app,w_toolkit *toolkit);

SWT_PUBLIC void w_app_dispose_all();
SWT_PUBLIC void w_app_dispose_registre(w_disposable *disposable);
SWT_PUBLIC void w_app_dispose_remove(w_disposable *disposable);

#ifdef __cplusplus
}
#endif
#endif //SWT_WIDGETS_APP_H
