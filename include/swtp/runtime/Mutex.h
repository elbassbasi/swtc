/*
 * Mutex.h
 *
 *  Created on: 12 nov. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_RUNTIME_MUTEX_H_
#define SWTP_RUNTIME_MUTEX_H_
#include "../core/Resource.h"
class WMutexBase: public WResource {
protected:
	WMutexBase() {
		w_mutex_init((w_mutex*) this);
	}
public:
	~WMutexBase() {
		Dispose();
	}
	bool Create(w_mutex_type type = W_MUTEX_SYSTEM_DEFAULT) {
		return w_mutex_create((w_mutex*) this, type) > 0;
	}
	void Dispose() {
		w_mutex_dispose((w_mutex*) this);
	}
	bool Lock() {
		return w_mutex_lock((w_mutex*) this) > 0;
	}
	bool LockTimeout(size_t ms) {
		return w_mutex_lock_timeout((w_mutex*) this, ms) > 0;
	}
	bool TryLock() {
		return w_mutex_try_lock((w_mutex*) this) > 0;
	}
	bool UnLock() {
		return w_mutex_unlock((w_mutex*) this) > 0;
	}
private:
	void *handle[sizeof(w_mutex) / sizeof(void*)];
};
class WMutex: public WMutexBase {
public:
	WMutex() {
		Create(W_MUTEX_SYSTEM_DEFAULT);
	}
	WMutex(int not_init) {
	}
};

class WCriticalSection: public WMutexBase {
public:
	WCriticalSection() {
		Create(W_MUTEX_DEFAULT);
	}
	WCriticalSection(int not_init) {
	}
	~WCriticalSection() {
	}
	bool Enter() {
		return Lock();
	}
	bool TryEnter() {
		return TryLock();
	}
	bool Leave() {
		return UnLock();
	}
};

typedef WCriticalSection WLock;

class WLocker {
public:
	WMutexBase *mutex;
	WLocker(WMutexBase &mutex) {
		this->mutex = &mutex;
		this->mutex->Lock();
	}
	WLocker(WMutexBase *mutex) {
		this->mutex = mutex;
		this->mutex->Lock();
	}
	~WLocker() {
		mutex->UnLock();
	}
};

#endif /* SWTP_RUNTIME_MUTEX_H_ */
