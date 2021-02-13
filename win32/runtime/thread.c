/*
 * Name:        thread.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include <swtc.h>
#include <windows.h>
#include "tlhelp32.h"
void w_thread_init(w_thread *thread) {
	thread->id = 0;
	thread->args = 0;
	thread->start_proc = 0;
}
wresult w_threadid_equal(w_threadid thread1, w_threadid thread2) {
	return thread1 == thread2;
}
wresult w_thread_equal(w_thread *thread1, w_thread *thread2) {
	return thread1->id == thread2->id;
}
void w_thread_dispose(w_thread *thread) {
	if (thread != 0 && thread->id != 0) {
		w_thread_cancel(thread);
		thread->id = 0;
	}
}
DWORD dwTlsIndex = TLS_OUT_OF_INDEXES;
DWORD WINAPI w_thread_start_function(LPVOID lpThreadParameter) {
	w_thread *thread = (w_thread*) lpThreadParameter;
	TlsSetValue(dwTlsIndex, thread);
	DWORD ret = thread->start_proc(thread->args);
	TlsSetValue(dwTlsIndex, 0);
	return ret;
}
wresult w_thread_create(w_thread *thread, size_t stackSize) {
	if (dwTlsIndex == TLS_OUT_OF_INDEXES) {
		dwTlsIndex = TlsAlloc();
	}
	DWORD threadId;
	thread->reserved[0] = CreateThread(NULL, stackSize,
			(LPTHREAD_START_ROUTINE) w_thread_start_function, thread, 0,
			&threadId);
	if (thread->reserved[0] == INVALID_HANDLE_VALUE)
		return W_ERROR_NO_HANDLES;
	else {
		thread->id = threadId;
		return W_TRUE;
	}
}
void _w_thread_cleanup_all(w_thread *thread) {
	w_thread_cleanup *cleanup = thread->cleanup;
	while (cleanup != 0) {
		cleanup->routine(cleanup->prev);
		cleanup = cleanup->prev;
	}
}
void _w_thread_cancel_callback() {
	w_thread *thread = (w_thread*) TlsGetValue(dwTlsIndex);
	if (thread != 0) {
		_w_thread_cleanup_all(thread);
	}
	thread->id = 0;
	thread->reserved[0] = INVALID_HANDLE_VALUE;
	ExitThread(1);
}
DWORD _w_thread_register_cancelation(HANDLE threadH) {
	CONTEXT context;
	context.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(threadH, &context);
	context.Rip = (DWORD_PTR) _w_thread_cancel_callback;
	SetThreadContext(threadH, &context);
	return 0;
}
wresult w_thread_cancel(w_thread *thread) {
	if (GetCurrentThreadId() == thread->id)
		return W_ERROR_FAILED_EXEC;
	HANDLE threadH = (HANDLE) thread->reserved[0];
	SuspendThread(threadH);
	if (WaitForSingleObject(threadH, 0) == WAIT_TIMEOUT) {
		_w_thread_register_cancelation(threadH);
		ResumeThread(threadH);
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult w_thread_join(w_thread *thread) {
	HANDLE threadH = (HANDLE) thread->reserved[0];
	DWORD result = WaitForSingleObject(threadH, INFINITE);
	if (result == WAIT_FAILED)
		return W_FALSE;
	return W_TRUE;
}
wresult w_thread_join_timeout(w_thread *thread, wuint64 ms) {
	HANDLE threadH = (HANDLE) thread->reserved[0];
	DWORD result = WaitForSingleObject(threadH, ms);
	if (result == WAIT_FAILED)
		return W_FALSE;
	return W_TRUE;
}
wuint w_thread_get_priority(w_thread *thread) {
	return GetThreadPriority((HANDLE) thread->reserved[0]);
}
wresult w_thread_set_priority(w_thread *thread, wuint prio) {
	return SetThreadPriority((HANDLE) thread->reserved[0], prio);
}
//static function
w_thread* w_thread_get_current() {
	return (w_thread*) TlsGetValue(dwTlsIndex);
}
w_threadid w_thread_get_current_id() {
	return GetCurrentThreadId();
}
wresult w_thread_is_current(w_thread *thread) {
	return GetCurrentThreadId() == thread->id;
}
void w_thread_yield() {
	Sleep(0);
}
void w_thread_sleep(wuint64 ms) {
	Sleep(ms);
}
size_t w_thread_get_cpu_count() {
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
}
wresult w_thread_set_concurrency(int level) {
	if (level == 0)
		return 0;
	HANDLE hProcess = GetCurrentProcess();
	DWORD_PTR dwProcMask, dwSysMask;
	if (GetProcessAffinityMask(hProcess, &dwProcMask, &dwSysMask) == 0) {
		return W_FALSE;
	}
	if (dwSysMask == 1) {
		return level == 1;
	}
	DWORD bit = 1;
	while (bit) {
		if (dwSysMask & bit) {
			dwProcMask |= bit;
			if (--level == 0) {
				break;
			}
		}
		bit <<= 1;
	}
	if (level != 0) {
		return W_FALSE;
	}
	if (SetProcessAffinityMask(hProcess, dwProcMask) == 0) {
		return W_FALSE;
	}
	return W_TRUE;
}

void w_thread_cleanup_push(w_thread_cleanup *thread_cleanup,
		void (*routine)(void*), void *arg) {
	w_thread *thread = (w_thread*) TlsGetValue(dwTlsIndex);
	if (thread == 0) {
		return;
	}
	thread_cleanup->args = arg;
	thread_cleanup->routine = routine;
	thread_cleanup->prev = thread->cleanup;
	thread->cleanup = thread_cleanup;
}
void w_thread_cleanup_pop(int execute) {
	w_thread *thread = (w_thread*) TlsGetValue(dwTlsIndex);
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
