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
/*
 * link
 */
void w_link_linkfirst_0(w_link_0 *e, void *e_p, void **first) {
	void *_first = *first;
	e->next = _first;
	e->prev = 0;
	*first = e_p;
	if (_first == 0) {
		e->prev = e_p;
	} else {
		const wintptr diff = (wintptr) e - (wintptr) e_p;
		w_link_0 *_f = (w_link_0*) (_first + diff);
		_f->prev = e_p;
	}
}
void w_link_linkfirst(w_link *e, w_link **first) {
	w_link_linkfirst_0((w_link_0*) e, e, (void**) first);
}
void w_link_linklast_0(w_link_0 *e, void *e_p, void **first) {
	void *_first = *first;
	if (_first == 0) {
		*first = e_p;
		e->next = 0;
		e->prev = e_p;
	} else {
		const wintptr diff = (wintptr) e - (wintptr) e_p;
		w_link_0 *_f = (w_link_0*) (_first + diff);
		void *l = _f->prev;
		w_link_0 *_l = (w_link_0*) (l + diff);
		e->next = 0;
		e->prev = l;
		_l->next = e_p;
		_f->prev = e_p;
	}
}
void w_link_linklast(w_link *e, w_link **first) {
	w_link_linklast_0((w_link_0*) e, e, (void**) first);
}
void w_link_linkbefore_0(w_link_0 *e, void *e_p, void *succ, void **first) {
	const wintptr diff = (wintptr) e - (wintptr) e_p;
	w_link_0 *_succ = (w_link_0*) (succ + diff);
	void *pred = _succ->prev;
	e->next = succ;
	e->prev = pred;
	_succ->prev = e_p;
	if (pred == 0) {
		*first = e_p;
	} else {
		w_link_0 *_pred = (w_link_0*) (pred + diff);
		_pred->next = e_p;
	}
}
void w_link_linkbefore(w_link *e, w_link *succ, w_link **first) {
	w_link_linkbefore_0((w_link_0*) e, e, succ, (void**) first);
}
void w_link_linkafter_0(w_link_0 *e, void *e_p, void *succ, void **first) {
	const wintptr diff = (wintptr) e - (wintptr) e_p;
	w_link_0 *_succ = (w_link_0*) (succ + diff);
	void *next = _succ->next;
	e->next = next;
	e->prev = succ;
	_succ->next = e;
	if (next == 0) {
		void *_first = *first;
		if (_first == 0) {
			*first = e_p;
			e->prev = e_p;
		} else {
			w_link_0 *_f = (w_link_0*) (_first + diff);
			_f->prev = e_p;
		}
	} else {
		w_link_0 *_next = (w_link_0*) (next + diff);
		_next->prev = e_p;
	}
}
void w_link_linkafter(w_link *e, w_link *succ, w_link **first) {
	w_link_linkafter_0((w_link_0*) e, e, succ, (void**) first);
}
void w_link_unlink_0(w_link_0 *x, void *x_p, void **first) {
	const wintptr diff = (wintptr) x - (wintptr) x_p;
	void *_first = *first;
	if (_first == x_p) {
		*first = x->next;
		if (x->next != 0) {
			w_link_0 *_next0 = (w_link_0*) (x->next + diff);
			_next0->prev = x->prev;
		}
		return;
	} else if (x->next == 0) {
		w_link_0 *_f = (w_link_0*) (_first + diff);
		_f->prev = x->prev;
		return;
	}
	w_link_0 *_next = (w_link_0*) (x->next + diff);
	_next->prev = x->prev;
	x->next = 0;
}
void w_link_unlink(w_link *e, w_link **first) {
	w_link_unlink_0((w_link_0*) e, e, (void**) first);
}
void w_link_replace_1(w_link_0 *newlink, void *newelement, w_link_0 *oldlink,
		void *oldelement, void **first) {
	const wintptr diff = (wintptr) newlink - (wintptr) newelement;
	newlink->next = oldlink->next;
	newlink->prev = oldlink->prev;
	if (*first == oldelement) {
		*first = newelement;
	} else {
		w_link_0 *_prev = (w_link_0*) (newlink->prev + diff);
		_prev->next = newelement;
	}
	if (newlink->next != 0) {
		w_link_0 *_next = (w_link_0*) (newlink->next + diff);
		_next->prev = newelement;
	} else {
		w_link_0 *_first = (w_link_0*) (*first + diff);
		_first->prev = newelement;
	}
}
void w_link_replace_0(w_link_0 *newlink, void *newelement, void *oldElement,
		void **first) {
	const wintptr diff = (wintptr) newlink - (wintptr) newelement;
	w_link_0 *oldlink = (w_link_0*) (oldElement + diff);
	w_link_replace_1(newlink, newelement, oldlink, oldElement, first);
}
void w_link_replace(w_link *newelement, w_link *oldElement, w_link **first) {
	w_link_replace_0((w_link_0*) newelement, newelement, oldElement,
			(void**) first);
}
