/*
 * Link.h
 *
 *  Created on: 11 juil. 2021
 *      Author: azeddine
 */

#ifndef SWTP_CORE_LINK_H_
#define SWTP_CORE_LINK_H_
#include "Iterator.h"
template<typename T>
class WLink {
public:
	T *next;
	T *prev;
public:
	void LinkFirst(T *e, T *&first) {
		w_link_linkfirst_0((w_link_0*) this, e, (void**) &first);
	}
	void LinkLast(T *e, T *&first) {
		w_link_linklast_0((w_link_0*) this, e, (void**) &first);
	}
	void LinkBefore(T *e, T *succ, T *&first) {
		w_link_linkbefore_0((w_link_0*) this, e, succ, (void**) &first);
	}
	void LinkAfter(T *e, T *succ, T *&first) {
		w_link_linkafter_0((w_link_0*) this, e, succ, (void**) &first);
	}
	void Unlink(T *e, T *&first) {
		w_link_unlink_0((w_link_0*) this, e, (void**) &first);
	}
	void Replace(T *newelement, T *oldElement, T *&first) {
		w_link_replace_0((w_link_0*) this, newelement, oldElement,
				(void**) &first);
	}
	void Replace1(T *newelement, WLink<T> *oldlink, T *oldElement, T *&first) {
		w_link_replace_1((w_link_0*) this, newelement, (w_link_0*) oldlink,
				oldElement, (void**) &first);
	}
public:
	void LinkFirst(T *&first) {
		LinkFirst(this, &first);
	}
	void LinkLast(T *&first) {
		LinkLast(this, &first);
	}
	void LinkBefore(T *succ, T *&first) {
		LinkBefore(this, succ, &first);
	}
	void LinkAfter(T *succ, T *&first) {
		LinkAfter(this, succ, &first);
	}
	void Unlink(T *&first) {
		Unlink(this, &first);
	}
	void Replace(T *oldElement, T *&first) {
		Replace(this, oldElement, &first);
	}
	void Replace1(WLink<T> *oldlink, T *oldElement, T *&first) {
		Replace1(this, oldlink, oldElement, &first);
	}
};
template<typename T>
class WListHead {
public:
	T *first;
	size_t count;
	WListHead() {
		this->first = 0;
		this->count = 0;
	}
	size_t GetCount() {
		return count;
	}
	void Add(WLink<T> *link, T *element) {
		link->LinkLast(element, first);
		count++;
	}
	void Add(T *element) {
		Add(element, element);
	}
	void AddBefore(WLink<T> *link, T *element, T *succ) {
		link->LinkBefore(element, succ, first);
	}
	void AddBefore(T *element, T *succ) {
		AddBefore(element, element, succ);
	}
	void AddAfter(WLink<T> *link, T *element, T *succ) {
		link->LinkAfter(element, succ, first);
	}
	void AddAfter(T *element, T *succ) {
		AddAfter(element, element, succ);
	}
	void Remove(WLink<T> *link, T *element) {
		link->Unlink(element, first);
		count--;
	}
	void Remove(T *element) {
		Remove(element, element);
	}
	void Delete(WLink<T> *link, T *element) {
		Remove(link, element);
		delete element;
	}
	void Delete(T *element) {
		Delete(element, element);
	}
	void Replace(WLink<T> *newlink, T *newelement, WLink<T> *oldlink,
			T *oldElement) {
		newlink->Replace1(newelement, oldlink, oldElement, first);
	}
	void Replace(T *newelement, T *oldElement) {
		Replace(newelement, newelement, oldElement, oldElement);
	}
};
template<typename T>
class WArray {
public:
	w_array *array;
	static void array_remove_element(w_array *array, T *element, int index) {
		element->~T();
	}
	static void array_delete_element(w_array *array, T *element, int index) {
		if (*element != 0) {
			delete *element;
		}
	}
public:
	WArray() {
		this->array = 0;
	}
	~WArray() {
		this->RemoveAll();
	}
	int GetCount(T **arr) {
		return w_array_get_count(array, (void**) arr);
	}
	int GetCount(T *&arr) {
		return GetCount(&arr);
	}
	int GetCount() {
		return GetCount((T**) 0);
	}
	T* Get(int index) {
		return (T*) w_array_get(array, index, sizeof(T));
	}
	T* Set(int index) {
		return (T*) w_array_set(&array, index, sizeof(T));
	}
	int Alloc(int newalloc) {
		return w_array_alloc(&array, newalloc, sizeof(T));
	}
	T* Add(int index, int *newIndex) {
		return (T*) w_array_add(&array, index, sizeof(T), newIndex);
	}
	T* Add(int *newIndex) {
		return Add(-1, newIndex);
	}
	T* Add(int index) {
		return Add(index, 0);
	}
	T* Add() {
		return Add(-1, 0);
	}
	void Remove(int index) {
		w_array_remove(array, index, sizeof(T),
				(w_array_free_element) array_remove_element);
	}
	void RemoveAll() {
		w_array_free(array, sizeof(T),
				(w_array_free_element) array_remove_element);
		this->array = 0;
	}
	void Delete(int index) {
		w_array_remove(array, index, sizeof(T),
				(w_array_free_element) array_delete_element);
	}
	void DeleteAll(int index) {
		w_array_free(array, sizeof(T),
				(w_array_free_element) array_delete_element);
		this->array = 0;
	}
};
template<typename T>
class WNode: public WLink<T> {
public:
	T *firstElement;
	size_t count;
public:
	T* GetFirst() {
		return this->firstElement;
	}
	T* GetLast(WLink<T> *first) {
		if (this->firstElement == 0)
			return 0;
		else
			return first->prev;
	}
	T* GetLast() {
		return GetLast(static_cast<WLink<T>>(this));
	}
	void Add(WLink<T> *link, T *e) {
		link->LinkLast(e, firstElement);
		count++;
	}
	void Add(WLink<T> *link) {
		link->LinkLast(reinterpret_cast<T*>(link), firstElement);
		count++;
	}
};

#endif /* SWTP_CORE_LINK_H_ */
