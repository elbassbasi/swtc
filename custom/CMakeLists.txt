	set(src_custom ${swtc_src})
	
	add_subdirectory(themes)
	add_subdirectory(widgets)
	add_subdirectory(controls)

	set(swtc_src ${src_custom} PARENT_SCOPE)