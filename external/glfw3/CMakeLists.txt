cmake_minimum_required(VERSION 3.8)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/utils.cmake)

project(libglfw3)

set(SOURCE_FILES
dummy.c
glfw3.h
glfw3native.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib)

target_include_directories(${PROJECT_NAME} PRIVATE
	$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
)

group_sources("${SOURCE_FILES}")
configure_library(${PROJECT_NAME} "external")
