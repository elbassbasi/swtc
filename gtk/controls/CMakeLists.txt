	set(CURRENT_DIR gtk/controls)
	set(src_gtk 
		${src_gtk}
		${CURRENT_DIR}/button.c
		${CURRENT_DIR}/label.c
		${CURRENT_DIR}/listview.c
		${CURRENT_DIR}/listviewbase.c
		${CURRENT_DIR}/progressbar.c
		${CURRENT_DIR}/sash.c
		${CURRENT_DIR}/textedit.c
		${CURRENT_DIR}/treeview.c
		PARENT_SCOPE
	)