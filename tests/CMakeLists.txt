	set(src_tests ${src_swttest})
	
	add_subdirectory(main)
	add_subdirectory(widgets)
	add_subdirectory(graphics)
	add_subdirectory(controls)

	set(src_swttest ${src_tests} PARENT_SCOPE)