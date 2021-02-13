/*
 * Semaphore.h
 *
 *  Created on: 6 sept. 2020
 *      Author: azeddine
 */

#ifndef SWT_SWTP_RUNTIME_SEMAPHORE_H_
#define SWT_SWTP_RUNTIME_SEMAPHORE_H_
#include "Mutex.h"

class WSemaphore: public WResource {
public:
	WSemaphore() {
		w_semaphore_init((w_semaphore*) this);
		w_semaphore_create((w_semaphore*) this, 1, -1);
	}
	WSemaphore(int initialcount, int maxcount) {
		w_semaphore_init((w_semaphore*) this);
		w_semaphore_create((w_semaphore*) this, initialcount, maxcount);
	}
	~WSemaphore() {
		w_semaphore_dispose((w_semaphore*) this);
	}
	WResult Create(int initialcount, int maxcount) {
		return w_semaphore_create((w_semaphore*) this, initialcount, maxcount);
	}
	WResult Wait() {
		return w_semaphore_wait((w_semaphore*) this);
	}
	WResult TryWait() {
		return w_semaphore_try_wait((w_semaphore*) this);
	}
	WResult WaitTimeout(size_t milliseconds) {
		return w_semaphore_wait_timeout((w_semaphore*) this, milliseconds);
	}
	WResult Post() {
		return w_semaphore_post((w_semaphore*) this);
	}
private:
	void *handle[sizeof(w_semaphore) / sizeof(void*)];
};

#endif /* SWT_SWTP_RUNTIME_SEMAPHORE_H_ */
