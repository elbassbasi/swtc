/*
 * Name:        text.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_TEXT_H_
#define SWTC_CONTROLS_TEXT_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_textedit {
	w_scrollable scrollable;
	void *handle[8];
} w_textedit;
#define W_TEXTEDIT(x) ((w_textedit*)x)
#define W_TEXTEDIT_CLASS(x) ((struct _w_textedit_class*)x)
#define W_TEXTEDIT_GET_CLASS(x) ((struct _w_textedit_class*)W_WIDGET_GET_CLASS(x))
struct _w_textedit_class {
	struct _w_scrollable_class scrollable;
	wresult (*append)(w_textedit *text, const char *string, size_t length,int enc);
	wresult (*clear_selection)(w_textedit *text);
	wresult (*copy)(w_textedit *text);
	wresult (*cut)(w_textedit *text);
	wresult (*get_caret_line_number)(w_textedit *text);
	wresult (*get_caret_location)(w_textedit *text, w_point *location);
	wresult (*get_caret_position)(w_textedit *text,int enc);
	wresult (*get_char_count)(w_textedit *text,int enc);
	wresult (*get_doubleclick_enabled)(w_textedit *text);
	wresult (*get_echo_char)(w_textedit *text);
	wresult (*get_editable)(w_textedit *text);
	wresult (*get_line_count)(w_textedit *text);
	wresult (*get_line_delimiter)(w_textedit *text);
	wresult (*get_line_height)(w_textedit *text);
	wresult (*get_message)(w_textedit *text, w_alloc alloc, void *user_data,int enc);
	wresult (*get_position)(w_textedit *text, w_point *point,int enc);
	wresult (*get_selection)(w_textedit *text, w_range *range,int enc);
	wresult (*get_selection_count)(w_textedit *text,int enc);
	wresult (*get_selection_text)(w_textedit *text, w_alloc alloc, void *user_data,int enc);
	wresult (*get_tabs)(w_textedit *text);
	wresult (*get_text)(w_textedit *text, w_alloc alloc, void *user_data,int enc);
	wresult (*get_text_range)(w_textedit *text, w_range *range, w_alloc alloc,
			void *user_data,int enc);
	wresult (*get_text_limit)(w_textedit *text,int enc);
	wresult (*get_top_index)(w_textedit *text);
	wresult (*get_top_pixel)(w_textedit *text);
	wresult (*insert)(w_textedit *text, const char *string, size_t length,int enc);
	wresult (*paste)(w_textedit *text);
	wresult (*select_all)(w_textedit *text);
	wresult (*set_doubleclick_enabled)(w_textedit *text, int doubleClick);
	wresult (*set_echochar)(w_textedit *text, int echo);
	wresult (*set_editable)(w_textedit *text, int editable);
	wresult (*set_message)(w_textedit *text, const char *message, size_t length,int enc);
	wresult (*set_selection)(w_textedit *text, int start,int enc);
	wresult (*set_selection_range)(w_textedit *text, w_range *selection,int enc);
	wresult (*set_tabs)(w_textedit *text, int tabs);
	wresult (*set_text)(w_textedit *text, const char *string, size_t length,int enc);
	wresult (*set_text_limit)(w_textedit *text, int limit);
	wresult (*set_top_index)(w_textedit *text, int index);
	wresult (*show_selection)(w_textedit *text);
};
SWT_PUBLIC wresult w_textedit_create(w_textedit *text, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_textedit* w_textedit_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_textedit_append(w_textedit *text, const char*, size_t length,int enc);
SWT_PUBLIC wresult w_textedit_clear_selection(w_textedit *text);
SWT_PUBLIC wresult w_textedit_copy(w_textedit *text);
SWT_PUBLIC wresult w_textedit_cut(w_textedit *text);
SWT_PUBLIC wresult w_textedit_get_caret_line_number(w_textedit *text);
SWT_PUBLIC wresult w_textedit_get_caret_location(w_textedit *text, w_point *location);
SWT_PUBLIC wresult w_textedit_get_caret_position(w_textedit *text,int enc);
SWT_PUBLIC wresult w_textedit_get_char_count(w_textedit *text,int enc);
SWT_PUBLIC wresult w_textedit_get_doubleclick_enabled(w_textedit *text);
SWT_PUBLIC wresult w_textedit_get_echo_char(w_textedit *text);
SWT_PUBLIC wresult w_textedit_get_editable(w_textedit *text);
SWT_PUBLIC wresult w_textedit_get_line_count(w_textedit *text);
SWT_PUBLIC wresult w_textedit_get_line_delimiter(w_textedit *text);
SWT_PUBLIC wresult w_textedit_get_line_height(w_textedit *text);
SWT_PUBLIC wresult w_textedit_get_message(w_textedit *text, w_alloc alloc,
		void *user_data,int enc);
SWT_PUBLIC wresult w_textedit_get_position(w_textedit *text, w_point *point,int enc);
SWT_PUBLIC wresult w_textedit_get_selection(w_textedit *text, w_range *range,int enc);
SWT_PUBLIC wresult w_textedit_get_selection_count(w_textedit *text,int enc);
SWT_PUBLIC wresult w_textedit_get_selection_text(w_textedit *text, w_alloc alloc,
		void *user_data,int enc);
SWT_PUBLIC wresult w_textedit_get_tabs(w_textedit *text);
SWT_PUBLIC wresult w_textedit_get_text(w_textedit *text, w_alloc alloc,
		void *user_data,int enc);
SWT_PUBLIC wresult w_textedit_get_text_range(w_textedit *text, w_range *range,
		w_alloc alloc, void *user_data,int enc);
SWT_PUBLIC wresult w_textedit_get_text_limit(w_textedit *text,int enc);
SWT_PUBLIC wresult w_textedit_get_top_index(w_textedit *text);
SWT_PUBLIC wresult w_textedit_get_top_pixel(w_textedit *text);
SWT_PUBLIC wresult w_textedit_insert(w_textedit *text, const char *string,
		size_t length,int enc);
SWT_PUBLIC wresult w_textedit_paste(w_textedit *text);
SWT_PUBLIC wresult w_textedit_select_all(w_textedit *text);
SWT_PUBLIC wresult w_textedit_set_doubleclick_enabled(w_textedit *text,
		int doubleClick);
SWT_PUBLIC wresult w_textedit_set_echochar(w_textedit *text, char echo);
SWT_PUBLIC wresult w_textedit_set_editable(w_textedit *text, int editable);
SWT_PUBLIC wresult w_textedit_set_message(w_textedit *text, const char *message,
		size_t length,int enc);
SWT_PUBLIC wresult w_textedit_set_selection(w_textedit *text, int start,int enc);
SWT_PUBLIC wresult w_textedit_set_selection_range(w_textedit *text, w_range *selection,int enc);
SWT_PUBLIC wresult w_textedit_set_tabs(w_textedit *text, int tabs);
SWT_PUBLIC wresult w_textedit_set_text(w_textedit *text, const char *string,
		size_t length,int enc);
SWT_PUBLIC wresult w_textedit_set_text_limit(w_textedit *text, int limit);
SWT_PUBLIC wresult w_textedit_set_top_index(w_textedit *text, int index);
SWT_PUBLIC wresult w_textedit_show_selection(w_textedit *text);
#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_TEXT_H_ */
