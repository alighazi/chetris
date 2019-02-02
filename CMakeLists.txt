include(CMakeToolsHelpers OPTIONAL)
cmake_minimum_required(VERSION 2.8.9)
project(gl)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")


include_directories(include)

LINK_DIRECTORIES(lib)

find_package(glfw3 3.2 REQUIRED)
find_package(assimp 4 REQUIRED)

file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/core")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/util_external")

add_executable(gl ${SOURCES})
target_link_libraries(gl glfw libfreeimage.so libassimp.so.4)


project(gl_tests)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")


include_directories(include)

LINK_DIRECTORIES(lib)

find_package(glfw3 3.2 REQUIRED)
find_package(assimp 4 REQUIRED)

add_subdirectory("${PROJECT_SOURCE_DIR}/src/test")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")
message("${SOURCES}") 

add_executable(gl_tests ${SOURCES})
target_link_libraries(gl_tests glfw libfreeimage.so libassimp.so.4)