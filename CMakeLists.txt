project (ladybug)

set(ladybug_VERSION_MAJOR 0)
set(ladybug_VERSION_MINOR 1)

cmake_minimum_required (VERSION 2.8)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_subdirectory (src)
add_subdirectory (texture)
