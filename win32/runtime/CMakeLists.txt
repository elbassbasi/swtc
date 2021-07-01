	set(CURRENT_DIR win32/runtime)
	set(src_win32 
		${src_win32}
		${CURRENT_DIR}/atomic.c
		${CURRENT_DIR}/condition.c
		${CURRENT_DIR}/core.c
		${CURRENT_DIR}/file.c
		${CURRENT_DIR}/module.c
		${CURRENT_DIR}/mutex.c
		${CURRENT_DIR}/semaphore.c
		${CURRENT_DIR}/thread.c
		${CURRENT_DIR}/swt.rc
		PARENT_SCOPE
	)