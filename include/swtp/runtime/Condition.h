/*
 * Condition.h
 *
 *  Created on: 6 sept. 2020
 *      Author: azeddine
 */

#ifndef SWT_SWTP_RUNTIME_CONDITION_H_
#define SWT_SWTP_RUNTIME_CONDITION_H_
#include "Mutex.h"
class WCondition: public WResource {
public:
	WCondition() {
		w_condition_init((w_condition*) this);
	}
	WCondition(WMutex &mutex) {
		Create(&mutex);
	}
	~WCondition() {
		w_condition_dispose((w_condition*) this);
	}
	WResult Create(WMutex *mutex) {
		return w_condition_create((w_condition*) this, (w_mutex*) mutex);
	}
	WResult Create(WMutex &mutex) {
		return Create(&mutex);
	}
	WResult Wait() {
		return w_condition_wait((w_condition*) this);
	}
	WResult WaitTimeout(size_t milliseconds) {
		return w_condition_wait_timeout((w_condition*) this, milliseconds);
	}
	WResult Signal(size_t milliseconds) {
		return w_condition_signal((w_condition*) this);
	}
	WResult Broadcast(size_t milliseconds) {
		return w_condition_broadcast((w_condition*) this);
	}
private:
	void *handle[sizeof(w_condition) / sizeof(void*)];
};

class WConditionWithMutex: public WCondition {
public:
	WMutex mutex;
	WConditionWithMutex() {
		mutex.Create(W_MUTEX_SYSTEM_DEFAULT);
		this->Create(&mutex);
	}
};

#endif /* SWT_SWTP_RUNTIME_CONDITION_H_ */
