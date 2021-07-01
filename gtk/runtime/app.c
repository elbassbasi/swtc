
/*
 * Name:        app.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include <swtc/swtc.h>
#include <unistd.h>
w_toolkit* w_toolkit_get_platform(w_app *app);
void w_app_init_thread(w_app *app) {
	app->platformToolkit = w_toolkit_get_platform(app);
}
w_toolkit* w_app_get_platform_toolkit(w_app* app){
	return app->platformToolkit;
}
const char* w_app_get_current_directory(w_app *app) {
	if (app->currentdir == 0) {
		app->currentdir = getcwd(NULL, 0);
	}
	return app->currentdir;
}
