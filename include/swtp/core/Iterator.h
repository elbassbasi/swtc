/*
 * Iterator.h
 *
 *  Created on: 24 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CORE_ITERATOR_H_
#define SWTP_CORE_ITERATOR_H_
#include "Resource.h"
class SWTP_PUBLIC IDestruct {
public:
	virtual ~IDestruct();
};
class SWTP_PUBLIC IWIterator: public IDestruct {
private:
	_w_iterator_class *clazz;
public:
	IWIterator();
	~IWIterator();
	virtual bool Next(void *obj)=0;
	virtual WResult Reset();
	virtual WResult Remove();
	virtual size_t GetCount();
};
template<typename T>
class WBasicIterator: public IWIterator {
public:
	bool Next(void *obj) {
		return Next(*reinterpret_cast<T*>(obj));
	}
	virtual bool Next(T &t) {
		return false;
	}
};
template<typename T>
class WIterator {
private:
	w_iterator iterator;
private:
	WIterator(const WIterator<T> &it) {
	}
	void operator=(const WIterator<T> &it) {
	}
public:
	static bool CreateFromArray(WIterator<T> &it, T *array, size_t length,
			w_iterator_filter filter, void *user_data) {
		return w_iterator_array_create(&it.iterator, array, length, sizeof(T),
				filter, user_data);
	}
	static bool CreateFromArray(WIterator<T> &it, T *array, size_t length) {
		return CreateFromArray(it, array, length, 0, 0);
	}
	WIterator() {
		w_iterator_init(&iterator);
	}
	~WIterator() {
		w_iterator_close(&iterator);
	}
	bool Next(T &t) {
		return w_iterator_next(&iterator, (void*) &t);
	}
	bool Close() {
		return w_iterator_close(&iterator) > 0;
	}
	bool Reset() {
		return w_iterator_reset(&iterator) > 0;
	}
	bool Remove() {
		return w_iterator_remove(&iterator) > 0;
	}
	size_t GetCount() {
		return w_iterator_get_count(&iterator);
	}
	size_t ToArray(T *array, size_t length) {
		size_t i = 0;
		while (i < length && Next(array[i]))
			i++;
		return i;
	}
	size_t ToArray(T *&array) {
		size_t length = GetCount();
		array = new T[length];
		return ToArray(array, length);
	}
};

namespace swt {
template<typename T>
class Iterator: public ::WIterator<T> {
private:
	Iterator(const Iterator<T> &it) {
	}
	void operator=(const WIterator<T> &it) {
	}
public:
};
}  // namespace swt

#endif /* SWTP_CORE_ITERATOR_H_ */
