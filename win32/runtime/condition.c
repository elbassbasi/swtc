/*
 * Name:        condition.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include <swtc.h>
#include <windows.h>
typedef struct _w_condition {
	w_mutex *mutex;
	LONG numWaiters;
	HANDLE semaphore;
} _w_condition;
#define _W_CONDITION(x) ((_w_condition*)x)
void w_condition_init(w_condition *condition) {
	_W_CONDITION(condition)->semaphore = INVALID_HANDLE_VALUE;
}
wresult w_condition_create(w_condition *condition, w_mutex *mutex) {
	w_condition_dispose(condition);
	_W_CONDITION(condition)->mutex = mutex;
	_W_CONDITION(condition)->numWaiters = 0;
	_W_CONDITION(condition)->semaphore = CreateSemaphoreW(NULL, 0, INT_MAX,
	NULL);
	if (_W_CONDITION(condition)->semaphore != 0) {
		return W_TRUE;
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
void w_condition_dispose(w_condition *condition) {
	if (_W_CONDITION(condition)->semaphore != INVALID_HANDLE_VALUE) {
		CloseHandle(_W_CONDITION(condition)->semaphore);
		_W_CONDITION(condition)->semaphore = INVALID_HANDLE_VALUE;
	}
}
wresult w_condition_wait_timeout(w_condition *condition, size_t milliseconds) {
	if (_W_CONDITION(condition)->semaphore != INVALID_HANDLE_VALUE) {
		InterlockedIncrement(&_W_CONDITION(condition)->numWaiters);
		w_mutex_unlock(_W_CONDITION(condition)->mutex);
		DWORD rc = WaitForSingleObject(_W_CONDITION(condition)->semaphore,
				milliseconds);
		if (rc == WAIT_TIMEOUT) {
			DWORD rc = WaitForSingleObject(_W_CONDITION(condition)->semaphore,
					0);
			if (rc != WAIT_OBJECT_0) {
				InterlockedDecrement(&_W_CONDITION(condition)->numWaiters);
			}
		}
		w_mutex_lock(_W_CONDITION(condition)->mutex);
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_condition_wait(w_condition *condition) {
	return w_condition_wait_timeout(condition, INFINITE);
}
wresult w_condition_signal(w_condition *condition) {
	if (_W_CONDITION(condition)->semaphore != INVALID_HANDLE_VALUE) {
		if (_W_CONDITION(condition)->numWaiters > 0) {
			if (!ReleaseSemaphore(_W_CONDITION(condition)->semaphore, 1,
					&_W_CONDITION(condition)->numWaiters))
				return W_FALSE;
			InterlockedDecrement(&_W_CONDITION(condition)->numWaiters);
		}
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_condition_broadcast(w_condition *condition) {
	if (_W_CONDITION(condition)->semaphore != INVALID_HANDLE_VALUE) {
		if (_W_CONDITION(condition)->numWaiters > 0) {
			if (!ReleaseSemaphore(_W_CONDITION(condition)->semaphore,
			_W_CONDITION(condition)->numWaiters,
					&_W_CONDITION(condition)->numWaiters))
				return W_FALSE;
			_W_CONDITION(condition)->numWaiters = 0;
			InterlockedDecrement(&_W_CONDITION(condition)->numWaiters);
		}
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
