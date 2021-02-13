	set(CURRENT_DIR common/core)
	set(src_common 
		${src_common}
		${CURRENT_DIR}/alloc.c 
		${CURRENT_DIR}/iterator.c 
		${CURRENT_DIR}/utf8.c 
		${CURRENT_DIR}/value.c
		PARENT_SCOPE
	)