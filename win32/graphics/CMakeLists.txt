	set(CURRENT_DIR win32/graphics)
	set(src_win32 
		${src_win32}
		${CURRENT_DIR}/cursor.c
		${CURRENT_DIR}/font.c
		${CURRENT_DIR}/gc.c
		${CURRENT_DIR}/image.c
		${CURRENT_DIR}/imagelist.c
		${CURRENT_DIR}/path.c
		${CURRENT_DIR}/pattern.c
		${CURRENT_DIR}/region.c
		${CURRENT_DIR}/stream.c
		${CURRENT_DIR}/textlayout.c
		${CURRENT_DIR}/transform.c
		PARENT_SCOPE
	)