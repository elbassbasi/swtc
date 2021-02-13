
/*
 * Name:        mutex.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include <swtc/swtc.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
typedef struct _w_mutex {
	pthread_mutex_t mutex;
	unsigned isok :1;
} _w_mutex;
#define _W_MUTEX(x) ((_w_mutex*)x)
wresult _w_mutex_result(int err) {
	switch (err) {
	case EBUSY:
		return W_ERROR_MUTEX_BUSY;
		break;
	case EPERM:
		return W_ERROR_MUTEX_UNLOCKED;
		break;
	case EDEADLK:
		return W_ERROR_MUTEX_DEAD_LOCK;
	case EINVAL:
		break;
	case ETIMEDOUT:
		return W_ERROR_MUTEX_TIMEOUT;
	case 0:
		return W_ERROR_MUTEX_NO_ERROR;
	}
	return W_ERROR_MUTEX_MISC_ERROR;
}
void w_mutex_init(w_mutex *mutex) {
	_W_MUTEX(mutex)->isok = 0;
}
wresult w_mutex_create(w_mutex *mutex, w_mutex_type type) {
	w_mutex_dispose(mutex);
	pthread_mutexattr_t _attr, *attr = NULL;
	if (type == W_MUTEX_RECURSIVE) {
		pthread_mutexattr_init(&_attr);
		pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);
		attr = &_attr;
	}
	int err = pthread_mutex_init(&_W_MUTEX(mutex)->mutex, attr);
	if (err == 0) {
		_W_MUTEX(mutex)->isok = 1;
		return W_OK;
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
void w_mutex_dispose(w_mutex *mutex) {
	if (_W_MUTEX(mutex)->isok != 0) {
		pthread_mutex_destroy(&_W_MUTEX(mutex)->mutex);
		_W_MUTEX(mutex)->isok = 0;
	}
}
wresult w_mutex_lock(w_mutex *mutex) {
	if (_W_MUTEX(mutex)->isok != 0) {
		return _w_mutex_result(pthread_mutex_lock(&_W_MUTEX(mutex)->mutex));
	} else
		return W_ERROR_NO_HANDLES;
}
#define MSEC_IN_SEC   1000
#define  NSEC_IN_MSEC  1000000
#define  NSEC_IN_USEC  1000
#define NSEC_IN_SEC   (MSEC_IN_SEC * NSEC_IN_MSEC)
wresult w_mutex_lock_timeout(w_mutex *mutex, unsigned long ms) {
	if (_W_MUTEX(mutex)->isok != 0) {
		struct timespec ts;
		time_t seconds = ms / MSEC_IN_SEC;
		long nanoseconds = (ms % MSEC_IN_SEC) * NSEC_IN_MSEC;
		ts.tv_sec = time(NULL);
		ts.tv_sec += seconds;
		ts.tv_nsec += nanoseconds;
		if (ts.tv_nsec > NSEC_IN_SEC) {
			ts.tv_sec += 1;
			ts.tv_nsec -= NSEC_IN_SEC;
		}
		return _w_mutex_result(
				pthread_mutex_timedlock(&_W_MUTEX(mutex)->mutex, &ts));
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_mutex_try_lock(w_mutex *mutex) {
	if (_W_MUTEX(mutex)->isok != 0) {
		return _w_mutex_result(pthread_mutex_trylock(&_W_MUTEX(mutex)->mutex));
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_mutex_unlock(w_mutex *mutex) {
	if (_W_MUTEX(mutex)->isok != 0) {
		return _w_mutex_result(pthread_mutex_unlock(&_W_MUTEX(mutex)->mutex));
	} else
		return W_ERROR_NO_HANDLES;
}
