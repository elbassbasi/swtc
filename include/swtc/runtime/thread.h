/*
 * Name:        thread.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_RUNTIME_THREAD_H_
#define SWT_RUNTIME_THREAD_H_
#include "condition.h"
#include "semaphore.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef int (*w_thread_start)(void *args);
typedef wintptr w_threadid;

typedef struct w_thread_cleanup {
	void (*routine)(void*);
	void *args;
	struct w_thread_cleanup *prev;
	void *reserved[2];
} w_thread_cleanup;

typedef struct w_thread {
	void *vtbl;      //used in c++
	w_thread_start start_proc;
	void *args;
	w_threadid id;
	w_thread_cleanup *cleanup;
	void *reserved[2];
} w_thread;
SWT_PUBLIC void w_thread_init(w_thread *thread);
SWT_PUBLIC wresult w_thread_equal(w_thread *thread1, w_thread *thread2);
SWT_PUBLIC wresult w_threadid_equal(w_threadid thread1, w_threadid thread2);
SWT_PUBLIC void w_thread_dispose(w_thread *thread);
SWT_PUBLIC wresult w_thread_create(w_thread *thread,size_t stackSize);
SWT_PUBLIC wresult w_thread_cancel(w_thread *thread);
SWT_PUBLIC wresult w_thread_join(w_thread *thread);
SWT_PUBLIC wresult w_thread_join_timeout(w_thread *thread, size_t ms);
SWT_PUBLIC wuint w_thread_get_priority(w_thread *thread);
SWT_PUBLIC wresult w_thread_set_priority(w_thread *thread, wuint prio);
//static function
SWT_PUBLIC w_thread* w_thread_get_current();
SWT_PUBLIC w_threadid w_thread_get_current_id();
SWT_PUBLIC wresult w_thread_is_current(w_thread *thread);
SWT_PUBLIC void w_thread_yield();
SWT_PUBLIC void w_thread_sleep(wuint64 ms);
SWT_PUBLIC size_t w_thread_get_cpu_count();
SWT_PUBLIC wresult w_thread_set_concurrency(int level);

SWT_PUBLIC void w_thread_cleanup_push(w_thread_cleanup *thread_cleanup,
		void (*routine)(void*), void *arg);
SWT_PUBLIC void w_thread_cleanup_pop(int execute);

#ifdef __cplusplus
}
#endif

#endif /* SWT_RUNTIME_THREAD_H_ */
