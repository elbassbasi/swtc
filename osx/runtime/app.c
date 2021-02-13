/*
 * app.c
 *
 *  Created on: 20 oct. 2020
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
#include <unistd.h>
w_toolkit* w_toolkit_get_platform(w_app *app);
void w_app_init_thread(w_app *app) {
	app->systemToolkit = w_toolkit_get_platform(app);
}
w_toolkit* w_app_get_platform_toolkit(w_app* app){
	return app->systemToolkit;
}
const char* w_app_get_current_directory(w_app *app) {
	if (app->currentdir == 0) {
		app->currentdir = getcwd(NULL, 0);
	}
	return app->currentdir;
}
