/*
 * Thread.h
 *
 *  Created on: 25 déc. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_RUNTIME_THREAD_H_
#define SWTP_RUNTIME_THREAD_H_
#include "Mutex.h"
class SWTP_PUBLIC IWRunnable {
public:
	virtual ~IWRunnable();
	virtual void Run(void *args)=0;
};
#if __cplusplus >= 201103L
typedef std::function<int(void *args)> WRunnable;
#endif
typedef w_threadid WThreadID;
class WThreadClean;
/**
 * A <i>thread</i> is a thread of execution in a program. The Java
 * Virtual Machine allows an application to have multiple threads of
 * execution running concurrently.
 * <p>
 * Every thread has a priority. Threads with higher priority are
 * executed in preference to threads with lower priority. Each thread
 * may or may not also be marked as a daemon. When code running in
 * some thread creates a new <code>Thread</code> object, the new
 * thread has its priority initially set equal to the priority of the
 * creating thread, and is a daemon thread if and only if the
 * creating thread is a daemon.
 * <p>
 * When a Java Virtual Machine starts up, there is usually a single
 * non-daemon thread (which typically calls the method named
 * <code>main</code> of some designated class). The Java Virtual
 * Machine continues to execute threads until either of the following
 * occurs:
 * <ul>
 * <li>The <code>exit</code> method of class <code>Runtime</code> has been
 *     called and the security manager has permitted the exit operation
 *     to take place.
 * <li>All threads that are not daemon threads have died, either by
 *     returning from the call to the <code>run</code> method or by
 *     throwing an exception that propagates beyond the <code>run</code>
 *     method.
 * </ul>
 * <p>
 * There are two ways to create a new thread of execution. One is to
 * declare a class to be a subclass of <code>Thread</code>. This
 * subclass should override the <code>run</code> method of class
 * <code>Thread</code>. An instance of the subclass can then be
 * allocated and started. For example, a thread that computes primes
 * larger than a stated value could be written as follows:
 * <p><hr><blockquote><pre>
 *     class PrimeThread extends Thread {
 *         long minPrime;
 *         PrimeThread(long minPrime) {
 *             this.minPrime = minPrime;
 *         }
 *
 *         public void run() {
 *             // compute primes larger than minPrime
 *             &nbsp;.&nbsp;.&nbsp;.
 *         }
 *     }
 * </pre></blockquote><hr>
 * <p>
 * The following code would then create a thread and start it running:
 * <p><blockquote><pre>
 *     PrimeThread p = new PrimeThread(143);
 *     p.start();
 * </pre></blockquote>
 * <p>
 * The other way to create a thread is to declare a class that
 * implements the <code>Runnable</code> interface. That class then
 * implements the <code>run</code> method. An instance of the class can
 * then be allocated, passed as an argument when creating
 * <code>Thread</code>, and started. The same example in this other
 * style looks like the following:
 * <p><hr><blockquote><pre>
 *     class PrimeRun implements Runnable {
 *         long minPrime;
 *         PrimeRun(long minPrime) {
 *             this.minPrime = minPrime;
 *         }
 *
 *         public void run() {
 *             // compute primes larger than minPrime
 *             &nbsp;.&nbsp;.&nbsp;.
 *         }
 *     }
 * </pre></blockquote><hr>
 * <p>
 * The following code would then create a thread and start it running:
 * <p><blockquote><pre>
 *     PrimeRun p = new PrimeRun(143);
 *     new Thread(p).start();
 * </pre></blockquote>
 * <p>
 * Every thread has a name for identification purposes. More than
 * one thread may have the same name. If a name is not specified when
 * a thread is created, a new name is generated for it.
 * <p>
 * Unless otherwise noted, passing a {@code null} argument to a constructor
 * or method in this class will cause a {@link NullPointerException} to be
 * thrown.
 */
class SWTP_PUBLIC WThread: public IWRunnable {
public:
	enum State {
		/**
		 * Thread state for a thread which has not yet started.
		 */
		NEW,

		/**
		 * Thread state for a runnable thread.  A thread in the runnable
		 * state is executing in the Java virtual machine but it may
		 * be waiting for other resources from the operating system
		 * such as processor.
		 */
		RUNNABLE,

		/**
		 * Thread state for a thread blocked waiting for a monitor lock.
		 * A thread in the blocked state is waiting for a monitor lock
		 * to enter a synchronized block/method or
		 * reenter a synchronized block/method after calling
		 * {@link Object#wait() Object.wait}.
		 */
		BLOCKED,

		/**
		 * Thread state for a waiting thread.
		 * A thread is in the waiting state due to calling one of the
		 * following methods:
		 * <ul>
		 *   <li>{@link Object#wait() Object.wait} with no timeout</li>
		 *   <li>{@link #join() Thread.join} with no timeout</li>
		 *   <li>{@link LockSupport#park() LockSupport.park}</li>
		 * </ul>
		 *
		 * <p>A thread in the waiting state is waiting for another thread to
		 * perform a particular action.
		 *
		 * For example, a thread that has called <tt>Object.wait()</tt>
		 * on an object is waiting for another thread to call
		 * <tt>Object.notify()</tt> or <tt>Object.notifyAll()</tt> on
		 * that object. A thread that has called <tt>Thread.join()</tt>
		 * is waiting for a specified thread to terminate.
		 */
		WAITING,

		/**
		 * Thread state for a waiting thread with a specified waiting time.
		 * A thread is in the timed waiting state due to calling one of
		 * the following methods with a specified positive waiting time:
		 * <ul>
		 *   <li>{@link #sleep Thread.sleep}</li>
		 *   <li>{@link Object#wait(long) Object.wait} with timeout</li>
		 *   <li>{@link #join(long) Thread.join} with timeout</li>
		 *   <li>{@link LockSupport#parkNanos LockSupport.parkNanos}</li>
		 *   <li>{@link LockSupport#parkUntil LockSupport.parkUntil}</li>
		 * </ul>
		 */
		TIMED_WAITING,

		/**
		 * Thread state for a terminated thread.
		 * The thread has completed execution.
		 */
		TERMINATED,
	};
	enum {
		/**
		 * The minimum priority that a thread can have.
		 */
		MIN_PRIORITY = 1,

		/**
		 * The default priority that is assigned to a thread.
		 */
		NORM_PRIORITY = 5,

		/**
		 * The maximum priority that a thread can have.
		 */
		MAX_PRIORITY = 10,
	};
	WThread();
	~WThread();
	bool operator ==(const WThread &thread) {
		return w_thread_equal((w_thread*) this, (w_thread*) &thread);
	}
	bool operator !=(const WThread &thread) {
		return !w_thread_equal((w_thread*) this, (w_thread*) &thread);
	}
protected:
	void Init() {
		w_thread_init((w_thread*) this);
	}
	void Dispose() {
		w_thread_dispose((w_thread*) this);
	}
public:
	bool Create(w_thread_start function, void *userdata, void *args,
			size_t stackSize) {
		this->start_proc = function;
		this->args = args;
		this->user_data = userdata;
		return w_thread_create((w_thread*) this, stackSize) > 0;
	}
	bool Create(IWRunnable *runnable, void *args, size_t stackSize) {
		return Create(WThread::w_thread_runnable_start, runnable, args,
				stackSize);
	}
	bool Create(IWRunnable *runnable, void *args) {
		return Create(runnable, args, 0);
	}
#if __cplusplus >= 201103L
	bool Create(WRunnable &run, void *args);
	void operator <<=(WRunnable &run) {
		Create(run, 0);
	}
#endif
	bool Cancel() {
		return w_thread_cancel((w_thread*) this) > 0;
	}

	/**
	 * Returns this thread's priority.
	 *
	 * @return  this thread's priority.
	 * @see     #setPriority
	 */
	wuint GetPriority() {
		return w_thread_get_priority((w_thread*) this);
	}
	WThreadID GetID() {
		return this->id;
	}
	/**
	 * Changes the priority of this thread.
	 * <p>
	 * First the <code>checkAccess</code> method of this thread is called
	 * with no arguments. This may result in throwing a
	 * <code>SecurityException</code>.
	 * <p>
	 * Otherwise, the priority of this thread is set to the smaller of
	 * the specified <code>newPriority</code> and the maximum permitted
	 * priority of the thread's thread group.
	 *
	 * @param newPriority priority to set this thread to
	 */
	bool SetPriority(wuint prio) {
		return w_thread_set_priority((w_thread*) this, prio) > 0;
	}
	bool IsCurrent() {
		return w_thread_is_current((w_thread*) this);
	}
	/**
	 * Waits for this thread to die.
	 *
	 * <p> An invocation of this method behaves in exactly the same
	 * way as the invocation
	 *
	 * <blockquote>
	 * {@linkplain #join(long) join}{@code (0)}
	 * </blockquote>
	 */
	bool Join() {
		return w_thread_join((w_thread*) this) > 0;
	}
	/**
	 * Waits at most {@code millis} milliseconds for this thread to
	 * die. A timeout of {@code 0} means to wait forever.
	 *
	 * <p> This implementation uses a loop of {@code this.wait} calls
	 * conditioned on {@code this.isAlive}. As a thread terminates the
	 * {@code this.notifyAll} method is invoked. It is recommended that
	 * applications not use {@code wait}, {@code notify}, or
	 * {@code notifyAll} on {@code Thread} instances.
	 *
	 * @param  millis
	 *         the time to wait in milliseconds
	 */
	bool Join(size_t millis) {
		return w_thread_join_timeout((w_thread*) this, millis) > 0;
	}

	/**
	 * Causes this thread to begin execution; the Java Virtual Machine
	 * calls the <code>run</code> method of this thread.
	 * <p>
	 * The result is that two threads are running concurrently: the
	 * current thread (which returns from the call to the
	 * <code>start</code> method) and the other thread (which executes its
	 * <code>run</code> method).
	 * <p>
	 * It is never legal to start a thread more than once.
	 * In particular, a thread may not be restarted once it has completed
	 * execution.
	 */
	void Start() {

	}
	void Run(void *args);
	/**
	 * Causes the currently executing thread to sleep (temporarily cease
	 * execution) for the specified number of milliseconds, subject to
	 * the precision and accuracy of system timers and schedulers. The thread
	 * does not lose ownership of any monitors.
	 *
	 * @param  millis
	 *         the length of time to sleep in milliseconds
	 */
	static void Sleep(wint64 milliseconds) {
		w_thread_sleep(milliseconds);
	}
	/**
	 * Returns a reference to the currently executing thread object.
	 *
	 * @return  the currently executing thread.
	 */
	static WThread* GetCurrent() {
		return (WThread*) w_thread_get_current();
	}
	static WThreadID GetCurrentID() {
		return w_thread_get_current_id();
	}
	static int GetCPUCount() {
		return w_thread_get_cpu_count();
	}

	/**
	 * A hint to the scheduler that the current thread is willing to yield
	 * its current use of a processor. The scheduler is free to ignore this
	 * hint.
	 *
	 * <p> Yield is a heuristic attempt to improve relative progression
	 * between threads that would otherwise over-utilise a CPU. Its use
	 * should be combined with detailed profiling and benchmarking to
	 * ensure that it actually has the desired effect.
	 *
	 * <p> It is rarely appropriate to use this method. It may be useful
	 * for debugging or testing purposes, where it may help to reproduce
	 * bugs due to race conditions. It may also be useful when designing
	 * concurrency control constructs such as the ones in the
	 * {@link java.util.concurrent.locks} package.
	 */
	static void Yield() {
		w_thread_yield();
	}
	static int w_thread_runnable_start(void *userdata, void *args);
private:
	w_thread_start start_proc;
	void *user_data;
	void *args;
	WThreadID id;
	WThreadClean *cleanup;
	void *reserved[2];
};
template<typename T, int n>
class SWTP_PUBLIC WThreadM: public WThread {
public:
	using WThread::Create;
	T mem[n];
	WThreadM() {

	}
	bool Create(IWRunnable *runnable, size_t stackSize) {
		return WThread::Create(runnable, mem, stackSize);
	}
	bool Create(IWRunnable *runnable) {
		return WThread::Create(runnable, mem);
	}
};
class SWT_PUBLIC WThreadClean {
private:
	void Init() {

	}
public:
	WThreadClean() {
		Init();
	}
	WThreadClean(void (*routine)(void*), void *arg) {
		Init();
		w_thread_cleanup_push((w_thread_cleanup*) this, routine, arg);
	}
	~WThreadClean() {
		Pop();
	}
	void Push(void (*routine)(void*), void *arg) {
		w_thread_cleanup_push((w_thread_cleanup*) this, routine, arg);
	}
	static void Pop(int execute) {
		w_thread_cleanup_pop(execute);
	}
	static void Pop() {
		Pop(0);
	}
private:
	void (*routine)(void*);
	void *args;
	struct w_thread_cleanup *prev;
	void *reserved[2];
};

#endif /* SWTP_RUNTIME_THREAD_H_ */
