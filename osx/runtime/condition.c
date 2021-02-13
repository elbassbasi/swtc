/*
 * condition.c
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
typedef struct _w_condition {
	pthread_cond_t cond;
	w_mutex *mutex;
	unsigned isok :1;
} _w_condition;
#define _W_CONDITION(x) ((_w_condition*)x)
void w_condition_init(w_condition *condition) {
	_W_CONDITION(condition)->isok = 0;
}
wresult w_condition_create(w_condition *condition, w_mutex *mutex) {
	w_condition_dispose(condition);
	int err = pthread_cond_init(&_W_CONDITION(condition)->cond,
	NULL /* default attributes */);
	if (err == 0) {
		_W_CONDITION(condition)->mutex = mutex;
		return W_OK;
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
void w_condition_dispose(w_condition *condition) {
	if (_W_CONDITION(condition)->isok != 0) {
		pthread_cond_destroy(&_W_CONDITION(condition)->cond);
		_W_CONDITION(condition)->isok = 0;
	}
}
wresult w_condition_wait(w_condition *condition) {
	if (_W_CONDITION(condition)->isok != 0) {
		int err = pthread_cond_wait(&_W_CONDITION(condition)->cond,
				(pthread_mutex_t*) _W_CONDITION(condition)->mutex);
		if (err != 0) {
			return W_ERROR_MUTEX_MISC_ERROR;
		} else
			return W_OK;
	} else
		return W_ERROR_NO_HANDLES;
}
#define MSEC_IN_SEC   1000
#define  NSEC_IN_MSEC  1000000
#define  NSEC_IN_USEC  1000
#define NSEC_IN_SEC   (MSEC_IN_SEC * NSEC_IN_MSEC)
wresult w_condition_wait_timeout(w_condition *condition, wuint64 milliseconds) {
	if (_W_CONDITION(condition)->isok != 0) {
		struct timespec ts;
		time_t seconds = milliseconds / MSEC_IN_SEC;
		wuint64 nanoseconds = (milliseconds % MSEC_IN_SEC) * NSEC_IN_MSEC;
		ts.tv_sec = time(NULL);
		ts.tv_sec += seconds;
		ts.tv_nsec += nanoseconds;
		if (ts.tv_nsec > NSEC_IN_SEC) {
			ts.tv_sec += 1;
			ts.tv_nsec -= NSEC_IN_SEC;
		}
		int err = pthread_cond_timedwait(&_W_CONDITION(condition)->cond,
				(pthread_mutex_t*) _W_CONDITION(condition)->mutex, &ts);
		if (err != 0) {
			return W_ERROR_MUTEX_MISC_ERROR;
		} else
			return W_OK;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_condition_signal(w_condition *condition) {
	if (_W_CONDITION(condition)->isok != 0) {
		int err = pthread_cond_signal(&_W_CONDITION(condition)->cond);
		if (err != 0) {
			return W_ERROR_MUTEX_MISC_ERROR;
		} else
			return W_OK;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_condition_broadcast(w_condition *condition) {
	if (_W_CONDITION(condition)->isok != 0) {
		int err = pthread_cond_broadcast(&_W_CONDITION(condition)->cond);
		if (err != 0) {
			return W_ERROR_MUTEX_MISC_ERROR;
		} else
			return W_OK;
	} else
		return W_ERROR_NO_HANDLES;
}
