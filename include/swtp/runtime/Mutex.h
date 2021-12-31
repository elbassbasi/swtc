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

class WFutex {
public:
	volatile int val;
	WFutex(){
		this->val = 0;
	}
	WResult Wait(int value) {
		return w_futex_wait((w_futex*) this, value);
	}
	WResult WaitTimeout(int value, wuint64 timeout) {
		return w_futex_wait_timeout((w_futex*) this, value, timeout);
	}
	WResult WakeAll() {
		return w_futex_wake_all((w_futex*) this);
	}
	WResult WakeSingle() {
		return w_futex_wake_single((w_futex*) this);
	}
public:
	WResult Lock() {
		return w_futex_lock((w_futex*) this);
	}
	WResult UnLock() {
		return w_futex_unlock((w_futex*) this);
	}
};
class WFutexLock {
public:
	volatile int val;
	w_threadid owner;
public:
	WFutexLock(){
		this->val =0;
		this->owner = 0;
	}
	WResult Lock() {
		return w_futexlock_lock((w_futexlock*) this);
	}
	WResult UnLock() {
		return w_futexlock_unlock((w_futexlock*) this);
	}
};

typedef WFutexLock WLock;

class WLocker {
public:
	WLock *m_lock;
	WLocker(WLock &mutex) {
		this->m_lock = &mutex;
		this->m_lock->Lock();
	}
	WLocker(WLock *mutex) {
		this->m_lock = mutex;
		this->m_lock->Lock();
	}
	~WLocker() {
		m_lock->UnLock();
	}
};

#endif /* SWTP_RUNTIME_MUTEX_H_ */
