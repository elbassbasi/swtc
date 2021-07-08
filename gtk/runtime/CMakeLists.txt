	set(CURRENT_DIR gtk/runtime)
	set(src_gtk 
		${src_gtk}
		${CURRENT_DIR}/atomic.c
		${CURRENT_DIR}/condition.c
		${CURRENT_DIR}/core.c
		${CURRENT_DIR}/file.c
		${CURRENT_DIR}/module.c
		${CURRENT_DIR}/mutex.c
		${CURRENT_DIR}/semaphore.c
		${CURRENT_DIR}/thread.c
		PARENT_SCOPE
	)