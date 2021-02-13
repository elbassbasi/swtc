	set(CURRENT_DIR osx/widgets)
	set(src_osx 
		${src_osx}
		${CURRENT_DIR}/canvas.c
		${CURRENT_DIR}/composite.c
		${CURRENT_DIR}/control0.c
		${CURRENT_DIR}/control1.c
		${CURRENT_DIR}/dnd0.c
		${CURRENT_DIR}/dnd1.c
		${CURRENT_DIR}/menu.c
		${CURRENT_DIR}/scrollable.c
		${CURRENT_DIR}/shell.c
		${CURRENT_DIR}/theme.c
		${CURRENT_DIR}/toolkit0.c
		${CURRENT_DIR}/toolkit1.c
		${CURRENT_DIR}/widget.c
		PARENT_SCOPE
	)