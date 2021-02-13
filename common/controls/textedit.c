/*
 * text.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_textedit_create(w_textedit *text, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(text), toolkit, W_WIDGET(parent), style,
			_W_CLASS_TEXTEDIT, post_event);
}
w_textedit* w_textedit_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_TEXTEDIT(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TEXTEDIT,post_event));
}
wresult w_textedit_append(w_textedit *text, const char *string,
		size_t length, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->append(text, string, length, enc);
	} else
		return result;
}
wresult w_textedit_clear_selection(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->clear_selection(text);
	} else
		return result;
}
wresult w_textedit_copy(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->copy(text);
	} else
		return result;
}
wresult w_textedit_cut(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->cut(text);
	} else
		return result;
}
wresult w_textedit_get_caret_line_number(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_caret_line_number(text);
	} else
		return result;
}
wresult w_textedit_get_caret_location(w_textedit *text, w_point *location) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_caret_location(text, location);
	} else
		return result;
}
wresult w_textedit_get_caret_position(w_textedit *text, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_caret_position(text, enc);
	} else
		return result;
}
wresult w_textedit_get_char_count(w_textedit *text, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_char_count(text, enc);
	} else
		return result;
}
wresult w_textedit_get_doubleclick_enabled(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_doubleclick_enabled(text);
	} else
		return result;
}
wresult w_textedit_get_echo_char(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_echo_char(text);
	} else
		return result;
}
wresult w_textedit_get_editable(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_editable(text);
	} else
		return result;
}
wresult w_textedit_get_line_count(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_line_count(text);
	} else
		return result;
}
wresult w_textedit_get_line_delimiter(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_line_delimiter(text);
	} else
		return result;
}
wresult w_textedit_get_line_height(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_line_height(text);
	} else
		return result;
}
wresult w_textedit_get_message(w_textedit *text, w_alloc alloc, void *user_data,
		int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_message(text, alloc, user_data,
				enc);
	} else
		return result;
}
wresult w_textedit_get_position(w_textedit *text, w_point *point, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_position(text, point, enc);
	} else
		return result;
}
wresult w_textedit_get_selection(w_textedit *text, w_range *range, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_selection(text, range, enc);
	} else
		return result;
}
wresult w_textedit_get_selection_count(w_textedit *text, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_selection_count(text, enc);
	} else
		return result;
}
wresult w_textedit_get_selection_text(w_textedit *text, w_alloc alloc,
		void *user_data, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_selection_text(text, alloc,
				user_data, enc);
	} else
		return result;
}
wresult w_textedit_get_tabs(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_tabs(text);
	} else
		return result;
}
wresult w_textedit_get_text(w_textedit *text, w_alloc alloc, void *user_data,
		int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_text(text, alloc, user_data, enc);
	} else
		return result;
}
wresult w_textedit_get_text_range(w_textedit *text, w_range *range,
		w_alloc alloc, void *user_data, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_text_range(text, range, alloc,
				user_data, enc);
	} else
		return result;
}
wresult w_textedit_get_text_limit(w_textedit *text, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_text_limit(text, enc);
	} else
		return result;
}
wresult w_textedit_get_top_index(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_top_index(text);
	} else
		return result;
}
wresult w_textedit_get_top_pixel(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->get_top_pixel(text);
	} else
		return result;
}
wresult w_textedit_insert(w_textedit *text, const char *string, size_t length,
		int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->insert(text, string, length, enc);
	} else
		return result;
}
wresult w_textedit_paste(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->paste(text);
	} else
		return result;
}
wresult w_textedit_select_all(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->select_all(text);
	} else
		return result;
}
wresult w_textedit_set_doubleclick_enabled(w_textedit *text, int doubleClick) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->set_doubleclick_enabled(text,
				doubleClick);
	} else
		return result;
}
wresult w_textedit_set_echochar(w_textedit *text, char echo) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->set_echochar(text, echo);
	} else
		return result;
}
wresult w_textedit_set_editable(w_textedit *text, int editable) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->set_editable(text, editable);
	} else
		return result;
}
wresult w_textedit_set_message(w_textedit *text, const char *message,
		size_t length, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->set_message(text, message, length,
				enc);
	} else
		return result;
}
wresult w_textedit_set_selection(w_textedit *text, int start, int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->set_selection(text, start, enc);
	} else
		return result;
}
wresult w_textedit_set_selection_range(w_textedit *text, w_range *selection,
		int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->set_selection_range(text, selection,
				enc);
	} else
		return result;
}
wresult w_textedit_set_tabs(w_textedit *text, int tabs) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->set_tabs(text, tabs);
	} else
		return result;
}
wresult w_textedit_set_text(w_textedit *text, const char *string, size_t length,
		int enc) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->set_text(text, string, length, enc);
	} else
		return result;
}
wresult w_textedit_set_text_limit(w_textedit *text, int limit) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->set_text_limit(text, limit);
	} else
		return result;
}
wresult w_textedit_set_top_index(w_textedit *text, int index) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->set_top_index(text, index);
	} else
		return result;
}
wresult w_textedit_show_selection(w_textedit *text) {
	wresult result = W_WIDGET_CHECK0(text);
	if (result > 0) {
		return W_TEXTEDIT_GET_CLASS(text)->show_selection(text);
	} else
		return result;
}
