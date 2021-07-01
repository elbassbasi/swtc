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

/*
 *
 */
#define W_ARRAY_GROW 16
int w_array_get_count(w_array *array, void **firstElement) {
	if (array != 0) {
		if (firstElement != 0) {
			*firstElement = array->data;
		}
		return array->count;
	} else {
		if (firstElement != 0) {
			*firstElement = 0;
		}
		return 0;
	}
}
void* w_array_get(w_array *array, int index, int element_size) {
	if (array != 0 && array->count > index) {
		return &array->data[index * element_size];
	} else
		return 0;
}
int w_array_alloc(w_array **array, int newalloc, int element_size) {
	w_array *_array = *array;
	int lastalloc = 0, alloc;
	if (_array != 0) {
		lastalloc = _array->alloc;
	}
	if (lastalloc >= newalloc)
		return lastalloc;
	alloc = newalloc;
	int mod = (alloc % W_ARRAY_GROW);
	if (mod != 0) {
		alloc += W_ARRAY_GROW - mod;
	}
	_array = realloc(_array, sizeof(w_array) + alloc * element_size);
	if (_array == 0) {
		return 0;
	}
	if (*array == 0) {
		memset(_array, 0, sizeof(w_array));
	}
	_array->alloc = alloc;
	*array = _array;
	return alloc;
}
void* w_array_set(w_array **array, int index, int element_size) {
	int ret = w_array_alloc(array, index + 1, element_size);
	if (ret == 0)
		return 0;
	w_array *_array = *array;
	if (index >= _array->count) {
		void *starElement = &_array->data[_array->count * element_size];
		memset(starElement, 0, (index - _array->count + 1) * element_size);
		_array->count = index + 1;
	}
	return &_array->data[index * element_size];
}
void* w_array_add(w_array **array, int index, int element_size, int *newIndex) {
	int _newIndex;
	int ret;
	int count;
	if (*array != 0)
		count = (*array)->count;
	else
		count = 0;
	ret = w_array_alloc(array, count + 1, element_size);
	if (ret == 0)
		return 0;
	w_array *_array = *array;
	if (index >= 0 && index < count) {
		_newIndex = index;
	} else {
		_newIndex = count;
	}
	if (_newIndex < count) {
		int _count = (count - _newIndex) * element_size;
		char *d1 = &_array->data[count * element_size];
		char *d2 = &_array->data[(count - 1) * element_size];
		while (_count >= 0) {
			memcpy(d1, d2, element_size);
			d1 -= element_size;
			d2 -= element_size;
			_count--;
		}
	}
	_array->count = count + 1;
	if (*newIndex != 0)
		*newIndex = _newIndex;
	void *element = &_array->data[_newIndex * element_size];
	memset(element, 0, element_size);
	return element;
}
void w_array_remove(w_array *array, int index, int element_size,
		w_array_free_element free_element) {
	if (array != 0 && array->count > index) {
		if (free_element != 0) {
			free_element(array, &array->data[index * element_size], index);
		}
		array->count--;
		if (index < array->count) {
			int count = (array->count - index) * element_size;
			char *d1 = &array->data[index * element_size];
			char *d2 = &array->data[(index + 1) * element_size];
			memcpy(d1, d2, count);
		}
	}
}
void w_array_free(w_array *array, int element_size,
		w_array_free_element free_element) {
	if (array == 0)
		return;
	if (free_element != 0) {
		int count = array->alloc;
		char *data = array->data;
		for (int i = 0; i < count; i++) {
			free_element(array, data, i);
			data += element_size;
		}
	}
	free(array);
}
