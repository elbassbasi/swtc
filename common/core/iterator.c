/*
 * iterator.c
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#include <swtc.h>
void w_iterator_init(w_iterator *it) {
	it->base.clazz = &_w_iterator_class_empty;
}
wresult w_iterator_next(w_iterator *it, void *obj) {
	return it->base.clazz->next(it, obj);
}
wresult w_iterator_reset(w_iterator *it) {
	return it->base.clazz->reset(it);
}
wresult w_iterator_close(w_iterator *it) {
	wresult ret = it->base.clazz->close(it);
	it->base.clazz = &_w_iterator_class_empty;
	return ret;
}
wresult w_iterator_remove(w_iterator *it) {
	return it->base.clazz->remove(it);
}
size_t w_iterator_get_count(w_iterator *it) {
	return it->base.clazz->get_count(it);
}
wresult _w_iterator_next(w_iterator *it, void *obj) {
	return W_FALSE;
}
wresult w_iterator_not_implemented(w_iterator *it) {
	return W_FALSE;
}
size_t _w_iterator_get_count(w_iterator *it) {
	return 0;
}
_w_iterator_class _w_iterator_class_empty = { //
		w_iterator_not_implemented, //
				_w_iterator_next, //
				w_iterator_not_implemented, //
				w_iterator_not_implemented, //
				_w_iterator_get_count //
		};
/*
 *
 */
wresult _w_iterator_array_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_iterator_array_next(w_iterator *it, void *obj) {
	w_iterator_array *it_arr = (w_iterator_array*) it;
	if (it_arr->filter == 0) {
		if (it_arr->i < it_arr->length) {
			void *cursor = it_arr->array + it_arr->i * it_arr->type_size;
			memcpy(obj, cursor, it_arr->type_size);
			it_arr->i++;
			return W_TRUE;
		}
	} else {
		while (it_arr->i < it_arr->length) {
			void *cursor = it_arr->array + it_arr->i * it_arr->type_size;
			if (it_arr->filter(it_arr->user_data, cursor, obj) >= 0) {
				return W_TRUE;
			}
			it_arr->i++;
		}
	}
	return W_FALSE;
}
wresult _w_iterator_array_reset(w_iterator *it) {
	w_iterator_array *it_arr = (w_iterator_array*) it;
	it_arr->i = 0;
	return W_TRUE;
}
wresult _w_iterator_array_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_iterator_array_get_count(w_iterator *it) {
	w_iterator_array *it_arr = (w_iterator_array*) it;
	if (it_arr->count == -1) {
		size_t count = 0;
		size_t i = it_arr->i;
		it_arr->i = 0;
		if (it_arr->filter == 0) {
			while (it_arr->i < it_arr->length) {
				count++;
				it_arr->i++;
			}
		} else {
			while (it_arr->i < it_arr->length) {
				void *cursor = it_arr->array + it_arr->i * it_arr->type_size;
				if (it_arr->filter(it_arr->user_data, cursor, 0) >= 0) {
					count++;
				}
				it_arr->i++;
			}
		}
		it_arr->i = i;
		it_arr->count = count;
	}
	return it_arr->count;
}
_w_iterator_class _w_iterator_array_class = { _w_iterator_array_close, //
		_w_iterator_array_next, //
		_w_iterator_array_reset, //
		_w_iterator_array_remove, //
		_w_iterator_array_get_count };
wresult w_iterator_array_create(w_iterator *it, void *array, size_t length,
		size_t type_size, w_iterator_filter filter, void *user_data) {
	w_iterator_close(it);
	w_iterator_array *it_arr = (w_iterator_array*) it;
	it_arr->base.clazz = &_w_iterator_array_class;
	it_arr->array = array;
	it_arr->length = length;
	it_arr->type_size = type_size;
	it_arr->i = 0;
	it_arr->user_data = user_data;
	it_arr->filter = filter;
	it_arr->count = length;
	return W_TRUE;
}
wresult _w_iterator_link_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_iterator_link_next(w_iterator *it, void *obj) {
	w_iterator_link *it_link = (w_iterator_link*) it;
	w_link_0 *_current;
	if (it_link->filter == 0) {
		if (it_link->current != 0) {
			*((void**) obj) = it_link->current;
			_current = (w_link_0*) (it_link->current + it_link->diff);
			it_link->current = _current->next;
			return W_TRUE;
		}
	} else {
		while (it_link->current != 0) {
			if (it_link->filter(it_link->user_data, it_link->current, obj)
					>= 0) {
				return W_TRUE;
			}
			_current = (w_link_0*) (it_link->current + it_link->diff);
			it_link->current = _current->next;
		}
	}
	return W_FALSE;
}
wresult _w_iterator_link_reset(w_iterator *it) {
	w_iterator_link *it_link = (w_iterator_link*) it;
	it_link->current = it_link->first;
	return W_TRUE;
}
wresult _w_iterator_link_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_iterator_link_get_count(w_iterator *it) {
	w_iterator_link *it_link = (w_iterator_link*) it;
	if (it_link->count == -1) {
		wuint count = 0;
		w_link_0 *_current;
		void *current = it_link->first;
		if (it_link->filter == 0) {
			while (current != 0) {
				_current = (w_link_0*) (it_link->current + it_link->diff);
				current = _current->next;
				count++;
			}
		} else {
			while (current != 0) {
				if (it_link->filter(it_link->user_data, current, 0) >= 0) {
					count++;
				}
				_current = (w_link_0*) (it_link->current + it_link->diff);
				it_link->current = _current->next;
			}
		}
		it_link->count = count;
	}
	return it_link->count;
}
_w_iterator_class _w_iterator_link_class = { _w_iterator_link_close, //
		_w_iterator_link_next, //
		_w_iterator_link_reset, //
		_w_iterator_link_remove, //
		_w_iterator_link_get_count };
wresult w_iterator_link_create(w_iterator *it, w_link_0 *first, void *firstObj,
		wuint count, w_iterator_filter filter, void *user_data) {
	w_iterator_close(it);
	w_iterator_link *it_link = (w_iterator_link*) it;
	it_link->base.clazz = &_w_iterator_link_class;
	it_link->first = first;
	it_link->diff = (wintptr) first - (wintptr) firstObj;
	it_link->count = count;
	it_link->filter = filter;
	it_link->user_data = user_data;
	return W_TRUE;
}
