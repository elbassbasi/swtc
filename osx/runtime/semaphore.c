/*
 * semaphore.c
 *
 *  Created on: 22 déc. 2020
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
typedef struct _w_semaphore {
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	unsigned isok :1;
	unsigned count :15;
	unsigned maxcount :16;
} _w_semaphore;
#define _W_SEMAPHORE(x) ((_w_semaphore*)x)
wresult w_semaphore_init(w_semaphore *semaphore) {
	_W_SEMAPHORE(semaphore)->isok = 0;
	return W_TRUE;
}
wresult w_semaphore_create(w_semaphore *semaphore, int initialcount,
		int maxcount) {
	w_semaphore_dispose(semaphore);
	if (initialcount > maxcount)
		return W_ERROR_INVALID_ARGUMENT;
	int err = pthread_mutex_init(&_W_SEMAPHORE(semaphore)->mutex, NULL);
	if (err != 0) {
		return W_ERROR_NO_HANDLES;
	}
	err = pthread_cond_init(&_W_SEMAPHORE(semaphore)->cond,
	NULL /* default attributes */);
	if (err != 0) {
		pthread_mutex_destroy(&_W_SEMAPHORE(semaphore)->mutex);
		return W_ERROR_NO_HANDLES;
	}
	_W_SEMAPHORE(semaphore)->isok = 1;
	_W_SEMAPHORE(semaphore)->count = initialcount;
	_W_SEMAPHORE(semaphore)->maxcount = maxcount;
	return W_TRUE;
}
void w_semaphore_dispose(w_semaphore *semaphore) {
	if (_W_SEMAPHORE(semaphore)->isok != 0) {
		pthread_mutex_destroy(&_W_SEMAPHORE(semaphore)->mutex);
		pthread_cond_destroy(&_W_SEMAPHORE(semaphore)->cond);
		_W_SEMAPHORE(semaphore)->isok = 0;
	}
}
wresult w_semaphore_wait(w_semaphore *semaphore) {
	int err = pthread_mutex_lock(&_W_SEMAPHORE(semaphore)->mutex);
	while ( _W_SEMAPHORE(semaphore)->count == 0) {
		err = pthread_cond_wait(&_W_SEMAPHORE(semaphore)->cond,
				&_W_SEMAPHORE(semaphore)->mutex);
		if (err != 0) {
			pthread_mutex_unlock(&_W_SEMAPHORE(semaphore)->mutex);
			return W_ERROR_MUTEX_MISC_ERROR;
		}
	}
	_W_SEMAPHORE(semaphore)->count--;
	pthread_mutex_unlock(&_W_SEMAPHORE(semaphore)->mutex);
	return W_TRUE;
}
wresult w_semaphore_try_wait(w_semaphore *semaphore) {
	int err = pthread_mutex_lock(&_W_SEMAPHORE(semaphore)->mutex);
	if ( _W_SEMAPHORE(semaphore)->count == 0) {
		return W_ERROR_MUTEX_BUSY;
	}
	_W_SEMAPHORE(semaphore)->count--;
	pthread_mutex_unlock(&_W_SEMAPHORE(semaphore)->mutex);
	return W_TRUE;
}
#define MSEC_IN_SEC   1000
#define  NSEC_IN_MSEC  1000000
#define  NSEC_IN_USEC  1000
#define NSEC_IN_SEC   (MSEC_IN_SEC * NSEC_IN_MSEC)
wresult w_semaphore_wait_timeout(w_semaphore *semaphore, wuint64 milliseconds) {
	int err = pthread_mutex_lock(&_W_SEMAPHORE(semaphore)->mutex);
	struct timespec ts;
	clock_t startTime = clock();
	clock_t endTime = startTime + milliseconds * CLOCKS_PER_SEC;
	while ( _W_SEMAPHORE(semaphore)->count == 0) {
		clock_t newtime = clock();
		if (newtime > endTime) {
			// timeout
			pthread_mutex_unlock(&_W_SEMAPHORE(semaphore)->mutex);
			return W_ERROR_SEMAPHORE_TIMEOUT;
		}
		clock_t ellapsed = endTime - newtime;
		time_t seconds = ellapsed / MSEC_IN_SEC;
		wuint64 nanoseconds = (ellapsed % MSEC_IN_SEC) * NSEC_IN_MSEC;
		ts.tv_sec = time(NULL);
		ts.tv_sec += seconds;
		ts.tv_nsec += nanoseconds;
		if (ts.tv_nsec > NSEC_IN_SEC) {
			ts.tv_sec += 1;
			ts.tv_nsec -= NSEC_IN_SEC;
		}
		err = pthread_cond_timedwait(&_W_SEMAPHORE(semaphore)->cond,
				&_W_SEMAPHORE(semaphore)->mutex, &ts);
		switch (err) {
		case ETIMEDOUT:
			pthread_mutex_unlock(&_W_SEMAPHORE(semaphore)->mutex);
			return W_ERROR_SEMAPHORE_TIMEOUT;

		default:
			pthread_mutex_unlock(&_W_SEMAPHORE(semaphore)->mutex);
			return W_ERROR_SEMAPHORE_MISC;

		case 0:
			;
		}
	}
	_W_SEMAPHORE(semaphore)->count--;
	pthread_mutex_unlock(&_W_SEMAPHORE(semaphore)->mutex);
	return W_TRUE;
}
wresult w_semaphore_post(w_semaphore *semaphore) {
	int err = pthread_mutex_lock(&_W_SEMAPHORE(semaphore)->mutex);
	if (_W_SEMAPHORE(semaphore)->maxcount > 0
			&& _W_SEMAPHORE(semaphore)->count
					== _W_SEMAPHORE(semaphore)->maxcount) {
		pthread_mutex_unlock(&_W_SEMAPHORE(semaphore)->mutex);
		return W_ERROR_SEMAPHORE_OVERFLOW;
	}
	_W_SEMAPHORE(semaphore)->count++;
	err = pthread_cond_signal(&_W_SEMAPHORE(semaphore)->cond);
	pthread_mutex_unlock(&_W_SEMAPHORE(semaphore)->mutex);
	if (err == 0)
		return W_TRUE;
	else
		return W_ERROR_SEMAPHORE_MISC;
}
