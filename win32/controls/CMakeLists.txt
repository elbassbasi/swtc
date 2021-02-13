	set(CURRENT_DIR win32/controls)
	set(src_win32 
		${src_win32}
		${CURRENT_DIR}/listview.c
		${CURRENT_DIR}/listviewbase.c
		${CURRENT_DIR}/treeview.c
		PARENT_SCOPE
	)