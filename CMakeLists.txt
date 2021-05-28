cmake_minimum_required(VERSION 3.0)

project(swtc VERSION 0.1)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

#build swtc
	set(swtc_src )
	set(swtc_inc include)
	set(swtc_lib )
	set(swtc_def SWTBUILD)
if (WIN32)
	set(swtc_lib gdiplus usp10 uxtheme oleaut32 gdi32 ole32 uuid msimg32 opengl32 Imm32 comctl32)
	add_subdirectory(win32)
	ENABLE_LANGUAGE(RC)
endif(WIN32)

if (UNIX AND NOT APPLE)
	if (POLICY CMP0072)
    	cmake_policy(SET CMP0072 NEW)
	endif()
	FIND_PACKAGE(PkgConfig REQUIRED)
    PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
    find_package(OpenGL REQUIRED)
    
	set(swtc_inc ${swtc_inc} ${GTK3_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
	set(swtc_lib ${swtc_lib} ${GTK3_LIBRARIES} ${OPENGL_LIBRARIES} pthread dl)
	set(swtc_def ${swtc_def} )
	add_subdirectory(gtk)
endif()

if (UNIX AND APPLE)
    find_package(OpenGL REQUIRED)
    find_library(COCOA_LIBRARY Cocoa)
    
	set(swtc_inc ${swtc_inc} ${OPENGL_INCLUDE_DIRS})
	set(swtc_lib ${swtc_lib} ${COCOA_LIBRARY} ${OPENGL_LIBRARIES})
	add_subdirectory(osx)
endif()
	add_subdirectory(common)

#build swtc
if(swtc)
    add_library(swtc SHARED ${swtc_src})
    target_include_directories(swtc PRIVATE ${swtc_inc})
	target_compile_definitions(swtc PRIVATE ${swtc_def})
    target_link_libraries(swtc ${swtc_lib})
endif()

#build swtp
	set(swtp_src )
	add_subdirectory(swtp)
	add_library(swtp SHARED ${swtc_src} ${src_swtp})
    target_include_directories(swtp PRIVATE ${swtc_inc})
	target_compile_definitions(swtp PRIVATE -DSWTPBUILD ${swtc_def})
	target_link_libraries(swtp ${swtc_lib})

#build tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	find_package(OpenGL REQUIRED)
	set(src_swttest )
	add_subdirectory(tests)
	
	add_executable(swttest ${src_swttest})
	target_include_directories(swttest PRIVATE include ${OPENGL_INCLUDE_DIRS})
	target_link_libraries(swttest swtp ${OPENGL_LIBRARIES})

	add_custom_target(swttest-copy-files ALL
    	COMMAND ${CMAKE_COMMAND} -E copy_directory
    	${CMAKE_CURRENT_SOURCE_DIR}/tests/images
    	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/images
    )
endif()
