/*
 * alloc.c
 *
 *  Created on: 2 oct. 2020
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
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

