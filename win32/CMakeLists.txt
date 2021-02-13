	set(src_win32 ${swtc_src})
	
	add_subdirectory(runtime)
	add_subdirectory(graphics)
	add_subdirectory(widgets)
	add_subdirectory(controls)

	set(swtc_src ${src_win32} PARENT_SCOPE)