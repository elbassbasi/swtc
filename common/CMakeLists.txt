	set(src_common ${swtc_src})
	
	add_subdirectory(core)
	add_subdirectory(graphics)
	add_subdirectory(layouts)
	add_subdirectory(widgets)
	add_subdirectory(controls)

	set(swtc_src ${src_common} PARENT_SCOPE)