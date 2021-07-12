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
	void LinkFirst(T *e, T **first) {
		w_link_linkfirst_0((w_link*) this, e, (void**) first);
	}
	void LinkLast(T *e, T **first) {
		w_link_linklast_0((w_link*) this, e, (void**) first);
	}
	void LinkBefore(T *e, T *succ, T **first) {
		w_link_linkbefore_0((w_link*) this, e, succ, (void**) first);
	}
	void LinkAfter(T *e, T *succ, T **first) {
		w_link_linkafter_0((w_link*) this, e, succ, (void**) first);
	}
	void Unlink(T *e, T **first) {
		w_link_unlink_0((w_link*) this, e, (void**) first);
	}
	void Replace(T *x, T *newElement, T **first) {
		w_link_replace_0((w_link*) this, x, newElement, (void**) first);
	}
public:
	void LinkFirst(T *e, T *&first) {
		LinkFirst(e, &first);
	}
	void LinkLast(T *e, T *&first) {
		LinkLast(e, &first);
	}
	void LinkBefore(T *e, T *succ, T *&first) {
		LinkBefore(e, succ, &first);
	}
	void LinkAfter(T *e, T *succ, T *&first) {
		LinkAfter(e, succ, &first);
	}
	void Unlink(T *e, T *&first) {
		Unlink(e, &first);
	}
	void Replace(T *x, T *newElement, T *&first) {
		Replace(x, newElement, &first);
	}
};
template<typename T>
class WNode: protected WLink<T> {
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
