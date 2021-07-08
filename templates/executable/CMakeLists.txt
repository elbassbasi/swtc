cmake_minimum_required (VERSION 2.6)

# set the project name
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 OLD)
endif (POLICY CMP0048)
project(MyProject CXX)
set(LIB_MAJOR_VERSION "1")
set(LIB_MINOR_VERSION "1")
set(LIB_PATCH_VERSION "0")
set(LIB_VERSION_STRING "${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_PATCH_VERSION}")
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_subdirectory(swtc)

file(GLOB srcs "*.cpp")

add_executable(MyProject ${srcs})
target_include_directories(MyProject PRIVATE ./swtc)
target_link_libraries(MyProject PRIVATE swtp)
