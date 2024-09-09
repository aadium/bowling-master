cmake_minimum_required(VERSION 3.19)
project(bowling_master)

set(CMAKE_CXX_STANDARD 17)

add_executable(bowling_master main.cpp)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glfw/lib/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glfw/include"
)

add_library(freeglut STATIC IMPORTED)
set_target_properties(freeglut PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/freeglut/lib/libfreeglut.dll.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/freeglut/include"
)

find_package(OpenGL REQUIRED)
target_link_libraries(bowling_master glfw freeglut OpenGL::GL)