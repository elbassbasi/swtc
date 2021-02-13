	set(src_osx ${swtc_src})
	
	add_subdirectory(objc)
	add_subdirectory(runtime)
	add_subdirectory(graphics)
	add_subdirectory(widgets)
	add_subdirectory(controls)

	set(swtc_src ${src_osx} PARENT_SCOPE)