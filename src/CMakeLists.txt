cmake_minimum_required (VERSION 2.8)
project (Cocci CXX)

set (Cocci_VERSION_MAJOR 0)
set (Cocci_VERSION_MINOR 1)

set (Cocci_LIBRARIES model view glut)
include_directories (.)

set (SRC main)
add_subdirectory (model)
add_subdirectory (view)

add_executable (cocci ${SRC})
target_link_libraries (cocci ${Cocci_LIBRARIES})
