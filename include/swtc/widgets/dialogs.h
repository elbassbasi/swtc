/*
 * Name:        dialogs.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_DIALOGS_H_
#define SWT_WIDGETS_DIALOGS_H_
#include "shell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_messagebox {
	w_shell *parent;
	wuint64 style;
	const char *title;
	const char *message;
} w_messagebox;

SWT_PUBLIC wresult w_messagebox_open(w_toolkit *toolkit,
		w_messagebox *messagebox);

typedef struct w_dialog_color {
	w_shell *parent;
	int style;
	const char *title;
	w_color selected_color;
	w_color *custom_colors;
	size_t count;
} w_dialog_color;

SWT_PUBLIC wresult w_dialog_color_open(w_toolkit *toolkit, w_dialog_color *dialog_color);

typedef struct w_dialog_font {
	w_shell *parent;
	int style;
	const char *title;
	w_color color;
	w_fontdata data;
} w_dialog_font;

wresult w_dialog_font_open(w_toolkit *toolkit, w_dialog_font *dialog_font);

typedef struct w_dialog_directory {
	w_shell *parent;
	int style;
	const char *title;
	const char *filterpath;
	const char *message;
	w_alloc *path;
} w_dialog_directory;

wresult w_dialog_directory_open(w_toolkit *toolkit,
		w_dialog_directory *dialog_directory);

typedef struct w_dialog_selected_file {
	w_alloc alloc;
	void *user_data;
	int enc;
} w_dialog_selected_file;

typedef struct w_dialog_file {
	w_shell *parent;
	int style;
	const char *title;
	const char *filename;
	const char *extensions;
	int index;
	const char *names;
	const char *path;
	w_iterator selectedfiles;
} w_dialog_file;

wresult w_dialog_file_open(w_toolkit *toolkit, w_dialog_file *dialog_file);

#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_DIALOGS_H_ */
