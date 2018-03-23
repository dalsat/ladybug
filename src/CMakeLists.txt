
set (LIBS model view glut)
include_directories (".")

set (SRC main)
add_subdirectory (model)
add_subdirectory (view)

add_executable (ladybug ${SRC})
target_link_libraries (ladybug ${LIBS})
