/*
 * Name:        semaphore.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include <swtc.h>
#include <windows.h>

typedef struct _w_semaphore {
	HANDLE semaphore;
} _w_semaphore;
#define _W_SEMAPHORE(x) ((_w_semaphore*)x)
wresult w_semaphore_init(w_semaphore *semaphore) {
	_W_SEMAPHORE(semaphore)->semaphore = INVALID_HANDLE_VALUE;
	return W_TRUE;
}
wresult w_semaphore_create(w_semaphore *semaphore, int initialcount,
		int maxcount) {
	if (maxcount == 0) {
		maxcount = INT_MAX;
	}
	_W_SEMAPHORE(semaphore)->semaphore = CreateSemaphoreW(NULL, initialcount,
			maxcount, NULL);
	if (!_W_SEMAPHORE(semaphore)->semaphore) {
		return W_FALSE;
	} else
		return W_TRUE;
}
void w_semaphore_dispose(w_semaphore *semaphore) {
	if (_W_SEMAPHORE(semaphore)->semaphore != INVALID_HANDLE_VALUE) {
		CloseHandle(_W_SEMAPHORE(semaphore)->semaphore);
		_W_SEMAPHORE(semaphore)->semaphore = INVALID_HANDLE_VALUE;
	}
}
wresult w_semaphore_wait_timeout(w_semaphore *semaphore, wuint64 milliseconds) {
	if (_W_SEMAPHORE(semaphore)->semaphore != INVALID_HANDLE_VALUE) {
		DWORD rc = WaitForSingleObject(_W_SEMAPHORE(semaphore)->semaphore,
				milliseconds);
		switch (rc) {
		case WAIT_OBJECT_0:
			return W_TRUE;
		case WAIT_TIMEOUT:
			return W_ERROR_SEMAPHORE_TIMEOUT;
		}
		return W_ERROR_SEMAPHORE_MISC;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_semaphore_wait(w_semaphore *semaphore) {
	return w_semaphore_wait_timeout(semaphore, INFINITE);
}
wresult w_semaphore_try_wait(w_semaphore *semaphore) {
	wresult rc = w_semaphore_wait_timeout(semaphore, 0);
	return rc;
}
wresult w_semaphore_post(w_semaphore *semaphore) {
	if (_W_SEMAPHORE(semaphore)->semaphore != INVALID_HANDLE_VALUE) {
		if (!ReleaseSemaphore(_W_SEMAPHORE(semaphore)->semaphore, 1, NULL)) {
			if (GetLastError() == ERROR_TOO_MANY_POSTS) {
				return W_ERROR_SEMAPHORE_BUSY;
			} else {
				return W_ERROR_SEMAPHORE_MISC;
			}
			return W_FALSE;
		}
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
