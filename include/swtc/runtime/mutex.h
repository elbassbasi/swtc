/*
 * Name:        mutex.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_RUNTIME_MUTEX_H_
#define SWT_RUNTIME_MUTEX_H_
#include "../core/core.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef wintptr w_threadid;
typedef struct w_mutex {
	union {
		void *handle[14];
	};
} w_mutex;
typedef enum {
	W_MUTEX_DEFAULT,
	W_MUTEX_RECURSIVE,
	W_MUTEX_SYSTEM_DEFAULT,
	W_MUTEX_SYSTEM_RECURSIVE,
} w_mutex_type;
enum {
	W_ERROR_MUTEX_NO_ERROR = 0,   // operation completed successfully
	W_ERROR_MUTEX_INVALID,        // mutex hasn't been initialized
	W_ERROR_MUTEX_DEAD_LOCK,    // mutex is already locked by the calling thread
	W_ERROR_MUTEX_BUSY,           // mutex is already locked by another thread
	W_ERROR_MUTEX_UNLOCKED,     // attempt to unlock a mutex which is not locked
	W_ERROR_MUTEX_TIMEOUT,        // LockTimeout() has timed out
	W_ERROR_MUTEX_MISC_ERROR      // any other error
};
SWT_PUBLIC void w_mutex_init(w_mutex *mutex);
SWT_PUBLIC wresult w_mutex_create(w_mutex *mutex, w_mutex_type type);
SWT_PUBLIC wresult w_mutex_isok(w_mutex *mutex);
SWT_PUBLIC void w_mutex_dispose(w_mutex *mutex);
SWT_PUBLIC wresult w_mutex_lock(w_mutex *mutex);
SWT_PUBLIC wresult w_mutex_lock_timeout(w_mutex *mutex, wuint64 ms);
SWT_PUBLIC wresult w_mutex_try_lock(w_mutex *mutex);
SWT_PUBLIC wresult w_mutex_unlock(w_mutex *mutex);

typedef struct w_futex {
	volatile int val;
} w_futex;
SWT_PUBLIC wresult w_futex_wait(w_futex *futex, int value);
SWT_PUBLIC wresult w_futex_wait_timeout(w_futex *futex, int value,
		wuint64 timeout);
SWT_PUBLIC wresult w_futex_wake_all(w_futex *futex);
SWT_PUBLIC wresult w_futex_wake_single(w_futex *futex);
/*
 * this function is not recursive
 * 0 unlocked
 * 1 locked, no waiters
 * 2 locked, one or more waiters
 */
wresult w_futex_lock(w_futex *futex);
wresult w_futex_unlock(w_futex *futex);
/*
 * recursive futex
 */
typedef struct w_futexlock {
	volatile int val;
	w_threadid owner;
} w_futexlock;
SWT_PUBLIC wresult w_futexlock_lock(w_futexlock *futex);
SWT_PUBLIC wresult w_futexlock_unlock(w_futexlock *futex);

#ifdef __cplusplus
}
#endif
#endif /* SWT_RUNTIME_MUTEX_H_ */
