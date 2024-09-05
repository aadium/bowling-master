cmake_minimum_required(VERSION 3.19)
project(cpp_game)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_game main.cpp)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glfw/lib-vc2022/glfw3.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glfw/include"
)

find_package(OpenGL REQUIRED)
target_link_libraries(cpp_game glfw OpenGL::GL)