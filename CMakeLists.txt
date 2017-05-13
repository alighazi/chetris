include(CMakeToolsHelpers OPTIONAL)
cmake_minimum_required(VERSION 2.8.9)
project(gl)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")


include_directories(include)

find_package(glfw3 3.2 REQUIRED)

file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/core")

add_executable(gl ${SOURCES})
target_link_libraries(gl glfw libfreeimage.so libassimp.so)