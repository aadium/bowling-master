cmake_minimum_required(VERSION 3.19)
project(cpp_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_game main.cpp)

include_directories(Dependencies/x64/include)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/x64/lib-vc2022/glfw3.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/x64/include"
)
target_link_libraries(cpp_game glfw)