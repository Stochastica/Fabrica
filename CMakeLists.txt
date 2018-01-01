project(Fabrica)
cmake_minimum_required(VERSION 3.5)

set(PROJECT_SOURCE_DIR     ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_BINARY_DIR       ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/modules)

add_subdirectory(${PROJECT_SOURCE_DIR}/Fabrica)
add_subdirectory(${PROJECT_SOURCE_DIR}/Launcher)
add_subdirectory(${PROJECT_SOURCE_DIR}/modules)
