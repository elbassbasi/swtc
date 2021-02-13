/*
 * combobox.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc/swtc.h>
wresult w_combobox_create(w_combobox *combo, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(combo), toolkit, W_WIDGET(parent), style,
			_W_CLASS_COMBOBOX, post_event);
}
w_combobox* w_combobox_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_COMBOBOX(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_COMBOBOX,post_event));
}
wresult w_combobox_insert_item(w_combobox *combo, w_comboitem *item,
		int index) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->insert_item(combo, item, index);
	} else
		return result;
}
wresult w_combobox_clear_selection(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->clear_selection(combo);
	} else
		return result;
}
wresult w_combobox_copy(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->copy(combo);
	} else
		return result;
}
wresult w_combobox_cut(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->cut(combo);
	} else
		return result;
}
wresult w_combobox_deselect(w_combobox *combo, int index) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->deselect(combo, index);
	} else
		return result;
}
wresult w_combobox_deselect_all(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->deselect_all(combo);
	} else
		return result;
}
wresult w_combobox_get_item(w_combobox *combo, int index, w_comboitem *item) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_item(combo, index, item);
	} else
		return result;
}
wresult w_combobox_get_item_count(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_item_count(combo);
	} else
		return result;
}
wresult w_combobox_get_item_height(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_item_height(combo);
	} else
		return result;
}
wresult w_combobox_get_items(w_combobox *combo, w_iterator *items) {
	w_iterator_close(items);
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_items(combo, items);
	} else
		return result;
}
wresult w_combobox_get_list_visible(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_list_visible(combo);
	} else
		return result;
}
wresult w_combobox_set_list_visible(w_combobox *combo, int visible) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->set_list_visible(combo, visible);
	} else
		return result;
}
wresult w_combobox_get_selection(w_combobox *combo, w_point *pt) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_selection(combo, pt);
	} else
		return result;
}
wresult w_combobox_get_selection_index(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_selection_index(combo);
	} else
		return result;
}
wresult w_combobox_get_text(w_combobox *combo, w_alloc alloc, void *user_data,
		int enc) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_text(combo, alloc, user_data,
				enc);
	} else
		return result;
}
wresult w_combobox_get_text_height(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_text_height(combo);
	} else
		return result;
}
wresult w_combobox_get_text_limit(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_text_limit(combo);
	} else
		return result;
}
wresult w_combobox_get_visible_item_count(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->get_visible_item_count(combo);
	} else
		return result;
}
wresult w_combobox_indexof(w_combobox *combo, const char *string, int length,
		int enc, int start) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->indexof(combo, string, length,
				start, start);
	} else
		return result;
}
wresult w_combobox_paste(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->paste(combo);
	} else
		return result;
}
wresult w_combobox_remove_index(w_combobox *combo, int index) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->remove_index(combo, index);
	} else
		return result;
}
wresult w_combobox_remove_range(w_combobox *combo, int start, int end) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->remove_range(combo, start, end);
	} else
		return result;
}
wresult w_combobox_remove_string(w_combobox *combo, const char *string,
		int length, int enc) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->remove_string(combo, string, length,
				enc);
	} else
		return result;
}
wresult w_combobox_remove_all(w_combobox *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->remove_all(combo);
	} else
		return result;
}
wresult w_combobox_select(w_combobox *combo, int index) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->select(combo, index);
	} else
		return result;
}
wresult w_combobox_set_selection(w_combobox *combo, w_point *selection) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->set_selection(combo, selection);
	} else
		return result;
}
wresult w_combobox_set_text(w_combobox *combo, const char *text, int length,
		int enc) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->set_text(combo, text, length, enc);
	} else
		return result;
}
wresult w_combobox_set_text_limit(w_combobox *combo, int limit) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->set_text_limit(combo, limit);
	} else
		return result;
}
wresult w_combobox_set_visible_item_count(w_combobox *combo, int count) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBOBOX_GET_CLASS(combo)->set_visible_item_count(combo, count);
	} else
		return result;
}
