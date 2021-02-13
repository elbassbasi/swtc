	set(CURRENT_DIR osx/runtime)
	set(src_osx 
		${src_osx}
		${CURRENT_DIR}/app.c
		${CURRENT_DIR}/atomic.c
		${CURRENT_DIR}/condition.c
		${CURRENT_DIR}/file.c
		${CURRENT_DIR}/module.c
		${CURRENT_DIR}/mutex.c
		${CURRENT_DIR}/semaphore.c
		${CURRENT_DIR}/thread.c
		PARENT_SCOPE
	)