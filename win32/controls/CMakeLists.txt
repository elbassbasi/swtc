	set(CURRENT_DIR win32/controls)
	set(src_win32 
		${src_win32}
		${CURRENT_DIR}/button.c
		${CURRENT_DIR}/groupbox.c
		${CURRENT_DIR}/label.c
		${CURRENT_DIR}/listview.c
		${CURRENT_DIR}/listviewbase.c
		${CURRENT_DIR}/progressbar.c
		${CURRENT_DIR}/sash.c
		${CURRENT_DIR}/textedit.c
		${CURRENT_DIR}/treeview.c
		PARENT_SCOPE
	)