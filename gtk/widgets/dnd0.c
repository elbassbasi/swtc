/*
 * Name:        dnd.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
void w_clipboard_init(w_clipboard *clipboard) {
}
wresult w_clipboard_clear_contents(w_clipboard *clipboard) {
	return W_FALSE;
}
wresult w_clipboard_close(w_clipboard *clipboard) {
	return W_FALSE;
}
wresult w_clipboard_is_ok(w_clipboard *clipboard) {
	return W_FALSE;
}
wresult w_clipboard_get_available_types(w_clipboard *clipboard,
		w_iterator *types) {
	return W_FALSE;
}
wresult w_clipboard_get_contents(w_clipboard *clipboard, w_transfer *transfer,
		w_alloc alloc, void *user_data) {
	return W_FALSE;
}
wresult w_clipboard_set_contents(w_clipboard *clipboard, w_transfers *transfers,
		size_t length) {
	return W_FALSE;
}
