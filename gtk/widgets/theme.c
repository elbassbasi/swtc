/*
 * Name:        theme.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
#if GTK3
const char *gtk_tooltip_name = "gtk-tooltip";
#endif
#if GTK2
	const char* gtk_tooltip_name = "gtk-tooltips";
#endif
void _w_theme_init_widget(_gtk_theme *theme) {
	theme->handle[GTK_THEME_HANDLE_SHELL] = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	theme->handle[GTK_THEME_HANDLE_TOOLTIP] = gtk_window_new(GTK_WINDOW_POPUP);
	gtk_widget_set_name(theme->handle[GTK_THEME_HANDLE_TOOLTIP],
			gtk_tooltip_name);

}
void _w_theme_init() {
	_w_theme_init_widget(&gtk_toolkit->gtktheme);
}

