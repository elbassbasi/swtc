/*
 * thread.c
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
void w_thread_init(w_thread *thread) {
	thread->id = 0;
	thread->args = 0;
	thread->start_proc = 0;
}
wresult w_threadid_equal(w_threadid thread1, w_threadid thread2) {
	return pthread_equal(thread1, thread2);
}
wresult w_thread_equal(w_thread *thread1, w_thread *thread2) {
	return pthread_equal(thread1->id, thread2->id);
}
void w_thread_dispose(w_thread *thread) {
	if (thread != 0 && thread->id != 0) {
		pthread_cancel(thread->id);
		thread->id = 0;
	}
}
static pthread_key_t _pthread_key = 0;
static void _w_cleanup_handler(void *arg) {
	w_thread *thread = (w_thread*) arg;
	w_thread_cleanup *clean = thread->cleanup;
	while (clean != 0) {
		clean->routine(clean->args);
		clean = clean->prev;
	}
}
void w_thread_cleanup_push(w_thread_cleanup *thread_cleanup,
		void (*routine)(void*), void *arg) {
	w_thread *thread = (w_thread*) pthread_getspecific(_pthread_key);
	if (thread == 0) {
		return;
	}
	thread_cleanup->args = arg;
	thread_cleanup->routine = routine;
	thread_cleanup->prev = thread->cleanup;
	thread->cleanup = thread_cleanup;
}
void w_thread_cleanup_pop(int execute) {
	w_thread *thread = (w_thread*) pthread_getspecific(_pthread_key);
	if (thread == 0) {
		return;
	}
	w_thread_cleanup *thread_cleanup = thread->cleanup;
	if (thread_cleanup == 0)
		return;
	thread->cleanup = thread_cleanup->prev;
	if (execute) {
		thread_cleanup->routine(thread_cleanup->args);
	}
}
void* _w_start_routine(void *_thread) {
	w_thread *thread = (w_thread*) _thread;
	int ret;
	pthread_setspecific(_pthread_key, thread);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	thread->cleanup = 0;
	pthread_cleanup_push(_w_cleanup_handler,thread);
		ret = thread->start_proc(thread->args);
		pthread_setspecific(_pthread_key, 0);
		pthread_cleanup_pop(0);
	return (void*) ((intptr_t) ret);
}
wresult w_thread_create(w_thread *thread,size_t stackSize) {
	w_thread_dispose(thread);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if (stackSize)
		pthread_attr_setstacksize(&attr, stackSize);
	if (_pthread_key == 0) {
		pthread_key_create(&_pthread_key, NULL);
	}
	thread->cleanup = 0;
	int rc = pthread_create((pthread_t*) &thread->id, &attr, _w_start_routine,
			thread);
	pthread_attr_destroy(&attr);
	if (rc == 0) {
		return W_ERROR_NO_HANDLES;
	} else {
		return W_TRUE;
	}
}
wresult w_thread_cancel(w_thread *thread) {
	if (thread->id != 0) {
		pthread_cancel(thread->id);
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_thread_join(w_thread *thread) {
	if (thread->id != 0) {
		pthread_join(thread->id, NULL);
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_thread_join_timeout(w_thread *thread, size_t ms) {
	if (thread->id != 0) {
		struct timespec tm;
		tm.tv_sec = ms / 1000;
		tm.tv_nsec = (ms % 1000) * 1000;
		//pthread_timedjoin_np(thread->id, NULL, &tm);
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
wuint w_thread_get_priority(w_thread *thread) {
	if (thread->id != 0) {
		return 0;
	} else
		return 0;
}
wresult w_thread_set_priority(w_thread *thread, wuint prio) {
	if (thread->id != 0) {
		return W_FALSE;
	} else
		return W_ERROR_NO_HANDLES;
}
//static function
w_thread* w_thread_get_current() {
	return (w_thread*) pthread_getspecific(_pthread_key);
}
w_threadid w_thread_get_current_id() {
	return pthread_self();
}
wresult w_thread_is_current(w_thread *thread) {
	return pthread_equal(pthread_self(), thread->id);
}
void w_thread_yield() {
	sleep(0);
}
void w_thread_sleep(wuint64 milliseconds) {
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, &ts);
}
int number_of_processors = 0;
size_t w_thread_get_cpu_count() {
	if (number_of_processors == 0)
		number_of_processors = sysconf(_SC_NPROCESSORS_ONLN);
	return number_of_processors;
}
wresult w_thread_set_concurrency(int level) {
	int rc = pthread_setconcurrency( level );
	return W_TRUE;
}
