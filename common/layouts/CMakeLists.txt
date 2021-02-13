	set(CURRENT_DIR common/layouts)
	set(src_common 
		${src_common}
		${CURRENT_DIR}/fill.c 
		${CURRENT_DIR}/form.c
		${CURRENT_DIR}/grid.c
		${CURRENT_DIR}/layout.c
		${CURRENT_DIR}/row.c
		${CURRENT_DIR}/sash.c
		PARENT_SCOPE
	)