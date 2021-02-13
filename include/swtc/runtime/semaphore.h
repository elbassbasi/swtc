/*
 * Name:        semaphore.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_RUNTIME_SEMAPHORE_H_
#define SWT_RUNTIME_SEMAPHORE_H_
#include "mutex.h"
#ifdef __cplusplus
extern "C" {
#endif
enum {
	W_ERROR_SEMAPHORE_NO_ERROR = 0,
	W_ERROR_SEMAPHORE_INVALID,
	W_ERROR_SEMAPHORE_DEAD_LOCK,
	W_ERROR_SEMAPHORE_BUSY,
	W_ERROR_SEMAPHORE_OVERFLOW,
	W_ERROR_SEMAPHORE_TIMEOUT,
	W_ERROR_SEMAPHORE_MISC
};

typedef struct w_semaphore {
	void *handle[0x20];
} w_semaphore;
SWT_PUBLIC wresult w_semaphore_init(w_semaphore *semaphore);
SWT_PUBLIC wresult w_semaphore_create(w_semaphore *semaphore, int initialcount,
		int maxcount);
SWT_PUBLIC void w_semaphore_dispose(w_semaphore *semaphore);
SWT_PUBLIC wresult w_semaphore_wait(w_semaphore *semaphore);
SWT_PUBLIC wresult w_semaphore_try_wait(w_semaphore *semaphore);
SWT_PUBLIC wresult w_semaphore_wait_timeout(w_semaphore *semaphore,
		wuint64 milliseconds);
SWT_PUBLIC wresult w_semaphore_post(w_semaphore *semaphore);

#ifdef __cplusplus
}
#endif
#endif /* SWT_RUNTIME_SEMAPHORE_H_ */
