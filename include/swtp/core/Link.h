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
class WLink0 {
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
	void Replace1(T *newelement, WLink0<T> *oldlink, T *oldElement, T *&first) {
		w_link_replace_1((w_link_0*) this, newelement, (w_link_0*) oldlink,
				oldElement, (void**) &first);
	}
};
template<typename T>
class WListHead0 {
public:
	T *first;
	size_t count;
	WListHead0() {
		this->first = 0;
		this->count = 0;
	}
	size_t GetCount() {
		return count;
	}
	T* GetFirst() {
		return this->first;
	}
	T* GetLast(WLink0<T> *first) {
		if (this->first == 0)
			return 0;
		else
			return first->prev;
	}
	void AddLast(WLink0<T> *link, T *element) {
		link->LinkLast(element, first);
		count++;
	}
	void Add(WLink0<T> *link, T *element) {
		AddLast(link, element);
	}
	void AddFirst(WLink0<T> *link, T *element) {
		link->LinkFirst(element, first);
		count++;
	}
	void AddBefore(WLink0<T> *link, T *element, T *succ) {
		link->LinkBefore(element, succ, first);
		count++;
	}
	void AddAfter(WLink0<T> *link, T *element, T *succ) {
		link->LinkAfter(element, succ, first);
		count++;
	}
	void Remove(WLink0<T> *link, T *element) {
		link->Unlink(element, first);
		count--;
	}
	void Delete(WLink0<T> *link, T *element) {
		Remove(link, element);
		delete element;
	}
	void Replace(WLink0<T> *newlink, T *newelement, WLink0<T> *oldlink,
			T *oldElement) {
		newlink->Replace1(newelement, oldlink, oldElement, first);
	}
};
template<typename T>
class WLink {
public:
	T *next;
	T *prev;
public:
	void LinkFirst(T *&first) {
		w_link_linkfirst((w_link*) this, (w_link**) &first);
	}
	void LinkLast(T *&first) {
		w_link_linklast((w_link*) this, (w_link**) &first);
	}
	void LinkBefore(T *succ, T *&first) {
		w_link_linkbefore((w_link*) this, (w_link*)succ, (w_link**) &first);
	}
	void LinkAfter(T *succ, T *&first) {
		w_link_linkafter((w_link*) this, (w_link*)succ, (w_link**) &first);
	}
	void Unlink(T *&first) {
		w_link_unlink((w_link*) this, (w_link**) &first);
	}
	void Replace(T *oldElement, T *&first) {
		w_link_replace((w_link*) this, (w_link*)oldElement, (w_link**) &first);
	}
	void Replace1(T *newelement, WLink<T> *oldlink, T *oldElement, T *&first) {
		w_link_replace_1((w_link_0*) this, newelement, (w_link_0*) oldlink,
				oldElement, (void**) &first);
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
	~WListHead() {
		DeleteAll();
	}
	size_t GetCount() {
		return count;
	}
	T* GetFirst() {
		return this->first;
	}
	T* GetLast() {
		if (this->first == 0)
			return 0;
		else
			return first->prev;
	}
	T* Get(int index) {
		if (index < (this->count >> 1)) {
			T *x = first;
			for (int i = 0; i < index; i++)
				x = x->next;
			return x;
		} else {
			T *x = first->prev;
			for (int i = this->count - 1; i > index; i--)
				x = x->prev;
			return x;
		}
	}
	void Clear() {
		for (T *x = first; x != 0;) {
			T *next = x->next;
			x->next = 0;
			x->prev = 0;
			x = next;
		}
		this->first = 0;
		this->count = 0;
	}
	void DeleteAll() {
		for (T *x = first; x != 0;) {
			T *next = x->next;
			delete x;
			x = next;
		}
		this->first = 0;
		this->count = 0;
	}
	void AddLast(T *element) {
		element->LinkLast(first);
		count++;
	}
	void Add(T *element) {
		AddLast(element);
	}
	void Add(int index, T *element) {
		T *before = Get(index);
		AddBefore(element, before);
	}
	void AddFirst(T *element) {
		element->LinkFirst(first);
		count++;
	}
	void AddBefore(T *element, T *succ) {
		element->LinkBefore(succ, first);
		count++;
	}
	void AddAfter(T *element, T *succ) {
		element->LinkAfter(succ, first);
		count++;
	}
	void Remove(T *element) {
		element->Unlink(first);
		count--;
	}
	void Delete(T *element) {
		Remove(element);
		delete element;
	}
	void Replace(WLink<T> *newlink, T *newelement, WLink<T> *oldlink,
			T *oldElement) {
		newlink->Replace1(newelement, oldlink, oldElement, first);
	}
	void Replace(T *newelement, T *oldElement) {
		newelement->Replace(oldElement, first);
	}
	size_t ToArray(T **array, size_t size) {
		int i = 0;
		size_t retSize = this->count > size ? size : this->count;
		for (T *x = first; retSize; x = x->next)
			array[i++] = x;
		return retSize;
	}
};
template<typename T>
class WLinkedList {
public:
	class WLinkData: protected WLink<WLinkData> {
	public:
		friend class WLinkedList;
		WLinkData(const T &o) {
			this->data = o;
		}
		T data;
	};
private:
	WLinkData *first;
	size_t count;
public:
	WLinkedList() {
		this->first = 0;
		this->count = 0;
	}
	~WLinkedList() {
		DeleteAll();
	}
	size_t GetCount() {
		return count;
	}
	T& GetFirst() {
		return this->first->data;
	}
	T& GetLast() {
		if (this->first == 0)
			return 0;
		else
			return first->prev->data;
	}
	T& Get(int index) {
		return GetLink(index)->data;
	}
	void Set(int index, const T &o) {
		GetLink(index)->data = o;
	}
	WLinkData* GetLink(int index) {
		if (index < (this->count >> 1)) {
			WLinkData *x = first;
			for (int i = 0; i < index; i++)
				x = x->next;
			return x;
		} else {
			WLinkData *x = first->prev;
			for (int i = this->count - 1; i > index; i--)
				x = x->prev;
			return x;
		}
	}
	void Clear() {
		for (WLinkData *x = first; x != 0;) {
			WLinkData *next = x->next;
			x->next = 0;
			x->prev = 0;
			x = next;
		}
		this->first = 0;
		this->count = 0;
	}
	void DeleteAll() {
		for (WLinkData *x = first; x != 0;) {
			WLinkData *next = x->next;
			delete x;
			x = next;
		}
		this->first = 0;
		this->count = 0;
	}
	void AddLast(WLinkData *link) {
		link->LinkLast(first);
		count++;
	}
	void AddLast(const T &o) {
		WLinkData *link = new WLinkData(o);
		link->LinkLast(first);
		count++;
	}
	void Add(WLinkData *link) {
		AddLast(link);
	}
	void Add(const T &o) {
		AddLast(o);
	}
	void Add(int index, WLinkData *link) {
		T *before = Get(index);
		AddBefore(link, before);
	}
	void Add(int index, const T &o) {
		WLinkData *link = new WLinkData(o);
		Add(index, link);
	}
	void AddFirst(WLinkData *link) {
		link->LinkFirst(first);
		count++;
	}
	void AddFirst(const T &o) {
		WLinkData *link = new WLinkData(o);
		AddFirst(link);
	}
	void AddBefore(WLinkData *link, WLinkData *succ) {
		link->LinkBefore(succ, first);
		count++;
	}
	void AddBefore(const T &o, WLinkData *succ) {
		WLinkData *link = new WLinkData(o);
		AddBefore(link, succ);
	}
	void AddAfter(WLinkData *link, WLinkData *succ) {
		link->LinkAfter(succ, first);
		count++;
	}
	void AddAfter(const T &o, WLinkData *succ) {
		WLinkData *link = new WLinkData(o);
		AddAfter(link, succ);
	}
	WLinkData* Next(WLinkData *link) {
		return link->next;
	}
	WLinkData* Prev(WLinkData *link) {
		if (link == this->first)
			return 0;
		else
			return link->prev;
	}
	void Remove(WLinkData *link) {
		link->Unlink(first);
		count--;
	}
	void Remove(int index) {
		WLinkData *link = GetLink(index);
		Remove(link);
	}
	size_t ToArray(T *array, size_t size) {
		int i = 0;
		size_t retSize = this->count > size ? size : this->count;
		for (T *x = first; retSize; x = x->next)
			array[i++] = x->data;
		return retSize;
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
