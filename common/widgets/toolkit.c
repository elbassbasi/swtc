/*
 * classes.c
 *
 *  Created on: 29 avr. 2020
 *      Author: Azeddine El Bassbasi
 */
#include <swtc.h>
w_toolkit *w_toolkit_default = 0;
w_toolkit* w_toolkit_get_default() {
	return w_app_get_default_toolkit(w_app_get());
}
wresult w_toolkit_set_default(w_toolkit *toolkit) {
	w_app_set_default_toolkit(w_app_get(), toolkit);
	return W_TRUE;
}
struct _w_widget_class* w_toolkit_get_class(w_toolkit *toolkit,
		wushort clazz_id) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_class(toolkit, clazz_id);
}
void w_toolkit_init_class(w_toolkit *toolkit, wushort clazz_id,
		struct _w_widget_class *clazz) {
	if (toolkit == 0)
		return;
	W_TOOLKIT_GET_CLASS(toolkit)->init_class(toolkit, clazz_id, clazz);
}
wresult w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->async_exec(toolkit, function, args);
}
wresult w_toolkit_beep(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->beep(toolkit);
}
w_shell* w_toolkit_get_active_shell(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_active_shell(toolkit);
}
w_menu* w_toolkit_get_menubar(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_menubar(toolkit);
}
wresult w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *bounds) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_bounds(toolkit, bounds);
}
wresult w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_client_area(toolkit, clientArea);
}
w_control* w_toolkit_get_cursor_control(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_cursor_control(toolkit);
}
wresult w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_cursor_location(toolkit, location);
}
size_t w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_cursor_sizes(toolkit, sizes,
			length);
}
int w_toolkit_get_dismissal_alignment(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_dismissal_alignment(toolkit);
}
int w_toolkit_get_double_click_time(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_double_click_time(toolkit);
}
w_control* w_toolkit_get_focus_control(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_focus_control(toolkit);
}
wresult w_toolkit_get_high_contrast(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_high_contrast(toolkit);
}
int w_toolkit_get_icon_depth(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_icon_depth(toolkit);
}
size_t w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_icon_sizes(toolkit, sizes, length);
}
void w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	w_iterator_close(shells);
	return W_TOOLKIT_GET_CLASS(toolkit)->get_shells(toolkit, shells);
}
w_color w_toolkit_get_system_color(w_toolkit *toolkit, wuint id) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_color(toolkit, id);
}
w_cursor* w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint id) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_cursor(toolkit, id);
}
w_font* w_toolkit_get_system_font(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_font(toolkit);
}
wresult w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
		w_image **image) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_image(toolkit, id, image);
}
w_menu* w_toolkit_get_system_menu(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_menu(toolkit);
}
struct w_taskbar* w_toolkit_get_system_taskbar(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_taskbar(toolkit);
}
struct w_tray* w_toolkit_get_system_tray(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_system_tray(toolkit);
}
w_theme* w_toolkit_get_theme(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_theme(toolkit);
}
w_thread* w_toolkit_get_thread(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_thread(toolkit);
}
wuint64 w_toolkit_get_thread_id(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_thread_id(toolkit);
}
wresult w_toolkit_get_touch_enabled(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->get_touch_enabled(toolkit);
}
wresult w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
		w_point *result, w_point *point) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->map_0(toolkit, from, to, result, point);
}
wresult w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
		w_rect *result, w_rect *rectangle) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->map_1(toolkit, from, to, result,
			rectangle);
}
wresult w_toolkit_post(w_toolkit *toolkit, w_event *event) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->post(toolkit, event);
}
wresult w_toolkit_post_quit(w_toolkit *toolkit, int quit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->post_quit(toolkit, quit);
}
wresult w_toolkit_read_and_dispatch(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->dispatch(toolkit);
}
wresult w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *point) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->set_cursor_location(toolkit, point);
}
wresult w_toolkit_set_theme(w_toolkit *toolkit, w_theme *theme) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->set_theme(toolkit, theme);
}
int w_toolkit_run(w_toolkit *toolkit, w_shell *shell) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->run(toolkit, shell);
}
wresult w_toolkit_sleep(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->read(toolkit);
}
wresult w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->sync_exec(toolkit, function, args);
}
wresult w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->timer_exec(toolkit, milliseconds,
			function, args);
}
wresult w_toolkit_update(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->update(toolkit);
}
wresult w_toolkit_wake(w_toolkit *toolkit) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->wake(toolkit);
}
/*
 *
 */
int w_toolkit_register_type(w_toolkit *toolkit, const char *formatName) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->register_type(toolkit, formatName);
}
wresult w_toolkit_open_clipboard(w_toolkit *toolkit, w_clipboard *clipboard,
		int clipboards) {
	if (toolkit == 0)
		toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->open_clipboard(toolkit, clipboard,
			clipboards);
}

/*
 * init classes
 */
void _w_toolkit_classes_init(struct _w_toolkit_classes *classes) {
	/*  shell */
	W_SCROLLABLE_CLASS(&classes->class_shell)->class_scrollbar =
			&classes->class_scrollbar;
	classes->classes[_W_CLASS_SHELL] = W_WIDGET_CLASS(&classes->class_shell);
	/*  canvas */
	W_SCROLLABLE_CLASS(&classes->class_canvas)->class_scrollbar =
			&classes->class_scrollbar;
	classes->classes[_W_CLASS_CANVAS] = W_WIDGET_CLASS(&classes->class_canvas);
	classes->classes[_W_CLASS_CCANVAS] = W_WIDGET_CLASS(
			&classes->class_ccanvas);
	/* composite */
	W_SCROLLABLE_CLASS(&classes->class_composite)->class_scrollbar =
			&classes->class_scrollbar;
	classes->classes[_W_CLASS_COMPOSITE] = W_WIDGET_CLASS(
			&classes->class_composite);
	/*  menu  */
	W_MENU_CLASS(&classes->class_menu)->class_menuitem =
			&classes->class_menuitem;
	classes->classes[_W_CLASS_MENU] = W_WIDGET_CLASS(&classes->class_menu);
	/*  treeview */
	W_SCROLLABLE_CLASS(&classes->class_treeview)->class_scrollbar =
			&classes->class_scrollbar;
	W_LISTVIEWBASE_CLASS(&classes->class_treeview)->class_item = W_ITEM_CLASS(
			&classes->class_treeitem);
	W_LISTVIEWBASE_CLASS(&classes->class_treeview)->class_column =
			&classes->class_treecolumn;
	classes->classes[_W_CLASS_TREEVIEW] = W_WIDGET_CLASS(
			&classes->class_treeview);
	/* listview */
	W_SCROLLABLE_CLASS(&classes->class_listview)->class_scrollbar =
			&classes->class_scrollbar;
	W_LISTVIEWBASE_CLASS(&classes->class_listview)->class_item = W_ITEM_CLASS(
			&classes->class_listitem);
	W_LISTVIEWBASE_CLASS(&classes->class_listview)->class_column =
			&classes->class_listcolumn;
	classes->classes[_W_CLASS_LISTVIEW] = W_WIDGET_CLASS(
			&classes->class_listview);
	/*  sash */
	classes->classes[_W_CLASS_SASH] = W_WIDGET_CLASS(&classes->class_sash);
	/* button */
	classes->classes[_W_CLASS_BUTTON] = W_WIDGET_CLASS(&classes->class_button);
	/* label */
	classes->classes[_W_CLASS_LABEL] = W_WIDGET_CLASS(&classes->class_label);
	/* textedit */
	W_SCROLLABLE_CLASS(&classes->class_textedit)->class_scrollbar =
			&classes->class_scrollbar;
	classes->classes[_W_CLASS_TEXTEDIT] = W_WIDGET_CLASS(
			&classes->class_textedit);
	/* progressbar */
	classes->classes[_W_CLASS_PROGRESSBAR] = W_WIDGET_CLASS(
			&classes->class_progressbar);
	/* groupbox */
	W_SCROLLABLE_CLASS(&classes->class_groupbox)->class_scrollbar =
			&classes->class_scrollbar;
	classes->classes[_W_CLASS_GROUPBOX] = W_WIDGET_CLASS(
			&classes->class_groupbox);
	/* combobox */
	W_SCROLLABLE_CLASS(&classes->class_combobox)->class_scrollbar =
			&classes->class_scrollbar;
	W_COMBOBOX_CLASS(&classes->class_combobox)->class_comboitem =
			&classes->class_comboitem;
	classes->classes[_W_CLASS_COMBOBOX] = W_WIDGET_CLASS(
			&classes->class_combobox);
	/* coolbar */
	W_SCROLLABLE_CLASS(&classes->class_coolbar)->class_scrollbar =
			&classes->class_scrollbar;
	W_COOLBAR_CLASS(&classes->class_coolbar)->class_coolitem =
			&classes->class_coolitem;
	classes->classes[_W_CLASS_COOLBAR] = W_WIDGET_CLASS(
			&classes->class_coolbar);
	/* datetime */
	W_SCROLLABLE_CLASS(&classes->class_datetime)->class_scrollbar =
			&classes->class_scrollbar;
	classes->classes[_W_CLASS_DATETIME] = W_WIDGET_CLASS(
			&classes->class_datetime);
	/* expandbar */
	W_SCROLLABLE_CLASS(&classes->class_expandbar)->class_scrollbar =
			&classes->class_scrollbar;
	W_EXPANDBAR_CLASS(&classes->class_expandbar)->class_expanditem =
			&classes->class_expanditem;
	classes->classes[_W_CLASS_EXPANDBAR] = W_WIDGET_CLASS(
			&classes->class_expandbar);
	/* slider */
	classes->classes[_W_CLASS_SLIDER] = W_WIDGET_CLASS(&classes->class_slider);
	/* spinner */
	W_SCROLLABLE_CLASS(&classes->class_spinner)->class_scrollbar =
			&classes->class_scrollbar;
	classes->classes[_W_CLASS_SPINNER] = W_WIDGET_CLASS(
			&classes->class_spinner);
	/* tabview */
	W_SCROLLABLE_CLASS(&classes->class_tabview)->class_scrollbar =
			&classes->class_scrollbar;
	W_TABVIEW_CLASS(&classes->class_tabview)->class_tabitem =
			&classes->class_tabitem;
	classes->classes[_W_CLASS_TABVIEW] = W_WIDGET_CLASS(
			&classes->class_tabview);
	/* toolbar */
	W_SCROLLABLE_CLASS(&classes->class_toolbar)->class_scrollbar =
			&classes->class_scrollbar;
	W_TOOLBAR_CLASS(&classes->class_toolbar)->class_toolitem =
			&classes->class_toolitem;
	classes->classes[_W_CLASS_TOOLBAR] = W_WIDGET_CLASS(
			&classes->class_toolbar);
	/* tray */
	W_TRAY_CLASS(&classes->class_tray)->class_trayitem =
			&classes->class_trayitem;
	classes->classes[_W_CLASS_TRAY] = W_WIDGET_CLASS(&classes->class_tray);
	classes->classes[_W_CLASS_TRAYITEM] = W_WIDGET_CLASS(
			&classes->class_trayitem);
	/* tooltip */
	classes->classes[_W_CLASS_TOOLTIP] = W_WIDGET_CLASS(
			&classes->class_tooltip);
}
const char* w_app_get_executable_file(w_app *app) {
	if (app->app->argv != 0)
		return app->app->argv[0];
	else
		return 0;
}
wresult w_app_get_executable_path(w_app *app, w_alloc alloc, void *userdata,
		int enc) {
	char *file = app->app->argv[0];
	int i = 0, last_index = 0;
	while (file[i] != 0) {
		if (file[i] == '/' || file[i] == '\\') {
			last_index = i;
		}
		i++;
	}
	return w_alloc_set_text(alloc, userdata, enc, app->app->argv[0], last_index,
			W_ENCODING_UTF8);
}
w_toolkit* w_app_get_default_toolkit(w_app *app) {
	if (app->app->defaultToolkit != 0)
		return app->app->defaultToolkit;
	else
		return app->app->platformToolkit;
}
w_toolkit* w_app_set_default_toolkit(w_app *app, w_toolkit *toolkit) {
	w_toolkit *last = app->app->defaultToolkit;
	if (toolkit == 0) {
		app->app->defaultToolkit = app->app->platformToolkit;
	} else {
		app->app->defaultToolkit = toolkit;
	}
	return last;
}
w_toolkit* w_toolkit_get_custom();
wresult w_toolkit_registre_themes();
w_toolkit* w_app_get_custom_toolkit(w_app *app) {
	if (app->app->customToolkit == 0) {
		app->app->customToolkit = w_toolkit_get_custom();
	}
	return app->app->customToolkit;
}
w_toolkit* w_app_set_custom_toolkit(w_app *app, w_toolkit *toolkit) {
	w_toolkit *last = app->app->customToolkit;
	app->app->customToolkit = toolkit;
	return last;
}
w_toolkit* w_app_load_toolkit(w_app *app, const char *module,
		const char *function) {
	return 0;
}
wresult w_app_load_themes(w_app *app, const char *module,
		const char *function) {

}
wresult w_app_registre_theme(w_app *app, const char *name,
		themes_creates_function create) {
	int newIndex;
	themes_function_create *fn = w_array_add(&w_app_get()->app->themes, -1,
			sizeof(themes_function_create), &newIndex);
	if (fn != 0) {
		fn->name = strdup(name);
		fn->create = create;
		return W_TRUE;
	}
	return W_FALSE;
}
w_theme* w_app_create_theme(w_app *app, const char *name) {
	if (w_app_get()->app->themes == 0) {
		w_toolkit_registre_themes();
	}
	themes_function_create *fns;
	int count = w_array_get_count(w_app_get()->app->themes, (void**) &fns);
	for (int i = 0; i < count; i++) {
		if (!strcmp(fns[i].name, name)) {
			return fns[i].create(&fns[i]);
		}

	}
	return 0;
}
/**
 *
 */
void w_app_dispose_all() {
	w_app *app = w_app_get();
	w_disposable *disposable = app->app->disposable, *next;
	while (disposable != 0) {
		next = disposable->next;
		disposable->clazz->dispose(disposable);
		disposable = next;
	}
}
void w_app_dispose_registre(w_disposable *disposable) {
	w_app *app = w_app_get();
	w_disposable *d = app->app->disposable;
	while (d != 0) {
		if (d == disposable) {
			return;
		}
		d = d->next;
	}
	disposable->next = app->app->disposable;
	app->app->disposable = disposable;
}
void w_app_dispose_remove(w_disposable *disposable) {
	w_app *app = w_app_get();
	w_disposable *d = app->app->disposable, *prev = 0;
	while (d != 0) {
		if (d == disposable) {
			if (prev == 0) {
				app->app->disposable = d->next;
			} else {
				prev->next = d->next;
			}
			return;
		}
		prev = d;
		d = d->next;
	}
}
