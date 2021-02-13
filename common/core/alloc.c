/*
 * alloc.c
 *
 *  Created on: 2 oct. 2020
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
wresult w_alloc_set_text(w_alloc alloc, void *user_data, const char *text,
		size_t length) {
	if (text == 0)
		return W_FALSE;
	size_t l = length;
	if (length == -1)
		l = strlen(text);
	char *t = 0;
	int ll = alloc(user_data, l + 1, (void**) &t);
	if (t != 0 && ll > 0) {
		memcpy(t, text, ll - 1);
		t[ll - 1] = 0;
		return W_TRUE;
	}
	return W_FALSE;
}
size_t w_alloc_buffer_new(void *user_data, size_t size, void **buffer) {
	*buffer = malloc(size);
	*((void**) user_data) = *buffer;
	if (*buffer != 0) {
		return size;
	} else {
		return 0;
	}
}
size_t w_alloc_buffer_copy(void *user_data, size_t size, void **buffer) {
	w_alloc_buffer *b = (w_alloc_buffer*) user_data;
	*buffer = b->buffer;
	b->total_size = size;
	return b->size;
}

