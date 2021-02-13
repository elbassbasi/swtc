/*
 * app.c
 *
 *  Created on: 27/08/2020
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
w_app *swt_app = 0;
void w_app_init_thread(w_app *app);
void w_app_init_0(w_app *app) {
	if (swt_app != 0) {
		printf("warning : w_app created");
	} else {
		swt_app = app;
	}
	memset(app, 0, sizeof(w_app));
}
void w_app_init(w_app *app) {
	w_app_init_with_args(app, 0, 0);
}
void w_app_init_with_args(w_app *app, int argc, char **argv) {
	w_app_init_0(app);
	app->argc = argc;
	app->argv = argv;
	w_app_init_thread(app);
}
w_app* w_app_get() {
	return swt_app;
}
void w_app_dispose(w_app *app) {
	w_app_dispose_all();
	if (app->exepath != 0) {
		free(app->exepath);
	}
	if (app->currentdir != 0) {
		free(app->currentdir);
	}
}
const char* w_app_get_executable_file(w_app *app) {
	if (app->argv != 0)
		return app->argv[0];
	else
		return 0;
}
const char* w_app_get_executable_path(w_app *app) {
	if (app->exepath == 0) {
		if (app->argv != 0) {
			int i = 0, last_index = 0;
			while (app->argv[0][i] != 0) {
				if (app->argv[0][i] == '/' || app->argv[0][i] == '\\') {
					last_index = i;
				}
				i++;
			}
			char *exepath = malloc(last_index + 1);
			if (exepath != 0) {
				memcpy(exepath, app->argv[0], last_index);
				exepath[last_index] = 0;
				app->exepath = exepath;
			}
			return exepath;
		}
	}
	return app->exepath;
}
w_toolkit* w_app_get_default_toolkit(w_app *app) {
	if (app->defaultToolkit != 0)
		return app->defaultToolkit;
	else
		return app->systemToolkit;
}
w_toolkit* w_app_set_default_toolkit(w_app *app, w_toolkit *toolkit) {
	w_toolkit *last = app->defaultToolkit;
	if (toolkit == 0) {
		app->defaultToolkit = app->systemToolkit;
	} else {
		app->defaultToolkit = toolkit;
	}
	return last;
}
/**
 *
 */
void w_app_dispose_all() {
	w_disposable *disposable = swt_app->disposable, *next;
	while (disposable != 0) {
		next = disposable->next;
		disposable->clazz->dispose(disposable);
		disposable = next;
	}
}
void w_app_dispose_registre(w_disposable *disposable) {
	w_disposable *d = swt_app->disposable;
	while (d != 0) {
		if (d == disposable) {
			return;
		}
		d = d->next;
	}
	disposable->next = swt_app->disposable;
	swt_app->disposable = disposable;
}
void w_app_dispose_remove(w_disposable *disposable) {
	w_disposable *d = swt_app->disposable, *prev = 0;
	while (d != 0) {
		if (d == disposable) {
			if (prev == 0) {
				swt_app->disposable = d->next;
			} else {
				prev->next = d->next;
			}
			return;
		}
		prev = d;
		d = d->next;
	}
}
