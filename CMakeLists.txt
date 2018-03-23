cmake_minimum_required (VERSION 3.10)
project (Ladybug)

set(ladybug_VERSION_MAJOR 0)
set(ladybug_VERSION_MINOR 2)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARIES})

add_subdirectory (src)
add_subdirectory (textures)
