cmake_minimum_required(VERSION 3.0)
project(archery_game)

set(SOIL_INCLUDE_DIR /usr/include/SOIL/)
set(SOIL_LIBRARY /usr/lib/libSOIL.so)

set(FREEGLUT_INCLUDE_DIR /usr/include/GL/)
set(FREEGLUT_LIBRARY /usr/lib/libglut.so)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${FREEGLUT_INCLUDE_DIRS} ${SOIL_INCLUDE_DIR})


set (SOURCE_DIR src)
set (SOURCE_FILES 
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/arrow.cpp
    ${SOURCE_DIR}/target.cpp
    ${SOURCE_DIR}/bow.cpp
    )
add_executable(archery_game ${SOURCE_FILES})

target_link_libraries(archery_game ${OPENGL_LIBRARIES} ${FREEGLUT_LIBRARY} ${SOIL_LIBRARY})
